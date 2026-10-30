#ifndef GUARD_BRISK_BOSS_RUSH_H
#define GUARD_BRISK_BOSS_RUSH_H

bool32 BriskBossRushIsActive(void);
bool32 BriskBossRushBagAllowed(void);
u16 BriskBossRushInitRules(void);
u16 BriskBossRushBufferRules(void);
u16 BriskBossRushToggleHeal(void);
u16 BriskBossRushToggleBag(void);
u16 BriskBossRushCycleDifficulty(void);
u16 BriskBossRushPrepare(void);
u16 BriskBossRushCheckActive(void);
u16 BriskBossRushBufferNext(void);
u16 BriskBossRushFight(void);
u16 BriskBossRushEndRound(void);
#endif
