#include "global.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_player_avatar.h"
#include "fieldmap.h"
#include "malloc.h"
#include "overworld.h"
#include "regions.h"
#include "region_map.h"
#include "script.h"
#include "test/test.h"
#include "constants/layouts.h"
#include "constants/event_objects.h"
#include "constants/trainer_types.h"
#include "constants/map_scripts.h"

extern const u8 BriskKantoAccessOnly_EventScript_NotReady[];
u8 Test_GetInitialPlayerAvatarTransitionFlags(u16 behavior, enum MapType mapType);
void Test_RepairCachedBriskWarpTiles(u16 layoutId);

static const struct MapHeader *Header(u16 id)
{
    return Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(id), MAP_NUM(id));
}

TEST("Brisk Kanto access includes original cities and interiors without enabling unported events")
{
    const u16 maps[] = {MAP_PALLET_TOWN, MAP_VIRIDIAN_CITY, MAP_PEWTER_CITY,
        MAP_CERULEAN_CITY, MAP_LAVENDER_TOWN, MAP_VERMILION_CITY, MAP_CELADON_CITY,
        MAP_FUCHSIA_CITY, MAP_CINNABAR_ISLAND, MAP_SAFFRON_CITY, MAP_INDIGO_PLATEAU_EXTERIOR,
        MAP_ROUTE20, MAP_ROUTE21_SOUTH, MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB,
        MAP_CINNABAR_ISLAND_POKEMON_CENTER_1F};
    struct MapHeader saved = gMapHeader;
    u16 story = VarGet(VAR_ROUTE118_STATE);
    for (u32 i = 0; i < ARRAY_COUNT(maps); i++)
    {
        const struct MapHeader *h = Header(maps[i]);
        EXPECT(h != NULL);
        EXPECT(h->mapLayout != NULL);
        EXPECT(h->mapLayout->isFrlg);
        EXPECT(h->mapLayout->width > 0);
        EXPECT(h->mapLayout->height > 0);
        EXPECT_EQ(h->mapScripts[0], 0);
        EXPECT_EQ(h->events->coordEventCount, 0);
        for (u32 j = 0; j < h->events->objectEventCount; j++)
        {
            const struct ObjectEventTemplate *obj = &h->events->objectEvents[j];
            if (obj->kind != OBJ_KIND_CLONE)
            {
                EXPECT_EQ(obj->trainerType, TRAINER_TYPE_NONE);
                EXPECT_EQ(obj->script, BriskKantoAccessOnly_EventScript_NotReady);
            }
        }
        gMapHeader = *h;
        EXPECT_EQ(MapHeaderGetScriptTable(MAP_SCRIPT_ON_TRANSITION), NULL);
        EXPECT_EQ(MapHeaderGetScriptTable(MAP_SCRIPT_ON_FRAME_TABLE), NULL);
    }
    EXPECT_EQ(Header(MAP_CINNABAR_ISLAND)->events->warpCount, 5);
    EXPECT_EQ(VarGet(VAR_ROUTE118_STATE), story);
    gMapHeader = saved;
}

TEST("Brisk Kanto water crossings resume Surf in both tileset formats and reject walking")
{
    struct MapHeader savedHeader = gMapHeader;
    struct BackupMapLayout savedBackup = gBackupMapLayout;
    struct PlayerAvatar savedPlayer = gPlayerAvatar;
    struct ObjectEvent savedObject = gObjectEvents[0];
    bool32 cruise = FlagGet(FLAG_SYS_CRUISE_MODE);
    FlagClear(FLAG_SYS_CRUISE_MODE);
    const u16 maps[] = {MAP_ORANGE_STRAIT_WEST, MAP_ORANGE_STRAIT_EAST};
    for (u32 i = 0; i < ARRAY_COUNT(maps); i++)
    {
        gMapHeader = *Header(maps[i]);
        const struct MapLayout *layout = gMapHeader.mapLayout;
        u32 w = layout->width + MAP_OFFSET_W, h = layout->height + MAP_OFFSET_H;
        u16 *grid = AllocZeroed(w*h*sizeof(u16));
        EXPECT(grid != NULL);
        gBackupMapLayout = (struct BackupMapLayout){.map = grid, .width = w, .height = h};
        for (u32 y = 0; y < layout->height; y++)
            for (u32 x = 0; x < layout->width; x++)
                grid[(y+MAP_OFFSET)*w+x+MAP_OFFSET] = layout->map[y*layout->width+x];
        s16 x = 20+MAP_OFFSET, y = 2+MAP_OFFSET;
        u16 behavior = MapGridGetMetatileBehaviorAt(x,y);
        EXPECT_EQ(Test_GetInitialPlayerAvatarTransitionFlags(behavior, MAP_TYPE_ROUTE), PLAYER_AVATAR_FLAG_SURFING);
        EXPECT_EQ(MapGridGetElevationAt(x,y), ELEVATION_SURF);
        gPlayerAvatar.objectEventId = 0;
        gObjectEvents[0] = (struct ObjectEvent){.active = TRUE, .currentElevation = ELEVATION_SURF};
        gObjectEvents[0].currentCoords.x = x-1;
        gObjectEvents[0].currentCoords.y = y;
        gPlayerAvatar.flags = PLAYER_AVATAR_FLAG_SURFING;
        EXPECT_EQ(CheckForObjectEventCollision(&gObjectEvents[0],x,y,DIR_EAST,behavior), COLLISION_NONE);
        gPlayerAvatar.flags = PLAYER_AVATAR_FLAG_ON_FOOT;
        EXPECT_EQ(CheckForObjectEventCollision(&gObjectEvents[0],x,y,DIR_EAST,behavior), COLLISION_IMPASSABLE);
        Free(grid);
    }
    if (cruise) FlagSet(FLAG_SYS_CRUISE_MODE);
    gObjectEvents[0] = savedObject;
    gPlayerAvatar = savedPlayer;
    gMapHeader = savedHeader;
    gBackupMapLayout = savedBackup;
}

TEST("Brisk Kanto passage repairs old Route 146 water caches without changing land or other water")
{
    struct MapHeader savedHeader = gMapHeader;
    struct BackupMapLayout savedBackup = gBackupMapLayout;
    gMapHeader = *Header(MAP_ROUTE146);
    const struct MapLayout *layout = gMapHeader.mapLayout;
    u32 w = layout->width+MAP_OFFSET_W, h = layout->height+MAP_OFFSET_H;
    u16 *grid = AllocZeroed(w*h*sizeof(u16));
    EXPECT(grid != NULL);
    gBackupMapLayout = (struct BackupMapLayout){.map = grid, .width = w, .height = h};
    for (u32 i = 0; i < w*h; i++) grid[i] = 0x3C00;
    Test_RepairCachedBriskWarpTiles(LAYOUT_ROUTE146);
    for (u32 y = 0; y < layout->height; y++)
        for (u32 x = 0; x < layout->width; x++)
        {
            u16 block = layout->map[y*layout->width+x];
            bool32 approach = x>=20 && x<=62 && y>=24 && y<=43
                && UNPACK_METATILE(block)==0x12B && UNPACK_COLLISION(block)==0;
            EXPECT_EQ(grid[(y+MAP_OFFSET)*w+x+MAP_OFFSET], approach ? block : 0x3C00);
        }
    Free(grid);
    gMapHeader = savedHeader;
    gBackupMapLayout = savedBackup;
}

TEST("Brisk Kanto Town Map switches regions at the Orange Strait without enabling Kanto events")
{
    const struct MapHeader *west = Header(MAP_ORANGE_STRAIT_WEST);
    const struct MapHeader *east = Header(MAP_ORANGE_STRAIT_EAST);
    struct MapHeader savedHeader = gMapHeader;
    EXPECT_EQ(west->regionMapSectionId, MAPSEC_ORANGE_STRAIT);
    EXPECT_EQ(east->regionMapSectionId, MAPSEC_ROUTE_20);
    gMapHeader = *west;
    EXPECT_EQ(GetCurrentRegion(), REGION_HOENN);
    EXPECT_EQ(GetRegionMapType(gMapHeader.regionMapSectionId), REGION_MAP_HOENN);
    gMapHeader = *east;
    EXPECT_EQ(GetCurrentRegion(), REGION_KANTO);
    EXPECT_EQ(GetRegionMapType(gMapHeader.regionMapSectionId), REGION_MAP_KANTO);
    EXPECT_EQ(east->events->coordEventCount, 40);
    EXPECT_EQ(west->events->coordEventCount, 20);
    gMapHeader = savedHeader;
}
