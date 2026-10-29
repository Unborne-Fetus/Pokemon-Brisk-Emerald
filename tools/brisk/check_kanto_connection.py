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
    connection(approach,'right',west,24); connection(west,'left',approach,-24)
    connection(east,'left',cinnabar,0); connection(cinnabar,'right',east,0)
    connection(east,'right',route20,0); connection(route20,'left',east,0)
    for a,b in ((approach,west),(east,cinnabar),(east,route20)):
        for key in ('layout_version','primary_tileset','secondary_tileset'):
            assert a.layout[key] == b.layout[key], (a.name,b.name,key)
    for m in (west,east):
        assert (m.w,m.h) == (40,20)
        assert all(m.water((x,y)) for y in range(m.h) for x in range(m.w)), m.name
        assert not m.meta['object_events'] and not m.meta['warp_events']
        assert m.meta['region_map_section'] == 'MAPSEC_ORANGE_STRAIT'
    for a,b,trigger,arrival in ((west,east,{(39,y) for y in range(20)},(20,2)),
                                (east,west,{(x,0) for x in range(40)},(37,10))):
        assert {(c['x'],c['y']) for c in a.meta['coord_events']} == trigger
        assert all(c['elevation']==1 and c['var']=='VAR_TEMP_0' and c['var_value']=='0'
                   and c['script']==a.name+'_EventScript_CrossStrait' for c in a.meta['coord_events'])
        script = (ROOT/'data/maps'/a.name/'scripts.inc').read_text()
        assert f'warp {b.meta["id"]}, {arrival[0]}, {arrival[1]}' in script
        assert b.water(arrival)
        assert min(abs(arrival[0]-c['x'])+abs(arrival[1]-c['y']) for c in b.meta['coord_events']) >= 2
        assert 'setflag ' not in script and 'clearflag ' not in script and 'setvar ' not in script
    # Existing route entry -> Surf approach -> every row of the strait entrance.
    seen = approach.flood((0,y) for y in range(approach.h))
    assert all((62,y) in seen and approach.water((62,y)) for y in range(24,44)), 'Route 146 approach blocked'
    # Cinnabar's west edge is reef-enclosed, so the junction uses its existing
    # eastern sea exit. Check that it actually reaches the city and Route 20.
    seen = cinnabar.flood((cinnabar.w-1,y) for y in range(cinnabar.h) if cinnabar.water((cinnabar.w-1,y)))
    assert (15,12) in seen, 'Cinnabar Pokemon Center approach unreachable'
    assert any((0,y) in route20.flood([(1,10)]) for y in range(20)), 'Route 20 west approach blocked'
    print('PASS: Orange Strait links Route 146, Cinnabar and Route 20 both ways; Surf terrain, graphics formats, approach and loop-free crossings valid.')

if __name__ == '__main__': audit()
