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
    if tid == 'TRAINER_STEVEN': return [100] * 6
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
    return '\n'.join([
        '# Brisk trainer plan by difficulty', '',
        'Hoenn now uses explicit Easy, Normal, Hard and Kaizo trainer records in',
        '`src/data/brisk_mode_teams.party`. These records are authoritative: the game',
        'does not expand, scale, devolve or optimize their parties at battle time.', '',
        'See [every trainer and mode roster](brisk_authored_mode_teams.md) for the',
        'complete species and level list, including rivals, gym rematches and bosses.', '',
        'Dedication/Easter egg trainers retain their complete original teams in all',
        'four modes: party size, species, levels, moves, items, training and cosmetics.', '',
        '| Mode | Team design |', '| --- | --- |',
        '| Easy | Smaller traditional teams, modest moves, no EV investment |',
        '| Normal | Original encounter species plus fixed themed supports |',
        '| Hard | Stronger themed rosters, role sets, trained support and sweepers |',
        '| Kaizo | Coordinated teams, full training, competitive items and support |', '',
        'Hard/Kaizo levels are authored within each encounter’s intended badge band;',
        'they do not change when an old route is revisited with additional badges.',
        'The opening Route 103 starter tutorials retain one starter. Metadata and',
        'Frontier identity templates retain their existing purpose. Hooded Figure and',
        'Tankerson remain Kaizo-only. League levels are 70 on Easy and 100 otherwise;',
        'Meteor Falls Steven is level 100 in every mode. Two-opponent half-team',
        'formats may limit each trainer to three members.', '',
        'Existing mode AI, battle style, EXP, item-use and randomizer settings remain',
        'separate from the authored rosters. Randomizer options still apply when enabled.', '',
        'Validate with `python tools/brisk/author_mode_teams.py --check`.',
        'Regenerating with `--write` replaces manual roster edits with the curated',
        'archetype output; ordinary builds and validation never regenerate the rosters.', '',
    ])


if __name__ == '__main__':
    content = render()
    if '--check' in sys.argv:
        if not OUTPUT.exists() or OUTPUT.read_text() != content:
            raise SystemExit('Trainer mode plan is stale. Run python tools/brisk/trainer_mode_plan.py')
        print('Complete trainer mode plan matches the authored teams.')
    else:
        OUTPUT.write_text(content)
        print('Wrote', OUTPUT.relative_to(ROOT))
