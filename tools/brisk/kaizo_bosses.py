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
        if m.get('shiny'): lines += ['Shiny: Yes']
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
    rewards = {'TRAINER_HOODED_FIGURE': ('NECROZMA', 'FLAG_HIDE_ROUTE126_NECROZMA'),
               'TRAINER_TANKERSON': ('GUZZLORD', 'FLAG_DEFEATED_GUZZLORD')}
    if set(curated) != set(labels): errors.append('Expected exactly the two Kaizo bosses')
    ace = curated['TRAINER_TANKERSON']['members'][-1]
    if ace['species'] != 'SPECIES_UMBREON' or not ace.get('shiny') or 'MOVE_FOUL_PLAY' not in ace['moves']:
        errors.append('Tankerson ace must be shiny Umbreon with Foul Play')
    if sum(m.get('shiny', False) for m in curated['TRAINER_TANKERSON']['members']) != 1:
        errors.append('Tankerson must have exactly one forced shiny')
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
        species, claimed_flag = rewards[tid]
        section = scripts.split(start+'::', 1)[-1].split(start+'Ignored::', 1)[0]
        for required in (f'goto_if_set {claimed_flag}, {start}OfferBattle',
                         f'checktrainerflag {tid}',
                         f'goto_if TRUE, {start}GiveReward',
                         f'goto_if_set {claimed_flag}, {start}AfterVictory',
                         f'givemon SPECIES_{species}, 70',
                         f'goto_if_eq VAR_RESULT, MON_CANT_GIVE, {start}StorageFull',
                         f'setflag {claimed_flag}',
                         'call Common_EventScript_ReceivedGiftPokemon'):
            if required not in section: errors.append(tid+': missing gift safety gate '+required)
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
    for map_name, old_script in (('MeteorFalls_B1F_2R', 'BriskStatic_EventScript_GUZZLORD'),
                                 ('Route126', 'Route126_EventScript_NECROZMA')):
        original = json.loads((ROOT / 'data/maps' / map_name / 'map.json').read_text())
        if any(o['script'] == old_script for o in original['object_events']):
            errors.append(map_name+': moved legendary still appears as static')
    return errors


def document():
    lines = ['# Orange City Kaizo challenges', '',
             'Both trainers stand in the Orange City plaza. Speak to them in Kaizo to accept a repeatable challenge; choose No to leave. In Easy, Normal and Hard they pay you no mind. Each uses six level-100 Pokemon and full competitive training.', '',
             'Hooded Figure gives a one-time level-70 Necrozma after a win; Tankerson gives a one-time level-70 Guzzlord. If both party and PC are full, the gift stays unclaimed and can be collected by talking to that trainer again without another win. Existing captures remain claimed. Rematches are still unlimited.', '',
             'These are unrestricted, cross-generation teams with legal species abilities, learnable moves and at most 510 EVs (252 per stat). Legendary Pokemon are allowed. Each team has one Mega candidate; Tankerson uses Mega Mawile rather than Primal Kyogre. No single team is strongest in every matchup.', '',
             'Hooded Figure uses singles offense, with Ground immunity, special walls, hazard removal and multiple sweepers. Tankerson\'s shiny Umbreon is his defensive ace with Foul Play, Snarl, Helping Hand and Protect. Grimmsnarl supplies Prankster screens; Porygon2 sets Trick Room; Amoonguss redirects and heals with Pollen Puff; Mega Mawile pressures Fighting- and Fairy-types; Milotic checks Fire- and Ground-types and punishes Intimidate with Competitive. Zero Speed IVs support the Trick Room core.', '',
             'Existing Extreme saves now display Kaizo without changing the saved mode value. Trainer flag bounds and save structures remain unchanged.', '']
    for tid, t in sets().items():
        lines += ['## '+t['display_name']+' — '+('doubles' if t['double'] else 'singles'), '', '| Pokemon | Item | Ability | Nature; EVs | Moves |', '| --- | --- | --- | --- | --- |']
        for m in t['members']:
            clean = lambda s: s.removeprefix('SPECIES_').removeprefix('MOVE_').replace('_', ' ').title()
            lines += ['| '+clean(m['species'])+(' (shiny ace)' if m.get('shiny') else '')+' | '+m['item']+' | '+m['ability']+' | '+m['nature']+'; '+m['evs']+(' (0 Speed IV)' if m.get('zero_speed') else '')+' | '+', '.join(clean(n) for n in m['moves'])+' |']
        lines += ['']
    lines += ['Zacian starts in its base form with Iron Head; Rusted Sword applies Crowned form and Behemoth Blade through the native battle transformation. Rayquaza qualifies for Mega Evolution through Dragon Ascent. Tankerson\'s Mawile Mega Evolves using Mawilite; Umbreon is guaranteed shiny.', '']
    return '\n'.join(lines)


if __name__ == '__main__':
    errors = audit(dict(TEAM_RE.findall((ROOT / 'src/data/trainers.party').read_text())))
    for e in errors: print(e)
    if not errors:
        (ROOT / 'docs/brisk_kaizo_bosses.md').write_text(document())
        print('Both Kaizo teams, legal moves/EVs, map placement and mode gates validated.')
    raise SystemExit(bool(errors))
