# Trainer balance and legal evolution stages

The four difficulty rosters remain explicit, fixed parties. Returning to an old
route or changing the player's badge count does not change their authored
levels. Easy uses smaller parties, zero training for regular encounters and
level-appropriate stages. In the early game, Brisk's actual base stats are also
checked: a buffed species must not slip into Easy merely because its vanilla
stats were low.

## Easy progression regression targets

| Encounter | Party level range | Ace |
| --- | --- | --- |
| Roxanne | 11-13 | 13 |
| Brawly | 16-18 | 18 |
| Route 110 May / Brendan | 21-22 | 22 |
| Wattson | 21-23 | 23 |

All starter variants are checked. Combusken, Grovyle and Marshtomp require at
least level 16; the fully evolved Hoenn starters require 36. Lower-level
encounters use the preceding stage instead of raising opponents' levels.

The minimum-level graph comes from the compiled Brisk species data. It follows
whole evolution chains, regional and battle forms, item evolutions, split
Shedinja evolution, move-dependent evolutions and Brisk's custom evolution
rules. An item ID is never treated as an evolution level. Missing species fail
the audit instead of silently defaulting to level 1.

Named dedication parties retain their cross-mode identity and cosmetics, but
must also obey evolution and species/move compatibility rules. Thomas's level
50 Dragapult is now Drakloak; Joey's level 23 Gurdurr is now Timburr. The Normal
baseline is updated alongside all four mode records so regeneration preserves
these corrections.

Move validation includes legitimate cross-generation TM/tutor compatibility
from Brisk's own `all_learnables.json`; visiting trainers are not limited to
what the player's current town sells. Easy move selection favors level-up
moves and limits early power. Canonical move IDs detect duplicate slots even
when old names such as `SOLARBEAM` and `SOLAR_BEAM` refer to the same move.
Tankerson's shiny Umbreon retains Foul Play.

Repels are sold by the general marts from zero badges onward, including
Mauville before Wattson. Custom specialty shops keep their own inventories.

## Build cache protection

Every build revalidates `brisk_mode_teams.h` from its actual party sources,
including Kanto. A newer timestamp on a stale copied header cannot preserve old
trainer parties. Identical generated contents retain their timestamp to avoid
unnecessary recompilation.

## Validation and maintenance

```
python3 tools/brisk/check_trainer_progression.py
python3 tools/brisk/check_trainer_fairness.py
python3 tools/brisk/author_mode_teams.py --check
python3 tools/brisk/check_trainer_legality.py
python3 tools/brisk/test_trainer_legality.py
python3 tools/brisk/check_trainer_header.py
```

The header regression requires the built trainerproc tool. The legality audit
covers fixed Hoenn and Kanto parties, including the Normal source baseline.
Frontier identity templates are excluded because their actual parties are
constructed separately by the facility systems. Randomized species settings
are separate from these fixed-roster checks.

After changing species rules or learnsets, rebuild Emerald and regenerate the
compiled snapshot (requires pyelftools and the matching ARM compiler):

```
python3 tools/brisk/export_trainer_legality.py pokeemerald.elf
```

`repair_trainer_legality.py` can apply targeted stage/move repairs without
regenerating every team. Review its changes, update protected fingerprints
only after reviewing the affected sets, and rerun the audits. In-game
regressions cover generated Easy rival/gym levels and early-mart Repel stock.
