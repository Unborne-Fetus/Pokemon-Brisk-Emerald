#!/usr/bin/env python3
"""Generate/check the complete authored Hoenn trainer plan for every mode."""
import json
import re
import sys
from check_trainer_progression import ROOT, TEAM_RE

OUTPUT = ROOT / 'docs/brisk_trainer_mode_plan.md'

def party_levels(team):
    # trainerproc defaults a Pokemon without a Level field to level 100.
    # Include the Archive champions that deliberately use that shorthand.
    blocks = [b.strip() for b in re.split(r'\n(?:[ \t]*\n)+', team.strip())]
    result = []
    for block in blocks[1:]:
        if not block or block.startswith(('-', '/*', 'Ability:', 'Nature:')):
            continue
        explicit = [int(n) for n in re.findall(r'^Level:[ \t]*(\d+)', block, re.M)]
        result.extend(explicit or [100])
    return result


def scaled(level, mode):
    if mode == 'Easy': return max(1, (level + 1) // 2)
    if mode == 'Hard': return min(100, level + max(3, level // 5))
    if mode == 'Extreme': return min(100, level + max(4, level * 3 // 10))
    return level


def ordinary_scaled(level, mode, highest, cap=100):
    if mode == 'Easy':
        value = (level + 1) // 2
        return max(1, value - (level < highest and value == (highest + 1) // 2))
    if mode == 'Hard':
        boost = lambda n: n + max(2, n // 5)
    elif mode == 'Extreme':
        boost = lambda n: n + max(4, n * 2 // 5)
    else:
        return level
    return max(1, min(100, min(boost(level), max(1, cap - (highest - level)))))


def story_scaled(level, mode, highest, cap=100):
    if mode == 'Easy': return max(1, (level + 1) // 2)
    if mode == 'Hard': value = level + max(3, level // 4)
    elif mode == 'Extreme': value = level + max(5, level * 45 // 100)
    else: return level
    return max(1, min(value, max(1, cap - (highest - level))))


def render():
    teams = dict(TEAM_RE.findall((ROOT / 'src/data/trainers.party').read_text()))
    policy = json.loads((ROOT / 'tools/brisk/trainer_fairness_policy.json').read_text())
    lines = ['# Brisk trainer plan by difficulty', '',
        'This is the complete Hoenn trainer plan, including ordinary trainers, rematches,',
        'story fights, rivals, gyms, the League and dedication/Easter egg encounters.',
        'Normal levels are authored per encounter; revisiting an old route does not',
        'scale its trainers up to the player. Only dedication/Easter egg teams are exempt.', '',
        '## Progression targets', '',
        '| Section | Normal | Easy | Hard, before badge cap | Extreme, before badge cap |',
        '| --- | --- | --- | --- | --- |',
    ]
    stages = [('Route 102',10,15),('Route 104',14,21),('Petalburg Woods',16,20),
              ('Route 116 / Rusturf approach',20,24),('Rustboro Gym trainers',21,24),
              ('Dewford / Brawly approach',28,34),('Slateport / Route 110',35,42),
              ('Wattson approach',40,44),('Northern routes / Flannery approach',43,49),
              ('Norman approach',51,54),('Route 119 / Fortree',57,64),
              ('Lilycove / hideouts',66,73),('Mossdeep Gym approach',71,74),
              ('Space Center / eastern sea',76,83),('Seafloor / Juan approach',82,89),
              ('Victory Road',86,94),('League',100,100)]
    for name,low,high in stages:
        ranges = [f'{ordinary_scaled(low,m,high)}–{ordinary_scaled(high,m,high)}' for m in ('Normal','Easy','Hard','Extreme')]
        lines.append('| ' + name + ' | ' + ' | '.join(ranges) + ' |')
    lines += ['', 'The section bands describe ordinary encounter targets, not blanket map scaling.',
        'Individual teams and intentional protected exceptions are listed below.',
        'Important fights now use their own authored teams and stronger mode curve.',
        'The opening Route 103 rival has a level-9 starter on Normal.', '',
        '## Mode rules', '',
        '| Rule | Easy | Normal | Hard | Extreme |',
        '| --- | --- | --- | --- | --- |',
        '| Ordinary trainer levels | Half, with support below ace | Varied authored curve | +20%, at least +2 | +40%, at least +4 |',
        '| Story trainer levels | Half, rounded up | Authored boss curve | +25%, at least +3 | +45%, at least +5 |',
        '| IVs | 0 | 8 / 12 / 18 / 24 by stage | 31 | 31 |',
        '| EVs | 0 | 0 / 128 / 256 / 384 by stage | 128 / 256 / 384 / 510 (at least authored total) | 510, chosen for the set |',
        '| Ordinary moves | Weaker legal attacks | Legal authored coverage/support | Optimized ordinary moves and stronger abilities | Optimized legal moves within stage power limits |',
        '| AI | Basic bad-move checks | Basic move scoring | Smart switching, HP and KO checks | Omniscient with smart switching |',
        '| Battle style | Shift | Player preference | Set | Set |',
        '| Player battle consumables | Allowed | Allowed | Blocked | Blocked |',
        '| Shared EXP | 200% | 100% | 50% | Disabled |', '',
        'Dedication/Easter egg scaling remains +20% (minimum 3) on Hard and +30%',
        '(minimum 4) on Extreme; Easy still halves their authored levels.',
        'Ordinary and story Hard/Extreme supports stay below the current badge cap by their',
        'authored gap from the ace, preserving a spread instead of flattening the team.',
        'Protected Hard and Extreme trainer levels are clamped to the save’s current',
        'badge cap: **25 / 35 / 45 / 50 / 55 / 65 / 75 / 90 / 100**. The individual',
        'table assumes a cap of 100; earlier caps limit ordinary teams and clamp',
        'protected levels individually. Extreme enforces the player EXP cap too.',
        'Difficulty does not evolve an opponent into a different species at runtime.', '',
        '670 ordinary IDs use their original stage power guard. 155 story IDs now use',
        'a separate stage guard: 50 below 12; 75 below 25; 90 below 35; 100 below 50;',
        '120 thereafter. Dedication/Easter egg teams retain their previous behavior. Ordinary',
        'generated attacks are capped at base power **50 below level 12; 60 below 20;',
        '75 below 30; 90 below 40; 100 below 50; 120 thereafter**. Multi-hit, recoil,',
        'setup, typing and abilities still affect actual damage; this is a power-spike',
        'guard rather than a guarantee of equal battle difficulty.', '',
        '## Overworld encounter preference', '',
        '**Options page 7 → OW ENCOUNTERS → ON/OFF.** ON is the default for old and',
        'new saves. OFF removes generated roaming Pokémon after returning to the',
        'field and uses normal random encounters instead. Scripted/static Pokémon,',
        'story encounters, followers, fishing and DexNav remain available. The',
        'preference persists in the save and is independent of difficulty.', '',
        '## Every trainer entry', '',
        'Levels are in authored party order. Protected entries are byte-for-byte',
        'unchanged only for dedication/Easter eggs. Metadata-only entries supply NPC',
        'identity/templates, not playable battles; their levels are not a challenge plan.', '',
        '| Trainer ID | Display name | Role | Normal | Easy | Hard* | Extreme* |',
        '| --- | --- | --- | --- | --- | --- | --- |',
    ]
    count=0
    for tid, team in teams.items():
        lv = party_levels(team)
        if not lv: continue
        name = re.search(r'^Name: (.*)',team,re.M)[1].strip().replace('|','\\|')
        role = 'Protected' if tid in policy['protected'] else 'Story' if tid in policy['story'] else 'Metadata' if tid in policy['metadata_only'] else 'Ordinary'
        cells = [' / '.join(str(scaled(n,m) if role == 'Protected' else story_scaled(n,m,max(lv)) if role == 'Story' else n if role == 'Metadata' else ordinary_scaled(n,m,max(lv))) for n in lv) for m in ('Normal','Easy','Hard','Extreme')]
        lines.append('| `' + tid + '` | ' + name + ' | ' + role + ' | ' + ' | '.join(cells) + ' |')
        count += 1
    lines += ['', f'{count} populated trainer entries; *Hard/Extreme values assume a cap of 100; earlier badge caps still apply.', '',
        'Regenerate with `python tools/brisk/trainer_mode_plan.py`; validate with `--check`.', '']
    return '\n'.join(lines)


if __name__ == '__main__':
    content = render()
    if '--check' in sys.argv:
        if not OUTPUT.exists() or OUTPUT.read_text() != content:
            raise SystemExit('Trainer mode plan is stale. Run python tools/brisk/trainer_mode_plan.py')
        print('Complete trainer mode plan matches the authored teams.')
    else:
        OUTPUT.write_text(content)
        print('Wrote', OUTPUT.relative_to(ROOT))
