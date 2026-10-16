#include "global.h"
#include "brisk_modes.h"
#include "brisk_trainer_moves.h"
#include "item.h"
#include "move.h"
#include "pokemon.h"
#include "constants/battle_move_effects.h"
#include "constants/battle_set_effect.h"
#include "constants/hold_effects.h"

#define MOVE_POOL_WORDS ((MOVES_COUNT + 31) / 32)

struct MoveContext
{
    enum Ability ability;
    enum HoldEffect item;
    u16 attack, special, defense, speed;
    u8 level, friendship;
    enum Type types[2];
};

static void InitMoveContext(struct MoveContext *ctx, struct Pokemon *mon)
{
    enum Species species = GetMonData(mon, MON_DATA_SPECIES);
    ctx->speed = GetSpeciesBaseSpeed(species);
    ctx->ability = GetMonAbility(mon);
    ctx->item = GetItemHoldEffect(GetMonData(mon, MON_DATA_HELD_ITEM));
    u32 nature = GetNature(mon);
    ctx->attack = ModifyStatByNature(nature, GetSpeciesBaseAttack(species), STAT_ATK);
    ctx->special = ModifyStatByNature(nature, GetSpeciesBaseSpAttack(species), STAT_SPATK);
    ctx->defense = ModifyStatByNature(nature, GetSpeciesBaseDefense(species), STAT_DEF);
    ctx->level = GetMonData(mon, MON_DATA_LEVEL);
    ctx->friendship = GetMonData(mon, MON_DATA_FRIENDSHIP);
    ctx->types[0] = GetSpeciesType(species, 0);
    ctx->types[1] = GetSpeciesType(species, 1);
}

static void AddPoolMove(u32 *pool, enum Move move)
{
    if (move > MOVE_NONE && move < MOVES_COUNT && move != MOVE_STRUGGLE && GetMoveEffect(move) != EFFECT_PLACEHOLDER)
        pool[move / 32] |= 1u << (move % 32);
}

static bool32 InMovePool(const u32 *pool, enum Move move)
{
    return move > MOVE_NONE && move < MOVES_COUNT && (pool[move / 32] & (1u << (move % 32))) != 0;
}

static void BuildMovePool(u32 *pool, enum Species species, u32 level)
{
    memset(pool, 0, MOVE_POOL_WORDS * sizeof(*pool));
    const u16 *teachable = GetSpeciesTeachableLearnset(species);
    for (u32 i = 0; teachable[i] != MOVE_UNAVAILABLE; i++)
        AddPoolMove(pool, teachable[i]);
    // Level-up and inherited egg moves are legal from pre-evolutions as well.
    for (u32 depth = 0; species != SPECIES_NONE && depth < 10; depth++)
    {
        const struct LevelUpMove *levels = GetSpeciesLevelUpLearnset(species);
        for (u32 i = 0; levels[i].move != LEVEL_UP_MOVE_END; i++)
            if (levels[i].level <= level)
                AddPoolMove(pool, levels[i].move);
        const u16 *eggs = GetSpeciesEggMoves(species);
        for (u32 i = 0; eggs[i] != MOVE_UNAVAILABLE; i++)
            AddPoolMove(pool, eggs[i]);
        species = GetSpeciesPreEvolution(species);
    }
}

static enum Type ContextMoveType(const struct MoveContext *ctx, enum Move move)
{
    enum Type type = GetMoveType(move);
    if (ctx->ability == ABILITY_NORMALIZE) return TYPE_NORMAL;
    if (type == TYPE_NORMAL)
    {
        if (ctx->ability == ABILITY_PIXILATE) return TYPE_FAIRY;
        if (ctx->ability == ABILITY_AERILATE) return TYPE_FLYING;
        if (ctx->ability == ABILITY_REFRIGERATE) return TYPE_ICE;
        if (ctx->ability == ABILITY_GALVANIZE) return TYPE_ELECTRIC;
    }
    return type;
}

static s32 SecondaryScore(const struct MoveContext *ctx, const struct AdditionalEffect *effect)
{
    s32 value = 0;
    switch (effect->moveEffect)
    {
    case MOVE_EFFECT_RECHARGE: return -45;
    case MOVE_EFFECT_RECOIL_HP_25: return -15;
    case MOVE_EFFECT_THRASH: return -12;
    case MOVE_EFFECT_GLAIVE_RUSH: return -15;
    case MOVE_EFFECT_SLEEP: value = 45; break;
    case MOVE_EFFECT_FREEZE: value = 40; break;
    case MOVE_EFFECT_PARALYSIS: case MOVE_EFFECT_BURN: case MOVE_EFFECT_TOXIC: value = 30; break;
    case MOVE_EFFECT_POISON: case MOVE_EFFECT_CONFUSION: case MOVE_EFFECT_FROSTBITE: value = 18; break;
    case MOVE_EFFECT_FLINCH: value = ctx->speed >= 80 ? 25 : 8; break;
    case MOVE_EFFECT_ABSORB: value = effect->argument.absorbPercentage / 2; break;
    case MOVE_EFFECT_STAT_PLUS: case MOVE_EFFECT_STAT_MINUS:
        value = 12 * (effect->attack + effect->defense + effect->spAtk + effect->spDef + effect->speed + effect->accuracy + effect->evasion);
        if ((effect->moveEffect == MOVE_EFFECT_STAT_MINUS) == effect->self)
            value = -value;
        if (effect->self && ctx->ability == ABILITY_CONTRARY)
            value = -value;
        break;
    case MOVE_EFFECT_CLEAR_SMOG: case MOVE_EFFECT_REMOVE_STATUS: case MOVE_EFFECT_SALT_CURE: value = 20; break;
    default: value = 0; break;
    }
    if (effect->onChargeTurnOnly || effect->onlyIfTargetRaisedStats || effect->pledgeCombo)
        value /= 2;
    u32 chance = effect->chance == 0 ? 100 : effect->chance;
    if (ctx->ability == ABILITY_SERENE_GRACE)
        chance = min(100, chance * 2);
    if (ctx->ability == ABILITY_SHEER_FORCE && effect->chance != 0 && value > 0 && !effect->self)
        return 0;
    return value * chance / 100;
}

// A target-independent estimate for team generation, separate from turn-by-turn AI.
static s32 DamageMoveScore(const struct MoveContext *ctx, enum Move move)
{
    const struct MoveInfo *info = &gMovesInfo[move];
    if (info->category == DAMAGE_CATEGORY_STATUS || info->effect == EFFECT_PLACEHOLDER || info->effect == EFFECT_OHKO
        || info->effect == EFFECT_DREAM_EATER || info->effect == EFFECT_FLING || info->effect == EFFECT_NATURAL_GIFT
        || (info->effect == EFFECT_SNORE && ctx->ability != ABILITY_COMATOSE))
        return -1;
    s32 power = info->power;
    bool32 fixed = FALSE;
    switch (info->effect)
    {
    case EFFECT_LEVEL_DAMAGE: power = ctx->level * 2; fixed = TRUE; break;
    case EFFECT_FIXED_HP_DAMAGE: power = info->argument.fixedDamage * 2; fixed = TRUE; break;
    case EFFECT_FIXED_PERCENT_DAMAGE: power = 60; fixed = TRUE; break;
    case EFFECT_REFLECT_DAMAGE: case EFFECT_BIDE: power = 40; fixed = TRUE; break;
    case EFFECT_PSYWAVE: power = ctx->level; fixed = TRUE; break;
    case EFFECT_RETURN: power = max(1, ctx->friendship * 10 / 25); break;
    case EFFECT_FRUSTRATION: power = max(1, (255 - ctx->friendship) * 10 / 25); break;
    case EFFECT_LOW_KICK: case EFFECT_HEAT_CRASH: power = 80; break;
    case EFFECT_GYRO_BALL: power = ctx->speed < 50 ? 100 : 40; break;
    case EFFECT_ELECTRO_BALL: power = ctx->speed >= 100 ? 100 : 40; break;
    case EFFECT_HIDDEN_POWER: power = 60; break;
    case EFFECT_STORED_POWER: power = 20; break;
    default: break;
    }
    if (power <= 1)
        power = 60; // Variable-power damaging moves without a fixed preview power.
    u32 technicianPower = power;
    if (info->multiHit)
        power = power * (ctx->ability == ABILITY_SKILL_LINK ? 5 : 3);
    else if (info->strikeCount > 1)
        power *= info->strikeCount;
    u32 accuracy = info->accuracy == 0 ? 100 : info->accuracy;
    if (ctx->ability == ABILITY_COMPOUND_EYES)
        accuracy = min(100, accuracy * 13 / 10);
    if (ctx->ability == ABILITY_NO_GUARD)
        accuracy = 100;
    if (ctx->ability == ABILITY_HUSTLE && info->category == DAMAGE_CATEGORY_PHYSICAL)
        accuracy = accuracy * 80 / 100;
    s32 score = power;
    if (!fixed)
    {
        u32 offense = info->category == DAMAGE_CATEGORY_PHYSICAL ? ctx->attack : ctx->special;
        if (move == MOVE_BODY_PRESS) offense = ctx->defense;
        if (move == MOVE_FOUL_PLAY) offense = max(ctx->attack, ctx->special);
        if (info->category == DAMAGE_CATEGORY_PHYSICAL && (ctx->ability == ABILITY_HUGE_POWER || ctx->ability == ABILITY_PURE_POWER) && move != MOVE_BODY_PRESS && move != MOVE_FOUL_PLAY)
            offense *= 2;
        score = score * offense / max(1, max(ctx->attack, ctx->special));
        enum Type type = ContextMoveType(ctx, move);
        if (type == ctx->types[0] || type == ctx->types[1])
            score = score * (ctx->ability == ABILITY_ADAPTABILITY ? 200 : 150) / 100;
        if (ctx->ability == ABILITY_TECHNICIAN && technicianPower <= 60 && technicianPower > 0) score = score * 150 / 100;
        if (ctx->ability == ABILITY_IRON_FIST && info->punchingMove) score = score * 120 / 100;
        if (ctx->ability == ABILITY_TOUGH_CLAWS && info->makesContact) score = score * 130 / 100;
        if ((ctx->ability == ABILITY_STRONG_JAW && info->bitingMove) || (ctx->ability == ABILITY_SHARPNESS && info->slicingMove) || (ctx->ability == ABILITY_MEGA_LAUNCHER && info->pulseMove)) score = score * 150 / 100;
        if (ctx->ability == ABILITY_HUSTLE && info->category == DAMAGE_CATEGORY_PHYSICAL) score = score * 150 / 100;
        if ((ctx->item == HOLD_EFFECT_CHOICE_BAND && info->category == DAMAGE_CATEGORY_PHYSICAL) || (ctx->item == HOLD_EFFECT_CHOICE_SPECS && info->category == DAMAGE_CATEGORY_SPECIAL)) score = score * 150 / 100;
        if (ctx->item == HOLD_EFFECT_LIFE_ORB) score = score * 130 / 100;
        if (ContextMoveType(ctx, move) != info->type || ctx->ability == ABILITY_NORMALIZE) score = score * 120 / 100;
        if (ctx->ability == ABILITY_DROUGHT && type == TYPE_FIRE) score = score * 150 / 100;
        if (ctx->ability == ABILITY_DRIZZLE && type == TYPE_WATER) score = score * 150 / 100;
        if ((ctx->ability == ABILITY_DROUGHT && type == TYPE_WATER) || (ctx->ability == ABILITY_DRIZZLE && type == TYPE_FIRE)) score /= 2;
        if (ctx->ability == ABILITY_SHEER_FORCE)
            for (u32 i = 0; i < info->numAdditionalEffects; i++)
                if (info->additionalEffects[i].chance > 0 && !info->additionalEffects[i].self && SecondaryScore(ctx, &info->additionalEffects[i]) >= 0)
                { score = score * 130 / 100; break; }
    }
    score = score * accuracy / 100;
    if (info->alwaysCriticalHit) score = score * 150 / 100;
    else if (info->criticalHitStage) score += score / 12;
    if (info->effect == EFFECT_TWO_TURNS_ATTACK && !(ctx->ability == ABILITY_DROUGHT && (move == MOVE_SOLAR_BEAM || move == MOVE_SOLAR_BLADE))) score = score * 60 / 100;
    if (info->effect == EFFECT_RECOIL && ctx->ability != ABILITY_ROCK_HEAD && ctx->ability != ABILITY_MAGIC_GUARD) score = score * (100 - info->argument.recoilPercentage / 2) / 100;
    if (info->explosion || info->effect == EFFECT_MAX_HP_50_RECOIL || info->effect == EFFECT_CHLOROBLAST) score /= 2;
    if (info->effect == EFFECT_FOCUS_PUNCH || info->effect == EFFECT_LAST_RESORT || info->effect == EFFECT_SUCKER_PUNCH || info->cantUseTwice) score = score * 65 / 100;
    if (info->effect == EFFECT_FALSE_SWIPE) score /= 2;
    if (info->priority > 0) score += 18;
    if (info->priority < 0) score -= 8;
    for (u32 i = 0; i < info->numAdditionalEffects; i++)
        score += SecondaryScore(ctx, &info->additionalEffects[i]);
    score += min(info->pp, 20) / 5;
    return max(1, score);
}

static s32 SupportMoveScore(const struct MoveContext *ctx, enum Move move)
{
    if (ctx->item == HOLD_EFFECT_CHOICE_BAND || ctx->item == HOLD_EFFECT_CHOICE_SPECS || ctx->item == HOLD_EFFECT_CHOICE_SCARF || ctx->item == HOLD_EFFECT_ASSAULT_VEST)
        return 0;
    switch (move)
    {
    case MOVE_RECOVER: case MOVE_ROOST: case MOVE_SOFT_BOILED: case MOVE_SLACK_OFF: case MOVE_WISH: case MOVE_SYNTHESIS: return 110;
    case MOVE_SPORE: return 110;
    case MOVE_STEALTH_ROCK: case MOVE_SPIKES: case MOVE_STICKY_WEB: return 90;
    case MOVE_WILL_O_WISP: case MOVE_THUNDER_WAVE: case MOVE_TOXIC: case MOVE_LEECH_SEED: return 85;
    case MOVE_REFLECT: case MOVE_LIGHT_SCREEN: return 75;
    case MOVE_TRICK_ROOM: return ctx->speed <= 60 ? 120 : 0;
    case MOVE_SWORDS_DANCE: case MOVE_DRAGON_DANCE: return ctx->ability != ABILITY_CONTRARY && ctx->attack >= ctx->special ? 100 : 0;
    case MOVE_NASTY_PLOT: case MOVE_CALM_MIND: case MOVE_QUIVER_DANCE: return ctx->ability != ABILITY_CONTRARY && ctx->special >= ctx->attack ? 100 : 0;
    case MOVE_PROTECT: return ctx->item == HOLD_EFFECT_LEFTOVERS || ctx->ability == ABILITY_POISON_HEAL ? 80 : 0;
    default: return 0;
    }
}

static void SelectAbility(struct Pokemon *mon, bool32 easy)
{
    enum Species species = GetMonData(mon, MON_DATA_SPECIES);
    u32 best = GetMonData(mon, MON_DATA_ABILITY_NUM);
    s32 bestScore = easy ? 10000 : -10000;
    for (u32 slot = 0; slot < NUM_ABILITY_SLOTS; slot++)
    {
        enum Ability ability = GetAbilityBySpecies(species, slot);
        if (ability == ABILITY_NONE)
            continue;
        s32 score = gAbilitiesInfo[ability].aiRating * 10;
        if (ability == ABILITY_HUGE_POWER || ability == ABILITY_PURE_POWER) score += 40;
        if ((ability == ABILITY_MOXIE || ability == ABILITY_HUSTLE) && GetSpeciesBaseAttack(species) < GetSpeciesBaseSpAttack(species)) score -= 20;
        if ((ability == ABILITY_TECHNICIAN || ability == ABILITY_SHEER_FORCE || ability == ABILITY_SKILL_LINK) && GetSpeciesBaseAttack(species) + GetSpeciesBaseSpAttack(species) > 160) score += 5;
        if ((easy && score < bestScore) || (!easy && score > bestScore))
        {
            best = slot;
            bestScore = score;
        }
    }
    SetMonData(mon, MON_DATA_ABILITY_NUM, &best);
}

void ApplyBriskTrainerLoadout(struct Pokemon *mon)
{
    u32 mode = GetBriskDifficulty();
    if (mode == BRISK_MODE_NORMAL)
        return;
    bool32 easy = mode == BRISK_MODE_EASY;
    SelectAbility(mon, easy);
    if (mode == BRISK_MODE_HARD)
        return;
    struct MoveContext ctx;
    InitMoveContext(&ctx, mon);
    u32 pool[MOVE_POOL_WORDS];
    BuildMovePool(pool, GetMonData(mon, MON_DATA_SPECIES), ctx.level);
    u16 selected[MAX_MON_MOVES] = {0};
    enum Move support = MOVE_NONE;
    s32 supportScore = 0;
    if (!easy)
        for (u32 i = 0; i < MAX_MON_MOVES; i++)
        {
            enum Move move = GetMonData(mon, MON_DATA_MOVE1 + i);
            if (!InMovePool(pool, move) || GetMoveCategory(move) != DAMAGE_CATEGORY_STATUS)
                continue;
            s32 score = SupportMoveScore(&ctx, move);
            if (score > supportScore) { support = move; supportScore = score; }
        }
    u32 count = 0;
    for (; count < MAX_MON_MOVES - (support != MOVE_NONE); count++)
    {
        s32 bestScore = easy ? 100000 : -1;
        enum Move best = MOVE_NONE;
        for (enum Move move = 1; move < MOVES_COUNT; move++)
        {
            if (!InMovePool(pool, move)) continue;
            bool32 duplicate = FALSE;
            for (u32 i = 0; i < count; i++) if (selected[i] == move) duplicate = TRUE;
            if (duplicate) continue;
            s32 score = DamageMoveScore(&ctx, move);
            if (score < 0) continue;
            if (!easy)
                for (u32 i = 0; i < count; i++)
                    if (ContextMoveType(&ctx, selected[i]) == ContextMoveType(&ctx, move)) score = score * 60 / 100;
            if ((easy && score < bestScore) || (!easy && score > bestScore)) { best = move; bestScore = score; }
        }
        if (best == MOVE_NONE) break;
        selected[count] = best;
    }
    // Ditto and other species without legal attacks keep their functional moves.
    if (count == 0) return;
    if (support != MOVE_NONE) selected[count++] = support;
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
        SetMonMoveSlot(mon, selected[i], i);
}

#if TESTING
s32 Test_BriskTrainerMoveScore(struct Pokemon *mon, enum Move move)
{
    struct MoveContext ctx;
    InitMoveContext(&ctx, mon);
    return DamageMoveScore(&ctx, move);
}
bool32 Test_BriskTrainerMoveLegal(enum Species species, u32 level, enum Move move)
{
    u32 pool[MOVE_POOL_WORDS];
    BuildMovePool(pool, species, level);
    return InMovePool(pool, move);
}
#endif
