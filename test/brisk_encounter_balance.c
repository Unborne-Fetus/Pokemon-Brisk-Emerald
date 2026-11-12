#include "global.h"
#include "brisk_options.h"
#include "brisk_modes.h"
#include "brisk_trainer_moves.h"
#include "data.h"
#include "event_data.h"
#include "main.h"
#include "move.h"
#include "pokemon.h"
#include "random.h"
#include "task.h"
#include "trainer_util.h"
#include "wild_encounter_ow.h"
#include "test/test.h"
#include "constants/opponents.h"
#include "constants/event_objects.h"
#include "constants/trainer_types.h"

u8 Test_OpenBriskWorldOptions(void);
u8 Test_OpenBriskAutoBattleOptions(void);
void Test_BriskAutoBattleAwardExp(struct Pokemon *mon, enum Species foeSpecies, u32 foeLevel);
void Test_CloseBriskThemeOption(u8 taskId);
bool32 Test_ShouldDisableRandomEncounters(void);

TEST("Brisk overworld encounters toggle persists in Options 7 and restores random encounters")
{
    u16 saved = VarGet(VAR_BRISK_OW_ENCOUNTERS_DISABLED);
    VarSet(VAR_BRISK_OW_ENCOUNTERS_DISABLED, 0);
    EXPECT(BriskOverworldEncountersEnabled());
    u8 task = Test_OpenBriskWorldOptions();
    gMain.newKeys = 0;
    gMain.newAndRepeatedKeys = DPAD_DOWN;
    RunTasks();
    gMain.newAndRepeatedKeys = 0;
    gMain.newKeys = A_BUTTON;
    RunTasks();
    EXPECT(!BriskOverworldEncountersEnabled());
    EXPECT(!Test_ShouldDisableRandomEncounters());
    Test_CloseBriskThemeOption(task);
    gMain.newKeys = 0;
    task = Test_OpenBriskWorldOptions();
    EXPECT(!BriskOverworldEncountersEnabled());
    gMain.newAndRepeatedKeys = DPAD_DOWN;
    RunTasks();
    gMain.newAndRepeatedKeys = 0;
    gMain.newKeys = A_BUTTON;
    RunTasks();
    EXPECT(BriskOverworldEncountersEnabled());
    gMain.newKeys = 0;
    Test_CloseBriskThemeOption(task);
    VarSet(VAR_BRISK_OW_ENCOUNTERS_DISABLED, saved);
}

TEST("Brisk auto battle modes are opt-in and persist on Options 8")
{
    u16 saved = VarGet(VAR_BRISK_AUTO_BATTLE_MODE);
    VarSet(VAR_BRISK_AUTO_BATTLE_MODE, 0);
    EXPECT_EQ(BriskAutoBattleMode(), BRISK_AUTO_BATTLE_OFF);

    u8 task = Test_OpenBriskAutoBattleOptions();
    gMain.newAndRepeatedKeys = 0;
    gMain.newKeys = A_BUTTON;
    RunTasks();
    EXPECT_EQ(BriskAutoBattleMode(), BRISK_AUTO_BATTLE_GO);

    gMain.newKeys = A_BUTTON;
    RunTasks();
    EXPECT_EQ(BriskAutoBattleMode(), BRISK_AUTO_BATTLE_PASSIVE);

    gMain.newKeys = A_BUTTON;
    RunTasks();
    EXPECT_EQ(BriskAutoBattleMode(), BRISK_AUTO_BATTLE_OFF);

    Test_CloseBriskThemeOption(task);
    gMain.newKeys = 0;
    gMain.newAndRepeatedKeys = 0;
    VarSet(VAR_BRISK_AUTO_BATTLE_MODE, 4);
    EXPECT_EQ(BriskAutoBattleMode(), BRISK_AUTO_BATTLE_OFF);
    VarSet(VAR_BRISK_AUTO_BATTLE_MODE, saved);
}

TEST("Brisk auto battle gives reduced EXP and stops at a level cap")
{
    struct Pokemon mon;
    u16 oldMode = VarGet(VAR_BRISK_DIFFICULTY);
    u16 oldCapSetting = VarGet(VAR_BRISK_NO_LEVEL_CAP);
    u32 expBefore;
    u32 expAfter;
    u32 hpBefore;
    u32 levelBefore;

    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_EASY);
    VarSet(VAR_BRISK_NO_LEVEL_CAP, 1);
    CreateMonWithIVs(&mon, SPECIES_ZIGZAGOON, 10, 0, OTID_STRUCT_PRESET(0), 0);
    GiveMonInitialMoveset(&mon);
    expBefore = GetMonData(&mon, MON_DATA_EXP);
    hpBefore = GetMonData(&mon, MON_DATA_HP);
    levelBefore = GetMonData(&mon, MON_DATA_LEVEL);

    Test_BriskAutoBattleAwardExp(&mon, SPECIES_CHANSEY, 10);
    expAfter = GetMonData(&mon, MON_DATA_EXP);
    EXPECT_GT(expAfter, expBefore);
    EXPECT_LT(expAfter - expBefore, ((u32)gSpeciesInfo[SPECIES_CHANSEY].expYield * 10) / 7);
    EXPECT_GE(GetMonData(&mon, MON_DATA_LEVEL), levelBefore);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_HP), hpBefore - 1);

    VarSet(VAR_BRISK_DIFFICULTY, oldMode);
    VarSet(VAR_BRISK_NO_LEVEL_CAP, oldCapSetting);
}

TEST("Brisk turning overworld encounters off preserves scripted Pokemon and followers and prevents generated contacts")
{
    struct ObjectEvent saved[OBJECT_EVENTS_COUNT];
    memcpy(saved, gObjectEvents, sizeof(saved));
    u16 oldSetting = VarGet(VAR_BRISK_OW_ENCOUNTERS_DISABLED);
    u16 oldTalked = gSpecialVar_LastTalked;
    memset(gObjectEvents, 0, sizeof(saved));
    gObjectEvents[0] = (struct ObjectEvent){.active = TRUE, .localId = 1, .trainerType = TRAINER_TYPE_OW_WILD_ENCOUNTER};
    gObjectEvents[1] = (struct ObjectEvent){.active = TRUE, .localId = OBJ_EVENT_ID_FOLLOWER};
    VarSet(VAR_BRISK_OW_ENCOUNTERS_DISABLED, 1);
    UpdateOverworldWildEncounter(); // Also works without a current encounter header.
    EXPECT(gObjectEvents[0].active);
    EXPECT(gObjectEvents[1].active);
    EXPECT_EQ(GetNumberOfActiveOWEs(OWE_GENERATED), 0);
    gObjectEvents[2] = (struct ObjectEvent){.active = TRUE, .localId = LOCALID_OW_ENCOUNTER_END, .trainerType = TRAINER_TYPE_OW_WILD_ENCOUNTER};
    gObjectEvents[3] = (struct ObjectEvent){.active = TRUE, .isPlayer = TRUE};
    gSpecialVar_LastTalked = 0x1234;
    TryTriggerOverworldWildEncounter(&gObjectEvents[2], &gObjectEvents[3]);
    EXPECT_EQ(gSpecialVar_LastTalked, 0x1234);
    memcpy(gObjectEvents, saved, sizeof(saved));
    VarSet(VAR_BRISK_OW_ENCOUNTERS_DISABLED, oldSetting);
    gSpecialVar_LastTalked = oldTalked;
}

TEST("Brisk Route 102 Normal 10-15 curve maps to Easy 5-8 Hard 13-18 and Extreme 14-19")
{
    const u8 minimum[] = {[BRISK_MODE_NORMAL]=10,[BRISK_MODE_EASY]=5,[BRISK_MODE_HARD]=13,[BRISK_MODE_EXTREME]=14};
    const u8 maximum[] = {[BRISK_MODE_NORMAL]=15,[BRISK_MODE_EASY]=8,[BRISK_MODE_HARD]=18,[BRISK_MODE_EXTREME]=19};
    u16 old = VarGet(VAR_BRISK_DIFFICULTY);
    for (u32 mode = 0; mode <= BRISK_MODE_EXTREME; mode++)
    {
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        EXPECT_EQ(ScaleBriskTrainerLevel(10, 0), minimum[mode]);
        EXPECT_EQ(ScaleBriskTrainerLevel(15, 0), maximum[mode]);
    }
    VarSet(VAR_BRISK_DIFFICULTY, old);
}

TEST("Brisk runtime fair moves apply to ordinary IDs and exclude bosses and dedication teams")
{
    struct TrainerGenerator generator = {0};
    MakeTrainerGenerator(&generator, &gTrainers[DIFFICULTY_NORMAL][TRAINER_ALLEN]);
    EXPECT(generator.fairLoadout);
    MakeTrainerGenerator(&generator, &gTrainers[DIFFICULTY_NORMAL][TRAINER_ROXANNE_1]);
    EXPECT(!generator.fairLoadout);
    MakeTrainerGenerator(&generator, &gTrainers[DIFFICULTY_NORMAL][TRAINER_DALTON_1]);
    EXPECT(!generator.fairLoadout); // Queen's dedication team.
}

TEST("Brisk ordinary Extreme moves remain within stage power limits and Normal keeps authored moves")
{
    const u8 levels[] = {10, 15, 25, 40, 60};
    const u8 limits[] = {50, 60, 75, 100, 120};
    u16 old = VarGet(VAR_BRISK_DIFFICULTY);
    for (u32 i = 0; i < ARRAY_COUNT(levels); i++)
    {
        struct Pokemon mon;
        CreateMonWithIVs(&mon, SPECIES_RATICATE, levels[i], 0, OTID_STRUCT_PRESET(0), 0);
        GiveMonInitialMoveset(&mon);
        struct Pokemon original = mon;
        VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
        ApplyBriskFairTrainerLoadout(&mon);
        EXPECT_EQ(memcmp(&mon, &original, sizeof(mon)), 0);
        VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_EXTREME);
        ApplyBriskFairTrainerLoadout(&mon);
        u32 attacks = 0;
        for (u32 slot = 0; slot < MAX_MON_MOVES; slot++)
        {
            enum Move move = GetMonData(&mon, MON_DATA_MOVE1 + slot);
            if (move == MOVE_NONE) continue;
            EXPECT_LE(GetMovePower(move), limits[i]);
            if (GetMovePower(move) > 1) attacks++;
        }
        EXPECT(attacks > 0);
    }
    VarSet(VAR_BRISK_DIFFICULTY, old);
}

TEST("Brisk Allen ordinary curve is Slowpoke 7 13 15 18 and Rattata 6 11 13 15")
{
    const u8 slowpoke[] = {13, 7, 15, 18};
    const u8 rattata[] = {11, 6, 13, 15};
    u16 oldMode = VarGet(VAR_BRISK_DIFFICULTY);
    for (u32 mode = 0; mode < 4; mode++)
    {
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        EXPECT_EQ(ScaleBriskOrdinaryTrainerLevel(13, 13), slowpoke[mode]);
        EXPECT_EQ(ScaleBriskOrdinaryTrainerLevel(11, 13), rattata[mode]);
    }
    VarSet(VAR_BRISK_DIFFICULTY, oldMode);
}

TEST("Brisk ordinary teams retain level differences through Easy rounding and Hard Extreme caps")
{
    u16 oldMode = VarGet(VAR_BRISK_DIFFICULTY);
    for (u32 mode = 0; mode < 4; mode++)
    {
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        for (u32 ace = 2; ace <= 100; ace++)
        {
            u32 top = ScaleBriskOrdinaryTrainerLevel(ace, ace);
            u32 support = ScaleBriskOrdinaryTrainerLevel(ace - 1, ace);
            if (mode != BRISK_MODE_EASY || top > 1) EXPECT_LT(support, top);
            EXPECT_GE(support, 1);
            EXPECT_LE(top, 100);
        }
    }
    VarSet(VAR_BRISK_DIFFICULTY, oldMode);
}
