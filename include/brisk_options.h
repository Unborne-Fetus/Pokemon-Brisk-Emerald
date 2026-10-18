#ifndef GUARD_BRISK_OPTIONS_H
#define GUARD_BRISK_OPTIONS_H

#define BRISK_SHINY_CHOICES 9
u32 GetBriskShinyDenominator(void);
u32 GetBriskShinyDenominatorForSetting(u32 setting);
u32 GetBriskShinyThreshold(void);
u32 GetBriskWildShinyDenominator(u32 mapSection);
bool32 BriskIslandShiniesEnabled(void);
bool32 BriskDisplayTypes(void);
void BriskSetWildShinyContext(bool32 active);
bool32 BriskSkipMovePrompts(void);
bool32 BriskInfiniteRematches(void);
bool32 BriskShinyHunting(void);

enum BriskMenuPaletteStyle
{
    BRISK_PALETTE_AUTO,
    BRISK_PALETTE_TEXT,
    BRISK_PALETTE_FRAME,
    BRISK_PALETTE_OPTIONS,
    BRISK_PALETTE_BAG,
    BRISK_PALETTE_PARTY,
    BRISK_PALETTE_SUMMARY,
    BRISK_PALETTE_STORAGE,
    BRISK_PALETTE_PP,
};

bool32 BriskDarkMode(void);
void BriskCopyMenuPaletteStyled(u16 *dest, const u16 *src, u32 colors, u32 firstColor, enum BriskMenuPaletteStyle style);
void LoadBriskMenuPaletteStyled(const void *src, u32 offset, u32 size, enum BriskMenuPaletteStyle style);
void BriskCopyMenuPalette(u16 *dest, const u16 *src, u32 colors, u32 firstColor);
void LoadBriskMenuPalette(const void *src, u32 offset, u32 size);

#endif
