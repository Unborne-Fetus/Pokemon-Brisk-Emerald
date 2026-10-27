# Difficulty targets

| Mode | Intended experience | Default combat rules |
| --- | --- | --- |
| Easy | Around a typical Pokemon game | Smaller teams, level-appropriate evolutions, stock level-up moves, basic AI, Shift and battle items |
| Normal | Difficult | More complete teams, stronger staged training, sensible scoring and KO awareness, Set; battle items available |
| Hard | Very difficult; repeated attempts expected | Larger teams, perfect IVs and stronger EVs, optimized coverage/support and held items, omniscient switching AI, Set and no player consumables |
| Kaizo | Kaizo-style challenge target | Full teams from midgame, perfect IVs/510 EVs, coverage/support, competitive held items, full party knowledge and switch/move prediction, strict caps, Set and no player consumables |

Exact Emerald Kaizo parity is a playtesting target, not something automated tests
can certify. These are stronger default rules and scalable teams, with meaningful
mode separation. Route-by-route losses, wins and resource availability should
inform the next balance pass. Retry remains available. Debug, randomizers and
custom options can change the experience.

## Teams

Counts below are minimum targets: larger authored teams remain larger except
when Easy deliberately trims them. League and tutorial sizes are fixed.

| Mode | Ordinary minimum | Story/boss minimum |
| --- | --- | --- |
| Easy | Maximum 2 below authored level 40, 3 below 70, then 4 | Maximum 2 below 30, 3 below 50, 4 below 80, then 5 |
| Normal | 2 below 20, 3 below 40, then 4 | Same minimum, preserving larger authored parties |
| Hard | 3 below 20, 4 below 40, then 5 | 4 below 25, 5 below 40, then 6 |
| Kaizo | 4 below 20, 5 below 30, then 6 | 4 below 20, then 6 |

The threshold uses the team's highest authored Normal level. Two-opponent
half-team battles still obey their side limits. Added reserves are level-gated,
prefer the trainer class's type and fill missing type coverage. They use distinct
families and the strongest eligible stage rather than padding a late team with
basic forms. Selection is stable for the save and does not consume battle RNG.
The last authored slot stays last; reserves are inserted before it.

Easy devolves premature evolved opponents according to their generated level,
removes non-League Mega/Primal/Z items and disables their Tera/G-Max use. This is
why an Easy opening gym no longer keeps its Normal-mode final evolutions. The
League retains all six curated members at 70, as requested.

Normal adds IV floors of 16 below 20, 20 below 35, 24 below 50 and 31 thereafter.
Its EV budgets are 0 below 15, 64 below 25, 128 below 40, 256 below 60 and 384 later.
Hard uses 31 IVs and 256 EVs below 20, 384 below 35 and 510 later. Kaizo uses
31 IVs and 510 EVs throughout. Higher authored training totals are preserved;
stat caps remain 252 per stat / 510 total. League role spreads are preserved on
Normal/Hard/Kaizo; Easy retains its reduced IV/EV training.

Hard/Kaizo generate useful coverage and support within the existing stage
power limits. All their non-League members receive appropriate held items: Oran
before 25, Sitrus before 40, then offensive Life Orb or recovery Leftovers; Guts
sets receive Flame Orb from 30. Authored specialized items and gimmicks remain.
Curated League moves, abilities, items, natures and role spreads stay intact.

## Caps, AI and exceptions

Hard stops battle EXP, candy growth and daycare growth at the current badge cap
by default. The existing cap-off preference still works on Hard. Kaizo always
stops growth at its badge cap. Existing over-cap owned Pokemon are not lowered.
League opponents override progression caps: 100 on Normal/Hard/Kaizo, 70 Easy.
Normal retains authored levels and its existing cap/obedience preference.

Normal's general profile does not receive hidden-move/item/ability knowledge or
prediction flags. Hard receives full knowledge and smart switching. Kaizo adds
full party knowledge, move/switch predictions and conservative damage estimates.
This uses the engine's prediction logic; it does not directly read the player's
uncommitted button input. Easy wild AI uses basic move checks too.

All 27 dedication/Easter egg teams retain their authored parties, levels, training
behavior and previous mode AI modifiers. Rental/Frontier identities are not padded by these rules. The Mossdeep Steven
partner is separately aligned to the story curve: three Pokemon on Easy/Normal,
six on Hard/Kaizo when full parties are available, with evolved, useful sets
and levels matched to Maxie/Tabitha rather than the old level-42 template. Other
partner identities retain their existing behavior. Balls remain usable on every mode, subject to ordinary
catching restrictions and the explicit Bag preference. Prices, shared EXP rates
and existing convenience options keep their previous settings.

`brisk_trainer_mode_plan.md` lists generated default party levels, including
reserves, for every authored encounter. The C tests include the actual generated
trainer source so progression, party sizes, early Easy evolutions, held items,
training and League/exemption behavior are exercised through the real party path.
