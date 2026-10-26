# Trainer progression

Hoenn's authored teams use one Normal-mode progression. Easy, Hard and Extreme
are applied by `GenerateMonFromTrainerMon` for both ordinary trainers and rivals.
Normal uses authored levels; Easy halves them; Hard adds 10% (at least one);
Extreme adds 20% (at least two). Hard and Extreme respect the current badge cap.
Their IV, EV, ability, moveset and AI upgrades still apply at the cap.

| Encounter | Normal ace |
| --- | ---: |
| First rival, Route 103 | 7 |
| Rustboro rival / Roxanne | 24 / 25 |
| Brawly | 35 |
| Mauville Wally / Route 110 rival / Wattson | 41 / 42 / 45 |
| Flannery / Norman | 50 / 55 |
| Route 119 rival / Winona | 63 / 65 |
| Lilycove rival / Tate and Liza | 72 / 75 |
| Juan / Victory Road Wally | 90 / 94 |

Routes, gyms and villain encounters now have area-specific level bands. Split
routes account for Surf access, the northern Route 115 beach and Route 111's
northern road/desert. All six starter/gender variants share each rival target;
later rival starters and partners evolve. Gym aces are at most four levels above
their teammates. Rematches cannot drop below the preceding team's ace. Archive,
dedication, Frontier and Dynamax Adventure teams retain their separate designs.

Hard and Extreme always enable basic move scoring, including trainers whose
authored AI only enables viability or setup. Normal retains authored AI.

Run `python tools/brisk/check_trainer_progression.py` to audit the actual party
file and map scripts. It checks area bands, gym/rival targets, late rival
evolutions and the actual rematch table (numbered grunts are separate trainers).
The C tests also exercise difficulty through the party generator; the test
runner's replacement trainer table is not used as evidence for authored teams.
