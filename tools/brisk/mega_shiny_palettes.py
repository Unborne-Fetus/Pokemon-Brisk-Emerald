#!/usr/bin/env python3
"""Adapt base-form shiny colors to custom Mega palette indexes."""
import json
import math
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


def palette(path):
    return [tuple(map(int, line.split())) for line in path.read_text().splitlines()[3:19]]


def lab(rgb):
    rgb = [v / 3294.6 if v <= 10.31475 else ((v / 255 + .055) / 1.055) ** 2.4 for v in rgb]
    xyz = [sum(a * b for a, b in zip(row, rgb)) / white for row, white in zip(
        ((.4124564, .3575761, .1804375), (.2126729, .7151522, .0721750), (.0193339, .1191920, .9503041)),
        (.95047, 1, 1.08883))]
    x, y, z = [v ** (1 / 3) if v > .008856 else 7.787 * v + 16 / 116 for v in xyz]
    return 116 * y - 16, 500 * (x - y), 200 * (y - z)


def main():
    graphics = ROOT / 'src/data/graphics/custom_megas.h'
    definitions = graphics.read_text()
    info = ROOT / 'src/data/pokemon/species_info/custom_megas.h'
    species_info = info.read_text()
    for filename in ('manifest.json', 'added_manifest.json'):
        manifest_path = ROOT / 'graphics/pokemon/custom_megas' / filename
        rows = json.loads(manifest_path.read_text())
        for row in rows:
            if not (row['shiny'].startswith('Uses supplied normal') or row['shiny'].startswith('Normal palette;')
                    or row['shiny'].startswith('Base-form shiny')):
                continue
            asset = ROOT / row['asset_path']
            base_folder = ROOT / 'graphics/pokemon' / row['base_species'][8:].lower()
            base = palette(base_folder / 'normal.pal')
            shiny = palette(base_folder / 'shiny.pal')
            source = palette(asset / 'normal.pal')
            base_lab = list(map(lab, base))
            result = [source[0]]  # Preserve the transparent slot.
            for color in source[1:]:
                value = lab(color)
                slot = min(range(1, 16), key=lambda i: math.dist(value, base_lab[i]))
                # Apply the base-form color change while retaining the artwork's shading.
                result.append(tuple(max(0, min(248, round((color[c] + shiny[slot][c] - base[slot][c]) / 8) * 8)) for c in range(3)))
            (asset / 'shiny.pal').write_text('JASC-PAL\n0100\n16\n' + '\n'.join(' '.join(map(str, c)) for c in result) + '\n')
            relative = row['asset_path']
            match = re.search(r'const u16 gMonPalette_(\w+)\[\] = INCGFX_U16\("' + re.escape(relative) + r'/normal.pal"', definitions)
            symbol = match[1]
            declaration = f'const u16 gMonShinyPalette_{symbol}[] = INCGFX_U16("{relative}/shiny.pal", ".gbapal");'
            if declaration not in definitions:
                definitions += declaration + '\n'
            species_info = species_info.replace(f'.shinyPalette = gMonPalette_{symbol},', f'.shinyPalette = gMonShinyPalette_{symbol},')
            row['shiny'] = 'Base-form shiny colors adapted to Mega palette indexes'
            row['shiny_palette_source'] = str((base_folder / 'shiny.pal').relative_to(ROOT))
            print(row['species'])
        manifest_path.write_text(json.dumps(rows, indent=2) + '\n')
    graphics.write_text(definitions)
    info.write_text(species_info)


if __name__ == '__main__':
    main()
