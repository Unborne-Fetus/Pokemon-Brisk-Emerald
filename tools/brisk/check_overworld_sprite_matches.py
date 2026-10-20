#!/usr/bin/env python3
"""Validate sprite donors against active species data after make KEEP_TEMPS=1."""
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[2]


def main():
    path = ROOT / 'build/emerald/src/pokemon.i'
    if not path.exists():
        raise SystemExit('Run make KEEP_TEMPS=1 first to generate active species data.')
    text = path.read_text().split('const struct SpeciesInfo gSpeciesInfo[] =', 1)[1].split('\n};', 1)[0]
    markers = list(re.finditer(r'\[(SPECIES_\w+)\]\s*=', text))
    rows = {marker[1][8:]: text[marker.end():markers[i + 1].start() if i + 1 < len(markers) else len(text)]
            for i, marker in enumerate(markers)}
    matches = json.loads((ROOT / 'docs/brisk/overworld-sprite-matches.json').read_text())
    def graphics(row):
        fields = re.search(r'\.overworldData\s*=\s*\{(.*?)\}', row, re.S)
        if not fields:
            return None
        images = re.search(r'\.images\s*=\s*(\w+)', fields[1])
        return (images[1], fields[1]) if images else None
    for species, donor in matches.items():
        actual, expected = graphics(rows[species]), graphics(rows[donor])
        assert actual and expected and actual == expected, f'{species}: sprite geometry/animation does not match {donor}'
        for field in ['overworldPalette', 'overworldShinyPalette']:
            a = re.search(r'\.' + field + r'\s*=\s*(\w+)', rows[species])
            b = re.search(r'\.' + field + r'\s*=\s*(\w+)', rows[donor])
            assert a and b and a[1] == b[1], f'{species}: mismatched {field}'
    missing = []
    excluded = 0
    for species, row in rows.items():
        if not re.search(r'\.baseHP\s*=\s*[1-9]', row):
            continue
        if re.search(r'\.(?:isMegaEvolution|isGigantamax)\s*=\s*1', row):
            excluded += 1
            continue
        data = graphics(row)
        if not data or data[0] == 'sPicTable_Substitute':
            missing.append(species)
    assert not missing, 'Missing non-Mega/Gigantamax sprites: ' + ', '.join(missing)
    print(f'PASS: {len(matches)} sprite matches have matching geometry, animation and normal/shiny palettes; no enabled non-Mega/Gigantamax species use missing/Substitute overworld graphics ({excluded} battle forms excluded).')


if __name__ == '__main__':
    main()
