#include "global.h"
#include "fieldmap.h"
#include "test/test.h"
#include "constants/layouts.h"

void Test_RepairCachedBriskWarpTiles(u16 layoutId);

TEST("Brisk old saved map tiles cannot cover corrected exits or Mart approaches")
{
    static const struct WarpEvent warps[] = {{.x = 1, .y = 2}, {.x = 2, .y = 2}};
    static const struct MapEvents events = {.warpCount = ARRAY_COUNT(warps), .warps = warps};
    static const u16 romTiles[16] = {[5] = 0x3001, [6] = 0x3002, [9] = 0x3265, [10] = 0x3265};
    static const struct MapLayout layout = {.width = 4, .height = 4, .map = romTiles};
    static const u16 layouts[] = {LAYOUT_MART, LAYOUT_AQUA_HIDEOUT_1F, LAYOUT_SKY_PILLAR_5F, LAYOUT_SKY_PILLAR_5F_CLEAN};
    struct MapHeader previousHeader = gMapHeader;
    struct BackupMapLayout previousBackup = gBackupMapLayout;
    u16 cached[24 * 24];
    gMapHeader.mapLayout = &layout;
    gMapHeader.events = &events;
    gBackupMapLayout.map = cached;
    gBackupMapLayout.width = gBackupMapLayout.height = 24;
    for (u32 i = 0; i < ARRAY_COUNT(layouts); i++)
    {
        for (u32 j = 0; j < ARRAY_COUNT(cached); j++)
            cached[j] = 0x3C00; // A blocked tile from an older save.
        Test_RepairCachedBriskWarpTiles(layouts[i]);
        EXPECT_EQ(cached[(2 + MAP_OFFSET) * 24 + 1 + MAP_OFFSET], romTiles[9]);
        EXPECT_EQ(cached[(2 + MAP_OFFSET) * 24 + 2 + MAP_OFFSET], romTiles[10]);
        EXPECT_EQ(cached[(1 + MAP_OFFSET) * 24 + 1 + MAP_OFFSET], layouts[i] == LAYOUT_MART ? romTiles[5] : 0x3C00);
        EXPECT_EQ(cached[MAP_OFFSET * 24 + MAP_OFFSET], 0x3C00); // Unrelated changes survive.
    }
    Test_RepairCachedBriskWarpTiles(LAYOUT_LITTLEROOT_TOWN);
    EXPECT_EQ(cached[MAP_OFFSET * 24 + MAP_OFFSET], 0x3C00);
    gMapHeader = previousHeader;
    gBackupMapLayout = previousBackup;
}
