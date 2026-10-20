#!/usr/bin/env python3
"""Encode Hisuian Avalugg for Emerald's PRESS START screen, preserving its sky.

Uses the front-facing title mask, integer nearest-neighbor scaling, and native 4bpp
BG tiles. Bank 15 is private to this layer; clouds retain original bank 14.
"""
from collections import Counter
from pathlib import Path
import struct
from PIL import Image

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'graphics/title_screen'


def main():
    old = Image.open(OUT / 'rayquaza.png')
    old_map = struct.unpack('<1024H', (OUT / 'rayquaza.bin').read_bytes())
    def tile(value):
        index = value & 1023
        data = [[old.getpixel(((index % 16) * 8 + x, (index // 16) * 8 + y)) for x in range(8)] for y in range(8)]
        if value & 1024:
            data = [row[::-1] for row in data]
        if value & 2048:
            data.reverse()
        return data
    canvas = [[1] * 256 for _ in range(256)]
    # Each visible tile row has untouched sky tiles. Retain their dithering and
    # original palette indices rather than tinting the shared logo/cloud colors.
    for ty in range(20):
        candidates = [v for v in old_map[ty*32:ty*32+30] if v >> 12 == 14
                      and all(c in {1, 3, 4, 5, 6, 7, 8, 9, 10} for row in tile(v) for c in row)]
        assert candidates, f'No original sky sample in tile row {ty}'
        sky = tile(Counter(candidates).most_common(1)[0][0])
        for y in range(8):
            canvas[ty*8+y] = sky[y] * 32
    sprite = Image.open(OUT / 'avalugg_hisui_front_mask.png')
    assert sprite.mode == 'P'
    width, height = sprite.size
    assert set(sprite.get_flattened_data()) == {0, 1, 2}
    scale = 3
    x0, y0 = (240 - width*scale)//2, 160-height*scale
    assert x0 >= 0 and y0 >= 0
    eyes = []
    for y in range(height):
        for x in range(width):
            value = sprite.getpixel((x, y))
            if not value:
                continue
            color = 15 if value == 2 else 11
            if value == 2:
                eyes.append((x, y))
            for dy in range(scale):
                for dx in range(scale):
                    canvas[y0+y*scale+dy][x0+x*scale+dx] = color
    assert any(x < width//2 for x, y in eyes)
    assert any(x > width//2 for x, y in eyes)
    assert all(80 <= y0+y*scale < 104 for x, y in eyes)
    # Deduplicate so this BG stays inside its 16 KiB character block and cannot
    # overwrite the title's clouds in the next block.
    tiles, lookup, tilemap = [], {}, []
    for ty in range(32):
        for tx in range(32):
            data = bytes(canvas[ty*8+y][tx*8+x] | canvas[ty*8+y][tx*8+x+1] << 4
                         for y in range(8) for x in range(0, 8, 2))
            if data not in lookup:
                lookup[data] = len(tiles)
                tiles.append(data)
            tilemap.append((15 << 12) | lookup[data])
    assert len(tiles) <= 512
    (OUT / 'avalugg_hisui.4bpp').write_bytes(b''.join(tiles))
    (OUT / 'avalugg_hisui.bin').write_bytes(struct.pack('<1024H', *tilemap))
    palette = (OUT / 'rayquaza_and_clouds.pal').read_text().splitlines()[3:19]
    palette[11], palette[15] = '0 0 0', '255 246 123'
    (OUT / 'avalugg_hisui.pal').write_text('JASC-PAL\n0100\n16\n'+'\n'.join(palette)+'\n')
    print(f'Encoded PRESS START Avalugg: {len(tiles)} tiles, full silhouette within viewport, {len(eyes)*scale*scale} yellow-eye pixels; original sky retained.')


if __name__ == '__main__':
    main()
