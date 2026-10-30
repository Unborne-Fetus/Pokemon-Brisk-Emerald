#ifndef GUARD_BRISK_BOSS_RUSH_H
#define GUARD_BRISK_BOSS_RUSH_H

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
