#include "global.h"
#include "dynamax_adventure.h"
#include "brisk_modes.h"
#include "brisk_options.h"
#include "brisk_difficulty.h"
#include "caps.h"
#include "battle.h"
#include "event_data.h"
#include "item.h"
#include "pokemon.h"
#include "constants/trainers.h"
#include "constants/battle_ai.h"
#include "constants/battle.h"
#include "constants/hold_effects.h"

static u32 ModeHash(u32 value, u32 salt)
{
    // Stable for this save; never consumes the battle/encounter RNG.
    u32 hash = READ_OTID_FROM_SAVE ^ value * 0x9E3779B9u ^ salt;
    hash ^= hash >> 16;
    hash *= 0x7FEB352Du;
    hash ^= hash >> 15;
    hash *= 0x846CA68Bu;
    return hash ^ (hash >> 16);
}

u32 GetBriskDifficulty(void)
{
    u32 mode = VarGet(VAR_BRISK_DIFFICULTY);
    return mode <= BRISK_MODE_EXTREME ? mode : BRISK_MODE_NORMAL;
}

u32 GetBriskPriceMultiplier(void)
{
    static const u8 multipliers[] = {2, 1, 4, 10};
    return multipliers[GetBriskDifficulty()];
}

static u64 GetLegacyBriskTrainerAiFlags(u64 flags)
{
    switch (GetBriskDifficulty())
    {
    case BRISK_MODE_EASY: return AI_FLAG_CHECK_BAD_MOVE;
    // Some legacy parties only specify viability (or no flags). Knowledge and
    // switch selection alone do not enable the basic move-scoring scripts.
    case BRISK_MODE_HARD: return flags | AI_FLAG_BASIC_TRAINER | AI_FLAG_SMART_SWITCHING | AI_FLAG_HP_AWARE | AI_FLAG_TRY_TO_2HKO | AI_FLAG_SMART_TERA | AI_FLAG_PP_STALL_PREVENTION;
    case BRISK_MODE_EXTREME: return flags | AI_FLAG_SMART_TRAINER | AI_FLAG_HP_AWARE | AI_FLAG_TRY_TO_2HKO;
    default: return flags;
    }
}

u64 GetBriskTrainerAiFlags(u64 flags)
{
    switch (GetBriskDifficulty())
    {
    case BRISK_MODE_EASY: return AI_FLAG_CHECK_BAD_MOVE;
    case BRISK_MODE_NORMAL:
        return (flags & ~(AI_FLAG_OMNISCIENT | AI_FLAG_ABILITY_OMNISCIENCE | AI_FLAG_ITEM_OMNISCIENCE
            | AI_FLAG_MOVE_OMNISCIENCE | AI_FLAG_PREDICTION | AI_FLAG_KNOW_OPPONENT_PARTY | AI_FLAG_FORCE_SETUP_FIRST_TURN))
            | AI_FLAG_BASIC_TRAINER | AI_FLAG_HP_AWARE | AI_FLAG_TRY_TO_2HKO | AI_FLAG_SMART_MON_CHOICES;
    case BRISK_MODE_HARD: return flags | AI_FLAG_SMART_TRAINER | AI_FLAG_HP_AWARE | AI_FLAG_TRY_TO_2HKO;
    case BRISK_MODE_EXTREME: return flags | AI_FLAG_SMART_TRAINER | AI_FLAG_HP_AWARE | AI_FLAG_TRY_TO_2HKO
        | AI_FLAG_PREDICTION | AI_FLAG_KNOW_OPPONENT_PARTY | AI_FLAG_CONSERVATIVE;
    }
    return flags;
}

u64 GetBriskTrainerAiFlagsForTrainer(u64 flags, u32 id)
{
    return BriskTrainerDifficultyExempt(id) ? GetLegacyBriskTrainerAiFlags(flags) : GetBriskTrainerAiFlags(flags);
}

u32 GetBriskBattleStyle(u32 preference)
{
    switch (GetBriskDifficulty())
    {
    case BRISK_MODE_EASY: return OPTIONS_BATTLE_STYLE_SHIFT;
    case BRISK_MODE_NORMAL:
    case BRISK_MODE_HARD:
    case BRISK_MODE_EXTREME: return OPTIONS_BATTLE_STYLE_SET;
    default: return preference;
    }
}

bool32 BriskBattleItemsAllowed(void)
{
    return GetBriskDifficulty() == BRISK_MODE_EASY || GetBriskDifficulty() == BRISK_MODE_NORMAL;
}

bool32 BriskCanUseBattleItem(enum Item item)
{
    if (!BriskBattleBagAllowed())
        return FALSE;
    // The restriction applies to player-used consumables, not held items or catching.
    return BriskBattleItemsAllowed() || GetItemPocket(item) == POCKET_POKE_BALLS;
}

bool32 BriskModernExpShare(void)
{
    u32 setting = VarGet(VAR_BRISK_EXP_SHARE);
    return setting == 1 || setting == 2;
}

bool32 BriskModernExpShareEnabled(void)
{
    return GetBriskDifficulty() != BRISK_MODE_EXTREME
        && VarGet(VAR_BRISK_EXP_SHARE) == 1
        && CheckBagHasItem(ITEM_MODERN_EXP_SHARE, 1);
}

bool32 BriskMonGetsExpShare(struct Pokemon *mon)
{
    if (GetBriskDifficulty() == BRISK_MODE_EXTREME)
        return FALSE;
    if (BriskModernExpShare())
        return BriskModernExpShareEnabled();
    return GetItemHoldEffect(GetMonData(mon, MON_DATA_HELD_ITEM)) == HOLD_EFFECT_EXP_SHARE;
}

u32 ScaleBriskSharedExp(u32 exp)
{
    switch (GetBriskDifficulty())
    {
    case BRISK_MODE_EASY: return exp * 2;
    case BRISK_MODE_HARD: return exp == 0 ? 0 : max(1, exp / 2);
    case BRISK_MODE_EXTREME: return 0;
    default: return exp;
    }
}

void SetBriskExpShareMode(bool32 modern)
{
    // Preserve the Key Item's on/off switch when saving unrelated options.
    if (!modern)
        VarSet(VAR_BRISK_EXP_SHARE, 0);
    else
    {
        if (!BriskModernExpShare())
            VarSet(VAR_BRISK_EXP_SHARE, 1);
        bool32 earned = FlagGet(FLAG_RECEIVED_EXP_SHARE) || CheckBagHasItem(ITEM_EXP_SHARE, 1) || CheckPCHasItem(ITEM_EXP_SHARE, 1);
        for (u32 i = 0; i < PARTY_SIZE && !earned; i++)
            earned = GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_HELD_ITEM) == ITEM_EXP_SHARE;
        // Migrate existing saves without moving or deleting their held Shares.
        if (earned && !CheckBagHasItem(ITEM_MODERN_EXP_SHARE, 1))
            AddBagItem(ITEM_MODERN_EXP_SHARE, 1);
    }
}

u32 ScaleBriskLevel(u32 level, u32 salt)
{
    // Small proportional boosts keep early-route training viable. The salt
    // remains in the API for callers; level difficulty is now predictable.
    (void)salt;
    switch (GetBriskDifficulty())
    {
    case BRISK_MODE_EASY: level = (level + 1) / 2; break;
    case BRISK_MODE_HARD: level += max(1, level / 10); break;
    case BRISK_MODE_EXTREME: level += max(2, level / 5); break;
    }
    return max(1, min(level, MAX_LEVEL));
}

u32 ScaleBriskTrainerLevel(u32 level, u32 salt)
{
    // Trainer-only increases; wild encounter difficulty remains unchanged.
    switch (GetBriskDifficulty())
    {
    case BRISK_MODE_HARD: level += max(3, level / 5); break;
    case BRISK_MODE_EXTREME: level += max(4, level * 3 / 10); break;
    default: level = ScaleBriskLevel(level, salt); break;
    }
    level = min(level, MAX_LEVEL);
    // Do not put a level-80 boss in front of a team hard-capped at level 15.
    // Owned levels and Frontier/Adventure rentals are not rewritten.
    if (GetBriskDifficulty() == BRISK_MODE_HARD || GetBriskDifficulty() == BRISK_MODE_EXTREME)
        level = min(level, GetProgressionLevelCap());
    return level;
}

u32 ScaleBriskOrdinaryTrainerLevel(u32 level, u32 highestLevel)
{
    u32 mode = GetBriskDifficulty();
    highestLevel = max(level, highestLevel);
    u32 ace = highestLevel;
    u32 originalLevel = level;
    switch (mode)
    {
    case BRISK_MODE_EASY:
        level = (level + 1) / 2;
        ace = (ace + 1) / 2;
        // A lower-level support must not round up to the ace's Easy level.
        if (originalLevel < highestLevel && level == ace && level > 1)
            level -= 1;
        break;
    case BRISK_MODE_HARD:
        level += max(2, level / 5);
        break;
    case BRISK_MODE_EXTREME:
        level += max(4, level * 2 / 5);
        break;
    default: return max(1, min(level, MAX_LEVEL));
    }
    if (mode == BRISK_MODE_HARD || mode == BRISK_MODE_EXTREME)
    {
        // Keep each support below the capped ace by its authored level gap.
        // Unlike subtracting the boosted ace's excess from the whole team,
        // this cannot make an Extreme support lower-level than its Hard set.
        u32 cap = GetProgressionLevelCap();
        u32 gap = highestLevel - originalLevel;
        u32 ceiling = cap > gap ? cap - gap : 1;
        level = min(level, ceiling);
    }
    return max(1, min(level, MAX_LEVEL));
}

u32 ScaleBriskStoryTrainerLevel(u32 level, u32 highestLevel)
{
    u32 originalLevel = level;
    highestLevel = max(level, highestLevel);
    switch (GetBriskDifficulty())
    {
    case BRISK_MODE_EASY: return max(1, (level + 1) / 2);
    case BRISK_MODE_HARD: level += max(3, level / 4); break;
    case BRISK_MODE_EXTREME: level += max(5, level * 45 / 100); break;
    default: return max(1, min(level, MAX_LEVEL));
    }
    u32 cap = GetProgressionLevelCap();
    u32 gap = highestLevel - originalLevel;
    u32 ceiling = cap > gap ? cap - gap : 1;
    return max(1, min(level, ceiling));
}

bool32 BriskRandomizerEnabled(u32 category)
{
    return (VarGet(VAR_BRISK_RANDOMIZER) & (category | BRISK_RAND_ALL)) != 0;
}

bool32 BriskIsImportantTrainer(u32 trainerClass)
{
    switch (trainerClass)
    {
    case TRAINER_CLASS_LEADER: case TRAINER_CLASS_ELITE_FOUR: case TRAINER_CLASS_CHAMPION:
    case TRAINER_CLASS_RIVAL: case TRAINER_CLASS_RS_PROTAG:
    case TRAINER_CLASS_AQUA_ADMIN: case TRAINER_CLASS_AQUA_LEADER:
    case TRAINER_CLASS_MAGMA_ADMIN: case TRAINER_CLASS_MAGMA_LEADER:
    case TRAINER_CLASS_SALON_MAIDEN: case TRAINER_CLASS_DOME_ACE: case TRAINER_CLASS_PALACE_MAVEN:
    case TRAINER_CLASS_ARENA_TYCOON: case TRAINER_CLASS_FACTORY_HEAD: case TRAINER_CLASS_PIKE_QUEEN: case TRAINER_CLASS_PYRAMID_KING:
    case TRAINER_CLASS_RIVAL_EARLY_FRLG: case TRAINER_CLASS_RIVAL_LATE_FRLG:
    case TRAINER_CLASS_BOSS_FRLG: case TRAINER_CLASS_LEADER_FRLG:
    case TRAINER_CLASS_ELITE_FOUR_FRLG: case TRAINER_CLASS_CHAMPION_FRLG:
        return TRUE;
    default: return FALSE;
    }
}

static bool32 IsRandomSpecies(enum Species species)
{
    if (species == SPECIES_NONE || species == SPECIES_EGG || !IsSpeciesEnabled(species))
        return FALSE;
    const struct SpeciesInfo *info = &gSpeciesInfo[species];
    // Only permanent base species, never temporary battle forms or cosmetic states.
    return !info->isMegaEvolution && !info->isPrimalReversion && !info->isUltraBurst && !info->isGigantamax
        && (info->formSpeciesIdTable == NULL || info->formSpeciesIdTable[0] == species);
}

static enum Species PickSpecies(enum Species species, u32 salt)
{
    u32 start = 1 + ModeHash(species, salt) % (NUM_SPECIES - 1);
    for (u32 i = 0; i < NUM_SPECIES - 1; i++)
    {
        enum Species candidate = 1 + (start - 1 + i) % (NUM_SPECIES - 1);
        if (candidate != species && IsRandomSpecies(candidate))
            return candidate;
    }
    return species;
}

enum Species BriskRandomSpecies(enum Species species, u32 category)
{
    if (species == SPECIES_NONE || species == SPECIES_EGG || !BriskRandomizerEnabled(category))
        return species;
    return PickSpecies(species, category + 0x200);
}

enum Species BriskTraitSpecies(enum Species species, u32 salt)
{
    if (species == SPECIES_NONE || species == SPECIES_EGG || !BriskRandomizerEnabled(BRISK_RAND_ALL))
        return SanitizeSpeciesId(species);
    return PickSpecies(species, salt + 0x300);
}

static bool32 CanRandomizeItem(enum Item item)
{
    return item > ITEM_NONE && item < ITEMS_COUNT && gItemsInfo[item].price > 0
        && !gItemsInfo[item].importance && gItemsInfo[item].pocket != POCKET_KEY_ITEMS
        && gItemsInfo[item].iconPic != NULL;
}

enum Item BriskRandomItem(enum Item item)
{
    if (!BriskRandomizerEnabled(BRISK_RAND_ITEMS) || !CanRandomizeItem(item))
        return item;
    u32 start = 1 + ModeHash(item, 0x400) % (ITEMS_COUNT - 1);
    for (u32 i = 0; i < ITEMS_COUNT - 1; i++)
    {
        enum Item candidate = 1 + (start - 1 + i) % (ITEMS_COUNT - 1);
        if (candidate != item && CanRandomizeItem(candidate))
            return candidate;
    }
    return item;
}

void BriskRandomizeScriptItem(void)
{
    gSpecialVar_0x8000 = BriskRandomItem(gSpecialVar_0x8000);
}
