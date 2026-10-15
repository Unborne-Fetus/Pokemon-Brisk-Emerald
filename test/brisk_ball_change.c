#include "global.h"
#include "battle.h"
#include "item.h"
#include "main.h"
#include "party_menu.h"
#include "pokeball.h"
#include "pokemon.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "test/test.h"

static struct Pokemon *MakeBallTestMon(enum PokeBall ball)
{
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][0];
    CreateMonWithIVs(mon, SPECIES_PIKACHU, 20, 0, OTID_STRUCT_PRESET(0), 0);
    u8 value = ball;
    SetMonData(mon, MON_DATA_POKEBALL, &value);
    ClearBag();
    return mon;
}

TEST("Brisk ball changes consume one replacement and survive boxing")
{
    struct Pokemon *mon = MakeBallTestMon(BALL_POKE);
    AddBagItem(ITEM_LUXURY_BALL, 2);
    u8 capsule = 27;
    SetMonData(mon, MON_DATA_BALL_CAPSULE, &capsule);
    u32 personality = GetMonData(mon, MON_DATA_PERSONALITY);
    EXPECT(Test_ChangeMonBall(mon, BALL_LUXURY));
    EXPECT_EQ(CountTotalItemQuantityInBag(ITEM_LUXURY_BALL), 1);
    EXPECT_EQ(CountTotalItemQuantityInBag(ITEM_POKE_BALL), 0);
    struct Pokemon restored;
    BoxMonToMon(&mon->box, &restored);
    EXPECT_EQ(GetMonData(&restored, MON_DATA_POKEBALL), BALL_LUXURY);
    EXPECT_EQ(GetMonData(&restored, MON_DATA_BALL_CAPSULE), capsule);
    EXPECT_EQ(GetMonData(&restored, MON_DATA_PERSONALITY), personality);
    EXPECT_EQ(GetMonData(&restored, MON_DATA_SPECIES), SPECIES_PIKACHU);
    EXPECT_EQ(GetMonData(&restored, MON_DATA_LEVEL), 20);
    EXPECT(!Test_ChangeMonBall(mon, BALL_LUXURY));
    EXPECT_EQ(CountTotalItemQuantityInBag(ITEM_LUXURY_BALL), 1);
}

TEST("Brisk ball changes reject Master Balls in either direction")
{
    struct Pokemon *mon = MakeBallTestMon(BALL_MASTER);
    AddBagItem(ITEM_POKE_BALL, 1);
    EXPECT(!Test_ChangeMonBall(mon, BALL_POKE));
    EXPECT_EQ(GetMonData(mon, MON_DATA_POKEBALL), BALL_MASTER);
    EXPECT_EQ(CountTotalItemQuantityInBag(ITEM_POKE_BALL), 1);
    mon = MakeBallTestMon(BALL_POKE);
    AddBagItem(ITEM_MASTER_BALL, 1);
    EXPECT(!Test_ChangeMonBall(mon, BALL_MASTER));
    EXPECT_EQ(GetMonData(mon, MON_DATA_POKEBALL), BALL_POKE);
    EXPECT_EQ(CountTotalItemQuantityInBag(ITEM_MASTER_BALL), 1);
}

TEST("Brisk ball changes reject missing balls, Eggs and invalid selections")
{
    struct Pokemon *mon = MakeBallTestMon(BALL_POKE);
    EXPECT(!Test_ChangeMonBall(mon, BALL_ULTRA));
    EXPECT(!Test_ChangeMonBall(mon, BALL_STRANGE));
    EXPECT(!Test_ChangeMonBall(mon, POKEBALL_COUNT));
    AddBagItem(ITEM_ULTRA_BALL, 1);
    bool8 egg = TRUE;
    SetMonData(mon, MON_DATA_IS_EGG, &egg);
    EXPECT(!Test_ChangeMonBall(mon, BALL_ULTRA));
    EXPECT_EQ(CountTotalItemQuantityInBag(ITEM_ULTRA_BALL), 1);
    EXPECT_EQ(GetMonData(mon, MON_DATA_POKEBALL), BALL_POKE);
}

TEST("Brisk ball selector skips Master Balls and cancellation spends nothing")
{
    struct Pokemon *mon = MakeBallTestMon(BALL_POKE);
    AddBagItem(ITEM_GREAT_BALL, 2);
    AddBagItem(ITEM_MASTER_BALL, 1);
    AddBagItem(ITEM_PREMIER_BALL, 1);
    ResetTasks();
    u8 taskId = Test_OpenBriskBallEditor();
    EXPECT_EQ(gTasks[taskId].data[12], BALL_GREAT);
    EXPECT_LE(GetStringWidth(FONT_NORMAL, gStringVar4, 0), 224);
    gMain.newKeys = 0;
    gMain.newAndRepeatedKeys = DPAD_RIGHT;
    RunTasks();
    EXPECT_EQ(gTasks[taskId].data[12], BALL_PREMIER);
    gMain.newAndRepeatedKeys = DPAD_LEFT;
    RunTasks();
    EXPECT_EQ(gTasks[taskId].data[12], BALL_GREAT);
    gMain.newAndRepeatedKeys = 0;
    gMain.newKeys = B_BUTTON;
    RunTasks();
    EXPECT_EQ(GetMonData(mon, MON_DATA_POKEBALL), BALL_POKE);
    EXPECT_EQ(CountTotalItemQuantityInBag(ITEM_GREAT_BALL), 2);
    Test_CloseBriskCapsuleEditor();
    taskId = Test_OpenBriskBallEditor();
    gMain.newKeys = A_BUTTON;
    RunTasks();
    EXPECT_EQ(GetMonData(mon, MON_DATA_POKEBALL), BALL_GREAT);
    EXPECT_EQ(CountTotalItemQuantityInBag(ITEM_GREAT_BALL), 1);
    gMain.newKeys = gMain.newAndRepeatedKeys = 0;
    Test_CloseBriskCapsuleEditor();
}
