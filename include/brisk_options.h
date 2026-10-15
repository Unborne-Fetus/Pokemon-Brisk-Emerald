#ifndef GUARD_BRISK_OPTIONS_H
#define GUARD_BRISK_OPTIONS_H

#define BRISK_SHINY_CHOICES 9
u32 GetBriskShinyDenominator(void);
u32 GetBriskShinyDenominatorForSetting(u32 setting);
u32 GetBriskShinyThreshold(void);
bool32 BriskSkipMovePrompts(void);
bool32 BriskInfiniteRematches(void);
bool32 BriskShinyHunting(void);

#endif
