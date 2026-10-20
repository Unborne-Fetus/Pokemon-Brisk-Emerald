#!/usr/bin/env python3
"""Encode existing Hisuian Avalugg pixels as the intro's native 4bpp silhouette.

No generated artwork: nearest-neighbor 2x scaling of the existing front sprite.
Palette zero is transparent, one is the black body, fourteen is the yellow eye.
"""
from pathlib import Path
import struct
from PIL import Image

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'graphics/intro/scene_3'


def main():
    sprite = Image.open(ROOT / 'graphics/pokemon/avalugg/hisui/front.png')
    assert sprite.mode == 'P' and sprite.size == (64, 64)
    pixels = sprite.load()
    # The source's four golden eye pixels use index 14; no body color shares it.
    eye = {(x, y) for y in range(64) for x in range(64) if pixels[x, y] == 14}
    assert eye == {(21, 36), (22, 36), (19, 37), (21, 37)}
    scaled = [[0 if pixels[x // 2, y // 2] == 0 else 14 if (x // 2, y // 2) in eye else 1
               for x in range(128)] for y in range(128)]
    tiles = bytearray()
    for ty in range(16):
        for tx in range(16):
            for y in range(8):
                for x in range(0, 8, 2):
                    tiles.append(scaled[ty * 8 + y][tx * 8 + x] | scaled[ty * 8 + y][tx * 8 + x + 1] << 4)
    assert len(tiles) == 8192 and not any(tiles[:32])  # Tile zero stays blank.
    tilemap = [5 << 12] * 1024
    # Center the 128x128 source canvas within the 240x160 movie window.
    for y in range(16):
        for x in range(16):
            tilemap[(y + 2) * 32 + x + 7] = (5 << 12) | (y * 16 + x)
    (OUT / 'avalugg_hisui_silhouette.4bpp').write_bytes(tiles)
    (OUT / 'avalugg_hisui_silhouette.bin').write_bytes(struct.pack('<1024H', *tilemap))
    colors = ['0 0 0'] * 16
    colors[14] = '255 246 123'
    (OUT / 'avalugg_hisui_silhouette.pal').write_text('JASC-PAL\n0100\n16\n' + '\n'.join(colors) + '\n')
    print('Encoded Hisuian Avalugg intro silhouette: 256 tiles, palette bank 5, yellow eye index 14.')


if __name__ == '__main__':
    main()
