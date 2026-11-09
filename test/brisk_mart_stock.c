#include "global.h"
#include "event_data.h"
#include "test/test.h"
#include "constants/maps.h"

bool32 Test_BriskGeneralMartSells(u16 item);
bool32 Test_BriskMartInventorySells(u8 mapGroup, u8 mapNum, u16 item);

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

TEST("Slateport Mart carries Harbor Mail while regular marts retain their own stock")
{
    EXPECT(Test_BriskMartInventorySells(MAP_GROUP(MAP_SLATEPORT_CITY_MART),
        MAP_NUM(MAP_SLATEPORT_CITY_MART), ITEM_HARBOR_MAIL));
    EXPECT(!Test_BriskMartInventorySells(MAP_GROUP(MAP_MAUVILLE_CITY_MART),
        MAP_NUM(MAP_MAUVILLE_CITY_MART), ITEM_HARBOR_MAIL));
    EXPECT(Test_BriskMartInventorySells(MAP_GROUP(MAP_SLATEPORT_CITY_MART),
        MAP_NUM(MAP_SLATEPORT_CITY_MART), ITEM_POKE_BALL));
}
