#include "global.h"
#include "brisk_options.h"
#include "dynamax_adventure.h"
#include "event_data.h"
#include "palette.h"
#include "menu.h"
#include "constants/rgb.h"
#include "constants/region_map_sections.h"
#include "constants/map_types.h"

static bool32 sWildShinyContext = FALSE;

bool32 BriskBattleBagAllowed(void)
{
    return !IsDynamaxAdventureBattle() && VarGet(VAR_BRISK_BATTLE_BAG_DISABLED) == 0;
}

bool32 BriskDisplayTypes(void)
{
    return VarGet(VAR_BRISK_TYPES_DISABLED) == 0;
}

bool32 BriskIslandShiniesEnabled(void)
{
    return VarGet(VAR_BRISK_ISLAND_SHINIES_DISABLED) == 0;
}

void BriskSetWildShinyContext(bool32 active)
{
    sWildShinyContext = active;
}

u32 GetBriskWildShinyDenominator(u32 mapSection)
{
    // Only the tropical island maps; the forest approach and indoor caves
    // retain the global setting, including the Adventure's independent odds.
    if (BriskIslandShiniesEnabled()
     && ((mapSection >= MAPSEC_ROUTE_143 && mapSection <= MAPSEC_ROUTE_146)
      || mapSection == MAPSEC_ROUTE_149 || mapSection == MAPSEC_ROUTE_150))
        return 50;
    return GetBriskShinyDenominator();
}

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
    bool32 exterior = gMapHeader.mapType == MAP_TYPE_ROUTE || gMapHeader.mapType == MAP_TYPE_CITY;
    u32 denominator = sWildShinyContext && exterior ? GetBriskWildShinyDenominator(gMapHeader.regionMapSectionId) : GetBriskShinyDenominator();
    return (65536 + denominator / 2) / denominator;
}

bool32 BriskSkipMovePrompts(void) { return VarGet(VAR_BRISK_SKIP_MOVE_PROMPTS) != 0; }
bool32 BriskInfiniteRematches(void) { return VarGet(VAR_BRISK_INFINITE_REMATCHES) != 0; }
bool32 BriskShinyHunting(void) { return VarGet(VAR_BRISK_SHINY_HUNT) != 0; }


bool32 BriskDarkMode(void)
{
    return gSaveBlock1Ptr != NULL && VarGet(VAR_BRISK_DARK_MODE) == 1;
}

// Roles are shared across screens; their palette indices are screen-specific.
// In particular, white can be either lettering or a panel, so never invert it
// globally. Colored status bars and sprite palettes are outside these roles.
#define THEME_PANEL RGB(3, 4, 6)
#define THEME_RAISED RGB(5, 7, 10)
#define THEME_EDGE RGB(9, 12, 16)
#define THEME_INK RGB(1, 2, 3)
#define THEME_TEXT RGB(29, 30, 31)
#define THEME_MUTED RGB(20, 23, 27)
#define THEME_SHADOW RGB(8, 10, 13)
#define THEME_ACCENT RGB(12, 26, 29)

static const u16 sDarkText[16] =
{
    0, THEME_PANEL, THEME_TEXT, THEME_SHADOW,
    RGB(31, 15, 15), RGB(12, 4, 5), RGB(17, 29, 18), RGB(4, 10, 6),
    RGB(16, 24, 31), RGB(4, 7, 13),
    THEME_PANEL, THEME_EDGE, THEME_RAISED, THEME_EDGE, THEME_ACCENT, THEME_INK,
};

static const u16 sDarkOptions[16] =
{
    0, THEME_PANEL, THEME_MUTED, THEME_INK,
    THEME_ACCENT, RGB(3, 10, 13), THEME_TEXT, THEME_SHADOW,
    THEME_INK, THEME_INK, THEME_INK, THEME_INK,
    THEME_INK, THEME_INK, THEME_INK, THEME_INK,
};

static const u16 sDarkBag[2][16] =
{
    {0, THEME_PANEL, THEME_RAISED, THEME_EDGE, RGB(13, 17, 21),
     THEME_EDGE, THEME_INK, RGB(7, 14, 19), RGB(5, 10, 15),
     RGB(25, 28, 27), RGB(15, 21, 22), RGB(31, 12, 12),
     RGB(24, 17, 31), RGB(13, 22, 31), RGB(23, 20, 14), THEME_INK},
    {0, THEME_TEXT, THEME_PANEL, THEME_SHADOW, RGB(23, 20, 14),
     RGB(31, 15, 15), THEME_MUTED, THEME_INK, THEME_INK,
     RGB(12, 20, 23), RGB(8, 14, 18), RGB(10, 5, 15),
     RGB(24, 17, 31), RGB(13, 22, 31), RGB(31, 12, 12), RGB(31, 20, 24)},
};

static const u16 sDarkStorage[16] =
{
    0, THEME_RAISED, THEME_EDGE, THEME_SHADOW, RGB(5, 12, 14),
    RGB(4, 9, 11), THEME_MUTED, RGB(6, 12, 10), THEME_TEXT,
    THEME_ACCENT, RGB(6, 18, 23), RGB(4, 8, 13),
    RGB(9, 17, 18), THEME_INK, RGB(17, 25, 19), THEME_EDGE,
};

static u16 DarkPaletteColor(u16 color, u32 index, enum BriskMenuPaletteStyle style)
{
    u32 slot = index & 15, bank = index / 16;
    if (slot == 0)
        return color;
    switch (style)
    {
    case BRISK_PALETTE_TEXT: return sDarkText[slot];
    case BRISK_PALETTE_OPTIONS: return sDarkOptions[slot];
    case BRISK_PALETTE_BAG:
        return bank < ARRAY_COUNT(sDarkBag) ? sDarkBag[bank][slot] : color;
    case BRISK_PALETTE_STORAGE: return sDarkStorage[slot];
    case BRISK_PALETTE_FRAME:
        // All 20 frames use 14 for the fill and 15 for the outside shadow.
        // Keep the border's original hue and shading order, even for black.
        if (slot == 14) return THEME_PANEL;
        if (slot == 15) return THEME_INK;
        return RGB((color & 31) * 3 / 4, ((color >> 5) & 31) * 3 / 4, ((color >> 10) & 31) * 3 / 4);
    case BRISK_PALETTE_PARTY:
        if (bank == 0)
        {
            if (slot == 1) return THEME_PANEL;
            if (slot == 2) return THEME_TEXT;
            if (slot == 3) return THEME_SHADOW;
            return color; // Gender/status accents.
        }
        if (bank < 3)
        {
            static const u16 backdrop[16] = {0, THEME_EDGE, THEME_PANEL, THEME_RAISED,
                THEME_EDGE, THEME_SHADOW, THEME_INK, THEME_PANEL,
                THEME_PANEL, THEME_PANEL, THEME_MUTED, THEME_RAISED,
                THEME_EDGE, THEME_PANEL, THEME_PANEL, THEME_EDGE};
            return backdrop[slot];
        }
        if (slot == 1) return THEME_INK;
        if (slot == 2 || slot == 13) return THEME_SHADOW;
        if (slot == 3) return THEME_TEXT; // Nickname lettering was already white.
        if (slot >= 4 && slot <= 8)
        {
            // Card surfaces: ordinary slate, selected teal, fainted burgundy,
            // and a green action/switch state. The engine still swaps these
            // exact colors through the existing party animation buffer.
            static const u16 cards[4][3] = {
                {RGB(8, 12, 17), RGB(5, 8, 12), RGB(3, 5, 8)},
                {RGB(10, 20, 24), RGB(6, 14, 19), RGB(3, 8, 12)},
                {RGB(15, 8, 12), RGB(10, 5, 8), RGB(6, 3, 5)},
                {RGB(9, 19, 14), RGB(5, 12, 9), RGB(3, 7, 6)},
            };
            u32 state = bank == 5 || bank == 9 ? 2 : bank == 6 || bank == 10 ? 3 : bank == 7 || bank == 8 ? 1 : 0;
            u32 shade = slot == 4 || slot == 7 ? 0 : slot == 5 ? 1 : 2;
            return cards[state][shade];
        }
        return color; // HP, gender and EXP retain their familiar colors.
    case BRISK_PALETTE_SUMMARY:
        if (bank < 6)
        {
            if (slot == 1) return THEME_TEXT;
            if (slot == 2 || slot == 5) return THEME_PANEL;
            if (slot == 3) return THEME_RAISED;
            if (slot == 4) return THEME_EDGE;
            if (slot == 6) return THEME_SHADOW;
            // Page decoration is subdued without reversing its gradient.
            return RGB((color & 31) / 2, ((color >> 5) & 31) / 2, ((color >> 10) & 31) / 2);
        }
        if (slot == 1) return THEME_TEXT;
        if (slot == 2 || slot == 4) return THEME_SHADOW;
        if (slot == 3) return THEME_MUTED;
        if (slot == 5) return RGB(31, 15, 15);
        if (slot == 6) return RGB(12, 4, 5);
        return color; // Gender symbols and stat changes.
    case BRISK_PALETTE_PP:
        // This asset starts at destination slot 1, with no transparent entry.
        if (slot == 6) return THEME_TEXT;
        if (slot == 7) return THEME_SHADOW;
        return color; // Low-PP warning colors.
    case BRISK_PALETTE_AUTO:
    default:
    {
        u32 r = color & 31, g = (color >> 5) & 31, b = (color >> 10) & 31;
        if (max(r, max(g, b)) - min(r, min(g, b)) > 4)
            return color;
        // Conservative fallback for an unclassified palette. Known screens
        // above use their actual roles instead of guessing from brightness.
        u32 value = (r + g + b) / 3;
        return value <= 7 ? THEME_TEXT : value <= 16 ? THEME_MUTED : value <= 24 ? THEME_EDGE : THEME_PANEL;
    }
    }
}

void BriskCopyMenuPaletteStyled(u16 *dest, const u16 *src, u32 colors, u32 firstColor, enum BriskMenuPaletteStyle style)
{
    bool32 dark = BriskDarkMode();
    for (u32 i = 0; i < colors; i++)
        dest[i] = dark ? DarkPaletteColor(src[i], firstColor + i, style) : src[i];
}

void BriskCopyMenuPalette(u16 *dest, const u16 *src, u32 colors, u32 firstColor)
{
    BriskCopyMenuPaletteStyled(dest, src, colors, firstColor, BRISK_PALETTE_AUTO);
}

void LoadBriskMenuPaletteStyled(const void *src, u32 offset, u32 size, enum BriskMenuPaletteStyle style)
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
        // Styled assets are indexed from their own first color. The fallback
        // uses the destination position for legacy partial palette calls.
        u32 first = style == BRISK_PALETTE_AUTO ? offset + start : start;
        BriskCopyMenuPaletteStyled(palette, colors + start, count, first, style);
        // BG color zero is the opaque backdrop, unlike transparent zero in
        // every other palette bank. Only full-screen UI assets own this slot.
        if (offset == 0 && start == 0
         && (style == BRISK_PALETTE_BAG || style == BRISK_PALETTE_PARTY || style == BRISK_PALETTE_SUMMARY))
            palette[0] = THEME_INK;
        LoadPalette(palette, offset + start, count * sizeof(u16));
    }
}

void LoadBriskMenuPalette(const void *src, u32 offset, u32 size)
{
    // Existing callers include arbitrary partial ranges; keep that contract.
    LoadBriskMenuPaletteStyled(src, offset, size, src == gStandardMenuPalette ? BRISK_PALETTE_TEXT : BRISK_PALETTE_AUTO);
}
