#include "global.h"
#include "item.h"
#include "test/test.h"

TEST("Brisk PC item storage compacts fully withdrawn stacks")
{
    memset(gSaveBlock1Ptr->pcItems, 0, sizeof(gSaveBlock1Ptr->pcItems));
    EXPECT(AddPCItem(ITEM_POTION, 1));
    EXPECT(AddPCItem(ITEM_SUPER_POTION, 2));

    RemovePCItem(0, 1);

    EXPECT_EQ(gSaveBlock1Ptr->pcItems[0].itemId, ITEM_SUPER_POTION);
    EXPECT_EQ(gSaveBlock1Ptr->pcItems[0].quantity, 2);
    EXPECT_EQ(gSaveBlock1Ptr->pcItems[1].itemId, ITEM_NONE);
    EXPECT_EQ(gSaveBlock1Ptr->pcItems[1].quantity, 0);
    EXPECT_EQ(CountUsedPCItemSlots(), 1);

    EXPECT(AddPCItem(ITEM_POKE_BALL, 3));
    EXPECT_EQ(gSaveBlock1Ptr->pcItems[1].itemId, ITEM_POKE_BALL);
    EXPECT_EQ(gSaveBlock1Ptr->pcItems[1].quantity, 3);
}

TEST("Brisk PC item storage keeps partially withdrawn stacks")
{
    memset(gSaveBlock1Ptr->pcItems, 0, sizeof(gSaveBlock1Ptr->pcItems));
    EXPECT(AddPCItem(ITEM_POTION, 5));
    EXPECT(AddPCItem(ITEM_SUPER_POTION, 1));

    RemovePCItem(0, 2);

    EXPECT_EQ(gSaveBlock1Ptr->pcItems[0].itemId, ITEM_POTION);
    EXPECT_EQ(gSaveBlock1Ptr->pcItems[0].quantity, 3);
    EXPECT_EQ(gSaveBlock1Ptr->pcItems[1].itemId, ITEM_SUPER_POTION);
    EXPECT_EQ(gSaveBlock1Ptr->pcItems[1].quantity, 1);
}

TEST("Brisk PC item storage rejects invalid withdrawals without losing items")
{
    memset(gSaveBlock1Ptr->pcItems, 0, sizeof(gSaveBlock1Ptr->pcItems));
    EXPECT(AddPCItem(ITEM_POTION, 2));

    RemovePCItem(0, 3);
    RemovePCItem(PC_ITEMS_COUNT, 1);
    RemovePCItem(0, 0);

    EXPECT_EQ(gSaveBlock1Ptr->pcItems[0].itemId, ITEM_POTION);
    EXPECT_EQ(gSaveBlock1Ptr->pcItems[0].quantity, 2);
}
