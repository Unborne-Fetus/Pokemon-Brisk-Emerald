#include "global.h"
#include "brisk_battle_format.h"
#include "brisk_options.h"
#include "brisk_modes.h"
#include "battle.h"
#include "pokemon.h"
#include "option_menu.h"
#include "bg.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "main.h"
#include "menu.h"
#include "palette.h"
#include "scanline_effect.h"
#include "sprite.h"
#include "strings.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "window.h"
#include "gba/m4a_internal.h"
#include "event_data.h"
#include "constants/flags.h"
#include "config/overworld.h"
#include "constants/rgb.h"
#include "string_util.h"
#include "player_name.h"
#include "m4a.h"
#include "debug.h"
#include "overworld.h"
#include "field_screen_effect.h"
#include "field_weather.h"
#include "event_object_movement.h"
#include "script.h"

#define tMenuSelection data[0]
#define tTextSpeed data[1]
#define tBattleSceneOff data[2]
#define tBattleStyle data[3]
#define tSound data[4]
#define tButtonMode data[5]
#define tWindowFrameType data[6]
#define tBattleSpeed data[7]
#define tOverworldSpeed data[8]
#define tAutoRunDisabled data[9]
#define tSkipMoves data[10]
#define tNoLevelCap data[11]
#define tRematches data[12]
#define tShinyOdds data[13]
#define tShinyHunt data[14]
#define tExpShare data[15]

enum
{
    MENUITEM_TEXTSPEED,
    MENUITEM_BATTLESCENE,
    MENUITEM_BATTLESTYLE,
    MENUITEM_SOUND,
    MENUITEM_BUTTONMODE,
    MENUITEM_FRAMETYPE,
    MENUITEM_CANCEL,
    MENUITEM_COUNT,
};

enum
{
    MENUITEM_BATTLESPEED,
    MENUITEM_OVERWORLDSPEED,
    MENUITEM_AUTORUN,
    MENUITEM_MOVEPROMPTS,
    MENUITEM_LEVELCAP,
    MENUITEM_REMATCHES,
    MENUITEM_CANCEL_PG2,
    MENUITEM_COUNT_PG2,
};

enum { MENUITEM_SHINYODDS, MENUITEM_SHINYHUNT, MENUITEM_EXPSHARE, MENUITEM_THEME, MENUITEM_HELP, MENUITEM_RENAME, MENUITEM_CANCEL_PG3, MENUITEM_COUNT_PG3 };

enum
{
    WIN_HEADER,
    WIN_OPTIONS
};


#define YPOS_TEXTSPEED    (MENUITEM_TEXTSPEED * 16)
#define YPOS_BATTLESCENE  (MENUITEM_BATTLESCENE * 16)
#define YPOS_BATTLESTYLE  (MENUITEM_BATTLESTYLE * 16)
#define YPOS_SOUND        (MENUITEM_SOUND * 16)
#define YPOS_BUTTONMODE   (MENUITEM_BUTTONMODE * 16)
#define YPOS_FRAMETYPE    (MENUITEM_FRAMETYPE * 16)

// Page 2
#define YPOS_BATTLESPEED      (MENUITEM_BATTLESPEED * 16)
#define YPOS_OVERWORLDSPEED   (MENUITEM_OVERWORLDSPEED * 16)
#define YPOS_AUTORUN          (MENUITEM_AUTORUN * 16)

#define PAGE_COUNT 7

static void DrawDebugOptions(void);
static void Task_DebugOptions(u8 taskId);
static void CB2_ReturnToFieldDebug(void);

static void Task_OptionMenuFadeIn(u8 taskId);
static void Task_OptionMenuRename(u8 taskId);
static void Task_OptionMenuProcessInput(u8 taskId);
static void Task_OptionMenuFadeIn_Pg2(u8 taskId);
static void Task_OptionMenuProcessInput_Pg2(u8 taskId);
static void Task_OptionMenuSave(u8 taskId);
static void StoreAllCurrentSettings(u8 taskId);
static void Task_OptionMenuProcessInput_Pg3(u8 taskId);
static void DrawOptionsPg3(u8 taskId);
static void DrawOptionsPg4(u8 taskId);
static void DrawBattleOptions(void);
static void Task_BattleOptions(u8 taskId);
static void Task_OptionMenuProcessInput_Pg4(u8 taskId);
static void Task_OptionMenuProcessInput_Help(u8 taskId);
static void DrawHelpPage(void);
static void DrawToggle(u8 disabled, u8 y);
static void ShinyOdds_DrawChoices(u8 selection);
static void Task_OptionMenuFadeOut(u8 taskId);
static void HighlightOptionMenuItem(u8 selection);
static u8 TextSpeed_ProcessInput(u8 selection);
static void TextSpeed_DrawChoices(u8 selection);
static u8 BattleScene_ProcessInput(u8 selection);
static void BattleScene_DrawChoices(u8 selection);
static u8 BattleStyle_ProcessInput(u8 selection);
static void BattleStyle_DrawChoices(u8 selection);
static u8 Sound_ProcessInput(u8 selection);
static void Sound_DrawChoices(u8 selection);
static u8 FrameType_ProcessInput(u8 selection);
static void FrameType_DrawChoices(u8 selection);
static u8 ButtonMode_ProcessInput(u8 selection);
static void ButtonMode_DrawChoices(u8 selection);
static u8   BattleSpeed_ProcessInput(u8 selection);
static void BattleSpeed_DrawChoices(u8 selection);
static u8 OverworldSpeed_ProcessInput(u8 selection);
static void OverworldSpeed_DrawChoices(u8 selection);
static void AutoRun_DrawChoices(u8 selection);
static void DrawHeaderText(void);
static void DrawOptionMenuTexts(void);
static void DrawBgWindowFrames(void);

EWRAM_DATA static bool8 sArrowPressed = FALSE;
EWRAM_DATA static u8 sCurrPage = 0;
EWRAM_DATA static u8 sHelpTopic = 0;

static const u8 gText_Option[]             = _("OPTION");
static const u8 gText_TextSpeedSlow[]      = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}SLOW");
static const u8 gText_TextSpeedMid[]       = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}MID");
static const u8 gText_TextSpeedFast[]      = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}FAST");
static const u8 gText_BattleSceneOn[]      = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}ON");
static const u8 gText_BattleSceneOff[]     = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}OFF");
static const u8 gText_BattleStyleShift[]   = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}SHIFT");
static const u8 gText_BattleStyleSet[]     = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}SET");
static const u8 gText_SoundMono[]          = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}MONO");
static const u8 gText_SoundStereo[]        = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}STEREO");
static const u8 gText_FrameType[]          = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}TYPE");
static const u8 gText_FrameTypeNumber[]    = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}");
static const u8 gText_ButtonTypeNormal[]   = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}NORMAL");
static const u8 gText_ButtonTypeLR[]       = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}LR");
static const u8 gText_ButtonTypeLEqualsA[] = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}L=A");


static const u16 sOptionMenuText_Pal[] = INCGFX_U16("graphics/interface/option_menu_text.pal", ".gbapal");
// note: this is only used in the Japanese release
static const u8 sEqualSignGfx[] = INCGFX_U8("graphics/interface/option_menu_equals_sign.png", ".4bpp");

static const u8 *const sOptionMenuItemsNames[MENUITEM_COUNT] =
{
    [MENUITEM_TEXTSPEED]   = COMPOUND_STRING("TEXT SPEED"),
    [MENUITEM_BATTLESCENE] = COMPOUND_STRING("BATTLE SCENE"),
    [MENUITEM_BATTLESTYLE] = COMPOUND_STRING("BATTLE STYLE"),
    [MENUITEM_SOUND]       = COMPOUND_STRING("SOUND"),
    [MENUITEM_BUTTONMODE]  = COMPOUND_STRING("BUTTON MODE"),
    [MENUITEM_FRAMETYPE]   = COMPOUND_STRING("FRAME"),
    [MENUITEM_CANCEL]      = COMPOUND_STRING("CANCEL"),
};

static const u8 *const sOptionMenuItemsNames_Pg2[MENUITEM_COUNT_PG2] =
{
    [MENUITEM_BATTLESPEED]    = gText_BattleSpeed,
    [MENUITEM_OVERWORLDSPEED] = gText_OverworldSpeed,
    [MENUITEM_AUTORUN]        = COMPOUND_STRING("AUTO RUN"),
    [MENUITEM_MOVEPROMPTS] = COMPOUND_STRING("MOVE PROMPTS"),
    [MENUITEM_LEVELCAP] = COMPOUND_STRING("LEVEL CAPS"),
    [MENUITEM_REMATCHES] = COMPOUND_STRING("REMATCHES"),
    [MENUITEM_CANCEL_PG2]     = gText_OptionMenuCancel,
};

static const u8 *const sOptionMenuItemsNames_Pg3[MENUITEM_COUNT_PG3] =
{
    COMPOUND_STRING("SHINY ODDS"), COMPOUND_STRING("SHINY HUNT"), COMPOUND_STRING("EXP SHARE"), COMPOUND_STRING("APPEARANCE"), COMPOUND_STRING("HELP"), COMPOUND_STRING("CHANGE NAME"), gText_OptionMenuCancel,
};

enum { MENUITEM_DIFFICULTY, MENUITEM_RANDOM_WILD, MENUITEM_RANDOM_TRAINERS, MENUITEM_RANDOM_ITEMS, MENUITEM_RANDOM_IMPORTANT, MENUITEM_RANDOM_ALL, MENUITEM_CANCEL_PG4, MENUITEM_COUNT_PG4 };
static const u8 *const sOptionMenuItemsNames_Pg4[] = {
    COMPOUND_STRING("DIFFICULTY"), COMPOUND_STRING("RANDOM WILD"),
    COMPOUND_STRING("TRAINERS"), COMPOUND_STRING("ITEMS"),
    COMPOUND_STRING("IMPORTANT"), COMPOUND_STRING("EVERYTHING"), gText_OptionMenuCancel,
};

// In-game guide for recent Brisk Emerald features. Keep lines short for the GBA window.
static const struct {
    const u8 *title;
    const u8 *description;
} sHelpTopics[] = {
    {COMPOUND_STRING("AUDIO MUTES"), COMPOUND_STRING("Options 7 has three mutes:\nSOUND: clicks and effects.\nMUSIC: BGM and fanfares.\nCRIES: POKéMON voices only.")},
    {COMPOUND_STRING("GIMMICK GEN"), COMPOUND_STRING("Options 7: GIMMICK GEN ON/OFF.\nON recharges your TERA ORB\nafter each battle. Per-battle\nlimits and required items stay.")},
    {COMPOUND_STRING("OVERWORLD ENCOUNTERS"), COMPOUND_STRING("Options 7: OW ENCOUNTERS.\nOFF removes roaming spawns;\nrandom grass and water fights\nremain. Story encounters stay.")},
    {COMPOUND_STRING("BATTLE FORMAT"), COMPOUND_STRING("Options 5: REGULAR keeps teams.\nSINGLES uses one active per side.\nDOUBLES uses two when available.\nWild doubles add a second wild mon.")},
    {COMPOUND_STRING("BATTLE BAG"), COMPOUND_STRING("Options 5: BATTLE BAG ON/OFF.\nOFF blocks items and ball throws.\nDifficulty rules still apply.\nAdventures always block the Bag.")},
    {COMPOUND_STRING("DISPLAY TYPES"), COMPOUND_STRING("Options 5: DISPLAY TYPES ON/OFF.\nShows known types in battles.\nAdventure battles always hide\nthe floating type icons.")},
    {COMPOUND_STRING("FOLLOWERS"), COMPOUND_STRING("Options 5: FOLLOWERS ON/OFF.\nYour lead returns when switched on.\nStory companions stay available.\nMap restrictions still apply.")},
    {COMPOUND_STRING("2-IN-1 BIKE"), COMPOUND_STRING("Either BIKE has both modes.\nStop, then press L to switch.\nMACH: speed. ACRO: B tricks.\nKeep ACRO mode on rails.")},
    {COMPOUND_STRING("CHANGE NAME"), COMPOUND_STRING("Options 3: CHANGE NAME.\nEdit your name, then select OK.\nYour Trainer ID stays the same;\nyour POKéMON remain yours.")},
    {COMPOUND_STRING("APPEARANCE"), COMPOUND_STRING("Options 3: LIGHT or DARK.\nUse Left/Right or A.\nSlate panels, cyan accents;\nPOKéMON keep their colors.")},
    {COMPOUND_STRING("DIFFICULTY"), COMPOUND_STRING("NORMAL keeps original levels.\nEASY halves opponent levels.\nOrdinary HARD +20%; EXT +40%.\nTrainer levels respect your cap.")},
    {COMPOUND_STRING("EASY RULES"), COMPOUND_STRING("No level caps; prices x1.\nBasic AI; items allowed.\nSHIFT style; shared EXP 200%.\nOver-level freely.")},
    {COMPOUND_STRING("NORMAL RULES"), COMPOUND_STRING("Prices x2; original AI.\nBattle items are allowed.\nChoose SHIFT or SET style.\nShared EXP remains 100%.")},
    {COMPOUND_STRING("HARD RULES"), COMPOUND_STRING("Caps start at 25; end at 100.\nPrices x4; smarter switching.\nNo items; SET; shared EXP 50%.\nUnder-cap catches can grow.")},
    {COMPOUND_STRING("EXTREME RULES"), COMPOUND_STRING("Caps start at 25; EXP stops.\nPrices x10; omniscient AI.\nNo items; SET; no EXP Share.\nWild POKé BALLS still work.")},
    {COMPOUND_STRING("EXP SHARE MODES"), COMPOUND_STRING("Options 3: GEN 3 is held.\nMODERN shares with the team.\nBoth keep full battler EXP.\nDifficulty sets shared EXP.")},
    {COMPOUND_STRING("MODERN EXP SHARE"), COMPOUND_STRING("Devon reward: a Key Item.\nUse it in the Bag for ON/OFF.\nSwitch MODERN on Options 3.\nIt isn't sold in Lilycove.")},
    {COMPOUND_STRING("TRAINER TRAINING"), COMPOUND_STRING("EASY: zero IVs and EVs.\nNORMAL: original training.\nHARD: perfect IVs, old EVs.\nEXTREME: perfect IVs, new EVs.")},
    {COMPOUND_STRING("TRAINER LOADOUTS"), COMPOUND_STRING("EASY: weak attacks, abilities.\nNORMAL: original sets.\nHARD: better legal abilities.\nEXTREME: stronger moves too.")},
    {COMPOUND_STRING("RANDOMIZER"), COMPOUND_STRING("Options 4: randomize wild\nPOKéMON, trainers, items,\nmajor fights or everything.\nWarps are never randomized.")},
    {COMPOUND_STRING("CONTEST RHYTHM"), COMPOUND_STRING("Tap A on the shown beat: 2-4.\nPerfect: +4 hearts. Good: +2.\nMissing keeps normal appeal.\nHigher ranks tighten the timing.")},
    {COMPOUND_STRING("SHINY ODDS"), COMPOUND_STRING("Options 3 sets global odds.\nOptions 5: ISLAND SHINY\ntoggles the Orange Islands\n1/50 wild shiny boost.")},
    {COMPOUND_STRING("DEXNAV"), COMPOUND_STRING("L/R: page land or water.\nSELECT: register. A: search.\nR outside: search again.\nUnlocks with your POKéDEX.")},
    {COMPOUND_STRING("SHINY HUNT"), COMPOUND_STRING("Enable Shiny Hunt on\nOptions page 3 to speed up\nthe encounter-hunting\nprocess.")},
    {COMPOUND_STRING("SPEED CONTROLS"), COMPOUND_STRING("Options 2: battle speed\nranges from 1x to 4x.\nOverworld speed has 1x,\n2x, 4x and 8x settings.")},
    {COMPOUND_STRING("AUTO RUN"), COMPOUND_STRING("Auto Run is on by default.\nNo need to hold B to run.\nTurn it off on Options\npage 2 if you prefer.")},
    {COMPOUND_STRING("MENUS AND SAVING"), COMPOUND_STRING("Hold the D-pad to repeat\nthrough menus more quickly.\nSave messages are faster;\nyour save still gets written.")},
    {COMPOUND_STRING("MOVE PROMPTS"), COMPOUND_STRING("Options 2 lets you skip\nautomatic level-up move\nprompts or turn them\nback on when you want.")},
    {COMPOUND_STRING("LEVEL CAPS"), COMPOUND_STRING("Easy: no cap. Normal: regular.\nHard/Extreme: raised gym caps.\nExtreme stops EXP at its cap.\nSTART shows the current cap.")},
    {COMPOUND_STRING("REMATCHES"), COMPOUND_STRING("Options page 2 controls\nunlimited trainer rematches.\nTurn this on to repeat\ntrainer battles freely.")},
    {COMPOUND_STRING("MOVE RELEARNER"), COMPOUND_STRING("Summary: moves page, START.\nUse L/R for Level, Egg,\nTM and Tutor move lists.\nEmpty lists can be browsed.")},
    {COMPOUND_STRING("TM AND EGG MOVES"), COMPOUND_STRING("Egg moves come from the\nPOKéMON's egg move list.\nTM moves need the matching\nTM in your Bag.")},
    {COMPOUND_STRING("MULTI-USE CANDY"), COMPOUND_STRING("Use several EXP Candies\nin one action to level up.\nLevel-up evolution checks\nstill occur during use.")},
    {COMPOUND_STRING("BALL CAPSULES"), COMPOUND_STRING("Party: ITEM, BALL CAPSULE.\nUp/Down picks a seal slot.\nLeft/Right picks shape/letter.\nA saves; B discards edits.")},
    {COMPOUND_STRING("SEND-OUT TEXT"), COMPOUND_STRING("Party: ITEM, SEND-OUT TEXT.\nPresets or 3 shared phrases.\nCustom: START opens keyboard.\n15 letters; A saves choice.")},
    {COMPOUND_STRING("OVERWORLD BALLS"), COMPOUND_STRING("Overworld POKé BALLS only\ncatch at or below your cap.\nRoutes that are too strong\nshow a warning instead.")},
    {COMPOUND_STRING("CATCH CHANCES"), COMPOUND_STRING("During battle, catch-rate\ninformation appears near\nthe POKé BALL selection\nto help choose a ball.")},
    {COMPOUND_STRING("REPELS"), COMPOUND_STRING("Repels now prevent wild\nencounters at all levels,\nregardless of your lead\nPOKéMON's level.")},
    {COMPOUND_STRING("CUSTOM MEGAS"), COMPOUND_STRING("New custom Mega forms use\nnative 80x80 battle art.\nTheir Mega Stones are sold\nin the expanded shops.")},
    {COMPOUND_STRING("STARTER FIXES"), COMPOUND_STRING("Starter natures, gender,\nIVs and shiny chances\nare rolled normally.\nNo fixed Hardy/zero IVs.")},
    {COMPOUND_STRING("ORANGE ISLANDS"), COMPOUND_STRING("Explore Routes 143-150\nand ORANGE CITY. Its mart\nsells custom MEGA STONES.\nThe lookout has a cave.")},
    {COMPOUND_STRING("CHAMPION ROUTE"), COMPOUND_STRING("Route 135 is guarded\nuntil you have entered\nthe Hall of Fame.\nChampion Archives await.")},
    {COMPOUND_STRING("ENCOUNTER RATES"), COMPOUND_STRING("Wild encounters in caves\nand on water have been\nrebalanced; regular land\ntables are unchanged.")},
    {COMPOUND_STRING("WONDER TRADE"), COMPOUND_STRING("Use the in-game Wonder\nTrade to exchange a\nPOKéMON for a random\neligible trade partner.")},
};
static const u8 sHelpControls[] = _("UP/DOWN: TOPIC  A: NEXT");
static const u8 sHelpExit[] = _("B: BACK    L/R: PAGES");

static const struct WindowTemplate sOptionMenuWinTemplates[] =
{
    [WIN_HEADER] = {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 1,
        .width = 26,
        .height = 2,
        .paletteNum = 1,
        .baseBlock = 2
    },
    [WIN_OPTIONS] = {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 5,
        .width = 26,
        .height = 14,
        .paletteNum = 1,
        .baseBlock = 0x36
    },
    DUMMY_WIN_TEMPLATE
};

static const struct BgTemplate sOptionMenuBgTemplates[] =
{
    {
        .bg = 1,
        .charBaseIndex = 1,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 0,
        .charBaseIndex = 1,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0
    }
};

static const u16 sOptionMenuBg_Pal[] = {RGB(17, 18, 31)};
static const u16 sOptionMenuDarkBg_Pal[] = {RGB(2, 3, 5)};

static void LoadOptionTheme(u8 frame)
{
    SetGpuReg(REG_OFFSET_BLDY, BriskDarkMode() ? 2 : 4);
    LoadPalette(BriskDarkMode() ? sOptionMenuDarkBg_Pal : sOptionMenuBg_Pal, BG_PLTT_ID(0), sizeof(sOptionMenuBg_Pal));
    LoadPalette(GetWindowFrameTilesPal(frame)->pal, BG_PLTT_ID(7), PLTT_SIZE_4BPP);
    LoadBriskMenuPaletteStyled(sOptionMenuText_Pal, BG_PLTT_ID(1), sizeof(sOptionMenuText_Pal), BRISK_PALETTE_OPTIONS);
}


static void MainCB2(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void ReadAllCurrentSettings(u8 taskId)
{
    gTasks[taskId].tMenuSelection = 0;
    gTasks[taskId].tTextSpeed = gSaveBlock2Ptr->optionsTextSpeed;
    gTasks[taskId].tBattleSceneOff = gSaveBlock2Ptr->optionsBattleSceneOff;
    gTasks[taskId].tBattleStyle = gSaveBlock2Ptr->optionsBattleStyle;
    gTasks[taskId].tSound = gSaveBlock2Ptr->optionsSound;
    gTasks[taskId].tButtonMode = gSaveBlock2Ptr->optionsButtonMode;
    gTasks[taskId].tWindowFrameType = gSaveBlock2Ptr->optionsWindowFrameType;
    gTasks[taskId].tBattleSpeed = gSaveBlock2Ptr->optionsBattleSpeed;
    gTasks[taskId].tOverworldSpeed = VarGet(VAR_OVERWORLD_SPEEDUP);
    gTasks[taskId].tAutoRunDisabled = VarGet(VAR_AUTO_RUN_DISABLED) != 0;
    gTasks[taskId].tSkipMoves = BriskSkipMovePrompts();
    gTasks[taskId].tNoLevelCap = VarGet(VAR_BRISK_NO_LEVEL_CAP) != 0;
    gTasks[taskId].tRematches = BriskInfiniteRematches();
    u32 odds = VarGet(VAR_BRISK_SHINY_ODDS);
    gTasks[taskId].tShinyOdds = odds >= 1 && odds <= BRISK_SHINY_CHOICES ? odds : 5;
    gTasks[taskId].tShinyHunt = BriskShinyHunting();
    gTasks[taskId].tExpShare = BriskModernExpShare();
}

static void DrawOptionsPg1(u8 taskId)
{  
    ReadAllCurrentSettings(taskId);
    TextSpeed_DrawChoices(gTasks[taskId].tTextSpeed);
    BattleScene_DrawChoices(gTasks[taskId].tBattleSceneOff);
    BattleStyle_DrawChoices(gTasks[taskId].tBattleStyle);
    Sound_DrawChoices(gTasks[taskId].tSound);
    ButtonMode_DrawChoices(gTasks[taskId].tButtonMode);
    FrameType_DrawChoices(gTasks[taskId].tWindowFrameType);
    HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_FULL);
}

static void DrawOptionsPg2(u8 taskId)
{
    ReadAllCurrentSettings(taskId);
    BattleSpeed_DrawChoices(gTasks[taskId].tBattleSpeed);
    OverworldSpeed_DrawChoices(gTasks[taskId].tOverworldSpeed);
    AutoRun_DrawChoices(gTasks[taskId].tAutoRunDisabled);
    DrawToggle(gTasks[taskId].tSkipMoves, MENUITEM_MOVEPROMPTS * 16);
    DrawToggle(GetBriskDifficulty() == BRISK_MODE_EASY ? TRUE : GetBriskDifficulty() == BRISK_MODE_EXTREME ? FALSE : gTasks[taskId].tNoLevelCap, MENUITEM_LEVELCAP * 16);
    DrawToggle(!gTasks[taskId].tRematches, MENUITEM_REMATCHES * 16);
    HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_FULL);
}

static u8 Process_ChangePage(u8 CurrentPage)
{
    if (JOY_NEW(R_BUTTON))
    {
        if (CurrentPage < PAGE_COUNT - 1)
            CurrentPage++;
        else
            CurrentPage = 0;
    }
    if (JOY_NEW(L_BUTTON))
    {
        if (CurrentPage != 0)
            CurrentPage--;
        else
            CurrentPage = PAGE_COUNT - 1;
    }
    return CurrentPage;
}

static void Task_ChangePage(u8 taskId)
{
    FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(1));
    DrawHeaderText();
    PutWindowTilemap(1);
    DrawOptionMenuTexts();
    switch(sCurrPage)
    {
    case 0:
        DrawOptionsPg1(taskId);
        gTasks[taskId].func = Task_OptionMenuFadeIn;
        break;
    case 1:
        DrawOptionsPg2(taskId);
        gTasks[taskId].func = Task_OptionMenuFadeIn_Pg2;
        break;
    case 2:
        DrawOptionsPg3(taskId);
        gTasks[taskId].func = Task_OptionMenuProcessInput_Pg3;
        break;
    case 3:
        DrawOptionsPg4(taskId);
        gTasks[taskId].func = Task_OptionMenuProcessInput_Pg4;
        break;
    case 4:
        gTasks[taskId].tMenuSelection = 0;
        DrawBattleOptions();
        HighlightOptionMenuItem(0);
        gTasks[taskId].func = Task_BattleOptions;
        break;
    case 5:
        DrawHelpPage();
        gTasks[taskId].func = Task_OptionMenuProcessInput_Help;
        break;
    case 6:
        gTasks[taskId].tMenuSelection = 0;
        DrawDebugOptions();
        HighlightOptionMenuItem(0);
        gTasks[taskId].func = Task_DebugOptions;
        break;
    }
}

void CB2_InitOptionMenu(void)
{
    u8 taskId;
    switch (gMain.state)
    {
    default:
    case 0:
        SetVBlankCallback(NULL);
        gMain.state++;
        break;
    case 1:
        DmaClearLarge16(3, (void *)(VRAM), VRAM_SIZE, 0x1000);
        DmaClear32(3, OAM, OAM_SIZE);
        DmaClear16(3, PLTT, PLTT_SIZE);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sOptionMenuBgTemplates, ARRAY_COUNT(sOptionMenuBgTemplates));
        ChangeBgX(0, 0, BG_COORD_SET);
        ChangeBgY(0, 0, BG_COORD_SET);
        ChangeBgX(1, 0, BG_COORD_SET);
        ChangeBgY(1, 0, BG_COORD_SET);
        ChangeBgX(2, 0, BG_COORD_SET);
        ChangeBgY(2, 0, BG_COORD_SET);
        ChangeBgX(3, 0, BG_COORD_SET);
        ChangeBgY(3, 0, BG_COORD_SET);
        InitWindows(sOptionMenuWinTemplates);
        DeactivateAllTextPrinters();
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG0);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_CLR);
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_DARKEN);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, BriskDarkMode() ? 2 : 4);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
        ShowBg(0);
        ShowBg(1);
        gMain.state++;
        break;
    case 2:
        ResetPaletteFade();
        ScanlineEffect_Stop();
        ResetTasks();
        ResetSpriteData();
        gMain.state++;
        break;
    case 3:
        LoadBgTiles(1, GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->tiles, 0x120, 0x1A2);
        gMain.state++;
        break;
    case 4:
        LoadPalette(BriskDarkMode() ? sOptionMenuDarkBg_Pal : sOptionMenuBg_Pal, BG_PLTT_ID(0), sizeof(sOptionMenuBg_Pal));
        LoadPalette(GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->pal, BG_PLTT_ID(7), PLTT_SIZE_4BPP);
        gMain.state++;
        break;
    case 5:
        LoadBriskMenuPaletteStyled(sOptionMenuText_Pal, BG_PLTT_ID(1), sizeof(sOptionMenuText_Pal), BRISK_PALETTE_OPTIONS);
        gMain.state++;
        break;
    case 6:
        PutWindowTilemap(WIN_HEADER);
        DrawHeaderText();
        gMain.state++;
        break;
    case 7:
        gMain.state++;
        break;
    case 8:
        PutWindowTilemap(WIN_OPTIONS);
        DrawOptionMenuTexts();
        gMain.state++;
    case 9:
        DrawBgWindowFrames();
        gMain.state++;
        break;
    case 10:
    {
                switch(sCurrPage)
        {
        case 0:
            taskId = CreateTask(Task_OptionMenuFadeIn, 0);
            DrawOptionsPg1(taskId);
            break;
        case 1:
            taskId = CreateTask(Task_OptionMenuFadeIn_Pg2, 0);
            DrawOptionsPg2(taskId);
            break;
        case 2:
            taskId = CreateTask(Task_OptionMenuProcessInput_Pg3, 0);
            DrawOptionsPg3(taskId);
            break;
        case 3:
            taskId = CreateTask(Task_OptionMenuProcessInput_Pg4, 0);
            DrawOptionsPg4(taskId);
            break;
        case 4:
            taskId = CreateTask(Task_BattleOptions, 0);
            ReadAllCurrentSettings(taskId);
            DrawBattleOptions();
            HighlightOptionMenuItem(0);
            break;
        case 5:
            taskId = CreateTask(Task_OptionMenuProcessInput_Help, 0);
            DrawHelpPage();
            break;
        case 6:
            taskId = CreateTask(Task_DebugOptions, 0);
            ReadAllCurrentSettings(taskId);
            DrawDebugOptions();
            HighlightOptionMenuItem(0);
            break;
        }
        gMain.state++;
        break;
    }
    case 11:
        BeginNormalPaletteFade(PALETTES_ALL, MENU_FADE_DELAY, 16, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB);
        SetMainCallback2(MainCB2);
        return;
    }
}

static void Task_OptionMenuFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_OptionMenuProcessInput;
}

static void Task_OptionMenuProcessInput(u8 taskId)
{
    if (JOY_NEW(L_BUTTON) || JOY_NEW(R_BUTTON))
    {
        FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(1));
        ClearStdWindowAndFrame(WIN_OPTIONS, FALSE);
        StoreAllCurrentSettings(taskId);
        sCurrPage = Process_ChangePage(sCurrPage);
        gTasks[taskId].func = Task_ChangePage;
    }
    else if (JOY_NEW(A_BUTTON))
    {
        if (gTasks[taskId].tMenuSelection == MENUITEM_CANCEL)
            gTasks[taskId].func = Task_OptionMenuSave;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        gTasks[taskId].func = Task_OptionMenuSave;
    }
    else if (JOY_REPEAT(DPAD_UP))
    {
        if (gTasks[taskId].tMenuSelection > 0)
            gTasks[taskId].tMenuSelection--;
        else
            gTasks[taskId].tMenuSelection = MENUITEM_CANCEL;
        HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
    }
    else if (JOY_REPEAT(DPAD_DOWN))
    {
        if (gTasks[taskId].tMenuSelection < MENUITEM_CANCEL)
            gTasks[taskId].tMenuSelection++;
        else
            gTasks[taskId].tMenuSelection = 0;
        HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
    }
    else
    {
        u8 previousOption;

        switch (gTasks[taskId].tMenuSelection)
        {
        case MENUITEM_TEXTSPEED:
            previousOption = gTasks[taskId].tTextSpeed;
            gTasks[taskId].tTextSpeed = TextSpeed_ProcessInput(gTasks[taskId].tTextSpeed);

            if (previousOption != gTasks[taskId].tTextSpeed)
                TextSpeed_DrawChoices(gTasks[taskId].tTextSpeed);
            break;
        case MENUITEM_BATTLESCENE:
            previousOption = gTasks[taskId].tBattleSceneOff;
            gTasks[taskId].tBattleSceneOff = BattleScene_ProcessInput(gTasks[taskId].tBattleSceneOff);

            if (previousOption != gTasks[taskId].tBattleSceneOff)
                BattleScene_DrawChoices(gTasks[taskId].tBattleSceneOff);
            break;
        case MENUITEM_BATTLESTYLE:
            previousOption = gTasks[taskId].tBattleStyle;
            gTasks[taskId].tBattleStyle = BattleStyle_ProcessInput(gTasks[taskId].tBattleStyle);

            if (previousOption != gTasks[taskId].tBattleStyle)
                BattleStyle_DrawChoices(gTasks[taskId].tBattleStyle);
            break;
        case MENUITEM_SOUND:
            previousOption = gTasks[taskId].tSound;
            gTasks[taskId].tSound = Sound_ProcessInput(gTasks[taskId].tSound);

            if (previousOption != gTasks[taskId].tSound)
                Sound_DrawChoices(gTasks[taskId].tSound);
            break;
        case MENUITEM_BUTTONMODE:
            previousOption = gTasks[taskId].tButtonMode;
            gTasks[taskId].tButtonMode = ButtonMode_ProcessInput(gTasks[taskId].tButtonMode);

            if (previousOption != gTasks[taskId].tButtonMode)
                ButtonMode_DrawChoices(gTasks[taskId].tButtonMode);
            break;
        case MENUITEM_FRAMETYPE:
            previousOption = gTasks[taskId].tWindowFrameType;
            gTasks[taskId].tWindowFrameType = FrameType_ProcessInput(gTasks[taskId].tWindowFrameType);

            if (previousOption != gTasks[taskId].tWindowFrameType)
                FrameType_DrawChoices(gTasks[taskId].tWindowFrameType);
            break;
        default:
            return;
        }

        if (sArrowPressed)
        {
            sArrowPressed = FALSE;
            CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);
        }
    }
}

static void Task_OptionMenuFadeIn_Pg2(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_OptionMenuProcessInput_Pg2;
}

static void Task_OptionMenuProcessInput_Pg2(u8 taskId)
{
    if (JOY_NEW(L_BUTTON) || JOY_NEW(R_BUTTON))
    {
        FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(1));
        ClearStdWindowAndFrame(WIN_OPTIONS, FALSE);
        StoreAllCurrentSettings(taskId);
        sCurrPage = Process_ChangePage(sCurrPage);
        gTasks[taskId].func = Task_ChangePage;
    }
    else if (JOY_NEW(A_BUTTON))
    {
        if (gTasks[taskId].tMenuSelection == MENUITEM_CANCEL_PG2)
            gTasks[taskId].func = Task_OptionMenuSave;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        gTasks[taskId].func = Task_OptionMenuSave;
    }
    else if (JOY_REPEAT(DPAD_UP))
    {
        if (gTasks[taskId].tMenuSelection > 0)
            gTasks[taskId].tMenuSelection--;
        else
            gTasks[taskId].tMenuSelection = MENUITEM_CANCEL_PG2;
        HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
    }
    else if (JOY_REPEAT(DPAD_DOWN))
    {
        if (gTasks[taskId].tMenuSelection < MENUITEM_CANCEL_PG2)
            gTasks[taskId].tMenuSelection++;
        else
            gTasks[taskId].tMenuSelection = 0;
        HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
    }
    else
    {
        u8 previousOption;

        switch (gTasks[taskId].tMenuSelection)
        {
        case MENUITEM_BATTLESPEED:
            previousOption = gTasks[taskId].tBattleSpeed;
            gTasks[taskId].tBattleSpeed = BattleSpeed_ProcessInput(gTasks[taskId].tBattleSpeed);

            if (previousOption != gTasks[taskId].tBattleSpeed)
                BattleSpeed_DrawChoices(gTasks[taskId].tBattleSpeed);
            break;   
        case MENUITEM_OVERWORLDSPEED:
            previousOption = gTasks[taskId].tOverworldSpeed;
            gTasks[taskId].tOverworldSpeed = OverworldSpeed_ProcessInput(gTasks[taskId].tOverworldSpeed);

            if (previousOption != gTasks[taskId].tOverworldSpeed)
                OverworldSpeed_DrawChoices(gTasks[taskId].tOverworldSpeed);
            break;
        case MENUITEM_AUTORUN:
            previousOption = gTasks[taskId].tAutoRunDisabled;
            gTasks[taskId].tAutoRunDisabled = BattleScene_ProcessInput(previousOption);
            if (previousOption != gTasks[taskId].tAutoRunDisabled)
                AutoRun_DrawChoices(gTasks[taskId].tAutoRunDisabled);
            break;
        case MENUITEM_MOVEPROMPTS:
            gTasks[taskId].tSkipMoves = BattleScene_ProcessInput(gTasks[taskId].tSkipMoves);
            DrawToggle(gTasks[taskId].tSkipMoves, MENUITEM_MOVEPROMPTS * 16);
            break;
        case MENUITEM_LEVELCAP:
            if (GetBriskDifficulty() == BRISK_MODE_EASY || GetBriskDifficulty() == BRISK_MODE_EXTREME)
                break;
            gTasks[taskId].tNoLevelCap = BattleScene_ProcessInput(gTasks[taskId].tNoLevelCap);
            DrawToggle(gTasks[taskId].tNoLevelCap, MENUITEM_LEVELCAP * 16);
            break;
        case MENUITEM_REMATCHES:
            gTasks[taskId].tRematches = BattleScene_ProcessInput(gTasks[taskId].tRematches);
            DrawToggle(!gTasks[taskId].tRematches, MENUITEM_REMATCHES * 16);
            break;
        default:
            return;
        }
        if (sArrowPressed)
        {
            sArrowPressed = FALSE;
            CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);
        }
    }
}

static void StoreAllCurrentSettings(u8 taskId)
{
    gSaveBlock2Ptr->optionsTextSpeed = gTasks[taskId].tTextSpeed;
    gSaveBlock2Ptr->optionsBattleSceneOff = gTasks[taskId].tBattleSceneOff;
    gSaveBlock2Ptr->optionsBattleStyle = gTasks[taskId].tBattleStyle;
    gSaveBlock2Ptr->optionsSound = gTasks[taskId].tSound;
    gSaveBlock2Ptr->optionsButtonMode = gTasks[taskId].tButtonMode;
    gSaveBlock2Ptr->optionsWindowFrameType = gTasks[taskId].tWindowFrameType;
    gSaveBlock2Ptr->optionsBattleSpeed = gTasks[taskId].tBattleSpeed;
    VarSet(VAR_OVERWORLD_SPEEDUP, gTasks[taskId].tOverworldSpeed);
    VarSet(VAR_AUTO_RUN_DISABLED, gTasks[taskId].tAutoRunDisabled);

    VarSet(VAR_BRISK_SKIP_MOVE_PROMPTS, gTasks[taskId].tSkipMoves);
    VarSet(VAR_BRISK_NO_LEVEL_CAP, gTasks[taskId].tNoLevelCap);
    VarSet(VAR_BRISK_INFINITE_REMATCHES, gTasks[taskId].tRematches);
    VarSet(VAR_BRISK_SHINY_ODDS, gTasks[taskId].tShinyOdds);
    VarSet(VAR_BRISK_SHINY_HUNT, gTasks[taskId].tShinyHunt);
    SetBriskExpShareMode(gTasks[taskId].tExpShare);
}

static void Task_OptionMenuSave(u8 taskId)
{
    StoreAllCurrentSettings(taskId);
    for (u32 i = 0; i < gPartiesCount[B_TRAINER_PLAYER]; i++)
        CalculateMonStats(&gParties[B_TRAINER_PLAYER][i]);
    BeginNormalPaletteFade(PALETTES_ALL, MENU_FADE_DELAY, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_OptionMenuFadeOut;
}

static void Task_OptionMenuFadeOut(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        DestroyTask(taskId);
        FreeAllWindowBuffers();
        SetMainCallback2(gMain.savedCallback);
    }
}

static void Task_OpenOptionsDebug(u8 taskId)
{
    if (gPaletteFade.active || !IsWeatherNotFadingIn())
        return;
    DestroyTask(taskId);
    Debug_ShowMainMenu();
}

static bool8 FieldCB_OptionsDebug(void)
{
    // Rebuild the field before creating debug windows or executing its actions.
    LockPlayerFieldControls();
    FreezeObjectEvents();
    FadeInFromBlack();
    CreateTask(Task_OpenOptionsDebug, 10);
    return TRUE;
}

static void CB2_ReturnToFieldDebug(void)
{
    gFieldCallback2 = FieldCB_OptionsDebug;
    CB2_ReturnToField();
}

static void DrawDebugOptions(void)
{
    FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(1));
    AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, COMPOUND_STRING("DEBUG MENU"), 8, 1, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, COMPOUND_STRING("OW ENCOUNTERS"), 8, 17, TEXT_SKIP_DRAW, NULL);
    DrawToggle(!BriskOverworldEncountersEnabled(), 16);
    AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, COMPOUND_STRING("GIMMICK GEN"), 8, 33, TEXT_SKIP_DRAW, NULL);
    DrawToggle(!BriskGimmickGenEnabled(), 32);
    AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, COMPOUND_STRING("MUTE SOUND"), 8, 49, TEXT_SKIP_DRAW, NULL);
    DrawToggle(!BriskSoundMuted(), 48);
    AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, COMPOUND_STRING("MUTE MUSIC"), 8, 65, TEXT_SKIP_DRAW, NULL);
    DrawToggle(!BriskMusicMuted(), 64);
    AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, COMPOUND_STRING("MUTE CRIES"), 8, 81, TEXT_SKIP_DRAW, NULL);
    DrawToggle(!BriskCriesMuted(), 80);
    AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, gText_OptionMenuCancel, 8, 97, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_FULL);
}

static void Task_DebugOptions(u8 taskId)
{
    if (gPaletteFade.active)
        return;
    if (JOY_NEW(L_BUTTON | R_BUTTON))
    {
        StoreAllCurrentSettings(taskId);
        sCurrPage = Process_ChangePage(sCurrPage);
        gTasks[taskId].func = Task_ChangePage;
    }
    else if (JOY_NEW(B_BUTTON) || (JOY_NEW(A_BUTTON) && gTasks[taskId].tMenuSelection == 6))
        gTasks[taskId].func = Task_OptionMenuSave;
    else if (JOY_REPEAT(DPAD_UP | DPAD_DOWN))
    {
        gTasks[taskId].tMenuSelection = (gTasks[taskId].tMenuSelection + (JOY_REPEAT(DPAD_DOWN) ? 1 : 6)) % 7;
        HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
    }
    else if (gTasks[taskId].tMenuSelection == 1 && (JOY_NEW(A_BUTTON) || JOY_REPEAT(DPAD_LEFT | DPAD_RIGHT)))
    {
        VarSet(VAR_BRISK_OW_ENCOUNTERS_DISABLED, BriskOverworldEncountersEnabled());
        DrawDebugOptions();
    }
    else if (gTasks[taskId].tMenuSelection == 2 && (JOY_NEW(A_BUTTON) || JOY_REPEAT(DPAD_LEFT | DPAD_RIGHT)))
    {
        FlagToggle(FLAG_BRISK_GIMMICK_GEN);
        BriskRechargeGimmicks(); // Also make an already-spent Orb usable on enabling.
        DrawDebugOptions();
    }
    else if (gTasks[taskId].tMenuSelection >= 3 && gTasks[taskId].tMenuSelection <= 5
          && (JOY_NEW(A_BUTTON) || JOY_REPEAT(DPAD_LEFT | DPAD_RIGHT)))
    {
        static const u16 flags[] = {FLAG_BRISK_MUTE_SOUND, FLAG_BRISK_MUTE_MUSIC, FLAG_BRISK_MUTE_CRIES};
        FlagToggle(flags[gTasks[taskId].tMenuSelection - 3]);
        RefreshBriskAudioMute();
        DrawDebugOptions();
    }
    else if (JOY_NEW(A_BUTTON) && gTasks[taskId].tMenuSelection == 0)
    {
        // Use the usual settings-save and fade-out path before opening debug.
        gMain.savedCallback = CB2_ReturnToFieldDebug;
        gTasks[taskId].func = Task_OptionMenuSave;
    }
}

static void HighlightOptionMenuItem(u8 index)
{
    SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(16, DISPLAY_WIDTH - 16));
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(index * 16 + 40, index * 16 + 56));
}

static void DrawOptionMenuChoice(const u8 *text, u8 x, u8 y, u8 style)
{
    u8 dst[16];
    u16 i;

    for (i = 0; *text != EOS && i < ARRAY_COUNT(dst) - 1; i++)
        dst[i] = *(text++);

    if (BriskDarkMode())
    {
        dst[2] = style ? TEXT_COLOR_RED : TEXT_COLOR_GREEN;
        dst[5] = style ? TEXT_COLOR_LIGHT_RED : TEXT_COLOR_LIGHT_GREEN;
    }
    else if (style != 0)
    {
        dst[2] = TEXT_COLOR_RED;
        dst[5] = TEXT_COLOR_LIGHT_RED;
    }

    dst[i] = EOS;
    AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, dst, x, y + 1, TEXT_SKIP_DRAW, NULL);
}

static u8 BattleSpeed_ProcessInput(u8 selection)
{
    if (JOY_REPEAT(DPAD_RIGHT))
    {
        if (++selection > 3)  // If the selection exceeds 3, wrap around to 0
            selection = 0;
            sArrowPressed = TRUE;
    }
    if (JOY_REPEAT(DPAD_LEFT))
    {
        if (--selection > 3)  // If the selection is negative, wrap around to 3
            selection = 3;
            sArrowPressed = TRUE;
    }
    // Update the battle speed variable based on the selection
    VarSet(VAR_BATTLE_SPEED, selection);  // Set the battle speed variable
    return selection;
}

static u8 OverworldSpeed_ProcessInput(u8 selection)
{
    if (JOY_REPEAT(DPAD_RIGHT))
    {
        if (++selection > 3)
            selection = 0;

        sArrowPressed = TRUE;
    }

    if (JOY_REPEAT(DPAD_LEFT))
    {
        if (--selection > 3)
            selection = 3;

        sArrowPressed = TRUE;
    }

    VarSet(VAR_OVERWORLD_SPEEDUP, selection);
    return selection;
}

static void OverworldSpeed_DrawChoices(u8 selection)
{
    u8 styles[4];

    styles[0] = 0;
    styles[1] = 0;
    styles[2] = 0;
    styles[3] = 0;
    styles[selection] = 1;

    int xSpacer;
    xSpacer = (GetStringRightAlignXOffset(1, gText_OverworldSpeed8x, 198) - 104) / 3;

    DrawOptionMenuChoice(gText_OverworldSpeed1x, 104, YPOS_OVERWORLDSPEED, styles[0]);
    DrawOptionMenuChoice(gText_OverworldSpeed2x, 104 + xSpacer, YPOS_OVERWORLDSPEED, styles[1]);
    DrawOptionMenuChoice(gText_OverworldSpeed4x, 104 + 2 * xSpacer, YPOS_OVERWORLDSPEED, styles[2]);
    DrawOptionMenuChoice(gText_OverworldSpeed8x, GetStringRightAlignXOffset(1, gText_OverworldSpeed8x, 198), YPOS_OVERWORLDSPEED, styles[3]);
}


static void BattleSpeed_DrawChoices(u8 selection)
{
    u8 styles[4];
    styles[0] = 0;
    styles[1] = 0;
    styles[2] = 0;
    styles[3] = 0;
    styles[selection] = 1;  // Highlight the selected option
    int xSpacer;
    xSpacer = (GetStringRightAlignXOffset(1, gText_BattleSpeed4x, 198) - 104) / 3;

    // Draw each menu choice at the calculated positions
    DrawOptionMenuChoice(gText_BattleSpeed1x, 104, YPOS_BATTLESPEED, styles[0]);
    DrawOptionMenuChoice(gText_BattleSpeed2x, 104 + xSpacer, YPOS_BATTLESPEED, styles[1]);
    DrawOptionMenuChoice(gText_BattleSpeed3x, 104 + 2 * xSpacer, YPOS_BATTLESPEED, styles[2]);
    DrawOptionMenuChoice(gText_BattleSpeed4x, GetStringRightAlignXOffset(1, gText_BattleSpeed4x, 198), YPOS_BATTLESPEED, styles[3]);
}


static u8 TextSpeed_ProcessInput(u8 selection)
{
    if (JOY_REPEAT(DPAD_RIGHT))
    {
        if (selection <= 1)
            selection++;
        else
            selection = 0;

        sArrowPressed = TRUE;
    }
    if (JOY_REPEAT(DPAD_LEFT))
    {
        if (selection != 0)
            selection--;
        else
            selection = 2;

        sArrowPressed = TRUE;
    }
    return selection;
}

static void TextSpeed_DrawChoices(u8 selection)
{
    u8 styles[3];
    s32 widthSlow, widthMid, widthFast, xMid;

    styles[0] = 0;
    styles[1] = 0;
    styles[2] = 0;
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_TextSpeedSlow, 104, YPOS_TEXTSPEED, styles[0]);

    widthSlow = GetStringWidth(FONT_NORMAL, gText_TextSpeedSlow, 0);
    widthMid = GetStringWidth(FONT_NORMAL, gText_TextSpeedMid, 0);
    widthFast = GetStringWidth(FONT_NORMAL, gText_TextSpeedFast, 0);

    widthMid -= 94;
    xMid = (widthSlow - widthMid - widthFast) / 2 + 104;
    DrawOptionMenuChoice(gText_TextSpeedMid, xMid, YPOS_TEXTSPEED, styles[1]);

    DrawOptionMenuChoice(gText_TextSpeedFast, GetStringRightAlignXOffset(FONT_NORMAL, gText_TextSpeedFast, 198), YPOS_TEXTSPEED, styles[2]);
}

static u8 BattleScene_ProcessInput(u8 selection)
{
    if (JOY_REPEAT(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void BattleScene_DrawChoices(u8 selection)
{
    u8 styles[2];

    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_BattleSceneOn, 104, YPOS_BATTLESCENE, styles[0]);
    DrawOptionMenuChoice(gText_BattleSceneOff, GetStringRightAlignXOffset(FONT_NORMAL, gText_BattleSceneOff, 198), YPOS_BATTLESCENE, styles[1]);
}

static u8 BattleStyle_ProcessInput(u8 selection)
{
    if (GetBriskDifficulty() != BRISK_MODE_NORMAL)
        return selection;
    if (JOY_REPEAT(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void BattleStyle_DrawChoices(u8 selection)
{
    u8 styles[2];

    selection = GetBriskBattleStyle(selection);
    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_BattleStyleShift, 104, YPOS_BATTLESTYLE, styles[0]);
    DrawOptionMenuChoice(gText_BattleStyleSet, GetStringRightAlignXOffset(FONT_NORMAL, gText_BattleStyleSet, 198), YPOS_BATTLESTYLE, styles[1]);
}

static u8 Sound_ProcessInput(u8 selection)
{
    if (JOY_REPEAT(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        SetPokemonCryStereo(selection);
        sArrowPressed = TRUE;
    }

    return selection;
}

static void Sound_DrawChoices(u8 selection)
{
    u8 styles[2];

    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_SoundMono, 104, YPOS_SOUND, styles[0]);
    DrawOptionMenuChoice(gText_SoundStereo, GetStringRightAlignXOffset(FONT_NORMAL, gText_SoundStereo, 198), YPOS_SOUND, styles[1]);
}

static u8 FrameType_ProcessInput(u8 selection)
{
    if (JOY_REPEAT(DPAD_RIGHT))
    {
        if (selection < WINDOW_FRAMES_COUNT - 1)
            selection++;
        else
            selection = 0;

        LoadBgTiles(1, GetWindowFrameTilesPal(selection)->tiles, 0x120, 0x1A2);
        LoadPalette(GetWindowFrameTilesPal(selection)->pal, BG_PLTT_ID(7), PLTT_SIZE_4BPP);
        sArrowPressed = TRUE;
    }
    if (JOY_REPEAT(DPAD_LEFT))
    {
        if (selection != 0)
            selection--;
        else
            selection = WINDOW_FRAMES_COUNT - 1;

        LoadBgTiles(1, GetWindowFrameTilesPal(selection)->tiles, 0x120, 0x1A2);
        LoadPalette(GetWindowFrameTilesPal(selection)->pal, BG_PLTT_ID(7), PLTT_SIZE_4BPP);
        sArrowPressed = TRUE;
    }
    return selection;
}

static void FrameType_DrawChoices(u8 selection)
{
    u8 text[16] = {EOS};
    u8 n = selection + 1;
    u16 i;

    for (i = 0; gText_FrameTypeNumber[i] != EOS && i <= 5; i++)
        text[i] = gText_FrameTypeNumber[i];

    // Convert a number to decimal string
    if (n / 10 != 0)
    {
        text[i] = n / 10 + CHAR_0;
        i++;
        text[i] = n % 10 + CHAR_0;
        i++;
    }
    else
    {
        text[i] = n % 10 + CHAR_0;
        i++;
        text[i] = CHAR_SPACER;
        i++;
    }

    text[i] = EOS;

    DrawOptionMenuChoice(gText_FrameType, 104, YPOS_FRAMETYPE, 0);
    DrawOptionMenuChoice(text, 128, YPOS_FRAMETYPE, 1);
}

static u8 ButtonMode_ProcessInput(u8 selection)
{
    if (JOY_REPEAT(DPAD_RIGHT))
    {
        if (selection <= 1)
            selection++;
        else
            selection = 0;

        sArrowPressed = TRUE;
    }
    if (JOY_REPEAT(DPAD_LEFT))
    {
        if (selection != 0)
            selection--;
        else
            selection = 2;

        sArrowPressed = TRUE;
    }
    return selection;
}

static void ButtonMode_DrawChoices(u8 selection)
{
    s32 widthNormal, widthLR, widthLA, xLR;
    u8 styles[3];

    styles[0] = 0;
    styles[1] = 0;
    styles[2] = 0;
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_ButtonTypeNormal, 104, YPOS_BUTTONMODE, styles[0]);

    widthNormal = GetStringWidth(FONT_NORMAL, gText_ButtonTypeNormal, 0);
    widthLR = GetStringWidth(FONT_NORMAL, gText_ButtonTypeLR, 0);
    widthLA = GetStringWidth(FONT_NORMAL, gText_ButtonTypeLEqualsA, 0);

    widthLR -= 94;
    xLR = (widthNormal - widthLR - widthLA) / 2 + 104;
    DrawOptionMenuChoice(gText_ButtonTypeLR, xLR, YPOS_BUTTONMODE, styles[1]);

    DrawOptionMenuChoice(gText_ButtonTypeLEqualsA, GetStringRightAlignXOffset(FONT_NORMAL, gText_ButtonTypeLEqualsA, 198), YPOS_BUTTONMODE, styles[2]);
}
static void DrawHeaderText(void)
{
    u32 i, widthOptions, xMid;
    u8 pageDots[16] = _("");  // Array size should be at least (2 * PAGE_COUNT) -1
    widthOptions = GetStringWidth(FONT_NORMAL, gText_Option, 0);

    for (i = 0; i < PAGE_COUNT; i++)
    {
        if (i == sCurrPage)
            StringAppend(pageDots, gText_LargeDot);
        else
            StringAppend(pageDots, gText_SmallDot);
        if (i < PAGE_COUNT - 1)
            StringAppend(pageDots, gText_Space);            
    }
    xMid = (8 + widthOptions + 5);
    FillWindowPixelBuffer(WIN_HEADER, PIXEL_FILL(1));
    AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_Option, 8, 1, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, pageDots, xMid, 1, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_PageNav, GetStringRightAlignXOffset(FONT_NORMAL, gText_PageNav, 198), 1, TEXT_SKIP_DRAW, NULL);  
    CopyWindowToVram(WIN_HEADER, COPYWIN_FULL);
}

static void DrawOptionMenuTexts(void)
{
    u8 i, items;
    items = 0;
    const u8* const* menu = NULL;

    switch (sCurrPage){
    case 0:
        items = MENUITEM_COUNT;
        menu = sOptionMenuItemsNames;
        break;
    case 1:
        items = MENUITEM_COUNT_PG2;
        menu = sOptionMenuItemsNames_Pg2;
        break;
    case 2:
        items = MENUITEM_COUNT_PG3;
        menu = sOptionMenuItemsNames_Pg3;
        break;
    case 3:
        items = MENUITEM_COUNT_PG4;
        menu = sOptionMenuItemsNames_Pg4;
        break;    
    }  

    FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(1));
    for (i = 0; i < items; i++)
        AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, menu[i], 8, (i * 16) + 1, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_FULL);
}

#define TILE_TOP_CORNER_L 0x1A2
#define TILE_TOP_EDGE     0x1A3
#define TILE_TOP_CORNER_R 0x1A4
#define TILE_LEFT_EDGE    0x1A5
#define TILE_RIGHT_EDGE   0x1A7
#define TILE_BOT_CORNER_L 0x1A8
#define TILE_BOT_EDGE     0x1A9
#define TILE_BOT_CORNER_R 0x1AA

static void DrawBgWindowFrames(void)
{
    //                     bg, tile,              x, y, width, height, palNum
    // Draw title window frame
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  1,  0,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      2,  0, 27,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 28,  0,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     1,  1,  1,  2,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   28,  1,  1,  2,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1,  3,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2,  3, 27,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28,  3,  1,  1,  7);

    // Draw options list window frame
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  1,  4,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      2,  4, 26,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 28,  4,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     1,  5,  1, 18,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   28,  5,  1, 18,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1, 19,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2, 19, 26,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28, 19,  1,  1,  7);

    CopyBgTilemapBufferToVram(1);
}

static void AutoRun_DrawChoices(u8 selection)
{
    DrawOptionMenuChoice(gText_BattleSceneOn, 104, YPOS_AUTORUN, selection == 0);
    DrawOptionMenuChoice(gText_BattleSceneOff, 160, YPOS_AUTORUN, selection != 0);
}

static void DrawToggle(u8 disabled, u8 y)
{
    DrawOptionMenuChoice(gText_BattleSceneOn, 104, y, disabled == 0);
    DrawOptionMenuChoice(gText_BattleSceneOff, 160, y, disabled != 0);
}

static void ShinyOdds_DrawChoices(u8 selection)
{
    FillWindowPixelRect(WIN_OPTIONS, PIXEL_FILL(1), 104, 0, 104, 16);
    StringCopy(gStringVar1, COMPOUND_STRING("{COLOR GREEN}{SHADOW LIGHT_GREEN}1/"));
    ConvertIntToDecimalStringN(gStringVar2, GetBriskShinyDenominatorForSetting(selection), STR_CONV_MODE_LEFT_ALIGN, 4);
    StringAppend(gStringVar1, gStringVar2);
    DrawOptionMenuChoice(gStringVar1, 104, 0, 1);
}

static void ExpShare_DrawChoices(u8 modern)
{
    DrawOptionMenuChoice(COMPOUND_STRING("{COLOR GREEN}{SHADOW LIGHT_GREEN}GEN 3"), 104, MENUITEM_EXPSHARE * 16, !modern);
    DrawOptionMenuChoice(COMPOUND_STRING("{COLOR GREEN}{SHADOW LIGHT_GREEN}MODERN"), 155, MENUITEM_EXPSHARE * 16, modern);
}

static void Theme_DrawChoices(void)
{
    u8 y = MENUITEM_THEME * 16;
    DrawOptionMenuChoice(COMPOUND_STRING("{COLOR GREEN}{SHADOW LIGHT_GREEN}LIGHT"), 104, y, !BriskDarkMode());
    DrawOptionMenuChoice(COMPOUND_STRING("{COLOR GREEN}{SHADOW LIGHT_GREEN}DARK"), 158, y, BriskDarkMode());
}

static void ToggleTheme(u8 taskId)
{
    VarSet(VAR_BRISK_DARK_MODE, !BriskDarkMode());
    LoadOptionTheme(gTasks[taskId].tWindowFrameType);
    Theme_DrawChoices();
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);
}

static void DrawOptionsPg3(u8 taskId)
{
    ReadAllCurrentSettings(taskId);
    ShinyOdds_DrawChoices(gTasks[taskId].tShinyOdds);
    DrawToggle(!gTasks[taskId].tShinyHunt, 16);
    ExpShare_DrawChoices(gTasks[taskId].tExpShare);
    Theme_DrawChoices();
    HighlightOptionMenuItem(0);
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_FULL);
}

static void Task_OptionMenuProcessInput_Pg3(u8 taskId)
{
    if (gPaletteFade.active)
        return;
    if (JOY_NEW(L_BUTTON | R_BUTTON))
    {
        StoreAllCurrentSettings(taskId);
        sCurrPage = Process_ChangePage(sCurrPage);
        gTasks[taskId].func = Task_ChangePage;
    }
    else if (JOY_NEW(B_BUTTON) || (JOY_NEW(A_BUTTON) && gTasks[taskId].tMenuSelection == MENUITEM_CANCEL_PG3))
        gTasks[taskId].func = Task_OptionMenuSave;
    else if (JOY_NEW(A_BUTTON) && gTasks[taskId].tMenuSelection == MENUITEM_RENAME)
    {
        StoreAllCurrentSettings(taskId);
        BeginNormalPaletteFade(PALETTES_ALL, MENU_FADE_DELAY, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_OptionMenuRename;
    }
    else if (JOY_NEW(A_BUTTON) && gTasks[taskId].tMenuSelection == MENUITEM_THEME)
        ToggleTheme(taskId);
    else if (JOY_NEW(A_BUTTON) && gTasks[taskId].tMenuSelection == MENUITEM_HELP)
    {
        StoreAllCurrentSettings(taskId);
        sHelpTopic = 0;
        sCurrPage = 5;
        gTasks[taskId].func = Task_ChangePage;
    }
    else if (JOY_REPEAT(DPAD_UP | DPAD_DOWN))
    {
        gTasks[taskId].tMenuSelection = (gTasks[taskId].tMenuSelection + (JOY_REPEAT(DPAD_DOWN) ? 1 : MENUITEM_COUNT_PG3 - 1)) % MENUITEM_COUNT_PG3;
        HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
    }
    else if (JOY_REPEAT(DPAD_LEFT | DPAD_RIGHT))
    {
        if (gTasks[taskId].tMenuSelection == MENUITEM_SHINYODDS)
        {
            u32 value = gTasks[taskId].tShinyOdds - 1;
            value = (value + (JOY_REPEAT(DPAD_RIGHT) ? 1 : BRISK_SHINY_CHOICES - 1)) % BRISK_SHINY_CHOICES;
            gTasks[taskId].tShinyOdds = value + 1;
            ShinyOdds_DrawChoices(value + 1);
        }
        else if (gTasks[taskId].tMenuSelection == MENUITEM_SHINYHUNT)
        {
            gTasks[taskId].tShinyHunt ^= 1;
            DrawToggle(!gTasks[taskId].tShinyHunt, 16);
        }
        else if (gTasks[taskId].tMenuSelection == MENUITEM_EXPSHARE)
        {
            gTasks[taskId].tExpShare ^= 1;
            ExpShare_DrawChoices(gTasks[taskId].tExpShare);
        }
        else if (gTasks[taskId].tMenuSelection == MENUITEM_THEME)
            ToggleTheme(taskId);
        CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);
    }
}


static void DrawModesChoices(void)
{
    static const u8 *const modes[] = {COMPOUND_STRING("{COLOR GREEN}{SHADOW LIGHT_GREEN}NORMAL"), COMPOUND_STRING("{COLOR GREEN}{SHADOW LIGHT_GREEN}EASY"), COMPOUND_STRING("{COLOR GREEN}{SHADOW LIGHT_GREEN}HARD"), COMPOUND_STRING("{COLOR GREEN}{SHADOW LIGHT_GREEN}EXTREME")};
    FillWindowPixelRect(WIN_OPTIONS, PIXEL_FILL(1), 104, 0, 104, 16);
    DrawOptionMenuChoice(modes[GetBriskDifficulty()], 104, 0, TRUE);
    u32 settings = VarGet(VAR_BRISK_RANDOMIZER);
    for (u32 row = MENUITEM_RANDOM_WILD; row <= MENUITEM_RANDOM_ALL; row++)
        DrawToggle(!(settings & (1 << (row - MENUITEM_RANDOM_WILD))), row * 16);
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);
}

static void DrawOptionsPg4(u8 taskId)
{
    ReadAllCurrentSettings(taskId);
    DrawModesChoices();
    HighlightOptionMenuItem(0);
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_FULL);
}

static void Task_OptionMenuProcessInput_Pg4(u8 taskId)
{
    if (gPaletteFade.active)
        return;
    if (JOY_NEW(L_BUTTON | R_BUTTON))
    {
        StoreAllCurrentSettings(taskId);
        sCurrPage = Process_ChangePage(sCurrPage);
        gTasks[taskId].func = Task_ChangePage;
    }
    else if (JOY_NEW(B_BUTTON) || (JOY_NEW(A_BUTTON) && gTasks[taskId].tMenuSelection == MENUITEM_CANCEL_PG4))
        gTasks[taskId].func = Task_OptionMenuSave;
    else if (JOY_REPEAT(DPAD_UP | DPAD_DOWN))
    {
        gTasks[taskId].tMenuSelection = (gTasks[taskId].tMenuSelection + (JOY_REPEAT(DPAD_DOWN) ? 1 : MENUITEM_COUNT_PG4 - 1)) % MENUITEM_COUNT_PG4;
        HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
    }
    else if (JOY_REPEAT(DPAD_LEFT | DPAD_RIGHT))
    {
        u32 row = gTasks[taskId].tMenuSelection;
        if (row == MENUITEM_DIFFICULTY)
            VarSet(VAR_BRISK_DIFFICULTY, (GetBriskDifficulty() + (JOY_REPEAT(DPAD_RIGHT) ? 1 : 3)) % 4);
        else if (row >= MENUITEM_RANDOM_WILD && row <= MENUITEM_RANDOM_ALL)
            VarSet(VAR_BRISK_RANDOMIZER, VarGet(VAR_BRISK_RANDOMIZER) ^ (1 << (row - MENUITEM_RANDOM_WILD)));
        DrawModesChoices();
    }
}


static void DrawHelpPage(void)
{
    FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(1));
    // No option row is highlighted while reading a help topic.
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(0, 0));
    AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, sHelpTopics[sHelpTopic].title, 8, 2, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, sHelpTopics[sHelpTopic].description, 8, 20, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(WIN_OPTIONS, FONT_SMALL, sHelpControls, 8, 88, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(WIN_OPTIONS, FONT_SMALL, sHelpExit, 8, 100, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_FULL);
}

static void Task_OptionMenuRename(u8 taskId)
{
    if (gPaletteFade.active)
        return;
    FreeAllWindowBuffers();
    DestroyTask(taskId);
    OpenPlayerNameEditor(CB2_InitOptionMenu);
}

static void Task_OptionMenuProcessInput_Help(u8 taskId)
{
    if (gPaletteFade.active)
        return;

    if (JOY_NEW(L_BUTTON | R_BUTTON))
    {
        StoreAllCurrentSettings(taskId);
        sCurrPage = Process_ChangePage(sCurrPage);
        gTasks[taskId].func = Task_ChangePage;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        sCurrPage = 2; // Return to the Options page containing HELP.
        gTasks[taskId].func = Task_ChangePage;
    }
    else if (JOY_REPEAT(DPAD_UP | DPAD_DOWN) || JOY_NEW(A_BUTTON))
    {
        if (JOY_REPEAT(DPAD_UP))
            sHelpTopic = sHelpTopic == 0 ? ARRAY_COUNT(sHelpTopics) - 1 : sHelpTopic - 1;
        else
            sHelpTopic = (sHelpTopic + 1) % ARRAY_COUNT(sHelpTopics);
        DrawHelpPage();
    }
}

#if TESTING
u8 Test_OpenBriskWorldOptions(void)
{
    ResetTasks();
    InitBgsFromTemplates(0, sOptionMenuBgTemplates, ARRAY_COUNT(sOptionMenuBgTemplates));
    InitWindows(sOptionMenuWinTemplates);
    DeactivateAllTextPrinters();
    sCurrPage = 6;
    u8 taskId = CreateTask(Task_ChangePage, 0);
    ReadAllCurrentSettings(taskId);
    Task_ChangePage(taskId);
    gPaletteFade.active = FALSE;
    return taskId;
}

u8 Test_OpenBriskThemeOption(void)
{
    ResetTasks();
    InitBgsFromTemplates(0, sOptionMenuBgTemplates, ARRAY_COUNT(sOptionMenuBgTemplates));
    InitWindows(sOptionMenuWinTemplates);
    DeactivateAllTextPrinters();
    sCurrPage = 2;
    u8 taskId = CreateTask(Task_OptionMenuProcessInput_Pg3, 0);
    DrawOptionMenuTexts();
    DrawOptionsPg3(taskId);
    gTasks[taskId].tMenuSelection = MENUITEM_THEME;
    HighlightOptionMenuItem(MENUITEM_THEME);
    LoadOptionTheme(gTasks[taskId].tWindowFrameType);
    gPaletteFade.active = FALSE;
    return taskId;
}

void Test_CloseBriskThemeOption(u8 taskId)
{
    DestroyTask(taskId);
    FreeAllWindowBuffers();
    sCurrPage = 0;
}
#endif

// New settings use persistent variables, like difficulty/theme, since the task's
// sixteen data slots are already occupied by the original option pages.
static void DrawBattleOptions(void)
{
    static const u8 *const choices[] = {
        COMPOUND_STRING("{COLOR GREEN}{SHADOW LIGHT_GREEN}Regular"),
        COMPOUND_STRING("{COLOR GREEN}{SHADOW LIGHT_GREEN}Singles"),
        COMPOUND_STRING("{COLOR GREEN}{SHADOW LIGHT_GREEN}Doubles"),
    };
    FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(1));
    AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, COMPOUND_STRING("FORMAT"), 8, 1, TEXT_SKIP_DRAW, NULL);
    for (u32 i = 0; i < ARRAY_COUNT(choices); i++)
        DrawOptionMenuChoice(choices[i], 65 + i * 45, 0, GetBriskBattleFormat() == i);
    AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, COMPOUND_STRING("FOLLOWERS"), 8, 17, TEXT_SKIP_DRAW, NULL);
    DrawToggle(!BriskPokemonFollowersEnabled(), 16);
    AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, COMPOUND_STRING("ISLAND SHINY"), 8, 33, TEXT_SKIP_DRAW, NULL);
    DrawToggle(!BriskIslandShiniesEnabled(), 32);
    AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, COMPOUND_STRING("DISPLAY TYPES"), 8, 49, TEXT_SKIP_DRAW, NULL);
    DrawToggle(!BriskDisplayTypes(), 48);
    AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, COMPOUND_STRING("BATTLE BAG"), 8, 65, TEXT_SKIP_DRAW, NULL);
    DrawToggle(VarGet(VAR_BRISK_BATTLE_BAG_DISABLED) != 0, 64);
    AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, COMPOUND_STRING("TALK ONLY"), 8, 81, TEXT_SKIP_DRAW, NULL);
    DrawToggle(!FlagGet(OW_FLAG_NO_TRAINER_SEE), 80);
    AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, gText_OptionMenuCancel, 8, 97, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_FULL);
}

static void Task_BattleOptions(u8 taskId)
{
    if (gPaletteFade.active) return;
    u32 row = gTasks[taskId].tMenuSelection;
    if (JOY_NEW(L_BUTTON | R_BUTTON))
    {
        StoreAllCurrentSettings(taskId);
        sCurrPage = Process_ChangePage(sCurrPage);
        gTasks[taskId].func = Task_ChangePage;
    }
    else if (JOY_NEW(B_BUTTON) || (JOY_NEW(A_BUTTON) && row == 6))
        gTasks[taskId].func = Task_OptionMenuSave;
    else if (JOY_REPEAT(DPAD_UP | DPAD_DOWN))
    {
        gTasks[taskId].tMenuSelection = (row + (JOY_REPEAT(DPAD_DOWN) ? 1 : 6)) % 7;
        HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
    }
    else if (JOY_REPEAT(DPAD_LEFT | DPAD_RIGHT) || JOY_NEW(A_BUTTON))
    {
        if (row == 0)
            VarSet(VAR_BRISK_BATTLE_FORMAT, (GetBriskBattleFormat() + (JOY_REPEAT(DPAD_LEFT) ? 2 : 1)) % 3);
        else if (row == 1)
            VarSet(VAR_BRISK_FOLLOWERS_DISABLED, BriskPokemonFollowersEnabled());
        else if (row == 2)
            VarSet(VAR_BRISK_ISLAND_SHINIES_DISABLED, BriskIslandShiniesEnabled());
        else if (row == 3)
            VarSet(VAR_BRISK_TYPES_DISABLED, BriskDisplayTypes());
        else if (row == 4)
            VarSet(VAR_BRISK_BATTLE_BAG_DISABLED, VarGet(VAR_BRISK_BATTLE_BAG_DISABLED) == 0);
        else if (row == 5)
            FlagToggle(OW_FLAG_NO_TRAINER_SEE);
        DrawBattleOptions();
    }
}

#if TESTING
u8 Test_OpenBriskBattleOptions(void)
{
    ResetTasks();
    InitBgsFromTemplates(0, sOptionMenuBgTemplates, ARRAY_COUNT(sOptionMenuBgTemplates));
    InitWindows(sOptionMenuWinTemplates);
    DeactivateAllTextPrinters();
    sCurrPage = 4;
    u8 taskId = CreateTask(Task_ChangePage, 0);
    ReadAllCurrentSettings(taskId);
    Task_ChangePage(taskId);
    gPaletteFade.active = FALSE;
    return taskId;
}
#endif
