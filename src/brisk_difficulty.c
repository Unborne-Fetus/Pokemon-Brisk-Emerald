#include "global.h"
#include "battle.h"
#include "brisk_difficulty.h"
#include "brisk_modes.h"
#include "data.h"
#include "item.h"
#include "move.h"
#include "pokemon.h"
#include "trainer_util.h"
#include "constants/opponents.h"
#include "constants/trainers.h"
#include "constants/hold_effects.h"
#include "data/brisk_exempt_trainers.h"

bool32 BriskTrainerDifficultyExempt(u32 trainerId)
{
    return trainerId < TRAINERS_COUNT && sBriskExemptTrainer[trainerId];
}

u32 BriskDifficultyPartySize(u32 count, u32 highestLevel, bool32 story, bool32 league, bool32 tuned)
{
    if (!count || !tuned || league || (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE))
        return count;
    u32 target = count;
    switch (GetBriskDifficulty())
    {
    case BRISK_MODE_EASY:
        if (story) target = highestLevel < 30 ? 2 : highestLevel < 50 ? 3 : highestLevel < 80 ? 4 : 5;
        else target = highestLevel < 40 ? 2 : highestLevel < 70 ? 3 : 4;
        return min(count, target);
    case BRISK_MODE_NORMAL:
        target = highestLevel < 20 ? 2 : highestLevel < 40 ? 3 : 4;
        break;
    case BRISK_MODE_HARD:
        target = story ? (highestLevel < 25 ? 4 : highestLevel < 40 ? 5 : 6)
                       : (highestLevel < 20 ? 3 : highestLevel < 40 ? 4 : 5);
        break;
    case BRISK_MODE_EXTREME:
        target = story ? (highestLevel < 20 ? 4 : 6) : (highestLevel < 20 ? 4 : highestLevel < 30 ? 5 : 6);
        break;
    }
    return min(PARTY_SIZE, max(count, target));
}

enum Species BriskEasyTrainerSpecies(enum Species species, u32 level)
{
    // Stock-game-style progression: don't fight a final evolution merely
    // because its Normal-mode level was halved. Trade/item stages need time too.
    for (u32 depth = 0; depth < 8; depth++)
    {
        enum Species parent = GetSpeciesPreEvolution(species);
        if (parent == SPECIES_NONE) break;
        u32 required = GetSpeciesBaseHP(species) + GetSpeciesBaseAttack(species)
            + GetSpeciesBaseDefense(species) + GetSpeciesBaseSpeed(species)
            + GetSpeciesBaseSpAttack(species) + GetSpeciesBaseSpDefense(species) >= 500 ? 30 : 24;
        const struct Evolution *edges = GetSpeciesEvolutions(parent);
        for (u32 i = 0; edges && edges[i].method != EVOLUTIONS_END; i++)
            if (edges[i].targetSpecies == species && edges[i].method == EVO_LEVEL && edges[i].param > 0)
                required = edges[i].param;
        if (level >= required) break;
        species = parent;
    }
    return species;
}

struct ReserveCandidate {u16 species; u16 family; u8 level;};
#define RESERVE(species, level, family) {SPECIES_##species, SPECIES_##family, level}
static const struct ReserveCandidate sReserves[] = {
    RESERVE(ZIGZAGOON,1,ZIGZAGOON), RESERVE(LINOONE,20,ZIGZAGOON), RESERVE(EEVEE,1,EEVEE), RESERVE(SNORLAX,35,MUNCHLAX),
    RESERVE(MANKEY,1,MANKEY), RESERVE(PRIMEAPE,28,MANKEY), RESERVE(ANNIHILAPE,40,MANKEY), RESERVE(MACHOKE,28,MACHOP), RESERVE(HARIYAMA,24,MAKUHITA), RESERVE(LUCARIO,35,RIOLU),
    RESERVE(WINGULL,1,WINGULL), RESERVE(PELIPPER,25,WINGULL), RESERVE(GOLBAT,22,ZUBAT), RESERVE(CROBAT,35,ZUBAT),
    RESERVE(SHROOMISH,1,SHROOMISH), RESERVE(BRELOOM,23,SHROOMISH), RESERVE(ROSELIA,20,BUDEW), RESERVE(ROSERADE,35,BUDEW),
    RESERVE(ELECTRIKE,1,ELECTRIKE), RESERVE(MANECTRIC,26,ELECTRIKE), RESERVE(MAREEP,1,MAREEP), RESERVE(FLAAFFY,15,MAREEP), RESERVE(AMPHAROS,30,MAREEP), RESERVE(RAICHU,30,PICHU), RESERVE(MAGNEMITE,10,MAGNEMITE), RESERVE(MAGNETON,30,MAGNEMITE),
    RESERVE(VULPIX,10,VULPIX), RESERVE(NINETALES,35,VULPIX), RESERVE(NUMEL,10,NUMEL), RESERVE(CAMERUPT,33,NUMEL), RESERVE(GROWLITHE,10,GROWLITHE), RESERVE(ARCANINE,35,GROWLITHE), RESERVE(ROLYCOLY,10,ROLYCOLY), RESERVE(CARKOL,18,ROLYCOLY), RESERVE(COALOSSAL,34,ROLYCOLY), RESERVE(LARVESTA,15,LARVESTA), RESERVE(VOLCARONA,59,LARVESTA),
    RESERVE(GEODUDE,1,GEODUDE), RESERVE(GRAVELER,25,GEODUDE), RESERVE(GOLEM,36,GEODUDE), RESERVE(LILEEP,20,LILEEP), RESERVE(CRADILY,40,LILEEP),
    RESERVE(SANDSHREW,1,SANDSHREW), RESERVE(SANDSLASH,22,SANDSHREW), RESERVE(WOOPER,1,WOOPER), RESERVE(QUAGSIRE,20,WOOPER),
    RESERVE(DUSKULL,10,DUSKULL), RESERVE(DUSCLOPS,37,DUSKULL), RESERVE(DUSKNOIR,45,DUSKULL), RESERVE(LAMPENT,41,LITWICK), RESERVE(CHANDELURE,45,LITWICK), RESERVE(HAUNTER,25,GASTLY), RESERVE(GENGAR,40,GASTLY),
    RESERVE(ABRA,10,ABRA), RESERVE(KADABRA,16,ABRA), RESERVE(ALAKAZAM,36,ABRA), RESERVE(GARDEVOIR,30,RALTS),
    RESERVE(SEWADDLE,1,SEWADDLE), RESERVE(SWADLOON,20,SEWADDLE), RESERVE(LEAVANNY,30,SEWADDLE),
    RESERVE(VENIPEDE,1,VENIPEDE), RESERVE(WHIRLIPEDE,22,VENIPEDE), RESERVE(SCOLIPEDE,30,VENIPEDE),
    RESERVE(NINCADA,1,NINCADA), RESERVE(NINJASK,20,NINCADA), RESERVE(HERACROSS,25,HERACROSS), RESERVE(SCIZOR,35,SCYTHER),
    RESERVE(CARVANHA,10,CARVANHA), RESERVE(SHARPEDO,30,CARVANHA), RESERVE(LOMBRE,14,LOTAD), RESERVE(LUDICOLO,30,LOTAD),
    RESERVE(SKARMORY,30,SKARMORY), RESERVE(CORVIKNIGHT,38,ROOKIDEE), RESERVE(EXCADRILL,31,DRILBUR),
    RESERVE(ALTARIA,35,SWABLU), RESERVE(FLYGON,45,TRAPINCH), RESERVE(KINGDRA,40,HORSEA), RESERVE(GYARADOS,30,MAGIKARP), RESERVE(MILOTIC,40,FEEBAS), RESERVE(STARMIE,35,STARYU), RESERVE(GARCHOMP,48,GIBLE),
    RESERVE(ABSOL,30,ABSOL), RESERVE(UMBREON,35,EEVEE), RESERVE(KROKOROK,29,SANDILE), RESERVE(KROOKODILE,40,SANDILE), RESERVE(SWINUB,10,SWINUB), RESERVE(PILOSWINE,33,SWINUB), RESERVE(MAMOSWINE,40,SWINUB), RESERVE(SNOVER,15,SNOVER), RESERVE(ABOMASNOW,40,SNOVER),
    RESERVE(CLEFAIRY,10,CLEFFA), RESERVE(CLEFABLE,35,CLEFFA), RESERVE(TOGEKISS,40,TOGEPI),
};
#undef RESERVE

static u32 ThemeType(u32 trainerClass)
{
    switch (trainerClass)
    {
    case TRAINER_CLASS_BUG_CATCHER: case TRAINER_CLASS_BUG_MANIAC: return TYPE_BUG;
    case TRAINER_CLASS_BIRD_KEEPER: return TYPE_FLYING;
    case TRAINER_CLASS_HIKER: case TRAINER_CLASS_RUIN_MANIAC: return TYPE_ROCK;
    case TRAINER_CLASS_SWIMMER_M: case TRAINER_CLASS_SWIMMER_F: case TRAINER_CLASS_FISHERMAN: case TRAINER_CLASS_SAILOR: return TYPE_WATER;
    case TRAINER_CLASS_KINDLER: return TYPE_FIRE;
    case TRAINER_CLASS_GUITARIST: return TYPE_ELECTRIC;
    case TRAINER_CLASS_PSYCHIC: return TYPE_PSYCHIC;
    case TRAINER_CLASS_HEX_MANIAC: return TYPE_GHOST;
    case TRAINER_CLASS_BLACK_BELT: case TRAINER_CLASS_BATTLE_GIRL: return TYPE_FIGHTING;
    default: return TYPE_MYSTERY;
    }
}

static enum Species PickReserve(struct Pokemon *party, u32 count, u32 level, u32 trainerClass, u32 seed)
{
    u32 seenTypes = 0;
    enum Species families[PARTY_SIZE];
    for (u32 i = 0; i < count; i++)
    {
        enum Species species = GetMonData(&party[i], MON_DATA_SPECIES);
        seenTypes |= (1u << GetSpeciesType(species,0)) | (1u << GetSpeciesType(species,1));
        for (u32 depth = 0; depth < 8; depth++)
        {
            enum Species parent = GetSpeciesPreEvolution(species);
            if (parent == SPECIES_NONE) break;
            species = parent;
        }
        families[i] = species;
    }
    enum Species best = SPECIES_NONE;
    s32 bestScore = -1;
    u32 theme = ThemeType(trainerClass);
    for (u32 pass = 0; pass < 2 && best == SPECIES_NONE; pass++)
    for (u32 n = 0; n < ARRAY_COUNT(sReserves); n++)
    {
        enum Species candidate = sReserves[n].species;
        if (sReserves[n].level > level || !IsSpeciesEnabled(candidate)) continue;
        u32 first = GetSpeciesType(candidate,0), second = GetSpeciesType(candidate,1);
        if (pass == 0 && theme != TYPE_MYSTERY && first != theme && second != theme) continue;
        bool32 duplicate = FALSE;
        for (u32 i = 0; i < count; i++) if (families[i] == sReserves[n].family) duplicate = TRUE;
        for (u32 i = 0; i < ARRAY_COUNT(sReserves); i++)
            if (sReserves[i].family == sReserves[n].family && sReserves[i].level > sReserves[n].level && sReserves[i].level <= level)
                duplicate = TRUE; // Choose the strongest available stage in a family.
        if (duplicate) continue;
        u32 stats = 0;
        for (u32 stat = 0; stat < NUM_STATS; stat++) stats += GetSpeciesBaseStat(candidate,stat);
        if (stats > (level < 12 ? 350 : level < 20 ? 420 : level < 30 ? 500 : 600)) continue;
        s32 score = stats + (!(seenTypes & (1u << first)) ? 160 : 0) + (!(seenTypes & (1u << second)) ? 100 : 0)
            + ((candidate ^ seed) % 17); // Stable variety; no battle RNG consumption.
        if (score > bestScore) {best = candidate; bestScore = score;}
    }
    return best;
}

void BriskExpandTrainerParty(struct Pokemon *party, u32 count, u32 target, struct TrainerGenerator *generator)
{
    if (!count || target <= count) return;
    struct Pokemon ace = party[count-1];
    // Temporarily keep the ace in the candidate scan, then move it back to last.
    u32 originalCount = count;
    while (count < target)
    {
        struct TrainerMon reserve = {0};
        reserve.lvl = max(1, generator->highestLevel - min(generator->highestLevel-1, 1+(count-originalCount)%2));
        u32 actualLevel = generator->storyLoadout ? ScaleBriskStoryTrainerLevel(reserve.lvl,generator->highestLevel)
                                                : ScaleBriskOrdinaryTrainerLevel(reserve.lvl,generator->highestLevel);
        reserve.species = PickReserve(party,count,actualLevel,generator->trainerClass, (u32)generator->localRngState.a);
        if (reserve.species == SPECIES_NONE) break;
        reserve.gender = TRAINER_MON_RANDOM_GENDER;
        reserve.ball = BALL_POKE;
        reserve.teraType = TYPE_MYSTERY;
        reserve.nature = GetSpeciesBaseAttack(reserve.species) >= GetSpeciesBaseSpAttack(reserve.species) ? NATURE_ADAMANT : NATURE_MODEST;
        GenerateMonFromTrainerMon(&party[count++], &reserve, generator);
    }
    if (count > originalCount)
    {
        memmove(&party[originalCount-1], &party[originalCount], (count-originalCount)*sizeof(*party));
        party[count-1] = ace;
    }
}

void BriskDifficultyHeldItem(struct Pokemon *mon)
{
    if (GetBriskDifficulty() != BRISK_MODE_HARD && GetBriskDifficulty() != BRISK_MODE_EXTREME) return;
    enum Item item = GetMonData(mon,MON_DATA_HELD_ITEM);
    if (item != ITEM_NONE && item != ITEM_ORAN_BERRY && item != ITEM_SITRUS_BERRY && item != ITEM_EXPERT_BELT) return;
    u32 level = GetMonData(mon,MON_DATA_LEVEL);
    item = level < 25 ? ITEM_ORAN_BERRY : level < 40 ? ITEM_SITRUS_BERRY : ITEM_LIFE_ORB;
    if (GetMonAbility(mon) == ABILITY_GUTS && level >= 30) item = ITEM_FLAME_ORB;
    else if (level >= 40)
        for (u32 i = 0; i < MAX_MON_MOVES; i++)
        {
            enum Move move = GetMonData(mon,MON_DATA_MOVE1+i);
            if (move == MOVE_RECOVER || move == MOVE_ROOST || move == MOVE_SLACK_OFF || move == MOVE_SOFT_BOILED || move == MOVE_WISH)
                item = ITEM_LEFTOVERS;
        }
    SetMonData(mon,MON_DATA_HELD_ITEM,&item);
}
