# Ordinary trainer team rework

Current generated rosters and training: [difficulty targets](brisk_mode_targets.md).

670 ordinary Hoenn trainer entries (1,309 Pokémon, including rematches) have
revised species, moves, items and training. This follows the area-level pass;
lowering an evolved Pokémon's level alone did not make early fights fair.

## Protected encounters

Only the 27 dedication/Easter egg entries retain their exact definitions and
previous runtime behavior. These include Queen, Sugimori, Elvis, Manolete, Flo,
Spro, Neo, Sam, Scotty, Unborne, Joey and the Archive guest Champions. Twelve
metadata-only records provide unused/Frontier identity templates rather than
playable authored fights.

Story trainers, rivals, gym leaders, Elite Four members, Wallace, villain teams,
Steven, Winstrates and interviewers are now eligible for balance changes. See
[the story-team pass](brisk_story_and_balls.md) for the current targets.

## Ordinary encounters

- Retain an existing Pokémon's family when it fits the encounter. Devolve
  premature evolutions; otherwise use an appropriate species for the trainer
  class. Respect Brisk's actual evolution levels and modified base stats.
- Remove legendaries, mythicals, Ultra Beasts, Paradox Pokémon, temporary battle
  forms and Mega stones from ordinary teams. Bosses retain their gimmicks.
- Use legal level-up and inherited moves, with a damage-power ceiling that
  rises from 50 early on to 120 late in the game. Ordinary authored sets avoid
  OHKO moves, evasion spam, suicide moves and trapping-only Wobbuffet teams.
- Give later teams coverage and one useful support/setup move where available.
  Normal's basic trainer AI evaluates moves without omniscient knowledge.
- Authored IVs progress through 8 / 12 / 18 / 24 before runtime mode floors. No early EV training; later
  spreads total 128, 256 or 384. Beneficial natures start at level 25.
- Limit early party size; preserve doubles and existing trainer identities.
  Only the final party slot gets a held item: Oran from level 12, Sitrus from
  level 35, Sitrus or Expert Belt from level 50. Remove trainer healing-item
  inventories from ordinary fights.

The existing Easy/Hard/Extreme modifiers still apply at battle time, including
Extreme's stronger moves and full EV training. Ordinary trainers now also respect
the stage power ceilings when Hard/Extreme generates moves; Easy uses stock level-up sets. Dedication/Easter egg trainer
IDs retain their previous runtime behavior. See [the complete mode plan](brisk_trainer_mode_plan.md)
for every trainer's levels across all four modes and the revised opening curve.

## Examples

| Trainer | Before | After |
| --- | --- | --- |
| Allen, Route 102 | Level 7 Galarian Slowking and Slowbro | Galarian Slowpoke at 13 and Rattata at 11 |
| Rick, Route 102 | Armaldo and Scolipede with Expert Belts | Stage-appropriate bugs at levels 12–13 |
| Tiana, Route 102 | Ursaring and Zangoose with Expert Belts | Teddiursa at 12 and Furret at 15; one Oran Berry |
| Gina & Mia, Route 104 | Indeedee pair with Psychic Seed/Choice Scarf | Lombre and Zigzagoon; one Oran Berry |
| Marc, Rustboro Gym | Golem with Life Orb and Coalossal with Leftovers | Geodude and Carkol; one Oran Berry |

## Validation

Run `python tools/brisk/check_trainer_progression.py` and
`python tools/brisk/check_trainer_fairness.py`. Both run in the GBA build workflow.
The fairness policy fingerprints protected entries and records the compiled
Brisk stats, minimum evolution levels and learnsets used for this pass. Refresh
that catalog deliberately if species data changes; update a protected fingerprint
only when an explicitly requested team edit is intended. A full ROM build also
validates the trainer processor's species, move and item definitions.

Battle balance still benefits from playtesting, especially where several
ordinary trainers must be fought consecutively without healing.
