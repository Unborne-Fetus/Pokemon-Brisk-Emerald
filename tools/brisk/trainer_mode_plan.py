#!/usr/bin/env python3
"""Generate/check the complete authored Hoenn trainer plan for every mode."""
import json
import re
import sys
from check_trainer_progression import ROOT, TEAM_RE, LEAGUE_IDS

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
    if mode == 'Kaizo': return min(100, level + max(4, level * 3 // 10))
    return level


def ordinary_scaled(level, mode, highest, cap=100):
    if mode == 'Easy':
        value = (level + 1) // 2
        return max(1, value - (level < highest and value == (highest + 1) // 2))
    if mode == 'Hard':
        boost = lambda n: n + max(2, n // 5)
    elif mode == 'Kaizo':
        boost = lambda n: n + max(4, n * 2 // 5)
    else:
        return level
    return max(1, min(100, min(boost(level), max(1, cap - (highest - level)))))


def story_scaled(level, mode, highest, cap=100):
    if mode == 'Easy': return max(1, (level + 1) // 2)
    if mode == 'Hard': value = level + max(3, level // 4)
    elif mode == 'Kaizo': value = level + max(5, level * 45 // 100)
    else: return level
    return max(1, min(value, max(1, cap - (highest - level))))


def profile_levels(tid, role, levels, mode):
    if role == 'Kaizo only': return [100] * 6 if mode == 'Kaizo' else []
    if tid in LEAGUE_IDS: return [70 if mode == 'Easy' else 100] * len(levels)
    if role == 'Protected': return [scaled(n, mode) for n in levels]
    if role == 'Metadata': return levels
    highest, count = max(levels), len(levels)
    target = count
    tutorial = '_ROUTE_103_' in tid
    if not tutorial:
        if mode == 'Easy':
            limit = (2 if highest < 30 else 3 if highest < 50 else 4 if highest < 80 else 5) if role == 'Story' else (2 if highest < 40 else 3 if highest < 70 else 4)
            target = min(count, limit)
        elif mode == 'Normal': target = max(count, 2 if highest < 20 else 3 if highest < 40 else 4)
        elif mode == 'Hard': target = max(count, (4 if highest < 25 else 5 if highest < 40 else 6) if role == 'Story' else (3 if highest < 20 else 4 if highest < 40 else 5))
        elif mode == 'Kaizo': target = max(count, (4 if highest < 20 else 6) if role == 'Story' else (4 if highest < 20 else 5 if highest < 30 else 6))
    target = min(6, target)
    values = levels if target >= count else levels[:target-1]+levels[-1:]
    scaler = story_scaled if role == 'Story' else ordinary_scaled
    result = [scaler(n, mode, highest) for n in values]
    extras = [scaler(max(1, highest-1-i%2), mode, highest) for i in range(target-count)]
    return result[:-1] + extras + result[-1:]


def render():
    teams = dict(TEAM_RE.findall((ROOT / 'src/data/trainers.party').read_text()))
    policy = json.loads((ROOT / 'tools/brisk/trainer_fairness_policy.json').read_text())
    lines = ['# Brisk trainer plan by difficulty', '',
        'This is the complete Hoenn trainer plan, including ordinary trainers, rematches,',
        'story fights, rivals, gyms, the League and dedication/Easter egg encounters.',
        'Normal levels are authored per encounter; revisiting an old route does not',
        'scale its trainers up to the player. Only dedication/Easter egg teams are exempt.', '',
        '## Progression targets', '',
        '| Section | Normal | Easy | Hard, before badge cap | Kaizo, before badge cap |',
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
        ranges = ['100', '70', '100', '100'] if name == 'League' else [f'{ordinary_scaled(low,m,high)}–{ordinary_scaled(high,m,high)}' for m in ('Normal','Easy','Hard','Kaizo')]
        lines.append('| ' + name + ' | ' + ' | '.join(ranges) + ' |')
    lines += ['', 'The section bands describe ordinary encounter targets, not blanket map scaling.',
        'Individual teams and intentional protected exceptions are listed below.',
        'Important fights now use their own authored teams and stronger mode curve.',
        'The opening Route 103 rival has a level-9 starter on Normal.',
        'Every Elite Four and Wallace party member is fixed at 100 on Normal/Hard/Kaizo',
        'and 70 on Easy, independent of badge caps and ordinary/story scaling.', '',
        '## Mode rules', '',
        '| Rule | Easy | Normal | Hard | Kaizo |',
        '| --- | --- | --- | --- | --- |',
        '| Ordinary trainer levels | Half, with support below ace | Varied authored curve | +20%, at least +2 | +40%, at least +4 |',
        '| Story trainer levels | Half, rounded up | Authored boss curve | +25%, at least +3 | +45%, at least +5 |',
        '| IVs | 0 | At least 16 / 20 / 24 / 31 by stage | 31 | 31 |',
        '| EVs | 0 | 0 / 64 / 128 / 256 / 384 by stage | 256 / 384 / 510 by stage | 510, chosen for the set |',
        '| Ordinary moves | Stock level-up sets | Authored coverage/support | Optimized coverage with useful support | Optimized coverage with useful support |',
        '| AI | Basic bad-move checks | Full scoring, HP and KO awareness | Omniscient, smart switching | Omniscient, party knowledge and predictions |',
        '| Battle style | Shift | Set | Set | Set |',
        '| Player battle consumables | Allowed | Allowed | Blocked | Blocked |',
        '| Shared EXP | 200% | 100% | 50% | Disabled |', '',
        'See [mode targets](brisk_mode_targets.md) for party sizes, item upgrades, Easy',
        'species/gimmick rules and caps. League role training remains curated.', '',
        'Dedication/Easter egg scaling remains +20% (minimum 3) on Hard and +30%',
        '(minimum 4) on Kaizo; Easy still halves their authored levels.',
        'Ordinary and story Hard/Kaizo supports stay below the current badge cap by their',
        'authored gap from the ace, preserving a spread instead of flattening the team.',
        'Protected Hard and Kaizo trainer levels are clamped to the save’s current',
        'badge cap: **25 / 35 / 45 / 50 / 55 / 65 / 75 / 90 / 100**. The individual',
        'table assumes a cap of 100; earlier caps limit ordinary teams and clamp',
        'protected levels individually. Hard enforces EXP caps when the cap option is on;',
        'Kaizo always enforces EXP caps. Owned over-cap Pokemon are not lowered.',
        'Difficulty does not evolve an opponent into a different species at runtime.', '',
        '670 ordinary IDs use their original stage power guard. 155 story IDs now use',
        'a separate stage guard: 50 below 12; 75 below 25; 90 below 35; 100 below 50;',
        '120 thereafter. Dedication/Easter egg teams retain their previous behavior. Ordinary',
        'optimized Hard/Kaizo attacks are capped at **50 below level 12; 60 below 20;',
        '75 below 30; 90 below 40; 100 below 50; 120 thereafter**. Multi-hit, recoil,',
        'setup, typing and abilities still affect actual damage; this is a power-spike',
        'guard rather than a guarantee of equal battle difficulty. Easy uses stock',
        'level-up sets rather than the optimized-attack ceiling.', '',
        '## Overworld encounter preference', '',
        '**Options page 7 → OW ENCOUNTERS → ON/OFF.** ON is the default for old and',
        'new saves. OFF removes generated roaming Pokémon after returning to the',
        'field and uses normal random encounters instead. Scripted/static Pokémon,',
        'story encounters, followers, fishing and DexNav remain available. The',
        'preference persists in the save and is independent of difficulty.', '',
        '## Every trainer entry', '',
        'Levels show the default generated party, including mode reserves. Reserve species',
        'are selected from level-gated themed/coverage pools; their exact choice is stable',
        'for the save. The last authored slot stays last. Protected entries are byte-for-byte',
        'unchanged only for dedication/Easter eggs. Easy may devolve species and trim teams.',
        'Tutorials and League teams retain their sizes. Two-trainer half-team formats can',
        'limit a side to three; this table assumes ordinary full teams. Metadata entries supply NPC',
        'identity/templates, not playable battles; their levels are not a challenge plan.', '',
        '| Trainer ID | Display name | Role | Normal | Easy | Hard* | Kaizo* |',
        '| --- | --- | --- | --- | --- | --- | --- |',
    ]
    count=0
    for tid, team in teams.items():
        lv = party_levels(team)
        if not lv: continue
        name = ('Hooded Figure' if tid == 'TRAINER_HOODED_FIGURE' else re.search(r'^Name: (.*)',team,re.M)[1].strip().replace('|','\\|'))
        role = 'Kaizo only' if tid in policy['kaizo_bosses'] else 'Protected' if tid in policy['protected'] else 'Story' if tid in policy['story'] else 'Metadata' if tid in policy['metadata_only'] else 'Ordinary'
        cells = [' / '.join(str(n) for n in profile_levels(tid,role,lv,m)) or '—' for m in ('Normal','Easy','Hard','Kaizo')]
        lines.append('| `' + tid + '` | ' + name + ' | ' + role + ' | ' + ' | '.join(cells) + ' |')
        count += 1
    lines += ['', f'{count} populated trainer entries; *Hard/Kaizo values assume a cap of 100; earlier badge caps still apply.', '',
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
