#include "global.h"
#include "dynamax_adventure.h"
#include "battle.h"
#include "battle_main.h"
#include "battle_setup.h"
#include "battle_partner.h"
#include "constants/opponents.h"
#include "brisk_trainer_moves.h"
#include "data.h"
#include "move.h"
#include "event_data.h"
#include "main.h"
#include "pokemon_storage_system.h"
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
};

#include "data/dynamax_adventure.h"

u32 DynamaxAdventure_GetRentalPoolCount(void)
{
    return ARRAY_COUNT(sRentals);
}

enum Species DynamaxAdventure_GetRentalPoolSpecies(u32 index)
{
    return index < ARRAY_COUNT(sRentals) ? sRentals[index].species : SPECIES_NONE;
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
    u8 selectedOffers;
    struct Pokemon encounter;
    struct Pokemon caught[DA_ENCOUNTERS];
    struct Pokemon originalParty[PARTY_SIZE];
    u8 originalPartyCount;
    struct Pokemon originalPartnerParty[PARTY_SIZE];
    u8 originalPartnerCount;
    u16 originalPartnerId;
    u16 originalOpponentB;
    u16 bossSpecies;
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
    bool32 physical = GetSpeciesBaseAttack(species) >= GetSpeciesBaseSpAttack(species);
    bool32 slow = GetSpeciesBaseSpeed(species) <= 60;
    u8 evs[NUM_STATS] = {0};
    u8 nature;
    if (support)
    {
        bool32 defense = GetSpeciesBaseDefense(species) <= GetSpeciesBaseSpDefense(species);
        evs[STAT_HP] = 252;
        evs[defense ? STAT_DEF : STAT_SPDEF] = 252;
        evs[defense ? STAT_SPDEF : STAT_DEF] = 4;
        nature = defense ? (physical ? NATURE_IMPISH : NATURE_BOLD) : (physical ? NATURE_CAREFUL : NATURE_CALM);
    }
    else
    {
        evs[physical ? STAT_ATK : STAT_SPATK] = 252;
        evs[slow ? STAT_HP : STAT_SPEED] = 252;
        evs[slow ? STAT_SPDEF : STAT_HP] = 4;
        nature = physical ? (slow ? NATURE_ADAMANT : NATURE_JOLLY) : (slow ? NATURE_MODEST : NATURE_TIMID);
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
    u8 shiny = FALSE;
    CreateMonWithIVs(mon, set->species, level, personality, OTID_STRUCT_PLAYER_ID, 31);
    if (set->moves[0] != MOVE_NONE)
        for (u32 i = 0; i < MAX_MON_MOVES; i++)
            SetMonMoveSlot(mon, set->moves[i], i);
    BriskSetBestAbility(mon);
    for (u32 slot = 0; slot < NUM_ABILITY_SLOTS; slot++)
        if (set->ability != ABILITY_NONE && GetAbilityBySpecies(set->species, slot) == set->ability)
            SetMonData(mon, MON_DATA_ABILITY_NUM, &slot);
    TrainMon(mon, set->support);
    SetMonData(mon, MON_DATA_DYNAMAX_LEVEL, &dynamaxLevel);
    // Shininess is rolled only after winning, never previewed on a rental/boss.
    SetMonData(mon, MON_DATA_IS_SHINY, &shiny);
    HealRental(mon);
}

static void CreateBoss(struct Pokemon *mon)
{
    const struct AdventureSet set = {.species = sAdventure->bossSpecies};
    CreateRental(mon, &set, DA_BOSS_LEVEL);
    ApplyBriskStrongLoadout(mon);
    // Cosmog and Cosmoem have no damaging learnset; give them a usable raid set.
    bool32 hasAttack = FALSE;
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
    {
        enum Move move = GetMonData(mon, MON_DATA_MOVE1 + i);
        if (move != MOVE_NONE && GetMoveCategory(move) != DAMAGE_CATEGORY_STATUS)
            hasAttack = TRUE;
    }
    if (!hasAttack)
    {
        static const u16 moves[] = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_LIGHT_SCREEN, MOVE_PROTECT};
        for (u32 i = 0; i < MAX_MON_MOVES; i++)
            SetMonMoveSlot(mon, moves[i], i);
    }
    HealRental(mon);
}

#if TESTING
bool32 Test_DynamaxAdventure_CreateBoss(struct Pokemon *mon, enum Species species)
{
    if (sAdventure == NULL || !DynamaxAdventure_IsBossSpecies(species))
        return FALSE;
    u16 previous = sAdventure->bossSpecies;
    sAdventure->bossSpecies = species;
    CreateBoss(mon);
    sAdventure->bossSpecies = previous;
    return TRUE;
}
#endif

static void GeneratePaths(void)
{
    if (sAdventure->status.stage == DA_ENCOUNTERS - 1)
    {
        CreateBoss(&sAdventure->paths[0]);
        sAdventure->paths[1] = sAdventure->paths[0];
    }
    else
    {
        u32 first = NextRandom() % ARRAY_COUNT(sRentals);
        u32 second = (first + 1 + NextRandom() % (ARRAY_COUNT(sRentals) - 1)) % ARRAY_COUNT(sRentals);
        CreateRental(&sAdventure->paths[0], &sRentals[first], DA_RENTAL_LEVEL);
        CreateRental(&sAdventure->paths[1], &sRentals[second], DA_RENTAL_LEVEL);
    }
    sAdventure->status.phase = DA_PHASE_PATH;
}

bool32 DynamaxAdventure_Begin(u32 seed)
{
    u32 first;
    if (sAdventure != NULL || gMain.inBattle)
        return FALSE;
    u32 bossCount = DynamaxAdventure_GetBossPoolCount();
    if (bossCount == 0)
        return FALSE;
    memset(&sAdventureData, 0, sizeof(sAdventureData));
    sAdventure = &sAdventureData;
    sAdventure->rng = seed;
    sAdventure->status.phase = DA_PHASE_RENTAL;
    sAdventure->status.lives = DA_STARTING_LIVES;
    sAdventure->bossSpecies = DynamaxAdventure_GetBossPoolSpecies(NextRandom() % bossCount);
    first = NextRandom() % ARRAY_COUNT(sRentals);
    // Distinct offers, with deterministic run generation from the supplied seed.
    for (u32 i = 0; i < DA_RENTAL_CHOICES; i++)
        CreateRental(&sAdventure->rentals[i], &sRentals[(first + i * 5) % ARRAY_COUNT(sRentals)], DA_RENTAL_LEVEL);
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
    // The rival chooses independently from the remaining offers.
    u32 rival = (choice + 1 + NextRandom() % (DA_RENTAL_CHOICES - 1)) % DA_RENTAL_CHOICES;
    sAdventure->team[1] = sAdventure->rentals[rival];
    sAdventure->status.rentalsChosen = DA_TEAM_SIZE;
    sAdventure->selectedOffers = (1u << choice) | (1u << rival);
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
    memcpy(sAdventure->originalParty, gParties[B_TRAINER_PLAYER], sizeof(sAdventure->originalParty));
    sAdventure->originalPartyCount = gPartiesCount[B_TRAINER_PLAYER];
    memcpy(sAdventure->originalPartnerParty, gParties[B_TRAINER_PARTNER], sizeof(sAdventure->originalPartnerParty));
    sAdventure->originalPartnerCount = gPartiesCount[B_TRAINER_PARTNER];
    sAdventure->originalPartnerId = gPartnerTrainerId;
    sAdventure->originalOpponentB = TRAINER_BATTLE_PARAM.opponentB;
    ZeroPartyMons(gParties[B_TRAINER_PLAYER]);
    ZeroPartyMons(gParties[B_TRAINER_PARTNER]);
    gParties[B_TRAINER_PLAYER][0] = sAdventure->team[0];
    gParties[B_TRAINER_PARTNER][0] = sAdventure->team[1];
    u8 factor = FALSE;
    SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_GIGANTAMAX_FACTOR, &factor);
    SetMonData(&gParties[B_TRAINER_PARTNER][0], MON_DATA_GIGANTAMAX_FACTOR, &factor);
    gPartiesCount[B_TRAINER_PLAYER] = 1;
    gPartiesCount[B_TRAINER_PARTNER] = 1;
    gPartnerTrainerId = DynamaxAdventure_GetRivalPartnerId();
    TRAINER_BATTLE_PARAM.opponentB = 0xFFFF;
    ZeroEnemyPartyMons();
    gParties[B_TRAINER_OPPONENT_A][0] = sAdventure->encounter;
    enum Species species = GetMonData(&sAdventure->encounter, MON_DATA_SPECIES);
    enum Species gmax = GetGMaxTargetSpecies(species);
    factor = gmax != species && gmax != SPECIES_NONE && IsSpeciesEnabled(gmax) && gSpeciesInfo[gmax].isGigantamax;
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
    sAdventure->team[1] = gParties[B_TRAINER_PARTNER][0];
    memcpy(gParties[B_TRAINER_PLAYER], sAdventure->originalParty, sizeof(sAdventure->originalParty));
    gPartiesCount[B_TRAINER_PLAYER] = sAdventure->originalPartyCount;
    memcpy(gParties[B_TRAINER_PARTNER], sAdventure->originalPartnerParty, sizeof(sAdventure->originalPartnerParty));
    gPartiesCount[B_TRAINER_PARTNER] = sAdventure->originalPartnerCount;
    gPartnerTrainerId = sAdventure->originalPartnerId;
    TRAINER_BATTLE_PARAM.opponentB = sAdventure->originalOpponentB;
    if (outcome == B_OUTCOME_WON)
    {
        for (u32 i = 0; i < DA_TEAM_SIZE; i++)
            if (GetMonData(&sAdventure->team[i], MON_DATA_HP) == 0)
            {
                HealRental(&sAdventure->team[i]);
                u16 hp = GetMonData(&sAdventure->team[i], MON_DATA_MAX_HP) / 2;
                SetMonData(&sAdventure->team[i], MON_DATA_HP, &hp);
            }
        sAdventure->status.phase = DA_PHASE_CATCH;
    }
    else if (outcome == B_OUTCOME_LOST || outcome == B_OUTCOME_DREW)
    {
        sAdventure->status.lives--;
        for (u32 i = 0; i < DA_TEAM_SIZE; i++)
            HealRental(&sAdventure->team[i]);
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
        u8 shiny = NextRandom() % 300 == 0;
        *mon = sAdventure->encounter;
        SetMonData(mon, MON_DATA_IS_SHINY, &shiny);
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
    if (slot != DA_RENTAL_KEEP)
    {
        // Use the pre-shiny encounter for the rental; reveal shinies at rewards.
        sAdventure->team[slot] = sAdventure->encounter;
        HealRental(&sAdventure->team[slot]);
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
    case DA_OP_PATH_INFO: gSpecialVar_Result = BufferMon(DynamaxAdventure_GetPath(arg)); break;
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
            StringCopy(gStringVar1, gTypesInfo[gSpeciesInfo[sAdventure->bossSpecies].types[0]].name);
            gSpecialVar_Result = TRUE;
        }
        break;
    case DA_OP_IN_REST_CAVE:
        gSpecialVar_Result = gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_DYNAMAX_ADVENTURE_REST_CAVE)
                         && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_DYNAMAX_ADVENTURE_REST_CAVE);
        break;
    }
}
