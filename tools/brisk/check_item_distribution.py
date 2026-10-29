#!/usr/bin/env python3
"""Audit the static Mega Stone/mint pickups, terrain, persistence and shop stock."""
from collections import Counter, deque
from functools import lru_cache
import json
from pathlib import Path
import re
import struct

ROOT = Path(__file__).resolve().parents[2]
MANIFEST = ROOT / 'data/brisk_item_distribution.json'
LAYOUTS = {x['id']:x for x in json.loads((ROOT/'data/layouts/layouts.json').read_text())['layouts']}
HEADERS = (ROOT/'src/data/tilesets/headers.h').read_text()
METATILES = (ROOT/'src/data/tilesets/metatiles.h').read_text()
ATTR_PATHS = dict(re.findall(r'const u16 (gMetatileAttributes_\w+)\[\] = INCBIN_U16\("([^"]+)"\)', METATILES))
BEHAVIORS = re.findall(r'^\s*(MB_\w+)\s*,?\s*$', re.sub(r'//[^\n]*','',(ROOT/'include/constants/metatile_behaviors.h').read_text()),re.M)
SAFE_BEHAVIORS = {BEHAVIORS.index(n) for n in ('MB_NORMAL','MB_TALL_GRASS','MB_LONG_GRASS','MB_DEEP_SAND','MB_SHORT_GRASS','MB_CAVE','MB_NO_RUNNING','MB_INDOOR_ENCOUNTER','MB_MOUNTAIN_TOP','MB_ASHGRASS','MB_SAND','MB_PUDDLE') if n in BEHAVIORS}


def custom_stones():
    return set(re.findall(r'\[(ITEM_\w+)\]',(ROOT/'src/data/custom_mega_stones.h').read_text())) | {'ITEM_DRAGONINITE_CDRX'}


def stock(path,label):
    text=(ROOT/path).read_text()
    block=re.search(r'^'+re.escape(label)+r':\n(.*?)^\s*pokemartlistend',text,re.M|re.S)
    assert block, f'Missing shop table {label}'
    return re.findall(r'\.2byte\s+(ITEM_\w+)',block[1])


@lru_cache(None)
def attributes(symbol):
    m=re.search(r'const struct Tileset '+symbol+r'\s*=\s*\{(.*?)\};',HEADERS,re.S)
    assert m,symbol
    name=re.search(r'\.metatileAttributes\s*=\s*(\w+)',m[1])[1]
    data=(ROOT/ATTR_PATHS[name]).read_bytes()
    return struct.unpack('<'+'H'*(len(data)//2),data)


class Terrain:
    def __init__(self,name):
        self.map=json.loads((ROOT/'data/maps'/name/'map.json').read_text())
        self.layout=LAYOUTS[self.map['layout']]
        self.w,self.h=self.layout['width'],self.layout['height']
        data=(ROOT/self.layout['blockdata_filepath']).read_bytes()
        self.blocks=struct.unpack('<'+'H'*(len(data)//2),data)
        self.attrs=[attributes(self.layout[k]) for k in ('primary_tileset','secondary_tileset')]
        self.floor=set()
        for y in range(self.h):
            for x in range(self.w):
                block=self.block((x,y));tile=block&1023
                group,index=(0,tile) if tile<512 else (1,tile-512)
                if not block&0xC00 and index<len(self.attrs[group]) and self.attrs[group][index]&255 in SAFE_BEHAVIORS:
                    self.floor.add((x,y))

    def block(self,p):return self.blocks[p[1]*self.w+p[0]]
    def elevation(self,p):return self.block(p)>>12
    def neighbors(self,p):
        x,y=p
        return [(x-1,y),(x+1,y),(x,y-1),(x,y+1)]
    def flood(self,seeds,blocked=()):
        blocked=set(blocked);seen=set(seeds)&self.floor-blocked;q=deque(seen)
        while q:
            p=q.popleft()
            for n in self.neighbors(p):
                if n in self.floor and n not in seen and n not in blocked and self.elevation(n) in (0,self.elevation(p)):
                    seen.add(n);q.append(n)
        return seen
    def objects(self):return self.map['object_events']
    def occupied(self):return {(o['x'],o['y']) for o in self.objects()}
    def anchors(self):
        original=[o for o in self.objects() if not o.get('flag','').startswith('FLAG_ITEM_BRISK_')]
        anchors=[(o['x'],o['y']) for o in original]+[(o['x'],o['y']) for o in self.map['warp_events']]
        seeds=[]
        for p in anchors:seeds.extend([p]+self.neighbors(p))
        if self.map.get('connections'):
            seeds.extend((x,y) for x,y in self.floor if x in (0,self.w-1) or y in (0,self.h-1))
        return seeds


def audit():
    entries=json.loads(MANIFEST.read_text())['placements']
    official=set(stock('data/maps/LilycoveCity_DepartmentStore_4F/scripts.inc','LilycoveCity_DepartmentStore_4F_Pokemart_TOPRIGHT'))
    custom=custom_stones()
    assert not official&custom
    counts=Counter(e['kind'] for e in entries)
    assert counts=={'official':28,'custom':12,'mint':42},counts
    assert len({e['flag'] for e in entries})==len(entries),'Duplicate pickup flag'
    mint_stock=stock('data/maps/LilycoveCity_DepartmentStore_3F/scripts.inc','LilycoveCity_DepartmentStore_3F_Pokemart_Vitamins')
    assert Counter(e['item'] for e in entries if e['kind']=='mint')==Counter({i:2 for i in mint_stock if i.endswith('_MINT')})
    assert set(stock('data/maps/LilycoveCity_DepartmentStore_4F/scripts.inc','LilycoveCity_DepartmentStore_4F_Pokemart_CustomMegaStones'))==custom
    flags=(ROOT/'include/constants/flags.h').read_text();seen_values=set()
    terrain={name:Terrain(name) for name in {e['map'] for e in entries}}
    for e in entries:
        t=terrain[e['map']];p=(e['x'],e['y'])
        assert e['item'] in (official if e['kind']=='official' else custom if e['kind']=='custom' else set(mint_stock)),e
        matching=[o for o in t.objects() if o.get('flag')==e['flag']]
        assert len(matching)==1,(e,'missing/duplicate pickup')
        o=matching[0]
        assert o['script']=='Common_EventScript_FindItem' and o['graphics_id']=='OBJ_EVENT_GFX_ITEM_BALL' and o['trainer_sight_or_berry_tree_id']==e['item'],e
        assert o['movement_range_x']==0 and o['trainer_type']=='TRAINER_TYPE_NONE',e
        assert (o['x'],o['y'],o['elevation'])==(e['x'],e['y'],e['elevation']),e
        assert p in t.floor and o['elevation']==t.elevation(p),(e,'bad terrain')
        assert sum((x['x'],x['y'])==p for x in t.objects())==1,(e,'object overlap')
        assert p in t.flood(t.anchors()),(e,'unreachable')
        adjacent=[n for n in t.neighbors(p) if n in t.floor and t.elevation(n)==t.elevation(p)]
        assert len(adjacent)>=3,(e,'narrow passage')
        pickups={(x['x'],x['y']) for x in t.objects() if x.get('flag','').startswith('FLAG_ITEM_BRISK_')}
        assert set(adjacent)<=t.flood(adjacent[:1],blocked=pickups),(e,'pickups block route')
        assert not any(abs(w['x']-p[0])+abs(w['y']-p[1])<=2 for w in t.map['warp_events']+t.map['coord_events']),(e,'blocks warp/trigger')
        value=int(re.search(r'^#define '+e['flag']+r'\s+(0x[0-9A-Fa-f]+)',flags,re.M)[1],16)
        assert value>=32 and value<0x500 and value not in seen_values,(e,'unsafe flag')
        aliases=re.findall(r'^#define (\w+)\s+0x0*'+format(value,'X')+r'\b',flags,re.M|re.I)
        assert aliases==[e['flag']],(e,'flag alias conflict',aliases)
        seen_values.add(value)
        active=sum(abs(x['x']-p[0])<=9 and abs(x['y']-p[1])<=7 for x in t.objects())
        assert active<=12,(e,'object density',active)
    print(f'PASS: 40 Mega Stones (28 official / 12 custom = 70/30), 42 mints, {len(terrain)} maps; reachable ground, unique persistent flags, clear paths and full Lilycove stock.')

if __name__=='__main__':audit()
