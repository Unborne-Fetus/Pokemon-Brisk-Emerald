#include "global.h"
#include "brisk_fakemon.h"
#include "menu.h"
#include "text.h"
#include "window.h"
#include "constants/characters.h"

static EWRAM_DATA u16 sDescriptionSpecies = 0;
static EWRAM_DATA u8 sDescriptionPage = 0;

u32 BriskFakemon_CopyDexPage(enum Species species, u32 page, u8 *out)
{
    const u8 *description = GetSpeciesPokedexDescription(species);
    u32 lines = 1, line = 0, length = 0;
    for (const u8 *p = description; *p != EOS; p++)
        lines += *p == CHAR_NEWLINE;
    u32 pages = (lines + 3) / 4;
    page %= pages;
    for (const u8 *p = description; *p != EOS; p++)
    {
        if (line >= page * 4 && line < (page + 1) * 4 && length < 254)
        {
            if (*p != CHAR_NEWLINE || line < (page + 1) * 4 - 1)
                out[length++] = *p;
        }
        line += *p == CHAR_NEWLINE;
    }
    out[length] = EOS;
    return pages;
}

static void DrawDescription(void)
{
    u8 page[256];
    const u8 colors[] = {TEXT_COLOR_TRANSPARENT, TEXT_DYNAMIC_COLOR_6, TEXT_COLOR_LIGHT_GRAY};
    u32 pages = BriskFakemon_CopyDexPage(sDescriptionSpecies, sDescriptionPage, page);
    FillWindowPixelRect(0, PIXEL_FILL(0), 0, 93, DISPLAY_WIDTH, 64);
    AddTextPrinterParameterized4(0, FONT_SMALL, 4, 93, 0, 0, colors, TEXT_SKIP_DRAW, page);
    if (pages > 1)
        AddTextPrinterParameterized4(0, FONT_SMALL, 154, 144, 0, 0, colors, TEXT_SKIP_DRAW, COMPOUND_STRING("START: More"));
    CopyWindowToVram(0, COPYWIN_GFX);
}

bool32 BriskFakemon_PrintDescription(enum Species species)
{
    sDescriptionSpecies = IS_BRISK_FAKEMON(species) ? species : SPECIES_NONE;
    sDescriptionPage = 0;
    if (sDescriptionSpecies == SPECIES_NONE)
        return FALSE;
    DrawDescription();
    return TRUE;
}

bool32 BriskFakemon_NextDexPage(void)
{
    u8 page[256];
    if (sDescriptionSpecies == SPECIES_NONE)
        return FALSE;
    u32 pages = BriskFakemon_CopyDexPage(sDescriptionSpecies, 0, page);
    if (pages < 2)
        return FALSE;
    sDescriptionPage = (sDescriptionPage + 1) % pages;
    DrawDescription();
    return TRUE;
}
