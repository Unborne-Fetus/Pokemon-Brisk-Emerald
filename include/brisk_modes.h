#ifndef GUARD_BRISK_MODES_H
#define GUARD_BRISK_MODES_H

#define BRISK_MODE_NORMAL 0
#define BRISK_MODE_EASY 1
#define BRISK_MODE_HARD 2
#define BRISK_MODE_EXTREME 3
#define BRISK_RAND_WILD (1 << 0)
#define BRISK_RAND_TRAINERS (1 << 1)
#define BRISK_RAND_ITEMS (1 << 2)
#define BRISK_RAND_IMPORTANT (1 << 3)
#define BRISK_RAND_ALL (1 << 4)

struct Pokemon;

u32 GetBriskDifficulty(void);
u32 GetBriskPriceMultiplier(void);
u64 GetBriskTrainerAiFlags(u64 flags);
u32 GetBriskBattleStyle(u32 preference);
bool32 BriskBattleItemsAllowed(void);
bool32 BriskCanUseBattleItem(enum Item item);
bool32 BriskModernExpShare(void);
bool32 BriskModernExpShareEnabled(void);
bool32 BriskMonGetsExpShare(struct Pokemon *mon);
u32 ScaleBriskSharedExp(u32 exp);
void SetBriskExpShareMode(bool32 modern);
u32 ScaleBriskLevel(u32 level, u32 salt);
u32 ScaleBriskTrainerLevel(u32 level, u32 salt);
u32 ScaleBriskOrdinaryTrainerLevel(u32 level, u32 highestLevel);
bool32 BriskRandomizerEnabled(u32 category);
bool32 BriskIsImportantTrainer(u32 trainerClass);
enum Species BriskRandomSpecies(enum Species species, u32 category);
enum Species BriskTraitSpecies(enum Species species, u32 salt);
enum Item BriskRandomItem(enum Item item);
void BriskRandomizeScriptItem(void);

#endif
