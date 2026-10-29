#!/usr/bin/env python3
"""Audit the bidirectional Surf passage without changing Kanto's content."""
from collections import deque
import json
from pathlib import Path
import re
import struct

ROOT = Path(__file__).resolve().parents[2]
LAYOUTS = {x['id']: x for x in json.loads((ROOT/'data/layouts/layouts.json').read_text())['layouts']}
HEADERS = (ROOT/'src/data/tilesets/headers.h').read_text()
METATILES = (ROOT/'src/data/tilesets/metatiles.h').read_text()
ATTRS = dict(re.findall(r'const u16 (\w+)\[\] = INCBIN_U16\("([^"]+)"\)', METATILES))
WATER = {0x10, 0x12, 0x15}
LAND = {0, 2, 8, 0x17, 0x21}  # Normal ground and sand, shared by the two tileset formats.

class Map:
    def __init__(self, name):
        self.name = name
        self.meta = json.loads((ROOT/'data/maps'/name/'map.json').read_text())
        self.layout = LAYOUTS[self.meta['layout']]
        self.w, self.h = self.layout['width'], self.layout['height']
        raw = (ROOT/self.layout['blockdata_filepath']).read_bytes()
        assert len(raw) == self.w*self.h*2, name
        self.blocks = struct.unpack('<'+'H'*(len(raw)//2), raw)
        self.attrs = []
        self.frlg = self.layout['layout_version'] == 'frlg'
        for key in ('primary_tileset', 'secondary_tileset'):
            symbol = self.layout[key]
            header = re.search(r'const struct Tileset '+symbol+r'\s*=\s*\{(.*?)\};', HEADERS, re.S)[1]
            attr = re.search(r'\.metatileAttributes\s*=\s*(\w+)', header)[1]
            raw = (ROOT/ATTRS[attr]).read_bytes()
            fmt, size = ('I', 4) if self.frlg else ('H', 2)
            self.attrs.append(struct.unpack('<'+fmt*(len(raw)//size), raw))

    def block(self, p): return self.blocks[p[1]*self.w+p[0]]
    def behavior(self, p):
        t = self.block(p)&1023
        split = 640 if self.frlg else 512
        return (self.attrs[0][t] if t < split else self.attrs[1][t-split]) & (511 if self.frlg else 255)
    def water(self, p): return not self.block(p)&0xC00 and self.block(p)>>12 == 1 and self.behavior(p) in WATER
    def flood(self, starts):
        valid = {(x,y) for y in range(self.h) for x in range(self.w)
                 if not self.block((x,y))&0xC00 and self.behavior((x,y)) in WATER|LAND}
        seen = set(starts)&valid
        q = deque(seen)
        while q:
            x,y = q.popleft()
            for p in ((x-1,y),(x+1,y),(x,y-1),(x,y+1)):
                if p in valid and p not in seen: seen.add(p); q.append(p)
        return seen


def audit():
    approach, west, east, cinnabar, route20 = [Map(n) for n in
        ('Route146','OrangeStraitWest','OrangeStraitEast','CinnabarIsland_Frlg','Route20_Frlg')]
    def connection(a, direction, b, offset):
        assert {'direction':direction,'map':b.meta['id'],'offset':offset} in a.meta['connections'], (a.name,b.name)
    connection(approach,'right',west,7); connection(west,'left',approach,-7)
    connection(west,'down',east,0); connection(east,'up',west,0)
    connection(east,'left',cinnabar,0); connection(cinnabar,'right',east,0)
    connection(east,'right',route20,0); connection(route20,'left',east,0)
    for a,b in ((approach,west),(east,cinnabar),(east,route20)):
        for key in ('layout_version','primary_tileset','secondary_tileset'):
            assert a.layout[key] == b.layout[key], (a.name,b.name,key)
    for m in (west,east):
        assert (m.w,m.h) == (40,52 if m is west else 20)
        assert not m.meta['object_events'] and not m.meta['warp_events'] and not m.meta['coord_events']
    # The east half must select the actual Kanto Town Map. Its map section
    # is Route 20 because that section has a valid coastal Town Map position.
    # The west half retains the Orange Strait name on the Hoenn map.
    assert west.meta['region_map_section'] == 'MAPSEC_ORANGE_STRAIT'
    assert east.meta['region_map_section'] == 'MAPSEC_ROUTE_20'
    assert west.meta['region'] == 'REGION_HOENN'
    assert east.meta['region'] == 'REGION_KANTO'
    sections = [entry['id'] for entry in json.loads((ROOT/'src/data/region_map/region_map_sections.json').read_text())['map_sections']]
    assert sections.index('MAPSEC_PALLET_TOWN') <= sections.index('MAPSEC_ROUTE_20') < sections.index('MAPSEC_SPECIAL_AREA')
    # Keep the crossing scripts active while original Kanto story scripts stay guarded.
    converter = (ROOT/'tools/mapjson/mapjson.cpp').read_text()
    assert 'json_to_string(map_data, "id", true) != "MAP_ORANGE_STRAIT_EAST"' in converter
    # Incoming border blocks are briefly interpreted using the current map's
    # primary tileset before the camera transition loads the neighboring map.
    for x in range(west.w):
        block=west.block((x,west.h-1))
        if west.water((x,west.h-1)):
            tile=block&1023
            assert tile<640 and (east.attrs[0][tile]&511) in WATER, 'Return seam decodes as land in Kanto'
    # Seamless north/south handoff: a continuous swimmable corridor joins
    # Route 146's shifted approach to both edges of the Kanto junction.
    seen = west.flood((0,y) for y in range(west.h) if west.water((0,y)))
    assert any((x,west.h-1) in seen and east.water((x,0)) for x in range(40)), 'Regional seam blocked'
    seen = east.flood((x,0) for x in range(40) if east.water((x,0)))
    assert any((0,y) in seen and cinnabar.water((cinnabar.w-1,y)) for y in range(20)), 'Cinnabar seam blocked'
    assert any((39,y) in seen and route20.water((0,y)) for y in range(20)), 'Route 20 seam blocked'
    # Safeguard the separate Kanto battle-only compiler path.
    converter = (ROOT/'tools/mapjson/mapjson.cpp').read_text()
    for requirement in ('rocketBase', 'rivalFinale', 'storyActor', 'brisk_kanto_postgame.json',
                        'access["coord_events"] = Json::array()'):
        assert requirement in converter, requirement
    access = (ROOT/'data/scripts/brisk_kanto_access.inc').read_text()
    assert not any(chr(92) * 2 + esc in access for esc in ('p', 'n')), 'Doubled Kanto escape breaks event_scripts.o'
    assert 'trainerbattle_' not in access
    assert 'BriskKantoAccessOnly_EventScript_ChallengePending' in access
    # Existing route entry -> Surf approach -> strait entrance of the strait entrance.
    seen = approach.flood((0,y) for y in range(approach.h))
    assert all((62,y) in seen and approach.water((62,y)) for y in range(24,44)), 'Route 146 approach blocked'
    # Cinnabar's west edge is reef-enclosed, so the junction uses its existing
    # eastern sea exit. Check that it actually reaches the city and Route 20.
    seen = cinnabar.flood((cinnabar.w-1,y) for y in range(cinnabar.h) if cinnabar.water((cinnabar.w-1,y)))
    assert (15,12) in seen, 'Cinnabar Pokemon Center approach unreachable'
    assert any((0,y) in route20.flood([(1,10)]) for y in range(20)), 'Route 20 west approach blocked'
    print('PASS: Orange Strait Surf crossings, Kanto Town Map handoff and story guards valid.')

if __name__ == '__main__': audit()
