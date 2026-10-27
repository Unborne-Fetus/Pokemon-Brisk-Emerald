#!/usr/bin/env python3
"""Render or check the five curated League teams; never touch other trainers."""
import argparse
import json
import re
from check_trainer_progression import ROOT, TEAM_RE, LEAGUE_IDS


def sets():
    return json.loads((ROOT / 'tools/brisk/league_teams.json').read_text())


def mon_block(mon):
    lines = [mon['species'] + ' @ ' + mon['item'], 'Level: 100',
             'Ability: ' + mon['ability'], 'Nature: ' + mon['nature'],
             'EVs: ' + mon['evs'], 'IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe']
    if mon.get('shiny'): lines += ['Shiny: Yes']
    if mon.get('gigantamax'): lines += ['Dynamax Level: 10', 'Gigantamax: Yes']
    if mon.get('tera'): lines += ['Tera Type: ' + mon['tera']]
    return '\n'.join(lines + ['- ' + move for move in mon['moves']])


def render_team(team, members):
    header = re.split(r'\n[ \t]*\n', team.strip())[0]
    if members[-1]['ability'] == 'Supreme Overlord' and 'Ace Pokemon' not in header:
        header = re.sub(r'^AI: (.*)$', r'AI: \1 / Ace Pokemon', header, flags=re.M)
    return header + '\n\n' + '\n\n'.join(mon_block(m) for m in members) + '\n\n'


def audit(teams):
    errors = []
    curated = sets()
    learnables = json.loads((ROOT / 'src/data/pokemon/all_learnables.json').read_text())
    if set(curated) != LEAGUE_IDS:
        errors.append('Curated League IDs must be exactly the Elite Four and Wallace')
    for tid, members in curated.items():
        if len(members) != 6:
            errors.append(f'{tid}: curated team must have six Pokemon')
        if teams.get(tid) != render_team(teams.get(tid, ''), members):
            errors.append(f'{tid}: party differs from curated League set')
        for mon in members:
            compatible = set(learnables.get(mon['species'].removeprefix('SPECIES_'), []))
            for move in mon['moves']:
                if move not in compatible:
                    errors.append(f'{tid}: {mon["species"]} cannot learn {move}')
            if not mon['item'] or len(set(mon['moves'])) != 4:
                errors.append(f'{tid}: missing item or four distinct moves')
            evs = [int(n) for n in re.findall(r'\d+', mon['evs'])]
            if sum(evs) > 510 or any(n > 252 for n in evs):
                errors.append(f'{tid}: illegal EV spread')
    return errors


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--write', action='store_true')
    args = parser.parse_args()
    path = ROOT / 'src/data/trainers.party'
    source = path.read_text()
    curated = sets()
    if set(curated) != LEAGUE_IDS:
        raise SystemExit('Refusing to edit trainers outside the five League IDs')
    if args.write:
        path.write_text(TEAM_RE.sub(lambda m: '=== '+m[1]+' ===\n'+render_team(m[2], curated[m[1]]) if m[1] in curated else m[0], source))
    errors = audit(dict(TEAM_RE.findall(path.read_text())))
    for error in errors: print(error)
    if not errors: print('All five League parties match the curated six-Pokemon sets.')
    raise SystemExit(bool(errors))
