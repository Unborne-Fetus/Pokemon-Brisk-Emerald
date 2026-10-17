#include "global.h"
#include "battle.h"
#include "battle_controllers.h"
#include "dynamax_adventure.h"
#include "bg.h"
#include "string_util.h"
#include "text.h"
#include "menu.h"
#include "window.h"

// Stored as ID + 1: zero remains valid before the first adventure.
static EWRAM_DATA u8 sRaidHudWindow = 0;
static EWRAM_DATA u32 sRaidHudHash = 0;
static EWRAM_DATA u8 sRaidHudTop = 0;
static const u8 sAllySlots[] = {0, 2, 3, 4};
static const u8 sOwnerLetters[] = _("PRSB");

void DynamaxAdventure_ResetRaidHud(void)
{
    sRaidHudWindow = 0;
}

void DynamaxAdventure_InitRaidHud(void)
{
    const struct WindowTemplate template = {
        .bg = 0, .tilemapLeft = 0, .tilemapTop = 0,
        .width = 30, .height = 3, .paletteNum = 0, .baseBlock = 0x200,
    };
    u8 window = AddWindow(&template);
    if (window == WINDOW_NONE)
        return;
    sRaidHudWindow = window + 1;
    sRaidHudHash = 0;
    sRaidHudTop = 0;
    DynamaxAdventure_UpdateRaidHud();
}

void DynamaxAdventure_UpdateRaidHud(void)
{
    if (!IsAdventureRaid() || sRaidHudWindow == 0 || !IsDynamaxAdventureBattle() || gBattlersCount != 5)
        return;
    u8 window = sRaidHudWindow - 1;
    u32 hash = 2166136261u;
    u8 top = (GetBgY(0) >> 11) & 63;
    bool32 selecting = gBattlerControllerFuncs[0] == HandleInputChooseTarget;
    for (u32 i = 0; i < ARRAY_COUNT(sAllySlots); i++)
    {
        struct BattlePokemon *mon = &gBattleMons[sAllySlots[i]];
        hash = (hash ^ mon->hp) * 16777619;
        hash = (hash ^ mon->maxHP) * 16777619;
        hash = (hash ^ mon->species) * 16777619;
        hash = (hash ^ mon->status1) * 16777619;
    }
    hash ^= top;
    if (selecting)
        hash ^= (gMultiUsePlayerCursor + 1) << 16;
    if (sRaidHudHash == hash)
        return;
    sRaidHudHash = hash;
    if (top != sRaidHudTop)
    {
        ClearWindowTilemap(window);
        SetWindowAttribute(window, WINDOW_TILEMAP_TOP, top);
        sRaidHudTop = top;
    }
    FillWindowPixelBuffer(window, PIXEL_FILL(15));
    for (u32 i = 0; i < ARRAY_COUNT(sAllySlots); i++)
    {
        enum BattlerId battler = sAllySlots[i];
        struct BattlePokemon *mon = &gBattleMons[battler];
        u8 name[POKEMON_NAME_LENGTH + 4];
        u8 nickname[POKEMON_NAME_LENGTH + 1];
        u8 hp[12];
        const u8 colors[] = {15, 1, 6};
        u32 x = i * 60 + 2;
        name[0] = sOwnerLetters[i];
        name[1] = CHAR_COLON;
        GetMonData(GetBattlerMon(battler), MON_DATA_NICKNAME, nickname);
        StringCopyN(name + 2, nickname, 5);
        name[7] = EOS;
        AddTextPrinterParameterized4(window, FONT_SMALL, x, 0, 0, 0, colors, TEXT_SKIP_DRAW, name);
        u8 *end = ConvertIntToDecimalStringN(hp, mon->hp, STR_CONV_MODE_LEFT_ALIGN, 4);
        *end++ = CHAR_SLASH;
        ConvertIntToDecimalStringN(end, mon->maxHP, STR_CONV_MODE_LEFT_ALIGN, 4);
        AddTextPrinterParameterized4(window, FONT_SMALL, x, 12, 0, 0, colors, TEXT_SKIP_DRAW, hp);
        const u8 *status = NULL;
        if (mon->hp == 0) status = COMPOUND_STRING("FNT");
        else if (mon->status1 & STATUS1_SLEEP) status = COMPOUND_STRING("SLP");
        else if (mon->status1 & (STATUS1_POISON | STATUS1_TOXIC_POISON)) status = COMPOUND_STRING("PSN");
        else if (mon->status1 & STATUS1_BURN) status = COMPOUND_STRING("BRN");
        else if (mon->status1 & STATUS1_FREEZE) status = COMPOUND_STRING("FRZ");
        else if (mon->status1 & STATUS1_PARALYSIS) status = COMPOUND_STRING("PAR");
        if (status != NULL)
            AddTextPrinterParameterized4(window, FONT_SMALL, x + 40, 0, 0, 0, colors, TEXT_SKIP_DRAW, status);
        FillWindowPixelRect(window, 6, x, 22, 54, 2);
        if (mon->hp && mon->maxHP)
            FillWindowPixelRect(window, 1, x, 22, max(1, 54 * mon->hp / mon->maxHP), 2);
        if (selecting && gMultiUsePlayerCursor == battler)
        {
            FillWindowPixelRect(window, 1, x, 0, 54, 1);
            FillWindowPixelRect(window, 1, x, 21, 54, 1);
        }
    }
    PutWindowTilemap(window);
    CopyWindowToVram(window, COPYWIN_FULL);
}
