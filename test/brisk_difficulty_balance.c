#include "global.h"
#include "brisk_modes.h"
#include "caps.h"
#include "data.h"
#include "event_data.h"
#include "pokemon.h"
#include "random.h"
#include "trainer_util.h"
#include "test/test.h"
#include "constants/trainers.h"

TEST("Brisk ordinary trainers and rivals both receive difficulty when their parties are generated")
{
    const u8 classes[] = {TRAINER_CLASS_YOUNGSTER, TRAINER_CLASS_RIVAL};
    const u8 expectedLevels[] = {
        [BRISK_MODE_NORMAL] = 20, [BRISK_MODE_EASY] = 10,
        [BRISK_MODE_HARD] = 22, [BRISK_MODE_EXTREME] = 24,
    };
    u16 savedMode = VarGet(VAR_BRISK_DIFFICULTY);
    u16 savedRandomizer = VarGet(VAR_BRISK_RANDOMIZER);
    VarSet(VAR_BRISK_RANDOMIZER, 0);
    struct TrainerMon entry = {
        .species = SPECIES_RATICATE, .lvl = 20, .moves = {MOVE_TACKLE},
        .gender = TRAINER_MON_RANDOM_GENDER, .nature = NATURE_HARDY, .ball = BALL_POKE,
    };
    struct Trainer trainer = {.party = &entry, .partySize = 1, .trainerName = _("TEST")};
    for (u32 i = 0; i < ARRAY_COUNT(classes); i++)
    {
        trainer.trainerClass = classes[i];
        for (u32 mode = BRISK_MODE_NORMAL; mode <= BRISK_MODE_EXTREME; mode++)
        {
            struct TrainerGenerator generator = {0};
            struct Pokemon mon;
            VarSet(VAR_BRISK_DIFFICULTY, mode);
            MakeTrainerGenerator(&generator, &trainer);
            GenerateMonFromTrainerMon(&mon, &entry, &generator);
            EXPECT_EQ(GetMonData(&mon, MON_DATA_LEVEL), expectedLevels[mode]);
            EXPECT_EQ(GetLevelFromMonExp(&mon), expectedLevels[mode]);
            EXPECT_EQ(GetMonData(&mon, MON_DATA_IVS), mode >= BRISK_MODE_HARD ? 0x3FFFFFFF : 0);
            if (mode == BRISK_MODE_EXTREME)
            {
                u32 totalEvs = 0;
                for (u32 stat = 0; stat < NUM_STATS; stat++)
                    totalEvs += GetMonData(&mon, MON_DATA_HP_EV + stat);
                EXPECT_EQ(totalEvs, 510);
            }
        }
    }
    VarSet(VAR_BRISK_DIFFICULTY, savedMode);
    VarSet(VAR_BRISK_RANDOMIZER, savedRandomizer);
}

TEST("Hard and Extreme generate level 25 Roxanne teams and reach level 100 for the League")
{
    // First-party levels from Roxanne through Juan, then the Elite Four.
    // The runner replaces gTrainers, so exercise the real generator with fixtures.
    static const u8 original[] = {40, 50, 55, 65, 70, 80, 85, 90, 100};
    static const u8 expected[] = {25, 35, 45, 50, 55, 65, 75, 90, 100};
    static const u16 flags[] = {FLAG_BADGE01_GET, FLAG_BADGE02_GET, FLAG_BADGE03_GET, FLAG_BADGE04_GET,
        FLAG_BADGE05_GET, FLAG_BADGE06_GET, FLAG_BADGE07_GET, FLAG_BADGE08_GET, FLAG_IS_CHAMPION};
    bool8 saved[ARRAY_COUNT(flags)];
    for (u32 i = 0; i < ARRAY_COUNT(flags); i++) { saved[i] = FlagGet(flags[i]); FlagClear(flags[i]); }
    VarSet(VAR_BRISK_NO_LEVEL_CAP, 0);
    VarSet(VAR_BRISK_RANDOMIZER, 0);
    struct TrainerMon trainerMon = {
        .species = SPECIES_GOLEM, .moves = {MOVE_TACKLE},
        .gender = TRAINER_MON_RANDOM_GENDER, .nature = NATURE_HARDY, .ball = BALL_POKE,
    };
    struct Trainer trainer = {
        .trainerClass = TRAINER_CLASS_LEADER, .party = &trainerMon, .partySize = 1,
        .trainerName = _("TEST"),
    };
    for (u32 stage = 0; stage < ARRAY_COUNT(expected); stage++)
    {
        trainerMon.lvl = original[stage];
        for (u32 mode = BRISK_MODE_HARD; mode <= BRISK_MODE_EXTREME; mode++)
        {
            VarSet(VAR_BRISK_DIFFICULTY, mode);
            struct TrainerGenerator generator = {0};
            struct Pokemon mon;
            MakeTrainerGenerator(&generator, &trainer);
            GenerateMonFromTrainerMon(&mon, &trainerMon, &generator);
            EXPECT_EQ(GetMonData(&mon, MON_DATA_LEVEL), expected[stage]);
            EXPECT_EQ(GetLevelFromMonExp(&mon), expected[stage]);
            EXPECT_EQ(GetProgressionLevelCap(), expected[stage]);
            if (mode == BRISK_MODE_EXTREME)
            {
                EXPECT_EQ(GetCurrentLevelCap(), expected[stage]);
                EXPECT_EQ(GetSoftLevelCapExpValue(expected[stage] - 1, 100), 100);
                EXPECT_EQ(GetSoftLevelCapExpValue(expected[stage], 100), 0);
            }
        }
        FlagSet(flags[stage]);
    }
    for (u32 i = 0; i < ARRAY_COUNT(flags); i++) if (saved[i]) FlagSet(flags[i]); else FlagClear(flags[i]);
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
}

TEST("Hard and Extreme use small early boosts and scale mandatory trainers within every badge cap")
{
    const u16 flags[] = {FLAG_BADGE01_GET, FLAG_BADGE02_GET, FLAG_BADGE03_GET, FLAG_BADGE04_GET,
        FLAG_BADGE05_GET, FLAG_BADGE06_GET, FLAG_BADGE07_GET, FLAG_BADGE08_GET, FLAG_IS_CHAMPION};
    bool8 saved[ARRAY_COUNT(flags)];
    for (u32 i = 0; i < ARRAY_COUNT(flags); i++) { saved[i] = FlagGet(flags[i]); FlagClear(flags[i]); }
    VarSet(VAR_BRISK_NO_LEVEL_CAP, 0);
    for (u32 stage = 0; stage <= ARRAY_COUNT(flags); stage++)
    {
        for (u32 mode = BRISK_MODE_HARD; mode <= BRISK_MODE_EXTREME; mode++)
        {
            VarSet(VAR_BRISK_DIFFICULTY, mode);
            u32 cap = GetProgressionLevelCap();
            for (u32 level = 1; level <= 100; level++)
            {
                EXPECT_LE(ScaleBriskTrainerLevel(level, level), cap);
                EXPECT_GE(ScaleBriskTrainerLevel(level, level), 1);
                EXPECT_EQ(ScaleBriskTrainerLevel(level, level), min(ScaleBriskLevel(level, level), cap));
            }
        }
        if (stage < ARRAY_COUNT(flags)) FlagSet(flags[stage]);
    }
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_HARD);
    EXPECT_EQ(ScaleBriskLevel(7, 0), 8);
    EXPECT_EQ(ScaleBriskLevel(40, 0), 44);
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_EXTREME);
    EXPECT_EQ(ScaleBriskLevel(7, 0), 9);
    EXPECT_EQ(ScaleBriskLevel(40, 0), 48);
    for (u32 i = 0; i < ARRAY_COUNT(flags); i++) if (saved[i]) FlagSet(flags[i]); else FlagClear(flags[i]);
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
}
