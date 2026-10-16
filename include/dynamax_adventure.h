#ifndef GUARD_DYNAMAX_ADVENTURE_H
#define GUARD_DYNAMAX_ADVENTURE_H

#include "pokemon.h"
#include "constants/dynamax_adventure.h"

struct DynamaxAdventureStatus
{
    u8 phase;
    u8 stage; // 0..3; 4 after defeating the final boss.
    u8 lives;
    u8 caughtCount;
    bool8 cleared;
    u8 rentalsChosen;
};

u32 DynamaxAdventure_GetBossHPMultiplier(void);
u32 DynamaxAdventure_GetBossDamageMultiplier(void);
const u8 *DynamaxAdventure_GetPartnerName(u32 slot);
void DynamaxAdventure_SwitchPartner(enum BattlerId battler, u32 newSlot);
u32 DynamaxAdventure_ScaleMaxHP(struct Pokemon *mon, u32 hp);
u16 DynamaxAdventure_GetRivalPartnerId(void);
u32 DynamaxAdventure_GetRentalPoolCount(void);
enum Species DynamaxAdventure_GetRentalPoolSpecies(u32 index);
u32 DynamaxAdventure_GetBossPoolCount(void);
enum Species DynamaxAdventure_GetBossPoolSpecies(u32 index);
bool32 DynamaxAdventure_IsBossSpecies(enum Species species);
bool32 DynamaxAdventure_Begin(u32 seed);
struct DynamaxAdventureStatus DynamaxAdventure_GetStatus(void);
struct Pokemon *DynamaxAdventure_GetRental(u32 choice);
struct Pokemon *DynamaxAdventure_GetPath(u32 choice);
struct Pokemon *DynamaxAdventure_GetCaught(u32 choice);
bool32 DynamaxAdventure_ChooseRental(u32 choice);
bool32 DynamaxAdventure_ChoosePath(u32 choice);
bool32 DynamaxAdventure_PrepareBattle(void);
bool32 DynamaxAdventure_FinishBattle(u32 outcome);
bool32 DynamaxAdventure_Catch(bool32 accept);
bool32 DynamaxAdventure_Swap(bool32 accept);
bool32 DynamaxAdventure_SwapRental(u32 slot);
bool32 DynamaxAdventure_Claim(u32 choice);
bool32 DynamaxAdventure_Quit(void);
bool32 IsDynamaxAdventureBattle(void);
void RunDynamaxAdventure(void);

#if TESTING
bool32 Test_DynamaxAdventure_CreateBoss(struct Pokemon *mon, enum Species species);
#endif

#endif
