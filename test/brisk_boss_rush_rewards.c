#include "global.h"
#include "brisk_boss_rush.h"
#include "test/test.h"

TEST("Scott Summit rewards: first nine victories are Mega Stones")
{
    EXPECT_EQ(BriskBossRushRewardForWins(0), BRISK_RUSH_REWARD_NONE);
    for (u32 wins = 1; wins < 10; wins++)
        EXPECT_EQ(BriskBossRushRewardForWins(wins), BRISK_RUSH_REWARD_MEGA_STONE);
}

TEST("Scott Summit rewards: key milestone species categories")
{
    EXPECT_EQ(BriskBossRushRewardForWins(10), BRISK_RUSH_REWARD_RANDOM_MON);
    EXPECT_EQ(BriskBossRushRewardForWins(25), BRISK_RUSH_REWARD_SHINY_MON);
    EXPECT_EQ(BriskBossRushRewardForWins(50), BRISK_RUSH_REWARD_LEGENDARY);
    EXPECT_EQ(BriskBossRushRewardForWins(75), BRISK_RUSH_REWARD_SHINY_LEGENDARY);
}

TEST("Scott Summit rewards: one shiny legendary every 25 wins after 75")
{
    for (u32 wins = 76; wins <= 300; wins++)
    {
        u16 expected = (wins % 25 == 0)
            ? BRISK_RUSH_REWARD_SHINY_LEGENDARY : BRISK_RUSH_REWARD_NONE;
        EXPECT_EQ(BriskBossRushRewardForWins(wins), expected);
    }
    EXPECT_EQ(BriskBossRushRewardForWins(11), BRISK_RUSH_REWARD_NONE);
    EXPECT_EQ(BriskBossRushRewardForWins(26), BRISK_RUSH_REWARD_NONE);
}
