#include "global.h"
#include "brisk_options.h"
#include "event_data.h"
#include "graphics.h"
#include "main.h"
#include "menu.h"
#include "option_menu.h"
#include "palette.h"
#include "pokedex_plus_hgss.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "window.h"
#include "constants/rgb.h"
#include "test/test.h"

TEST("Brisk theme keeps light defaults and preserves transparency and accent colors")
{
    const u16 source[16] = {RGB(0,0,0), RGB(31,31,31), RGB(0,0,0), RGB(26,26,26), RGB(31,0,0), RGB(0,31,0), RGB(0,0,31)};
    u16 palette[16];
    VarSet(VAR_BRISK_DARK_MODE, 0);
    EXPECT(!BriskDarkMode());
    BriskCopyMenuPalette(palette, source, ARRAY_COUNT(source), 0);
    EXPECT_EQ(memcmp(palette, source, sizeof(source)), 0);
    VarSet(VAR_BRISK_DARK_MODE, 1);
    EXPECT(BriskDarkMode());
    BriskCopyMenuPalette(palette, source, ARRAY_COUNT(source), 0);
    EXPECT_EQ(palette[0], source[0]);
    EXPECT_LE(palette[1] & 31, 4); // Bright surface becomes dark.
    EXPECT_GE(palette[2] & 31, 28); // Dark text becomes light.
    for (u32 i = 4; i < 7; i++)
        EXPECT_EQ(palette[i], source[i]);
    VarSet(VAR_BRISK_DARK_MODE, 0xFFFF);
    EXPECT(!BriskDarkMode()); // Unknown save values preserve the original appearance.
    VarSet(VAR_BRISK_DARK_MODE, 0);
}

TEST("Brisk theme loads partial palette ranges without recoloring adjacent slots")
{
    const u16 source[3] = {RGB(31,31,31), RGB(0,0,0), RGB(31,0,0)};
    const u32 start = BG_PLTT_ID(8) + 15;
    const u16 sentinel = RGB(4,11,23);
    gPlttBufferUnfaded[start - 1] = sentinel;
    gPlttBufferUnfaded[start + 3] = sentinel;
    VarSet(VAR_BRISK_DARK_MODE, 1);
    LoadBriskMenuPalette(source, start, sizeof(source));
    EXPECT_LE(gPlttBufferUnfaded[start] & 31, 4);
    EXPECT_EQ(gPlttBufferUnfaded[start + 1], RGB(0,0,0)); // Next palette's transparent slot.
    EXPECT_EQ(gPlttBufferUnfaded[start + 2], source[2]);
    EXPECT_EQ(gPlttBufferUnfaded[start - 1], sentinel);
    EXPECT_EQ(gPlttBufferUnfaded[start + 3], sentinel);
    VarSet(VAR_BRISK_DARK_MODE, 0);
    LoadBriskMenuPalette(source, start, sizeof(source));
    EXPECT_EQ(memcmp(&gPlttBufferUnfaded[start], source, sizeof(source)), 0);
}

TEST("Brisk theme restores original window palettes and keeps cached frames independent")
{
    VarSet(VAR_BRISK_DARK_MODE, 0);
    u16 original[16];
    memcpy(original, GetTextWindowPalette(3), sizeof(original));
    const struct TilesPal *light = GetWindowFrameTilesPal(0);
    const u8 *tiles = light->tiles;
    u16 frame[16];
    memcpy(frame, light->pal, sizeof(frame));
    VarSet(VAR_BRISK_DARK_MODE, 1);
    EXPECT_NE(GetTextWindowPalette(3)[1], original[1]);
    EXPECT_EQ(GetWindowFrameTilesPal(0)->tiles, tiles);
    const struct TilesPal *dark = GetWindowFrameTilesPal(0);
    const u16 *pal = dark->pal;
    const u8 *otherTiles = GetWindowFrameTilesPal(1)->tiles;
    EXPECT_EQ(dark->pal, pal);
    EXPECT_EQ(dark->tiles, tiles);
    EXPECT_NE(otherTiles, tiles);
    EXPECT(GetOverworldTextboxPalettePtr() != NULL);
    VarSet(VAR_BRISK_DARK_MODE, 0);
    EXPECT_EQ(memcmp(original, GetTextWindowPalette(3), sizeof(original)), 0);
    EXPECT_EQ(memcmp(frame, GetWindowFrameTilesPal(0)->pal, sizeof(frame)), 0);
}

TEST("Brisk theme option previews immediately and survives closing and reopening")
{
    VarSet(VAR_BRISK_DARK_MODE, 0);
    u8 task = Test_OpenBriskThemeOption();
    u16 lightBackground = gPlttBufferUnfaded[0];
    gMain.newKeys = 0;
    gMain.newAndRepeatedKeys = DPAD_RIGHT;
    RunTasks();
    EXPECT(BriskDarkMode());
    EXPECT_NE(gPlttBufferUnfaded[0], lightBackground);
    Test_CloseBriskThemeOption(task);
    gMain.newAndRepeatedKeys = 0;
    task = Test_OpenBriskThemeOption();
    EXPECT(BriskDarkMode());
    EXPECT_NE(gPlttBufferUnfaded[0], lightBackground);
    gMain.newKeys = A_BUTTON;
    RunTasks();
    EXPECT(!BriskDarkMode());
    EXPECT_EQ(gPlttBufferUnfaded[0], lightBackground);
    gMain.newKeys = 0;
    gMain.newAndRepeatedKeys = 0;
    Test_CloseBriskThemeOption(task);
}

TEST("Brisk theme stays light before save pointers are initialized")
{
    const u16 source[3] = {RGB(0,0,0), RGB(31,31,31), RGB(0,0,0)};
    u16 copy[3];
    struct SaveBlock1 *saved = gSaveBlock1Ptr;
    gSaveBlock1Ptr = NULL;
    bool32 dark = BriskDarkMode();
    BriskCopyMenuPalette(copy, source, ARRAY_COUNT(source), 0);
    gSaveBlock1Ptr = saved;
    EXPECT(!dark);
    EXPECT_EQ(memcmp(copy, source, sizeof(source)), 0);
}

TEST("Brisk theme gives identical whites distinct text, surface and border roles")
{
    u16 source[16], text[16], frame[16], party[16];
    for (u32 i = 0; i < ARRAY_COUNT(source); i++)
        source[i] = RGB(31, 31, 31);
    VarSet(VAR_BRISK_DARK_MODE, 1);
    BriskCopyMenuPaletteStyled(text, source, 16, 0, BRISK_PALETTE_TEXT);
    BriskCopyMenuPaletteStyled(frame, source, 16, 0, BRISK_PALETTE_FRAME);
    BriskCopyMenuPaletteStyled(party, source, 16, 48, BRISK_PALETTE_PARTY);
    EXPECT_EQ(text[1], RGB(3, 4, 6));
    EXPECT_EQ(text[2], RGB(29, 30, 31));
    EXPECT_EQ(frame[14], text[1]);
    EXPECT_NE(frame[1], text[1]); // Border remains brighter than the fill.
    EXPECT_EQ(party[3], text[2]); // Already-white nickname text stays light.
    EXPECT_NE(party[4], party[3]);
    EXPECT_NE(party[4], text[1]); // Card is raised above the main panel.
    for (u32 style = BRISK_PALETTE_TEXT; style <= BRISK_PALETTE_PP; style++)
    {
        BriskCopyMenuPaletteStyled(text, source, 16, 0, style);
        EXPECT_EQ(text[0], source[0]);
    }
    VarSet(VAR_BRISK_DARK_MODE, 0);
    for (u32 style = BRISK_PALETTE_TEXT; style <= BRISK_PALETTE_PP; style++)
    {
        BriskCopyMenuPaletteStyled(text, source, 16, 0, style);
        EXPECT_EQ(memcmp(text, source, sizeof(source)), 0);
    }
}

TEST("Brisk theme preserves party HP and gender colors while distinguishing selection and fainting")
{
    u16 palette[176];
    VarSet(VAR_BRISK_DARK_MODE, 1);
    BriskCopyMenuPaletteStyled(palette, gPartyMenuBg_Pal, ARRAY_COUNT(palette), 0, BRISK_PALETTE_PARTY);
    for (u32 bank = 3; bank < 11; bank++)
    {
        for (u32 slot = 9; slot <= 12; slot++)
            EXPECT_EQ(palette[bank * 16 + slot], gPartyMenuBg_Pal[bank * 16 + slot]);
        EXPECT_EQ(palette[bank * 16 + 3], RGB(29, 30, 31));
    }
    EXPECT_NE(palette[3 * 16 + 4], palette[7 * 16 + 4]);
    EXPECT_NE(palette[3 * 16 + 4], palette[5 * 16 + 4]);
    EXPECT_NE(palette[6 * 16 + 4], palette[5 * 16 + 4]);
    VarSet(VAR_BRISK_DARK_MODE, 0);
}

TEST("Brisk theme styles multiple palette banks without changing PP warnings or neighboring colors")
{
    u16 copy[32];
    VarSet(VAR_BRISK_DARK_MODE, 1);
    BriskCopyMenuPaletteStyled(copy, gBagScreenMale_Pal, 32, 0, BRISK_PALETTE_BAG);
    EXPECT_EQ(copy[1], RGB(3, 4, 6));
    EXPECT_EQ(copy[17], RGB(29, 30, 31));
    EXPECT_EQ(copy[0], gBagScreenMale_Pal[0]);
    EXPECT_EQ(copy[16], gBagScreenMale_Pal[16]);
    LoadBriskMenuPaletteStyled(gBagScreenMale_Pal, 0, PLTT_SIZEOF(32), BRISK_PALETTE_BAG);
    EXPECT_EQ(gPlttBufferUnfaded[0], RGB(1, 2, 3));
    EXPECT_EQ(gPlttBufferUnfaded[16], gBagScreenMale_Pal[16]);
    LoadBriskMenuPaletteStyled(gPPTextPalette, BG_PLTT_ID(8) + 1, PLTT_SIZEOF(15), BRISK_PALETTE_PP);
    for (u32 i = 0; i < 6; i++)
        EXPECT_EQ(gPlttBufferUnfaded[BG_PLTT_ID(8) + 1 + i], gPPTextPalette[i]);
    EXPECT_EQ(gPlttBufferUnfaded[BG_PLTT_ID(8) + 7], RGB(29, 30, 31));
    EXPECT_EQ(gPlttBufferUnfaded[BG_PLTT_ID(8) + 8], RGB(8, 10, 13));
    VarSet(VAR_BRISK_DARK_MODE, 0);
}

TEST("Brisk dark battle text separates message ink from command panel fills")
{
    u16 box[32], text[16];
    VarSet(VAR_BRISK_DARK_MODE, 1);
    BriskCopyMenuPaletteStyled(box, gBattleTextboxPalette, 32, 0, BRISK_PALETTE_BATTLE_BOX);
    BriskCopyMenuPaletteStyled(text, gBattleWindowTextPalette, 16, 0, BRISK_PALETTE_BATTLE_TEXT);
    EXPECT_EQ(box[1], RGB(29,30,31)); // Message ink.
    EXPECT_EQ(box[15], RGB(3,4,6)); // Message fill.
    EXPECT_EQ(text[13], box[1]); // Command ink.
    EXPECT_EQ(text[14], RGB(5,7,10)); // Separate raised command panel.
    EXPECT_NE(text[14], box[15]);
    for (u32 i = 1; i <= 4; i++)
        EXPECT_EQ(text[i], gBattleWindowTextPalette[i]); // PP warning pairs.
    EXPECT_EQ(box[0], gBattleTextboxPalette[0]);
    EXPECT_EQ(box[16], gBattleTextboxPalette[16]);
    VarSet(VAR_BRISK_DARK_MODE, 0);
}

TEST("Brisk expanded themes preserve original assets when returning to light mode")
{
    u16 source[32], copy[32];
    for (u32 i = 0; i < ARRAY_COUNT(source); i++)
        source[i] = RGB(i & 31, (i * 3) & 31, (i * 7) & 31);
    for (u32 style = BRISK_PALETTE_BATTLE_BOX; style <= BRISK_PALETTE_TRAINER_CARD; style++)
    {
        VarSet(VAR_BRISK_DARK_MODE, 1);
        BriskCopyMenuPaletteStyled(copy, source, 32, 0, style);
        EXPECT_EQ(copy[0], source[0]);
        EXPECT_EQ(copy[16], source[16]);
        VarSet(VAR_BRISK_DARK_MODE, 0);
        BriskCopyMenuPaletteStyled(copy, source, 32, 0, style);
        EXPECT_EQ(memcmp(copy, source, sizeof(source)), 0);
    }
}

TEST("Brisk naming and PC text have light lettering on dark fills without touching symbols")
{
    u16 source[16], keyboard[16], pc[16];
    for (u32 i = 0; i < 16; i++)
        source[i] = RGB(i, i, i);
    VarSet(VAR_BRISK_DARK_MODE, 1);
    BriskCopyMenuPaletteStyled(keyboard, source, 16, 0, BRISK_PALETTE_KEYBOARD);
    BriskCopyMenuPaletteStyled(pc, source, 16, 0, BRISK_PALETTE_STORAGE_TEXT);
    EXPECT_EQ(keyboard[1], RGB(3,4,6));
    EXPECT_EQ(keyboard[2], RGB(29,30,31));
    EXPECT_EQ(memcmp(pc, keyboard, sizeof(pc)), 0);
    for (u32 i = 4; i < 16; i++)
        EXPECT_EQ(keyboard[i], source[i]);
    VarSet(VAR_BRISK_DARK_MODE, 0);
}

TEST("Brisk dark borders stay visible even when the original frame is black")
{
    u16 source[16] = {0}, frame[16];
    VarSet(VAR_BRISK_DARK_MODE, 1);
    BriskCopyMenuPaletteStyled(frame, source, 16, 0, BRISK_PALETTE_FRAME);
    for (u32 i = 1; i < 14; i++)
    {
        EXPECT_GT(frame[i] & 31, frame[14] & 31);
        EXPECT_GT((frame[i] >> 10) & 31, (frame[14] >> 10) & 31);
    }
    VarSet(VAR_BRISK_DARK_MODE, 0);
}

TEST("Brisk dark options distinguish labels, muted choices, and cyan selection")
{
    u16 source[16] = {0}, copy[16];
    VarSet(VAR_BRISK_DARK_MODE, 1);
    BriskCopyMenuPaletteStyled(copy, source, 16, 0, BRISK_PALETTE_OPTIONS);
    EXPECT_EQ(copy[TEXT_COLOR_DARK_GRAY], RGB(29,30,31)); // Label ink.
    EXPECT_EQ(copy[TEXT_COLOR_GREEN], RGB(20,23,27)); // Unselected value.
    EXPECT_EQ(copy[TEXT_COLOR_RED], RGB(12,26,29)); // Selected value.
    EXPECT_NE(copy[TEXT_COLOR_GREEN], copy[TEXT_COLOR_RED]);
    EXPECT_NE(copy[TEXT_COLOR_DARK_GRAY], copy[TEXT_COLOR_RED]);
    VarSet(VAR_BRISK_DARK_MODE, 0);
}

TEST("Brisk Pokedex follows appearance without reading past its three palette banks")
{
    const u16 sentinel = RGB(11,17,23);
    gPlttBufferUnfaded[BG_PLTT_ID(3)] = sentinel;
    VarSet(VAR_BRISK_DARK_MODE, 1);
    EXPECT(TryLoadPokedexBgPalette_HGSS(FALSE));
    EXPECT_EQ(gPlttBufferUnfaded[1], RGB(3,4,6));
    EXPECT_EQ(gPlttBufferUnfaded[14], RGB(29,30,31));
    EXPECT_EQ(gPlttBufferUnfaded[BG_PLTT_ID(6) + 1], RGB(29,30,31));
    EXPECT_EQ(gPlttBufferUnfaded[BG_PLTT_ID(7) + 1], RGB(3,4,6));
    EXPECT_EQ(gPlttBufferUnfaded[BG_PLTT_ID(7) + 15], RGB(29,30,31));
    EXPECT_EQ(gPlttBufferUnfaded[BG_PLTT_ID(3)], sentinel);
    VarSet(VAR_BRISK_DARK_MODE, 0);
    EXPECT(TryLoadPokedexBgPalette_HGSS(FALSE));
    EXPECT_NE(gPlttBufferUnfaded[1], RGB(3,4,6));
    EXPECT_EQ(gPlttBufferUnfaded[BG_PLTT_ID(3)], sentinel);
}
