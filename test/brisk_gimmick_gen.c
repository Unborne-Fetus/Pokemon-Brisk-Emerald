#include "global.h"
#include "battle.h"
#include "battle_gimmick.h"
#include "battle_util2.h"
#include "brisk_options.h"
#include "event_data.h"
#include "item.h"
#include "main.h"
#include "task.h"
#include "test/test.h"

u8 Test_OpenBriskWorldOptions(void);
void Test_CloseBriskThemeOption(u8 taskId);

static void RestoreFlag(u16 flag, bool32 enabled)
{
    if (enabled) FlagSet(flag); else FlagClear(flag);
}

TEST("Brisk GIMMICK GEN toggles in Options 7, persists, and charges an existing Orb immediately")
{
    bool32 savedOption = BriskGimmickGenEnabled();
    bool32 savedCharge = FlagGet(B_FLAG_TERA_ORB_CHARGED);
    u16 oldOrbs = CountTotalItemQuantityInBag(ITEM_TERA_ORB);
    if (!oldOrbs) EXPECT(AddBagItem(ITEM_TERA_ORB, 1));
    FlagClear(FLAG_BRISK_GIMMICK_GEN);
    FlagClear(B_FLAG_TERA_ORB_CHARGED);
    u8 task = Test_OpenBriskWorldOptions();
    EXPECT(!BriskGimmickGenEnabled());
    gMain.newKeys = 0;
    gMain.newAndRepeatedKeys = DPAD_DOWN;
    RunTasks(); RunTasks();
    gMain.newAndRepeatedKeys = 0;
    gMain.newKeys = A_BUTTON;
    RunTasks();
    EXPECT(BriskGimmickGenEnabled());
    EXPECT(FlagGet(B_FLAG_TERA_ORB_CHARGED));
    Test_CloseBriskThemeOption(task);
    gMain.newKeys = 0;
    task = Test_OpenBriskWorldOptions();
    EXPECT(BriskGimmickGenEnabled());
    gMain.newAndRepeatedKeys = DPAD_DOWN;
    RunTasks(); RunTasks();
    gMain.newAndRepeatedKeys = 0;
    gMain.newKeys = A_BUTTON;
    RunTasks();
    EXPECT(!BriskGimmickGenEnabled());
    EXPECT(FlagGet(B_FLAG_TERA_ORB_CHARGED)); // OFF does not waste a current charge.
    gMain.newKeys = 0;
    Test_CloseBriskThemeOption(task);
    if (!oldOrbs) RemoveBagItem(ITEM_TERA_ORB, 1);
    RestoreFlag(FLAG_BRISK_GIMMICK_GEN, savedOption);
    RestoreFlag(B_FLAG_TERA_ORB_CHARGED, savedCharge);
}

TEST("Brisk GIMMICK GEN recharges on every completed battle outcome and OFF retains the spent charge")
{
    bool32 savedOption = BriskGimmickGenEnabled();
    bool32 savedCharge = FlagGet(B_FLAG_TERA_ORB_CHARGED);
    u16 oldOrbs = CountTotalItemQuantityInBag(ITEM_TERA_ORB);
    u32 oldFlags = gBattleTypeFlags;
    u8 oldOutcome = gBattleOutcome;
    const u8 outcomes[] = {B_OUTCOME_WON, B_OUTCOME_LOST, B_OUTCOME_RAN, B_OUTCOME_CAUGHT, B_OUTCOME_DREW, B_OUTCOME_FORFEITED, B_OUTCOME_PLAYER_TELEPORTED, B_OUTCOME_MON_FLED};
    if (!oldOrbs) EXPECT(AddBagItem(ITEM_TERA_ORB, 1));
    for (u32 enabled = 0; enabled < 2; enabled++)
    {
        RestoreFlag(FLAG_BRISK_GIMMICK_GEN, enabled);
        for (u32 i = 0; i < ARRAY_COUNT(outcomes); i++)
        {
            gBattleTypeFlags = i & 1 ? BATTLE_TYPE_TRAINER : 0;
            AllocateBattleResources();
            FlagClear(B_FLAG_TERA_ORB_CHARGED);
            gBattleOutcome = outcomes[i];
            FreeBattleResources();
            EXPECT_EQ(FlagGet(B_FLAG_TERA_ORB_CHARGED), enabled);
        }
    }
    FlagSet(FLAG_BRISK_GIMMICK_GEN);
    const u32 excluded[] = {BATTLE_TYPE_RECORDED, BATTLE_TYPE_RECORDED_LINK};
    for (u32 i = 0; i < ARRAY_COUNT(excluded); i++)
    {
        gBattleTypeFlags = excluded[i];
        AllocateBattleResources();
        FlagClear(B_FLAG_TERA_ORB_CHARGED);
        gBattleOutcome = B_OUTCOME_WON;
        FreeBattleResources();
        EXPECT(!FlagGet(B_FLAG_TERA_ORB_CHARGED));
    }
    gBattleTypeFlags = 0;
    AllocateBattleResources();
    FlagClear(B_FLAG_TERA_ORB_CHARGED);
    gBattleOutcome = 0; // Resource cleanup before an actual completed battle.
    FreeBattleResources();
    EXPECT(!FlagGet(B_FLAG_TERA_ORB_CHARGED));
    gBattleTypeFlags = oldFlags;
    gBattleOutcome = oldOutcome;
    if (!oldOrbs) RemoveBagItem(ITEM_TERA_ORB, 1);
    RestoreFlag(FLAG_BRISK_GIMMICK_GEN, savedOption);
    RestoreFlag(B_FLAG_TERA_ORB_CHARGED, savedCharge);
}

TEST("Brisk GIMMICK GEN needs an owned Orb and never resets usage within a battle")
{
    bool32 savedOption = BriskGimmickGenEnabled();
    bool32 savedCharge = FlagGet(B_FLAG_TERA_ORB_CHARGED);
    u16 oldOrbs = CountTotalItemQuantityInBag(ITEM_TERA_ORB);
    u32 oldFlags = gBattleTypeFlags;
    u8 oldOutcome = gBattleOutcome;
    if (oldOrbs) RemoveBagItem(ITEM_TERA_ORB, oldOrbs);
    FlagSet(FLAG_BRISK_GIMMICK_GEN);
    FlagClear(B_FLAG_TERA_ORB_CHARGED);
    BriskRechargeGimmicks();
    EXPECT(!FlagGet(B_FLAG_TERA_ORB_CHARGED));
    EXPECT(!CheckBagHasItem(ITEM_TERA_ORB, 1));
    EXPECT(AddBagItem(ITEM_TERA_ORB, 1));
    gBattleTypeFlags = 0;
    AllocateBattleResources();
    SetGimmickAsActivated(B_BATTLER_0, GIMMICK_TERA);
    BriskRechargeGimmicks();
    EXPECT(FlagGet(B_FLAG_TERA_ORB_CHARGED));
    EXPECT(HasTrainerUsedGimmick(B_BATTLER_0, GIMMICK_TERA));
    gBattleOutcome = B_OUTCOME_WON;
    FreeBattleResources();
    AllocateBattleResources();
    EXPECT(!HasTrainerUsedGimmick(B_BATTLER_0, GIMMICK_TERA));
    gBattleOutcome = 0;
    FreeBattleResources();
    RemoveBagItem(ITEM_TERA_ORB, 1);
    if (oldOrbs) AddBagItem(ITEM_TERA_ORB, oldOrbs);
    gBattleTypeFlags = oldFlags;
    gBattleOutcome = oldOutcome;
    RestoreFlag(FLAG_BRISK_GIMMICK_GEN, savedOption);
    RestoreFlag(B_FLAG_TERA_ORB_CHARGED, savedCharge);
}
