#include "global.h"
#include "item_ball.h"
#include "event_data.h"
#include "item.h"
#include "battle_pyramid.h"
#include "constants/event_objects.h"
#include "constants/items.h"

static u32 GetItemBallAmountFromTemplate(u32);
static u32 GetItemBallIdFromTemplate(u32);

static u32 GetItemBallAmountFromTemplate(u32 itemBallId)
{
    u32 amount = gMapHeader.events->objectEvents[itemBallId].movementRangeX;

    if (amount > MAX_BAG_ITEM_CAPACITY)
        return MAX_BAG_ITEM_CAPACITY;

    return (amount == 0) ? 1 : amount;
}

static u32 GetItemBallIdFromTemplate(u32 itemBallId)
{
    enum Item itemId = gMapHeader.events->objectEvents[itemBallId].trainerRange_berryTreeId;

    return (itemId >= ITEMS_COUNT) ? (ITEM_NONE + 1) : itemId;
}

void GetItemBallIdAndAmountFromTemplate(void)
{
    u32 itemBallId = (gSpecialVar_LastTalked - 1);
    gSpecialVar_Result = GetItemBallIdFromTemplate(itemBallId);
    gSpecialVar_0x8009 = GetItemBallAmountFromTemplate(itemBallId);
}


/*
 * A full standard Bag pocket must not make field pickups permanently
 * inaccessible. Use the existing, per-save PC item storage as overflow;
 * never do this inside the Battle Pyramid's temporary Bag.
 *
 * These entrypoints are called by the standard item pickup event scripts.
 * The scripts explicitly tell the player that the item went to the PC.
 */
static void BriskTryStoreOverflowItemInPC(enum Item item, u16 count)
{
    gSpecialVar_Result = FALSE;

    if (item == ITEM_NONE || item >= ITEMS_COUNT || count == 0)
        return;

    if (CurrentBattlePyramidLocation() != PYRAMID_LOCATION_NONE
     || FlagGet(FLAG_STORING_ITEMS_IN_PYRAMID_BAG))
        return;

    gSpecialVar_Result = AddPCItem(item, count);
}

void BriskTryStoreFoundItemInPC(void)
{
    BriskTryStoreOverflowItemInPC(gSpecialVar_0x8004, gSpecialVar_0x8005);
}

void BriskTryStoreGiftItemInPC(void)
{
    BriskTryStoreOverflowItemInPC(gSpecialVar_0x8000, gSpecialVar_0x8001);
}

void BriskTryStoreHiddenItemInPC(void)
{
    BriskTryStoreOverflowItemInPC(gSpecialVar_0x8005, gSpecialVar_0x8009);
}
