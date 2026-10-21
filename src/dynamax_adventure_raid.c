#include "global.h"
#include "battle.h"
#include "battle_interface.h"
#include "battle_controllers.h"
#include "battle_util.h"
#include "bg.h"
#include "dynamax_adventure.h"
#include "gpu_regs.h"
#include "menu.h"
#include "palette.h"
#include "sprite.h"
#include "string_util.h"
#include "text.h"
#include "window.h"
#include "constants/rgb.h"

// One boss and two allies use the ordinary four-slot battle engine. The top
// bar uses a small, nonoverlapping window, rather than additional battlers.
static EWRAM_DATA u8 sBossWindow = 0;
static EWRAM_DATA u8 sDamageBattler = 0;
static EWRAM_DATA u8 sDamageFrames = 0;
static EWRAM_DATA u16 sBossHP = 0;
static EWRAM_DATA u16 sBossSpecies = SPECIES_NONE;
static const u16 sBossPalette[16] = {RGB(3, 4, 7), RGB(3, 4, 7), RGB_WHITE, RGB(11, 13, 16), RGB(6, 26, 14), RGB(31, 22, 4), RGB(30, 6, 7)};
static const struct WindowTemplate sBossTemplate = {
    .bg = 0, .tilemapLeft = 1, .tilemapTop = 0, .width = 28,
    .height = 2, .paletteNum = 7, .baseBlock = 0x50,
};

bool32 DynamaxAdventure_IsEmptyBossSlot(enum BattlerId battler)
{
    return IsDynamaxAdventureBattle() && battler < gBattlersCount
        && GetBattlerPosition(battler) == B_POSITION_OPPONENT_RIGHT;
}

void DynamaxAdventure_KeepBossSlotEmpty(void)
{
    if (!IsDynamaxAdventureBattle())
        return;
    for (enum BattlerId battler = 0; battler < gBattlersCount; battler++)
    {
        if (!DynamaxAdventure_IsEmptyBossSlot(battler))
            continue;
        // Side-wide effects must never turn this reserved position into a
        // second boss by reusing the opponent's party index.
        gAbsentBattlerFlags |= 1u << battler;
        memset(&gBattleMons[battler], 0, sizeof(gBattleMons[battler]));
        gBattleStruct->battlerState[battler].notOnField = TRUE;
    }
}

void DynamaxAdventure_ResetRaidHud(void)
{
    sBossWindow = WINDOW_NONE;
    sDamageBattler = MAX_BATTLERS_COUNT;
    sDamageFrames = 0;
    sBossHP = 0xFFFF;
    sBossSpecies = SPECIES_NONE;
}

bool32 DynamaxAdventure_IsDamageHealthboxVisible(enum BattlerId battler)
{
    return sDamageFrames != 0 && sDamageBattler == battler;
}

void DynamaxAdventure_ShowDamageHealthbox(enum BattlerId battler)
{
    if (!IsDynamaxAdventureBattle() || !IsOnPlayerSide(battler))
        return;
    sDamageBattler = battler;
    sDamageFrames = 90;
    SetHealthboxSpriteVisible(gHealthboxSpriteIds[battler]);
}

u32 DynamaxAdventure_GetDisplayedBossHP(void)
{
    enum BattlerId boss = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
    const struct BattleBarInfo *bar = &gBattleSpritesDataPtr->battleBars[boss];
    if (gBattlerControllerFuncs[boss] == Controller_WaitForHealthBar
     && bar->currValue >= 0 && bar->maxValue == gBattleMons[boss].maxHP)
        return min(bar->maxValue, bar->maxValue < 48 ? Q_24_8_TO_INT(bar->currValue) : bar->currValue);
    return gBattleMons[boss].hp;
}

static void DrawBossBar(void)
{
    enum BattlerId boss = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
    u32 hp = DynamaxAdventure_GetDisplayedBossHP(), maxHP = gBattleMons[boss].maxHP;
    if (sBossWindow == WINDOW_NONE || maxHP == 0)
        return;
    if (hp != sBossHP || gBattleMons[boss].species != sBossSpecies)
    {
        u8 name[POKEMON_NAME_LENGTH + 1];
        const u8 colors[] = {1, 2, 3};
        sBossHP = hp;
        sBossSpecies = gBattleMons[boss].species;
        FillWindowPixelBuffer(sBossWindow, PIXEL_FILL(1));
        StringCopy(name, gBattleMons[boss].nickname);
        AddTextPrinterParameterized3(sBossWindow, FONT_SMALL, 2, 0, colors, TEXT_SKIP_DRAW, name);
        AddTextPrinterParameterized3(sBossWindow, FONT_SMALL, 176, 0, colors, TEXT_SKIP_DRAW, COMPOUND_STRING("BOSS"));
        FillWindowPixelRect(sBossWindow, 3, 2, 12, 220, 4);
        FillWindowPixelRect(sBossWindow, hp * 4 > maxHP ? (hp * 2 > maxHP ? 4 : 5) : 6, 2, 12, 220 * hp / maxHP, 4);
        LoadPalette(sBossPalette, 7 * 16, sizeof(sBossPalette));
        CopyWindowToVram(sBossWindow, COPYWIN_GFX);
    }
    // Battle menus switch BG0 between three vertical pages. Repeat the bar
    // on each page so it stays at the top when choosing actions or moves.
    for (u32 page = 0; page < 3; page++)
    {
        gWindows[sBossWindow].window.tilemapTop = page * 20;
        PutWindowTilemap(sBossWindow);
    }
    gWindows[sBossWindow].window.tilemapTop = 0;
    CopyBgTilemapBufferToVram(0);
}

void DynamaxAdventure_InitRaidHud(void)
{
    DynamaxAdventure_ResetRaidHud();
    sBossWindow = AddWindow(&sBossTemplate);
    DrawBossBar();
}

void DynamaxAdventure_UpdateRaidHud(void)
{
    if (!IsDynamaxAdventureBattle() || sBossWindow == WINDOW_NONE)
        return;
    if (sDamageFrames)
        --sDamageFrames;
    for (u32 battler = 0; battler < gBattlersCount; battler++)
    {
        if (IsOnPlayerSide(battler) && IsBattlerAlive(battler) && DynamaxAdventure_IsDamageHealthboxVisible(battler))
            SetHealthboxSpriteVisible(gHealthboxSpriteIds[battler]);
        else
            SetHealthboxSpriteInvisible(gHealthboxSpriteIds[battler]);
    }
    DrawBossBar();
}
