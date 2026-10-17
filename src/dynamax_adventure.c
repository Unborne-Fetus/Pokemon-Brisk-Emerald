#include "global.h"
#include "dynamax_adventure.h"
#include "battle.h"
#include "battle_controllers.h"
#include "battle_main.h"
#include "brisk_modes.h"
#include "constants/form_change_types.h"
#include "constants/items.h"
#include "battle_setup.h"
#include "battle_partner.h"
#include "constants/opponents.h"
#include "brisk_trainer_moves.h"
#include "data.h"
#include "move.h"
#include "event_data.h"
#include "main.h"
#include "pokemon_storage_system.h"
#include "player_name.h"
#include "random.h"
#include "string_util.h"
#include "constants/pokedex.h"
#include "constants/maps.h"

struct AdventureSet
{
    u16 species;
    u16 moves[MAX_MON_MOVES];
    u16 ability;
    bool8 support;
    u16 item;
};

#include "data/dynamax_adventure.h"

static u16 AdventureMegaStone(enum Species species)
{
    const struct FormChange *forms = GetSpeciesFormChanges(species);
    if (forms != NULL)
        for (u32 i = 0; forms[i].method != FORM_CHANGE_TERMINATOR; i++)
            if (forms[i].method == FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM
             && IsSpeciesEnabled(forms[i].targetSpecies) && gSpeciesInfo[forms[i].targetSpecies].isMegaEvolution)
            {
                if ((species == SPECIES_CHARIZARD && forms[i].param1 == ITEM_CHARIZARDITE_X)
                 || (species == SPECIES_MEWTWO && forms[i].param1 == ITEM_MEWTWONITE_X))
                    continue; // These sets use their special-attacking Mega forms.
                return forms[i].param1;
            }
    return ITEM_NONE;
}

static enum Species AdventureBattleSpecies(enum Species species)
{
    u16 stone = AdventureMegaStone(species);
    const struct FormChange *forms = GetSpeciesFormChanges(species);
    if (stone != ITEM_NONE && forms != NULL)
        for (u32 i = 0; forms[i].method != FORM_CHANGE_TERMINATOR; i++)
            if (forms[i].method == FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM && forms[i].param1 == stone)
                return forms[i].targetSpecies;
    return species;
}

static bool32 AdventureHasGMax(enum Species species)
{
    enum Species target = GetGMaxTargetSpecies(species);
    return target != species && target != SPECIES_NONE && IsSpeciesEnabled(target) && gSpeciesInfo[target].isGigantamax;
}

u32 DynamaxAdventure_GetRentalPoolCount(void)
{
    u32 count = 0;
    for (u32 i = 0; i < ARRAY_COUNT(sRentals); i++)
        if (AdventureMegaStone(sRentals[i].species) || AdventureHasGMax(sRentals[i].species))
            count++;
    return count;
}

static const struct AdventureSet *GetRentalSet(u32 index)
{
    for (u32 i = 0; i < ARRAY_COUNT(sRentals); i++)
        if (AdventureMegaStone(sRentals[i].species) || AdventureHasGMax(sRentals[i].species))
            if (index-- == 0)
                return &sRentals[i];
    return NULL;
}

enum Species DynamaxAdventure_GetRentalPoolSpecies(u32 index)
{
    const struct AdventureSet *set = GetRentalSet(index);
    return set == NULL ? SPECIES_NONE : set->species;
}

bool32 DynamaxAdventure_IsBossSpecies(enum Species species)
{
    if (species == SPECIES_NONE || species >= NUM_SPECIES || !IsSpeciesEnabled(species))
        return FALSE;
    const struct SpeciesInfo *info = &gSpeciesInfo[species];
    if (!(info->isRestrictedLegendary || info->isSubLegendary || info->isMythical))
        return FALSE;
    // One ordinary form per species, plus independently catchable regional forms.
    return !info->isMegaEvolution && !info->isPrimalReversion && !info->isUltraBurst
        && !info->isGigantamax && !info->isTeraForm
        && (GET_BASE_SPECIES_ID(species) == species || info->isGalarianForm
            || info->isHisuianForm || info->isAlolanForm || info->isPaldeanForm);
}

u32 DynamaxAdventure_GetBossPoolCount(void)
{
    u32 count = 0;
    for (enum Species species = 1; species < NUM_SPECIES; species++)
        if (DynamaxAdventure_IsBossSpecies(species))
            count++;
    return count;
}

enum Species DynamaxAdventure_GetBossPoolSpecies(u32 index)
{
    for (enum Species species = 1; species < NUM_SPECIES; species++)
        if (DynamaxAdventure_IsBossSpecies(species) && index-- == 0)
            return species;
    return SPECIES_NONE;
}

struct DynamaxAdventure
{
    struct DynamaxAdventureStatus status;
    u32 rng;
    struct Pokemon rentals[DA_RENTAL_CHOICES];
    struct Pokemon paths[DA_PATH_CHOICES];
    struct Pokemon team[DA_TEAM_SIZE];
    u16 selectedOffers;
    struct Pokemon encounter;
    struct Pokemon caught[DA_ENCOUNTERS];
    struct Pokemon originalParty[PARTY_SIZE];
    u8 originalPartyCount;
    struct Pokemon originalPartnerParty[PARTY_SIZE];
    u8 originalPartnerCount;
    u16 originalPartnerId;
    u16 originalOpponentB;
    u16 bossSpecies;
    u8 difficulty;
    u8 partnerOrder[DA_TEAM_SIZE - 1];
    u8 partnerGimmicks[DA_TEAM_SIZE - 1];
    u8 hpNumerator;
    u8 hpDenominator;
    u16 rentalItems[DA_TEAM_SIZE];
    u8 rentalAbilities[DA_TEAM_SIZE];
};

// Transient by design: no SaveBlock fields, story flags, or facility records.
// Battle/field initialization resets the heap, so run data must live outside it.
static EWRAM_DATA struct DynamaxAdventure sAdventureData = {0};
static EWRAM_DATA struct DynamaxAdventure *sAdventure = NULL;

static u32 NextRandom(void)
{
    sAdventure->rng = sAdventure->rng * 1664525u + 1013904223u;
    return sAdventure->rng >> 16;
}

static void HealRental(struct Pokemon *mon)
{
    u16 hp;
    u32 status = 0;
    CalculateMonStats(mon);
    hp = GetMonData(mon, MON_DATA_MAX_HP);
    SetMonData(mon, MON_DATA_HP, &hp);
    SetMonData(mon, MON_DATA_STATUS, &status);
    MonRestorePP(mon);
}

static void TrainMon(struct Pokemon *mon, bool32 support)
{
    enum Species species = GetMonData(mon, MON_DATA_SPECIES);
    u32 physicalMoves = 0, specialMoves = 0;
    bool32 bodyPress = FALSE;
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
    {
        enum Move move = GetMonData(mon, MON_DATA_MOVE1 + i);
        bodyPress |= move == MOVE_BODY_PRESS;
        if (GetMoveCategory(move) == DAMAGE_CATEGORY_PHYSICAL) physicalMoves++;
        if (GetMoveCategory(move) == DAMAGE_CATEGORY_SPECIAL) specialMoves++;
    }
    bool32 physical = physicalMoves == specialMoves ? GetSpeciesBaseAttack(species) >= GetSpeciesBaseSpAttack(species) : physicalMoves > specialMoves;
    bool32 slow = GetSpeciesBaseSpeed(species) <= 60;
    if (slow)
    {
        u8 zero = 0;
        SetMonData(mon, MON_DATA_SPEED_IV, &zero);
    }
    if (physicalMoves == 0 && specialMoves != 0)
    {
        u8 zero = 0;
        SetMonData(mon, MON_DATA_ATK_IV, &zero);
    }
    u8 evs[NUM_STATS] = {0};
    u8 nature;
    if (support)
    {
        bool32 defense = GetSpeciesBaseDefense(species) <= GetSpeciesBaseSpDefense(species);
        evs[STAT_HP] = 252;
        evs[defense ? STAT_DEF : STAT_SPDEF] = 252;
        evs[defense ? STAT_SPDEF : STAT_DEF] = 4;
        nature = defense ? (slow ? NATURE_RELAXED : physical ? NATURE_IMPISH : NATURE_BOLD) : (slow ? NATURE_SASSY : physical ? NATURE_CAREFUL : NATURE_CALM);
    }
    else
    {
        evs[physical ? STAT_ATK : STAT_SPATK] = 252;
        evs[slow ? STAT_HP : STAT_SPEED] = 252;
        evs[slow ? STAT_SPDEF : STAT_HP] = 4;
        nature = physical ? (slow ? NATURE_BRAVE : NATURE_JOLLY) : (slow ? NATURE_QUIET : NATURE_TIMID);
    }
    if (bodyPress)
    {
        memset(evs, 0, sizeof(evs));
        evs[STAT_HP] = 252;
        evs[STAT_DEF] = 252;
        evs[STAT_SPDEF] = 4;
        nature = slow ? NATURE_RELAXED : NATURE_IMPISH;
    }
    for (u32 stat = 0; stat < NUM_STATS; stat++)
        SetMonData(mon, MON_DATA_HP_EV + stat, &evs[stat]);
    SetMonData(mon, MON_DATA_HIDDEN_NATURE, &nature);
}

static void CreateRental(struct Pokemon *mon, const struct AdventureSet *set, u32 level)
{
    u32 personality = NextRandom() << 16;
    personality |= NextRandom();
    u8 dynamaxLevel = 10;
    u8 shiny = NextRandom() % DA_SHINY_DENOMINATOR == 0;
    u8 friendship = 255;
    CreateMonWithIVs(mon, set->species, level, personality, OTID_STRUCT_PLAYER_ID, 31);
    if (set->moves[0] != MOVE_NONE)
        for (u32 i = 0; i < MAX_MON_MOVES; i++)
            SetMonMoveSlot(mon, set->moves[i], i);
    u32 attacks = 0;
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
    {
        enum Move move = GetMonData(mon, MON_DATA_MOVE1 + i);
        if (move != MOVE_NONE && GetMoveCategory(move) != DAMAGE_CATEGORY_STATUS)
            attacks++;
    }
    if (attacks < 3)
        ApplyBriskStrongLoadout(mon);
    BriskSetBestAbility(mon);
    for (u32 slot = 0; slot < NUM_ABILITY_SLOTS; slot++)
        if (set->ability != ABILITY_NONE && GetAbilityBySpecies(set->species, slot) == set->ability)
            SetMonData(mon, MON_DATA_ABILITY_NUM, &slot);
    SetMonData(mon, MON_DATA_FRIENDSHIP, &friendship);
    TrainMon(mon, FALSE);
    u16 stone = AdventureMegaStone(set->species);
    u16 item = stone != ITEM_NONE ? stone : set->item != ITEM_NONE ? set->item : set->support ? ITEM_SITRUS_BERRY : ITEM_LIFE_ORB;
    SetMonData(mon, MON_DATA_HELD_ITEM, &item);
    u8 gmax = stone == ITEM_NONE && AdventureHasGMax(set->species);
    SetMonData(mon, MON_DATA_GIGANTAMAX_FACTOR, &gmax);
    SetMonData(mon, MON_DATA_DYNAMAX_LEVEL, &dynamaxLevel);
    // Roll once per generated Pokemon; catching and swapping preserve it.
    SetMonData(mon, MON_DATA_IS_SHINY, &shiny);
    HealRental(mon);
}

static void CreateBoss(struct Pokemon *mon)
{
    const struct AdventureSet fallback = {.species = sAdventure->bossSpecies};
    const struct AdventureSet *set = &fallback;
    for (u32 i = 0; i < ARRAY_COUNT(sBossSets); i++)
        if (sBossSets[i].species == sAdventure->bossSpecies)
            set = &sBossSets[i];
    CreateRental(mon, set, DA_BOSS_LEVEL);
    if (set == &fallback)
        ApplyBriskStrongLoadout(mon);
    // Cosmog and Cosmoem have no damaging learnset; give them a usable raid set.
    bool32 hasAttack = FALSE;
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
    {
        enum Move move = GetMonData(mon, MON_DATA_MOVE1 + i);
        if (move != MOVE_NONE && GetMoveCategory(move) != DAMAGE_CATEGORY_STATUS)
            hasAttack = TRUE;
    }
    if (!hasAttack || sAdventure->bossSpecies == SPECIES_COSMOG || sAdventure->bossSpecies == SPECIES_COSMOEM)
    {
        static const u16 moves[] = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_ENERGY_BALL, MOVE_PROTECT};
        for (u32 i = 0; i < MAX_MON_MOVES; i++)
            SetMonMoveSlot(mon, moves[i], i);
    }
    enum Species species = sAdventure->bossSpecies;
    bool32 bulky = species == SPECIES_CRESSELIA || species == SPECIES_UXIE || species == SPECIES_LUGIA
                || species == SPECIES_WO_CHIEN || species == SPECIES_TING_LU;
    TrainMon(mon, bulky);
    HealRental(mon);
}

#if TESTING
bool32 Test_DynamaxAdventure_CreateBoss(struct Pokemon *mon, enum Species species)
{
    if (sAdventure == NULL || !DynamaxAdventure_IsBossSpecies(species))
        return FALSE;
    bool32 curated = FALSE;
    for (u32 i = 0; i < ARRAY_COUNT(sBossSets); i++)
        curated |= sBossSets[i].species == species;
    if (!curated)
        return FALSE;
    u16 previous = sAdventure->bossSpecies;
    sAdventure->bossSpecies = species;
    CreateBoss(mon);
    sAdventure->bossSpecies = previous;
    return TRUE;
}
#endif

// Temporary rentals and rewards must retain the same ownership as the real party.
void DynamaxAdventure_UpdatePlayerName(const u8 *oldName, const u8 *newName)
{
    if (sAdventure == NULL)
        return;
    for (u32 i = 0; i < DA_RENTAL_CHOICES; i++)
        RenamePlayerOwnedBoxMon(&sAdventure->rentals[i].box, oldName, newName);
    for (u32 i = 0; i < DA_PATH_CHOICES; i++)
        RenamePlayerOwnedBoxMon(&sAdventure->paths[i].box, oldName, newName);
    for (u32 i = 0; i < DA_TEAM_SIZE; i++)
        RenamePlayerOwnedBoxMon(&sAdventure->team[i].box, oldName, newName);
    for (u32 i = 0; i < DA_ENCOUNTERS; i++)
        RenamePlayerOwnedBoxMon(&sAdventure->caught[i].box, oldName, newName);
    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        RenamePlayerOwnedBoxMon(&sAdventure->originalParty[i].box, oldName, newName);
        RenamePlayerOwnedBoxMon(&sAdventure->originalPartnerParty[i].box, oldName, newName);
    }
    RenamePlayerOwnedBoxMon(&sAdventure->encounter.box, oldName, newName);
}

static void GeneratePaths(void)
{
    if (sAdventure->status.stage == DA_ENCOUNTERS - 1)
    {
        CreateBoss(&sAdventure->paths[0]);
        sAdventure->paths[1] = sAdventure->paths[0];
    }
    else
    {
        // One Mega route and one G-Max route, with distinct species.
        for (u32 branch = 0; branch < DA_PATH_CHOICES; branch++)
        {
            u32 count = DynamaxAdventure_GetRentalPoolCount();
            u32 start = NextRandom() % count;
            const struct AdventureSet *set = GetRentalSet(start);
            for (u32 i = 0; i < count; i++)
            {
                const struct AdventureSet *candidate = GetRentalSet((start + i) % count);
                bool32 mega = AdventureMegaStone(candidate->species) != ITEM_NONE;
                if (mega == (branch == 0)) { set = candidate; break; }
            }
            CreateRental(&sAdventure->paths[branch], set, DA_RENTAL_LEVEL);
        }
    }
    sAdventure->status.phase = DA_PHASE_PATH;
}

bool32 DynamaxAdventure_Begin(u32 seed)
{
    bool8 used[ARRAY_COUNT(sRentals)] = {0};
    if (sAdventure != NULL || gMain.inBattle)
        return FALSE;
    u32 bossCount = DynamaxAdventure_GetBossPoolCount();
    if (bossCount == 0)
        return FALSE;
    memset(&sAdventureData, 0, sizeof(sAdventureData));
    sAdventure = &sAdventureData;
    sAdventure->rng = seed;
    sAdventure->difficulty = GetBriskDifficulty();
    sAdventure->hpNumerator = GetBriskDifficulty() == BRISK_MODE_EASY ? 4 : GetBriskDifficulty() == BRISK_MODE_NORMAL ? 2 : 1;
    sAdventure->hpDenominator = GetBriskDifficulty() == BRISK_MODE_EXTREME ? 2 : 1;
    sAdventure->status.phase = DA_PHASE_RENTAL;
    sAdventure->status.lives = DA_STARTING_LIVES;
    sAdventure->bossSpecies = DynamaxAdventure_GetBossPoolSpecies(NextRandom() % bossCount);
    // Sample across the entire eligible pool, rather than a contiguous block.
    // The run seed still makes the full choice list deterministic.
    u32 poolCount = DynamaxAdventure_GetRentalPoolCount();
    for (u32 i = 0; i < DA_RENTAL_CHOICES; i++)
    {
        u32 choice = NextRandom() % poolCount;
        while (used[choice])
            choice = (choice + 1) % poolCount;
        used[choice] = TRUE;
        CreateRental(&sAdventure->rentals[i], GetRentalSet(choice), DA_RENTAL_LEVEL);
    }
    return TRUE;
}

struct DynamaxAdventureStatus DynamaxAdventure_GetStatus(void)
{
    if (sAdventure == NULL)
        return (struct DynamaxAdventureStatus){0};
    return sAdventure->status;
}

struct Pokemon *DynamaxAdventure_GetRental(u32 choice)
{
    if (sAdventure == NULL || sAdventure->status.phase != DA_PHASE_RENTAL || choice >= DA_RENTAL_CHOICES
     || (sAdventure->selectedOffers & (1u << choice)))
        return NULL;
    return &sAdventure->rentals[choice];
}

struct Pokemon *DynamaxAdventure_GetPath(u32 choice)
{
    if (sAdventure == NULL || sAdventure->status.phase != DA_PHASE_PATH || choice >= DA_PATH_CHOICES)
        return NULL;
    return &sAdventure->paths[choice];
}

struct Pokemon *DynamaxAdventure_GetCaught(u32 choice)
{
    if (sAdventure == NULL || choice >= sAdventure->status.caughtCount || IsDynamaxAdventureBattle())
        return NULL;
    return &sAdventure->caught[choice];
}

bool32 DynamaxAdventure_ChooseRental(u32 choice)
{
    struct Pokemon *mon = DynamaxAdventure_GetRental(choice);
    if (mon == NULL)
        return FALSE;
    sAdventure->team[0] = *mon;
    sAdventure->selectedOffers = 1u << choice;
    // Each researcher selects a different rental without player input.
    for (u32 member = 1; member < DA_TEAM_SIZE; member++)
    {
        u32 offer = NextRandom() % DA_RENTAL_CHOICES;
        while (sAdventure->selectedOffers & (1u << offer))
            offer = (offer + 1) % DA_RENTAL_CHOICES;
        sAdventure->team[member] = sAdventure->rentals[offer];
        sAdventure->selectedOffers |= 1u << offer;
    }
    sAdventure->status.rentalsChosen = DA_TEAM_SIZE;
    for (u32 i = 0; i < DA_TEAM_SIZE; i++)
    {
        HealRental(&sAdventure->team[i]);
        sAdventure->rentalItems[i] = GetMonData(&sAdventure->team[i], MON_DATA_HELD_ITEM);
        sAdventure->rentalAbilities[i] = GetMonData(&sAdventure->team[i], MON_DATA_ABILITY_NUM);
    }
    GeneratePaths();
    return TRUE;
}

bool32 DynamaxAdventure_ChoosePath(u32 choice)
{
    struct Pokemon *mon = DynamaxAdventure_GetPath(choice);
    if (mon == NULL)
        return FALSE;
    sAdventure->encounter = *mon;
    sAdventure->status.phase = DA_PHASE_READY;
    return TRUE;
}

u32 DynamaxAdventure_GetBossHPMultiplier(void)
{
    return IsDynamaxAdventureBattle() ? (sAdventure->status.stage == DA_ENCOUNTERS - 1 ? 7 : 5) : 1;
}

bool32 IsDynamaxAdventureBattle(void)
{
    return sAdventure != NULL && sAdventure->status.phase == DA_PHASE_BATTLE;
}

u16 DynamaxAdventure_GetRivalPartnerId(void)
{
    return gSaveBlock2Ptr->playerGender == FEMALE ? TRAINER_PARTNER(PARTNER_BRENDAN) : TRAINER_PARTNER(PARTNER_MAY);
}

bool32 DynamaxAdventure_PrepareBattle(void)
{
    if (sAdventure == NULL || sAdventure->status.phase != DA_PHASE_READY || gMain.inBattle)
        return FALSE;
    DynamaxAdventure_ResetRaidHud();
    memcpy(sAdventure->originalParty, gParties[B_TRAINER_PLAYER], sizeof(sAdventure->originalParty));
    sAdventure->originalPartyCount = gPartiesCount[B_TRAINER_PLAYER];
    memcpy(sAdventure->originalPartnerParty, gParties[B_TRAINER_PARTNER], sizeof(sAdventure->originalPartnerParty));
    sAdventure->originalPartnerCount = gPartiesCount[B_TRAINER_PARTNER];
    sAdventure->originalPartnerId = gPartnerTrainerId;
    sAdventure->originalOpponentB = TRAINER_BATTLE_PARAM.opponentB;
    ZeroPartyMons(gParties[B_TRAINER_PLAYER]);
    ZeroPartyMons(gParties[B_TRAINER_PARTNER]);
    gParties[B_TRAINER_PLAYER][0] = sAdventure->team[0];
    memset(sAdventure->partnerGimmicks, 0, sizeof(sAdventure->partnerGimmicks));
    // All three researchers are active, with stable party slots across rounds.
    for (u32 i = 0; i < DA_TEAM_SIZE - 1; i++)
    {
        u32 member = 1 + i;
        sAdventure->partnerOrder[i] = member;
        gParties[B_TRAINER_PARTNER][i] = sAdventure->team[member];
    }
    gPartiesCount[B_TRAINER_PLAYER] = 1;
    gPartiesCount[B_TRAINER_PARTNER] = DA_TEAM_SIZE - 1;
    gPartnerTrainerId = DynamaxAdventure_GetRivalPartnerId();
    TRAINER_BATTLE_PARAM.opponentB = 0xFFFF;
    ZeroEnemyPartyMons();
    gParties[B_TRAINER_OPPONENT_A][0] = sAdventure->encounter;
    enum Species species = GetMonData(&sAdventure->encounter, MON_DATA_SPECIES);
    enum Species gmax = GetGMaxTargetSpecies(species);
    u8 factor = AdventureMegaStone(species) == ITEM_NONE && gmax != species && gmax != SPECIES_NONE && IsSpeciesEnabled(gmax) && gSpeciesInfo[gmax].isGigantamax;
    SetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_GIGANTAMAX_FACTOR, &factor);
    gPartiesCount[B_TRAINER_OPPONENT_A] = 1;
    sAdventure->status.phase = DA_PHASE_BATTLE;
    return TRUE;
}

bool32 DynamaxAdventure_FinishBattle(u32 outcome)
{
    if (!IsDynamaxAdventureBattle())
        return FALSE;
    sAdventure->team[0] = gParties[B_TRAINER_PLAYER][0];
    for (u32 i = 0; i < DA_TEAM_SIZE - 1; i++)
        sAdventure->team[sAdventure->partnerOrder[i]] = gParties[B_TRAINER_PARTNER][i];
    for (u32 i = 0; i < DA_TEAM_SIZE; i++)
    {
        enum Species species = GetMonData(&sAdventure->team[i], MON_DATA_SPECIES);
        if (gSpeciesInfo[species].isMegaEvolution || gSpeciesInfo[species].isGigantamax)
        {
            const struct FormChange *forms = GetSpeciesFormChanges(species);
            for (u32 j = 0; forms != NULL && forms[j].method != FORM_CHANGE_TERMINATOR; j++)
                if (forms[j].method == FORM_CHANGE_END_BATTLE)
                {
                    species = forms[j].targetSpecies;
                    SetMonData(&sAdventure->team[i], MON_DATA_SPECIES, &species);
                    break;
                }
        }
        SetMonData(&sAdventure->team[i], MON_DATA_ABILITY_NUM, &sAdventure->rentalAbilities[i]);
        SetMonData(&sAdventure->team[i], MON_DATA_HELD_ITEM, &sAdventure->rentalItems[i]);
        CalculateMonStats(&sAdventure->team[i]);
    }
    memcpy(gParties[B_TRAINER_PLAYER], sAdventure->originalParty, sizeof(sAdventure->originalParty));
    gPartiesCount[B_TRAINER_PLAYER] = sAdventure->originalPartyCount;
    memcpy(gParties[B_TRAINER_PARTNER], sAdventure->originalPartnerParty, sizeof(sAdventure->originalPartnerParty));
    gPartiesCount[B_TRAINER_PARTNER] = sAdventure->originalPartnerCount;
    gPartnerTrainerId = sAdventure->originalPartnerId;
    TRAINER_BATTLE_PARAM.opponentB = sAdventure->originalOpponentB;
    // Recover after every completed battle; only Easy cures status conditions.
    for (u32 i = 0; i < DA_TEAM_SIZE; i++)
    {
        struct Pokemon *mon = &sAdventure->team[i];
        u32 maxHP = GetMonData(mon, MON_DATA_MAX_HP);
        u32 heal = sAdventure->difficulty <= BRISK_MODE_NORMAL ? maxHP
                 : max(1, maxHP / (sAdventure->difficulty == BRISK_MODE_HARD ? 2 : 4));
        u16 hp = min(maxHP, GetMonData(mon, MON_DATA_HP) + heal);
        SetMonData(mon, MON_DATA_HP, &hp);
        if (sAdventure->difficulty == BRISK_MODE_EASY)
        {
            u32 status = 0;
            SetMonData(mon, MON_DATA_STATUS, &status);
        }
        MonRestorePP(mon);
    }
    if (outcome == B_OUTCOME_WON)
    {
        sAdventure->status.phase = DA_PHASE_CATCH;
    }
    else if (outcome == B_OUTCOME_LOST || outcome == B_OUTCOME_DREW)
    {
        sAdventure->status.lives--;
        sAdventure->status.phase = sAdventure->status.lives ? DA_PHASE_READY : DA_PHASE_FINISHED;
    }
    else
    {
        // Running/teleport/abort ends the run; previous catches remain selectable.
        sAdventure->status.phase = DA_PHASE_FINISHED;
    }
    return TRUE;
}

static void Advance(void)
{
    sAdventure->status.stage++;
    if (sAdventure->status.stage == DA_ENCOUNTERS)
    {
        sAdventure->status.cleared = TRUE;
        sAdventure->status.phase = DA_PHASE_FINISHED;
    }
    else
    {
        GeneratePaths();
    }
}

bool32 DynamaxAdventure_Catch(bool32 accept)
{
    if (sAdventure == NULL || sAdventure->status.phase != DA_PHASE_CATCH)
        return FALSE;
    if (accept)
    {
        struct Pokemon *mon = &sAdventure->caught[sAdventure->status.caughtCount++];
        *mon = sAdventure->encounter;
        HealRental(mon);
        if (sAdventure->status.stage < DA_ENCOUNTERS - 1)
        {
            sAdventure->status.phase = DA_PHASE_SWAP;
            return TRUE;
        }
    }
    Advance();
    return TRUE;
}

bool32 DynamaxAdventure_SwapRental(u32 slot)
{
    if (sAdventure == NULL || sAdventure->status.phase != DA_PHASE_SWAP
     || (slot != 0 && slot != DA_RENTAL_KEEP))
        return FALSE;
    // If the player declines, one randomly selected teammate may take it.
    if (slot == DA_RENTAL_KEEP && NextRandom() % 2 == 0)
        slot = 1 + NextRandom() % (DA_TEAM_SIZE - 1);
    if (slot != DA_RENTAL_KEEP)
    {
        // Keep the same encounter, including its original shiny roll.
        sAdventure->team[slot] = sAdventure->encounter;
        HealRental(&sAdventure->team[slot]);
        sAdventure->rentalItems[slot] = GetMonData(&sAdventure->team[slot], MON_DATA_HELD_ITEM);
        sAdventure->rentalAbilities[slot] = GetMonData(&sAdventure->team[slot], MON_DATA_ABILITY_NUM);
    }
    Advance();
    return TRUE;
}

bool32 DynamaxAdventure_Swap(bool32 accept)
{
    return DynamaxAdventure_SwapRental(accept ? 0 : DA_RENTAL_KEEP);
}

bool32 DynamaxAdventure_Claim(u32 choice)
{
    struct Pokemon *mon = DynamaxAdventure_GetCaught(choice);
    struct Pokemon reward;
    if (mon == NULL || sAdventure->status.phase != DA_PHASE_FINISHED || gMain.inBattle)
        return FALSE;
    reward = *mon;
    if (GiveCapturedMonToPlayer(&reward) == MON_CANT_GIVE)
        return FALSE; // Keep the run/reward available if party and PC are full.
    HandleSetPokedexFlagFromMon(&reward, FLAG_SET_SEEN);
    HandleSetPokedexFlagFromMon(&reward, FLAG_SET_CAUGHT);
    sAdventure = NULL;
    return TRUE;
}

bool32 DynamaxAdventure_Quit(void)
{
    if (sAdventure == NULL || IsDynamaxAdventureBattle() || gMain.inBattle)
        return FALSE;
    sAdventure = NULL;
    return TRUE;
}

static const u8 sRewardShiny[] = _("Shiny");
static const u8 sRewardRegular[] = _("Regular");

static bool32 BufferMon(struct Pokemon *mon)
{
    if (mon == NULL)
        return FALSE;
    StringCopy(gStringVar1, GetSpeciesName(GetMonData(mon, MON_DATA_SPECIES)));
    gSpecialVar_0x8000 = GetMonData(mon, MON_DATA_SPECIES);
    gSpecialVar_0x8001 = GetMonData(mon, MON_DATA_LEVEL);
    return TRUE;
}

void RunDynamaxAdventure(void)
{
    u32 op = gSpecialVar_0x8004;
    u32 arg = gSpecialVar_0x8005;
    gSpecialVar_Result = FALSE;
    switch (op)
    {
    case DA_OP_BEGIN:
        gSpecialVar_Result = DynamaxAdventure_Begin(Random32());
        break;
    case DA_OP_STATUS:
    {
        struct DynamaxAdventureStatus status = DynamaxAdventure_GetStatus();
        gSpecialVar_0x8000 = status.phase;
        gSpecialVar_0x8001 = status.stage;
        gSpecialVar_0x8002 = status.lives;
        gSpecialVar_0x8003 = status.caughtCount;
        gSpecialVar_0x8006 = status.cleared;
        gSpecialVar_0x8007 = status.rentalsChosen;
        gSpecialVar_Result = TRUE;
        break;
    }
    case DA_OP_RENTAL_INFO: gSpecialVar_Result = BufferMon(DynamaxAdventure_GetRental(arg)); break;
    case DA_OP_CHOOSE_RENTAL: gSpecialVar_Result = DynamaxAdventure_ChooseRental(arg); break;
    case DA_OP_PATH_INFO:
    {
        struct Pokemon *mon = DynamaxAdventure_GetPath(arg);
        if (mon != NULL)
        {
            enum Species species = AdventureBattleSpecies(GetMonData(mon, MON_DATA_SPECIES));
            StringCopy(gStringVar1, gTypesInfo[gSpeciesInfo[species].types[0]].name);
            gSpecialVar_Result = TRUE;
        }
        break;
    }
    case DA_OP_CHOOSE_PATH: gSpecialVar_Result = DynamaxAdventure_ChoosePath(arg); break;
    case DA_OP_START_BATTLE: gSpecialVar_Result = BattleSetup_StartDynamaxAdventureBattle(); break;
    case DA_OP_CATCH: gSpecialVar_Result = DynamaxAdventure_Catch(arg != 0); break;
    case DA_OP_SWAP: gSpecialVar_Result = DynamaxAdventure_SwapRental(arg == 0 ? DA_RENTAL_KEEP : arg - 1); break;
    case DA_OP_REWARD_INFO:
        if (DynamaxAdventure_GetStatus().phase == DA_PHASE_FINISHED)
        {
            struct Pokemon *mon = DynamaxAdventure_GetCaught(arg);
            gSpecialVar_Result = BufferMon(mon);
            if (mon != NULL)
                StringCopy(gStringVar2, GetMonData(mon, MON_DATA_IS_SHINY) ? sRewardShiny : sRewardRegular);
        }
        break;
    case DA_OP_CLAIM: gSpecialVar_Result = DynamaxAdventure_Claim(arg); break;
    case DA_OP_QUIT: gSpecialVar_Result = DynamaxAdventure_Quit(); break;
    case DA_OP_BOSS_HINT:
        if (sAdventure != NULL)
        {
            StringCopy(gStringVar1, gTypesInfo[gSpeciesInfo[AdventureBattleSpecies(sAdventure->bossSpecies)].types[0]].name);
            gSpecialVar_Result = TRUE;
        }
        break;
    case DA_OP_IN_REST_CAVE:
        gSpecialVar_Result = gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_DYNAMAX_ADVENTURE_REST_CAVE)
                         && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_DYNAMAX_ADVENTURE_REST_CAVE);
        break;
    }
}

u32 DynamaxAdventure_ScaleMaxHP(struct Pokemon *mon, u32 hp)
{
    if (sAdventure == NULL)
        return hp;
    bool32 rental = FALSE;
    for (u32 i = 0; i < DA_TEAM_SIZE; i++)
        rental |= mon == &sAdventure->team[i];
    if (IsDynamaxAdventureBattle())
    {
        rental |= mon == &gParties[B_TRAINER_PLAYER][0];
        for (u32 i = 0; i < DA_TEAM_SIZE - 1; i++)
            rental |= mon == &gParties[B_TRAINER_PARTNER][i];
    }
    if (rental)
        return max(1, hp * sAdventure->hpNumerator / sAdventure->hpDenominator);
    if (IsDynamaxAdventureBattle() && mon == &gParties[B_TRAINER_OPPONENT_A][0]
     && gSpeciesInfo[GetMonData(mon, MON_DATA_SPECIES)].isMegaEvolution)
        return hp * DynamaxAdventure_GetBossHPMultiplier();
    return hp;
}

u32 DynamaxAdventure_GetBossDamageMultiplier(void)
{
    return IsDynamaxAdventureBattle() && sAdventure->difficulty == BRISK_MODE_EXTREME ? 2 : 1;
}

const u8 *DynamaxAdventure_GetPartnerName(u32 slot)
{
    if (!IsDynamaxAdventureBattle() || slot >= DA_TEAM_SIZE - 1)
        return NULL;
    switch (sAdventure->partnerOrder[slot])
    {
    case 2: return COMPOUND_STRING("SCIENTIST");
    case 3: return COMPOUND_STRING("BIRCH");
    default: return gSaveBlock2Ptr->playerGender == FEMALE ? COMPOUND_STRING("BRENDAN") : COMPOUND_STRING("MAY");
    }
}

void DynamaxAdventure_SwitchPartner(enum BattlerId battler, u32 newSlot)
{
    if (!IsDynamaxAdventureBattle() || GetBattlerPosition(battler) != B_POSITION_PLAYER_RIGHT)
        return;
    u32 oldSlot = gBattlerPartyIndexes[battler];
    if (oldSlot >= DA_TEAM_SIZE - 1 || newSlot >= DA_TEAM_SIZE - 1)
        return;
    for (u32 gimmick = 0; gimmick < GIMMICKS_COUNT; gimmick++)
    {
        if (gBattleStruct->gimmick.activated[battler][gimmick])
            sAdventure->partnerGimmicks[oldSlot] |= 1u << gimmick;
        gBattleStruct->gimmick.activated[battler][gimmick] = (sAdventure->partnerGimmicks[newSlot] & (1u << gimmick)) != 0;
    }
}
