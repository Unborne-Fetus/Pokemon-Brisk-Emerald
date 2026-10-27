#!/usr/bin/env python3
"""Validate the two authored Kaizo challenges and their map gates."""
import json
import re
from check_trainer_progression import ROOT, TEAM_RE


def sets():
    return json.loads((ROOT / 'tools/brisk/kaizo_bosses.json').read_text())


def render(t):
    header = ['Name: '+t['name'], 'Class: Champion', 'Pic: '+t['pic'], 'Gender: Male', 'Music: Male',
              'Double Battle: '+('Yes' if t['double'] else 'No'),
              'AI: Smart Trainer / Prediction / Know Opponent Party / HP Aware / Try To 2HKO / Conservative']
    blocks = []
    for m in t['members']:
        iv = '31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / '+('0' if m.get('zero_speed') else '31')+' Spe'
        lines = [m['species']+' @ '+m['item'], 'Level: 100', 'Ability: '+m['ability'], 'Nature: '+m['nature'], 'EVs: '+m['evs'], 'IVs: '+iv]
        if m.get('tera'): lines += ['Tera Type: '+m['tera']]
        blocks += ['\n'.join(lines + ['- '+move for move in m['moves']])]
    return '\n'.join(header)+'\n\n'+'\n\n'.join(blocks)+'\n\n'


def audit(teams):
    errors = []
    curated = sets()
    learn = json.loads((ROOT / 'src/data/pokemon/all_learnables.json').read_text())
    scripts = (ROOT / 'data/maps/OrangeCity/scripts.inc').read_text()
    objects = json.loads((ROOT / 'data/maps/OrangeCity/map.json').read_text())['object_events']
    labels = {'TRAINER_HOODED_FIGURE': 'HoodedFigure', 'TRAINER_TANKERSON': 'Tankerson'}
    if set(curated) != set(labels): errors.append('Expected exactly the two Kaizo bosses')
    for tid, t in curated.items():
        if teams.get(tid, '').strip() != render(t).strip(): errors.append(tid+': source differs from curated party')
        if len(t['members']) != 6: errors.append(tid+': needs six Pokemon')
        for m in t['members']:
            key = m.get('learnset_species', m['species'].removeprefix('SPECIES_'))
            for move in m['moves']:
                if move not in learn.get(key, []): errors.append(tid+': illegal move '+move)
            if len(set(m['moves'])) != 4 or not m['item']: errors.append(tid+': invalid moves/item')
            evs = list(map(int, re.findall(r'\d+', m['evs'])))
            if sum(evs)>510 or any(n>252 for n in evs): errors.append(tid+': invalid EVs')
        label = labels[tid]
        start = 'OrangeCity_EventScript_'+label
        gate = start+'::\n\tlock\n\tgoto_if_ne VAR_BRISK_DIFFICULTY, BRISK_MODE_KAIZO, '+start+'Ignored\n'
        if gate not in scripts: errors.append(tid+': missing first-action mode gate')
        ignored = scripts.split(start+'Ignored::\n', 1)[-1].split('\n\n', 1)[0]
        if 'trainerbattle' in ignored or '\trelease\n\tend' not in ignored: errors.append(tid+': invalid ignored branch')
        npcs = [o for o in objects if o['script'] == start]
        if len(npcs)!=1 or npcs[0]['trainer_type']!='TRAINER_TYPE_NONE': errors.append(tid+': must have one non-sight trainer NPC')
        else:
            o = npcs[0]
            raw = (ROOT / 'data/layouts/OrangeCity/map.bin').read_bytes()
            offset = (o['y']*48+o['x'])*2
            block = int.from_bytes(raw[offset:offset+2], 'little')
            if block & 0xC00 or block>>12 != o['elevation']: errors.append(tid+': blocked tile/elevation')
    return errors


def document():
    lines = ['# Orange City Kaizo challenges', '',
             'Both trainers stand in the Orange City plaza. Speak to them in Kaizo to accept a repeatable challenge; choose No to leave. In Easy, Normal and Hard they pay you no mind. Each uses six level-100 Pokemon and full competitive training.', '',
             'These are unrestricted, cross-generation teams with legal species abilities, learnable moves and at most 510 EVs (252 per stat). Legendary Pokemon are allowed. Each team has one Mega candidate; Tankerson also uses Primal Kyogre. No single team is strongest in every matchup.', '',
             'Hooded Figure uses singles offense, with Ground immunity, special walls, hazard removal and multiple sweepers. Tankerson uses doubles with Fake Out, Intimidate, redirection, Wide Guard, rain and Trick Room. Zero Speed IVs support the slow rain/Trick Room core.', '',
             'Existing Extreme saves now display Kaizo without changing the saved mode value. Trainer flag bounds and save structures remain unchanged.', '']
    for tid, t in sets().items():
        lines += ['## '+t['display_name']+' — '+('doubles' if t['double'] else 'singles'), '', '| Pokemon | Item | Ability | Nature; EVs | Moves |', '| --- | --- | --- | --- | --- |']
        for m in t['members']:
            clean = lambda s: s.removeprefix('SPECIES_').removeprefix('MOVE_').replace('_', ' ').title()
            lines += ['| '+clean(m['species'])+' | '+m['item']+' | '+m['ability']+' | '+m['nature']+'; '+m['evs']+(' (0 Speed IV)' if m.get('zero_speed') else '')+' | '+', '.join(clean(n) for n in m['moves'])+' |']
        lines += ['']
    lines += ['Zacian starts in its base form with Iron Head; Rusted Sword applies Crowned form and Behemoth Blade through the native battle transformation. Zamazenta similarly uses Rusted Shield. Kyogre starts with Drizzle and transforms through Blue Orb. Rayquaza qualifies for Mega Evolution through Dragon Ascent.', '']
    return '\n'.join(lines)


if __name__ == '__main__':
    errors = audit(dict(TEAM_RE.findall((ROOT / 'src/data/trainers.party').read_text())))
    for e in errors: print(e)
    if not errors:
        (ROOT / 'docs/brisk_kaizo_bosses.md').write_text(document())
        print('Both Kaizo teams, legal moves/EVs, map placement and mode gates validated.')
    raise SystemExit(bool(errors))
