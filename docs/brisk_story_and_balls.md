# Important fights and all-ball battle use

## Balls

All 28 ball item types use the battle Bag handler, including Poké, Great and Ultra
Balls. Those three keep their overworld-use action, but their field item type no
longer causes battle-menu dispatch to silently do nothing. Embargo treats them as
thrown balls rather than party-target items. Hard/Extreme continue to allow every
ball in wild battles. Existing no-catching flags, full storage, two-wild-Pokemon
restrictions, and explicit Bag/rental restrictions still apply.

## Important fights

155 story/important trainer entries, totaling 533 Pokemon, have been rebuilt.
This covers all May/Brendan variants, Wally, gym leaders/rematches, villain teams,
Tabitha, Maxie, Shelly, Matt, Archie, the League, Wallace, Steven and interviewers.
They now have explicit legal attacks, coverage/support, stronger IV/EV training,
useful natures/items and smart switching. Conventional evolutions match their
levels; default low-IV, unevolved midgame teams are no longer carried forward.

| Fight | Normal levels | Notes |
| --- | --- | --- |
| Opening rival | 9 | One starter |
| Rustboro rival | 22–24 | Three Pokemon; evolved starter |
| Route 110 rival | 41–44 | Four Pokemon; final starter evolution |
| Route 119 rival | 61–65 | Five Pokemon; Sceptile/Blaziken/Swampert |
| Lilycove rival | 70–74 | Six Pokemon; final starter evolution |
| Tabitha, Mt. Chimney | 46–49 | Four evolved Pokemon |
| Maxie, Mt. Chimney | 46–50 | Five Pokemon; Camerupt ace |
| Shelly, Weather Institute | 60–63 | Four Pokemon |
| Tabitha / Maxie, Magma Hideout | 68–72 / 70–74 | Five / six Pokemon |
| Tabitha / Maxie, Mossdeep | 79–82 / 80–84 | Both opponents updated |
| Shelly, Seafloor Cavern / Archie | 84–88 / 86–90 | Five / six Pokemon |
| Victory Road Wally | 92–96 | Rematches rise to 99/100 |

Gym ace targets remain 25/35/45/50/55/65/75/90; their sets and training have been
strengthened. Every League/Wallace member is level 100 on Normal, Hard and
Extreme, and level 70 on Easy. These five teams bypass ordinary/story level
scaling and badge-cap clamps. Rival starter suffixes identify
the player's starter: TORCHIC variants give the rival Swampert, TREECKO gives
Blaziken, and MUDKIP gives Sceptile. May and Brendan share progression targets.

Story modes: Easy halves levels; Hard adds 25% (minimum 3); Extreme adds 45%
(minimum 5). Hard/Extreme supports stay below the badge cap by their authored gap
from the ace, so a capped double battle does not flatten every member to one
level. Both opponent parties use the same generation path independently.

## Exemptions and validation

Only 27 dedication/Easter egg team entries are exempt. All their original raw
sections and runtime mode behavior are preserved. Twelve unused/Frontier identity
records remain metadata rather than playable encounter teams. Ordinary trainer
teams retain their separate fair curve. The party parser now recognizes the
Archive headers without spaces before the closing ===, so all four previously
missed guest-Champion entries are audited and displayed independently.

The progression audit covers every trainer reference on mapped routes, including
both members of double battles. The fairness audit verifies all classifications,
exemption fingerprints, legal story moves, EV limits, rival party sizes and all
starter variants. Automated C tests cover ball routing, Embargo, catching
restrictions, all 28 ball throws on Hard/Extreme, story classification and both
opponents' generation rules. See brisk_trainer_mode_plan.md for every team.
