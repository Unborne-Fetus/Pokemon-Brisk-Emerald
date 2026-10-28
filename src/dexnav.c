#include "global.h"
#include "brisk_options.h"
#include "battle_main.h"
#include "battle_setup.h"
#include "bg.h"
#include "data.h"
#include "daycare.h"
#include "decompress.h"
#include "dexnav.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "event_scripts.h"
#include "field_effect.h"
#include "field_effect_helpers.h"
#include "field_message_box.h"
#include "field_player_avatar.h"
#include "field_screen_effect.h"
#include "fieldmap.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "item.h"
#include "international_string_util.h"
#include "m4a.h"
#include "map_name_popup.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "menu_helpers.h"
#include "metatile_behavior.h"
#include "move.h"
#include "overworld.h"
#include "palette.h"
#include "party_menu.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_icon.h"
#include "pokemon_summary_screen.h"
#include "random.h"
#include "region_map.h"
#include "rtc.h"
#include "scanline_effect.h"
#include "script.h"
#include "script_pokemon_util.h"
#include "sound.h"
#include "sprite.h"
#include "start_menu.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "wild_encounter.h"
#include "window.h"
#include "constants/species.h"
#include "constants/maps.h"
#include "constants/field_effects.h"
#include "constants/items.h"
#include "constants/songs.h"
#include "constants/abilities.h"
#include "constants/rgb.h"
#include "constants/region_map_sections.h"
#include "gba/m4a_internal.h"

#if DEXNAV_ENABLED
STATIC_ASSERT(DN_FLAG_SEARCHING != 0, DNFlagSearching_Must_Not_Be_Zero);
STATIC_ASSERT(DN_FLAG_DETECTOR_MODE != 0, DNFlagDetectorMode_Must_Not_Be_Zero);
STATIC_ASSERT(DN_VAR_SPECIES >= VARS_START && DN_VAR_SPECIES <= VARS_END, DNVarSpecies_Must_Be_A_Save_Var);
STATIC_ASSERT(DN_VAR_STEP_COUNTER >= VARS_START && DN_VAR_STEP_COUNTER <= VARS_END, DNVarStepCounter_Must_Be_A_Save_Var);
#endif

// Defines
enum WindowIds
{
    WINDOW_INFO,
    WINDOW_REGISTERED,
    WINDOW_PAGES,
    WINDOW_COUNT,
};

enum Statuses
{
    STATUS_INVALID_SEARCH,
    STATUS_CHOOSE_MON,
    STATUS_LOCKED,
    STATUS_NO_DATA,
    STATUS_INCORRECT_AREA,
};

struct DexNavSearch
{
    enum Species species;
    enum Move moves[MAX_MON_MOVES];
    enum Item heldItem;
    u8 abilityNum;
    u8 potential;
    u8 searchLevel;
    u8 monLevel;
    u8 proximity;
    u8 environment;
    s16 tileX;
    s16 tileY;
    u8 fldEffSpriteId;
    u8 fldEffId;
    u8 movementCount;
    u8 windowId;
    u8 iconSpriteId;
    u8 eyeSpriteId;
    u8 itemSpriteId;
    u8 starSpriteIds[3];
    u8 ownedIconSpriteId;
    u8 exclamationSpriteId;
    u32 startingTime;
    u8 hiddenSearch:1;
    u8 isHiddenMon:1;
    u8 unk:6;
    u16 palBuffer[16];
};

struct DexNavGUI
{
    MainCallback savedCallback;
    u8 state;
    u8 cursorSpriteId;
    enum Species landSpecies[NUM_LAND_MONS_ENCOUNTER_SLOTS];
    enum Species waterSpecies[NUM_WATER_MONS_ENCOUNTER_SLOTS];
    enum Species hiddenSpecies[NUM_HIDDEN_MONS_ENCOUNTER_SLOTS];
    u8 speciesCount[3];
    u8 page[3];
    u8 iconSpriteIds[DEXNAV_VISIBLE_ICONS];
    bool8 iconIsMon[DEXNAV_VISIBLE_ICONS];
    u8 cursorRow;
    u8 cursorCol;
    u8 environment;
    u8 potential;
    u8 typeIconSpriteIds[2];
    u8 starSpriteIds[3];
};

// RAM

EWRAM_DATA static struct DexNavSearch *sDexNavSearchDataPtr = NULL;
EWRAM_DATA static struct DexNavGUI *sDexNavUiDataPtr = NULL;
EWRAM_DATA static u8 *sBg1TilemapBuffer = NULL;
EWRAM_DATA enum Species gDexNavSpecies = SPECIES_NONE;

//// Function Declarations
//GUI
static void Task_DexNavWaitFadeIn(u8 taskId);
static void Task_DexNavMain(u8 taskId);
static void PrintCurrentSpeciesInfo(void);
// SEARCH
static bool8 TryStartHiddenMonFieldEffect(enum EncounterType environment, u8 xSize, u8 ySize, bool8 smallScan);
static void DexNavGenerateMoveset(enum Species species, u8 searchLevel, u8 encounterLevel, u16 *moveDst);
static enum Item DexNavGenerateHeldItem(enum Species species, u8 searchLevel);
static u8 DexNavGetAbilityNum(enum Species species, u8 searchLevel);
static u8 DexNavGeneratePotential(u8 searchLevel);
static u8 DexNavTryGenerateMonLevel(enum Species species, enum EncounterType environment);
static u8 GetEncounterLevelFromMapData(enum Species species, enum EncounterType environment);
static void CreateDexNavWildMon(enum Species species, u8 potential, u8 level, u8 abilityNum, enum Item item, enum Move *moves);
static u8 GetPlayerDistance(s16 x, s16 y);
static u8 DexNavPickTile(enum EncounterType environment, u8 xSize, u8 ySize, bool8 smallScan);
static void DexNavProximityUpdate(void);
static void DexNavDrawIcons(void);
static void ClearDexNavIcons(void);
static void PrintDexNavPages(void);
static void DexNavUpdateSearchWindow(u8 proximity, u8 searchLevel);
// HIDDEN MONS
static void DexNavDrawHiddenIcons(void);
static void DrawHiddenSearchWindow(u8 width);
static void RevealHiddenMon(void);

//// Const Data
// gui image data
static const u32 sDexNavGuiTiles[] = INCGFX_U32("graphics/dexnav/gui_tiles.png", ".4bpp.smol");
static const u32 sDexNavGuiTilemap[] = INCGFX_U32("graphics/dexnav/gui_tilemap.bin", ".smolTM");
static const u32 sDexNavGuiPal[] = INCGFX_U32("graphics/dexnav/gui.pal", ".gbapal");
static const u16 sDexNavDarkGuiPal[16] = {
    RGB(1,2,3), RGB(3,4,6), RGB(12,26,29), RGB(29,30,31),
    RGB(3,10,14), RGB(5,15,21), RGB(10,18,12), RGB(9,12,16),
    RGB(9,20,25), RGB(4,13,17), RGB(7,14,10), RGB(12,16,20),
    RGB(12,22,16), RGB(8,17,23), RGB(8,13,18), RGB(2,7,10),
};

static const u32 sSelectionCursorGfx[] = INCGFX_U32("graphics/dexnav/cursor.png", ".4bpp.smol");
static const u16 sSelectionCursorPal[] = INCGFX_U16("graphics/dexnav/cursor.png", ".gbapal");
static const u32 sCapturedAllMonsTiles[] = INCGFX_U32("graphics/dexnav/captured_all.png", ".4bpp.smol");  //uses selection cursor pal

static const u32 sNoDataGfx[] = INCGFX_U32("graphics/dexnav/no_data.png", ".4bpp.smol");

// searching image data
static const u32 sPotentialStarGfx[] = INCGFX_U32("graphics/dexnav/star.png", ".4bpp.smol");
static const u32 sHiddenSearchIconGfx[] = INCGFX_U32("graphics/dexnav/hidden_search.png", ".4bpp.smol");
static const u32 sOwnedIconGfx[] = INCGFX_U32("graphics/dexnav/owned_icon.png", ".4bpp.smol");
static const u32 sHiddenMonIconGfx[] = INCGFX_U32("graphics/dexnav/hidden.png", ".4bpp.smol");

// strings
static const u8 sText_DexNav_NoInfo[] = _("--------");
static const u8 sText_DexNav_CaptureToSee[] = _("Capture first!");
static const u8 sText_DexNav_PressSelectToRegister[] = _("SELECT: REG.");
static const u8 sText_DexNav_SearchForRegisteredSpecies[] = _("Search {STR_VAR_1}");
static const u8 sText_DexNav_NotFoundHere[] = _("This Pokémon cannot be found here!");
static const u8 sText_ThreeQmarks[] = _("???");
static const u8 sText_SearchLevel[] = _("SEARCH {LV}. {STR_VAR_1}");
static const u8 sText_MonLevel[] = _("{LV}. {STR_VAR_1}");
static const u8 sText_EggMove[] = _("MOVE: {STR_VAR_1}");
static const u8 sText_HeldItem[] = _("{STR_VAR_1}");
static const u8 sText_StartExit[] = _("{START_BUTTON} EXIT");
static const u8 sText_DexNavChain[] = _("{NO} {STR_VAR_1}");
static const u8 sText_DexNavChainLong[] = _("{NO}{STR_VAR_1}");

static const u8 sText_ArrowLeft[] = _("{LEFT_ARROW}");
static const u8 sText_ArrowRight[] = _("{RIGHT_ARROW}");
static const u8 sText_ArrowUp[] = _("{UP_ARROW}");
static const u8 sText_ArrowDown[] = _("{DOWN_ARROW}");

static const struct WindowTemplate sDexNavGuiWindowTemplates[] =
{
    [WINDOW_INFO] =
    {
        .bg = 0,
        .tilemapLeft = 21,
        .tilemapTop = 5,
        .width = 9,
        .height = 15,
        .paletteNum = 15,
        .baseBlock = 1,
    },
    [WINDOW_REGISTERED] =
    {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 30,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 200,
    },
    [WINDOW_PAGES] =
    {
        .bg = 0, .tilemapLeft = 21, .tilemapTop = 2,
        .width = 9, .height = 2, .paletteNum = 15, .baseBlock = 260,
    },
    DUMMY_WIN_TEMPLATE
};

//gui font
static const u8 sFontColor_Black[3] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY};
static const u8 sFontColor_WhiteLight[3] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY};
static const u8 sFontColor_WhiteDark[3] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY};
#define sFontColor_White (BriskDarkMode() ? sFontColor_WhiteDark : sFontColor_WhiteLight)
//search window font
static const u8 sSearchFontColor[3] = {0, 15, 13};

static const struct OamData sNoDataIconOam =
{
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 1,
};

static const struct OamData sHeldItemOam =
{
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(8x8),
    .size = SPRITE_SIZE(8x8),
    .priority = 0,
    .paletteNum = 13,
};

static const struct OamData sCapturedAllOam =
{
    .y = 0,
    .affineMode = 1,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0,
    .priority = 0, //Highest
    .paletteNum = 12,
    .affineParam = 0,
};

static const struct OamData sSearchIconOam =
{
    .y = 0,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = 0,
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 0, // above BG layers
    .paletteNum = 13,
    .affineParam = 0
};

static const struct OamData sSelectionCursorOam =
{
    .y = 0,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = 0,
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 0, // above BG layers
    .paletteNum = 12,
    .affineParam = 0
};

static const struct OamData sSightOam =
{
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(16x8),
    .size = SPRITE_SIZE(16x8),
    .priority = 0,
};
static const union AnimCmd sAnimCmdSight0[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END
};
static const union AnimCmd sAnimCmdSight1[] =
{
    ANIMCMD_FRAME(2, 1),
    ANIMCMD_END
};
static const union AnimCmd sAnimCmdSight2[] =
{
    ANIMCMD_FRAME(4, 1),
    ANIMCMD_END
};
static const union AnimCmd *const sAnimCmdTable_Sight[] =
{
    sAnimCmdSight0,
    sAnimCmdSight1,
    sAnimCmdSight2,
};

// gui sprite templates
static const struct SpriteTemplate sNoDataIconTemplate =
{
    .tileTag = ICON_GFX_TAG,
    .paletteTag = ICON_PAL_TAG,
    .oam = &sNoDataIconOam,
};

static const struct SpriteTemplate sCaptureAllMonsSpriteTemplate =
{
    .tileTag = CAPTURED_ALL_TAG,
    .paletteTag = 0xFFFF,
    .oam = &sCapturedAllOam,
};

static const struct SpriteTemplate sSelectionCursorSpriteTemplate =
{
    .tileTag = SELECTION_CURSOR_TAG,
    .paletteTag = 0xFFFF,
    .oam = &sSelectionCursorOam,
    .anims =  gDummySpriteAnimTable,
};

// search window sprite templates
static const struct SpriteTemplate sHeldItemTemplate =
{
    .tileTag = HELD_ITEM_TAG,
    .paletteTag = 0xFFFF,
    .oam = &sHeldItemOam,
};

static const struct SpriteTemplate sPotentialStarTemplate =
{
    .tileTag = LIT_STAR_TILE_TAG,
    .paletteTag = 0xFFFF,   //held item pal
    .oam = &sHeldItemOam,
};

static const struct SpriteTemplate sSearchIconSpriteTemplate =
{
    .tileTag = HIDDEN_SEARCH_TAG,
    .paletteTag = 0xFFFF,   //held item pal
    .oam = &sSearchIconOam,
    .anims =  gDummySpriteAnimTable,
};

static const struct SpriteTemplate sOwnedIconTemplate =
{
    .tileTag = OWNED_ICON_TAG,
    .paletteTag = 0xFFFF,   //held item pal
    .oam = &sHeldItemOam,
    .anims =  gDummySpriteAnimTable,
};

static const struct SpriteTemplate sHiddenMonIconTemplate =
{
    .tileTag = HIDDEN_MON_ICON_TAG,
    .paletteTag = 0xFFFF,   //held item pal
    .oam = &sHeldItemOam,
    .anims =  gDummySpriteAnimTable,
};

// gui sprite sheets
static const struct CompressedSpriteSheet sNoDataIconSpriteSheet = {sNoDataGfx, (32 * 32) / 2, ICON_GFX_TAG};
static const struct CompressedSpriteSheet sCapturedAllPokemonSpriteSheet = {sCapturedAllMonsTiles, (8 * 8) / 2, CAPTURED_ALL_TAG};
// search sprite sheets
static const struct CompressedSpriteSheet sPotentialStarSpriteSheet = {sPotentialStarGfx, (8 * 8) / 2, LIT_STAR_TILE_TAG};
static const struct CompressedSpriteSheet sOwnedIconSpriteSheet = {sOwnedIconGfx, (8 * 8) / 2, OWNED_ICON_TAG};
static const struct CompressedSpriteSheet sHiddenMonIconSpriteSheet = {sHiddenMonIconGfx, (8 * 8) / 2, HIDDEN_MON_ICON_TAG};

//// functions
///////////////////////
//// DEXNAV SEARCH ////
///////////////////////
static s16 GetSearchWindowY(void)
{
    return (GetWindowAttribute(sDexNavSearchDataPtr->windowId, WINDOW_TILEMAP_TOP) * 8);
}

#define SPECIES_ICON_X 28
static void DrawDexNavSearchMonIcon(enum Species species, u8 *dst, bool8 owned)
{
    u8 spriteId;

    LoadMonIconPalette(species);
    spriteId = CreateMonIcon(species, SpriteCB_MonIcon, SPECIES_ICON_X - 6, GetSearchWindowY() + 8, 0, 0xFFFFFFFF);
    gSprites[spriteId].oam.priority = 0;
    *dst = spriteId;

    if (owned)
        sDexNavSearchDataPtr->ownedIconSpriteId = CreateSprite(&sOwnedIconTemplate, SPECIES_ICON_X + 6, GetSearchWindowY() + 4, 0);
}

static void AddSearchWindow(u8 width)
{
    struct WindowTemplate template;
    u16 y = 16;

    if (sDexNavSearchDataPtr->tileY > (gSaveBlock1Ptr->pos.y + 7))
        y = 1;  //draw at top if chosen tile is below

    LoadDexNavWindowGfx(sDexNavSearchDataPtr->windowId, 0x1d5, 14 * 16);

    SetWindowTemplateFields(&template, 0, 1, y, width, 3, 14, 8);

    sDexNavSearchDataPtr->windowId = AddWindow(&template);
    FillWindowPixelBuffer(sDexNavSearchDataPtr->windowId, PIXEL_FILL(1));
    PutWindowTilemap(sDexNavSearchDataPtr->windowId);
    CopyWindowToVram(sDexNavSearchDataPtr->windowId, 3);

    DrawStdFrameWithCustomTileAndPalette(sDexNavSearchDataPtr->windowId, TRUE, 0x214, 14);
}

#define WINDOW_COL_0        (SPECIES_ICON_X + 4)
#define WINDOW_COL_1        (WINDOW_COL_0 + (GetFontAttribute(FONT_SMALL, FONTATTR_MAX_LETTER_WIDTH) * (POKEMON_NAME_LENGTH)))
#define WINDOW_MOVE_NAME_X  (WINDOW_COL_1 + (GetFontAttribute(FONT_SMALL, FONTATTR_MAX_LETTER_WIDTH) * 6))
#define SEARCH_ARROW_X      (WINDOW_MOVE_NAME_X + 90)
#define SEARCH_ARROW_Y      0

static void AddSearchWindowText(enum Species species, u8 proximity, u8 searchLevel, bool8 hidden)
{
    u8 windowId = sDexNavSearchDataPtr->windowId;

    //species name - always present
    if (hidden)
    {
        StringCopy(gStringVar4, sText_ThreeQmarks);
        AddTextPrinterParameterized3(sDexNavSearchDataPtr->windowId, FONT_SMALL, WINDOW_COL_0, 0, sSearchFontColor, TEXT_SKIP_DRAW, gStringVar4);
        return;
    }
    else
    {
        StringCopy(gStringVar1, GetSpeciesName(species));
        AddTextPrinterParameterized3(sDexNavSearchDataPtr->windowId, FONT_SMALL, WINDOW_COL_0, 0, sSearchFontColor, TEXT_SKIP_DRAW, gStringVar1);
    }

    //level - always present
    ConvertIntToDecimalStringN(gStringVar1, sDexNavSearchDataPtr->monLevel, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringExpandPlaceholders(gStringVar4, sText_MonLevel);
    AddTextPrinterParameterized3(sDexNavSearchDataPtr->windowId, FONT_SMALL, WINDOW_COL_1, 0, sSearchFontColor, TEXT_SKIP_DRAW, gStringVar4);

    if (proximity <= SNEAKING_PROXIMITY)
    {
        PlaySE(SE_POKENAV_ON);
        // move
        if (searchLevel > 1 && sDexNavSearchDataPtr->moves[0])
        {
            StringCopy(gStringVar1, GetMoveName(sDexNavSearchDataPtr->moves[0]));
            StringExpandPlaceholders(gStringVar4, sText_EggMove);
            AddTextPrinterParameterized3(windowId, FONT_SMALL, WINDOW_MOVE_NAME_X, 0, sSearchFontColor, TEXT_SKIP_DRAW, gStringVar4);
        }

        if (searchLevel > 2)
        {
            // ability name
            StringCopy(gStringVar1, gAbilitiesInfo[GetAbilityBySpecies(species, sDexNavSearchDataPtr->abilityNum)].name);
            AddTextPrinterParameterized3(windowId, FONT_SMALL, WINDOW_COL_1 + 16, 12, sSearchFontColor, TEXT_SKIP_DRAW, gStringVar1);

            // item name
            if (sDexNavSearchDataPtr->heldItem)
            {
                CopyItemName(sDexNavSearchDataPtr->heldItem, gStringVar1);
                StringExpandPlaceholders(gStringVar4, sText_HeldItem);
                AddTextPrinterParameterized3(windowId, FONT_SMALL, WINDOW_COL_0, 12, sSearchFontColor, TEXT_SKIP_DRAW, gStringVar4);
            }
        }
    }

    //chain level - always present
    ConvertIntToDecimalStringN(gStringVar1, gSaveBlock3Ptr->dexNavChain, STR_CONV_MODE_LEFT_ALIGN, 3);
    if (gSaveBlock3Ptr->dexNavChain > 99)
        StringExpandPlaceholders(gStringVar4, sText_DexNavChainLong);
    else
        StringExpandPlaceholders(gStringVar4, sText_DexNavChain);
    AddTextPrinterParameterized3(windowId, FONT_SMALL, SEARCH_ARROW_X - 16, 12, sSearchFontColor, TEXT_SKIP_DRAW, gStringVar4);

    CopyWindowToVram(sDexNavSearchDataPtr->windowId, 2);
}

#define SEARCH_WINDOW_WIDTH     28

static void DrawSearchWindow(enum Species species, u8 potential, bool8 hidden)
{
    u8 searchLevel = sDexNavSearchDataPtr->searchLevel;

    AddSearchWindow(SEARCH_WINDOW_WIDTH);
    AddSearchWindowText(species, sDexNavSearchDataPtr->proximity, searchLevel, hidden);
}

#undef SEARCH_WINDOW_WIDTH

static void RemoveDexNavWindowAndGfx(void)
{
    // try remove sprites
    if (sDexNavSearchDataPtr->iconSpriteId != MAX_SPRITES)
        DestroySprite(&gSprites[sDexNavSearchDataPtr->iconSpriteId]);
    if (sDexNavSearchDataPtr->itemSpriteId != MAX_SPRITES)
        DestroySprite(&gSprites[sDexNavSearchDataPtr->itemSpriteId]);
    if (sDexNavSearchDataPtr->eyeSpriteId != MAX_SPRITES)
        DestroySprite(&gSprites[sDexNavSearchDataPtr->eyeSpriteId]);
    if (sDexNavSearchDataPtr->ownedIconSpriteId != MAX_SPRITES)
        DestroySprite(&gSprites[sDexNavSearchDataPtr->ownedIconSpriteId]);
    if (sDexNavSearchDataPtr->exclamationSpriteId != MAX_SPRITES)
        DestroySprite(&gSprites[sDexNavSearchDataPtr->exclamationSpriteId]);

    for (u32 i = 0; i < NELEMS(sDexNavSearchDataPtr->starSpriteIds); i++)
    {
        if (sDexNavSearchDataPtr->starSpriteIds[i] != MAX_SPRITES)
            DestroySprite(&gSprites[sDexNavSearchDataPtr->starSpriteIds[i]]);
    }

    FreeSpriteTilesByTag(HELD_ITEM_TAG);
    FreeSpriteTilesByTag(OWNED_ICON_TAG);
    FreeSpriteTilesByTag(HIDDEN_SEARCH_TAG);
    FreeSpriteTilesByTag(HIDDEN_MON_ICON_TAG);
    FreeSpriteTilesByTag(LIT_STAR_TILE_TAG);
    FreeSpritePaletteByTag(HELD_ITEM_TAG);
    SafeFreeMonIconPalette(sDexNavSearchDataPtr->species);

    // remove window
    ClearStdWindowAndFrameToTransparent(sDexNavSearchDataPtr->windowId, FALSE);
    CopyWindowToVram(sDexNavSearchDataPtr->windowId, 3);
    RemoveWindow(sDexNavSearchDataPtr->windowId);
}


//////////////////////
////DEXNAV SEARCH/////
//////////////////////
static u8 GetPlayerDistance(s16 x, s16 y)
{
    u16 deltaX = abs(x - (gSaveBlock1Ptr->pos.x + 7));
    u16 deltaY = abs(y - (gSaveBlock1Ptr->pos.y + 7));
    return deltaX + deltaY;
}

static void DexNavProximityUpdate(void)
{
    sDexNavSearchDataPtr->proximity = GetPlayerDistance(sDexNavSearchDataPtr->tileX, sDexNavSearchDataPtr->tileY);
}

//Pick a specific tile based on environment
// Pick uniformly among all reachable encounter tiles rather than rolling a tiny
// independent success chance on each tile. A valid nearby habitat should not
// return "couldn't be found" simply because the tile RNG failed.
static bool8 DexNavPickTile(enum EncounterType environment, u8 areaX, u8 areaY, bool8 smallScan)
{
    // Map grid positions are offset by seven tiles from the player's coordinates.
    s16 startX = gSaveBlock1Ptr->pos.x - SCANSTART_X + (smallScan * 5);
    s16 startY = gSaveBlock1Ptr->pos.y - SCANSTART_Y + (smallScan * 5);
    s16 endX = startX + areaX;
    s16 endY = startY + areaY;
    enum MapType currMapType = GetCurrentMapType();
    u8 tileBuffer = 2;
    u32 candidates = 0;

    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_BIKE))
        tileBuffer = SNEAKING_PROXIMITY + 3;
    else if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_DASH))
        tileBuffer = SNEAKING_PROXIMITY + 1;

    for (s16 y = startY; y < endY; y++)
    {
        for (s16 x = startX; x < endX; x++)
        {
            bool8 occupied = FALSE;
            bool8 validTile = FALSE;
            u8 tileBehaviour;

            // Keep the existing terrain, obstruction, and player-distance
            // restrictions so no search spawns an unreachable Pokémon.
            if (GetPlayerDistance(x, y) <= tileBuffer || MapGridGetCollisionAt(x, y))
                continue;

            for (u8 i = 0; i < OBJECT_EVENTS_COUNT; i++)
            {
                if (gObjectEvents[i].currentCoords.x == x && gObjectEvents[i].currentCoords.y == y)
                {
                    occupied = TRUE;
                    break;
                }
            }
            if (occupied)
                continue;

            tileBehaviour = MapGridGetMetatileBehaviorAt(x, y);
            if (environment == ENCOUNTER_TYPE_LAND)
            {
                validTile = MetatileBehavior_IsLandWildEncounter(tileBehaviour);
                if (validTile && currMapType == MAP_TYPE_UNDERGROUND)
                    validTile = !IsElevationMismatchAt(gObjectEvents[gPlayerAvatar.objectEventId].currentElevation, x, y);
            }
            else if (environment == ENCOUNTER_TYPE_WATER)
            {
                validTile = MetatileBehavior_IsSurfableWaterOrUnderwater(tileBehaviour)
                    && !IsElevationMismatchAt(gObjectEvents[gPlayerAvatar.objectEventId].currentElevation, x, y);
            }

            // Reservoir sampling avoids per-tile encounter failures, avoids
            // allocating tile arrays, and handles large coordinates safely.
            if (validTile && (Random() % ++candidates) == 0)
            {
                sDexNavSearchDataPtr->tileX = x;
                sDexNavSearchDataPtr->tileY = y;
            }
        }
    }

    return candidates != 0;
}

static bool8 TryStartHiddenMonFieldEffect(enum EncounterType environment, u8 xSize, u8 ySize, bool8 smallScan)
{
    enum MapType currMapType = GetCurrentMapType();
    u8 fldEffId = 0;

    if (DexNavPickTile(environment, xSize, ySize, smallScan))
    {
        u8 metatileBehaviour = MapGridGetMetatileBehaviorAt(sDexNavSearchDataPtr->tileX, sDexNavSearchDataPtr->tileY);

        switch (environment)
        {
        case ENCOUNTER_TYPE_LAND:
            if (currMapType == MAP_TYPE_UNDERGROUND)
            {
                fldEffId = FLDEFF_CAVE_DUST;
            }
            else if (IsMapTypeIndoors(currMapType))
            {
                if (MetatileBehavior_IsTallGrass(metatileBehaviour)) //Grass in cave
                    fldEffId = FLDEFF_SHAKING_GRASS;
                else if (MetatileBehavior_IsLongGrass(metatileBehaviour)) //Really tall grass
                    fldEffId = FLDEFF_SHAKING_LONG_GRASS;
                else if (MetatileBehavior_IsSandOrDeepSand(metatileBehaviour))
                    fldEffId = FLDEFF_SAND_HOLE;
                else
                    fldEffId = FLDEFF_CAVE_DUST;
            }
            else //outdoor, underwater
            {
                if (MetatileBehavior_IsTallGrass(metatileBehaviour)) //Regular grass
                    fldEffId = FLDEFF_SHAKING_GRASS;
                else if (MetatileBehavior_IsLongGrass(metatileBehaviour)) //Really tall grass
                    fldEffId = FLDEFF_SHAKING_LONG_GRASS;
                else if (MetatileBehavior_IsSandOrDeepSand(metatileBehaviour)) //Desert Sand
                    fldEffId = FLDEFF_SAND_HOLE;
                else if (MetatileBehavior_IsMountain(metatileBehaviour)) //Rough Terrain
                    fldEffId = FLDEFF_CAVE_DUST;
                else
                    fldEffId = FLDEFF_BERRY_TREE_GROWTH_SPARKLE; //default
            }
            break;
        case ENCOUNTER_TYPE_WATER:
            fldEffId = FLDEFF_WATER_SURFACING;
            break;
        default:
            return FALSE;
        }

        if (fldEffId != 0)
        {
            gFieldEffectArguments[0] = sDexNavSearchDataPtr->tileX;
            gFieldEffectArguments[1] = sDexNavSearchDataPtr->tileY;
            gFieldEffectArguments[2] = 0xFF; // subpriority
            gFieldEffectArguments[3] = 2;   //priority
            sDexNavSearchDataPtr->fldEffSpriteId = FieldEffectStart(fldEffId);
            if (sDexNavSearchDataPtr->fldEffSpriteId == MAX_SPRITES)
                return FALSE;

            sDexNavSearchDataPtr->fldEffId = fldEffId;
            return TRUE;
        }
    }

    return FALSE;
}

static void DrawDexNavSearchHeldItem(u8 *dst)
{
    *dst = CreateSpriteUnchecked(&sHeldItemTemplate, SPECIES_ICON_X + 6, GetSearchWindowY() + 18, 0);
    if (*dst != MAX_SPRITES)
        gSprites[*dst].invisible = TRUE;
}

static void LoadSearchIconData(void)
{
    // palettes clash with mon icon, so must load manually
    LoadSpriteSheet(&gSpriteSheet_HeldItem);
    LoadPalette(gHeldItemPalette, OBJ_PLTT_ID(sHeldItemOam.paletteNum), PLTT_SIZE_4BPP);
    LoadCompressedSpriteSheetUsingHeap(&sPotentialStarSpriteSheet);
    //LoadCompressedSpriteSheetUsingHeap(&sSightSpriteSheet);   //eye replaced with arrow
    LoadCompressedSpriteSheetUsingHeap(&sOwnedIconSpriteSheet);
    LoadCompressedSpriteSheetUsingHeap(&sHiddenMonIconSpriteSheet);
}

static u8 GetSearchLevel(enum Species species)
{
    u8 searchLevel;
#if USE_DEXNAV_SEARCH_LEVELS == TRUE
    searchLevel = gSaveBlock3Ptr->dexNavSearchLevels[species];
#else
    searchLevel = 0;
#endif
    return searchLevel;
}

static void SetUpDexNavSearch(void)
{
    enum Species species = sDexNavSearchDataPtr->species;
    u8 searchLevel = GetSearchLevel(species);

    // init sprites
    sDexNavSearchDataPtr->iconSpriteId = MAX_SPRITES;
    sDexNavSearchDataPtr->itemSpriteId = MAX_SPRITES;
    sDexNavSearchDataPtr->eyeSpriteId = MAX_SPRITES;
    sDexNavSearchDataPtr->starSpriteIds[0] = MAX_SPRITES;
    sDexNavSearchDataPtr->starSpriteIds[1] = MAX_SPRITES;
    sDexNavSearchDataPtr->starSpriteIds[2] = MAX_SPRITES;
    sDexNavSearchDataPtr->ownedIconSpriteId = MAX_SPRITES;
    sDexNavSearchDataPtr->exclamationSpriteId = MAX_SPRITES;
    sDexNavSearchDataPtr->searchLevel = searchLevel;

    DexNavGenerateMoveset(species, searchLevel, sDexNavSearchDataPtr->monLevel, &sDexNavSearchDataPtr->moves[0]);
    sDexNavSearchDataPtr->heldItem = DexNavGenerateHeldItem(species, searchLevel);
    sDexNavSearchDataPtr->abilityNum = DexNavGetAbilityNum(species, searchLevel);
    sDexNavSearchDataPtr->potential = DexNavGeneratePotential(searchLevel);
    DexNavProximityUpdate();

    LoadSearchIconData();
    if (sDexNavSearchDataPtr->hiddenSearch)
    {
        DexNavDrawHiddenIcons();
    }
    else
    {
        DexNavDrawIcons();
        DexNavUpdateSearchWindow(sDexNavSearchDataPtr->proximity, searchLevel);
    }

    gPlayerAvatar.creeping = TRUE;  //initialize as true in case mon appears beside you
    sDexNavSearchDataPtr->proximity = gSprites[gPlayerAvatar.spriteId].x;
    sDexNavSearchDataPtr->startingTime = gMain.vblankCounter1;
    IncrementGameStat(GAME_STAT_DEXNAV_SCANNED);
}

static void DexNavSearchBail(const u8 *script)
{
    TRY_FREE_AND_SET_NULL(sDexNavSearchDataPtr);
    FlagClear(DN_FLAG_SEARCHING);
    FreeMonIconPalettes();
    ScriptContext_SetupScript(script);
}

static bool8 InitDexNavSearch(enum Species species, u32 environment)
{
    sDexNavSearchDataPtr = AllocZeroed(sizeof(struct DexNavSearch));
    if (sDexNavSearchDataPtr == NULL)
    {
        DexNavSearchBail(EventScript_NotFoundNearby);
        return TRUE;
    }
    FlagSet(DN_FLAG_SEARCHING);

    // assign non-objects to struct
    sDexNavSearchDataPtr->species = species;
    sDexNavSearchDataPtr->environment = environment;  //updated in DexNavTryGenerateMonLevel if hidden mon
    sDexNavSearchDataPtr->isHiddenMon = (environment == ENCOUNTER_TYPE_HIDDEN) ? TRUE : FALSE;
    sDexNavSearchDataPtr->monLevel = DexNavTryGenerateMonLevel(species, environment);

    if (GetFlashLevel() > 0)
    {
        DexNavSearchBail(EventScript_TooDark);
        return TRUE;
    }

    if (sDexNavSearchDataPtr->monLevel == MON_LEVEL_NONEXISTENT || !TryStartHiddenMonFieldEffect(sDexNavSearchDataPtr->environment, SCANSIZE_X, SCANSIZE_Y, FALSE))
    {
        DexNavSearchBail(EventScript_NotFoundNearby);
        return TRUE;
    }

    sDexNavSearchDataPtr->hiddenSearch = FALSE;
    SetUpDexNavSearch();
    return FALSE;
}

static void DexNavDrawPotentialStars(u8 potential, u8 *dst)
{
    u8 spriteId;
    u32 i;

    for (i = 0; i < NELEMS(sDexNavSearchDataPtr->starSpriteIds); i++)
    {
        spriteId = MAX_SPRITES;
        if (potential > i)
            spriteId = CreateSpriteUnchecked(&sPotentialStarTemplate, SPECIES_ICON_X - 20, GetSearchWindowY() + 4 + (i * 8), 0);

        dst[i] = spriteId;
        if (spriteId != MAX_SPRITES)
            gSprites[spriteId].invisible = TRUE;
    }
}

static void DexNavUpdateDirectionArrow(void)
{
    u16 tileX = sDexNavSearchDataPtr->tileX;
    u16 tileY = sDexNavSearchDataPtr->tileY;
    u16 playerX = gSaveBlock1Ptr->pos.x + MAP_OFFSET;
    u16 playerY = gSaveBlock1Ptr->pos.y + MAP_OFFSET;
    u16 deltaX = abs(tileX - playerX);
    u16 deltaY = abs(tileY - playerY);
    const u8 *str;
    u8 windowId = sDexNavSearchDataPtr->windowId;

    FillWindowPixelRect(windowId, PIXEL_FILL(1), SEARCH_ARROW_X, SEARCH_ARROW_Y, 12, 12);
    if (deltaX <= 1 && deltaY <= 1)
    {
        str = gText_EmptyString2;
    }
    else if (deltaX > deltaY)
    {
        if (playerX > tileX)
            str = sText_ArrowLeft;  //player to right
        else
            str = sText_ArrowRight; //player to left
    }
    else //greater Y diff
    {
        if (playerY > tileY)
            str = sText_ArrowUp;    //player below
        else
            str = sText_ArrowDown;  //player above
    }

    AddTextPrinterParameterized3(windowId, FONT_NORMAL, SEARCH_ARROW_X, SEARCH_ARROW_Y, sSearchFontColor, TEXT_SKIP_DRAW, str);
    CopyWindowToVram(windowId, 2);
}

static void DexNavDrawIcons(void)
{
    enum Species species = sDexNavSearchDataPtr->species;

    DrawSearchWindow(species, sDexNavSearchDataPtr->potential, FALSE);
    DrawDexNavSearchMonIcon(species, &sDexNavSearchDataPtr->iconSpriteId, GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT));
    DrawDexNavSearchHeldItem(&sDexNavSearchDataPtr->itemSpriteId);
    DexNavDrawPotentialStars(sDexNavSearchDataPtr->potential, &sDexNavSearchDataPtr->starSpriteIds[0]);
    DexNavUpdateDirectionArrow();
}

/////////////////////
//// SEARCH TASK ////
/////////////////////
static void RevealHiddenSearch(void)
{
    PlaySE(SE_DEX_SEARCH);
    ClearStdWindowAndFrameToTransparent(sDexNavSearchDataPtr->windowId, FALSE);
    CopyWindowToVram(sDexNavSearchDataPtr->windowId, 3);
    RemoveWindow(sDexNavSearchDataPtr->windowId);
    DestroySprite(&gSprites[sDexNavSearchDataPtr->iconSpriteId]);
    sDexNavSearchDataPtr->hiddenSearch = FALSE; //now its a regular dexnav search
    RevealHiddenMon();
}

bool32 TryStartDexNavSearch(void)
{
    u16 val = VarGet(DN_VAR_SPECIES);

    // A search cannot survive a reboot; the registered target can.
    if (sDexNavSearchDataPtr == NULL)
        FlagClear(DN_FLAG_SEARCHING);
    if (FlagGet(DN_FLAG_SEARCHING) && sDexNavSearchDataPtr->hiddenSearch)
    {
        RevealHiddenSearch();
        return FALSE;
    }

    if (FlagGet(DN_FLAG_SEARCHING) || (val & DEXNAV_MASK_SPECIES) == SPECIES_NONE)
        return FALSE;

    HideMapNamePopUpWindow();
    ChangeBgY_ScreenOff(0, 0, 0);
    PlaySE(SE_DEX_SEARCH);
    return InitDexNavSearch(val & DEXNAV_MASK_SPECIES, val >> 14);
}

void EndDexNavSearch(void)
{
    if (!FlagGet(DN_FLAG_SEARCHING) || sDexNavSearchDataPtr == NULL)
        return;
    RemoveDexNavWindowAndGfx();
    FieldEffectStop(&gSprites[sDexNavSearchDataPtr->fldEffSpriteId], sDexNavSearchDataPtr->fldEffId);
    FREE_AND_SET_NULL(sDexNavSearchDataPtr);
    FlagClear(DN_FLAG_SEARCHING);
}

static void EndDexNavSearchSetupScript(const u8 *script)
{
    gSaveBlock3Ptr->dexNavChain = 0;   //reset chain
    EndDexNavSearch();
    ScriptContext_SetupScript(script);
}

static u8 GetMovementProximityBySearchLevel(void)
{
    if (sDexNavSearchDataPtr->searchLevel < 20)
        return 2;
    else if (sDexNavSearchDataPtr->searchLevel < 50)
        return 3;
    else
        return 4;
}

static void RevealHiddenMon(void)
{
    enum Species species = sDexNavSearchDataPtr->species;

    // remove owned icon if it exists
    if (sDexNavSearchDataPtr->ownedIconSpriteId != MAX_SPRITES)
    {
        DestroySprite(&gSprites[sDexNavSearchDataPtr->ownedIconSpriteId]);
        sDexNavSearchDataPtr->ownedIconSpriteId = MAX_SPRITES;
    }

    // remove exclamation if it exists
    if (sDexNavSearchDataPtr->exclamationSpriteId != MAX_SPRITES)
    {
        DestroySprite(&gSprites[sDexNavSearchDataPtr->exclamationSpriteId]);
        sDexNavSearchDataPtr->exclamationSpriteId = MAX_SPRITES;
    }


    if (!GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_SEEN))
    {
        u8 index;

        //if not seen, hide name and whiteout mon
        DrawSearchWindow(species, sDexNavSearchDataPtr->potential, TRUE);
        DrawDexNavSearchMonIcon(species, &sDexNavSearchDataPtr->iconSpriteId, FALSE);
        // whiteout icon
        index = IndexOfSpritePaletteTag(gSprites[sDexNavSearchDataPtr->iconSpriteId].template->paletteTag);
        CpuCopy16(&gPlttBufferUnfaded[OBJ_PLTT_ID(index)], sDexNavSearchDataPtr->palBuffer, 32);
        TintPalette_CustomTone(sDexNavSearchDataPtr->palBuffer, 16, 510, 510, 510);
        LoadPalette(sDexNavSearchDataPtr->palBuffer, OBJ_PLTT_ID(index), PLTT_SIZE_4BPP);
    }
    else
    {
        DrawSearchWindow(species, sDexNavSearchDataPtr->potential, FALSE);
        DrawDexNavSearchMonIcon(species, &sDexNavSearchDataPtr->iconSpriteId, GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT));
    }

    sDexNavSearchDataPtr->startingTime = gMain.vblankCounter1;
    DexNavUpdateDirectionArrow();
}

bool32 OnStep_DexNavSearch(void)
{
    if (!FlagGet(DN_FLAG_SEARCHING) || sDexNavSearchDataPtr == NULL)
        return FALSE;

    u32 frameCount = gMain.vblankCounter1 - sDexNavSearchDataPtr->startingTime;
    DexNavProximityUpdate();
    if (!sDexNavSearchDataPtr->hiddenSearch)    //update search window info only if revealed mon
        DexNavUpdateSearchWindow(sDexNavSearchDataPtr->proximity, sDexNavSearchDataPtr->searchLevel);

    if (sDexNavSearchDataPtr->proximity > MAX_PROXIMITY)
    { // out of range
        if (sDexNavSearchDataPtr->hiddenSearch)
        {
            EndDexNavSearch();
            return FALSE;
        }
        else
        {
            EndDexNavSearchSetupScript(EventScript_LostSignal);
            return TRUE;
        }
    }

    if (sDexNavSearchDataPtr->proximity <= CREEPING_PROXIMITY && !gPlayerAvatar.creeping && frameCount > 60)
    { //should be creeping but player walks normally
        if (sDexNavSearchDataPtr->hiddenSearch)
        {
            EndDexNavSearch();
            return FALSE;
        }
        else
        {
            EndDexNavSearchSetupScript(EventScript_MovedTooFast);
            return TRUE;
        }
    }

    if (sDexNavSearchDataPtr->proximity <= SNEAKING_PROXIMITY && TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_DASH | PLAYER_AVATAR_FLAG_BIKE))
    { // running/biking too close
        //always do event script, even if player hasn't revealed a hidden mon. It's assumed they would be creeping towards it
        EndDexNavSearchSetupScript(EventScript_MovedTooFast);
        return TRUE;
    }

    if (frameCount > DEXNAV_TIMEOUT * 60)
    { // player took too long
        if (sDexNavSearchDataPtr->hiddenSearch)
        {
            EndDexNavSearch();
            return FALSE;
        }
        else
        {
            EndDexNavSearchSetupScript(EventScript_PokemonGotAway);
            return TRUE;
        }
        return FALSE;
    }

    if (sDexNavSearchDataPtr->proximity < 1)
    {
        gDexNavSpecies = sDexNavSearchDataPtr->species;
        CreateDexNavWildMon(sDexNavSearchDataPtr->species, sDexNavSearchDataPtr->potential, sDexNavSearchDataPtr->monLevel,
                            sDexNavSearchDataPtr->abilityNum, sDexNavSearchDataPtr->heldItem, sDexNavSearchDataPtr->moves);

        ScriptContext_SetupScript(EventScript_StartDexNavBattle);
        FREE_AND_SET_NULL(sDexNavSearchDataPtr);
        FlagClear(DN_FLAG_SEARCHING);
        return TRUE;
    }

    if (sDexNavSearchDataPtr->hiddenSearch && sDexNavSearchDataPtr->proximity < CREEPING_PROXIMITY)
    {
        RevealHiddenSearch();
        return FALSE;
    }

    //Caves and water the Pokémon moves around
    if ((sDexNavSearchDataPtr->environment == ENCOUNTER_TYPE_WATER || GetCurrentMapType() == MAP_TYPE_UNDERGROUND)
        && sDexNavSearchDataPtr->proximity < GetMovementProximityBySearchLevel() && sDexNavSearchDataPtr->movementCount < 2
        && !sDexNavSearchDataPtr->hiddenSearch)
    {
        FieldEffectStop(&gSprites[sDexNavSearchDataPtr->fldEffSpriteId], sDexNavSearchDataPtr->fldEffId);

        if (!TryStartHiddenMonFieldEffect(sDexNavSearchDataPtr->environment, 10, 10, TRUE))
        {
            EndDexNavSearchSetupScript(EventScript_PokemonGotAway);
            return TRUE;
        }

        sDexNavSearchDataPtr->movementCount++;
    }
    return FALSE;
}

static void DexNavUpdateSearchWindow(u8 proximity, u8 searchLevel)
{
    bool8 hideName = FALSE;

    if (sDexNavSearchDataPtr->hiddenSearch && !GetSetPokedexFlag(SpeciesToNationalPokedexNum(sDexNavSearchDataPtr->species), FLAG_GET_SEEN))
        hideName = TRUE;    //if a detector mode hidden search and player hasn't seen the mon, hide info

    FillWindowPixelBuffer(sDexNavSearchDataPtr->windowId, PIXEL_FILL(1));   //clear window
    AddSearchWindowText(sDexNavSearchDataPtr->species, proximity, searchLevel, hideName);

    DexNavUpdateDirectionArrow();

    //init hidden sprites
    if (sDexNavSearchDataPtr->itemSpriteId != MAX_SPRITES)
        gSprites[sDexNavSearchDataPtr->itemSpriteId].invisible = TRUE;
    if (sDexNavSearchDataPtr->starSpriteIds[0] != MAX_SPRITES)
        gSprites[sDexNavSearchDataPtr->starSpriteIds[0]].invisible = TRUE;
    if (sDexNavSearchDataPtr->starSpriteIds[1] != MAX_SPRITES)
        gSprites[sDexNavSearchDataPtr->starSpriteIds[1]].invisible = TRUE;
    if (sDexNavSearchDataPtr->starSpriteIds[2] != MAX_SPRITES)
        gSprites[sDexNavSearchDataPtr->starSpriteIds[2]].invisible = TRUE;

    if (proximity <= SNEAKING_PROXIMITY)
    {
        if (searchLevel > 2 && sDexNavSearchDataPtr->heldItem)
        {
            // toggle item view
            if (sDexNavSearchDataPtr->itemSpriteId != MAX_SPRITES)
                gSprites[sDexNavSearchDataPtr->itemSpriteId].invisible = FALSE;
        }

        if (searchLevel > 4)
        {
            if (sDexNavSearchDataPtr->starSpriteIds[0] != MAX_SPRITES)
                gSprites[sDexNavSearchDataPtr->starSpriteIds[0]].invisible = FALSE;

            if (sDexNavSearchDataPtr->starSpriteIds[1] != MAX_SPRITES)
                gSprites[sDexNavSearchDataPtr->starSpriteIds[1]].invisible = FALSE;

            if (sDexNavSearchDataPtr->starSpriteIds[2] != MAX_SPRITES)
                gSprites[sDexNavSearchDataPtr->starSpriteIds[2]].invisible = FALSE;
        }
    }
}

//////////////////////////////
//// DEXNAV MON GENERATOR ////
//////////////////////////////
static void CreateDexNavWildMon(enum Species species, u8 potential, u8 level, u8 abilityNum, enum Item item, enum Move *moves)
{
    struct Pokemon *mon = &gParties[B_TRAINER_OPPONENT_A][0];

    CreateWildMon(species, level);  // shiny rate bonus handled in CreateBoxMon
    SetBoxMonPerfectIVs(&mon->box, min(3, potential)); // Will not exceed 3 Perfect IVs

    //Set ability
    SetMonData(mon, MON_DATA_ABILITY_NUM, &abilityNum);

    // Set Held Item
    if (item)
        SetMonData(mon, MON_DATA_HELD_ITEM, &item);

    //Set moves
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
        SetMonMoveSlot(mon, moves[i], i);

    CalculateMonStats(mon);
}

// gets a random level of the species based on map data.
//if it was a hidden encounter, updates the environment it is to be found from the wildheader encounterRate
static u8 DexNavTryGenerateMonLevel(enum Species species, enum EncounterType environment)
{
    u8 levelBase = GetEncounterLevelFromMapData(species, environment);
    u8 levelBonus = gSaveBlock3Ptr->dexNavChain / 5;

    if (levelBase == MON_LEVEL_NONEXISTENT)
        return MON_LEVEL_NONEXISTENT;   //species not found in the area

    if (Random() % 100 < 4)
        levelBonus += 10; //4% chance of having a +10 level

    if (levelBase + levelBonus > MAX_LEVEL)
        return MAX_LEVEL;
    else
        return levelBase + levelBonus;
}

static enum Move GetRandomEggMove(enum Species species)
{
    u32 numEggMoves = 0;
    const u16 *eggMoveLearnset = GetSpeciesEggMoves(species);
    for (u32 i = 0; eggMoveLearnset[i] != MOVE_UNAVAILABLE; i++)
        numEggMoves++;

    enum Move result = *(const u16 *)(RandomElementArray(RNG_DEXNAV_RANDOM_EGG_MOVE, eggMoveLearnset, sizeof(u16), numEggMoves));
    return result;
}

static void DexNavGenerateMoveset(enum Species species, u8 searchLevel, u8 encounterLevel, u16 *moveDst)
{
    bool8 genMove = FALSE;
    u16 randVal = Random() % 100;
    u16 i;

    // see if first move slot should be an egg move
    if (searchLevel < 5)
    {
        if (SEARCHLEVEL0_MOVECHANCE != 0 && randVal < SEARCHLEVEL0_MOVECHANCE)
            genMove = TRUE;
    }
    else if (searchLevel < 10)
    {
        if (SEARCHLEVEL5_MOVECHANCE != 0 && randVal < SEARCHLEVEL5_MOVECHANCE)
            genMove = TRUE;
    }
    else if (searchLevel < 25)
    {
        if (SEARCHLEVEL10_MOVECHANCE != 0 && randVal < SEARCHLEVEL10_MOVECHANCE)
            genMove = TRUE;
    }
    else if (searchLevel < 50)
    {
        if (SEARCHLEVEL25_MOVECHANCE != 0 && randVal < SEARCHLEVEL25_MOVECHANCE)
            genMove = TRUE;
    }
    else if (searchLevel < 100)
    {
        if (SEARCHLEVEL50_MOVECHANCE != 0 && randVal < SEARCHLEVEL50_MOVECHANCE)
            genMove = TRUE;
    }
    else
    {
        if (SEARCHLEVEL100_MOVECHANCE != 0 && randVal < SEARCHLEVEL100_MOVECHANCE)
            genMove = TRUE;
    }

    // Generate a wild mon just to get the initial moveset (later overwritten by CreateDexNavWildMon)
    CreateWildMon(species, encounterLevel);

    // Store generated mon moves into Dex Nav Struct
    for (i = 0; i < MAX_MON_MOVES; i++)
        moveDst[i] = GetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_MOVE1 + i);

    // set first move slot to a random egg move if search level is good enough
    if (genMove)
        moveDst[0] = GetRandomEggMove(GetEggSpecies(species));
}

static enum Item DexNavGenerateHeldItem(enum Species species, u8 searchLevel)
{
    u16 randVal = Random() % 100;
    u8 searchLevelInfluence = searchLevel >> 1;
    enum Item item1 = gSpeciesInfo[species].itemCommon;
    enum Item item2 = gSpeciesInfo[species].itemRare;

    // if both are the same, 100% to hold
    if (item1 == item2)
        return item1;

    // if no items can be held, then yeah...no items
    if (item2 == ITEM_NONE && item1 == ITEM_NONE)
        return ITEM_NONE;

    // if only one entry, 50% chance
    if (item2 == ITEM_NONE && item1 != ITEM_NONE)
        return (randVal < 50) ? item1 : ITEM_NONE;

    // if both are distinct item1 = 50% + srclvl/2; item2 = 5% + srchlvl/2
    if (randVal < (50 + searchLevelInfluence + 5 + searchLevel))
        return (randVal > 5 + searchLevelInfluence) ? item1 : item2;
    else
        return ITEM_NONE;

    return ITEM_NONE;
}

static u8 DexNavGetAbilityNum(enum Species species, u8 searchLevel)
{
    bool8 genAbility = FALSE;
    u16 randVal = Random() % 100;
    u8 abilityNum = 0;

    if (searchLevel < 5)
    {
        #if (SEARCHLEVEL0_ABILITYCHANCE != 0)
        if (randVal < SEARCHLEVEL0_ABILITYCHANCE)
            genAbility = TRUE;
        #endif
    }
    else if (searchLevel < 10)
    {
        #if (SEARCHLEVEL5_ABILITYCHANCE != 0)
        if (randVal < SEARCHLEVEL5_ABILITYCHANCE)
            genAbility = TRUE;
        #endif
    }
    else if (searchLevel < 25)
    {
        #if (SEARCHLEVEL10_ABILITYCHANCE != 0)
        if (randVal < SEARCHLEVEL10_ABILITYCHANCE)
            genAbility = TRUE;
        #endif
    }
    else if (searchLevel < 50)
    {
        #if (SEARCHLEVEL25_ABILITYCHANCE != 0)
        if (randVal < SEARCHLEVEL25_ABILITYCHANCE)
            genAbility = TRUE;
        #endif
    }
    else if (searchLevel < 100)
    {
        #if (SEARCHLEVEL50_ABILITYCHANCE != 0)
        if (randVal < SEARCHLEVEL50_ABILITYCHANCE)
            genAbility = TRUE;
        #endif
    }
    else
    {
        #if (SEARCHLEVEL100_ABILITYCHANCE != 0)
        if (randVal < SEARCHLEVEL100_ABILITYCHANCE)
            genAbility = TRUE;
        #endif
    }

    if (genAbility
            && GetSpeciesAbility(species, 2) != ABILITY_NONE
            && GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
    {
        //Only give hidden ability if Pokemon has been caught before
        abilityNum = 2;
    }
    else
    {
        //Pick a normal ability of that Pokemon
        if (GetSpeciesAbility(species, 1) != ABILITY_NONE)
            abilityNum = Random() & 1;
        else
            abilityNum = 0;
    }

    return abilityNum;
}

static u8 DexNavGeneratePotential(u8 searchLevel)
{
    u8 genChance = 0;
    int randVal = Random() % 100;

    if (searchLevel < 5)
    {
        genChance = SEARCHLEVEL0_ONESTAR + SEARCHLEVEL0_TWOSTAR + SEARCHLEVEL0_THREESTAR;
        if (randVal < genChance)
        {
            // figure out which star it is
            if (randVal < SEARCHLEVEL0_ONESTAR)
                return 1;
            else if (randVal < (SEARCHLEVEL0_ONESTAR + SEARCHLEVEL0_TWOSTAR))
                return 2;
            else
                return 3;
        }
    }
    else if (searchLevel < 10)
    {
        genChance = SEARCHLEVEL5_ONESTAR + SEARCHLEVEL5_TWOSTAR + SEARCHLEVEL5_THREESTAR;
        if (randVal < genChance)
        {
            // figure out which star it is
            if (randVal < SEARCHLEVEL5_ONESTAR)
                return 1;
            else if (randVal < (SEARCHLEVEL5_ONESTAR + SEARCHLEVEL5_TWOSTAR))
                return 2;
            else
                return 3;
        }
    }
    else if (searchLevel < 25)
    {
        genChance = SEARCHLEVEL10_ONESTAR + SEARCHLEVEL10_TWOSTAR + SEARCHLEVEL10_THREESTAR;
        if (randVal < genChance)
        {
            // figure out which star it is
            if (randVal < SEARCHLEVEL10_ONESTAR)
                return 1;
            else if (randVal < (SEARCHLEVEL10_ONESTAR + SEARCHLEVEL10_TWOSTAR))
                return 2;
            else
                return 3;
        }
    }
    else if (searchLevel < 50)
    {
        genChance = SEARCHLEVEL25_ONESTAR + SEARCHLEVEL25_TWOSTAR + SEARCHLEVEL25_THREESTAR;
        if (randVal < genChance)
        {
            // figure out which star it is
            if (randVal < SEARCHLEVEL25_ONESTAR)
                return 1;
            else if (randVal < (SEARCHLEVEL25_ONESTAR + SEARCHLEVEL25_TWOSTAR))
                return 2;
            else
                return 3;
        }
    }
    else if (searchLevel < 100)
    {
        genChance = SEARCHLEVEL50_ONESTAR + SEARCHLEVEL50_TWOSTAR + SEARCHLEVEL50_THREESTAR;
        if (randVal < genChance)
        {
            // figure out which star it is
            if (randVal < SEARCHLEVEL50_ONESTAR)
                return 1;
            else if (randVal < (SEARCHLEVEL50_ONESTAR + SEARCHLEVEL50_TWOSTAR))
                return 2;
            else
                return 3;
        }
    }
    else
    {
        genChance = SEARCHLEVEL100_ONESTAR + SEARCHLEVEL100_TWOSTAR + SEARCHLEVEL100_THREESTAR;
        if (randVal < genChance)
        {
            // figure out which star it is
            if (randVal < SEARCHLEVEL100_ONESTAR)
                return 1;
            else if (randVal < (SEARCHLEVEL100_ONESTAR + SEARCHLEVEL100_TWOSTAR))
                return 2;
            else
                return 3;
        }
    }

    return 0;   // No potential
}

static u8 GetEncounterLevelFromMapData(enum Species species, enum EncounterType environment)
{
    u32 headerId = GetCurrentMapWildMonHeaderId();
    enum TimeOfDay timeOfDay;
    u8 min = MAX_LEVEL;
    u8 max = 0;
    u8 i;

    if (headerId == HEADER_NONE || species == SPECIES_NONE || species >= NUM_SPECIES)
        return MON_LEVEL_NONEXISTENT;

    switch (environment)
    {
    case ENCOUNTER_TYPE_LAND:    // grass
        timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_LAND);
        const struct WildPokemonInfo *landMonsInfo = gWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo;

        if (landMonsInfo == NULL)
            return MON_LEVEL_NONEXISTENT; //Hidden Pokémon should only appear on walkable tiles or surf tiles

        for (i = 0; i < NUM_LAND_MONS_ENCOUNTER_SLOTS; i++)
        {
            if (landMonsInfo->wildPokemon[i].species == species)
            {
                min = (min < landMonsInfo->wildPokemon[i].minLevel) ? min : landMonsInfo->wildPokemon[i].minLevel;
                max = (max > landMonsInfo->wildPokemon[i].maxLevel) ? max : landMonsInfo->wildPokemon[i].maxLevel;
            }
        }
        break;
    case ENCOUNTER_TYPE_WATER:    //water
        timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_WATER);
        const struct WildPokemonInfo *waterMonsInfo = gWildMonHeaders[headerId].encounterTypes[timeOfDay].waterMonsInfo;

        if (waterMonsInfo == NULL)
            return MON_LEVEL_NONEXISTENT; //Hidden Pokémon should only appear on walkable tiles or surf tiles

        for (i = 0; i < NUM_WATER_MONS_ENCOUNTER_SLOTS; i++)
        {
            if (waterMonsInfo->wildPokemon[i].species == species)
            {
                min = (min < waterMonsInfo->wildPokemon[i].minLevel) ? min : waterMonsInfo->wildPokemon[i].minLevel;
                max = (max > waterMonsInfo->wildPokemon[i].maxLevel) ? max : waterMonsInfo->wildPokemon[i].maxLevel;
            }
        }
        break;
    case ENCOUNTER_TYPE_HIDDEN:
        timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_HIDDEN);
        const struct WildPokemonInfo *hiddenMonsInfo = gWildMonHeaders[headerId].encounterTypes[timeOfDay].hiddenMonsInfo;

        if (hiddenMonsInfo == NULL)
            return MON_LEVEL_NONEXISTENT;

        for (i = 0; i < NUM_HIDDEN_MONS_ENCOUNTER_SLOTS; i++)
        {
            if (hiddenMonsInfo->wildPokemon[i].species == species)
            {
                min = (min < hiddenMonsInfo->wildPokemon[i].minLevel) ? min : hiddenMonsInfo->wildPokemon[i].minLevel;
                max = (max > hiddenMonsInfo->wildPokemon[i].maxLevel) ? max : hiddenMonsInfo->wildPokemon[i].maxLevel;
            }
        }

        // use encounter rate to signify is hidden Pokémon are on land or in water
        if (hiddenMonsInfo->encounterRate == 1)
            sDexNavSearchDataPtr->environment = ENCOUNTER_TYPE_WATER;
        else
            sDexNavSearchDataPtr->environment = ENCOUNTER_TYPE_LAND;
        break;
    default:
        return MON_LEVEL_NONEXISTENT;
    }

    if (max == 0)
        return MON_LEVEL_NONEXISTENT;

    return RandomUniform(RNG_DEXNAV_ENCOUNTER_LEVEL, min, max);
}


///////////
/// GUI ///
///////////
static const struct BgTemplate sDexNavMenuBgTemplates[2] =
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .priority = 0
    },
    {
        .bg = 1,
        .charBaseIndex = 3,
        .mapBaseIndex = 30,
        .priority = 1
    }
};

static void DexNav_VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void DexNav_MainCB(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static bool8 DexNav_InitBgs(void)
{
    ResetVramOamAndBgCntRegs();
    ResetAllBgsCoordinates();
    sBg1TilemapBuffer = Alloc(0x800);
    if (sBg1TilemapBuffer == NULL)
        return FALSE;

    memset(sBg1TilemapBuffer, 0, 0x800);
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sDexNavMenuBgTemplates, NELEMS(sDexNavMenuBgTemplates));
    SetBgTilemapBuffer(1, sBg1TilemapBuffer);
    ScheduleBgCopyTilemapToVram(1);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
    SetGpuReg(REG_OFFSET_BLDCNT , 0);
    ShowBg(0);
    ShowBg(1);
    return TRUE;
}

static bool8 DexNav_LoadGraphics(void)
{
    switch (sDexNavUiDataPtr->state)
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(1, sDexNavGuiTiles, 0, 0, 0);
        sDexNavUiDataPtr->state++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            DecompressDataWithHeaderWram(sDexNavGuiTilemap, sBg1TilemapBuffer);
            sDexNavUiDataPtr->state++;
        }
        break;
    case 2:
        LoadPalette(BriskDarkMode() ? (const void *)sDexNavDarkGuiPal : sDexNavGuiPal, BG_PLTT_ID(0), PLTT_SIZE_4BPP);
        sDexNavUiDataPtr->state++;
        break;
    default:
        sDexNavUiDataPtr->state = 0;
        return TRUE;
    }

    return FALSE;
}

static void UpdateCursorPosition(void)
{
    u16 x, y;

    switch (sDexNavUiDataPtr->cursorRow)
    {
    case ROW_WATER:
        x = ROW_WATER_ICON_X + (24 * sDexNavUiDataPtr->cursorCol);
        y = ROW_WATER_ICON_Y;
        sDexNavUiDataPtr->environment = ENCOUNTER_TYPE_WATER;
        break;
    case ROW_LAND_TOP: //land 1
        x = ROW_LAND_ICON_X + (24 * sDexNavUiDataPtr->cursorCol);
        y = ROW_LAND_TOP_ICON_Y;
        sDexNavUiDataPtr->environment = ENCOUNTER_TYPE_LAND;
        break;
    case ROW_LAND_BOT: //land 2
        x = ROW_LAND_ICON_X + (24 * sDexNavUiDataPtr->cursorCol);
        y = ROW_LAND_BOT_ICON_Y;
        sDexNavUiDataPtr->environment = ENCOUNTER_TYPE_LAND;
        break;
    case ROW_HIDDEN:
        x = ROW_HIDDEN_ICON_X + (24 * sDexNavUiDataPtr->cursorCol);
        y = ROW_HIDDEN_ICON_Y;
        sDexNavUiDataPtr->environment = ENCOUNTER_TYPE_HIDDEN;
        break;
    default:
        return;
    }

    gSprites[sDexNavUiDataPtr->cursorSpriteId].x = x;
    gSprites[sDexNavUiDataPtr->cursorSpriteId].y = y;

    PrintCurrentSpeciesInfo();
    PrintDexNavPages();
}

static void CreateSelectionCursor(void)
{
    u8 spriteId;
    struct CompressedSpriteSheet spriteSheet;

    spriteSheet.data = sSelectionCursorGfx;
    spriteSheet.size = 0x200;
    spriteSheet.tag = SELECTION_CURSOR_TAG;
    LoadCompressedSpriteSheet(&spriteSheet);

    LoadPalette(sSelectionCursorPal, OBJ_PLTT_ID(sSelectionCursorOam.paletteNum), PLTT_SIZE_4BPP);

    spriteId = CreateSprite(&sSelectionCursorSpriteTemplate, 12, 32, 0);
    //gSprites[spriteId].data[1] = -1;

    sDexNavUiDataPtr->cursorSpriteId = spriteId;
    UpdateCursorPosition();
}

static bool32 CapturedAllTable(const struct WildPokemonInfo *info, u32 slots, bool32 hidden)
{
    u32 count = 0;
    if (info == NULL || (!hidden && info->encounterRate == 0))
        return FALSE;
    for (u32 i = 0; i < slots; i++)
    {
        enum Species species = info->wildPokemon[i].species;
        if (species == SPECIES_NONE || species >= NUM_SPECIES || species == SPECIES_EGG)
            continue;
        if (!GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
            return FALSE;
        count++;
    }
    return count != 0;
}

static bool8 CapturedAllLandMons(u32 headerId)
{
    enum TimeOfDay time = GetTimeOfDayForEncounters(headerId, WILD_AREA_LAND);
    return CapturedAllTable(gWildMonHeaders[headerId].encounterTypes[time].landMonsInfo, NUM_LAND_MONS_ENCOUNTER_SLOTS, FALSE);
}

static bool8 CapturedAllWaterMons(u32 headerId)
{
    enum TimeOfDay time = GetTimeOfDayForEncounters(headerId, WILD_AREA_WATER);
    return CapturedAllTable(gWildMonHeaders[headerId].encounterTypes[time].waterMonsInfo, NUM_WATER_MONS_ENCOUNTER_SLOTS, FALSE);
}

static bool8 CapturedAllHiddenMons(u32 headerId)
{
    enum TimeOfDay time = GetTimeOfDayForEncounters(headerId, WILD_AREA_HIDDEN);
    return CapturedAllTable(gWildMonHeaders[headerId].encounterTypes[time].hiddenMonsInfo, NUM_HIDDEN_MONS_ENCOUNTER_SLOTS, TRUE);
}

static void DexNavLoadCapturedAllSymbols(void)
{
    u32 headerId = GetCurrentMapWildMonHeaderId();
    if (headerId == HEADER_NONE)
        return;

    LoadCompressedSpriteSheetUsingHeap(&sCapturedAllPokemonSpriteSheet);

    if (CapturedAllLandMons(headerId))
        CreateSprite(&sCaptureAllMonsSpriteTemplate, 152, 58, 0);

    if (CapturedAllWaterMons(headerId))
        CreateSprite(&sCaptureAllMonsSpriteTemplate, 139, 17, 0);

    if (CapturedAllHiddenMons(headerId))
        CreateSprite(&sCaptureAllMonsSpriteTemplate, 114, 123, 0);
}

//#define WIN_DETAILS_TILE        0x3a3
static void DexNav_InitWindows(void)
{
    InitWindows(sDexNavGuiWindowTemplates);
    LoadPalette(GetTextWindowPalette(3), BG_PLTT_ID(15), PLTT_SIZE_4BPP);
    DeactivateAllTextPrinters();
    ScheduleBgCopyTilemapToVram(0);
}

static void DexNavGuiFreeResources(void)
{
    ClearDexNavIcons();
    FreeSpriteTilesByTag(ICON_GFX_TAG);
    Free(sDexNavUiDataPtr);
    sDexNavUiDataPtr = NULL;
    Free(sBg1TilemapBuffer);
    FreeAllWindowBuffers();
}

static void CB1_InitDexNavSearch(void)
{
    if (!gPaletteFade.active && !ArePlayerFieldControlsLocked() && gMain.callback2 == CB2_Overworld)
    {
        SetMainCallback1(CB1_Overworld);
        InitDexNavSearch(gSpecialVar_0x8000, gSpecialVar_0x8001);
    }
}

static void CB1_DexNavSearchCallback(void)
{
    CB1_InitDexNavSearch();
}

static void Task_DexNavExitAndSearch(u8 taskId)
{
    DespawnAllOverworldWildEncounters(OWE_GENERATED, 0);
    DexNavGuiFreeResources();
    DestroyTask(taskId);
    SetMainCallback1(CB1_DexNavSearchCallback);
    SetMainCallback2(CB2_ReturnToField);
}

static void Task_DexNavFadeAndExit(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sDexNavUiDataPtr->savedCallback);
        DexNavGuiFreeResources();
        DestroyTask(taskId);
    }
}

static void DexNavFadeAndExit(void)
{
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_DexNavFadeAndExit, 0);
    SetVBlankCallback(DexNav_VBlankCB);
    SetMainCallback2(DexNav_MainCB);
}

static u32 CopyEncounterSpecies(enum Species *dest, const struct WildPokemonInfo *info, u32 slots, bool32 hidden)
{
    u32 count = 0;
    memset(dest, 0, slots * sizeof(*dest));
    if (info == NULL || (!hidden && info->encounterRate == 0))
        return 0;
    for (u32 i = 0; i < slots; i++)
    {
        enum Species species = info->wildPokemon[i].species;
        if (species == SPECIES_NONE || species >= NUM_SPECIES || species == SPECIES_EGG)
            continue;
        u32 j;
        for (j = 0; j < count && dest[j] != species; j++) {}
        if (j == count)
            dest[count++] = species;
    }
    return count;
}

static void LoadDexNavTables(const struct WildPokemonInfo *land, const struct WildPokemonInfo *water, const struct WildPokemonInfo *hidden)
{
    sDexNavUiDataPtr->speciesCount[ENCOUNTER_TYPE_LAND] = CopyEncounterSpecies(sDexNavUiDataPtr->landSpecies, land, NUM_LAND_MONS_ENCOUNTER_SLOTS, FALSE);
    sDexNavUiDataPtr->speciesCount[ENCOUNTER_TYPE_WATER] = CopyEncounterSpecies(sDexNavUiDataPtr->waterSpecies, water, NUM_WATER_MONS_ENCOUNTER_SLOTS, FALSE);
    sDexNavUiDataPtr->speciesCount[ENCOUNTER_TYPE_HIDDEN] = CopyEncounterSpecies(sDexNavUiDataPtr->hiddenSpecies, hidden, NUM_HIDDEN_MONS_ENCOUNTER_SLOTS, TRUE);
    memset(sDexNavUiDataPtr->page, 0, sizeof(sDexNavUiDataPtr->page));
}

static void DexNavLoadEncounterData(void)
{
    u32 headerId = GetCurrentMapWildMonHeaderId();
    if (headerId == HEADER_NONE)
    {
        LoadDexNavTables(NULL, NULL, NULL);
        return;
    }
    enum TimeOfDay landTime = GetTimeOfDayForEncounters(headerId, WILD_AREA_LAND);
    enum TimeOfDay waterTime = GetTimeOfDayForEncounters(headerId, WILD_AREA_WATER);
    enum TimeOfDay hiddenTime = GetTimeOfDayForEncounters(headerId, WILD_AREA_HIDDEN);
    LoadDexNavTables(gWildMonHeaders[headerId].encounterTypes[landTime].landMonsInfo,
                     gWildMonHeaders[headerId].encounterTypes[waterTime].waterMonsInfo,
                     gWildMonHeaders[headerId].encounterTypes[hiddenTime].hiddenMonsInfo);
}

static u32 GetDexNavPageSize(u32 section)
{
    return section == ENCOUNTER_TYPE_LAND ? DEXNAV_LAND_PAGE_SIZE : section == ENCOUNTER_TYPE_WATER ? COL_WATER_COUNT : COL_HIDDEN_COUNT;
}

static u32 GetDexNavPageCount(u32 section)
{
    u32 size = GetDexNavPageSize(section);
    return max(1, (sDexNavUiDataPtr->speciesCount[section] + size - 1) / size);
}

static enum Species GetDexNavPageSpecies(u32 section, u32 slot)
{
    u32 index = sDexNavUiDataPtr->page[section] * GetDexNavPageSize(section) + slot;
    if (index >= sDexNavUiDataPtr->speciesCount[section])
        return SPECIES_NONE;
    switch (section)
    {
    case ENCOUNTER_TYPE_LAND: return sDexNavUiDataPtr->landSpecies[index];
    case ENCOUNTER_TYPE_WATER: return sDexNavUiDataPtr->waterSpecies[index];
    default: return sDexNavUiDataPtr->hiddenSpecies[index];
    }
}

static u32 GetDexNavRowCount(u32 row)
{
    u32 section = row == ROW_WATER ? ENCOUNTER_TYPE_WATER : row == ROW_HIDDEN ? ENCOUNTER_TYPE_HIDDEN : ENCOUNTER_TYPE_LAND;
    u32 offset = sDexNavUiDataPtr->page[section] * GetDexNavPageSize(section);
    u32 remaining = sDexNavUiDataPtr->speciesCount[section] - offset;
    if (row == ROW_LAND_TOP) return min(COL_LAND_COUNT, remaining);
    if (row == ROW_LAND_BOT) return remaining > COL_LAND_COUNT ? min(COL_LAND_COUNT, remaining - COL_LAND_COUNT) : 0;
    return min(GetDexNavPageSize(section), remaining);
}

static void PrintDexNavPages(void)
{
    u32 section = sDexNavUiDataPtr->environment;
    FillWindowPixelBuffer(WINDOW_PAGES, PIXEL_FILL(1));
    StringCopy(gStringVar4, section == ENCOUNTER_TYPE_LAND ? COMPOUND_STRING("L/R LAND ")
        : section == ENCOUNTER_TYPE_WATER ? COMPOUND_STRING("L/R WATER ") : COMPOUND_STRING("HIDDEN "));
    ConvertIntToDecimalStringN(gStringVar1, sDexNavUiDataPtr->page[section] + 1, STR_CONV_MODE_LEFT_ALIGN, 2);
    StringAppend(gStringVar4, gStringVar1);
    StringAppend(gStringVar4, COMPOUND_STRING("/"));
    ConvertIntToDecimalStringN(gStringVar1, GetDexNavPageCount(section), STR_CONV_MODE_LEFT_ALIGN, 2);
    StringAppend(gStringVar4, gStringVar1);
    AddTextPrinterParameterized3(WINDOW_PAGES, FONT_SMALL, 0, 0, sFontColor_Black, TEXT_SKIP_DRAW, gStringVar4);
    PutWindowTilemap(WINDOW_PAGES);
    CopyWindowToVram(WINDOW_PAGES, COPYWIN_FULL);
}

static void ClearDexNavIcons(void)
{
    for (u32 i = 0; i < DEXNAV_VISIBLE_ICONS; i++)
    {
        u32 id = sDexNavUiDataPtr->iconSpriteIds[i];
        if (id < MAX_SPRITES)
        {
            if (sDexNavUiDataPtr->iconIsMon[i])
                FreeAndDestroyMonIconSprite(&gSprites[id]);
            else
                DestroySprite(&gSprites[id]);
        }
        sDexNavUiDataPtr->iconSpriteIds[i] = MAX_SPRITES;
    }
}

static void DrawDexNavIcon(u32 section, u32 slot, u32 icon, s16 x, s16 y)
{
    enum Species species = GetDexNavPageSpecies(section, slot);
    bool32 concealed = section == ENCOUNTER_TYPE_HIDDEN && !FlagGet(DN_FLAG_DETECTOR_MODE);
    sDexNavUiDataPtr->iconIsMon[icon] = species != SPECIES_NONE;
    if (species == SPECIES_NONE)
        sDexNavUiDataPtr->iconSpriteIds[icon] = CreateSprite(&sNoDataIconTemplate, x, y, 0);
    else
    {
        if (concealed || !GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_SEEN))
            species = SPECIES_NONE;
        sDexNavUiDataPtr->iconSpriteIds[icon] = CreateMonIcon(species, SpriteCB_MonIcon, x, y, 0, 0xFFFFFFFF);
    }
}

static void DrawSpeciesIcons(void)
{
    ClearDexNavIcons();
    if (GetSpriteTileStartByTag(ICON_GFX_TAG) == 0xFFFF)
        LoadCompressedSpriteSheetUsingHeap(&sNoDataIconSpriteSheet);
    for (u32 i = 0; i < DEXNAV_LAND_PAGE_SIZE; i++)
        DrawDexNavIcon(ENCOUNTER_TYPE_LAND, i, i, ROW_LAND_ICON_X + 24 * (i % COL_LAND_COUNT), ROW_LAND_TOP_ICON_Y + 28 * (i / COL_LAND_COUNT));
    for (u32 i = 0; i < COL_WATER_COUNT; i++)
        DrawDexNavIcon(ENCOUNTER_TYPE_WATER, i, DEXNAV_LAND_PAGE_SIZE + i, ROW_WATER_ICON_X + 24 * i, ROW_WATER_ICON_Y);
    for (u32 i = 0; i < COL_HIDDEN_COUNT; i++)
        DrawDexNavIcon(ENCOUNTER_TYPE_HIDDEN, i, DEXNAV_LAND_PAGE_SIZE + COL_WATER_COUNT + i, ROW_HIDDEN_ICON_X + 24 * i, ROW_HIDDEN_ICON_Y);
    PrintDexNavPages();
}

static enum Species DexNavGetSpecies(void)
{
    enum Species species;

    switch (sDexNavUiDataPtr->cursorRow)
    {
    case ROW_WATER:
        species = GetDexNavPageSpecies(ENCOUNTER_TYPE_WATER, sDexNavUiDataPtr->cursorCol);
        break;
    case ROW_LAND_TOP:
        species = GetDexNavPageSpecies(ENCOUNTER_TYPE_LAND, sDexNavUiDataPtr->cursorCol);
        break;
    case ROW_LAND_BOT:
        species = GetDexNavPageSpecies(ENCOUNTER_TYPE_LAND, sDexNavUiDataPtr->cursorCol + COL_LAND_COUNT);
        break;
    case ROW_HIDDEN:
        if (!FlagGet(DN_FLAG_DETECTOR_MODE))
            species = SPECIES_NONE;
        else
            species = GetDexNavPageSpecies(ENCOUNTER_TYPE_HIDDEN, sDexNavUiDataPtr->cursorCol);
        break;
    default:
        return SPECIES_NONE;
    }

    if (species == SPECIES_NONE || species >= NUM_SPECIES)
        return SPECIES_NONE;

    if (!GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_SEEN))
        return SPECIES_NONE;

    return species;
}

static void SetSpriteInvisibility(u8 spriteArrayId, bool8 invisible)
{
    gSprites[sDexNavUiDataPtr->typeIconSpriteIds[spriteArrayId]].invisible = invisible;
}

// different from pokemon_summary_screen
#define TYPE_ICON_PAL_NUM_0     13
#define TYPE_ICON_PAL_NUM_1     14
#define TYPE_ICON_PAL_NUM_2     15
static const u8 sMoveTypeToOamPaletteNum[NUMBER_OF_MON_TYPES] =
{
    [TYPE_NORMAL] = TYPE_ICON_PAL_NUM_0,
    [TYPE_FIGHTING] = TYPE_ICON_PAL_NUM_0,
    [TYPE_FLYING] = TYPE_ICON_PAL_NUM_1,
    [TYPE_POISON] = TYPE_ICON_PAL_NUM_1,
    [TYPE_GROUND] = TYPE_ICON_PAL_NUM_0,
    [TYPE_ROCK] = TYPE_ICON_PAL_NUM_0,
    [TYPE_BUG] = TYPE_ICON_PAL_NUM_2,
    [TYPE_GHOST] = TYPE_ICON_PAL_NUM_1,
    [TYPE_STEEL] = TYPE_ICON_PAL_NUM_0,
    [TYPE_MYSTERY] = TYPE_ICON_PAL_NUM_2,
    [TYPE_FIRE] = TYPE_ICON_PAL_NUM_0,
    [TYPE_WATER] = TYPE_ICON_PAL_NUM_1,
    [TYPE_GRASS] = TYPE_ICON_PAL_NUM_2,
    [TYPE_ELECTRIC] = TYPE_ICON_PAL_NUM_0,
    [TYPE_PSYCHIC] = TYPE_ICON_PAL_NUM_1,
    [TYPE_ICE] = TYPE_ICON_PAL_NUM_1,
    [TYPE_DRAGON] = TYPE_ICON_PAL_NUM_2,
    [TYPE_DARK] = TYPE_ICON_PAL_NUM_0,
    [TYPE_FAIRY] = TYPE_ICON_PAL_NUM_1,
};
static void SetTypeIconPosAndPal(u8 typeId, u8 x, u8 y, u8 spriteArrayId)
{
    struct Sprite *sprite;

    sprite = &gSprites[sDexNavUiDataPtr->typeIconSpriteIds[spriteArrayId]];
    StartSpriteAnim(sprite, typeId);
    sprite->oam.paletteNum = sMoveTypeToOamPaletteNum[typeId];
    sprite->x = x + 16;
    sprite->y = y + 8;
    SetSpriteInvisibility(spriteArrayId, FALSE);
}

static void PrintCurrentSpeciesInfo(void)
{
    enum Species species = DexNavGetSpecies();
    enum NationalDexOrder dexNum = SpeciesToNationalPokedexNum(species);
    enum Type type1, type2;

    if (!GetSetPokedexFlag(dexNum, FLAG_GET_SEEN))
        species = SPECIES_NONE;

    // clear windows
    FillWindowPixelBuffer(WINDOW_INFO, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    //species name
    if (species == SPECIES_NONE)
        AddTextPrinterParameterized3(WINDOW_INFO, FONT_SMALL, 0, SPECIES_INFO_Y, sFontColor_Black, 0, sText_DexNav_NoInfo);
    else
        AddTextPrinterParameterized3(WINDOW_INFO, FONT_SMALL, 0, SPECIES_INFO_Y, sFontColor_Black, 0, GetSpeciesName(species));

    //type icon(s)
    type1 = GetSpeciesType(species, 0);
    type2 = GetSpeciesType(species, 1);
    if (species == SPECIES_NONE)
        type1 = type2 = TYPE_MYSTERY;

    if (type1 == type2)
    {
        SetTypeIconPosAndPal(type1, 186, 69, 0);
        SetSpriteInvisibility(1, TRUE);
    }
    else
    {
        SetTypeIconPosAndPal(type1, 168, 69, 0);
        SetTypeIconPosAndPal(type2, 168 + 33, 69, 1);
    }

    //search level
    if (species == SPECIES_NONE)
    {
        AddTextPrinterParameterized3(WINDOW_INFO, FONT_SMALL, 0, SEARCH_LEVEL_Y, sFontColor_Black, 0, sText_DexNav_NoInfo);
    }
    else
    {
        ConvertIntToDecimalStringN(gStringVar4, GetSearchLevel(species), 0, 4);
        AddTextPrinterParameterized3(WINDOW_INFO, FONT_SMALL, 0, SEARCH_LEVEL_Y, sFontColor_Black, 0, gStringVar4);
    }

    //hidden ability
    if (species == SPECIES_NONE)
    {
        AddTextPrinterParameterized3(WINDOW_INFO, FONT_SMALL, 0, HA_INFO_Y, sFontColor_Black, 0, sText_DexNav_NoInfo);
    }
    else if (GetSetPokedexFlag(dexNum, FLAG_GET_CAUGHT))
    {
        if (GetSpeciesAbility(species, 2) != ABILITY_NONE)
            AddTextPrinterParameterized3(WINDOW_INFO, FONT_SMALL, 0, HA_INFO_Y, sFontColor_Black, 0, gAbilitiesInfo[GetSpeciesAbility(species, 2)].name);
        else
            AddTextPrinterParameterized3(WINDOW_INFO, FONT_SMALL, 0, HA_INFO_Y, sFontColor_Black, 0, gText_None);
    }
    else
    {
        AddTextPrinterParameterized3(WINDOW_INFO, FONT_SMALL, 0, HA_INFO_Y, sFontColor_Black, 0, sText_DexNav_CaptureToSee);
    }

    //current chain
    ConvertIntToDecimalStringN(gStringVar1, gSaveBlock3Ptr->dexNavChain, STR_CONV_MODE_LEFT_ALIGN, 3);
    AddTextPrinterParameterized3(WINDOW_INFO, FONT_SMALL, 0, CHAIN_BONUS_Y, sFontColor_Black, 0, gStringVar1);

    CopyWindowToVram(WINDOW_INFO, 3);
    PutWindowTilemap(WINDOW_INFO);
}

static void PrintMapName(void)
{
    GetMapName(gStringVar3, GetCurrentRegionMapSectionId(), 0);
    AddTextPrinterParameterized3(WINDOW_REGISTERED, FONT_SMALL,
        GetStringRightAlignXOffset(FONT_SMALL, gStringVar3, 236), 0, sFontColor_White, TEXT_SKIP_DRAW, gStringVar3);
    CopyWindowToVram(WINDOW_REGISTERED, 3);
}

static void PrintSearchableSpecies(enum Species species)
{
    FillWindowPixelBuffer(WINDOW_REGISTERED, PIXEL_FILL(15));
    PutWindowTilemap(WINDOW_REGISTERED);
    if (species == SPECIES_NONE)
    {
        AddTextPrinterParameterized3(WINDOW_REGISTERED, FONT_NORMAL, 0, 0, sFontColor_White, TEXT_SKIP_DRAW, sText_DexNav_PressSelectToRegister);
    }
    else
    {
        StringCopy(gStringVar1, GetSpeciesName(species));
        StringExpandPlaceholders(gStringVar4, sText_DexNav_SearchForRegisteredSpecies);
        AddTextPrinterParameterized3(WINDOW_REGISTERED, GetFontIdToFit(gStringVar4, FONT_NORMAL, 0, 124), 0, 0, sFontColor_White, TEXT_SKIP_DRAW, gStringVar4);
    }

    PrintMapName();
}

static void CreateTypeIconSprites(void)
{
    u8 i;

    LoadCompressedSpriteSheet(&gSpriteSheet_MoveTypes);
    LoadPalette(gMoveTypes_Pal, OBJ_PLTT_ID(13), 3 * PLTT_SIZE_4BPP);
    for (i = 0; i < 2; i++)
    {
        if (sDexNavUiDataPtr->typeIconSpriteIds[i] == 0xFF)
            sDexNavUiDataPtr->typeIconSpriteIds[i] = CreateSprite(&gSpriteTemplate_MoveTypes, 10, 10, 2);

        SetSpriteInvisibility(i, TRUE);
    }
}

#define tSpecies            data[2]
#define tEnvironment        data[3]

static bool8 DexNav_DoGfxSetup(void)
{
    u8 taskId;

    switch (gMain.state)
    {
    case 0:
        SetVBlankHBlankCallbacksToNull();
        ClearScheduledBgCopiesToVram();
        gMain.state++;
        break;
    case 1:
        ScanlineEffect_Stop();
        gMain.state++;
        break;
    case 2:
        FreeAllSpritePalettes();
        gMain.state++;
        break;
    case 3:
        ResetPaletteFade();
        ResetSpriteData();
        ResetTasks();
        gMain.state++;
        break;
    case 4:
        if (DexNav_InitBgs())
        {
            sDexNavUiDataPtr->state = 0;
            gMain.state++;
        }
        else
        {
            DexNavFadeAndExit();
            return TRUE;
        }
        break;
    case 5:
        if (DexNav_LoadGraphics() == TRUE)
            gMain.state++;
        break;
    case 6:
        DexNav_InitWindows();
        sDexNavUiDataPtr->cursorRow = ROW_LAND_TOP;
        sDexNavUiDataPtr->cursorCol = 0;
        sDexNavUiDataPtr->environment = ENCOUNTER_TYPE_LAND;
        gMain.state++;
        break;
    case 7:
        PrintSearchableSpecies(VarGet(DN_VAR_SPECIES) & DEXNAV_MASK_SPECIES);
        DexNavLoadEncounterData();
        gMain.state++;
        break;
    case 8:
        taskId = CreateTask(Task_DexNavWaitFadeIn, 0);
        gTasks[taskId].tSpecies = 0;
        gTasks[taskId].tEnvironment = sDexNavUiDataPtr->environment;
        gMain.state++;
        break;
    case 9:
        sDexNavUiDataPtr->typeIconSpriteIds[0] = 0xFF;
        sDexNavUiDataPtr->typeIconSpriteIds[1] = 0xFF;
        CreateTypeIconSprites();
        gMain.state++;
        break;
    case 10:
        LoadMonIconPalettes();
        DrawSpeciesIcons();
        CreateSelectionCursor();
        DexNavLoadCapturedAllSymbols();
        gMain.state++;
        break;
    case 11:
        BlendPalettes(0xFFFFFFFF, 16, RGB_BLACK);
        gMain.state++;
        break;
    case 12:
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
        gMain.state++;
        break;
    default:
        SetVBlankCallback(DexNav_VBlankCB);
        SetMainCallback2(DexNav_MainCB);
        return TRUE;
    }

    return FALSE;
}

static void DexNav_RunSetup(void)
{
    while (!DexNav_DoGfxSetup()) {}
}

// Entry point for the dexnav GUI
static void DexNavGuiInit(MainCallback callback)
{
    assertf(DEXNAV_ENABLED, "DexNav was opened when DEXNAV_ENABLED config was disabled.\nCheck include/config/dexnav.h")
    {
        SetMainCallback2(callback);
        return;
    }

    if ((sDexNavUiDataPtr = AllocZeroed(sizeof(struct DexNavGUI))) == NULL)
    {
        SetMainCallback2(callback);
        return;
    }

    memset(sDexNavUiDataPtr->iconSpriteIds, MAX_SPRITES, sizeof(sDexNavUiDataPtr->iconSpriteIds));
    sDexNavUiDataPtr->state = 0;
    sDexNavUiDataPtr->savedCallback = callback;
    SetMainCallback2(DexNav_RunSetup);
}

void Task_OpenDexNavFromStartMenu(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        DexNavGuiInit(CB2_ReturnToFieldWithOpenMenu);
        DestroyTask(taskId);
    }
}

static void Task_DexNavWaitFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_DexNavMain;
}

static void Task_DexNavMain(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    enum Species species;

    if (IsSEPlaying())
        return;

    if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_POKENAV_OFF);
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
        task->func = Task_DexNavFadeAndExit;
    }
    else if (JOY_NEW(L_BUTTON | R_BUTTON))
    {
        u32 section = sDexNavUiDataPtr->environment;
        u32 pages = GetDexNavPageCount(section);
        sDexNavUiDataPtr->page[section] = (sDexNavUiDataPtr->page[section] + (JOY_NEW(R_BUTTON) ? 1 : pages - 1)) % pages;
        if (GetDexNavRowCount(sDexNavUiDataPtr->cursorRow) == 0 && section == ENCOUNTER_TYPE_LAND)
            sDexNavUiDataPtr->cursorRow = ROW_LAND_TOP;
        u32 count = GetDexNavRowCount(sDexNavUiDataPtr->cursorRow);
        sDexNavUiDataPtr->cursorCol = min(sDexNavUiDataPtr->cursorCol, max(1, count) - 1);
        DrawSpeciesIcons();
        UpdateCursorPosition();
        PlaySE(SE_RG_BAG_CURSOR);
    }
    else if (JOY_REPEAT(DPAD_UP | DPAD_DOWN))
    {
        u32 step = JOY_REPEAT(DPAD_DOWN) ? 1 : ROWS_COUNT - 1;
        for (u32 i = 0; i < ROWS_COUNT; i++)
        {
            sDexNavUiDataPtr->cursorRow = (sDexNavUiDataPtr->cursorRow + step) % ROWS_COUNT;
            if (GetDexNavRowCount(sDexNavUiDataPtr->cursorRow) != 0)
                break;
        }
        u32 count = GetDexNavRowCount(sDexNavUiDataPtr->cursorRow);
        sDexNavUiDataPtr->cursorCol = min(sDexNavUiDataPtr->cursorCol, max(1, count) - 1);
        PlaySE(SE_RG_BAG_CURSOR);
        UpdateCursorPosition();
    }
    else if (JOY_REPEAT(DPAD_LEFT | DPAD_RIGHT))
    {
        u32 count = max(1, GetDexNavRowCount(sDexNavUiDataPtr->cursorRow));
        sDexNavUiDataPtr->cursorCol = (sDexNavUiDataPtr->cursorCol + (JOY_REPEAT(DPAD_RIGHT) ? 1 : count - 1)) % count;
        PlaySE(SE_RG_BAG_CURSOR);
        UpdateCursorPosition();
    }
    else if (JOY_NEW(SELECT_BUTTON))
    {
        // check selection is valid. Play sound if invalid
        species = DexNavGetSpecies();

        if (species != SPECIES_NONE)
        {
            PrintSearchableSpecies(species);
            //PlaySE(SE_DEX_SEARCH);
            PlayCry_Script(species, 0);

            // create value to store in a var
            VarSet(DN_VAR_SPECIES, ((sDexNavUiDataPtr->environment << 14) | species));
        }
        else
        {
            PlaySE(SE_FAILURE);
        }
    }
    else if (JOY_NEW(A_BUTTON))
    {
        species = DexNavGetSpecies();
        if (species == SPECIES_NONE)
        {
            PlaySE(SE_FAILURE);
        }
        else
        {
            gSpecialVar_0x8000 = species;
            gSpecialVar_0x8001 = sDexNavUiDataPtr->environment;
            gSpecialVar_0x8002 = (sDexNavUiDataPtr->cursorRow == ROW_HIDDEN) ? TRUE : FALSE;
            PlaySE(SE_DEX_SEARCH);
            BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
            task->func = Task_DexNavExitAndSearch;
        }
    }
}

/////////////////////////
//// HIDDEN POKEMON /////
/////////////////////////
bool32 TryFindHiddenPokemon(void)
{
    u16 *stepPtr = GetVarPointer(DN_VAR_STEP_COUNTER);

    if (DEXNAV_ENABLED == 0
            || sDexNavSearchDataPtr == NULL
            || !FlagGet(DN_FLAG_DETECTOR_MODE)
            || FlagGet(DN_FLAG_SEARCHING)
            || GetFlashLevel() > 0)
    {
        if (stepPtr != NULL)
            (*stepPtr) = 0;
        return FALSE;
    }

    (*stepPtr)++;
    (*stepPtr) %= HIDDEN_MON_STEP_COUNT;
    if ((*stepPtr) == 0 && (Random() % 100 < HIDDEN_MON_SEARCH_RATE))
    {
        // hidden Pokémon
        u32 headerId = GetCurrentMapWildMonHeaderId();
        u8 index;
        enum Species species;
        enum EncounterType environment;

        if (headerId == HEADER_NONE)
            return FALSE;

        enum TimeOfDay timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_HIDDEN);
        const struct WildPokemonInfo *hiddenMonsInfo = gWildMonHeaders[headerId].encounterTypes[timeOfDay].hiddenMonsInfo;
        bool8 isHiddenMon = FALSE;

        // while you can still technically find hidden Pokémon if there are not hidden-only Pokémon on a map,
        // this prevents any potential lagging on maps you dont want hidden Pokémon to appear on
        if (hiddenMonsInfo == NULL)
            return FALSE;

        // encounter rate signifies surfing (1) or land mons (0)!
        // again, for simplicity
        switch (hiddenMonsInfo->encounterRate)
        {
        case 0: // land
            // there are surely better ways to do this, but this allows greatest flexibility
            if (Random() % 100 < HIDDEN_MON_PROBABILTY)
            {
                index = ChooseHiddenMonIndex();
                if (index == 0xFF)
                    return FALSE;//no hidden info
                species = hiddenMonsInfo->wildPokemon[index].species;
                isHiddenMon = TRUE;
                environment = ENCOUNTER_TYPE_HIDDEN;
            }
            else
            {
                species = gWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo->wildPokemon[
                    ChooseWildMonIndex_Land(gWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo->wildPokemon)
                ].species;
                environment = ENCOUNTER_TYPE_LAND;
            }
            break;
        case 1: // water
            if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING))
            {
                if (Random() % 100 < HIDDEN_MON_PROBABILTY)
                {
                    index = ChooseHiddenMonIndex();
                    if (index == 0xFF)
                        return FALSE;//no hidden info
                    species = hiddenMonsInfo->wildPokemon[index].species;
                    isHiddenMon = TRUE;
                    environment = ENCOUNTER_TYPE_HIDDEN;
                }
                else
                {
                    species = gWildMonHeaders[headerId].encounterTypes[timeOfDay].waterMonsInfo->wildPokemon[
                        ChooseWildMonIndex_Water(gWildMonHeaders[headerId].encounterTypes[timeOfDay].waterMonsInfo->wildPokemon)
                    ].species;
                    environment = ENCOUNTER_TYPE_WATER;

                }
            }
            else
            {
                // not surfing -> can't find hidden water mons
                return FALSE;
            }
            break;
        default:
            return FALSE;
        }

        if (species == SPECIES_NONE)
            return FALSE;

        sDexNavSearchDataPtr = AllocZeroed(sizeof(struct DexNavSearch));
        FlagSet(DN_FLAG_SEARCHING);
        // init search data
        sDexNavSearchDataPtr->isHiddenMon = isHiddenMon;
        sDexNavSearchDataPtr->species = species;
        sDexNavSearchDataPtr->hiddenSearch = TRUE;
        sDexNavSearchDataPtr->environment = environment;    // updated in DexNavTryGenerateMonLevel if hidden mon
        sDexNavSearchDataPtr->monLevel = DexNavTryGenerateMonLevel(species, environment);
        if (sDexNavSearchDataPtr->monLevel == MON_LEVEL_NONEXISTENT)
        {
            FREE_AND_SET_NULL(sDexNavSearchDataPtr);
            FlagClear(DN_FLAG_SEARCHING);
            return FALSE;
        }

        // find tile for hidden mon and start effect if possible
        if (!TryStartHiddenMonFieldEffect(sDexNavSearchDataPtr->environment, 8, 8, TRUE))
        {
            FREE_AND_SET_NULL(sDexNavSearchDataPtr);
            FlagClear(DN_FLAG_SEARCHING);
            return FALSE;
        }

        // exclamation mark over player
        gFieldEffectArguments[0] = gSaveBlock1Ptr->pos.x;
        gFieldEffectArguments[1] = gSaveBlock1Ptr->pos.y;
        gFieldEffectArguments[2] = gSprites[gPlayerAvatar.spriteId].subpriority - 1;
        gFieldEffectArguments[3] = 2;
        ObjectEventGetLocalIdAndMap(&gObjectEvents[gPlayerAvatar.objectEventId], &gFieldEffectArguments[0], &gFieldEffectArguments[1], &gFieldEffectArguments[2]);
        FieldEffectStart(FLDEFF_EXCLAMATION_MARK_ICON);

        PlayCry_Script(species, 0);
        SetUpDexNavSearch();
        HideMapNamePopUpWindow();
        ChangeBgY_ScreenOff(0, 0, 0);
        return FALSE;   // we dont actually want to enable the script context or the game will freeze
    }

    return FALSE;
}

static void DrawSearchIcon(void)
{
    struct CompressedSpriteSheet spriteSheet;

    spriteSheet.data = sHiddenSearchIconGfx;
    spriteSheet.size = 0x200;
    spriteSheet.tag = SELECTION_CURSOR_TAG;
    LoadCompressedSpriteSheet(&spriteSheet);
    sDexNavSearchDataPtr->iconSpriteId = CreateSprite(&sSearchIconSpriteTemplate, 18, GetSearchWindowY() + 12, 0);
}

// the initial hidden icon window ONLY shows search icon, ??? instead of name, and the search level (and pokeball icon if owned)
// if the player presses R or moves close enough, the full search window will be created
// this way, if the player is not interested in hidden Pokémon it will not be too intrusive
static void DrawHiddenSearchWindow(u8 width)
{
    AddSearchWindow(width);
    AddTextPrinterParameterized3(sDexNavSearchDataPtr->windowId, FONT_SMALL, SPECIES_ICON_X + 4, 0, sSearchFontColor, TEXT_SKIP_DRAW, sText_ThreeQmarks);

    ConvertIntToDecimalStringN(gStringVar1, sDexNavSearchDataPtr->searchLevel, STR_CONV_MODE_LEFT_ALIGN, 2);
    StringExpandPlaceholders(gStringVar4, sText_SearchLevel);
    AddTextPrinterParameterized3(sDexNavSearchDataPtr->windowId, FONT_SMALL, SPECIES_ICON_X + 4, 12, sSearchFontColor, TEXT_SKIP_DRAW, gStringVar4);
    CopyWindowToVram(sDexNavSearchDataPtr->windowId, 2);
}

static void DexNavDrawHiddenIcons(void)
{
    enum Species species = sDexNavSearchDataPtr->species;

    DrawHiddenSearchWindow(12);
    DrawSearchIcon();

    if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
        sDexNavSearchDataPtr->ownedIconSpriteId = CreateSprite(&sOwnedIconTemplate, SPECIES_ICON_X + 6, GetSearchWindowY() + 2, 0);

    if (sDexNavSearchDataPtr->isHiddenMon)
        sDexNavSearchDataPtr->exclamationSpriteId = CreateSprite(&sHiddenMonIconTemplate, SPECIES_ICON_X + 34, GetSearchWindowY() + 8, 0);
}

/////////////////////////
//// GENERAL UTILITY ////
/////////////////////////
u32 CalculateDexNavShinyRolls(void)
{
    u32 chainBonus, rndBonus;
    u8 chain = gSaveBlock3Ptr->dexNavChain;

    chainBonus = (chain >= 100) ? 10 : (chain >= 50) ? 5 : 0;
    rndBonus = (Random() % 100 < 4) ? 4 : 0;
    return chainBonus + rndBonus;
}

void TryIncrementSpeciesSearchLevel()
{
#if USE_DEXNAV_SEARCH_LEVELS == TRUE
    if (gMapHeader.regionMapSectionId != MAPSEC_BATTLE_FRONTIER && gSaveBlock3Ptr->dexNavSearchLevels[gDexNavSpecies] < 255)
        gSaveBlock3Ptr->dexNavSearchLevels[gDexNavSpecies]++;
#endif
}

void ResetDexNavSearch(void)
{
    gSaveBlock3Ptr->dexNavChain = 0;    //reset dex nav chaining on new map
    VarSet(DN_VAR_STEP_COUNTER, 0); //reset hidden Pokémon step counter
    if (FlagGet(DN_FLAG_SEARCHING))
        EndDexNavSearch();   //moving to new map ends dexnav search
}

void IncrementDexNavChain(void)
{
    if (gSaveBlock3Ptr->dexNavChain < DEXNAV_CHAIN_MAX)
        gSaveBlock3Ptr->dexNavChain++;
}

#if TESTING
u8 Test_OpenBriskDexNav(const struct WildPokemonInfo *land, const struct WildPokemonInfo *water, const struct WildPokemonInfo *hidden)
{
    ResetTasks();
    ResetSpriteData();
    FreeAllSpritePalettes();
    sDexNavUiDataPtr = AllocZeroed(sizeof(*sDexNavUiDataPtr));
    memset(sDexNavUiDataPtr->iconSpriteIds, MAX_SPRITES, sizeof(sDexNavUiDataPtr->iconSpriteIds));
    DexNav_InitBgs();
    DexNav_InitWindows();
    LoadDexNavTables(land, water, hidden);
    sDexNavUiDataPtr->cursorRow = ROW_LAND_TOP;
    sDexNavUiDataPtr->environment = ENCOUNTER_TYPE_LAND;
    sDexNavUiDataPtr->typeIconSpriteIds[0] = 0xFF;
    sDexNavUiDataPtr->typeIconSpriteIds[1] = 0xFF;
    CreateTypeIconSprites();
    LoadMonIconPalettes();
    DrawSpeciesIcons();
    CreateSelectionCursor();
    gPaletteFade.active = FALSE;
    return CreateTask(Task_DexNavMain, 0);
}

void Test_BriskDexNavInput(u8 task, u16 keys)
{
    gMPlayInfo_SE1.status = gMPlayInfo_SE2.status = 0;
    gMain.newKeys = gMain.newAndRepeatedKeys = keys;
    Task_DexNavMain(task);
    gMain.newKeys = gMain.newAndRepeatedKeys = 0;
}

enum Species Test_BriskDexNavSpecies(void) { return DexNavGetSpecies(); }
u32 Test_BriskDexNavCount(u32 section) { return sDexNavUiDataPtr->speciesCount[section]; }
u32 Test_BriskDexNavPages(u32 section) { return GetDexNavPageCount(section); }
u32 Test_BriskDexNavPage(u32 section) { return sDexNavUiDataPtr->page[section]; }
u32 Test_BriskDexNavRow(void) { return sDexNavUiDataPtr->cursorRow; }
u32 Test_BriskDexNavCol(void) { return sDexNavUiDataPtr->cursorCol; }
bool32 Test_BriskDexNavCaught(const struct WildPokemonInfo *info, u32 slots) { return CapturedAllTable(info, slots, FALSE); }

void Test_CloseBriskDexNav(u8 task)
{
    DestroySprite(&gSprites[sDexNavUiDataPtr->cursorSpriteId]);
    for (u32 i = 0; i < 2; i++)
        DestroySprite(&gSprites[sDexNavUiDataPtr->typeIconSpriteIds[i]]);
    DexNavGuiFreeResources();
    FreeAllSpritePalettes();
    DestroyTask(task);
    // Registration starts an audio ducking task that normally outlives the UI.
    ResetTasks();
}
#endif
