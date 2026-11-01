#include "global.h"
#include "event_data.h"
#include "item.h"
#include "brisk_modes.h"
#include "shop_criteria.h"
#include "test/test.h"

bool32 Test_BriskGeneralMartSells(u16 item);

TEST("Brisk general marts sell Repels before the first gym and at pre-Wattson Mauville")
{
    bool32 saved[NUM_BADGES];
    for (u32 i = 0; i < NUM_BADGES; i++)
    {
        saved[i] = FlagGet(FLAG_BADGE01_GET + i);
        FlagClear(FLAG_BADGE01_GET + i);
    }
    for (u32 count = 0; count <= NUM_BADGES; count++)
    {
        if (count) FlagSet(FLAG_BADGE01_GET + count - 1);
        EXPECT(Test_BriskGeneralMartSells(ITEM_REPEL));
        EXPECT(Test_BriskGeneralMartSells(ITEM_POKE_BALL));
    }
    for (u32 i = 0; i < NUM_BADGES; i++)
        if (saved[i]) FlagSet(FLAG_BADGE01_GET + i);
        else FlagClear(FLAG_BADGE01_GET + i);
}

TEST("Brisk EXP Candy badge gates apply to general marts and fixed special inventories in every mode")
{
    static const struct { u16 item; u8 badges; } candies[] = {
        {ITEM_EXP_CANDY_XS, 3}, {ITEM_EXP_CANDY_S, 4},
        {ITEM_EXP_CANDY_M, 5}, {ITEM_EXP_CANDY_L, 7}, {ITEM_EXP_CANDY_XL, 8},
    };
    bool32 saved[NUM_BADGES];
    u16 savedMode = VarGet(VAR_BRISK_DIFFICULTY);
    for (u32 i = 0; i < NUM_BADGES; i++)
        saved[i] = FlagGet(FLAG_BADGE01_GET + i);
    for (u32 mode = BRISK_MODE_NORMAL; mode <= BRISK_MODE_MAX; mode++)
    {
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        for (u32 i = 0; i < NUM_BADGES; i++)
            FlagClear(FLAG_BADGE01_GET + i);
        for (u32 count = 0; count <= NUM_BADGES; count++)
        {
            if (count) FlagSet(FLAG_BADGE01_GET + count - 1);
            for (u32 i = 0; i < ARRAY_COUNT(candies); i++)
            {
                // Fixed shop lists pass through this same item predicate.
                EXPECT_EQ(IsItemShopCriteriaFulfilled(candies[i].item), count >= candies[i].badges);
                EXPECT_EQ(Test_BriskGeneralMartSells(candies[i].item), count >= candies[i].badges);
            }
            EXPECT(IsItemShopCriteriaFulfilled(ITEM_POTION));
            EXPECT(Test_BriskGeneralMartSells(ITEM_REPEL));
        }
    }
    VarSet(VAR_BRISK_DIFFICULTY, savedMode);
    for (u32 i = 0; i < NUM_BADGES; i++)
        if (saved[i]) FlagSet(FLAG_BADGE01_GET + i);
        else FlagClear(FLAG_BADGE01_GET + i);
}

TEST("Brisk filtered fixed shops keep their full stock when Buy is reopened")
{
    static const u16 original[] = {ITEM_EXP_CANDY_XL, ITEM_POTION, ITEM_POKE_BALL, ITEM_NONE};
    const u16 *items = original;
    u16 count = 3;
    bool32 saved[NUM_BADGES];
    for (u32 i = 0; i < NUM_BADGES; i++)
    {
        saved[i] = FlagGet(FLAG_BADGE01_GET + i);
        FlagClear(FLAG_BADGE01_GET + i);
    }
    for (u32 visit = 0; visit < 3; visit++)
    {
        TryBuildDynamicShopItemList(&items, &count);
        EXPECT_EQ(count, 2);
        EXPECT_EQ(items[0], ITEM_POTION);
        EXPECT_EQ(items[1], ITEM_POKE_BALL);
        EXPECT_EQ(items[2], ITEM_NONE);
        TryFreeDynamicShopItemList(&items, &count);
        EXPECT_EQ(items, original);
        EXPECT_EQ(count, 3);
    }
    for (u32 i = 0; i < NUM_BADGES; i++)
        if (saved[i]) FlagSet(FLAG_BADGE01_GET + i);
        else FlagClear(FLAG_BADGE01_GET + i);
}
