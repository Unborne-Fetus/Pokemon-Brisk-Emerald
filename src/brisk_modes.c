#include "global.h"
#include "brisk_modes.h"
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

u64 GetBriskTrainerAiFlags(u64 flags)
{
    switch (GetBriskDifficulty())
    {
    case BRISK_MODE_EASY: return AI_FLAG_CHECK_BAD_MOVE;
    case BRISK_MODE_HARD: return flags | AI_FLAG_SMART_MON_CHOICES;
    case BRISK_MODE_EXTREME: return flags | AI_FLAG_SMART_MON_CHOICES | AI_FLAG_OMNISCIENT;
    default: return flags;
    }
}

u32 GetBriskBattleStyle(u32 preference)
{
    switch (GetBriskDifficulty())
    {
    case BRISK_MODE_EASY: return OPTIONS_BATTLE_STYLE_SHIFT;
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
    switch (GetBriskDifficulty())
    {
    case BRISK_MODE_EASY: level = (level + 1) / 2; break;
    case BRISK_MODE_HARD: level += 10 + ModeHash(salt, 0x101) % 11; break;
    case BRISK_MODE_EXTREME: level += 40 + ModeHash(salt, 0x101) % 11; break;
    }
    return max(1, min(level, MAX_LEVEL));
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
