#include "global.h"
#include "brisk_options.h"
#include "event_data.h"
#include "palette.h"
#include "constants/rgb.h"

u32 GetBriskShinyDenominatorForSetting(u32 setting)
{
    static const u16 odds[BRISK_SHINY_CHOICES] = {8192, 4096, 2048, 1024, 512, 256, 128, 100, 50};
    // Zero keeps existing saves at Brisk's previous 1/512 baseline.
    return setting >= 1 && setting <= ARRAY_COUNT(odds) ? odds[setting - 1] : 512;
}

u32 GetBriskShinyDenominator(void)
{
    return GetBriskShinyDenominatorForSetting(VarGet(VAR_BRISK_SHINY_ODDS));
}

u32 GetBriskShinyThreshold(void)
{
    u32 denominator = GetBriskShinyDenominator();
    return (65536 + denominator / 2) / denominator;
}

bool32 BriskSkipMovePrompts(void) { return VarGet(VAR_BRISK_SKIP_MOVE_PROMPTS) != 0; }
bool32 BriskInfiniteRematches(void) { return VarGet(VAR_BRISK_INFINITE_REMATCHES) != 0; }
bool32 BriskShinyHunting(void) { return VarGet(VAR_BRISK_SHINY_HUNT) != 0; }


bool32 BriskDarkMode(void)
{
    return gSaveBlock1Ptr != NULL && VarGet(VAR_BRISK_DARK_MODE) == 1;
}

void BriskCopyMenuPalette(u16 *dest, const u16 *src, u32 colors, u32 firstColor)
{
    bool32 dark = BriskDarkMode();
    for (u32 i = 0; i < colors; i++)
    {
        u16 color = src[i];
        u32 r = color & 31, g = (color >> 5) & 31, b = (color >> 10) & 31;
        u32 high = max(r, max(g, b)), low = min(r, min(g, b));
        // Recolor neutral UI surfaces/text only. Preserve transparency and accents.
        if (dark && ((firstColor + i) & 15) != 0 && high - low <= 4)
        {
            u32 shade = 29 - (((r + g + b) / 3) * 26 + 15) / 31;
            color = RGB(shade, min(shade + 1, 31), min(shade + 3, 31));
        }
        dest[i] = color;
    }
}

void LoadBriskMenuPalette(const void *src, u32 offset, u32 size)
{
    const u16 *colors = src;
    u16 palette[16];
    if (!BriskDarkMode())
    {
        LoadPalette(src, offset, size);
        return;
    }
    for (u32 start = 0; start < size / sizeof(u16); start += ARRAY_COUNT(palette))
    {
        u32 count = min(ARRAY_COUNT(palette), size / sizeof(u16) - start);
        BriskCopyMenuPalette(palette, colors + start, count, offset + start);
        LoadPalette(palette, offset + start, count * sizeof(u16));
    }
}
