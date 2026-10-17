#!/usr/bin/env python3
"""Apply standard species' shiny colors to CDRX73 Halloween art without changing pixels.

Each list maps Halloween palette slots to the base species' shiny palette slots.
Mappings follow the artwork's materials (cloak, body, eyes, flame, wood, etc.);
copying raw palettes would scramble colors because the slot orders differ.
Run with --check to validate the installed palettes and both 80px previews.
"""
import argparse
from pathlib import Path
from PIL import Image

ROOT = Path(__file__).resolve().parents[2]
MAPPINGS = {
    'litwick':     [0,1,2,3,7,5,10,6,10,1,8,11,4,1,2,10],
    'lampent':     [0,3,1,10,2,6,10,2,8,9,3,9,1,11,10,1],
    'chandelure':  [0,6,3,4,5,1,5,4,9,8,3,9,7,1,2,3],
    'sinistea':    [0,8,5,11,7,6,13,11,2,9,7,5,6,1,3,9],
    'polteageist': [0,10,6,7,8,6,2,9,7,10,2,11,2,7,3,2],
    'ponyta':     [0,9,11,5,6,10,7,4,11,3,1,1,8,5,9,1],
    'rapidash':   [0,7,9,10,11,1,7,6,8,11,5,4,3,12,2,1],
    'tangela':    [0,2,1,3,14,1,7,6,6,8,7,4,14,14,14,14],
    'tangrowth':  [0,6,7,8,4,5,9,10,5,1,3,2,2,2,1,4],
    'timburr':    [0,14,13,2,1,6,3,1,10,12,6,2,15,13,1,9],
    'gurdurr':    [0,2,3,4,1,6,7,5,6,7,6,8,6,8,1,8],
    'conkeldurr': [0,8,9,8,4,9,1,3,10,2,3,4,1,2,1,2],
    'emolga':     [0,1,2,3,9,8,2,7,6,11,5,6,9,6,1,15],
    'snorunt':    [0,4,3,4,1,4,1,2,5,7,10,7,1,8,13,15],
    'glalie':     [0,5,6,6,7,1,5,2,3,4,9,3,7,10,4,8],
}

def read_palette(path):
    return [tuple(map(int, line.split())) for line in path.read_text().splitlines()[3:19]]

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    for species, mapping in MAPPINGS.items():
        folder = ROOT / 'graphics/pokemon/cdrx73' / (species + '_halloween')
        base = read_palette(ROOT / 'graphics/pokemon' / species / 'shiny.pal')
        normal = read_palette(folder / 'normal.pal')
        colors = [normal[0]] + [tuple(c // 8 * 8 for c in base[i]) for i in mapping[1:]]
        text = 'JASC-PAL\n0100\n16\n' + ''.join(' '.join(map(str, rgb)) + '\n' for rgb in colors)
        if args.check:
            assert read_palette(folder / 'shiny.pal') == colors, species
        else:
            (folder / 'shiny.pal').write_text(text)
        for side in ('front', 'back'):
            normal_image = Image.open(folder / (side + '_normal.png'))
            assert normal_image.mode == 'P' and normal_image.size == (80, 80)
            shiny = normal_image.copy()
            shiny.putpalette([c for rgb in colors for c in rgb] + [0] * 720)
            path = folder / (side + '_shiny.png')
            if args.check:
                installed = Image.open(path)
                assert installed.mode == 'P' and installed.size == (80, 80), species
                assert installed.tobytes() == normal_image.tobytes(), species
                assert installed.getpalette()[:48] == shiny.getpalette()[:48], species
                assert installed.info.get('transparency') == normal_image.info.get('transparency'), species
            else:
                shiny.save(path)
        assert colors != normal, species
    print(f'{len(MAPPINGS)} Halloween shiny palettes and 30 sprite previews ' + ('verified' if args.check else 'updated'))

if __name__ == '__main__':
    main()
