#ifndef GUARD_BRISK_BOSS_RUSH_H
#define GUARD_BRISK_BOSS_RUSH_H

enum BriskRushReward
{
    BRISK_RUSH_REWARD_NONE,
    BRISK_RUSH_REWARD_MEGA_STONE,
    BRISK_RUSH_REWARD_RANDOM_MON,
    BRISK_RUSH_REWARD_SHINY_MON,
    BRISK_RUSH_REWARD_LEGENDARY,
    BRISK_RUSH_REWARD_SHINY_LEGENDARY,
};

u16 BriskBossRushRewardForWins(u16 wins);
u16 BriskBossRushRewardCountForWins(u16 wins);
u16 BriskBossRushRewardAtIndex(u16 wins, u16 index);
u16 BriskBossRushRewardIsItem(void);
u16 BriskBossRushBufferReward(void);
u16 BriskBossRushClaimReward(void);
u16 BriskBossRushFinishReward(void);
bool32 BriskBossRushIsActive(void);
bool32 BriskBossRushBagAllowed(void);
bool32 BriskBossRushShiftAllowed(void);
bool32 BriskBossRushGimmicksRecharge(void);
void BriskBossRushAdjustOpponentParty(void);
u16 BriskBossRushInitRules(void);
u16 BriskBossRushBufferRules(void);
u16 BriskBossRushBufferExtras(void);
u16 BriskBossRushBufferBattleRules(void);
u16 BriskBossRushToggleHeal(void);
u16 BriskBossRushToggleBag(void);
u16 BriskBossRushToggleStatus(void);
u16 BriskBossRushTogglePP(void);
u16 BriskBossRushCycleEnemyLevels(void);
u16 BriskBossRushToggleBattleStyle(void);
u16 BriskBossRushToggleGimmicks(void);
u16 BriskBossRushCycleDifficulty(void);
u16 BriskBossRushPrepare(void);
u16 BriskBossRushCheckActive(void);
u16 BriskBossRushBufferNext(void);
u16 BriskBossRushFight(void);
u16 BriskBossRushEndRound(void);
u16 BriskBossRushForfeit(void);
#endif
