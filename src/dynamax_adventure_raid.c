#include "global.h"
#include "battle.h"
#include "battle_interface.h"
#include "dynamax_adventure.h"
#include "sprite.h"

// No background windows: the normal battle menus retain all of BG0.
// Only the current damage recipient gets a temporary stock healthbox.
static EWRAM_DATA u8 sDamageBattler = 0;
static EWRAM_DATA u8 sDamageFrames = 0;

void DynamaxAdventure_ResetRaidHud(void)
{
    sDamageBattler = MAX_BATTLERS_COUNT;
    sDamageFrames = 0;
}

void DynamaxAdventure_InitRaidHud(void)
{
    DynamaxAdventure_ResetRaidHud();
    for (u32 battler = 0; battler < gBattlersCount; battler++)
        SetHealthboxSpriteInvisible(gHealthboxSpriteIds[battler]);
}

bool32 DynamaxAdventure_IsDamageHealthboxVisible(enum BattlerId battler)
{
    return sDamageFrames != 0 && sDamageBattler == battler;
}

void DynamaxAdventure_ShowDamageHealthbox(enum BattlerId battler)
{
    if (!IsAdventureRaid())
        return;
    if (sDamageBattler < gBattlersCount && sDamageBattler != battler)
        SetHealthboxSpriteInvisible(gHealthboxSpriteIds[sDamageBattler]);
    sDamageBattler = battler;
    sDamageFrames = 60;
    SetHealthboxSpriteVisible(gHealthboxSpriteIds[battler]);
}

void DynamaxAdventure_UpdateRaidHud(void)
{
    if (!IsAdventureRaid() || sDamageFrames == 0)
        return;
    if (--sDamageFrames == 0 && sDamageBattler < gBattlersCount)
        SetHealthboxSpriteInvisible(gHealthboxSpriteIds[sDamageBattler]);
}
