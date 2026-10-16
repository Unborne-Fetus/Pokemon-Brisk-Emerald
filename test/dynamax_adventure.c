#include "global.h"
#include "dynamax_adventure.h"
#include "battle.h"
#include "data.h"
#include "brisk_modes.h"
#include "event_data.h"
#include "main.h"
#include "pokemon_storage_system.h"
#include "save.h"
#include "test/test.h"

static void StartRental(void)
{
    EXPECT(DynamaxAdventure_Begin(12345));
    EXPECT(DynamaxAdventure_ChooseRental(0));
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
    for (u32 i = 0; i < DA_PATH_CHOICES; i++)
        paths[i] = GetMonData(DynamaxAdventure_GetPath(i), MON_DATA_SPECIES);
    EXPECT_NE(paths[0], paths[1]);
    EXPECT(DynamaxAdventure_Quit());
    EXPECT(DynamaxAdventure_Begin(42));
    for (u32 i = 0; i < DA_RENTAL_CHOICES; i++)
        EXPECT_EQ(GetMonData(DynamaxAdventure_GetRental(i), MON_DATA_SPECIES), rentals[i]);
    EXPECT(DynamaxAdventure_ChooseRental(1));
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
    EXPECT_EQ(gPartiesCount[B_TRAINER_PLAYER], 1);
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

SINGLE_BATTLE_TEST("Adventure: bosses Dynamax automatically and outlast the rental three-turn timer")
{
    GIVEN {
        StartRental();
        EXPECT(DynamaxAdventure_ChoosePath(0));
        EXPECT(DynamaxAdventure_PrepareBattle());
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(200); HP(200); Moves(MOVE_CELEBRATE); DynamaxLevel(10); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(200); HP(200); Moves(MOVE_CELEBRATE); DynamaxLevel(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE, gimmick: GIMMICK_DYNAMAX); MOVE(opponent, MOVE_CELEBRATE); }
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_CELEBRATE); }
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_CELEBRATE); }
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_CELEBRATE); }
    } THEN {
        EXPECT_EQ(GetActiveGimmick(B_BATTLER_1), GIMMICK_DYNAMAX);
        EXPECT_EQ(opponent->maxHP, 400);
        EXPECT_EQ(GetActiveGimmick(B_BATTLER_0), GIMMICK_NONE);
        EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
        // The battle-test runner invokes THEN before its normal field return.
        gMain.inBattle = FALSE;
        EXPECT(DynamaxAdventure_Quit());
    }
}

SINGLE_BATTLE_TEST("Adventure: rental battles obey without badges and grant no EXP")
{
    GIVEN {
        StartRental();
        EXPECT(DynamaxAdventure_ChoosePath(0));
        EXPECT(DynamaxAdventure_PrepareBattle());
        PLAYER(SPECIES_WOBBUFFET) { Level(65); Speed(200); Moves(MOVE_DRAGON_RAGE); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); Speed(100); Moves(MOVE_SCRATCH); }
    } WHEN {
        TURN { MOVE(player, MOVE_DRAGON_RAGE); MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        MESSAGE("Wobbuffet used Dragon Rage!");
        MESSAGE("The opposing Wobbuffet fainted!");
    } THEN {
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_EXP), gExperienceTables[gSpeciesInfo[SPECIES_WOBBUFFET].growthRate][65]);
        EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
        // The battle-test runner invokes THEN before its normal field return.
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
