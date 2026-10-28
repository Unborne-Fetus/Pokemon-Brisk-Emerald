#!/usr/bin/env python3
"""Validate ordinary authored teams and fingerprint protected trainer entries.

The policy records compiled Brisk stats/learnsets used for this balance pass.
Update that catalog deliberately when changing species data or learnsets.
Hard/Kaizo's existing battle-time modifiers are separate from these sets.
"""
import hashlib
import json
import re
from check_trainer_progression import ROOT, TEAM_RE, LEAGUE_IDS
from trainer_mode_plan import ordinary_scaled
from league_teams import audit as audit_league
from kaizo_bosses import audit as audit_kaizo
from steven_champion import audit as audit_steven


def power_cap(level):
    return 50 if level < 12 else 60 if level < 20 else 75 if level < 30 else 90 if level < 40 else 100 if level < 50 else 120


def stat_cap(level):
    return 350 if level < 12 else 420 if level < 20 else 470 if level < 30 else 510 if level < 40 else 550 if level < 50 else 600


def audit():
    policy = json.loads((ROOT / 'tools/brisk/trainer_fairness_policy.json').read_text())
    teams = dict(TEAM_RE.findall((ROOT / 'src/data/trainers.party').read_text()))
    errors = []
    if set(teams) != set(policy['reworked']) | set(policy['story']) | policy['protected'].keys() | policy['metadata_only'].keys() | set(policy['kaizo_bosses']):
        errors.append('Every trainer entry must be explicitly classified as ordinary, story, dedication/Easter egg, or metadata-only')
    runtime_ids = set(re.findall(r'\[(TRAINER_\w+)\] = TRUE',
        (ROOT / 'src/data/brisk_ordinary_trainers.h').read_text()))
    if runtime_ids != set(policy['reworked']) or runtime_ids & policy['protected'].keys():
        errors.append('Runtime ordinary-trainer classification disagrees with protected-team policy')
    exempt_ids = set(re.findall(r'\[(TRAINER_\w+)\] = TRUE', (ROOT / 'src/data/brisk_exempt_trainers.h').read_text()))
    if exempt_ids != policy['protected'].keys():
        errors.append('Runtime difficulty exemptions disagree with dedication/Easter egg policy')
    story_ids = set(re.findall(r'\[(TRAINER_\w+)\] = TRUE', (ROOT / 'src/data/brisk_story_trainers.h').read_text()))
    if story_ids != set(policy['story']) or story_ids & (set(policy['reworked']) | policy['protected'].keys()):
        errors.append('Runtime story-trainer classification disagrees with policy')
    for trainer_id, fingerprint in (policy['protected'] | policy['metadata_only']).items():
        actual = hashlib.sha256(teams.get(trainer_id, '').encode()).hexdigest()
        if actual != fingerprint:
            errors.append(f'{trainer_id}: protected trainer changed; review explicitly')
    mons = 0
    for trainer_id in policy['reworked']:
        if trainer_id not in teams:
            errors.append(f'{trainer_id}: missing team'); continue
        team = teams[trainer_id]
        blocks = re.split(r'\n[ \t]*\n', team.strip())[1:]
        party_levels = []
        for block in blocks:
            name = block.splitlines()[0].split(' @ ')[0]
            match = re.search(r'^Level: (\d+)$', block, re.M)
            if name not in policy['species'] or not match:
                errors.append(f'{trainer_id}: unknown species or missing level: {name}'); continue
            level = int(match[1]); party_levels.append(level); mons += 1
            info = policy['species'][name]
            prefix = f'{trainer_id}: {name} at {level}'
            if level < info['minimum_level'] or sum(info['stats']) > stat_cap(level) or info.get('special'):
                errors.append(prefix + ': species is too advanced for this level')
            if re.search(r'^(Dynamax Level|Gigantamax|Tera Type|Ability):', block, re.M):
                errors.append(prefix + ': unexpected gimmick or ability override')
            item = block.splitlines()[0].partition(' @ ')[2]
            allowed_items = {''}
            if level >= 12: allowed_items.add('Oran Berry')
            if level >= 35: allowed_items.add('Sitrus Berry')
            if level >= 50: allowed_items.add('Expert Belt')
            if item not in allowed_items:
                errors.append(prefix + ': inappropriate held item')
            legal = set(); lineage = [name]; visited = set()
            while lineage:
                ancestor = lineage.pop()
                if ancestor in visited: continue
                visited.add(ancestor)
                legal.update(m for l,m in policy['species'][ancestor]['learnset'] if l <= level)
                lineage.extend(policy['parents'].get(ancestor, []))
            moves = re.findall(r'^- (MOVE_\w+)$', block, re.M)
            if not 1 <= len(moves) <= 4 or len(moves) != len(set(moves)):
                errors.append(prefix + ': invalid move count or duplicates')
            for move in moves:
                if move not in legal or move not in policy['moves'] or policy['moves'][move]['power'] > power_cap(level):
                    errors.append(prefix + ': illegal or excessive-power move: ' + move)
            if not any(policy['moves'].get(m, {}).get('power', 0) > 1 for m in moves):
                errors.append(prefix + ': no usable damaging move')
            ivs = [int(n) for n in re.findall(r'(\d+) (?:HP|Atk|Def|SpA|SpD|Spe)', re.search(r'^IVs: (.*)',block,re.M)[1])]
            expected_iv = 8 if level < 12 else 12 if level < 25 else 18 if level < 45 else 24
            if len(ivs) != 6 or any(n != expected_iv for n in ivs):
                errors.append(prefix + ': inconsistent IV progression')
            ev_match = re.search(r'^EVs: (.*)',block,re.M)
            evs = [int(n) for n in re.findall(r'\d+',ev_match[1])] if ev_match else []
            ev_cap = 0 if level < 25 else 128 if level < 45 else 256 if level < 65 else 384
            if sum(evs) > ev_cap or any(n > 252 for n in evs):
                errors.append(prefix + ': excessive EV training')
        if len(party_levels) > 1:
            for mode in ('Easy', 'Normal', 'Hard', 'Kaizo'):
                for cap in (25, 35, 45, 50, 55, 65, 75, 90, 100):
                    varied = {ordinary_scaled(level, mode, max(party_levels), cap) for level in party_levels}
                    if len(varied) == 1:
                        errors.append(f'{trainer_id}: flat {mode} team at cap {cap}')
        if party_levels:
            for cap in (25, 35, 45, 50, 55, 65, 75, 90, 100):
                for level in party_levels:
                    hard = ordinary_scaled(level, 'Hard', max(party_levels), cap)
                    extreme = ordinary_scaled(level, 'Kaizo', max(party_levels), cap)
                    if extreme < hard:
                        errors.append(f'{trainer_id}: Kaizo member below Hard at cap {cap}')
            ace = max(party_levels)
            cap = 2 if ace < 12 else 3 if ace < 25 else 4 if ace < 45 else 5 if ace < 65 else 6
            if len(party_levels) > cap:
                errors.append(f'{trainer_id}: excessive party size for this stage')
    errors.extend(audit_league(teams))
    errors.extend(audit_kaizo(teams))
    errors.extend(audit_steven(teams))
    story_mons = 0
    for tid in policy['story']:
        team = teams[tid]
        blocks = re.split(r'\n[ \t]*\n', team.strip())[1:]
        if not 1 <= len(blocks) <= 6:
            errors.append(f'{tid}: invalid story party size')
        if tid in LEAGUE_IDS or tid == 'TRAINER_STEVEN':
            # Full cross-generation learnability is checked above. Native tests
            # validate abilities and preserve the actual sets at 70 and 100.
            story_mons += len(blocks)
            continue
        for block in blocks:
            name = block.splitlines()[0].split(' @ ')[0]
            match = re.search(r'^Level: (\d+)$', block, re.M)
            if not match or name not in policy['story_species']:
                errors.append(f'{tid}: missing story species or level'); continue
            level = int(match[1]); story_mons += 1
            legal = set(); todo = [name]; visited = set()
            while todo:
                n = todo.pop()
                if n in visited: continue
                visited.add(n)
                legal.update(m for l,m in policy['story_species'][n]['learnset'] if l <= level)
                todo.extend(policy['story_parents'].get(n, []))
            moves = re.findall(r'^- (MOVE_\w+)$', block, re.M)
            if not 1 <= len(moves) <= 4 or len(set(moves)) != len(moves) or any(m not in legal for m in moves):
                errors.append(f'{tid}: illegal story moves on {name}')
            if not any(policy['moves'][m]['power'] > 1 for m in moves):
                errors.append(f'{tid}: no usable story attack on {name}')
            cap = 50 if level < 12 else 75 if level < 25 else 90 if level < 35 else 100 if level < 50 else 120
            if any(policy['moves'][m]['power'] > cap for m in moves):
                errors.append(f'{tid}: excessive story move power on {name}')
            ev = re.search(r'^EVs: (.*)$',block,re.M)
            values = [int(n) for n in re.findall(r'\d+',ev[1])] if ev else []
            if sum(values) > 510 or any(n > 252 for n in values):
                errors.append(f'{tid}: illegal story EVs on {name}')
    # Rival suffix refers to the player's starter; the rival takes its counter.
    for name in ('MAY', 'BRENDAN'):
        for suffix, starter in (('MUDKIP','SCEPTILE'),('TREECKO','BLAZIKEN'),('TORCHIC','SWAMPERT')):
            for location, count in (('ROUTE_110',4),('ROUTE_119',5),('LILYCOVE',6)):
                tid = f'TRAINER_{name}_{location}_{suffix}'
                blocks = re.split(r'\n[ \t]*\n', teams[tid].strip())[1:]
                if len(blocks) != count or not blocks[-1].startswith('SPECIES_'+starter):
                    errors.append(f'{tid}: incomplete or unevolved rival team')
    print(f'Story audit: {len(policy["story"])} teams, {story_mons} Pokemon; '
          f'{len(policy["protected"])} dedication/Easter egg entries; {len(policy["metadata_only"])} metadata-only records.')
    for error in errors: print(error)
    print(f'Fairness audit: {len(policy["reworked"])} teams, {mons} Pokemon; '
          f'{len(policy["protected"])} protected entries; {len(errors)} errors.')
    return bool(errors)


if __name__ == '__main__':
    raise SystemExit(audit())
