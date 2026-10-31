#include "global.h"
#include "brisk_boss_rush.h"
#include "test/test.h"

TEST("Scott Summit rewards: first nine victories are single Mega Stones")
{
    EXPECT_EQ(BriskBossRushRewardForWins(0), BRISK_RUSH_REWARD_NONE);
    EXPECT_EQ(BriskBossRushRewardCountForWins(0), 0);
    for (u32 wins = 1; wins < 10; wins++)
    {
        EXPECT_EQ(BriskBossRushRewardForWins(wins), BRISK_RUSH_REWARD_MEGA_STONE);
        EXPECT_EQ(BriskBossRushRewardCountForWins(wins), 1);
        EXPECT_EQ(BriskBossRushRewardAtIndex(wins, 0), BRISK_RUSH_REWARD_MEGA_STONE);
        EXPECT_EQ(BriskBossRushRewardAtIndex(wins, 1), BRISK_RUSH_REWARD_NONE);
    }
}

TEST("Scott Summit rewards: milestone bundles contain all previous reward tiers")
{
    EXPECT_EQ(BriskBossRushRewardCountForWins(10), 2);
    EXPECT_EQ(BriskBossRushRewardCountForWins(25), 3);
    EXPECT_EQ(BriskBossRushRewardCountForWins(50), 4);
    EXPECT_EQ(BriskBossRushRewardCountForWins(75), 5);
    EXPECT_EQ(BriskBossRushRewardCountForWins(100), 6);
    EXPECT_EQ(BriskBossRushRewardCountForWins(125), 7);
    EXPECT_EQ(BriskBossRushRewardCountForWins(150), 8);
    const u16 categories[] = {
        BRISK_RUSH_REWARD_MEGA_STONE,
        BRISK_RUSH_REWARD_RANDOM_MON,
        BRISK_RUSH_REWARD_SHINY_MON,
        BRISK_RUSH_REWARD_LEGENDARY,
        BRISK_RUSH_REWARD_SHINY_LEGENDARY,
    };
    const u16 milestones[] = {10, 25, 50, 75, 100, 125};
    for (u32 i = 0; i < ARRAY_COUNT(milestones); i++)
    {
        u16 wins = milestones[i];
        u16 count = BriskBossRushRewardCountForWins(wins);
        for (u32 j = 0; j < count; j++)
        {
            u16 expected = j < ARRAY_COUNT(categories) ? categories[j] : BRISK_RUSH_REWARD_SHINY_LEGENDARY;
            EXPECT_EQ(BriskBossRushRewardAtIndex(wins, j), expected);
        }
        EXPECT_EQ(BriskBossRushRewardAtIndex(wins, count), BRISK_RUSH_REWARD_NONE);
    }
}

TEST("Scott Summit rewards: nonmilestone wins never duplicate milestone payouts")
{
    for (u32 wins = 11; wins <= 500; wins++)
    {
        if (wins == 25 || wins == 50 || (wins >= 75 && wins % 25 == 0))
            continue;
        EXPECT_EQ(BriskBossRushRewardCountForWins(wins), 0);
        EXPECT_EQ(BriskBossRushRewardAtIndex(wins, 0), BRISK_RUSH_REWARD_NONE);
    }
    EXPECT_EQ(BriskBossRushRewardForWins(25), BRISK_RUSH_REWARD_SHINY_MON);
    EXPECT_EQ(BriskBossRushRewardForWins(50), BRISK_RUSH_REWARD_LEGENDARY);
    EXPECT_EQ(BriskBossRushRewardForWins(100), BRISK_RUSH_REWARD_SHINY_LEGENDARY);
}
