#!/usr/bin/env python3
"""Validate Meteor Falls Steven's fixed level-100 Steel champion roster."""
import json
import re
from check_trainer_progression import ROOT, TEAM_RE
from league_teams import render_team


def sets():
    return json.loads((ROOT / 'tools/brisk/steven_champion.json').read_text())


def audit(teams):
    members = sets()
    errors = []
    team = teams.get('TRAINER_STEVEN', '')
    if len(members) != 6 or team != render_team(team, members):
        errors.append('Steven must match the curated six level-100 champion sets')
    if 'Class: Champion\n' not in team: errors.append('Steven needs Champion class')
    learnables = json.loads((ROOT / 'src/data/pokemon/all_learnables.json').read_text())
    for m in members:
        compatible = learnables.get(m.get('learnset_species', m['species'].removeprefix('SPECIES_')), [])
        for move in m['moves']:
            if move not in compatible: errors.append(m['species']+': illegal move '+move)
        evs = list(map(int, re.findall(r'\d+', m['evs'])))
        if sum(evs)>510 or any(n>252 for n in evs): errors.append(m['species']+': illegal EVs')
    return errors


def document():
    lines = ['# Steven: Meteor Falls champion', '',
             'Steven now uses six level-100 Steel-type Pokemon in every difficulty, including Easy. This is a postgame challenge independent of badge caps. His Champion class, legal competitive moves, abilities, held items and EV spreads are preserved by the trainer generator. AI follows the selected difficulty, with expert behavior on Hard and prediction on Kaizo.', '',
             'Corviknight provides Ground immunity, physical defense, recovery, hazard removal and pivoting. Heatran absorbs Fire attacks and uses Air Balloon, trapping and Stealth Rock. Origin Dialga supplies bulky special coverage. Choice Scarf Gholdengo provides speed control and Trick against defensive teams. Mega Metagross remains Steven’s signature powerhouse. Kingambit closes with Supreme Overlord, priority and Flying Tera to turn its Ground/Fighting weaknesses around.', '',
             '| Pokemon | Item | Ability | Nature; EVs | Moves |', '| --- | --- | --- | --- | --- |']
    clean = lambda s:s.removeprefix('SPECIES_').removeprefix('MOVE_').replace('_',' ').title()
    for m in sets(): lines += ['| '+clean(m['species'])+' | '+m['item']+' | '+m['ability']+' | '+m['nature']+'; '+m['evs']+' | '+', '.join(clean(n) for n in m['moves'])+' |']
    lines += ['', 'Gholdengo deliberately carries Trick with Choice Scarf: it can give the Scarf to a wall. Mega Metagross begins as base Metagross with Clear Body and transforms through Metagrossite. Origin Dialga holds its required Adamant Crystal.', '',
              'The separate Mossdeep partner fight keeps its existing party. Its intro now waits for each multi-battle trainer picture to release its sprite tiles before starting that trainer’s send-out. Four party trays share one copy of their graphics and palettes, avoiding leaked duplicate tile allocations. Sprite creation errors now distinguish a full sprite pool from exhausted OBJ tile memory.', '']
    return '\n'.join(lines)

if __name__ == '__main__':
    errors = audit(dict(TEAM_RE.findall((ROOT / 'src/data/trainers.party').read_text())))
    for e in errors: print(e)
    if not errors:
        (ROOT / 'docs/brisk_steven_champion.md').write_text(document())
        print('Steven: curated six-Pokemon level-100 Steel champion roster validated.')
    raise SystemExit(bool(errors))
