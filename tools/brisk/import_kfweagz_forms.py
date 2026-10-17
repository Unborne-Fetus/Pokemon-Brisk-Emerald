#!/usr/bin/env python3
"""Rebuild Brisk's supplied KFWeagz battle art (no generated replacement art).

python tools/brisk/import_kfweagz_forms.py [directory containing original PNGs]
The default source directory retains the exact uploaded sheets for attribution
and repeatable crops. Normal/shiny pairs share the GBA's 15 visible indices.
"""
import argparse
import json
from collections import Counter
from pathlib import Path
from PIL import Image
from import_phoenix_fakemon import gba, distance, indexed, pack80

ROOT = Path(__file__).resolve().parents[2]
# Rectangles are exclusive at right/bottom, inside the black sheet grid.
# Each row: folder, source prefix, original enlargement, NF/SF/NB/SB crops.
STANDARD = [(2, 2, 190, 190), (192, 2, 380, 190),
            (2, 192, 190, 380), (192, 192, 380, 380)]
TRIPLE = [(6, 6, 300, 300), (306, 6, 600, 300),
          (6, 306, 300, 600), (306, 306, 600, 600)]
SHEETS = [
    ('drifblim_mega', 'dhw5723', 2,
     [(4, 4, 200, 200), (204, 4, 400, 200), (4, 204, 200, 400), (204, 204, 400, 400)]),
    ('lapras_mega', 'dhw4bbq', 2,
     [(500, 4, 744, 252), (500, 508, 744, 756), (500, 256, 744, 504), (500, 760, 744, 1008)]),
    ('seviper_mega', 'dhwoudr', 3, TRIPLE),
    ('hydreigon_mega', 'djhq9r3', 1,
     [(317, 2, 420, 99), (317, 200, 420, 297), (317, 101, 420, 198), (317, 299, 420, 396)]),
    ('regigigas_mega', 'djqxex0', 4,
     [(4, 4, 474, 434), (478, 4, 948, 434), (4, 438, 474, 868), (478, 438, 948, 868)]),
    ('noivern_mega', 'mega_noivern', 4,
     [(3, 3, 321, 281), (325, 3, 643, 281), (3, 285, 321, 564), (325, 285, 643, 564)]),
    ('iron_furnace', 'dki1mam', 2,
     [(2, 2, 190, 202), (192, 2, 380, 202), (2, 204, 190, 404), (192, 204, 380, 404)]),
    ('steam_shell', 'dkgciql', 2, STANDARD),
    ('kingdra_mega', 'dkgwqi5', 2, STANDARD),
    ('fearow_mega', 'dkig96i', 2, STANDARD),
    ('togekiss_mega', 'dkig6tc', 2, STANDARD),
    ('volcarona_mega', 'dkjif1v', 2, STANDARD),
    ('zangoose_mega', 'dmiyyys', 2, [STANDARD[i] for i in (0, 2, 1, 3)]),
    ('quagsire_mega', 'dhwxyq7', 3, TRIPLE),
    ('xatu_mega', 'dhwu1tg', 3, TRIPLE),
]


def normalize(frames, enlargement):
    # Some web exports contain almost transparent resampling halos. Threshold
    # alpha before cropping; otherwise those halos become opaque GBA pixels.
    for im in frames:
        im.putalpha(im.getchannel('A').point(lambda a: 255 if a >= 128 else 0))
    pairs = []
    for a, b in ((0, 1), (2, 3)):
        boxes = [frames[i].getbbox() for i in (a, b)]
        assert all(boxes), 'Empty source crop'
        box = (min(v[0] for v in boxes), min(v[1] for v in boxes),
               max(v[2] for v in boxes), max(v[3] for v in boxes))
        pairs.append((a, b, box))
    # Use the same scale for both views and both palettes; fit large designs
    # rather than clipping feet, wings, tails or the supplied back view.
    scale = min(1 / enlargement, 80 / max(max(box[2]-box[0], box[3]-box[1]) for _, _, box in pairs))
    result = [None] * 4
    for a, b, box in pairs:
        size = (max(1, round((box[2]-box[0])*scale)), max(1, round((box[3]-box[1])*scale)))
        for i in (a, b):
            sprite = frames[i].crop(box).resize(size, Image.Resampling.NEAREST)
            result[i] = Image.new('RGBA', (80, 80))
            result[i].paste(sprite, ((80-size[0])//2, 80-size[1]))
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('source', nargs='?', type=Path, default=ROOT/'resources/kfweagz')
    args = parser.parse_args()
    report = {}
    for name, stem, enlargement, boxes in SHEETS:
        paths = list(args.source.glob(stem+'*.png'))
        if len(paths) != 1:
            raise ValueError(f'Expected one original sheet for {stem}: {paths}')
        source = Image.open(paths[0]).convert('RGBA')
        frames = normalize([source.crop(box) for box in boxes], enlargement)
        pairs = Counter((gba(n), gba(s)) for a,b in ((0,1),(2,3))
                        for n,s in zip(frames[a].getdata(), frames[b].getdata()) if n[3])
        selected = [p for p, _ in pairs.most_common(15)]
        palettes = [[(0,0,0)] + [p[i] for p in selected] + [(0,0,0)]*(15-len(selected)) for i in (0,1)]
        def match(pair):
            return 1 + min(range(len(selected)), key=lambda j:
                           4*distance(pair[0], selected[j][0]) + distance(pair[1], selected[j][1]))
        folder = ROOT/'graphics/pokemon/kfweagz'/name
        folder.mkdir(parents=True, exist_ok=True)
        for a,b,label in ((0,1,'front'), (2,3,'back')):
            data = [0 if not n[3] else match((gba(n), gba(s)))
                    for n,s in zip(frames[a].getdata(), frames[b].getdata())]
            pic = indexed(data, (80,80), palettes[0])
            pic.save(folder/(label+'_80.png'))
            pack80(pic).save(folder/(label+'_tiles.png'))
            pic.resize((64,64), Image.Resampling.NEAREST).save(folder/(label+'.png'))
            indexed(data, (80,80), palettes[1]).save(folder/(label+'_shiny_80.png'))
        for label, pal in zip(('normal','shiny'), palettes):
            (folder/(label+'.pal')).write_text('JASC-PAL\n0100\n16\n' +
                '\n'.join(' '.join(map(str,p)) for p in pal) + '\n')
        report[name] = dict(source=paths[0].name, crops=boxes, enlargement=enlargement,
                            native_size=80, palette_pairs=len(pairs), shiny_supplied=True)
    (ROOT/'tools/brisk/kfweagz_sprite_import.json').write_text(json.dumps(report, indent=2)+'\n')
    print(f'Imported {len(report)} supplied normal/shiny front/back sets')


if __name__ == '__main__':
    main()
