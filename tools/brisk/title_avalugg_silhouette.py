#!/usr/bin/env python3
"""Encode Hisuian Avalugg for Emerald's PRESS START screen, preserving its sky.

Uses existing game pixels, integer nearest-neighbor scaling, and native 4bpp
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
    sprite = Image.open(ROOT / 'graphics/pokemon/avalugg/hisui/front.png')
    assert sprite.mode == 'P' and sprite.size == (64, 64)
    points = [(x, y) for y in range(64) for x in range(64) if sprite.getpixel((x, y))]
    left, top = min(x for x, y in points), min(y for x, y in points)
    right, bottom = max(x for x, y in points) + 1, max(y for x, y in points) + 1
    scale = 3
    x0, y0 = (240 - (right-left)*scale)//2, 160-(bottom-top)*scale
    assert y0 >= 0
    eye_count = 0
    for x, y in points:
        color = 15 if sprite.getpixel((x, y)) == 14 else 11
        eye_count += color == 15
        for dy in range(scale):
            for dx in range(scale):
                canvas[y0+(y-top)*scale+dy][x0+(x-left)*scale+dx] = color
    assert eye_count == 4
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
    print(f'Encoded PRESS START Avalugg: {len(tiles)} tiles, full silhouette within viewport, 36 yellow-eye pixels; original sky retained.')


if __name__ == '__main__':
    main()
