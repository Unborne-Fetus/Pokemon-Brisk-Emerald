#include "global.h"
#include "dynamax_adventure.h"
#include "battle.h"
#include "battle_setup.h"
#include "event_data.h"
#include "main.h"
#include "pokemon_storage_system.h"
#include "random.h"
#include "string_util.h"
#include "constants/pokedex.h"

struct AdventureSet
{
    u16 species;
    u16 moves[MAX_MON_MOVES];
};

static const struct AdventureSet sRentals[] =
{
    {SPECIES_CHARIZARD, {MOVE_FLAMETHROWER, MOVE_AIR_SLASH, MOVE_DRAGON_PULSE, MOVE_PROTECT}},
    {SPECIES_BLASTOISE, {MOVE_SURF, MOVE_ICE_BEAM, MOVE_AURA_SPHERE, MOVE_PROTECT}},
    {SPECIES_VENUSAUR, {MOVE_ENERGY_BALL, MOVE_SLUDGE_BOMB, MOVE_SLEEP_POWDER, MOVE_PROTECT}},
    {SPECIES_RAICHU, {MOVE_THUNDERBOLT, MOVE_GRASS_KNOT, MOVE_LIGHT_SCREEN, MOVE_PROTECT}},
    {SPECIES_MACHAMP, {MOVE_DRAIN_PUNCH, MOVE_ROCK_SLIDE, MOVE_KNOCK_OFF, MOVE_BULK_UP}},
    {SPECIES_GENGAR, {MOVE_SHADOW_BALL, MOVE_SLUDGE_BOMB, MOVE_THUNDERBOLT, MOVE_PROTECT}},
    {SPECIES_CLEFABLE, {MOVE_MOONBLAST, MOVE_FLAMETHROWER, MOVE_LIFE_DEW, MOVE_PROTECT}},
    {SPECIES_FLYGON, {MOVE_DRAGON_CLAW, MOVE_EARTHQUAKE, MOVE_ROCK_SLIDE, MOVE_PROTECT}},
    {SPECIES_SCIZOR, {MOVE_BULLET_PUNCH, MOVE_X_SCISSOR, MOVE_SWORDS_DANCE, MOVE_ROOST}},
    {SPECIES_LAPRAS, {MOVE_SURF, MOVE_ICE_BEAM, MOVE_THUNDERBOLT, MOVE_LIFE_DEW}},
    {SPECIES_LYCANROC, {MOVE_ROCK_SLIDE, MOVE_CRUNCH, MOVE_ACCELEROCK, MOVE_PROTECT}},
    {SPECIES_GARDEVOIR, {MOVE_PSYCHIC, MOVE_MOONBLAST, MOVE_MYSTICAL_FIRE, MOVE_LIFE_DEW}},
    {SPECIES_SNORLAX, {MOVE_BODY_SLAM, MOVE_CRUNCH, MOVE_HIGH_HORSEPOWER, MOVE_REST}},
    {SPECIES_TORKOAL, {MOVE_LAVA_PLUME, MOVE_BODY_PRESS, MOVE_SOLAR_BEAM, MOVE_PROTECT}},
    {SPECIES_LUDICOLO, {MOVE_SURF, MOVE_GIGA_DRAIN, MOVE_ICE_BEAM, MOVE_RAIN_DANCE}},
    {SPECIES_WEAVILE, {MOVE_ICE_PUNCH, MOVE_NIGHT_SLASH, MOVE_BRICK_BREAK, MOVE_SWORDS_DANCE}},
};

static const struct AdventureSet sBosses[] =
{
    {SPECIES_ARTICUNO, {MOVE_ICE_BEAM, MOVE_HURRICANE, MOVE_ANCIENT_POWER, MOVE_REFLECT}},
    {SPECIES_ZAPDOS, {MOVE_THUNDERBOLT, MOVE_DRILL_PECK, MOVE_HEAT_WAVE, MOVE_LIGHT_SCREEN}},
    {SPECIES_MOLTRES, {MOVE_FLAMETHROWER, MOVE_AIR_SLASH, MOVE_ANCIENT_POWER, MOVE_SAFEGUARD}},
    {SPECIES_RAIKOU, {MOVE_THUNDERBOLT, MOVE_EXTRASENSORY, MOVE_SHADOW_BALL, MOVE_CALM_MIND}},
    {SPECIES_ENTEI, {MOVE_SACRED_FIRE, MOVE_CRUNCH, MOVE_STONE_EDGE, MOVE_IRON_HEAD}},
    {SPECIES_SUICUNE, {MOVE_SURF, MOVE_ICE_BEAM, MOVE_EXTRASENSORY, MOVE_CALM_MIND}},
};

struct DynamaxAdventure
{
    struct DynamaxAdventureStatus status;
    u32 rng;
    struct Pokemon rentals[DA_RENTAL_CHOICES];
    struct Pokemon paths[DA_PATH_CHOICES];
    struct Pokemon rental;
    struct Pokemon encounter;
    struct Pokemon caught[DA_ENCOUNTERS];
    struct Pokemon originalParty[PARTY_SIZE];
    u8 originalPartyCount;
    u8 boss;
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

static void CreateRental(struct Pokemon *mon, const struct AdventureSet *set, u32 level)
{
    u32 personality = NextRandom() << 16;
    personality |= NextRandom();
    u8 dynamaxLevel = 10;
    u8 shiny = FALSE;
    CreateMonWithIVs(mon, set->species, level, personality, OTID_STRUCT_PLAYER_ID, 31);
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
        SetMonMoveSlot(mon, set->moves[i], i);
    SetMonData(mon, MON_DATA_DYNAMAX_LEVEL, &dynamaxLevel);
    // Shininess is rolled only after winning, never previewed on a rental/boss.
    SetMonData(mon, MON_DATA_IS_SHINY, &shiny);
    HealRental(mon);
}

static void GeneratePaths(void)
{
    if (sAdventure->status.stage == DA_ENCOUNTERS - 1)
    {
        CreateRental(&sAdventure->paths[0], &sBosses[sAdventure->boss], DA_BOSS_LEVEL);
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
    memset(&sAdventureData, 0, sizeof(sAdventureData));
    sAdventure = &sAdventureData;
    sAdventure->rng = seed;
    sAdventure->status.phase = DA_PHASE_RENTAL;
    sAdventure->status.lives = DA_STARTING_LIVES;
    sAdventure->boss = NextRandom() % ARRAY_COUNT(sBosses);
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
    if (sAdventure == NULL || sAdventure->status.phase != DA_PHASE_RENTAL || choice >= DA_RENTAL_CHOICES)
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
    sAdventure->rental = *mon;
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

bool32 IsDynamaxAdventureBattle(void)
{
    return sAdventure != NULL && sAdventure->status.phase == DA_PHASE_BATTLE;
}

bool32 DynamaxAdventure_PrepareBattle(void)
{
    if (sAdventure == NULL || sAdventure->status.phase != DA_PHASE_READY || gMain.inBattle)
        return FALSE;
    memcpy(sAdventure->originalParty, gParties[B_TRAINER_PLAYER], sizeof(sAdventure->originalParty));
    sAdventure->originalPartyCount = gPartiesCount[B_TRAINER_PLAYER];
    memset(gParties[B_TRAINER_PLAYER], 0, sizeof(sAdventure->originalParty));
    gParties[B_TRAINER_PLAYER][0] = sAdventure->rental;
    gPartiesCount[B_TRAINER_PLAYER] = 1;
    ZeroEnemyPartyMons();
    gParties[B_TRAINER_OPPONENT_A][0] = sAdventure->encounter;
    gPartiesCount[B_TRAINER_OPPONENT_A] = 1;
    sAdventure->status.phase = DA_PHASE_BATTLE;
    return TRUE;
}

bool32 DynamaxAdventure_FinishBattle(u32 outcome)
{
    if (!IsDynamaxAdventureBattle())
        return FALSE;
    sAdventure->rental = gParties[B_TRAINER_PLAYER][0];
    memcpy(gParties[B_TRAINER_PLAYER], sAdventure->originalParty, sizeof(sAdventure->originalParty));
    gPartiesCount[B_TRAINER_PLAYER] = sAdventure->originalPartyCount;
    if (outcome == B_OUTCOME_WON)
    {
        sAdventure->status.phase = DA_PHASE_CATCH;
    }
    else if (outcome == B_OUTCOME_LOST || outcome == B_OUTCOME_DREW)
    {
        sAdventure->status.lives--;
        HealRental(&sAdventure->rental);
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

bool32 DynamaxAdventure_Swap(bool32 accept)
{
    if (sAdventure == NULL || sAdventure->status.phase != DA_PHASE_SWAP)
        return FALSE;
    if (accept)
    {
        // Use the pre-shiny encounter for the rental; reveal shinies at rewards.
        sAdventure->rental = sAdventure->encounter;
        HealRental(&sAdventure->rental);
    }
    Advance();
    return TRUE;
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
        gSpecialVar_Result = TRUE;
        break;
    }
    case DA_OP_RENTAL_INFO: gSpecialVar_Result = BufferMon(DynamaxAdventure_GetRental(arg)); break;
    case DA_OP_CHOOSE_RENTAL: gSpecialVar_Result = DynamaxAdventure_ChooseRental(arg); break;
    case DA_OP_PATH_INFO: gSpecialVar_Result = BufferMon(DynamaxAdventure_GetPath(arg)); break;
    case DA_OP_CHOOSE_PATH: gSpecialVar_Result = DynamaxAdventure_ChoosePath(arg); break;
    case DA_OP_START_BATTLE: gSpecialVar_Result = BattleSetup_StartDynamaxAdventureBattle(); break;
    case DA_OP_CATCH: gSpecialVar_Result = DynamaxAdventure_Catch(arg != 0); break;
    case DA_OP_SWAP: gSpecialVar_Result = DynamaxAdventure_Swap(arg != 0); break;
    case DA_OP_REWARD_INFO:
        if (DynamaxAdventure_GetStatus().phase == DA_PHASE_FINISHED)
            gSpecialVar_Result = BufferMon(DynamaxAdventure_GetCaught(arg));
        break;
    case DA_OP_CLAIM: gSpecialVar_Result = DynamaxAdventure_Claim(arg); break;
    case DA_OP_QUIT: gSpecialVar_Result = DynamaxAdventure_Quit(); break;
    }
}
