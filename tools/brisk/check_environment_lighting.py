#!/usr/bin/env python3
"""Run the actual C lighting selector against outdoor/interior regressions.

Requires a host C compiler; no ROM/emulator or save file is needed.
"""
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]
with tempfile.TemporaryDirectory() as directory:
    tmp = Path(directory)
    (tmp / 'config').mkdir()
    (tmp / 'brisk_lighting.h').write_text((ROOT / 'include/brisk_lighting.h').read_text())
    (tmp / 'global.h').write_text('''#include <stdint.h>
#include <assert.h>
typedef uint32_t u32;
typedef uint32_t bool32;
#define TRUE 1
#define FALSE 0
''')
    (tmp / 'palette.h').write_text('''#ifndef TEST_PALETTE
#define TEST_PALETTE
struct BlendSettings { unsigned blendColor:24, isTint:1, coeff:5, unused:2; };
extern const struct BlendSettings gTimeOfDayBlend[];
#endif
''')
    (tmp / 'overworld.h').write_text('''struct TestMapHeader { u32 mapType, regionMapSectionId; };
extern struct TestMapHeader gMapHeader;
''')
    (tmp / 'config/overworld.h').write_text('''#define OW_ENABLE_DNS TRUE
#define OW_ENVIRONMENTAL_LIGHTING TRUE
''')
    harness = tmp / 'test.c'
    harness.write_text('''#include "global.h"
#include "brisk_lighting.h"
#include "overworld.h"
#include "constants/map_types.h"
#include "constants/region_map_sections.h"
#include "constants/rtc.h"
struct TestMapHeader gMapHeader;
const struct BlendSettings gTimeOfDayBlend[4] = {0};
int main(void) {
    unsigned section, type, time;
    struct BlendSettings dawn, day, dusk, night;
    assert(!BriskLighting_IsEnabled(MAP_TYPE_NONE));
    assert(!BriskLighting_IsEnabled(MAP_TYPE_UNKNOWN));
    assert(BriskLighting_IsEnabled(MAP_TYPE_UNDERGROUND));
    assert(BriskLighting_IsEnabled(MAP_TYPE_INDOOR));
    assert(BriskLighting_GetProfile(MAP_TYPE_INDOOR, MAPSEC_ORANGE_CITY) == BRISK_LIGHT_INTERIOR);
    assert(BriskLighting_GetProfile(MAP_TYPE_ROUTE, MAPSEC_ROUTE_145) == BRISK_LIGHT_ISLANDS);
    assert(BriskLighting_GetProfile(MAP_TYPE_UNDERGROUND, MAPSEC_GRANITE_CAVE) == BRISK_LIGHT_CAVE);
    assert(BriskLighting_GetProfile(MAP_TYPE_UNDERGROUND, MAPSEC_SHOAL_CAVE) == BRISK_LIGHT_ICE);
    assert(BriskLighting_GetProfile(MAP_TYPE_UNDERWATER, MAPSEC_ROUTE_145) == BRISK_LIGHT_UNDERWATER);
    assert(BriskLighting_GetProfile(MAP_TYPE_INDOOR, MAPSEC_NEW_MAUVILLE) == BRISK_LIGHT_INDUSTRIAL);
    // Every map type/section selects valid settings, including unexpected TOD.
    for (type = MAP_TYPE_TOWN; type <= MAP_TYPE_SECRET_BASE; type++) {
        for (section = 0; section <= MAPSEC_ROUTE_149; section++) {
            gMapHeader.mapType = type;
            gMapHeader.regionMapSectionId = section;
            assert(BriskLighting_GetProfile(type, section) < BRISK_LIGHT_PROFILE_COUNT);
            for (time = 0; time < 6; time++) {
                struct BlendSettings b = BriskLighting_GetTimeBlend(time);
                assert(b.coeff <= 8);
                assert(b.isTint || b.coeff > 0); // No neutral midday endpoints.
            }
        }
    }
    gMapHeader.mapType = MAP_TYPE_ROUTE;
    gMapHeader.regionMapSectionId = MAPSEC_ROUTE_145;
    dawn = BriskLighting_GetTimeBlend(TIME_MORNING);
    dusk = BriskLighting_GetTimeBlend(TIME_EVENING);
    night = BriskLighting_GetTimeBlend(TIME_NIGHT);
    assert(dawn.blendColor != dusk.blendColor);
    assert((dusk.blendColor & 255) > ((dusk.blendColor >> 16) & 255));
    assert((night.blendColor & 255) < ((night.blendColor >> 16) & 255));
    gMapHeader.mapType = MAP_TYPE_INDOOR;
    day = BriskLighting_GetTimeBlend(TIME_DAY);
    night = BriskLighting_GetTimeBlend(TIME_NIGHT);
    assert(day.blendColor == night.blendColor && day.coeff == night.coeff);
    return 0;
}
''')
    executable = tmp / 'test'
    subprocess.run(['cc', '-std=c99', '-Wall', '-Wextra', '-Werror',
                    '-I'+str(tmp), '-I'+str(ROOT / 'include'),
                    str(ROOT / 'src/brisk_lighting.c'), str(harness),
                    '-o', str(executable)], check=True)
    subprocess.run([str(executable)], check=True)
print('Environmental lighting: map selection, interior isolation, time colors, and parameter bounds passed.')
