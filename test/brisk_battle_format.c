#include "global.h"
#include "brisk_battle_format.h"
#include "brisk_options.h"
#include "battle.h"
#include "battle_controllers.h"
#include "event_data.h"
#include "main.h"
#include "malloc.h"
#include "pokemon.h"
#include "task.h"
#include "test/test.h"

u8 Test_OpenBriskBattleOptions(void);
void Test_CloseBriskThemeOption(u8 taskId);

TEST("Brisk Options cycles all battle formats and follower ON/OFF without losing selection")
{
    VarSet(VAR_BRISK_BATTLE_FORMAT, BRISK_BATTLE_REGULAR);
    VarSet(VAR_BRISK_FOLLOWERS_DISABLED, 0);
    u8 task = Test_OpenBriskBattleOptions();
    for (u32 i = 1; i <= 3; i++)
    {
        gMain.newKeys = A_BUTTON;
        RunTasks();
        EXPECT_EQ(GetBriskBattleFormat(), i % 3);
    }
    gMain.newKeys = 0;
    gMain.newAndRepeatedKeys = DPAD_DOWN;
    RunTasks();
    gMain.newAndRepeatedKeys = 0;
    gMain.newKeys = A_BUTTON;
    RunTasks();
    EXPECT(!BriskPokemonFollowersEnabled());
    EXPECT_EQ(VarGet(VAR_BRISK_FOLLOWERS_DISABLED), 1);
    gMain.newKeys = 0;
    Test_CloseBriskThemeOption(task);
    task = Test_OpenBriskBattleOptions();
    EXPECT(!BriskPokemonFollowersEnabled());
    gMain.newAndRepeatedKeys = DPAD_DOWN;
    RunTasks();
    gMain.newAndRepeatedKeys = 0;
    gMain.newKeys = A_BUTTON;
    RunTasks();
    EXPECT(BriskPokemonFollowersEnabled());
    gMain.newKeys = 0;
    Test_CloseBriskThemeOption(task);
}

TEST("Brisk battle formats preserve Regular and dedicated controllers, and convert ordinary formats")
{
    u32 saved = gBattleTypeFlags;
    u32 original = BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI | BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_TWO_OPPONENTS;
    VarSet(VAR_BRISK_BATTLE_FORMAT, BRISK_BATTLE_REGULAR);
    gBattleTypeFlags = original;
    BriskPrepareBattleFormat();
    EXPECT_EQ(gBattleTypeFlags, original);
    VarSet(VAR_BRISK_BATTLE_FORMAT, BRISK_BATTLE_SINGLES);
    BriskPrepareBattleFormat();
    EXPECT_EQ(gBattleTypeFlags, BATTLE_TYPE_TRAINER);
    EXPECT(BriskBattleHadTwoOpponents());
    VarSet(VAR_BRISK_BATTLE_FORMAT, BRISK_BATTLE_DOUBLES);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER;
    BriskPrepareBattleFormat();
    EXPECT(gBattleTypeFlags & BATTLE_TYPE_DOUBLE);
    static const u32 special[] = {BATTLE_TYPE_LINK, BATTLE_TYPE_RECORDED, BATTLE_TYPE_SAFARI, BATTLE_TYPE_CATCH_TUTORIAL};
    for (u32 i = 0; i < ARRAY_COUNT(special); i++)
    {
        gBattleTypeFlags = special[i];
        BriskPrepareBattleFormat();
        EXPECT_EQ(gBattleTypeFlags, special[i]);
    }
    VarSet(VAR_BRISK_BATTLE_FORMAT, 0);
    gBattleTypeFlags = saved;
    BriskPrepareBattleFormat();
}

TEST("Brisk doubles create a second valid wild opponent and singles remove that extra opponent")
{
    u32 saved = gBattleTypeFlags;
    ZeroEnemyPartyMons();
    CreateMonWithIVs(&gParties[B_TRAINER_OPPONENT_A][0], SPECIES_PIKACHU, 35, 0, OTID_STRUCT_PLAYER_ID, 31);
    gBattleTypeFlags = 0;
    VarSet(VAR_BRISK_BATTLE_FORMAT, BRISK_BATTLE_DOUBLES);
    BriskPrepareBattleFormat();
    BriskPrepareBattleParties();
    EXPECT(IsValidForBattle(&gParties[B_TRAINER_OPPONENT_A][1]));
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][1], MON_DATA_LEVEL), 35);
    VarSet(VAR_BRISK_BATTLE_FORMAT, BRISK_BATTLE_SINGLES);
    BriskPrepareBattleFormat();
    BriskPrepareBattleParties();
    EXPECT(!IsValidForBattle(&gParties[B_TRAINER_OPPONENT_A][1]));
    EXPECT(IsValidForBattle(&gParties[B_TRAINER_OPPONENT_A][0]));
    VarSet(VAR_BRISK_BATTLE_FORMAT, 0);
    gBattleTypeFlags = saved;
    BriskPrepareBattleFormat();
}

TEST("Brisk forced singles retain all six mons from the second trainer and preserve the fainted EXP source")
{
    u32 saved = gBattleTypeFlags;
    ZeroEnemyPartyMons();
    memset(gParties[B_TRAINER_OPPONENT_B], 0, sizeof(gParties[B_TRAINER_OPPONENT_B]));
    CreateMonWithIVs(&gParties[B_TRAINER_OPPONENT_A][0], SPECIES_CATERPIE, 20, 0, OTID_STRUCT_PLAYER_ID, 31);
    u16 hp = 0;
    SetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_HP, &hp);
    for (u32 i = 0; i < PARTY_SIZE; i++)
        CreateMonWithIVs(&gParties[B_TRAINER_OPPONENT_B][i], SPECIES_PIKACHU, 20 + i, 0, OTID_STRUCT_PLAYER_ID, 31);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_OPPONENTS;
    VarSet(VAR_BRISK_BATTLE_FORMAT, BRISK_BATTLE_SINGLES);
    BriskPrepareBattleFormat();
    gBattlerPositions[1] = B_POSITION_OPPONENT_LEFT;
    gBattlerPartyIndexes[1] = 0;
    struct BattleStruct *savedStruct = gBattleStruct;
    gBattleStruct = AllocZeroed(sizeof(*gBattleStruct));
    gBattleStruct->givenExpMons[0] = 0x3F;
    EXPECT(BriskRefillSinglesOpponent());
    EXPECT_EQ(gBattleStruct->givenExpMons[0], 1);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_SPECIES), SPECIES_CATERPIE);
    EXPECT(IsValidForBattle(&gParties[B_TRAINER_OPPONENT_B][5]));
    for (u32 i = 1; i < PARTY_SIZE; i++)
    {
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][i], MON_DATA_LEVEL), 19 + i);
        SetMonData(&gParties[B_TRAINER_OPPONENT_A][i], MON_DATA_HP, &hp);
    }
    EXPECT(BriskRefillSinglesOpponent());
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][1], MON_DATA_LEVEL), 25);
    EXPECT(!BriskRefillSinglesOpponent());
    Free(gBattleStruct);
    gBattleStruct = savedStruct;
    VarSet(VAR_BRISK_BATTLE_FORMAT, 0);
    gBattleTypeFlags = saved;
    BriskPrepareBattleFormat();
}

#include "test/battle.h"
void Test_BriskConvertRecordedBattle(bool32 enabled);

SINGLE_BATTLE_TEST("Brisk forced Singles initializes a two-battler fight from double battle flags")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
        VarSet(VAR_BRISK_BATTLE_FORMAT, BRISK_BATTLE_SINGLES);
        Test_BriskConvertRecordedBattle(TRUE);
        gBattleTestRunnerState->data.recordedBattle.battleFlags |= BATTLE_TYPE_DOUBLE;
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_CELEBRATE); }
    } THEN {
        EXPECT_EQ(gBattlersCount, 2);
        EXPECT(!IsDoubleBattle());
        VarSet(VAR_BRISK_BATTLE_FORMAT, 0);
        Test_BriskConvertRecordedBattle(FALSE);
    }
}

DOUBLE_BATTLE_TEST("Brisk forced Doubles initializes four occupied battlers from single battle flags")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
        VarSet(VAR_BRISK_BATTLE_FORMAT, BRISK_BATTLE_DOUBLES);
        Test_BriskConvertRecordedBattle(TRUE);
        gBattleTestRunnerState->data.recordedBattle.battleFlags &= ~BATTLE_TYPE_DOUBLE;
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_CELEBRATE); MOVE(playerRight, MOVE_CELEBRATE); MOVE(opponentLeft, MOVE_CELEBRATE); MOVE(opponentRight, MOVE_CELEBRATE); }
    } THEN {
        EXPECT_EQ(gBattlersCount, 4);
        EXPECT(IsDoubleBattle());
        EXPECT_EQ(gAbsentBattlerFlags, 0);
        VarSet(VAR_BRISK_BATTLE_FORMAT, 0);
        Test_BriskConvertRecordedBattle(FALSE);
    }
}

TEST("Display types can be toggled and retained when reopening Options 5")
{
    u16 saved = VarGet(VAR_BRISK_TYPES_DISABLED);
    VarSet(VAR_BRISK_TYPES_DISABLED, 0);
    u8 task = Test_OpenBriskBattleOptions();
    EXPECT(BriskDisplayTypes());
    for (u32 i = 0; i < 3; i++)
    {
        gMain.newKeys = 0;
        gMain.newAndRepeatedKeys = DPAD_DOWN;
        RunTasks();
    }
    gMain.newAndRepeatedKeys = 0;
    gMain.newKeys = A_BUTTON;
    RunTasks();
    EXPECT(!BriskDisplayTypes());
    gMain.newKeys = 0;
    Test_CloseBriskThemeOption(task);
    task = Test_OpenBriskBattleOptions();
    EXPECT(!BriskDisplayTypes());
    Test_CloseBriskThemeOption(task);
    VarSet(VAR_BRISK_TYPES_DISABLED, saved);
}
