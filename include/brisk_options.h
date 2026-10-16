#ifndef GUARD_BRISK_OPTIONS_H
#define GUARD_BRISK_OPTIONS_H

#define BRISK_SHINY_CHOICES 9
u32 GetBriskShinyDenominator(void);
u32 GetBriskShinyDenominatorForSetting(u32 setting);
u32 GetBriskShinyThreshold(void);
bool32 BriskSkipMovePrompts(void);
bool32 BriskInfiniteRematches(void);
bool32 BriskShinyHunting(void);

bool32 BriskDarkMode(void);
void BriskCopyMenuPalette(u16 *dest, const u16 *src, u32 colors, u32 firstColor);
void LoadBriskMenuPalette(const void *src, u32 offset, u32 size);

#endif
