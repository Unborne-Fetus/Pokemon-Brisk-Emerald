#!/usr/bin/env python3
"""Check reported story warps in both normal and script-selected clean layouts."""
import json
from pathlib import Path
import re
import struct

ROOT = Path(__file__).resolve().parents[1]


def main():
    layouts = {row['id']: row for row in json.loads((ROOT/'data/layouts/layouts.json').read_text())['layouts']}
    attributes = dict(re.findall(r'gMetatileAttributes_(\w+)\[\].*?"([^"]+)"',
                                (ROOT/'src/data/tilesets/metatiles.h').read_text()))
    headers = (ROOT/'src/data/tilesets/headers.h').read_text()
    maps = {row['id']: row for p in (ROOT/'data/maps').glob('*/map.json')
            for row in [json.loads(p.read_text())]}
    names = ['AquaHideout_1F'] + [f'SkyPillar_{n}F' for n in range(1,6)]
    checked = 0
    for name in names:
        data = next(m for m in maps.values() if m['name']==name)
        selected = [data['layout']]
        if data['layout']+'_CLEAN' in layouts:
            selected.append(data['layout']+'_CLEAN')
        for layout_id in selected:
            layout = layouts[layout_id]
            width, height = layout['width'], layout['height']
            blocks = struct.unpack(f'<{width*height}H', (ROOT/layout['blockdata_filepath']).read_bytes())
            sets = []
            for kind in ('primary','secondary'):
                header = re.search(r'const struct Tileset '+layout[kind+'_tileset']+r'\s*=\s*\{(.*?)\};', headers, re.S).group(1)
                attr = re.search(r'\.metatileAttributes\s*=\s*gMetatileAttributes_(\w+)', header).group(1)
                raw = (ROOT/attributes[attr]).read_bytes()
                sets.append(struct.unpack(f'<{len(raw)//2}H', raw))
            for warp in data['warp_events']:
                x,y = warp['x'], warp['y']
                assert 0<=x<width and 0<=y<height, (name,x,y)
                block = blocks[y*width+x]
                tile = block & 0x3ff
                behavior = sets[tile>=512][tile-512 if tile>=512 else tile] & 0xff
                expected = {0x6d} if name=='AquaHideout_1F' and y==27 else {0x60,0x65,0x67}
                assert behavior in expected, (layout_id,x,y,hex(behavior))
                assert not block & 0xc00, (layout_id,x,y,'collision')
                assert warp['elevation'] in (0,block>>12), (layout_id,x,y,'elevation')
                target = maps[warp['dest_map']]
                assert 0<=int(warp['dest_warp_id'])<len(target['warp_events']), (name,'destination')
                checked += 1
    print(f'PASS: {checked} Aqua Hideout/Sky Pillar warp tiles and destinations, including clean layouts.')


if __name__ == '__main__':
    main()
