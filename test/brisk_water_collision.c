#include "global.h"
#include "field_player_avatar.h"
#include "fieldmap.h"
#include "event_object_movement.h"
#include "test/test.h"
#include "constants/metatile_behaviors.h"

bool32 Test_PlayerCannotEnterWater(struct ObjectEvent *objectEvent, u8 behavior);
bool8 Test_CanStopSurfing(s16 x, s16 y, enum Direction direction);

TEST("Brisk water cannot be entered on foot or bike while bridges, shallows, Surf and Dive remain usable")
{
    struct PlayerAvatar saved = gPlayerAvatar;
    const u8 water[] = {MB_POND_WATER, MB_OCEAN_WATER, MB_DEEP_WATER, MB_WATERFALL};
    const u8 flags[] = {PLAYER_AVATAR_FLAG_ON_FOOT, PLAYER_AVATAR_FLAG_MACH_BIKE, PLAYER_AVATAR_FLAG_ACRO_BIKE};
    gPlayerAvatar.objectEventId = 0;
    for (u32 mode = 0; mode < ARRAY_COUNT(flags); mode++)
    {
        gPlayerAvatar.flags = flags[mode];
        for (u32 i = 0; i < ARRAY_COUNT(water); i++)
        {
            EXPECT(Test_PlayerCannotEnterWater(&gObjectEvents[0], water[i]));
            EXPECT_EQ(CheckForObjectEventCollision(&gObjectEvents[0], 8, 8, DIR_SOUTH, water[i]), COLLISION_IMPASSABLE);
        }
        EXPECT(!Test_PlayerCannotEnterWater(&gObjectEvents[0], MB_BRIDGE_OVER_OCEAN));
        EXPECT(!Test_PlayerCannotEnterWater(&gObjectEvents[0], MB_FORTREE_BRIDGE));
        EXPECT(!Test_PlayerCannotEnterWater(&gObjectEvents[0], MB_SHALLOW_WATER));
        EXPECT(!Test_PlayerCannotEnterWater(&gObjectEvents[0], MB_NORMAL));
        EXPECT(!Test_PlayerCannotEnterWater(&gObjectEvents[1], MB_POND_WATER));
    }
    gPlayerAvatar.flags = PLAYER_AVATAR_FLAG_SURFING;
    EXPECT(!Test_PlayerCannotEnterWater(&gObjectEvents[0], MB_POND_WATER));
    gPlayerAvatar.flags = PLAYER_AVATAR_FLAG_UNDERWATER;
    EXPECT(!Test_PlayerCannotEnterWater(&gObjectEvents[0], MB_DEEP_WATER));
    gPlayerAvatar = saved;
}

TEST("Brisk surfing cannot dismount onto water that mistakenly uses land elevation")
{
    static const u16 attributes[] = {MB_POND_WATER};
    static const struct Tileset tileset = {.metatileAttributes = attributes};
    static const u16 border[] = {0, 0, 0, 0};
    static const struct MapLayout layout = {
        .width = 4, .height = 4, .border = border, .borderWidth = 2, .borderHeight = 2,
        .primaryTileset = &tileset, .secondaryTileset = &tileset,
    };
    struct MapHeader savedHeader = gMapHeader;
    struct BackupMapLayout savedBackup = gBackupMapLayout;
    struct PlayerAvatar savedAvatar = gPlayerAvatar;
    u16 map[24 * 24];
    for (u32 i = 0; i < ARRAY_COUNT(map); i++) map[i] = ELEVATION_DEFAULT << 12;
    gMapHeader.mapLayout = &layout;
    gBackupMapLayout.map = map;
    gBackupMapLayout.width = gBackupMapLayout.height = 24;
    gPlayerAvatar.flags = PLAYER_AVATAR_FLAG_SURFING;
    EXPECT_EQ(MapGridGetElevationAt(8, 8), ELEVATION_DEFAULT);
    EXPECT_EQ(MapGridGetMetatileBehaviorAt(8, 8), MB_POND_WATER);
    EXPECT(!Test_CanStopSurfing(8, 8, DIR_SOUTH));
    gMapHeader = savedHeader;
    gBackupMapLayout = savedBackup;
    gPlayerAvatar = savedAvatar;
}
