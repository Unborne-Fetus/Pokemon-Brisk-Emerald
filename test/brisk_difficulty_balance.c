#include "global.h"
#include "brisk_modes.h"
#include "caps.h"
#include "event_data.h"
#include "test/test.h"

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
