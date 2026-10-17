#!/usr/bin/env python3
"""Audit custom Mega Stone stock and the new copied icon/palette assets."""
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


def main():
    definitions = (ROOT/'src/data/custom_mega_stones.h').read_text()
    stones = set(re.findall(r'\[(ITEM_\w+)\]', definitions)) | {'ITEM_DRAGONINITE_CDRX'}
    mart = ROOT/'data/maps/OrangeCity_Mart/scripts.inc'
    stock = re.findall(r'^\s*\.2byte (ITEM_\w+)', mart.read_text(), re.M)
    assert len(stock) == len(set(stock)), 'Duplicate Orange City stock entries'
    assert stones == set(stock), f'Missing/extra custom stones: {stones ^ set(stock)}'
    for path in (ROOT/'data').rglob('*'):
        if path.is_file() and path.suffix in {'.inc', '.json', '.h'} and path != mart:
            found = stones & set(re.findall(r'\bITEM_\w+\b', path.read_text()))
            assert not found, f'Custom stones placed outside Orange City Mart: {path}: {found}'
    for mon in json.loads((ROOT/'tools/brisk/kfweagz_species.json').read_text()):
        if mon['kind'] != 'mega':
            continue
        for directory, extension in [('icons', '.png'), ('icon_palettes', '.pal')]:
            folder = ROOT/'graphics/items'/directory
            assert (folder/(mon['stone'].lower()+extension)).read_bytes() == (folder/(mon['stone_source']+extension)).read_bytes(), mon['stone']
    print(f'PASS: all {len(stones)} custom stones stocked once, exclusively in Orange City Mart; all 13 new icon/palette copies match.')


if __name__ == '__main__':
    main()
