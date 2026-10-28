#!/usr/bin/env python3
"""Audit Brisk's default legendary acquisition sources and static encounter tiles.

Checks map data, not a substitute for playing through story/HM puzzles. Local
flood fills start at existing warps/connection borders; dynamic battle scenes
and puzzle-controlled native encounters retain their original scripts.
"""
from collections import defaultdict, deque
import argparse
import json
from pathlib import Path
import re
import struct

ROOT = Path(__file__).resolve().parents[2]
LEGENDS = """ARTICUNO ZAPDOS MOLTRES MEWTWO MEW RAIKOU ENTEI SUICUNE LUGIA HO_OH CELEBI
REGIROCK REGICE REGISTEEL LATIAS LATIOS KYOGRE GROUDON RAYQUAZA JIRACHI DEOXYS
UXIE MESPRIT AZELF DIALGA PALKIA HEATRAN REGIGIGAS GIRATINA CRESSELIA PHIONE
MANAPHY DARKRAI SHAYMIN ARCEUS VICTINI COBALION TERRAKION VIRIZION TORNADUS
THUNDURUS RESHIRAM ZEKROM LANDORUS KYUREM KELDEO MELOETTA GENESECT XERNEAS
YVELTAL ZYGARDE DIANCIE HOOPA VOLCANION TYPE_NULL SILVALLY TAPU_KOKO TAPU_LELE
TAPU_BULU TAPU_FINI COSMOG COSMOEM SOLGALEO LUNALA NIHILEGO BUZZWOLE PHEROMOSA
XURKITREE CELESTEELA KARTANA GUZZLORD NECROZMA MAGEARNA MARSHADOW POIPOLE
NAGANADEL STAKATAKA BLACEPHALON ZERAORA MELTAN MELMETAL ZACIAN ZAMAZENTA
ETERNATUS KUBFU URSHIFU ZARUDE REGIELEKI REGIDRAGO GLASTRIER SPECTRIER CALYREX
ENAMORUS WO_CHIEN CHIEN_PAO TING_LU CHI_YU KORAIDON MIRAIDON OKIDOGI MUNKIDORI
FEZANDIPITI OGERPON TERAPAGOS PECHARUNT""".split()


def family(species):
    return next((name for name in sorted(LEGENDS, key=len, reverse=True)
                 if species == name or species.startswith(name + '_')), None)


def load_maps():
    layouts = {row['id']: row for row in json.loads((ROOT / 'data/layouts/layouts.json').read_text())['layouts']}
    maps = {}
    for path in (ROOT / 'data/maps').glob('*/map.json'):
        data = json.loads(path.read_text())
        maps[data['id']] = data
    return layouts, maps


def grid(data, layouts, override=None):
    layout = layouts[override or data['layout']]
    raw = (ROOT / layout['blockdata_filepath']).read_bytes()
    return layout['width'], layout['height'], struct.unpack('<' + 'H' * (len(raw) // 2), raw)


def reachable(data, layouts, maps, override=None):
    width, height, blocks = grid(data, layouts, override)
    def walkable(x, y):
        return 0 <= x < width and 0 <= y < height and not blocks[y * width + x] & 0xC00
    seeds = set()
    for warp in data['warp_events']:
        for x, y in ((warp['x'], warp['y']), (warp['x'], warp['y'] + 1)):
            if walkable(x, y):
                seeds.add((x, y))
    for connection in data.get('connections') or []:
        other_width, other_height, _ = grid(maps[connection['map']], layouts)
        direction, offset = connection['direction'], connection['offset']
        if direction in ('left', 'right'):
            x = 0 if direction == 'left' else width - 1
            seeds.update((x, y) for y in range(max(0, offset), min(height, offset + other_height)) if walkable(x, y))
        elif direction in ('up', 'down'):
            y = 0 if direction == 'up' else height - 1
            seeds.update((x, y) for x in range(max(0, offset), min(width, offset + other_width)) if walkable(x, y))
    queue = deque(seeds)
    seen = set(seeds)
    while queue:
        x, y = queue.popleft()
        for a, b in ((x - 1, y), (x + 1, y), (x, y - 1), (x, y + 1)):
            if walkable(a, b) and (a, b) not in seen:
                seen.add((a, b))
                queue.append((a, b))
    return seen


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--write-report', action='store_true')
    args = parser.parse_args()
    layouts, maps = load_maps()
    sources = defaultdict(set)
    errors = []
    checked = 0
    globals_text = (ROOT / 'data/brisk_static_legendaries.inc').read_text()
    macro = re.compile(r'brisk_static_legendary\s+(\w+),\s*SPECIES_(\w+),\s*\d+,\s*(\w+)')
    definitions = {label: (species, flag) for label, species, flag in macro.findall(globals_text)}
    for data in maps.values():
        if data['name'].endswith('_Frlg'):
            continue  # Bundled Kanto source maps are not Brisk acquisition routes.
        path = ROOT / 'data/maps' / data['name'] / 'scripts.inc'
        text = path.read_text() if path.exists() else ''
        definitions.update({label: (species, flag) for label, species, flag in macro.findall(text)})
    for data in maps.values():
        if data['name'].endswith('_Frlg'):
            continue
        name = data['name']
        path = ROOT / 'data/maps' / name / 'scripts.inc'
        text = path.read_text() if path.exists() else ''
        # Vanilla event battles and trainer/gift rewards, including ship rooms.
        for species in re.findall(r'\b(?:givemon|setwildbattle|seteventmon)\s+SPECIES_(\w+)', text):
            if family(species):
                if name == 'OrangeCity' and species in ('GUZZLORD', 'NECROZMA'):
                    trainer = 'Tankerson' if species == 'GUZZLORD' else 'Hooded Figure'
                    sources[family(species)].add(f'OrangeCity (Kaizo only; {trainer} reward after victory, level 70)')
                else:
                    sources[family(species)].add(name + ' (scripted encounter/reward)')
        for obj in data.get('object_events', []):
            entry = definitions.get(obj['script'])
            if not entry or not family(entry[0]):
                continue
            species, flag = entry
            expected_gfx = 'OBJ_EVENT_GFX_SPECIES(' + species + ')'
            if obj['graphics_id'] not in (expected_gfx, 'OBJ_EVENT_GFX_' + species):
                errors.append(f'{name}: {species} sprite disagrees with encounter script')
            # Route 117 deliberately uses temp visibility flags populated from
            # permanent caught flags after Brawly. Other objects use caught flags.
            if obj['flag'] != flag and not (name == 'Route117' and obj['flag'].startswith('FLAG_TEMP_')):
                errors.append(f'{name}: {species} visibility/caught flag mismatch')
            variants = [None, 'LAYOUT_SKY_PILLAR_TOP_CLEAN'] if name == 'SkyPillar_Top' else [None]
            for variant in variants:
                seen = reachable(data, layouts, maps, variant)
                width, height, blocks = grid(data, layouts, variant)
                x, y = obj['x'], obj['y']
                def compatible(a, b):
                    return (a, b) in seen and (obj['elevation'] == 0 or blocks[b * width + a] >> 12 in (0, obj['elevation']))
                if not compatible(x, y) or not any(compatible(a, b) for a, b in ((x-1,y),(x+1,y),(x,y-1),(x,y+1))):
                    errors.append(f'{name}: {species} has no reachable standing/interaction tile ({variant or data["layout"]})')
            sources[family(species)].add(f'{species}: {name} (static, {obj["x"]}, {obj["y"]})')
            checked += 1
    # Default always-visible Mirage Island supports Route 130's land table.
    wild = json.loads((ROOT / 'src/data/wild_encounters.json').read_text())
    for group in wild['wild_encounter_groups']:
        if not group.get('for_maps'):
            continue
        for entry in group.get('encounters', []):
            if entry['map'].endswith('_FRLG'):
                continue
            for species in re.findall(r'SPECIES_(\w+)', json.dumps(entry)):
                if family(species):
                    sources[family(species)].add(entry['map'] + ' (wild table)')
    # Runtime roamer selection and ordinary, trade-free evolution routes.
    assert 'TryAddRoamer(SPECIES_LATIAS, 40)' in (ROOT / 'src/roamer.c').read_text()
    sources['LATIAS'].add('Hoenn roamer after the postgame TV scene')
    evo = {
        'SILVALLY': ('TYPE_NULL', 'friendship level-up'),
        'COSMOEM': ('COSMOG', 'level 43'),
        'SOLGALEO': ('COSMOEM', 'level 53 outside night'),
        'LUNALA': ('COSMOEM', 'level 53 at night'),
        'NAGANADEL': ('POIPOLE', 'level 30'),
        'MELMETAL': ('MELTAN', 'level 55'),
        'URSHIFU': ('KUBFU', 'Scroll of Darkness / Scroll of Waters, Slateport stone seller'),
    }
    for species, (parent, method) in evo.items():
        if sources[parent]:
            sources[species].add(parent + ' evolution: ' + method)
    for species in LEGENDS:
        if not sources[species]:
            errors.append(species + ': no default acquisition source found')
    # Cresselia inherits its prior caught flag at Necrozma's old Route 126 position.
    locations = [(data['id'], obj) for data in maps.values() for obj in data.get('object_events', []) if obj.get('script') == 'BriskStatic_EventScript_CRESSELIA']
    if len(locations) != 1 or locations[0][0] != 'MAP_ROUTE126' or (locations[0][1]['x'], locations[0][1]['y']) != (41, 52):
        errors.append('Cresselia: expected only Route 126 at 41,52')
    if any(o.get('script') == 'BriskStatic_EventScript_GUZZLORD' for o in maps['MAP_METEOR_FALLS_B1F_2R']['object_events']):
        errors.append('Old Guzzlord encounter spot must stay empty')
    for species, destination in [('TAPU_FINI', 'MAP_ROUTE143'), ('TAPU_BULU', 'MAP_ROUTE145'), ('TAPU_KOKO', 'MAP_ROUTE146')]:
        matches = [(data['id'], obj) for data in maps.values() for obj in data.get('object_events', []) if obj.get('script') == 'BriskStatic_EventScript_' + species]
        if len(matches) != 1 or matches[0][0] != destination:
            errors.append(species + ': expected exactly one Orange Islands encounter')
    # Existing Mystic Ticket owners must not depend on a missing legacy flag.
    harbor = (ROOT / 'data/maps/LilycoveCity_Harbor/scripts.inc').read_text().split('LilycoveCity_Harbor_EventScript_GetMysticTicketState::')[1].split('LilycoveCity_Harbor_EventScript_GetFirstTimeShowingTicket::')[0]
    assert 'setflag FLAG_ENABLE_SHIP_NAVEL_ROCK' in harbor
    assert 'goto_if_unset FLAG_ENABLE_SHIP_NAVEL_ROCK' not in harbor
    if errors:
        raise SystemExit('\n'.join(errors))
    if args.write_report:
        lines = ['# Legendary acquisition routes', '',
                 'Default, unrandomized Brisk. Includes mythicals and Ultra Beasts. Story, badge, puzzle and HM requirements still apply. Evolution routes use Brisk methods, not the original games\' methods.', '',
                 'Tapu Fini moved from Seafloor Cavern to Route 143; Tapu Bulu from Route 123 to Route 145; Tapu Koko from New Mauville to Route 146. Their original permanent caught flags are retained.', '',
                 'The static-map checker verifies floor collision, access from map entrances, interaction elevation, matching sprites/flags, both Sky Pillar layouts, acquisition source coverage, and the Mystic Ticket ferry repair. Story-controlled native encounters retain their original scripts; this audit does not simulate every story/HM puzzle.', '',
                 '| Pokémon | Acquisition route |', '| --- | --- |']
        for species in LEGENDS:
            lines.append('| ' + species.replace('_', ' ').title() + ' | ' + '; '.join(sorted(sources[species])) + ' |')
        (ROOT / 'docs/brisk/legendary-locations.md').write_text('\n'.join(lines) + '\n')
    print(f'PASS: {checked} repeatable legendary/mythical/Ultra Beast objects have accessible tiles; {len(LEGENDS)} species have acquisition sources; three unique Orange Islands relocations.')


if __name__ == '__main__':
    main()
