#include "global.h"
#include "event_object_movement.h"
#include "overworld.h"
#include "test/test.h"
#include "constants/event_objects.h"

extern const struct ObjectEventGraphicsInfo gObjectEventGraphicsInfo_Woman2Frlg;

TEST("Brisk Kanto arrival and onward-route NPC graphics have complete sprite data")
{
    EXPECT_EQ(GetObjectEventGraphicsInfo(OBJ_EVENT_GFX_WOMAN_2_FRLG), &gObjectEventGraphicsInfo_Woman2Frlg);
    const u16 palettes[] = {OBJ_EVENT_PAL_TAG_NPC_BLUE, OBJ_EVENT_PAL_TAG_NPC_PINK,
        OBJ_EVENT_PAL_TAG_NPC_GREEN, OBJ_EVENT_PAL_TAG_SEAGALLOP};
    for (u32 i = 0; i < ARRAY_COUNT(palettes); i++)
    {
        const struct SpritePalette *palette = GetObjectEventPaletteFromTag(palettes[i]);
        EXPECT(palette != NULL);
        EXPECT(palette->data != NULL);
    }
    const u16 maps[] = {MAP_CINNABAR_ISLAND, MAP_ROUTE20, MAP_ROUTE21_SOUTH,
        MAP_PALLET_TOWN, MAP_VIRIDIAN_CITY, MAP_CINNABAR_ISLAND_POKEMON_CENTER_1F};
    for (u32 m = 0; m < ARRAY_COUNT(maps); m++)
    {
        const struct MapHeader *header = Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(maps[m]), MAP_NUM(maps[m]));
        for (u32 i = 0; i < header->events->objectEventCount; i++)
        {
            const struct ObjectEventTemplate *obj = &header->events->objectEvents[i];
            if (obj->graphicsId >= OBJ_EVENT_GFX_VARS && obj->graphicsId <= OBJ_EVENT_GFX_VAR_F) continue;
            const struct ObjectEventGraphicsInfo *gfx = GetObjectEventGraphicsInfo(obj->graphicsId);
            EXPECT(gfx != NULL);
            EXPECT(gfx->images != NULL);
            EXPECT(gfx->anims != NULL);
            EXPECT(gfx->oam != NULL);
            EXPECT(gfx->width > 0);
            EXPECT(gfx->height > 0);
        }
    }
}
