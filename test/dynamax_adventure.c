#include "global.h"
#include "dynamax_adventure.h"
#include "battle.h"
#include "data.h"
#include "brisk_modes.h"
#include "event_data.h"
#include "main.h"
#include "move.h"
#include "pokemon_storage_system.h"
#include "save.h"
#include "test/test.h"

static void StartRental(void)
{
    EXPECT(DynamaxAdventure_Begin(12345));
    EXPECT(DynamaxAdventure_ChooseRental(0));
    EXPECT_EQ(DynamaxAdventure_GetStatus().rentalsChosen, 1);
    EXPECT(!DynamaxAdventure_ChooseRental(0));
    EXPECT(!DynamaxAdventure_ChoosePath(0));
    EXPECT(DynamaxAdventure_ChooseRental(1));
}

TEST("Adventure: state guards reject invalid actions without changing the party")
{
    struct Pokemon original[PARTY_SIZE];
    memcpy(original, gParties[B_TRAINER_PLAYER], sizeof(original));
    EXPECT(!DynamaxAdventure_ChooseRental(0));
    EXPECT(!DynamaxAdventure_PrepareBattle());
    EXPECT(!DynamaxAdventure_Claim(0));
    EXPECT(DynamaxAdventure_Begin(1));
    EXPECT(!DynamaxAdventure_Begin(2));
    EXPECT(!DynamaxAdventure_ChooseRental(DA_RENTAL_CHOICES));
    EXPECT(!DynamaxAdventure_ChoosePath(0));
    EXPECT(!DynamaxAdventure_Catch(TRUE));
    EXPECT(!DynamaxAdventure_Swap(TRUE));
    EXPECT(!DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
    EXPECT_EQ(DynamaxAdventure_GetStatus().phase, DA_PHASE_RENTAL);
    EXPECT_EQ(memcmp(original, gParties[B_TRAINER_PLAYER], sizeof(original)), 0);
    EXPECT(DynamaxAdventure_Quit());
    EXPECT_EQ(DynamaxAdventure_GetStatus().phase, DA_PHASE_INACTIVE);
}

TEST("Adventure: seed creates distinct rentals and consistent route choices")
{
    u16 rentals[DA_RENTAL_CHOICES];
    u16 paths[DA_PATH_CHOICES];
    EXPECT(DynamaxAdventure_Begin(42));
    for (u32 i = 0; i < DA_RENTAL_CHOICES; i++)
    {
        struct Pokemon *mon = DynamaxAdventure_GetRental(i);
        rentals[i] = GetMonData(mon, MON_DATA_SPECIES);
        EXPECT_EQ(GetMonData(mon, MON_DATA_LEVEL), DA_RENTAL_LEVEL);
        EXPECT_EQ(GetMonData(mon, MON_DATA_IS_SHINY), FALSE);
        for (u32 j = 0; j < MAX_MON_MOVES; j++)
            EXPECT_NE(GetMonData(mon, MON_DATA_MOVE1 + j), MOVE_NONE);
        for (u32 j = 0; j < i; j++)
            EXPECT_NE(rentals[i], rentals[j]);
    }
    EXPECT(DynamaxAdventure_ChooseRental(1));
    EXPECT(DynamaxAdventure_ChooseRental(2));
    for (u32 i = 0; i < DA_PATH_CHOICES; i++)
        paths[i] = GetMonData(DynamaxAdventure_GetPath(i), MON_DATA_SPECIES);
    EXPECT_NE(paths[0], paths[1]);
    EXPECT(DynamaxAdventure_Quit());
    EXPECT(DynamaxAdventure_Begin(42));
    for (u32 i = 0; i < DA_RENTAL_CHOICES; i++)
        EXPECT_EQ(GetMonData(DynamaxAdventure_GetRental(i), MON_DATA_SPECIES), rentals[i]);
    EXPECT(DynamaxAdventure_ChooseRental(1));
    EXPECT(DynamaxAdventure_ChooseRental(2));
    for (u32 i = 0; i < DA_PATH_CHOICES; i++)
        EXPECT_EQ(GetMonData(DynamaxAdventure_GetPath(i), MON_DATA_SPECIES), paths[i]);
    EXPECT(DynamaxAdventure_Quit());
}

TEST("Adventure: battle isolation restores the entire original party and blocks bag and saving")
{
    struct Pokemon original[PARTY_SIZE];
    u8 count;
    ZeroPlayerPartyMons();
    CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][0], SPECIES_PIKACHU, 12, 0, OTID_STRUCT_PLAYER_ID, 8);
    CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][1], SPECIES_EEVEE, 15, 0, OTID_STRUCT_PLAYER_ID, 9);
    CalculatePlayerPartyCount();
    count = gPartiesCount[B_TRAINER_PLAYER];
    memcpy(original, gParties[B_TRAINER_PLAYER], sizeof(original));
    StartRental();
    EXPECT(DynamaxAdventure_ChoosePath(0));
    EXPECT(DynamaxAdventure_PrepareBattle());
    EXPECT(IsDynamaxAdventureBattle());
    EXPECT_EQ(gPartiesCount[B_TRAINER_PLAYER], DA_TEAM_SIZE);
    EXPECT_EQ(gPartiesCount[B_TRAINER_OPPONENT_A], DA_TEAM_SIZE);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_LEVEL), DA_RENTAL_LEVEL);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][1], MON_DATA_LEVEL), DA_RENTAL_LEVEL);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_LEVEL), DA_RENTAL_LEVEL);
    EXPECT(!BriskCanUseBattleItem(ITEM_POTION));
    EXPECT(!BriskCanUseBattleItem(ITEM_MASTER_BALL));
    EXPECT_EQ(TrySavingData(SAVE_NORMAL), SAVE_STATUS_ERROR);
    EXPECT(!DynamaxAdventure_PrepareBattle());
    EXPECT(!DynamaxAdventure_Quit());
    EXPECT(!DynamaxAdventure_Claim(0));
    EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
    EXPECT_EQ(gPartiesCount[B_TRAINER_PLAYER], count);
    EXPECT_EQ(memcmp(original, gParties[B_TRAINER_PLAYER], sizeof(original)), 0);
    EXPECT(!IsDynamaxAdventureBattle());
    EXPECT(DynamaxAdventure_Quit());
}

TEST("Adventure: four losses end the run without whiteout or modifying the party")
{
    struct Pokemon original[PARTY_SIZE];
    memcpy(original, gParties[B_TRAINER_PLAYER], sizeof(original));
    StartRental();
    EXPECT(DynamaxAdventure_ChoosePath(1));
    for (u32 i = 0; i < DA_STARTING_LIVES; i++)
    {
        EXPECT(DynamaxAdventure_PrepareBattle());
        EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_LOST));
        EXPECT_EQ(DynamaxAdventure_GetStatus().lives, DA_STARTING_LIVES - i - 1);
        EXPECT_EQ(DynamaxAdventure_GetStatus().stage, 0);
    }
    EXPECT_EQ(DynamaxAdventure_GetStatus().phase, DA_PHASE_FINISHED);
    EXPECT_EQ(DynamaxAdventure_GetStatus().cleared, FALSE);
    EXPECT(!DynamaxAdventure_PrepareBattle());
    EXPECT(!DynamaxAdventure_Claim(0));
    EXPECT_EQ(memcmp(original, gParties[B_TRAINER_PLAYER], sizeof(original)), 0);
    EXPECT(DynamaxAdventure_Quit());
}

TEST("Adventure: four catches allow swaps and exactly one final reward")
{
    u16 species[DA_ENCOUNTERS];
    ZeroPlayerPartyMons();
    StartRental();
    for (u32 stage = 0; stage < DA_ENCOUNTERS; stage++)
    {
        EXPECT_EQ(DynamaxAdventure_GetStatus().stage, stage);
        species[stage] = GetMonData(DynamaxAdventure_GetPath(0), MON_DATA_SPECIES);
        EXPECT_EQ(GetMonData(DynamaxAdventure_GetPath(0), MON_DATA_LEVEL), stage == 3 ? DA_BOSS_LEVEL : DA_RENTAL_LEVEL);
        EXPECT(DynamaxAdventure_ChoosePath(0));
        EXPECT(DynamaxAdventure_PrepareBattle());
        EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
        EXPECT(!DynamaxAdventure_Claim(0));
        EXPECT(DynamaxAdventure_Catch(TRUE));
        EXPECT(!DynamaxAdventure_Catch(TRUE));
        if (stage < DA_ENCOUNTERS - 1)
        {
            EXPECT(DynamaxAdventure_Swap(TRUE));
            EXPECT(!DynamaxAdventure_Swap(TRUE));
        }
    }
    EXPECT_EQ(DynamaxAdventure_GetStatus().phase, DA_PHASE_FINISHED);
    EXPECT_EQ(DynamaxAdventure_GetStatus().caughtCount, DA_ENCOUNTERS);
    EXPECT(DynamaxAdventure_GetStatus().cleared);
    for (u32 i = 0; i < DA_ENCOUNTERS; i++)
        EXPECT_EQ(GetMonData(DynamaxAdventure_GetCaught(i), MON_DATA_SPECIES), species[i]);
    EXPECT(DynamaxAdventure_Claim(3));
    EXPECT_EQ(gPartiesCount[B_TRAINER_PLAYER], 1);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SPECIES), species[3]);
    EXPECT(!DynamaxAdventure_Claim(0));
    EXPECT_EQ(DynamaxAdventure_GetStatus().phase, DA_PHASE_INACTIVE);
}

TEST("Adventure: declining catches and swaps advances correctly and escape ends a run")
{
    StartRental();
    EXPECT(DynamaxAdventure_ChoosePath(0));
    EXPECT(DynamaxAdventure_PrepareBattle());
    EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
    EXPECT(DynamaxAdventure_Catch(FALSE));
    EXPECT_EQ(DynamaxAdventure_GetStatus().caughtCount, 0);
    EXPECT_EQ(DynamaxAdventure_GetStatus().stage, 1);
    EXPECT_EQ(DynamaxAdventure_GetStatus().phase, DA_PHASE_PATH);
    EXPECT(DynamaxAdventure_ChoosePath(0));
    EXPECT(DynamaxAdventure_PrepareBattle());
    EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
    EXPECT(DynamaxAdventure_Catch(TRUE));
    EXPECT(DynamaxAdventure_Swap(FALSE));
    EXPECT_EQ(DynamaxAdventure_GetStatus().stage, 2);
    EXPECT(DynamaxAdventure_ChoosePath(0));
    EXPECT(DynamaxAdventure_PrepareBattle());
    EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_RAN));
    EXPECT_EQ(DynamaxAdventure_GetStatus().phase, DA_PHASE_FINISHED);
    EXPECT_EQ(DynamaxAdventure_GetStatus().cleared, FALSE);
    EXPECT(DynamaxAdventure_Quit());
}

TEST("Adventure: script bridge reports state and rejects invalid operations")
{
    gSpecialVar_0x8004 = DA_OP_BEGIN;
    RunDynamaxAdventure();
    EXPECT_EQ(gSpecialVar_Result, TRUE);
    gSpecialVar_0x8004 = DA_OP_STATUS;
    RunDynamaxAdventure();
    EXPECT_EQ(gSpecialVar_0x8000, DA_PHASE_RENTAL);
    EXPECT_EQ(gSpecialVar_0x8002, DA_STARTING_LIVES);
    gSpecialVar_0x8004 = DA_OP_RENTAL_INFO;
    gSpecialVar_0x8005 = 0;
    RunDynamaxAdventure();
    EXPECT_EQ(gSpecialVar_Result, TRUE);
    EXPECT_NE(gSpecialVar_0x8000, SPECIES_NONE);
    EXPECT_EQ(gSpecialVar_0x8001, DA_RENTAL_LEVEL);
    gSpecialVar_0x8004 = 0xFFFF;
    RunDynamaxAdventure();
    EXPECT_EQ(gSpecialVar_Result, FALSE);
    EXPECT(DynamaxAdventure_Quit());
}

#include "battle_gimmick.h"
#include "test/battle.h"

DOUBLE_BATTLE_TEST("Adventure: doubles Dynamax only the boss and keep its timer beyond three turns")
{
    GIVEN {
        StartRental();
        EXPECT(DynamaxAdventure_ChoosePath(0));
        EXPECT(DynamaxAdventure_PrepareBattle());
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(200); HP(200); Moves(MOVE_CELEBRATE); DynamaxLevel(10); }
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(200); HP(200); Moves(MOVE_CELEBRATE); DynamaxLevel(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_CELEBRATE, gimmick: GIMMICK_DYNAMAX); MOVE(playerRight, MOVE_CELEBRATE); MOVE(opponentLeft, MOVE_CELEBRATE); MOVE(opponentRight, MOVE_CELEBRATE); }
        TURN { MOVE(playerLeft, MOVE_CELEBRATE); MOVE(playerRight, MOVE_CELEBRATE); MOVE(opponentLeft, MOVE_CELEBRATE); MOVE(opponentRight, MOVE_CELEBRATE); }
        TURN { MOVE(playerLeft, MOVE_CELEBRATE); MOVE(playerRight, MOVE_CELEBRATE); MOVE(opponentLeft, MOVE_CELEBRATE); MOVE(opponentRight, MOVE_CELEBRATE); }
        TURN { MOVE(playerLeft, MOVE_CELEBRATE); MOVE(playerRight, MOVE_CELEBRATE); MOVE(opponentLeft, MOVE_CELEBRATE); MOVE(opponentRight, MOVE_CELEBRATE); }
    } THEN {
        EXPECT_EQ(GetActiveGimmick(B_BATTLER_1), GIMMICK_DYNAMAX);
        EXPECT_EQ(opponentLeft->maxHP, 400);
        EXPECT_EQ(GetActiveGimmick(B_BATTLER_3), GIMMICK_NONE);
        EXPECT_EQ(GetActiveGimmick(B_BATTLER_0), GIMMICK_NONE);
        EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
        gMain.inBattle = FALSE;
        EXPECT(DynamaxAdventure_Quit());
    }
}

DOUBLE_BATTLE_TEST("Adventure: both rentals obey without badges and grant no EXP")
{
    GIVEN {
        StartRental();
        EXPECT(DynamaxAdventure_ChoosePath(0));
        EXPECT(DynamaxAdventure_PrepareBattle());
        PLAYER(SPECIES_WOBBUFFET) { Level(65); Speed(201); Moves(MOVE_DRAGON_RAGE); }
        PLAYER(SPECIES_WOBBUFFET) { Level(65); Speed(200); Moves(MOVE_DRAGON_RAGE); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); Speed(100); Moves(MOVE_SCRATCH); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); Speed(99); Moves(MOVE_SCRATCH); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_DRAGON_RAGE, target: opponentLeft);
            MOVE(playerRight, MOVE_DRAGON_RAGE, target: opponentRight);
            MOVE(opponentLeft, MOVE_SCRATCH, target: playerLeft);
            MOVE(opponentRight, MOVE_SCRATCH, target: playerLeft);
        }
    } SCENE {
        MESSAGE("Wobbuffet used Dragon Rage!");
        MESSAGE("The opposing Wobbuffet fainted!");
        MESSAGE("Wobbuffet used Dragon Rage!");
        MESSAGE("The opposing Wobbuffet fainted!");
    } THEN {
        for (u32 i = 0; i < DA_TEAM_SIZE; i++)
            EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_EXP), gExperienceTables[gSpeciesInfo[SPECIES_WOBBUFFET].growthRate][65]);
        EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
        gMain.inBattle = FALSE;
        EXPECT(DynamaxAdventure_Quit());
    }
}

TEST("Adventure: full storage preserves the reward until space is available")
{
    struct Pokemon filler;
    CreateMonWithIVs(&filler, SPECIES_PIKACHU, 10, 0, OTID_STRUCT_PLAYER_ID, 10);
    for (u32 i = 0; i < PARTY_SIZE; i++)
        gParties[B_TRAINER_PLAYER][i] = filler;
    CalculatePlayerPartyCount();
    for (u32 box = 0; box < TOTAL_BOXES_COUNT; box++)
        for (u32 slot = 0; slot < IN_BOX_COUNT; slot++)
            *GetBoxedMonPtr(box, slot) = filler.box;
    StartRental();
    EXPECT(DynamaxAdventure_ChoosePath(0));
    EXPECT(DynamaxAdventure_PrepareBattle());
    EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
    EXPECT(DynamaxAdventure_Catch(TRUE));
    EXPECT(DynamaxAdventure_Swap(FALSE));
    EXPECT(DynamaxAdventure_ChoosePath(0));
    EXPECT(DynamaxAdventure_PrepareBattle());
    EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_RAN));
    EXPECT(!DynamaxAdventure_Claim(0));
    EXPECT_EQ(DynamaxAdventure_GetStatus().phase, DA_PHASE_FINISHED);
    EXPECT_EQ(DynamaxAdventure_GetStatus().caughtCount, 1);
    ZeroBoxMonData(GetBoxedMonPtr(0, 0));
    EXPECT(DynamaxAdventure_Claim(0));
    EXPECT_EQ(DynamaxAdventure_GetStatus().phase, DA_PHASE_INACTIVE);
    EXPECT_NE(GetBoxMonData(GetBoxedMonPtr(0, 0), MON_DATA_SPECIES), SPECIES_NONE);
    for (u32 box = 0; box < TOTAL_BOXES_COUNT; box++)
        for (u32 slot = 0; slot < IN_BOX_COUNT; slot++)
            ZeroBoxMonData(GetBoxedMonPtr(box, slot));
}

TEST("Adventure: every canonical legendary and mythical is in the boss pool")
{
    u32 count = DynamaxAdventure_GetBossPoolCount();
    bool8 seen[NUM_SPECIES] = {0};
    EXPECT_GE(count, 90);
    for (u32 i = 0; i < count; i++)
    {
        enum Species species = DynamaxAdventure_GetBossPoolSpecies(i);
        EXPECT_NE(species, SPECIES_NONE);
        EXPECT(!seen[species]);
        seen[species] = TRUE;
        EXPECT(!gSpeciesInfo[species].isMegaEvolution);
        EXPECT(!gSpeciesInfo[species].isPrimalReversion);
        EXPECT(!gSpeciesInfo[species].isGigantamax);
        EXPECT(!gSpeciesInfo[species].isTeraForm);
    }
    for (enum Species species = 1; species < NUM_SPECIES; species++)
        if (IsSpeciesEnabled(species) && GET_BASE_SPECIES_ID(species) == species
         && (gSpeciesInfo[species].isRestrictedLegendary || gSpeciesInfo[species].isSubLegendary || gSpeciesInfo[species].isMythical))
            EXPECT(seen[species]);
    EXPECT(seen[SPECIES_ARTICUNO_GALAR]);
    EXPECT(seen[SPECIES_ZAPDOS_GALAR]);
    EXPECT(seen[SPECIES_MOLTRES_GALAR]);
    EXPECT(seen[SPECIES_MEW]);
    EXPECT(seen[SPECIES_MEWTWO]);
    EXPECT(seen[SPECIES_ARCEUS]);
    EXPECT(seen[SPECIES_ZACIAN]);
    EXPECT(seen[SPECIES_ETERNATUS]);
    EXPECT(seen[SPECIES_KORAIDON]);
    EXPECT(seen[SPECIES_MIRAIDON]);
    EXPECT(seen[SPECIES_TERAPAGOS]);
    EXPECT(seen[SPECIES_PECHARUNT]);
    EXPECT(!seen[SPECIES_PIKACHU]);
    EXPECT(!seen[SPECIES_MEWTWO_MEGA_X]);
    EXPECT_EQ(DynamaxAdventure_GetBossPoolSpecies(count), SPECIES_NONE);
    Test_MgbaPrintf("Adventure boss pool: %d species", count);
}

TEST("Adventure: competitive rental pool has level 100 moves abilities and full training")
{
    bool8 seen[NUM_SPECIES] = {0};
    u32 found = 0;
    u32 count = DynamaxAdventure_GetRentalPoolCount();
    EXPECT_EQ(count, 48);
    for (u32 seed = 0; seed < 1024 && found < count; seed++)
    {
        EXPECT(DynamaxAdventure_Begin(seed));
        for (u32 i = 0; i < DA_RENTAL_CHOICES; i++)
        {
            struct Pokemon *mon = DynamaxAdventure_GetRental(i);
            enum Species species = GetMonData(mon, MON_DATA_SPECIES);
            EXPECT_EQ(GetMonData(mon, MON_DATA_LEVEL), 100);
            u32 evs = 0;
            for (u32 stat = 0; stat < NUM_STATS; stat++)
            {
                EXPECT_EQ(GetMonData(mon, MON_DATA_HP_IV + stat), 31);
                evs += GetMonData(mon, MON_DATA_HP_EV + stat);
            }
            EXPECT_EQ(evs, 508);
            for (u32 slot = 0; slot < MAX_MON_MOVES; slot++)
                EXPECT_NE(GetMonData(mon, MON_DATA_MOVE1 + slot), MOVE_NONE);
            switch (species)
            {
            case SPECIES_INCINEROAR: EXPECT_EQ(GetMonAbility(mon), ABILITY_INTIMIDATE); break;
            case SPECIES_RILLABOOM: EXPECT_EQ(GetMonAbility(mon), ABILITY_GRASSY_SURGE); break;
            case SPECIES_AMOONGUSS: EXPECT_EQ(GetMonAbility(mon), ABILITY_REGENERATOR); break;
            case SPECIES_WHIMSICOTT: EXPECT_EQ(GetMonAbility(mon), ABILITY_PRANKSTER); break;
            case SPECIES_GHOLDENGO: EXPECT_EQ(GetMonAbility(mon), ABILITY_GOOD_AS_GOLD); break;
            case SPECIES_AZUMARILL: EXPECT_EQ(GetMonAbility(mon), ABILITY_HUGE_POWER); break;
            default: break;
            }
            if (!seen[species]) { found++; seen[species] = TRUE; }
        }
        EXPECT(DynamaxAdventure_Quit());
    }
    EXPECT_EQ(found, count);
    for (u32 i = 0; i < count; i++)
        EXPECT(seen[DynamaxAdventure_GetRentalPoolSpecies(i)]);
    EXPECT_EQ(DynamaxAdventure_GetRentalPoolSpecies(count), SPECIES_NONE);
}

TEST("Adventure: swapping either rental preserves the other and revives a fallen partner")
{
    StartRental();
    EXPECT(DynamaxAdventure_ChoosePath(0));
    EXPECT(DynamaxAdventure_PrepareBattle());
    u16 first = GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SPECIES);
    u16 encounter = GetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_SPECIES);
    u16 hp = 0;
    SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP, &hp);
    EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
    EXPECT(DynamaxAdventure_Catch(TRUE));
    EXPECT(!DynamaxAdventure_SwapRental(DA_TEAM_SIZE));
    EXPECT(DynamaxAdventure_SwapRental(1));
    EXPECT(DynamaxAdventure_ChoosePath(0));
    EXPECT(DynamaxAdventure_PrepareBattle());
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SPECIES), first);
    EXPECT_GT(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP), 0);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_SPECIES), encounter);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_HP), GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_MAX_HP));
    EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_RAN));
    EXPECT(DynamaxAdventure_Quit());
}

TEST("Adventure: every boss generates at level 100 with usable attacks")
{
    EXPECT(DynamaxAdventure_Begin(17));
    for (u32 index = 0; index < DynamaxAdventure_GetBossPoolCount(); index++)
    {
        struct Pokemon mon;
        enum Species species = DynamaxAdventure_GetBossPoolSpecies(index);
        EXPECT(Test_DynamaxAdventure_CreateBoss(&mon, species));
        EXPECT_EQ(GetMonData(&mon, MON_DATA_SPECIES), species);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_LEVEL), 100);
        bool32 attack = FALSE;
        for (u32 slot = 0; slot < MAX_MON_MOVES; slot++)
        {
            enum Move move = GetMonData(&mon, MON_DATA_MOVE1 + slot);
            if (move != MOVE_NONE && GetMoveCategory(move) != DAMAGE_CATEGORY_STATUS)
                attack = TRUE;
        }
        EXPECT(attack);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_HP), GetMonData(&mon, MON_DATA_MAX_HP));
    }
    EXPECT(DynamaxAdventure_Quit());
}
