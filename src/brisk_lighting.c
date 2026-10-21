#include "global.h"
#include "brisk_lighting.h"
#include "overworld.h"
#include "config/overworld.h"
#include "constants/map_types.h"
#include "constants/region_map_sections.h"
#include "constants/rtc.h"
#include "constants/rgb.h"

// Tint multipliers are unsigned Q0.8, packed R/G/B from low to high byte.
// Every endpoint has a visible ambient grade, including midday.
// Alpha-marked tileset colors still use expansion's warm emissive-light path.
#define TINT(r, g, b) {.blendColor = (r) | ((g) << 8) | ((b) << 16), .isTint = TRUE, .coeff = 8}
#define BLEND(r, g, b, strength) {.blendColor = RGB(r, g, b), .isTint = FALSE, .coeff = strength}

struct EnvironmentLighting
{
    struct BlendSettings time[TIME_NIGHT + 1];
};

// Morning, day, sunset, night. Existing clock interpolation blends these
// endpoints smoothly, rather than snapping colors when an hour changes.
static const struct EnvironmentLighting sProfiles[BRISK_LIGHT_PROFILE_COUNT] =
{
    [BRISK_LIGHT_PLAINS] = {{
        TINT(255, 224, 190), BLEND(29, 25, 18, 3),
        TINT(255, 205, 170), TINT(160, 176, 218),
    }},
    [BRISK_LIGHT_COAST] = {{
        TINT(245, 230, 205), BLEND(10, 25, 29, 3),
        TINT(255, 210, 184), TINT(155, 187, 228),
    }},
    [BRISK_LIGHT_ISLANDS] = {{
        TINT(255, 224, 185), BLEND(27, 28, 17, 3),
        TINT(255, 192, 155), TINT(166, 190, 235),
    }},
    [BRISK_LIGHT_FOREST] = {{
        TINT(205, 235, 184), TINT(204, 242, 195),
        TINT(240, 205, 169), TINT(144, 177, 181),
    }},
    [BRISK_LIGHT_MOUNTAIN] = {{
        TINT(229, 218, 240), BLEND(19, 24, 31, 3),
        TINT(250, 202, 202), TINT(155, 173, 230),
    }},
    [BRISK_LIGHT_VOLCANIC] = {{
        BLEND(28, 15, 8, 2), BLEND(29, 16, 8, 2),
        BLEND(29, 11, 5, 3), BLEND(20, 8, 9, 4),
    }},
    [BRISK_LIGHT_CAVE] = {{
        BLEND(8, 12, 19, 3), BLEND(8, 12, 19, 3),
        BLEND(8, 12, 19, 3), BLEND(8, 12, 19, 3),
    }},
    [BRISK_LIGHT_ICE] = {{
        BLEND(17, 25, 30, 3), BLEND(17, 25, 30, 3),
        BLEND(17, 25, 30, 3), BLEND(17, 25, 30, 3),
    }},
    [BRISK_LIGHT_UNDERWATER] = {{
        BLEND(3, 17, 25, 4), BLEND(3, 17, 25, 4),
        BLEND(3, 17, 25, 4), BLEND(3, 17, 25, 4),
    }},
    [BRISK_LIGHT_RUINS] = {{
        BLEND(18, 15, 10, 3), BLEND(18, 15, 10, 3),
        BLEND(18, 15, 10, 3), BLEND(18, 15, 10, 3),
    }},
    [BRISK_LIGHT_HAUNTED] = {{
        BLEND(14, 11, 21, 3), BLEND(14, 11, 21, 3),
        BLEND(14, 11, 21, 3), BLEND(14, 11, 21, 3),
    }},
    [BRISK_LIGHT_INDUSTRIAL] = {{
        BLEND(16, 23, 24, 3), BLEND(16, 23, 24, 3),
        BLEND(16, 23, 24, 3), BLEND(16, 23, 24, 3),
    }},
    [BRISK_LIGHT_INTERIOR] = {{
        BLEND(29, 25, 18, 3), BLEND(29, 25, 18, 3),
        BLEND(29, 25, 18, 3), BLEND(29, 25, 18, 3),
    }},
};

bool32 BriskLighting_IsEnabled(u32 mapType)
{
    if (!OW_ENVIRONMENTAL_LIGHTING || !OW_ENABLE_DNS)
        return FALSE;
    return mapType == MAP_TYPE_TOWN || mapType == MAP_TYPE_CITY
        || mapType == MAP_TYPE_ROUTE || mapType == MAP_TYPE_OCEAN_ROUTE
        || mapType == MAP_TYPE_UNDERGROUND || mapType == MAP_TYPE_UNDERWATER
        || mapType == MAP_TYPE_INDOOR || mapType == MAP_TYPE_SECRET_BASE;
}

enum BriskLightingProfile BriskLighting_GetProfile(u32 mapType, u32 section)
{
    // Indoor rooms don't inherit outdoor moonlight merely because they share
    // the city's map section. Dedicated dungeon interiors are exceptions.
    if (mapType == MAP_TYPE_UNDERWATER)
        return BRISK_LIGHT_UNDERWATER;
    switch (section)
    {
    case MAPSEC_FIERY_PATH:
    case MAPSEC_FIERY_PATH2:
    case MAPSEC_MT_CHIMNEY:
    case MAPSEC_JAGGED_PASS:
    case MAPSEC_JAGGED_PASS2:
    case MAPSEC_SCORCHED_SLAB:
        if (mapType != MAP_TYPE_INDOOR)
            return BRISK_LIGHT_VOLCANIC;
        break;
    case MAPSEC_SHOAL_CAVE:
    case MAPSEC_ICEFALL_CAVE:
        return BRISK_LIGHT_ICE;
    case MAPSEC_MT_PYRE:
    case MAPSEC_ABANDONED_SHIP:
        return BRISK_LIGHT_HAUNTED;
    case MAPSEC_NEW_MAUVILLE:
        return BRISK_LIGHT_INDUSTRIAL;
    case MAPSEC_SEALED_CHAMBER:
    case MAPSEC_DESERT_RUINS:
    case MAPSEC_SKY_PILLAR:
        return BRISK_LIGHT_RUINS;
    }
    if (mapType == MAP_TYPE_INDOOR || mapType == MAP_TYPE_SECRET_BASE)
        return BRISK_LIGHT_INTERIOR;
    if (mapType == MAP_TYPE_UNDERGROUND)
        return BRISK_LIGHT_CAVE;
    switch (section)
    {
    case MAPSEC_ROUTE_143:
    case MAPSEC_ROUTE_144:
    case MAPSEC_ORANGE_CITY:
    case MAPSEC_ROUTE_145:
    case MAPSEC_ROUTE_146:
    case MAPSEC_ROUTE_147:
    case MAPSEC_ROUTE_148:
    case MAPSEC_ROUTE_149:
        return BRISK_LIGHT_ISLANDS;
    case MAPSEC_PETALBURG_WOODS:
    case MAPSEC_FORTREE_CITY:
    case MAPSEC_ROUTE_119:
    case MAPSEC_ROUTE_120:
        return BRISK_LIGHT_FOREST;
    case MAPSEC_FALLARBOR_TOWN:
    case MAPSEC_ROUTE_113:
    case MAPSEC_ROUTE_114:
    case MAPSEC_ROUTE_115:
        return BRISK_LIGHT_MOUNTAIN;
    case MAPSEC_DEWFORD_TOWN:
    case MAPSEC_SLATEPORT_CITY:
    case MAPSEC_LILYCOVE_CITY:
    case MAPSEC_MOSSDEEP_CITY:
    case MAPSEC_PACIFIDLOG_TOWN:
        return BRISK_LIGHT_COAST;
    }
    return mapType == MAP_TYPE_OCEAN_ROUTE ? BRISK_LIGHT_COAST : BRISK_LIGHT_PLAINS;
}

struct BlendSettings BriskLighting_GetTimeBlend(u32 time)
{
    enum BriskLightingProfile profile;
    if (!OW_ENVIRONMENTAL_LIGHTING)
        return gTimeOfDayBlend[time <= TIME_NIGHT ? time : TIME_DAY];
    profile = BriskLighting_GetProfile(gMapHeader.mapType, gMapHeader.regionMapSectionId);
    return sProfiles[profile].time[time <= TIME_NIGHT ? time : TIME_DAY];
}
