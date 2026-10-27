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


def render():
    teams = dict(TEAM_RE.findall((ROOT / 'src/data/trainers.party').read_text()))
    policy = json.loads((ROOT / 'tools/brisk/trainer_fairness_policy.json').read_text())
    lines = ['# Brisk trainer plan by difficulty', '',
        'This is the complete Hoenn trainer plan, including ordinary trainers, rematches,',
        'story fights, rivals, gyms, the League and dedication/Easter egg encounters.',
        'Normal levels are authored per encounter; revisiting an old route does not',
        'scale its trainers up to the player. Important teams retain their definitions.', '',
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
        ranges = [f'{scaled(low,m)}–{scaled(high,m)}' for m in ('Normal','Easy','Hard','Extreme')]
        lines.append('| ' + name + ' | ' + ' | '.join(ranges) + ' |')
    lines += ['', 'The section bands describe ordinary encounter targets, not blanket map scaling.',
        'Individual teams and intentional protected exceptions are listed below.',
        'The opening Route 103 tutorial rival and protected story encounters retain',
        'their existing levels; they are separate from Route 102 ordinary trainers.', '',
        '## Mode rules', '',
        '| Rule | Easy | Normal | Hard | Extreme |',
        '| --- | --- | --- | --- | --- |',
        '| Trainer levels | Half, rounded up | Authored curve | +20%, at least +3 | +30%, at least +4 |',
        '| IVs | 0 | 8 / 12 / 18 / 24 by stage | 31 | 31 |',
        '| EVs | 0 | 0 / 128 / 256 / 384 by stage | 128 / 256 / 384 / 510 (at least authored total) | 510, chosen for the set |',
        '| Ordinary moves | Weaker legal attacks | Legal authored coverage/support | Optimized ordinary moves and stronger abilities | Optimized legal moves within stage power limits |',
        '| AI | Basic bad-move checks | Basic move scoring | Smart switching, HP and KO checks | Omniscient with smart switching |',
        '| Battle style | Shift | Player preference | Set | Set |',
        '| Player battle consumables | Allowed | Allowed | Blocked | Blocked |',
        '| Shared EXP | 200% | 100% | 50% | Disabled |', '',
        'Hard and Extreme trainer levels are additionally clamped to the save’s current',
        'badge cap: **25 / 35 / 45 / 50 / 55 / 65 / 75 / 90 / 100**. The individual',
        'table shows boosts before that clamp because the same rematch can be fought',
        'at different progression stages. Extreme enforces the player EXP cap too.',
        'Difficulty does not evolve an opponent into a different species at runtime.', '',
        'Only the 670 ordinary trainer IDs use the new move-power guard. Important',
        'and dedication/Easter egg teams keep their previous mode behavior. Ordinary',
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
        'unchanged by this pass; their existing runtime difficulty modifiers remain.', '',
        '| Trainer ID | Display name | Role | Normal | Easy | Hard* | Extreme* |',
        '| --- | --- | --- | --- | --- | --- | --- |',
    ]
    count=0
    for tid, team in teams.items():
        lv = party_levels(team)
        if not lv: continue
        name = re.search(r'^Name: (.*)',team,re.M)[1].strip().replace('|','\\|')
        role = 'Protected' if tid in policy['protected'] else 'Ordinary'
        cells = [' / '.join(str(scaled(n,m)) for n in lv) for m in ('Normal','Easy','Hard','Extreme')]
        lines.append('| `' + tid + '` | ' + name + ' | ' + role + ' | ' + ' | '.join(cells) + ' |')
        count += 1
    lines += ['', f'{count} populated trainer entries; *Hard/Extreme values are before the current badge cap.', '',
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
