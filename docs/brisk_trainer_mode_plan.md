# Brisk trainer plan by difficulty

This is the complete Hoenn trainer plan, including ordinary trainers, rematches,
story fights, rivals, gyms, the League and dedication/Easter egg encounters.
Normal levels are authored per encounter; revisiting an old route does not
scale its trainers up to the player. Only dedication/Easter egg teams are exempt.

## Progression targets

| Section | Normal | Easy | Hard, before badge cap | Extreme, before badge cap |
| --- | --- | --- | --- | --- |
| Route 102 | 10–15 | 5–8 | 12–18 | 14–21 |
| Route 104 | 14–21 | 7–11 | 16–25 | 19–29 |
| Petalburg Woods | 16–20 | 8–10 | 19–24 | 22–28 |
| Route 116 / Rusturf approach | 20–24 | 10–12 | 24–28 | 28–33 |
| Rustboro Gym trainers | 21–24 | 11–12 | 25–28 | 29–33 |
| Dewford / Brawly approach | 28–34 | 14–17 | 33–40 | 39–47 |
| Slateport / Route 110 | 35–42 | 18–21 | 42–50 | 49–58 |
| Wattson approach | 40–44 | 20–22 | 48–52 | 56–61 |
| Northern routes / Flannery approach | 43–49 | 22–25 | 51–58 | 60–68 |
| Norman approach | 51–54 | 26–27 | 61–64 | 71–75 |
| Route 119 / Fortree | 57–64 | 29–32 | 68–76 | 79–89 |
| Lilycove / hideouts | 66–73 | 33–37 | 79–87 | 92–100 |
| Mossdeep Gym approach | 71–74 | 36–37 | 85–88 | 97–100 |
| Space Center / eastern sea | 76–83 | 38–42 | 91–99 | 93–100 |
| Seafloor / Juan approach | 82–89 | 41–45 | 93–100 | 93–100 |
| Victory Road | 86–94 | 43–47 | 92–100 | 92–100 |
| League | 100 | 70 | 100 | 100 |

The section bands describe ordinary encounter targets, not blanket map scaling.
Individual teams and intentional protected exceptions are listed below.
Important fights now use their own authored teams and stronger mode curve.
The opening Route 103 rival has a level-9 starter on Normal.
Every Elite Four and Wallace party member is fixed at 100 on Normal/Hard/Extreme
and 70 on Easy, independent of badge caps and ordinary/story scaling.

## Mode rules

| Rule | Easy | Normal | Hard | Extreme |
| --- | --- | --- | --- | --- |
| Ordinary trainer levels | Half, with support below ace | Varied authored curve | +20%, at least +2 | +40%, at least +4 |
| Story trainer levels | Half, rounded up | Authored boss curve | +25%, at least +3 | +45%, at least +5 |
| IVs | 0 | 8 / 12 / 18 / 24 by stage | 31 | 31 |
| EVs | 0 | 0 / 128 / 256 / 384 by stage | 128 / 256 / 384 / 510 (at least authored total) | 510, chosen for the set |
| Ordinary moves | Weaker legal attacks | Legal authored coverage/support | Optimized ordinary moves and stronger abilities | Optimized legal moves within stage power limits |
| AI | Basic bad-move checks | Basic move scoring | Smart switching, HP and KO checks | Omniscient with smart switching |
| Battle style | Shift | Player preference | Set | Set |
| Player battle consumables | Allowed | Allowed | Blocked | Blocked |
| Shared EXP | 200% | 100% | 50% | Disabled |

Dedication/Easter egg scaling remains +20% (minimum 3) on Hard and +30%
(minimum 4) on Extreme; Easy still halves their authored levels.
Ordinary and story Hard/Extreme supports stay below the current badge cap by their
authored gap from the ace, preserving a spread instead of flattening the team.
Protected Hard and Extreme trainer levels are clamped to the save’s current
badge cap: **25 / 35 / 45 / 50 / 55 / 65 / 75 / 90 / 100**. The individual
table assumes a cap of 100; earlier caps limit ordinary teams and clamp
protected levels individually. Extreme enforces the player EXP cap too.
Difficulty does not evolve an opponent into a different species at runtime.

670 ordinary IDs use their original stage power guard. 155 story IDs now use
a separate stage guard: 50 below 12; 75 below 25; 90 below 35; 100 below 50;
120 thereafter. Dedication/Easter egg teams retain their previous behavior. Ordinary
generated attacks are capped at base power **50 below level 12; 60 below 20;
75 below 30; 90 below 40; 100 below 50; 120 thereafter**. Multi-hit, recoil,
setup, typing and abilities still affect actual damage; this is a power-spike
guard rather than a guarantee of equal battle difficulty.

## Overworld encounter preference

**Options page 7 → OW ENCOUNTERS → ON/OFF.** ON is the default for old and
new saves. OFF removes generated roaming Pokémon after returning to the
field and uses normal random encounters instead. Scripted/static Pokémon,
story encounters, followers, fishing and DexNav remain available. The
preference persists in the save and is independent of difficulty.

## Every trainer entry

Levels are in authored party order. Protected entries are byte-for-byte
unchanged only for dedication/Easter eggs. Metadata-only entries supply NPC
identity/templates, not playable battles; their levels are not a challenge plan.

| Trainer ID | Display name | Role | Normal | Easy | Hard* | Extreme* |
| --- | --- | --- | --- | --- | --- | --- |
| `TRAINER_SAWYER_1` | SAWYER | Ordinary | 46 | 23 | 55 | 64 |
| `TRAINER_GRUNT_AQUA_HIDEOUT_1` | GRUNT | Story | 73 | 37 | 91 | 100 |
| `TRAINER_GRUNT_AQUA_HIDEOUT_2` | GRUNT | Story | 72 / 73 | 36 / 37 | 90 / 91 | 99 / 100 |
| `TRAINER_GRUNT_AQUA_HIDEOUT_3` | GRUNT | Story | 73 | 37 | 91 | 100 |
| `TRAINER_GRUNT_AQUA_HIDEOUT_4` | GRUNT | Story | 73 | 37 | 91 | 100 |
| `TRAINER_GRUNT_SEAFLOOR_CAVERN_1` | GRUNT | Story | 87 | 44 | 100 | 100 |
| `TRAINER_GRUNT_SEAFLOOR_CAVERN_2` | GRUNT | Story | 87 | 44 | 100 | 100 |
| `TRAINER_GRUNT_SEAFLOOR_CAVERN_3` | GRUNT | Story | 87 | 44 | 100 | 100 |
| `TRAINER_GABRIELLE_1` | GABRIELLE | Ordinary | 66 / 66 / 66 / 66 / 66 / 67 | 33 / 33 / 33 / 33 / 33 / 34 | 79 / 79 / 79 / 79 / 79 / 80 | 92 / 92 / 92 / 92 / 92 / 93 |
| `TRAINER_GRUNT_PETALBURG_WOODS` | GRUNT | Story | 20 | 10 | 25 | 29 |
| `TRAINER_MARCEL` | MARCEL | Ordinary | 66 / 67 | 33 / 34 | 79 / 80 | 92 / 93 |
| `TRAINER_ALBERTO` | ALBERTO | Ordinary | 66 / 67 | 33 / 34 | 79 / 80 | 92 / 93 |
| `TRAINER_ED` | ED | Ordinary | 66 / 67 | 33 / 34 | 79 / 80 | 92 / 93 |
| `TRAINER_GRUNT_SEAFLOOR_CAVERN_4` | GRUNT | Story | 87 | 44 | 100 | 100 |
| `TRAINER_DECLAN` | DECLAN | Ordinary | 68 | 34 | 81 | 95 |
| `TRAINER_GRUNT_RUSTURF_TUNNEL` | GRUNT | Story | 24 | 12 | 30 | 34 |
| `TRAINER_GRUNT_WEATHER_INST_1` | GRUNT | Story | 60 / 61 | 30 / 31 | 75 / 76 | 87 / 88 |
| `TRAINER_GRUNT_WEATHER_INST_2` | GRUNT | Story | 61 / 62 | 31 / 31 | 76 / 77 | 88 / 89 |
| `TRAINER_GRUNT_WEATHER_INST_3` | GRUNT | Story | 60 / 61 / 62 | 30 / 31 / 31 | 75 / 76 / 77 | 87 / 88 / 89 |
| `TRAINER_GRUNT_MUSEUM_1` | GRUNT | Story | 40 | 20 | 50 | 58 |
| `TRAINER_GRUNT_MUSEUM_2` | GRUNT | Story | 39 / 40 | 20 / 20 | 48 / 50 | 56 / 58 |
| `TRAINER_GRUNT_SPACE_CENTER_1` | GRUNT | Story | 80 | 40 | 100 | 100 |
| `TRAINER_GRUNT_MT_PYRE_1` | GRUNT | Story | 70 | 35 | 87 | 100 |
| `TRAINER_GRUNT_MT_PYRE_2` | GRUNT | Story | 70 | 35 | 87 | 100 |
| `TRAINER_GRUNT_MT_PYRE_3` | GRUNT | Story | 69 / 70 | 35 / 35 | 86 / 87 | 99 / 100 |
| `TRAINER_GRUNT_WEATHER_INST_4` | GRUNT | Story | 61 | 31 | 76 | 88 |
| `TRAINER_GRUNT_AQUA_HIDEOUT_5` | GRUNT | Story | 73 | 37 | 91 | 100 |
| `TRAINER_GRUNT_AQUA_HIDEOUT_6` | GRUNT | Story | 73 | 37 | 91 | 100 |
| `TRAINER_FREDRICK` | FREDRICK | Ordinary | 66 / 67 | 33 / 34 | 79 / 80 | 92 / 93 |
| `TRAINER_MATT` | MATT | Story | 70 / 71 / 72 / 73 | 35 / 36 / 36 / 37 | 87 / 88 / 90 / 91 | 97 / 98 / 99 / 100 |
| `TRAINER_ZANDER` | ZANDER | Ordinary | 67 | 34 | 80 | 93 |
| `TRAINER_SHELLY_WEATHER_INSTITUTE` | SHELLY | Story | 60 / 61 / 62 / 63 | 30 / 31 / 31 / 32 | 75 / 76 / 77 / 78 | 87 / 88 / 89 / 91 |
| `TRAINER_SHELLY_SEAFLOOR_CAVERN` | SHELLY | Story | 84 / 85 / 86 / 87 / 88 | 42 / 43 / 43 / 44 / 44 | 96 / 97 / 98 / 99 / 100 | 96 / 97 / 98 / 99 / 100 |
| `TRAINER_ARCHIE` | ARCHIE | Story | 86 / 86 / 87 / 88 / 89 / 90 | 43 / 43 / 44 / 44 / 45 / 45 | 96 / 96 / 97 / 98 / 99 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_LEAH` | LEAH | Ordinary | 67 | 34 | 80 | 93 |
| `TRAINER_DAISY` | DAISY | Ordinary | 35 / 36 | 17 / 18 | 42 / 43 | 49 / 50 |
| `TRAINER_ROSE_1` | ROSE | Ordinary | 40 / 40 / 41 | 20 / 20 / 21 | 48 / 48 / 49 | 56 / 56 / 57 |
| `TRAINER_FELIX` | FELIX | Ordinary | 88 / 89 | 44 / 45 | 99 / 100 | 99 / 100 |
| `TRAINER_VIOLET` | VIOLET | Ordinary | 66 / 67 | 33 / 34 | 79 / 80 | 92 / 93 |
| `TRAINER_ROSE_2` | ROSE | Ordinary | 41 / 43 | 21 / 22 | 49 / 51 | 57 / 60 |
| `TRAINER_ROSE_3` | ROSE | Ordinary | 42 / 44 / 46 | 21 / 22 / 23 | 50 / 52 / 55 | 58 / 61 / 64 |
| `TRAINER_ROSE_4` | ROSE | Ordinary | 45 / 47 / 49 | 23 / 24 / 25 | 54 / 56 / 58 | 63 / 65 / 68 |
| `TRAINER_ROSE_5` | ROSE | Ordinary | 48 / 50 / 52 | 24 / 25 / 26 | 57 / 60 / 62 | 67 / 70 / 72 |
| `TRAINER_DUSTY_1` | DUSTY | Ordinary | 46 | 23 | 55 | 64 |
| `TRAINER_CHIP` | CHIP | Ordinary | 59 / 60 / 61 | 30 / 30 / 31 | 70 / 72 / 73 | 82 / 84 / 85 |
| `TRAINER_FOSTER` | FOSTER | Ordinary | 55 / 56 | 27 / 28 | 66 / 67 | 77 / 78 |
| `TRAINER_DUSTY_2` | DUSTY | Ordinary | 49 | 25 | 58 | 68 |
| `TRAINER_DUSTY_3` | DUSTY | Ordinary | 52 | 26 | 62 | 72 |
| `TRAINER_DUSTY_4` | DUSTY | Ordinary | 55 | 28 | 66 | 77 |
| `TRAINER_DUSTY_5` | DUSTY | Ordinary | 58 | 29 | 69 | 81 |
| `TRAINER_GABBY_AND_TY_1` | GABBY & TY | Story | 43 / 44 | 22 / 22 | 53 / 55 | 62 / 63 |
| `TRAINER_GABBY_AND_TY_2` | GABBY & TY | Story | 45 / 46 | 23 / 23 | 56 / 57 | 65 / 66 |
| `TRAINER_GABBY_AND_TY_3` | GABBY & TY | Story | 59 / 60 | 30 / 30 | 73 / 75 | 85 / 87 |
| `TRAINER_GABBY_AND_TY_4` | GABBY & TY | Story | 63 / 64 | 32 / 32 | 78 / 80 | 91 / 92 |
| `TRAINER_GABBY_AND_TY_5` | GABBY & TY | Story | 73 / 74 | 37 / 37 | 91 / 92 | 99 / 100 |
| `TRAINER_GABBY_AND_TY_6` | GABBY & TY | Story | 85 / 86 | 43 / 43 | 99 / 100 | 99 / 100 |
| `TRAINER_LOLA_1` | LOLA | Ordinary | 35 / 36 | 17 / 18 | 42 / 43 | 49 / 50 |
| `TRAINER_AUSTINA` | AUSTINA | Ordinary | 35 | 18 | 42 | 49 |
| `TRAINER_GWEN` | GWEN | Ordinary | 35 | 18 | 42 | 49 |
| `TRAINER_LOLA_2` | LOLA | Ordinary | 36 / 38 | 18 / 19 | 43 / 45 | 50 / 53 |
| `TRAINER_LOLA_3` | LOLA | Ordinary | 39 / 41 | 20 / 21 | 46 / 49 | 54 / 57 |
| `TRAINER_LOLA_4` | LOLA | Ordinary | 42 / 44 | 21 / 22 | 50 / 52 | 58 / 61 |
| `TRAINER_LOLA_5` | LOLA | Ordinary | 45 / 47 | 23 / 24 | 54 / 56 | 63 / 65 |
| `TRAINER_RICKY_1` | RICKY | Ordinary | 35 | 18 | 42 | 49 |
| `TRAINER_SIMON` | SIMON | Ordinary | 35 / 36 | 17 / 18 | 42 / 43 | 49 / 50 |
| `TRAINER_CHARLIE` | CHARLIE | Ordinary | 56 | 28 | 67 | 78 |
| `TRAINER_RICKY_2` | RICKY | Ordinary | 38 | 19 | 45 | 53 |
| `TRAINER_RICKY_3` | RICKY | Ordinary | 41 | 21 | 49 | 57 |
| `TRAINER_RICKY_4` | RICKY | Ordinary | 44 | 22 | 52 | 61 |
| `TRAINER_RICKY_5` | RICKY | Ordinary | 47 | 24 | 56 | 65 |
| `TRAINER_RANDALL` | RANDALL | Ordinary | 52 | 26 | 62 | 72 |
| `TRAINER_PARKER` | PARKER | Ordinary | 51 | 26 | 61 | 71 |
| `TRAINER_GEORGE` | GEORGE | Ordinary | 51 | 26 | 61 | 71 |
| `TRAINER_BERKE` | BERKE | Ordinary | 51 | 26 | 61 | 71 |
| `TRAINER_BRAXTON` | BRAXTON | Ordinary | 66 / 66 / 66 / 66 / 67 | 33 / 33 / 33 / 33 / 34 | 79 / 79 / 79 / 79 / 80 | 92 / 92 / 92 / 92 / 93 |
| `TRAINER_VINCENT` | VINCENT | Ordinary | 90 / 92 / 94 | 45 / 46 / 47 | 96 / 98 / 100 | 96 / 98 / 100 |
| `TRAINER_LEROY` | LEROY | Ordinary | 93 / 95 | 47 / 48 | 98 / 100 | 98 / 100 |
| `TRAINER_WILTON_1` | WILTON | Ordinary | 45 / 45 / 46 | 22 / 22 / 23 | 54 / 54 / 55 | 63 / 63 / 64 |
| `TRAINER_EDGAR` | EDGAR | Ordinary | 86 / 87 | 43 / 44 | 99 / 100 | 99 / 100 |
| `TRAINER_ALBERT` | ALBERT | Ordinary | 86 / 87 | 43 / 44 | 99 / 100 | 99 / 100 |
| `TRAINER_SAMUEL` | SAMUEL | Ordinary | 87 / 87 / 88 | 43 / 43 / 44 | 99 / 99 / 100 | 99 / 99 / 100 |
| `TRAINER_VITO` | VITO | Ordinary | 88 / 88 / 88 / 89 | 44 / 44 / 44 / 45 | 99 / 99 / 99 / 100 | 99 / 99 / 99 / 100 |
| `TRAINER_OWEN` | OWEN | Ordinary | 88 / 88 / 89 | 44 / 44 / 45 | 99 / 99 / 100 | 99 / 99 / 100 |
| `TRAINER_WILTON_2` | WILTON | Ordinary | 44 / 46 / 48 | 22 / 23 / 24 | 52 / 55 / 57 | 61 / 64 / 67 |
| `TRAINER_WILTON_3` | WILTON | Ordinary | 47 / 49 / 51 | 24 / 25 / 26 | 56 / 58 / 61 | 65 / 68 / 71 |
| `TRAINER_WILTON_4` | WILTON | Ordinary | 50 / 52 / 54 | 25 / 26 / 27 | 60 / 62 / 64 | 70 / 72 / 75 |
| `TRAINER_WILTON_5` | WILTON | Ordinary | 53 / 55 / 57 | 27 / 28 / 29 | 63 / 66 / 68 | 74 / 77 / 79 |
| `TRAINER_WARREN` | WARREN | Ordinary | 81 / 82 | 40 / 41 | 97 / 98 | 99 / 100 |
| `TRAINER_MARY` | MARY | Ordinary | 51 | 26 | 61 | 71 |
| `TRAINER_ALEXIA` | ALEXIA | Ordinary | 51 | 26 | 61 | 71 |
| `TRAINER_JODY` | JODY | Ordinary | 51 | 26 | 61 | 71 |
| `TRAINER_WENDY` | WENDY | Ordinary | 66 / 66 / 67 | 33 / 33 / 34 | 79 / 79 / 80 | 92 / 92 / 93 |
| `TRAINER_KEIRA` | KEIRA | Ordinary | 90 / 92 | 45 / 46 | 98 / 100 | 98 / 100 |
| `TRAINER_BROOKE_1` | BROOKE | Ordinary | 45 / 45 / 46 | 22 / 22 / 23 | 54 / 54 / 55 | 63 / 63 / 64 |
| `TRAINER_JENNIFER` | JENNIFER | Ordinary | 61 | 31 | 73 | 85 |
| `TRAINER_HOPE` | HOPE | Ordinary | 88 | 44 | 100 | 100 |
| `TRAINER_SHANNON` | SHANNON | Ordinary | 88 | 44 | 100 | 100 |
| `TRAINER_MICHELLE` | MICHELLE | Ordinary | 87 / 87 / 88 | 43 / 43 / 44 | 99 / 99 / 100 | 99 / 99 / 100 |
| `TRAINER_CAROLINE` | CAROLINE | Ordinary | 88 / 89 | 44 / 45 | 99 / 100 | 99 / 100 |
| `TRAINER_JULIE` | JULIE | Ordinary | 88 / 88 / 89 | 44 / 44 / 45 | 99 / 99 / 100 | 99 / 99 / 100 |
| `TRAINER_BROOKE_2` | BROOKE | Ordinary | 44 / 46 / 48 | 22 / 23 / 24 | 52 / 55 / 57 | 61 / 64 / 67 |
| `TRAINER_BROOKE_3` | BROOKE | Ordinary | 47 / 49 / 51 | 24 / 25 / 26 | 56 / 58 / 61 | 65 / 68 / 71 |
| `TRAINER_BROOKE_4` | BROOKE | Ordinary | 50 / 52 / 54 | 25 / 26 / 27 | 60 / 62 / 64 | 70 / 72 / 75 |
| `TRAINER_BROOKE_5` | BROOKE | Ordinary | 55 / 55 / 57 | 28 / 28 / 29 | 66 / 66 / 68 | 77 / 77 / 79 |
| `TRAINER_PATRICIA` | PATRICIA | Ordinary | 76 / 77 | 38 / 39 | 91 / 92 | 99 / 100 |
| `TRAINER_KINDRA` | KINDRA | Ordinary | 66 / 67 | 33 / 34 | 79 / 80 | 92 / 93 |
| `TRAINER_TAMMY` | TAMMY | Ordinary | 66 / 67 | 33 / 34 | 79 / 80 | 92 / 93 |
| `TRAINER_VALERIE_1` | VALERIE | Ordinary | 67 | 34 | 80 | 93 |
| `TRAINER_TASHA` | TASHA | Ordinary | 67 | 34 | 80 | 93 |
| `TRAINER_VALERIE_2` | VALERIE | Ordinary | 68 / 70 | 34 / 35 | 81 / 84 | 95 / 98 |
| `TRAINER_VALERIE_3` | VALERIE | Ordinary | 71 / 73 | 36 / 37 | 85 / 87 | 98 / 100 |
| `TRAINER_VALERIE_4` | VALERIE | Ordinary | 74 / 76 | 37 / 38 | 88 / 91 | 98 / 100 |
| `TRAINER_VALERIE_5` | VALERIE | Ordinary | 75 / 77 / 79 | 38 / 39 / 40 | 90 / 92 / 94 | 96 / 98 / 100 |
| `TRAINER_CINDY_1` | CINDY | Ordinary | 14 | 7 | 16 | 19 |
| `TRAINER_DAPHNE` | DAPHNE | Ordinary | 85 / 86 | 42 / 43 | 99 / 100 | 99 / 100 |
| `TRAINER_GRUNT_SPACE_CENTER_2` | GRUNT | Story | 78 / 79 / 80 | 39 / 40 / 40 | 97 / 98 / 100 | 98 / 99 / 100 |
| `TRAINER_CINDY_2` | CINDY | Ordinary | 21 | 11 | 25 | 29 |
| `TRAINER_BRIANNA` | BRIANNA | Ordinary | 87 | 44 | 100 | 100 |
| `TRAINER_NAOMI` | NAOMI | Ordinary | 60 | 30 | 72 | 84 |
| `TRAINER_CINDY_3` | CINDY | Ordinary | 39 | 20 | 46 | 54 |
| `TRAINER_CINDY_4` | CINDY | Ordinary | 45 | 23 | 54 | 63 |
| `TRAINER_CINDY_5` | CINDY | Ordinary | 48 | 24 | 57 | 67 |
| `TRAINER_CINDY_6` | CINDY | Ordinary | 51 | 26 | 61 | 71 |
| `TRAINER_MELISSA` | MELISSA | Ordinary | 47 | 24 | 56 | 65 |
| `TRAINER_SHEILA` | SHEILA | Ordinary | 47 | 24 | 56 | 65 |
| `TRAINER_SHIRLEY` | SHIRLEY | Ordinary | 47 | 24 | 56 | 65 |
| `TRAINER_JESSICA_1` | JESSICA | Ordinary | 66 / 67 | 33 / 34 | 79 / 80 | 92 / 93 |
| `TRAINER_CONNIE` | CONNIE | Ordinary | 87 | 44 | 100 | 100 |
| `TRAINER_BRIDGET` | BRIDGET | Ordinary | 87 | 44 | 100 | 100 |
| `TRAINER_OLIVIA` | OLIVIA | Ordinary | 85 / 85 / 86 | 42 / 42 / 43 | 99 / 99 / 100 | 99 / 99 / 100 |
| `TRAINER_TIFFANY` | TIFFANY | Ordinary | 85 / 86 | 42 / 43 | 99 / 100 | 99 / 100 |
| `TRAINER_JESSICA_2` | JESSICA | Ordinary | 68 / 70 | 34 / 35 | 81 / 84 | 95 / 98 |
| `TRAINER_JESSICA_3` | JESSICA | Ordinary | 71 / 73 | 36 / 37 | 85 / 87 | 98 / 100 |
| `TRAINER_JESSICA_4` | JESSICA | Ordinary | 74 / 76 | 37 / 38 | 88 / 91 | 98 / 100 |
| `TRAINER_JESSICA_5` | JESSICA | Ordinary | 77 / 79 | 39 / 40 | 92 / 94 | 98 / 100 |
| `TRAINER_WINSTON_1` | WINSTON | Ordinary | 14 | 7 | 16 | 19 |
| `TRAINER_MOLLIE` | MOLLIE | Ordinary | 81 / 82 | 40 / 41 | 97 / 98 | 99 / 100 |
| `TRAINER_GARRET` | GARRET | Ordinary | 53 | 27 | 63 | 74 |
| `TRAINER_WINSTON_2` | WINSTON | Ordinary | 37 | 19 | 44 | 51 |
| `TRAINER_WINSTON_3` | WINSTON | Ordinary | 43 | 22 | 51 | 60 |
| `TRAINER_WINSTON_4` | WINSTON | Ordinary | 46 | 23 | 55 | 64 |
| `TRAINER_WINSTON_5` | WINSTON | Ordinary | 50 | 25 | 60 | 70 |
| `TRAINER_STEVE_1` | STEVE | Ordinary | 44 | 22 | 52 | 61 |
| `TRAINER_THALIA_1` | THALIA | Ordinary | 56 / 57 | 28 / 29 | 67 / 68 | 78 / 79 |
| `TRAINER_MARK` | MARK | Ordinary | 67 | 34 | 80 | 93 |
| `TRAINER_GRUNT_MT_CHIMNEY_1` | GRUNT | Story | 49 | 25 | 61 | 71 |
| `TRAINER_STEVE_2` | STEVE | Ordinary | 47 | 24 | 56 | 65 |
| `TRAINER_STEVE_3` | STEVE | Ordinary | 48 / 50 | 24 / 25 | 57 / 60 | 67 / 70 |
| `TRAINER_STEVE_4` | STEVE | Ordinary | 51 / 53 | 26 / 27 | 61 / 63 | 71 / 74 |
| `TRAINER_STEVE_5` | STEVE | Ordinary | 54 / 56 | 27 / 28 | 64 / 67 | 75 / 78 |
| `TRAINER_LUIS` | LUIS | Ordinary | 56 | 28 | 67 | 78 |
| `TRAINER_DOMINIK` | DOMINIK | Ordinary | 55 | 28 | 66 | 77 |
| `TRAINER_DOUGLAS` | DOUGLAS | Ordinary | 55 / 56 | 27 / 28 | 66 / 67 | 77 / 78 |
| `TRAINER_DARRIN` | DARRIN | Ordinary | 55 / 55 / 56 | 27 / 27 / 28 | 66 / 66 / 67 | 77 / 77 / 78 |
| `TRAINER_TONY_1` | TONY | Ordinary | 55 | 28 | 66 | 77 |
| `TRAINER_JEROME` | JEROME | Ordinary | 56 | 28 | 67 | 78 |
| `TRAINER_MATTHEW` | MATTHEW | Ordinary | 55 | 28 | 66 | 77 |
| `TRAINER_DAVID` | DAVID | Ordinary | 55 / 56 | 27 / 28 | 66 / 67 | 77 / 78 |
| `TRAINER_SPENCER` | SPENCER | Ordinary | 68 / 70 | 34 / 35 | 81 / 84 | 95 / 98 |
| `TRAINER_ROLAND` | ROLAND | Ordinary | 70 | 35 | 84 | 98 |
| `TRAINER_NOLEN` | NOLEN | Ordinary | 70 | 35 | 84 | 98 |
| `TRAINER_STAN` | STAN | Ordinary | 69 / 70 | 34 / 35 | 82 / 84 | 96 / 98 |
| `TRAINER_BARRY` | BARRY | Ordinary | 73 | 37 | 87 | 100 |
| `TRAINER_DEAN` | DEAN | Ordinary | 72 / 72 / 73 | 36 / 36 / 37 | 86 / 86 / 87 | 99 / 99 / 100 |
| `TRAINER_RODNEY` | RODNEY | Ordinary | 81 | 41 | 97 | 100 |
| `TRAINER_RICHARD` | RICHARD | Ordinary | 81 | 41 | 97 | 100 |
| `TRAINER_HERMAN` | HERMAN | Ordinary | 80 / 81 | 40 / 41 | 96 / 97 | 99 / 100 |
| `TRAINER_SANTIAGO` | SANTIAGO | Ordinary | 80 / 81 | 40 / 41 | 96 / 97 | 99 / 100 |
| `TRAINER_GILBERT` | GILBERT | Ordinary | 82 | 41 | 98 | 100 |
| `TRAINER_FRANKLIN` | FRANKLIN | Ordinary | 84 | 42 | 100 | 100 |
| `TRAINER_KEVIN` | KEVIN | Ordinary | 82 | 41 | 98 | 100 |
| `TRAINER_JACK` | JACK | Ordinary | 83 | 42 | 99 | 100 |
| `TRAINER_DUDLEY` | DUDLEY | Ordinary | 43 / 45 / 47 | 22 / 23 / 24 | 51 / 54 / 56 | 60 / 63 / 65 |
| `TRAINER_CHAD` | CHAD | Ordinary | 69 / 71 | 35 / 36 | 82 / 85 | 96 / 99 |
| `TRAINER_TONY_2` | TONY | Ordinary | 58 | 29 | 69 | 81 |
| `TRAINER_TONY_3` | TONY | Ordinary | 61 | 31 | 73 | 85 |
| `TRAINER_TONY_4` | TONY | Ordinary | 62 / 64 | 31 / 32 | 74 / 76 | 86 / 89 |
| `TRAINER_TONY_5` | TONY | Ordinary | 68 / 70 | 34 / 35 | 81 / 84 | 95 / 98 |
| `TRAINER_TAKAO` | TAKAO | Ordinary | 32 | 16 | 38 | 44 |
| `TRAINER_HITOSHI` | HITOSHI | Ordinary | 82 / 83 | 41 / 42 | 98 / 99 | 99 / 100 |
| `TRAINER_KIYO` | KIYO | Ordinary | 82 | 41 | 98 | 100 |
| `TRAINER_KOICHI` | KOICHI | Ordinary | 59 / 61 | 30 / 31 | 70 / 73 | 82 / 85 |
| `TRAINER_NOB_1` | NOB | Ordinary | 48 | 24 | 57 | 67 |
| `TRAINER_NOB_2` | NOB | Ordinary | 51 | 26 | 61 | 71 |
| `TRAINER_NOB_3` | NOB | Ordinary | 52 / 54 | 26 / 27 | 62 / 64 | 72 / 75 |
| `TRAINER_NOB_4` | NOB | Ordinary | 56 / 58 / 60 | 28 / 29 / 30 | 67 / 69 / 72 | 78 / 81 / 84 |
| `TRAINER_NOB_5` | NOB | Ordinary | 57 / 59 / 61 / 63 | 29 / 30 / 31 / 32 | 68 / 70 / 73 / 75 | 79 / 82 / 85 / 88 |
| `TRAINER_YUJI` | YUJI | Ordinary | 51 / 53 | 26 / 27 | 61 / 63 | 71 / 74 |
| `TRAINER_DAISUKE` | Spro | Protected | 49 / 48 / 49 | 25 / 24 / 25 | 58 / 57 / 58 | 63 / 62 / 63 |
| `TRAINER_ATSUSHI` | ATSUSHI | Ordinary | 69 | 35 | 82 | 96 |
| `TRAINER_KIRK` | KIRK | Ordinary | 41 / 43 | 21 / 22 | 49 / 51 | 57 / 60 |
| `TRAINER_GRUNT_AQUA_HIDEOUT_7` | GRUNT | Story | 72 / 73 | 36 / 37 | 90 / 91 | 99 / 100 |
| `TRAINER_GRUNT_AQUA_HIDEOUT_8` | GRUNT | Story | 73 | 37 | 91 | 100 |
| `TRAINER_SHAWN` | SHAWN | Ordinary | 40 / 41 | 20 / 21 | 48 / 49 | 56 / 57 |
| `TRAINER_FERNANDO_1` | FERNANDO | Ordinary | 67 / 69 | 34 / 35 | 80 / 82 | 93 / 96 |
| `TRAINER_DALTON_1` | Queen | Protected | 44 / 43 | 22 / 22 | 52 / 51 | 57 / 55 |
| `TRAINER_DALTON_2` | Queen | Protected | 55 / 54 / 56 | 28 / 27 / 28 | 66 / 64 / 67 | 71 / 70 / 72 |
| `TRAINER_DALTON_3` | Queen | Protected | 65 / 63 / 64 / 64 | 33 / 32 / 32 / 32 | 78 / 75 / 76 / 76 | 84 / 81 / 83 / 83 |
| `TRAINER_DALTON_4` | Queen | Protected | 87 / 80 / 90 / 80 | 44 / 40 / 45 / 40 | 100 / 96 / 100 / 96 | 100 / 100 / 100 / 100 |
| `TRAINER_DALTON_5` | Queen | Protected | 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 |
| `TRAINER_COLE` | COLE | Ordinary | 47 | 24 | 56 | 65 |
| `TRAINER_JEFF` | JEFF | Ordinary | 46 / 47 | 23 / 24 | 55 / 56 | 64 / 65 |
| `TRAINER_AXLE` | AXLE | Ordinary | 47 | 24 | 56 | 65 |
| `TRAINER_JACE` | JACE | Ordinary | 46 | 23 | 55 | 64 |
| `TRAINER_KEEGAN` | KEEGAN | Ordinary | 47 | 24 | 56 | 65 |
| `TRAINER_BERNIE_1` | Sugimori | Protected | 45 / 45 | 23 / 23 | 54 / 54 | 58 / 58 |
| `TRAINER_BERNIE_2` | Sugimori | Protected | 50 / 50 | 25 / 25 | 60 / 60 | 65 / 65 |
| `TRAINER_BERNIE_3` | Sugimori | Protected | 60 / 60 | 30 / 30 | 72 / 72 | 78 / 78 |
| `TRAINER_BERNIE_4` | Sugimori | Protected | 70 / 70 | 35 / 35 | 84 / 84 | 91 / 91 |
| `TRAINER_BERNIE_5` | Sugimori | Protected | 100 / 100 | 50 / 50 | 100 / 100 | 100 / 100 |
| `TRAINER_DREW` | Manolete | Protected | 47 / 47 / 47 | 24 / 24 / 24 | 56 / 56 / 56 | 61 / 61 / 61 |
| `TRAINER_BEAU` | BEAU | Ordinary | 45 / 46 / 48 | 23 / 23 / 24 | 54 / 55 / 57 | 63 / 64 / 67 |
| `TRAINER_LARRY` | LARRY | Ordinary | 46 | 23 | 55 | 64 |
| `TRAINER_SHANE` | SHANE | Ordinary | 46 / 48 | 23 / 24 | 55 / 57 | 64 / 67 |
| `TRAINER_JUSTIN` | JUSTIN | Ordinary | 49 | 25 | 58 | 68 |
| `TRAINER_ETHAN_1` | ETHAN | Ordinary | 46 / 47 | 23 / 24 | 55 / 56 | 64 / 65 |
| `TRAINER_AUTUMN` | AUTUMN | Ordinary | 48 | 24 | 57 | 67 |
| `TRAINER_TRAVIS` | TRAVIS | Ordinary | 47 | 24 | 56 | 65 |
| `TRAINER_ETHAN_2` | ETHAN | Ordinary | 48 / 50 | 24 / 25 | 57 / 60 | 67 / 70 |
| `TRAINER_ETHAN_3` | ETHAN | Ordinary | 55 / 57 | 28 / 29 | 66 / 68 | 77 / 79 |
| `TRAINER_ETHAN_4` | ETHAN | Ordinary | 59 / 61 / 63 | 30 / 31 / 32 | 70 / 73 / 75 | 82 / 85 / 88 |
| `TRAINER_ETHAN_5` | ETHAN | Ordinary | 96 / 98 / 100 | 48 / 49 / 50 | 96 / 98 / 100 | 96 / 98 / 100 |
| `TRAINER_BRENT` | BRENT | Ordinary | 61 | 31 | 73 | 85 |
| `TRAINER_DONALD` | DONALD | Ordinary | 57 / 57 / 58 | 28 / 28 / 29 | 68 / 68 / 69 | 79 / 79 / 81 |
| `TRAINER_TAYLOR` | TAYLOR | Ordinary | 57 / 58 / 59 | 29 / 29 / 30 | 68 / 69 / 70 | 79 / 81 / 82 |
| `TRAINER_JEFFREY_1` | JEFFREY | Ordinary | 59 / 59 / 60 | 29 / 29 / 30 | 70 / 70 / 72 | 82 / 82 / 84 |
| `TRAINER_DEREK` | DEREK | Ordinary | 40 / 39 | 20 / 19 | 48 / 46 | 56 / 54 |
| `TRAINER_JEFFREY_2` | JEFFREY | Ordinary | 58 / 60 / 62 | 29 / 30 / 31 | 69 / 72 / 74 | 81 / 84 / 86 |
| `TRAINER_JEFFREY_3` | JEFFREY | Ordinary | 61 / 63 / 65 | 31 / 32 / 33 | 73 / 75 / 78 | 85 / 88 / 91 |
| `TRAINER_JEFFREY_4` | JEFFREY | Ordinary | 64 / 66 / 68 | 32 / 33 / 34 | 76 / 79 / 81 | 89 / 92 / 95 |
| `TRAINER_JEFFREY_5` | JEFFREY | Ordinary | 86 / 88 / 90 | 43 / 44 / 45 | 96 / 98 / 100 | 96 / 98 / 100 |
| `TRAINER_EDWARD` | EDWARD | Ordinary | 40 | 20 | 48 | 56 |
| `TRAINER_PRESTON` | PRESTON | Ordinary | 73 | 37 | 87 | 100 |
| `TRAINER_VIRGIL` | VIRGIL | Ordinary | 73 | 37 | 87 | 100 |
| `TRAINER_BLAKE` | BLAKE | Ordinary | 73 | 37 | 87 | 100 |
| `TRAINER_WILLIAM` | WILLIAM | Ordinary | 66 / 67 / 69 | 33 / 34 / 35 | 79 / 80 / 82 | 92 / 93 / 96 |
| `TRAINER_JOSHUA` | JOSHUA | Ordinary | 80 / 82 | 40 / 41 | 96 / 98 | 98 / 100 |
| `TRAINER_CAMERON_1` | CAMERON | Ordinary | 68 | 34 | 81 | 95 |
| `TRAINER_CAMERON_2` | CAMERON | Ordinary | 69 / 71 | 35 / 36 | 82 / 85 | 96 / 99 |
| `TRAINER_CAMERON_3` | CAMERON | Ordinary | 72 / 74 | 36 / 37 | 86 / 88 | 98 / 100 |
| `TRAINER_CAMERON_4` | CAMERON | Ordinary | 75 / 77 | 38 / 39 | 90 / 92 | 98 / 100 |
| `TRAINER_CAMERON_5` | CAMERON | Ordinary | 78 / 80 | 39 / 40 | 93 / 96 | 98 / 100 |
| `TRAINER_JACLYN` | JACLYN | Ordinary | 39 | 20 | 46 | 54 |
| `TRAINER_HANNAH` | HANNAH | Ordinary | 71 / 73 | 36 / 37 | 85 / 87 | 98 / 100 |
| `TRAINER_SAMANTHA` | SAMANTHA | Ordinary | 74 | 37 | 88 | 100 |
| `TRAINER_MAURA` | MAURA | Ordinary | 73 | 37 | 87 | 100 |
| `TRAINER_KAYLA` | KAYLA | Ordinary | 66 / 67 / 68 | 33 / 33 / 34 | 79 / 80 / 81 | 92 / 93 / 95 |
| `TRAINER_ALEXIS` | ALEXIS | Ordinary | 79 / 81 | 40 / 41 | 94 / 97 | 98 / 100 |
| `TRAINER_JACKI_1` | JACKI | Ordinary | 66 / 68 | 33 / 34 | 79 / 81 | 92 / 95 |
| `TRAINER_JACKI_2` | JACKI | Ordinary | 69 / 71 | 35 / 36 | 82 / 85 | 96 / 99 |
| `TRAINER_JACKI_3` | JACKI | Ordinary | 72 / 74 | 36 / 37 | 86 / 88 | 98 / 100 |
| `TRAINER_JACKI_4` | JACKI | Ordinary | 83 / 85 | 42 / 43 | 98 / 100 | 98 / 100 |
| `TRAINER_JACKI_5` | JACKI | Ordinary | 98 / 100 | 49 / 50 | 98 / 100 | 98 / 100 |
| `TRAINER_WALTER_1` | WALTER | Ordinary | 66 | 33 | 79 | 92 |
| `TRAINER_MICAH` | MICAH | Ordinary | 63 / 65 | 32 / 33 | 75 / 78 | 88 / 91 |
| `TRAINER_THOMAS` | Flo | Protected | 50 / 50 / 50 / 50 / 50 / 50 | 25 / 25 / 25 / 25 / 25 / 25 | 60 / 60 / 60 / 60 / 60 / 60 | 65 / 65 / 65 / 65 / 65 / 65 |
| `TRAINER_WALTER_2` | WALTER | Ordinary | 69 | 35 | 82 | 96 |
| `TRAINER_WALTER_3` | WALTER | Ordinary | 70 / 72 | 35 / 36 | 84 / 86 | 98 / 100 |
| `TRAINER_WALTER_4` | WALTER | Ordinary | 78 / 80 | 39 / 40 | 93 / 96 | 98 / 100 |
| `TRAINER_WALTER_5` | WALTER | Ordinary | 96 / 98 / 100 | 48 / 49 / 50 | 96 / 98 / 100 | 96 / 98 / 100 |
| `TRAINER_SIDNEY` | SIDNEY | Story | 100 / 100 / 100 / 100 / 100 / 100 | 70 / 70 / 70 / 70 / 70 / 70 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_PHOEBE` | PHOEBE | Story | 100 / 100 / 100 / 100 / 100 / 100 | 70 / 70 / 70 / 70 / 70 / 70 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_GLACIA` | GLACIA | Story | 100 / 100 / 100 / 100 / 100 / 100 | 70 / 70 / 70 / 70 / 70 / 70 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_DRAKE` | DRAKE | Story | 100 / 100 / 100 / 100 / 100 / 100 | 70 / 70 / 70 / 70 / 70 / 70 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_ROXANNE_1` | ROXANNE | Story | 22 / 23 / 24 / 25 | 11 / 12 / 12 / 13 | 27 / 28 / 30 / 31 | 31 / 33 / 34 / 36 |
| `TRAINER_BRAWLY_1` | BRAWLY | Story | 32 / 33 / 34 / 35 | 16 / 17 / 17 / 18 | 40 / 41 / 42 / 43 | 46 / 47 / 49 / 50 |
| `TRAINER_WATTSON_1` | WATTSON | Story | 42 / 43 / 44 / 45 | 21 / 22 / 22 / 23 | 52 / 53 / 55 / 56 | 60 / 62 / 63 / 65 |
| `TRAINER_FLANNERY_1` | FLANNERY | Story | 46 / 47 / 48 / 49 / 50 | 23 / 24 / 24 / 25 / 25 | 57 / 58 / 60 / 61 / 62 | 66 / 68 / 69 / 71 / 72 |
| `TRAINER_NORMAN_1` | NORMAN | Story | 52 / 53 / 54 / 55 | 26 / 27 / 27 / 28 | 65 / 66 / 67 / 68 | 75 / 76 / 78 / 79 |
| `TRAINER_WINONA_1` | WINONA | Story | 61 / 62 / 63 / 64 / 65 | 31 / 31 / 32 / 32 / 33 | 76 / 77 / 78 / 80 / 81 | 88 / 89 / 91 / 92 / 94 |
| `TRAINER_TATE_AND_LIZA_1` | TATE&LIZA | Story | 71 / 72 / 73 / 74 / 75 | 36 / 36 / 37 / 37 / 38 | 88 / 90 / 91 / 92 / 93 | 96 / 97 / 98 / 99 / 100 |
| `TRAINER_JUAN_1` | JUAN | Story | 86 / 86 / 87 / 88 / 89 / 90 | 43 / 43 / 44 / 44 / 45 / 45 | 96 / 96 / 97 / 98 / 99 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_JERRY_1` | JERRY | Ordinary | 20 | 10 | 24 | 28 |
| `TRAINER_TED` | TED | Ordinary | 44 | 22 | 52 | 61 |
| `TRAINER_PAUL` | PAUL | Ordinary | 42 / 43 / 44 | 21 / 21 / 22 | 50 / 51 / 52 | 58 / 60 / 61 |
| `TRAINER_JERRY_2` | JERRY | Ordinary | 38 / 40 | 19 / 20 | 45 / 48 | 53 / 56 |
| `TRAINER_JERRY_3` | JERRY | Ordinary | 48 / 50 | 24 / 25 | 57 / 60 | 67 / 70 |
| `TRAINER_JERRY_4` | JERRY | Ordinary | 58 / 60 | 29 / 30 | 69 / 72 | 81 / 84 |
| `TRAINER_JERRY_5` | JERRY | Ordinary | 76 / 78 / 80 | 38 / 39 / 40 | 91 / 93 / 96 | 96 / 98 / 100 |
| `TRAINER_KAREN_1` | KAREN | Ordinary | 20 | 10 | 24 | 28 |
| `TRAINER_GEORGIA` | GEORGIA | Ordinary | 42 / 43 | 21 / 22 | 50 / 51 | 58 / 60 |
| `TRAINER_KAREN_2` | KAREN | Ordinary | 38 / 40 | 19 / 20 | 45 / 48 | 53 / 56 |
| `TRAINER_KAREN_3` | KAREN | Ordinary | 48 / 50 | 24 / 25 | 57 / 60 | 67 / 70 |
| `TRAINER_KAREN_4` | KAREN | Ordinary | 58 / 60 | 29 / 30 | 69 / 72 | 81 / 84 |
| `TRAINER_KAREN_5` | KAREN | Ordinary | 68 / 70 | 34 / 35 | 81 / 84 | 95 / 98 |
| `TRAINER_KATE_AND_JOY` | KATE & JOY | Ordinary | 66 / 68 | 33 / 34 | 79 / 81 | 92 / 95 |
| `TRAINER_ANNA_AND_MEG_1` | ANNA & MEG | Ordinary | 39 / 40 | 19 / 20 | 46 / 48 | 54 / 56 |
| `TRAINER_ANNA_AND_MEG_2` | ANNA & MEG | Ordinary | 41 / 43 | 21 / 22 | 49 / 51 | 57 / 60 |
| `TRAINER_ANNA_AND_MEG_3` | ANNA & MEG | Ordinary | 44 / 46 | 22 / 23 | 52 / 55 | 61 / 64 |
| `TRAINER_ANNA_AND_MEG_4` | ANNA & MEG | Ordinary | 53 / 55 | 27 / 28 | 63 / 66 | 74 / 77 |
| `TRAINER_ANNA_AND_MEG_5` | ANNA & MEG | Ordinary | 68 / 70 | 34 / 35 | 81 / 84 | 95 / 98 |
| `TRAINER_VICTOR` | VICTOR | Story | 45 / 46 | 23 / 23 | 56 / 57 | 65 / 66 |
| `TRAINER_MIGUEL_1` | MIGUEL | Ordinary | 40 | 20 | 48 | 56 |
| `TRAINER_COLTON` | COLTON | Ordinary | 80 / 82 / 84 / 86 / 88 / 90 | 40 / 41 / 42 / 43 / 44 / 45 | 90 / 92 / 94 / 96 / 98 / 100 | 90 / 92 / 94 / 96 / 98 / 100 |
| `TRAINER_MIGUEL_2` | MIGUEL | Ordinary | 60 | 30 | 72 | 84 |
| `TRAINER_MIGUEL_3` | MIGUEL | Ordinary | 70 | 35 | 84 | 98 |
| `TRAINER_MIGUEL_4` | MIGUEL | Ordinary | 80 | 40 | 96 | 100 |
| `TRAINER_MIGUEL_5` | MIGUEL | Ordinary | 100 | 50 | 100 | 100 |
| `TRAINER_VICTORIA` | VICTORIA | Story | 46 | 23 | 57 | 66 |
| `TRAINER_VANESSA` | VANESSA | Ordinary | 69 | 35 | 82 | 96 |
| `TRAINER_BETHANY` | BETHANY | Ordinary | 85 / 86 / 88 | 43 / 43 / 44 | 97 / 98 / 100 | 97 / 98 / 100 |
| `TRAINER_ISABEL_1` | ISABEL | Ordinary | 37 / 39 | 19 / 20 | 44 / 46 | 51 / 54 |
| `TRAINER_ISABEL_2` | ISABEL | Ordinary | 43 / 45 | 22 / 23 | 51 / 54 | 60 / 63 |
| `TRAINER_ISABEL_3` | ISABEL | Ordinary | 58 / 60 | 29 / 30 | 69 / 72 | 81 / 84 |
| `TRAINER_ISABEL_4` | ISABEL | Ordinary | 68 / 70 | 34 / 35 | 81 / 84 | 95 / 98 |
| `TRAINER_ISABEL_5` | ISABEL | Ordinary | 88 / 90 | 44 / 45 | 98 / 100 | 98 / 100 |
| `TRAINER_TIMOTHY_1` | TIMOTHY | Ordinary | 61 | 31 | 73 | 85 |
| `TRAINER_TIMOTHY_2` | TIMOTHY | Ordinary | 64 | 32 | 76 | 89 |
| `TRAINER_TIMOTHY_3` | TIMOTHY | Ordinary | 67 | 34 | 80 | 93 |
| `TRAINER_TIMOTHY_4` | TIMOTHY | Ordinary | 70 | 35 | 84 | 98 |
| `TRAINER_TIMOTHY_5` | TIMOTHY | Ordinary | 100 | 50 | 100 | 100 |
| `TRAINER_VICKY` | VICKY | Story | 46 | 23 | 57 | 66 |
| `TRAINER_SHELBY_1` | SHELBY | Ordinary | 46 / 48 | 23 / 24 | 55 / 57 | 64 / 67 |
| `TRAINER_SHELBY_2` | SHELBY | Ordinary | 53 / 55 | 27 / 28 | 63 / 66 | 74 / 77 |
| `TRAINER_SHELBY_3` | SHELBY | Ordinary | 63 / 65 | 32 / 33 | 75 / 78 | 88 / 91 |
| `TRAINER_SHELBY_4` | SHELBY | Ordinary | 78 / 80 | 39 / 40 | 93 / 96 | 98 / 100 |
| `TRAINER_SHELBY_5` | SHELBY | Ordinary | 98 / 100 | 49 / 50 | 98 / 100 | 98 / 100 |
| `TRAINER_CALVIN_1` | CALVIN | Ordinary | 10 | 5 | 12 | 14 |
| `TRAINER_BILLY` | BILLY | Ordinary | 15 / 17 | 8 / 9 | 18 / 20 | 21 / 23 |
| `TRAINER_JOSH` | JOSH | Ordinary | 23 | 12 | 27 | 32 |
| `TRAINER_TOMMY` | TOMMY | Ordinary | 22 / 24 | 11 / 12 | 26 / 28 | 30 / 33 |
| `TRAINER_JOEY` | JOEY | Protected | 23 | 12 | 27 | 29 |
| `TRAINER_BEN` | BEN | Ordinary | 42 / 44 | 21 / 22 | 50 / 52 | 58 / 61 |
| `TRAINER_QUINCY` | QUINCY | Ordinary | 86 / 88 | 43 / 44 | 98 / 100 | 98 / 100 |
| `TRAINER_KATELYNN` | KATELYNN | Ordinary | 88 / 90 | 44 / 45 | 98 / 100 | 98 / 100 |
| `TRAINER_JAYLEN` | JAYLEN | Ordinary | 44 | 22 | 52 | 61 |
| `TRAINER_DILLON` | DILLON | Ordinary | 44 | 22 | 52 | 61 |
| `TRAINER_CALVIN_2` | CALVIN | Ordinary | 50 | 25 | 60 | 70 |
| `TRAINER_CALVIN_3` | CALVIN | Ordinary | 53 / 55 | 27 / 28 | 63 / 66 | 74 / 77 |
| `TRAINER_CALVIN_4` | CALVIN | Ordinary | 56 / 58 / 60 | 28 / 29 / 30 | 67 / 69 / 72 | 78 / 81 / 84 |
| `TRAINER_CALVIN_5` | CALVIN | Ordinary | 96 / 98 / 100 | 48 / 49 / 50 | 96 / 98 / 100 | 96 / 98 / 100 |
| `TRAINER_EDDIE` | EDDIE | Ordinary | 39 / 41 | 20 / 21 | 46 / 49 | 54 / 57 |
| `TRAINER_ALLEN` | ALLEN | Ordinary | 13 / 11 | 7 / 6 | 15 / 13 | 18 / 15 |
| `TRAINER_TIMMY` | TIMMY | Ordinary | 38 / 40 | 19 / 20 | 45 / 48 | 53 / 56 |
| `TRAINER_WALLACE` | WALLACE | Story | 100 / 100 / 100 / 100 / 100 / 100 | 70 / 70 / 70 / 70 / 70 / 70 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_ANDREW` | ANDREW | Ordinary | 35 / 36 / 38 | 18 / 18 / 19 | 42 / 43 / 45 | 49 / 50 / 53 |
| `TRAINER_IVAN` | IVAN | Ordinary | 14 / 16 / 18 | 7 / 8 / 9 | 16 / 19 / 21 | 19 / 22 / 25 |
| `TRAINER_CLAUDE` | CLAUDE | Ordinary | 44 / 44 / 45 | 22 / 22 / 23 | 52 / 52 / 54 | 61 / 61 / 63 |
| `TRAINER_ELLIOT_1` | ELLIOT | Ordinary | 28 / 28 / 29 | 14 / 14 / 15 | 33 / 33 / 34 | 39 / 39 / 40 |
| `TRAINER_NED` | NED | Ordinary | 33 | 17 | 39 | 46 |
| `TRAINER_DALE` | DALE | Ordinary | 36 / 37 / 39 / 41 | 18 / 19 / 20 / 21 | 43 / 44 / 46 / 49 | 50 / 51 / 54 / 57 |
| `TRAINER_NOLAN` | NOLAN | Ordinary | 45 | 23 | 54 | 63 |
| `TRAINER_BARNY` | BARNY | Ordinary | 58 / 60 | 29 / 30 | 69 / 72 | 81 / 84 |
| `TRAINER_WADE` | WADE | Ordinary | 43 | 22 | 51 | 60 |
| `TRAINER_CARTER` | CARTER | Ordinary | 36 / 38 | 18 / 19 | 43 / 45 | 50 / 53 |
| `TRAINER_ELLIOT_2` | ELLIOT | Ordinary | 51 / 53 / 55 | 26 / 27 / 28 | 61 / 63 / 66 | 71 / 74 / 77 |
| `TRAINER_ELLIOT_3` | ELLIOT | Ordinary | 54 / 56 / 58 / 60 | 27 / 28 / 29 / 30 | 64 / 67 / 69 / 72 | 75 / 78 / 81 / 84 |
| `TRAINER_ELLIOT_4` | ELLIOT | Ordinary | 62 / 64 / 66 / 68 | 31 / 32 / 33 / 34 | 74 / 76 / 79 / 81 | 86 / 89 / 92 / 95 |
| `TRAINER_ELLIOT_5` | ELLIOT | Ordinary | 72 / 74 / 76 / 78 | 36 / 37 / 38 / 39 | 86 / 88 / 91 / 93 | 94 / 96 / 98 / 100 |
| `TRAINER_RONALD` | RONALD | Ordinary | 80 / 81 / 82 / 83 / 84 / 85 | 40 / 41 / 41 / 42 / 42 / 43 | 95 / 96 / 97 / 98 / 99 / 100 | 95 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_JACOB` | JACOB | Ordinary | 36 / 37 / 39 | 18 / 19 / 20 | 43 / 44 / 46 | 50 / 51 / 54 |
| `TRAINER_ANTHONY` | ANTHONY | Ordinary | 39 / 41 | 20 / 21 | 46 / 49 | 54 / 57 |
| `TRAINER_BENJAMIN_1` | BENJAMIN | Ordinary | 39 | 20 | 46 | 54 |
| `TRAINER_BENJAMIN_2` | BENJAMIN | Ordinary | 50 | 25 | 60 | 70 |
| `TRAINER_BENJAMIN_3` | BENJAMIN | Ordinary | 60 | 30 | 72 | 84 |
| `TRAINER_BENJAMIN_4` | BENJAMIN | Ordinary | 70 | 35 | 84 | 98 |
| `TRAINER_BENJAMIN_5` | BENJAMIN | Ordinary | 100 | 50 | 100 | 100 |
| `TRAINER_ABIGAIL_1` | ABIGAIL | Ordinary | 39 | 20 | 46 | 54 |
| `TRAINER_JASMINE` | JASMINE | Ordinary | 37 / 39 / 41 | 19 / 20 / 21 | 44 / 46 / 49 | 51 / 54 / 57 |
| `TRAINER_ABIGAIL_2` | ABIGAIL | Ordinary | 50 | 25 | 60 | 70 |
| `TRAINER_ABIGAIL_3` | ABIGAIL | Ordinary | 60 | 30 | 72 | 84 |
| `TRAINER_ABIGAIL_4` | ABIGAIL | Ordinary | 70 | 35 | 84 | 98 |
| `TRAINER_ABIGAIL_5` | ABIGAIL | Ordinary | 100 | 50 | 100 | 100 |
| `TRAINER_DYLAN_1` | DYLAN | Ordinary | 41 | 21 | 49 | 57 |
| `TRAINER_DYLAN_2` | DYLAN | Ordinary | 50 | 25 | 60 | 70 |
| `TRAINER_DYLAN_3` | DYLAN | Ordinary | 60 | 30 | 72 | 84 |
| `TRAINER_DYLAN_4` | DYLAN | Ordinary | 70 | 35 | 84 | 98 |
| `TRAINER_DYLAN_5` | DYLAN | Ordinary | 100 | 50 | 100 | 100 |
| `TRAINER_MARIA_1` | MARIA | Ordinary | 43 | 22 | 51 | 60 |
| `TRAINER_MARIA_2` | MARIA | Ordinary | 60 | 30 | 72 | 84 |
| `TRAINER_MARIA_3` | MARIA | Ordinary | 70 | 35 | 84 | 98 |
| `TRAINER_MARIA_4` | MARIA | Ordinary | 80 | 40 | 96 | 100 |
| `TRAINER_MARIA_5` | MARIA | Ordinary | 100 | 50 | 100 | 100 |
| `TRAINER_CAMDEN` | CAMDEN | Ordinary | 78 / 80 | 39 / 40 | 93 / 96 | 98 / 100 |
| `TRAINER_DEMETRIUS` | DEMETRIUS | Ordinary | 57 / 59 | 29 / 30 | 68 / 70 | 79 / 82 |
| `TRAINER_ISAIAH_1` | ISAIAH | Ordinary | 83 | 42 | 99 | 100 |
| `TRAINER_PABLO_1` | PABLO | Ordinary | 72 / 74 | 36 / 37 | 86 / 88 | 98 / 100 |
| `TRAINER_CHASE` | CHASE | Ordinary | 79 / 80 | 39 / 40 | 94 / 96 | 99 / 100 |
| `TRAINER_ISAIAH_2` | ISAIAH | Ordinary | 86 | 43 | 100 | 100 |
| `TRAINER_ISAIAH_3` | ISAIAH | Ordinary | 89 | 45 | 100 | 100 |
| `TRAINER_ISAIAH_4` | ISAIAH | Ordinary | 92 | 46 | 100 | 100 |
| `TRAINER_ISAIAH_5` | ISAIAH | Ordinary | 100 | 50 | 100 | 100 |
| `TRAINER_ISOBEL` | ISOBEL | Ordinary | 76 | 38 | 91 | 100 |
| `TRAINER_DONNY` | DONNY | Ordinary | 77 / 79 | 39 / 40 | 92 / 94 | 98 / 100 |
| `TRAINER_TALIA` | TALIA | Ordinary | 82 | 41 | 98 | 100 |
| `TRAINER_KATELYN_1` | KATELYN | Ordinary | 83 | 42 | 99 | 100 |
| `TRAINER_ALLISON` | ALLISON | Ordinary | 82 / 84 | 41 / 42 | 98 / 100 | 98 / 100 |
| `TRAINER_KATELYN_2` | KATELYN | Ordinary | 86 | 43 | 100 | 100 |
| `TRAINER_KATELYN_3` | KATELYN | Ordinary | 89 | 45 | 100 | 100 |
| `TRAINER_KATELYN_4` | KATELYN | Ordinary | 92 | 46 | 100 | 100 |
| `TRAINER_KATELYN_5` | KATELYN | Ordinary | 100 | 50 | 100 | 100 |
| `TRAINER_NICOLAS_1` | NICOLAS | Ordinary | 82 / 84 | 41 / 42 | 98 / 100 | 98 / 100 |
| `TRAINER_NICOLAS_2` | NICOLAS | Ordinary | 85 / 87 | 43 / 44 | 98 / 100 | 98 / 100 |
| `TRAINER_NICOLAS_3` | NICOLAS | Ordinary | 88 / 90 | 44 / 45 | 98 / 100 | 98 / 100 |
| `TRAINER_NICOLAS_4` | NICOLAS | Ordinary | 89 / 91 / 93 | 45 / 46 / 47 | 96 / 98 / 100 | 96 / 98 / 100 |
| `TRAINER_NICOLAS_5` | NICOLAS | Ordinary | 96 / 98 / 100 | 48 / 49 / 50 | 96 / 98 / 100 | 96 / 98 / 100 |
| `TRAINER_AARON` | AARON | Ordinary | 84 | 42 | 100 | 100 |
| `TRAINER_PERRY` | PERRY | Ordinary | 58 | 29 | 69 | 81 |
| `TRAINER_HUGH` | HUGH | Ordinary | 58 / 60 | 29 / 30 | 69 / 72 | 81 / 84 |
| `TRAINER_PHIL` | PHIL | Ordinary | 59 | 30 | 70 | 82 |
| `TRAINER_JARED` | JARED | Ordinary | 61 / 62 / 63 | 31 / 31 / 32 | 73 / 74 / 75 | 85 / 86 / 88 |
| `TRAINER_HUMBERTO` | HUMBERTO | Ordinary | 64 | 32 | 76 | 89 |
| `TRAINER_PRESLEY` | Elvis | Protected | 70 / 70 / 73 / 70 | 35 / 35 / 37 / 35 | 84 / 84 / 87 / 84 | 91 / 91 / 94 / 91 |
| `TRAINER_EDWARDO` | EDWARDO | Ordinary | 61 / 63 | 31 / 32 | 73 / 75 | 85 / 88 |
| `TRAINER_COLIN` | COLIN | Ordinary | 61 / 63 | 31 / 32 | 73 / 75 | 85 / 88 |
| `TRAINER_ROBERT_1` | ROBERT | Ordinary | 62 | 31 | 74 | 86 |
| `TRAINER_BENNY` | BENNY | Ordinary | 66 / 68 / 70 | 33 / 34 / 35 | 79 / 81 / 84 | 92 / 95 / 98 |
| `TRAINER_CHESTER` | CHESTER | Ordinary | 58 / 60 | 29 / 30 | 69 / 72 | 81 / 84 |
| `TRAINER_ROBERT_2` | ROBERT | Ordinary | 63 / 65 | 32 / 33 | 75 / 78 | 88 / 91 |
| `TRAINER_ROBERT_3` | ROBERT | Ordinary | 68 / 70 | 34 / 35 | 81 / 84 | 95 / 98 |
| `TRAINER_ROBERT_4` | ROBERT | Ordinary | 78 / 80 | 39 / 40 | 93 / 96 | 98 / 100 |
| `TRAINER_ROBERT_5` | ROBERT | Ordinary | 98 / 100 | 49 / 50 | 98 / 100 | 98 / 100 |
| `TRAINER_ALEX` | ALEX | Ordinary | 82 / 83 | 41 / 42 | 98 / 99 | 99 / 100 |
| `TRAINER_BECK` | BECK | Ordinary | 84 | 42 | 100 | 100 |
| `TRAINER_YASU` | YASU | Ordinary | 59 | 30 | 70 | 82 |
| `TRAINER_TAKASHI` | TAKASHI | Ordinary | 57 / 59 | 29 / 30 | 68 / 70 | 79 / 82 |
| `TRAINER_DIANNE` | DIANNE | Ordinary | 90 / 92 | 45 / 46 | 98 / 100 | 98 / 100 |
| `TRAINER_JANI` | JANI | Ordinary | 58 | 29 | 69 | 81 |
| `TRAINER_LAO_1` | LAO | Ordinary | 43 / 44 / 45 | 22 / 22 / 23 | 51 / 52 / 54 | 60 / 61 / 63 |
| `TRAINER_LUNG` | LUNG | Ordinary | 45 / 47 | 23 / 24 | 54 / 56 | 63 / 65 |
| `TRAINER_LAO_2` | LAO | Ordinary | 56 / 58 / 60 | 28 / 29 / 30 | 67 / 69 / 72 | 78 / 81 / 84 |
| `TRAINER_LAO_3` | LAO | Ordinary | 64 / 66 / 68 / 70 | 32 / 33 / 34 / 35 | 76 / 79 / 81 / 84 | 89 / 92 / 95 / 98 |
| `TRAINER_LAO_4` | LAO | Ordinary | 74 / 76 / 78 / 80 | 37 / 38 / 39 / 40 | 88 / 91 / 93 / 96 | 94 / 96 / 98 / 100 |
| `TRAINER_LAO_5` | LAO | Ordinary | 90 / 92 / 94 / 96 / 98 / 100 | 45 / 46 / 47 / 48 / 49 / 50 | 90 / 92 / 94 / 96 / 98 / 100 | 90 / 92 / 94 / 96 / 98 / 100 |
| `TRAINER_JOCELYN` | JOCELYN | Ordinary | 30 | 15 | 36 | 42 |
| `TRAINER_LAURA` | LAURA | Ordinary | 34 | 17 | 40 | 47 |
| `TRAINER_CYNDY_1` | CYNDY | Ordinary | 46 / 48 | 23 / 24 | 55 / 57 | 64 / 67 |
| `TRAINER_CORA` | CORA | Ordinary | 54 | 27 | 64 | 75 |
| `TRAINER_PAULA` | PAULA | Ordinary | 53 | 27 | 63 | 74 |
| `TRAINER_CYNDY_2` | CYNDY | Ordinary | 53 / 55 | 27 / 28 | 63 / 66 | 74 / 77 |
| `TRAINER_CYNDY_3` | CYNDY | Ordinary | 63 / 65 | 32 / 33 | 75 / 78 | 88 / 91 |
| `TRAINER_CYNDY_4` | CYNDY | Ordinary | 78 / 80 | 39 / 40 | 93 / 96 | 98 / 100 |
| `TRAINER_CYNDY_5` | CYNDY | Ordinary | 96 / 98 / 100 | 48 / 49 / 50 | 96 / 98 / 100 | 96 / 98 / 100 |
| `TRAINER_MADELINE_1` | MADELINE | Ordinary | 44 | 22 | 52 | 61 |
| `TRAINER_CLARISSA` | CLARISSA | Ordinary | 60 / 62 | 30 / 31 | 72 / 74 | 84 / 86 |
| `TRAINER_ANGELICA` | ANGELICA | Ordinary | 62 | 31 | 74 | 86 |
| `TRAINER_MADELINE_2` | MADELINE | Ordinary | 55 | 28 | 66 | 77 |
| `TRAINER_MADELINE_3` | MADELINE | Ordinary | 60 | 30 | 72 | 84 |
| `TRAINER_MADELINE_4` | MADELINE | Ordinary | 68 / 70 | 34 / 35 | 81 / 84 | 95 / 98 |
| `TRAINER_MADELINE_5` | MADELINE | Ordinary | 98 / 100 | 49 / 50 | 98 / 100 | 98 / 100 |
| `TRAINER_BEVERLY` | BEVERLY | Ordinary | 56 / 58 | 28 / 29 | 67 / 69 | 78 / 81 |
| `TRAINER_IMANI` | IMANI | Ordinary | 58 | 29 | 69 | 81 |
| `TRAINER_KYLA` | KYLA | Ordinary | 58 | 29 | 69 | 81 |
| `TRAINER_DENISE` | DENISE | Ordinary | 58 / 60 | 29 / 30 | 69 / 72 | 81 / 84 |
| `TRAINER_BETH` | BETH | Ordinary | 58 | 29 | 69 | 81 |
| `TRAINER_TARA` | TARA | Ordinary | 56 / 58 | 28 / 29 | 67 / 69 | 78 / 81 |
| `TRAINER_MISSY` | MISSY | Ordinary | 56 | 28 | 67 | 78 |
| `TRAINER_ALICE` | ALICE | Ordinary | 55 / 55 / 56 | 27 / 27 / 28 | 66 / 66 / 67 | 77 / 77 / 78 |
| `TRAINER_JENNY_1` | JENNY | Ordinary | 72 | 36 | 86 | 100 |
| `TRAINER_GRACE` | GRACE | Ordinary | 71 | 36 | 85 | 99 |
| `TRAINER_TANYA` | TANYA | Ordinary | 70 | 35 | 84 | 98 |
| `TRAINER_SHARON` | SHARON | Ordinary | 70 | 35 | 84 | 98 |
| `TRAINER_NIKKI` | NIKKI | Ordinary | 72 / 74 | 36 / 37 | 86 / 88 | 98 / 100 |
| `TRAINER_BRENDA` | BRENDA | Ordinary | 74 | 37 | 88 | 100 |
| `TRAINER_KATIE` | KATIE | Ordinary | 82 / 84 | 41 / 42 | 98 / 100 | 98 / 100 |
| `TRAINER_SUSIE` | SUSIE | Ordinary | 82 | 41 | 98 | 100 |
| `TRAINER_KARA` | KARA | Ordinary | 84 | 42 | 100 | 100 |
| `TRAINER_DANA` | DANA | Ordinary | 81 | 41 | 97 | 100 |
| `TRAINER_SIENNA` | SIENNA | Ordinary | 72 / 74 | 36 / 37 | 86 / 88 | 98 / 100 |
| `TRAINER_DEBRA` | DEBRA | Ordinary | 86 | 43 | 100 | 100 |
| `TRAINER_LINDA` | LINDA | Ordinary | 83 / 85 | 42 / 43 | 98 / 100 | 98 / 100 |
| `TRAINER_KAYLEE` | KAYLEE | Ordinary | 48 / 50 | 24 / 25 | 57 / 60 | 67 / 70 |
| `TRAINER_LAUREL` | LAUREL | Ordinary | 82 / 83 | 41 / 42 | 98 / 99 | 99 / 100 |
| `TRAINER_CARLEE` | CARLEE | Ordinary | 83 | 42 | 99 | 100 |
| `TRAINER_JENNY_2` | JENNY | Ordinary | 75 | 38 | 90 | 100 |
| `TRAINER_JENNY_3` | JENNY | Ordinary | 78 | 39 | 93 | 100 |
| `TRAINER_JENNY_4` | JENNY | Ordinary | 79 / 81 | 40 / 41 | 94 / 97 | 98 / 100 |
| `TRAINER_JENNY_5` | JENNY | Ordinary | 96 / 98 / 100 | 48 / 49 / 50 | 96 / 98 / 100 | 96 / 98 / 100 |
| `TRAINER_HEIDI` | HEIDI | Ordinary | 46 / 48 | 23 / 24 | 55 / 57 | 64 / 67 |
| `TRAINER_BECKY` | BECKY | Ordinary | 45 / 47 | 23 / 24 | 54 / 56 | 63 / 65 |
| `TRAINER_CAROL` | CAROL | Ordinary | 42 / 44 | 21 / 22 | 50 / 52 | 58 / 61 |
| `TRAINER_NANCY` | NANCY | Ordinary | 44 / 45 | 22 / 23 | 52 / 54 | 61 / 63 |
| `TRAINER_MARTHA` | MARTHA | Ordinary | 46 / 48 | 23 / 24 | 55 / 57 | 64 / 67 |
| `TRAINER_DIANA_1` | DIANA | Ordinary | 46 / 46 / 47 | 23 / 23 / 24 | 55 / 55 / 56 | 64 / 64 / 65 |
| `TRAINER_CEDRIC` | CEDRIC | Ordinary | 67 | 34 | 80 | 93 |
| `TRAINER_IRENE` | IRENE | Ordinary | 45 / 47 | 23 / 24 | 54 / 56 | 63 / 65 |
| `TRAINER_DIANA_2` | DIANA | Ordinary | 46 / 48 / 50 | 23 / 24 / 25 | 55 / 57 / 60 | 64 / 67 / 70 |
| `TRAINER_DIANA_3` | DIANA | Ordinary | 55 / 57 / 59 | 28 / 29 / 30 | 66 / 68 / 70 | 77 / 79 / 82 |
| `TRAINER_DIANA_4` | DIANA | Ordinary | 66 / 68 / 70 | 33 / 34 / 35 | 79 / 81 / 84 | 92 / 95 / 98 |
| `TRAINER_DIANA_5` | DIANA | Ordinary | 96 / 98 / 100 | 48 / 49 / 50 | 96 / 98 / 100 | 96 / 98 / 100 |
| `TRAINER_AMY_AND_LIV_1` | AMY & LIV | Ordinary | 36 / 38 | 18 / 19 | 43 / 45 | 50 / 53 |
| `TRAINER_AMY_AND_LIV_2` | AMY & LIV | Ordinary | 48 / 50 | 24 / 25 | 57 / 60 | 67 / 70 |
| `TRAINER_GINA_AND_MIA_1` | GINA & MIA | Ordinary | 15 / 17 | 8 / 9 | 18 / 20 | 21 / 23 |
| `TRAINER_MIU_AND_YUKI` | MIU & YUKI | Ordinary | 66 / 67 | 33 / 34 | 79 / 80 | 92 / 93 |
| `TRAINER_AMY_AND_LIV_3` | AMY & LIV | Ordinary | 68 / 70 | 34 / 35 | 81 / 84 | 95 / 98 |
| `TRAINER_GINA_AND_MIA_2` | GINA & MIA | Ordinary | 36 / 38 | 18 / 19 | 43 / 45 | 50 / 53 |
| `TRAINER_AMY_AND_LIV_4` | AMY & LIV | Ordinary | 78 / 80 | 39 / 40 | 93 / 96 | 98 / 100 |
| `TRAINER_AMY_AND_LIV_5` | AMY & LIV | Ordinary | 88 / 90 | 44 / 45 | 98 / 100 | 98 / 100 |
| `TRAINER_AMY_AND_LIV_6` | AMY & LIV | Ordinary | 98 / 100 | 49 / 50 | 98 / 100 | 98 / 100 |
| `TRAINER_HUEY` | HUEY | Ordinary | 35 / 36 | 17 / 18 | 42 / 43 | 49 / 50 |
| `TRAINER_EDMOND` | EDMOND | Ordinary | 37 | 19 | 44 | 51 |
| `TRAINER_ERNEST_1` | ERNEST | Ordinary | 70 / 72 | 35 / 36 | 84 / 86 | 98 / 100 |
| `TRAINER_DWAYNE` | DWAYNE | Ordinary | 35 / 37 / 39 | 18 / 19 / 20 | 42 / 44 / 46 | 49 / 51 / 54 |
| `TRAINER_PHILLIP` | PHILLIP | Ordinary | 79 / 81 | 40 / 41 | 94 / 97 | 98 / 100 |
| `TRAINER_LEONARD` | LEONARD | Ordinary | 77 / 79 / 81 | 39 / 40 / 41 | 92 / 94 / 97 | 96 / 98 / 100 |
| `TRAINER_DUNCAN` | DUNCAN | Ordinary | 60 / 62 | 30 / 31 | 72 / 74 | 84 / 86 |
| `TRAINER_ERNEST_2` | ERNEST | Ordinary | 71 / 73 / 75 | 36 / 37 / 38 | 85 / 87 / 90 | 96 / 98 / 100 |
| `TRAINER_ERNEST_3` | ERNEST | Ordinary | 74 / 76 / 78 | 37 / 38 / 39 | 88 / 91 / 93 | 96 / 98 / 100 |
| `TRAINER_ERNEST_4` | ERNEST | Ordinary | 77 / 79 / 81 | 39 / 40 / 41 | 92 / 94 / 97 | 96 / 98 / 100 |
| `TRAINER_ERNEST_5` | ERNEST | Ordinary | 96 / 98 / 100 | 48 / 49 / 50 | 96 / 98 / 100 | 96 / 98 / 100 |
| `TRAINER_ELI` | ELI | Ordinary | 47 | 24 | 56 | 65 |
| `TRAINER_ANNIKA` | ANNIKA | Ordinary | 86 / 88 | 43 / 44 | 98 / 100 | 98 / 100 |
| `TRAINER_JAZMYN` | JAZMYN | Ordinary | 67 | 34 | 80 | 93 |
| `TRAINER_JONAS` | JONAS | Ordinary | 68 | 34 | 81 | 95 |
| `TRAINER_KAYLEY` | KAYLEY | Ordinary | 68 | 34 | 81 | 95 |
| `TRAINER_AURON` | AURON | Ordinary | 69 / 70 | 34 / 35 | 82 / 84 | 96 / 98 |
| `TRAINER_KELVIN` | KELVIN | Ordinary | 82 / 83 | 41 / 42 | 98 / 99 | 99 / 100 |
| `TRAINER_MARLEY` | MARLEY | Ordinary | 83 | 42 | 99 | 100 |
| `TRAINER_REYNA` | REYNA | Ordinary | 82 / 83 | 41 / 42 | 98 / 99 | 99 / 100 |
| `TRAINER_HUDSON` | HUDSON | Ordinary | 83 | 42 | 99 | 100 |
| `TRAINER_CONOR` | CONOR | Ordinary | 81 / 83 | 41 / 42 | 97 / 99 | 98 / 100 |
| `TRAINER_EDWIN_1` | EDWIN | Ordinary | 36 / 37 | 18 / 19 | 43 / 44 | 50 / 51 |
| `TRAINER_HECTOR` | HECTOR | Ordinary | 43 / 44 | 21 / 22 | 51 / 52 | 60 / 61 |
| `TRAINER_TABITHA_MOSSDEEP` | TABITHA | Story | 79 / 80 / 81 / 82 | 40 / 40 / 41 / 41 | 97 / 98 / 99 / 100 | 97 / 98 / 99 / 100 |
| `TRAINER_EDWIN_2` | EDWIN | Ordinary | 39 / 41 | 20 / 21 | 46 / 49 | 54 / 57 |
| `TRAINER_EDWIN_3` | EDWIN | Ordinary | 42 / 44 | 21 / 22 | 50 / 52 | 58 / 61 |
| `TRAINER_EDWIN_4` | EDWIN | Ordinary | 45 / 47 | 23 / 24 | 54 / 56 | 63 / 65 |
| `TRAINER_EDWIN_5` | EDWIN | Ordinary | 98 / 100 | 49 / 50 | 98 / 100 | 98 / 100 |
| `TRAINER_WALLY_VR_1` | WALLY | Story | 92 / 93 / 94 / 95 / 96 | 46 / 47 / 47 / 48 / 48 | 96 / 97 / 98 / 99 / 100 | 96 / 97 / 98 / 99 / 100 |
| `TRAINER_BRENDAN_ROUTE_103_MUDKIP` | BRENDAN | Story | 9 | 5 | 12 | 14 |
| `TRAINER_BRENDAN_ROUTE_110_MUDKIP` | BRENDAN | Story | 41 / 42 / 43 / 44 | 21 / 21 / 22 / 22 | 51 / 52 / 53 / 55 | 59 / 60 / 62 / 63 |
| `TRAINER_BRENDAN_ROUTE_119_MUDKIP` | BRENDAN | Story | 61 / 62 / 63 / 64 / 65 | 31 / 31 / 32 / 32 / 33 | 76 / 77 / 78 / 80 / 81 | 88 / 89 / 91 / 92 / 94 |
| `TRAINER_BRENDAN_ROUTE_103_TREECKO` | BRENDAN | Story | 9 | 5 | 12 | 14 |
| `TRAINER_BRENDAN_ROUTE_110_TREECKO` | BRENDAN | Story | 41 / 42 / 43 / 44 | 21 / 21 / 22 / 22 | 51 / 52 / 53 / 55 | 59 / 60 / 62 / 63 |
| `TRAINER_BRENDAN_ROUTE_119_TREECKO` | BRENDAN | Story | 61 / 62 / 63 / 64 / 65 | 31 / 31 / 32 / 32 / 33 | 76 / 77 / 78 / 80 / 81 | 88 / 89 / 91 / 92 / 94 |
| `TRAINER_BRENDAN_ROUTE_103_TORCHIC` | BRENDAN | Story | 9 | 5 | 12 | 14 |
| `TRAINER_BRENDAN_ROUTE_110_TORCHIC` | BRENDAN | Story | 41 / 42 / 43 / 44 | 21 / 21 / 22 / 22 | 51 / 52 / 53 / 55 | 59 / 60 / 62 / 63 |
| `TRAINER_BRENDAN_ROUTE_119_TORCHIC` | BRENDAN | Story | 61 / 62 / 63 / 64 / 65 | 31 / 31 / 32 / 32 / 33 | 76 / 77 / 78 / 80 / 81 | 88 / 89 / 91 / 92 / 94 |
| `TRAINER_MAY_ROUTE_103_MUDKIP` | MAY | Story | 9 | 5 | 12 | 14 |
| `TRAINER_MAY_ROUTE_110_MUDKIP` | MAY | Story | 41 / 42 / 43 / 44 | 21 / 21 / 22 / 22 | 51 / 52 / 53 / 55 | 59 / 60 / 62 / 63 |
| `TRAINER_MAY_ROUTE_119_MUDKIP` | MAY | Story | 61 / 62 / 63 / 64 / 65 | 31 / 31 / 32 / 32 / 33 | 76 / 77 / 78 / 80 / 81 | 88 / 89 / 91 / 92 / 94 |
| `TRAINER_MAY_ROUTE_103_TREECKO` | MAY | Story | 9 | 5 | 12 | 14 |
| `TRAINER_MAY_ROUTE_110_TREECKO` | MAY | Story | 41 / 42 / 43 / 44 | 21 / 21 / 22 / 22 | 51 / 52 / 53 / 55 | 59 / 60 / 62 / 63 |
| `TRAINER_MAY_ROUTE_119_TREECKO` | MAY | Story | 61 / 62 / 63 / 64 / 65 | 31 / 31 / 32 / 32 / 33 | 76 / 77 / 78 / 80 / 81 | 88 / 89 / 91 / 92 / 94 |
| `TRAINER_MAY_ROUTE_103_TORCHIC` | MAY | Story | 9 | 5 | 12 | 14 |
| `TRAINER_MAY_ROUTE_110_TORCHIC` | MAY | Story | 41 / 42 / 43 / 44 | 21 / 21 / 22 / 22 | 51 / 52 / 53 / 55 | 59 / 60 / 62 / 63 |
| `TRAINER_MAY_ROUTE_119_TORCHIC` | MAY | Story | 61 / 62 / 63 / 64 / 65 | 31 / 31 / 32 / 32 / 33 | 76 / 77 / 78 / 80 / 81 | 88 / 89 / 91 / 92 / 94 |
| `TRAINER_ISAAC_1` | ISAAC | Ordinary | 39 / 39 / 39 / 40 | 19 / 19 / 19 / 20 | 46 / 46 / 46 / 48 | 54 / 54 / 54 / 56 |
| `TRAINER_DAVIS` | DAVIS | Ordinary | 67 | 34 | 80 | 93 |
| `TRAINER_MITCHELL` | Neo | Protected | 91 / 91 / 91 | 46 / 46 / 46 | 100 / 100 / 100 | 100 / 100 / 100 |
| `TRAINER_ISAAC_2` | ISAAC | Ordinary | 40 / 38 / 42 / 42 | 20 / 19 / 21 / 21 | 48 / 45 / 50 / 50 | 56 / 53 / 58 / 58 |
| `TRAINER_ISAAC_3` | ISAAC | Ordinary | 40 / 39 / 42 / 43 / 45 | 20 / 20 / 21 / 22 / 23 | 48 / 46 / 50 / 51 / 54 | 56 / 54 / 58 / 60 / 63 |
| `TRAINER_ISAAC_4` | ISAAC | Ordinary | 40 / 42 / 44 / 46 / 48 | 20 / 21 / 22 / 23 / 24 | 48 / 50 / 52 / 55 / 57 | 56 / 58 / 61 / 64 / 67 |
| `TRAINER_ISAAC_5` | ISAAC | Ordinary | 90 / 92 / 94 / 96 / 98 / 100 | 45 / 46 / 47 / 48 / 49 / 50 | 90 / 92 / 94 / 96 / 98 / 100 | 90 / 92 / 94 / 96 / 98 / 100 |
| `TRAINER_LYDIA_1` | LYDIA | Ordinary | 39 / 39 / 39 / 40 | 19 / 19 / 19 / 20 | 46 / 46 / 46 / 48 | 54 / 54 / 54 / 56 |
| `TRAINER_HALLE` | HALLE | Ordinary | 87 / 88 | 43 / 44 | 99 / 100 | 99 / 100 |
| `TRAINER_GARRISON` | GARRISON | Ordinary | 57 | 29 | 68 | 79 |
| `TRAINER_LYDIA_2` | LYDIA | Ordinary | 36 / 42 / 40 / 42 | 18 / 21 / 20 / 21 | 43 / 50 / 48 / 50 | 50 / 58 / 56 / 58 |
| `TRAINER_LYDIA_3` | LYDIA | Ordinary | 37 / 42 / 41 / 43 / 45 | 19 / 21 / 21 / 22 / 23 | 44 / 50 / 49 / 51 / 54 | 51 / 58 / 57 / 60 / 63 |
| `TRAINER_LYDIA_4` | LYDIA | Ordinary | 40 / 42 / 44 / 46 / 48 | 20 / 21 / 22 / 23 / 24 | 48 / 50 / 52 / 55 / 57 | 56 / 58 / 61 / 64 / 67 |
| `TRAINER_LYDIA_5` | LYDIA | Ordinary | 90 / 92 / 94 / 96 / 98 / 100 | 45 / 46 / 47 / 48 / 49 / 50 | 90 / 92 / 94 / 96 / 98 / 100 | 90 / 92 / 94 / 96 / 98 / 100 |
| `TRAINER_JACKSON_1` | JACKSON | Ordinary | 58 | 29 | 69 | 81 |
| `TRAINER_LORENZO` | LORENZO | Ordinary | 59 / 60 / 61 | 30 / 30 / 31 | 70 / 72 / 73 | 82 / 84 / 85 |
| `TRAINER_SEBASTIAN` | SEBASTIAN | Ordinary | 68 | 34 | 81 | 95 |
| `TRAINER_JACKSON_2` | JACKSON | Ordinary | 61 | 31 | 73 | 85 |
| `TRAINER_JACKSON_3` | JACKSON | Ordinary | 64 | 32 | 76 | 89 |
| `TRAINER_JACKSON_4` | JACKSON | Ordinary | 67 | 34 | 80 | 93 |
| `TRAINER_JACKSON_5` | JACKSON | Ordinary | 98 / 100 | 49 / 50 | 98 / 100 | 98 / 100 |
| `TRAINER_CATHERINE_1` | CATHERINE | Ordinary | 57 / 58 | 28 / 29 | 68 / 69 | 79 / 81 |
| `TRAINER_JENNA` | JENNA | Ordinary | 59 / 60 / 61 | 30 / 30 / 31 | 70 / 72 / 73 | 82 / 84 / 85 |
| `TRAINER_SOPHIA` | SOPHIA | Ordinary | 66 / 68 | 33 / 34 | 79 / 81 | 92 / 95 |
| `TRAINER_CATHERINE_2` | CATHERINE | Ordinary | 59 / 61 | 30 / 31 | 70 / 73 | 82 / 85 |
| `TRAINER_CATHERINE_3` | CATHERINE | Ordinary | 62 / 64 | 31 / 32 | 74 / 76 | 86 / 89 |
| `TRAINER_CATHERINE_4` | CATHERINE | Ordinary | 65 / 67 | 33 / 34 | 78 / 80 | 91 / 93 |
| `TRAINER_CATHERINE_5` | CATHERINE | Ordinary | 98 / 100 | 49 / 50 | 98 / 100 | 98 / 100 |
| `TRAINER_JULIO` | JULIO | Ordinary | 47 | 24 | 56 | 65 |
| `TRAINER_GRUNT_SEAFLOOR_CAVERN_5` | GRUNT | Story | 86 / 87 | 43 / 44 | 99 / 100 | 99 / 100 |
| `TRAINER_GRUNT_UNUSED` | GRUNT | Story | 45 / 46 | 23 / 23 | 56 / 57 | 65 / 66 |
| `TRAINER_GRUNT_MT_PYRE_4` | GRUNT | Story | 69 / 70 | 35 / 35 | 86 / 87 | 99 / 100 |
| `TRAINER_GRUNT_JAGGED_PASS` | GRUNT | Story | 48 / 49 | 24 / 25 | 60 / 61 | 69 / 71 |
| `TRAINER_MARC` | MARC | Ordinary | 21 / 22 | 10 / 11 | 25 / 26 | 29 / 30 |
| `TRAINER_BRENDEN` | BRENDEN | Ordinary | 30 | 15 | 36 | 42 |
| `TRAINER_LILITH` | LILITH | Ordinary | 29 | 15 | 34 | 40 |
| `TRAINER_CRISTIAN` | CRISTIAN | Ordinary | 30 | 15 | 36 | 42 |
| `TRAINER_SYLVIA` | SYLVIA | Ordinary | 73 | 37 | 87 | 100 |
| `TRAINER_LEONARDO` | LEONARDO | Ordinary | 74 | 37 | 88 | 100 |
| `TRAINER_ATHENA` | ATHENA | Ordinary | 76 / 78 | 38 / 39 | 91 / 93 | 98 / 100 |
| `TRAINER_HARRISON` | HARRISON | Ordinary | 80 | 40 | 96 | 100 |
| `TRAINER_GRUNT_MT_CHIMNEY_2` | GRUNT | Story | 49 | 25 | 61 | 71 |
| `TRAINER_CLARENCE` | CLARENCE | Ordinary | 80 | 40 | 96 | 100 |
| `TRAINER_TERRY` | TERRY | Ordinary | 54 | 27 | 64 | 75 |
| `TRAINER_NATE` | NATE | Ordinary | 73 | 37 | 87 | 100 |
| `TRAINER_KATHLEEN` | Sam | Protected | 71 / 71 / 71 / 71 | 36 / 36 / 36 / 36 | 85 / 85 / 85 / 85 | 92 / 92 / 92 / 92 |
| `TRAINER_CLIFFORD` | CLIFFORD | Ordinary | 73 | 37 | 87 | 100 |
| `TRAINER_NICHOLAS` | NICHOLAS | Ordinary | 73 | 37 | 87 | 100 |
| `TRAINER_GRUNT_SPACE_CENTER_3` | GRUNT | Story | 79 / 80 | 40 / 40 | 98 / 100 | 99 / 100 |
| `TRAINER_GRUNT_SPACE_CENTER_4` | GRUNT | Story | 80 | 40 | 100 | 100 |
| `TRAINER_GRUNT_SPACE_CENTER_5` | GRUNT | Story | 82 | 41 | 100 | 100 |
| `TRAINER_GRUNT_SPACE_CENTER_6` | GRUNT | Story | 82 | 41 | 100 | 100 |
| `TRAINER_GRUNT_SPACE_CENTER_7` | GRUNT | Story | 82 | 41 | 100 | 100 |
| `TRAINER_MACEY` | MACEY | Ordinary | 73 | 37 | 87 | 100 |
| `TRAINER_BRENDAN_RUSTBORO_TREECKO` | BRENDAN | Story | 22 / 23 / 24 | 11 / 12 / 12 | 27 / 28 / 30 | 31 / 33 / 34 |
| `TRAINER_BRENDAN_RUSTBORO_MUDKIP` | BRENDAN | Story | 22 / 23 / 24 | 11 / 12 / 12 | 27 / 28 / 30 | 31 / 33 / 34 |
| `TRAINER_PAXTON` | PAXTON | Ordinary | 80 / 82 | 40 / 41 | 96 / 98 | 98 / 100 |
| `TRAINER_ISABELLA` | ISABELLA | Ordinary | 72 | 36 | 86 | 100 |
| `TRAINER_GRUNT_WEATHER_INST_5` | GRUNT | Story | 61 / 62 | 31 / 31 | 76 / 77 | 88 / 89 |
| `TRAINER_TABITHA_MT_CHIMNEY` | TABITHA | Story | 46 / 47 / 48 / 49 | 23 / 24 / 24 / 25 | 57 / 58 / 60 / 61 | 66 / 68 / 69 / 71 |
| `TRAINER_JONATHAN` | JONATHAN | Ordinary | 80 / 82 | 40 / 41 | 96 / 98 | 98 / 100 |
| `TRAINER_BRENDAN_RUSTBORO_TORCHIC` | BRENDAN | Story | 22 / 23 / 24 | 11 / 12 / 12 | 27 / 28 / 30 | 31 / 33 / 34 |
| `TRAINER_MAY_RUSTBORO_MUDKIP` | MAY | Story | 22 / 23 / 24 | 11 / 12 / 12 | 27 / 28 / 30 | 31 / 33 / 34 |
| `TRAINER_MAXIE_MAGMA_HIDEOUT` | MAXIE | Story | 70 / 70 / 71 / 72 / 73 / 74 | 35 / 35 / 36 / 36 / 37 / 37 | 87 / 87 / 88 / 90 / 91 / 92 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_MAXIE_MT_CHIMNEY` | MAXIE | Story | 46 / 47 / 48 / 49 / 50 | 23 / 24 / 24 / 25 / 25 | 57 / 58 / 60 / 61 / 62 | 66 / 68 / 69 / 71 / 72 |
| `TRAINER_TIANA` | TIANA | Ordinary | 13 / 15 | 7 / 8 | 15 / 18 | 18 / 21 |
| `TRAINER_HALEY_1` | HALEY | Ordinary | 14 / 15 | 7 / 8 | 16 / 18 | 19 / 21 |
| `TRAINER_JANICE` | JANICE | Ordinary | 20 | 10 | 24 | 28 |
| `TRAINER_VIVI` | VIVI | Story | 44 / 45 / 46 | 22 / 23 / 23 | 55 / 56 / 57 | 63 / 65 / 66 |
| `TRAINER_HALEY_2` | HALEY | Ordinary | 39 / 41 | 20 / 21 | 46 / 49 | 54 / 57 |
| `TRAINER_HALEY_3` | HALEY | Ordinary | 42 / 44 | 21 / 22 | 50 / 52 | 58 / 61 |
| `TRAINER_HALEY_4` | HALEY | Ordinary | 46 / 48 | 23 / 24 | 55 / 57 | 64 / 67 |
| `TRAINER_HALEY_5` | HALEY | Ordinary | 47 / 49 / 51 | 24 / 25 / 26 | 56 / 58 / 61 | 65 / 68 / 71 |
| `TRAINER_SALLY` | SALLY | Ordinary | 38 | 19 | 45 | 53 |
| `TRAINER_ROBIN` | ROBIN | Ordinary | 38 / 38 / 39 | 19 / 19 / 20 | 45 / 45 / 46 | 53 / 53 / 54 |
| `TRAINER_ANDREA` | ANDREA | Ordinary | 88 | 44 | 100 | 100 |
| `TRAINER_CRISSY` | CRISSY | Ordinary | 87 / 89 | 44 / 45 | 98 / 100 | 98 / 100 |
| `TRAINER_RICK` | RICK | Ordinary | 11 / 13 | 6 / 7 | 13 / 15 | 15 / 18 |
| `TRAINER_LYLE` | LYLE | Ordinary | 16 / 17 / 18 | 8 / 8 / 9 | 19 / 20 / 21 | 22 / 23 / 25 |
| `TRAINER_JOSE` | JOSE | Ordinary | 20 / 21 | 10 / 11 | 24 / 25 | 28 / 29 |
| `TRAINER_DOUG` | DOUG | Ordinary | 57 / 58 | 28 / 29 | 68 / 69 | 79 / 81 |
| `TRAINER_GREG` | GREG | Ordinary | 57 / 58 | 28 / 29 | 68 / 69 | 79 / 81 |
| `TRAINER_KENT` | KENT | Ordinary | 58 | 29 | 69 | 81 |
| `TRAINER_JAMES_1` | JAMES | Ordinary | 18 / 20 | 9 / 10 | 21 / 24 | 25 / 28 |
| `TRAINER_JAMES_2` | JAMES | Ordinary | 40 / 42 | 20 / 21 | 48 / 50 | 56 / 58 |
| `TRAINER_JAMES_3` | JAMES | Ordinary | 43 / 45 | 22 / 23 | 51 / 54 | 60 / 63 |
| `TRAINER_JAMES_4` | JAMES | Ordinary | 44 / 46 / 48 | 22 / 23 / 24 | 52 / 55 / 57 | 61 / 64 / 67 |
| `TRAINER_JAMES_5` | JAMES | Ordinary | 45 / 47 / 49 / 51 | 23 / 24 / 25 / 26 | 54 / 56 / 58 / 61 | 63 / 65 / 68 / 71 |
| `TRAINER_BRICE` | BRICE | Ordinary | 42 / 43 | 21 / 22 | 50 / 51 | 58 / 60 |
| `TRAINER_TRENT_1` | TRENT | Ordinary | 42 / 42 / 43 | 21 / 21 / 22 | 50 / 50 / 51 | 58 / 58 / 60 |
| `TRAINER_LENNY` | LENNY | Ordinary | 44 / 45 | 22 / 23 | 52 / 54 | 61 / 63 |
| `TRAINER_LUCAS_1` | LUCAS | Ordinary | 44 / 45 | 22 / 23 | 52 / 54 | 61 / 63 |
| `TRAINER_ALAN` | ALAN | Ordinary | 46 / 46 / 47 | 23 / 23 / 24 | 55 / 55 / 56 | 64 / 64 / 65 |
| `TRAINER_CLARK` | CLARK | Ordinary | 20 | 10 | 24 | 28 |
| `TRAINER_ERIC` | ERIC | Ordinary | 46 / 47 | 23 / 24 | 55 / 56 | 64 / 65 |
| `TRAINER_LUCAS_2` | LUCAS | Ordinary | 21 | 11 | 25 | 29 |
| `TRAINER_MIKE_1` | MIKE | Ordinary | 20 / 22 | 10 / 11 | 24 / 26 | 28 / 30 |
| `TRAINER_MIKE_2` | MIKE | Ordinary | 20 / 21 / 23 | 10 / 11 / 12 | 24 / 25 / 27 | 28 / 29 / 32 |
| `TRAINER_TRENT_2` | TRENT | Ordinary | 42 / 41 / 43 / 45 | 21 / 21 / 22 / 23 | 50 / 49 / 51 / 54 | 58 / 57 / 60 / 63 |
| `TRAINER_TRENT_3` | TRENT | Ordinary | 42 / 44 / 46 / 48 | 21 / 22 / 23 / 24 | 50 / 52 / 55 / 57 | 58 / 61 / 64 / 67 |
| `TRAINER_TRENT_4` | TRENT | Ordinary | 50 / 47 / 49 / 51 | 25 / 24 / 25 / 26 | 60 / 56 / 58 / 61 | 70 / 65 / 68 / 71 |
| `TRAINER_TRENT_5` | TRENT | Ordinary | 48 / 50 / 52 / 54 | 24 / 25 / 26 / 27 | 57 / 60 / 62 / 64 | 67 / 70 / 72 / 75 |
| `TRAINER_DEZ_AND_LUKE` | DEZ & LUKE | Ordinary | 66 / 68 | 33 / 34 | 79 / 81 | 92 / 95 |
| `TRAINER_LEA_AND_JED` | LEA & JED | Ordinary | 61 / 63 | 31 / 32 | 73 / 75 | 85 / 88 |
| `TRAINER_KIRA_AND_DAN_1` | KIRA & DAN | Ordinary | 56 / 57 | 28 / 29 | 67 / 68 | 78 / 79 |
| `TRAINER_KIRA_AND_DAN_2` | KIRA & DAN | Ordinary | 58 / 60 | 29 / 30 | 69 / 72 | 81 / 84 |
| `TRAINER_KIRA_AND_DAN_3` | KIRA & DAN | Ordinary | 61 / 63 | 31 / 32 | 73 / 75 | 85 / 88 |
| `TRAINER_KIRA_AND_DAN_4` | KIRA & DAN | Ordinary | 64 / 66 | 32 / 33 | 76 / 79 | 89 / 92 |
| `TRAINER_KIRA_AND_DAN_5` | KIRA & DAN | Ordinary | 67 / 69 | 34 / 35 | 80 / 82 | 93 / 96 |
| `TRAINER_JOHANNA` | JOHANNA | Ordinary | 35 | 18 | 42 | 49 |
| `TRAINER_GERALD` | GERALD | Ordinary | 47 | 24 | 56 | 65 |
| `TRAINER_VIVIAN` | VIVIAN | Ordinary | 40 / 41 | 20 / 21 | 48 / 49 | 56 / 57 |
| `TRAINER_DANIELLE` | DANIELLE | Ordinary | 47 | 24 | 56 | 65 |
| `TRAINER_HIDEO` | HIDEO | Ordinary | 57 / 58 | 28 / 29 | 68 / 69 | 79 / 81 |
| `TRAINER_KEIGO` | KEIGO | Ordinary | 59 / 61 | 30 / 31 | 70 / 73 | 82 / 85 |
| `TRAINER_RILEY` | RILEY | Ordinary | 59 / 61 | 30 / 31 | 70 / 73 | 82 / 85 |
| `TRAINER_FLINT` | FLINT | Ordinary | 61 / 62 | 30 / 31 | 73 / 74 | 85 / 86 |
| `TRAINER_ASHLEY` | ASHLEY | Ordinary | 61 / 61 / 62 | 30 / 30 / 31 | 73 / 73 / 74 | 85 / 85 / 86 |
| `TRAINER_WALLY_MAUVILLE` | WALLY | Story | 41 / 42 / 43 | 21 / 21 / 22 | 51 / 52 / 53 | 59 / 60 / 62 |
| `TRAINER_WALLY_VR_2` | WALLY | Story | 95 / 96 / 97 / 98 / 99 | 48 / 48 / 49 / 49 / 50 | 96 / 97 / 98 / 99 / 100 | 96 / 97 / 98 / 99 / 100 |
| `TRAINER_WALLY_VR_3` | WALLY | Story | 96 / 97 / 98 / 99 / 100 | 48 / 49 / 49 / 50 / 50 | 96 / 97 / 98 / 99 / 100 | 96 / 97 / 98 / 99 / 100 |
| `TRAINER_WALLY_VR_4` | WALLY | Story | 96 / 97 / 98 / 99 / 100 | 48 / 49 / 49 / 50 / 50 | 96 / 97 / 98 / 99 / 100 | 96 / 97 / 98 / 99 / 100 |
| `TRAINER_WALLY_VR_5` | WALLY | Story | 96 / 97 / 98 / 99 / 100 | 48 / 49 / 49 / 50 / 50 | 96 / 97 / 98 / 99 / 100 | 96 / 97 / 98 / 99 / 100 |
| `TRAINER_BRENDAN_LILYCOVE_MUDKIP` | BRENDAN | Story | 70 / 70 / 71 / 72 / 73 / 74 | 35 / 35 / 36 / 36 / 37 / 37 | 87 / 87 / 88 / 90 / 91 / 92 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_BRENDAN_LILYCOVE_TREECKO` | BRENDAN | Story | 70 / 70 / 71 / 72 / 73 / 74 | 35 / 35 / 36 / 36 / 37 / 37 | 87 / 87 / 88 / 90 / 91 / 92 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_BRENDAN_LILYCOVE_TORCHIC` | BRENDAN | Story | 70 / 70 / 71 / 72 / 73 / 74 | 35 / 35 / 36 / 36 / 37 / 37 | 87 / 87 / 88 / 90 / 91 / 92 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_MAY_LILYCOVE_MUDKIP` | MAY | Story | 70 / 70 / 71 / 72 / 73 / 74 | 35 / 35 / 36 / 36 / 37 / 37 | 87 / 87 / 88 / 90 / 91 / 92 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_MAY_LILYCOVE_TREECKO` | MAY | Story | 70 / 70 / 71 / 72 / 73 / 74 | 35 / 35 / 36 / 36 / 37 / 37 | 87 / 87 / 88 / 90 / 91 / 92 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_MAY_LILYCOVE_TORCHIC` | MAY | Story | 70 / 70 / 71 / 72 / 73 / 74 | 35 / 35 / 36 / 36 / 37 / 37 | 87 / 87 / 88 / 90 / 91 / 92 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_JONAH` | JONAH | Ordinary | 76 / 77 / 78 | 38 / 38 / 39 | 91 / 92 / 93 | 98 / 99 / 100 |
| `TRAINER_HENRY` | HENRY | Ordinary | 76 / 78 | 38 / 39 | 91 / 93 | 98 / 100 |
| `TRAINER_ROGER` | ROGER | Ordinary | 76 / 77 / 78 | 38 / 38 / 39 | 91 / 92 / 93 | 98 / 99 / 100 |
| `TRAINER_ALEXA` | ALEXA | Ordinary | 78 / 79 | 39 / 40 | 93 / 94 | 99 / 100 |
| `TRAINER_RUBEN` | RUBEN | Ordinary | 78 / 79 | 39 / 40 | 93 / 94 | 99 / 100 |
| `TRAINER_KOJI_1` | KOJI | Ordinary | 78 | 39 | 93 | 100 |
| `TRAINER_WAYNE` | WAYNE | Ordinary | 78 / 79 / 81 | 39 / 40 / 41 | 93 / 94 / 97 | 97 / 98 / 100 |
| `TRAINER_AIDAN` | AIDAN | Ordinary | 76 / 78 | 38 / 39 | 91 / 93 | 98 / 100 |
| `TRAINER_REED` | REED | Ordinary | 79 / 80 | 39 / 40 | 94 / 96 | 99 / 100 |
| `TRAINER_TISHA` | TISHA | Ordinary | 80 | 40 | 96 | 100 |
| `TRAINER_TORI_AND_TIA` | TORI & TIA | Ordinary | 43 / 44 | 21 / 22 | 51 / 52 | 60 / 61 |
| `TRAINER_KIM_AND_IRIS` | KIM & IRIS | Ordinary | 69 / 70 | 34 / 35 | 82 / 84 | 96 / 98 |
| `TRAINER_TYRA_AND_IVY` | TYRA & IVY | Ordinary | 44 / 45 | 22 / 23 | 52 / 54 | 61 / 63 |
| `TRAINER_MEL_AND_PAUL` | MEL & PAUL | Ordinary | 35 / 36 | 17 / 18 | 42 / 43 | 49 / 50 |
| `TRAINER_JOHN_AND_JAY_1` | JOHN & JAY | Ordinary | 78 / 79 | 39 / 40 | 93 / 94 | 99 / 100 |
| `TRAINER_JOHN_AND_JAY_2` | JOHN & JAY | Ordinary | 79 / 81 | 40 / 41 | 94 / 97 | 98 / 100 |
| `TRAINER_JOHN_AND_JAY_3` | JOHN & JAY | Ordinary | 82 / 84 | 41 / 42 | 98 / 100 | 98 / 100 |
| `TRAINER_JOHN_AND_JAY_4` | JOHN & JAY | Ordinary | 85 / 87 | 43 / 44 | 98 / 100 | 98 / 100 |
| `TRAINER_JOHN_AND_JAY_5` | JOHN & JAY | Ordinary | 88 / 90 | 44 / 45 | 98 / 100 | 98 / 100 |
| `TRAINER_RELI_AND_IAN` | RELI & IAN | Ordinary | 80 / 82 | 40 / 41 | 96 / 98 | 98 / 100 |
| `TRAINER_LILA_AND_ROY_1` | LILA & ROY | Ordinary | 70 / 72 | 35 / 36 | 84 / 86 | 98 / 100 |
| `TRAINER_LILA_AND_ROY_2` | LILA & ROY | Ordinary | 73 / 75 | 37 / 38 | 87 / 90 | 98 / 100 |
| `TRAINER_LILA_AND_ROY_3` | LILA & ROY | Ordinary | 76 / 78 | 38 / 39 | 91 / 93 | 98 / 100 |
| `TRAINER_LILA_AND_ROY_4` | LILA & ROY | Ordinary | 79 / 81 | 40 / 41 | 94 / 97 | 98 / 100 |
| `TRAINER_LILA_AND_ROY_5` | LILA & ROY | Ordinary | 82 / 84 | 41 / 42 | 98 / 100 | 98 / 100 |
| `TRAINER_LISA_AND_RAY` | LISA & RAY | Ordinary | 55 / 56 | 27 / 28 | 66 / 67 | 77 / 78 |
| `TRAINER_CHRIS` | CHRIS | Ordinary | 57 / 57 / 57 / 58 | 28 / 28 / 28 / 29 | 68 / 68 / 68 / 69 | 79 / 79 / 79 / 81 |
| `TRAINER_DAWSON` | DAWSON | Ordinary | 20 / 21 | 10 / 11 | 24 / 25 | 28 / 29 |
| `TRAINER_SARAH` | SARAH | Ordinary | 20 / 21 | 10 / 11 | 24 / 25 | 28 / 29 |
| `TRAINER_DARIAN` | DARIAN | Ordinary | 21 | 11 | 25 | 29 |
| `TRAINER_HAILEY` | HAILEY | Ordinary | 35 | 18 | 42 | 49 |
| `TRAINER_CHANDLER` | CHANDLER | Ordinary | 35 / 36 | 17 / 18 | 42 / 43 | 49 / 50 |
| `TRAINER_KALEB` | KALEB | Ordinary | 36 / 37 | 18 / 19 | 43 / 44 | 50 / 51 |
| `TRAINER_JOSEPH` | JOSEPH | Ordinary | 36 / 37 | 18 / 19 | 43 / 44 | 50 / 51 |
| `TRAINER_ALYSSA` | ALYSSA | Ordinary | 37 | 19 | 44 | 51 |
| `TRAINER_MARCOS` | MARCOS | Ordinary | 37 | 19 | 44 | 51 |
| `TRAINER_RHETT` | RHETT | Ordinary | 37 | 19 | 44 | 51 |
| `TRAINER_TYRON` | TYRON | Ordinary | 43 | 22 | 51 | 60 |
| `TRAINER_CELINA` | CELINA | Ordinary | 43 | 22 | 51 | 60 |
| `TRAINER_BIANCA` | BIANCA | Ordinary | 43 | 22 | 51 | 60 |
| `TRAINER_HAYDEN` | HAYDEN | Ordinary | 43 | 22 | 51 | 60 |
| `TRAINER_SOPHIE` | SOPHIE | Ordinary | 43 / 44 | 21 / 22 | 51 / 52 | 60 / 61 |
| `TRAINER_COBY` | COBY | Ordinary | 43 / 44 | 21 / 22 | 51 / 52 | 60 / 61 |
| `TRAINER_LAWRENCE` | LAWRENCE | Ordinary | 43 / 44 | 21 / 22 | 51 / 52 | 60 / 61 |
| `TRAINER_WYATT` | WYATT | Ordinary | 43 / 44 | 21 / 22 | 51 / 52 | 60 / 61 |
| `TRAINER_ANGELINA` | ANGELINA | Ordinary | 44 / 45 | 22 / 23 | 52 / 54 | 61 / 63 |
| `TRAINER_KAI` | KAI | Ordinary | 44 | 22 | 52 | 61 |
| `TRAINER_CHARLOTTE` | CHARLOTTE | Ordinary | 44 | 22 | 52 | 61 |
| `TRAINER_DEANDRE` | DEANDRE | Ordinary | 40 / 40 / 41 | 20 / 20 / 21 | 48 / 48 / 49 | 56 / 56 / 57 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_1` | GRUNT | Story | 71 | 36 | 88 | 100 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_2` | GRUNT | Story | 71 | 36 | 88 | 100 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_3` | GRUNT | Story | 71 | 36 | 88 | 100 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_4` | GRUNT | Story | 70 / 71 | 35 / 36 | 87 / 88 | 99 / 100 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_5` | GRUNT | Story | 70 / 71 | 35 / 36 | 87 / 88 | 99 / 100 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_6` | GRUNT | Story | 71 | 36 | 88 | 100 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_7` | GRUNT | Story | 71 | 36 | 88 | 100 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_8` | GRUNT | Story | 71 | 36 | 88 | 100 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_9` | GRUNT | Story | 71 | 36 | 88 | 100 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_10` | GRUNT | Story | 71 | 36 | 88 | 100 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_11` | GRUNT | Story | 71 | 36 | 88 | 100 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_12` | GRUNT | Story | 71 | 36 | 88 | 100 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_13` | GRUNT | Story | 71 | 36 | 88 | 100 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_14` | GRUNT | Story | 71 | 36 | 88 | 100 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_15` | GRUNT | Story | 71 | 36 | 88 | 100 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_16` | GRUNT | Story | 71 | 36 | 88 | 100 |
| `TRAINER_TABITHA_MAGMA_HIDEOUT` | TABITHA | Story | 68 / 69 / 70 / 71 / 72 | 34 / 35 / 35 / 36 / 36 | 85 / 86 / 87 / 88 / 90 | 96 / 97 / 98 / 99 / 100 |
| `TRAINER_DARCY` | DARCY | Ordinary | 80 / 82 | 40 / 41 | 96 / 98 | 98 / 100 |
| `TRAINER_MAXIE_MOSSDEEP` | MAXIE | Story | 80 / 81 / 82 / 83 / 84 | 40 / 41 / 41 / 42 / 42 | 96 / 97 / 98 / 99 / 100 | 96 / 97 / 98 / 99 / 100 |
| `TRAINER_PETE` | PETE | Ordinary | 37 | 19 | 44 | 51 |
| `TRAINER_ISABELLE` | ISABELLE | Ordinary | 37 | 19 | 44 | 51 |
| `TRAINER_ANDRES_1` | ANDRES | Ordinary | 55 / 56 | 27 / 28 | 66 / 67 | 77 / 78 |
| `TRAINER_JOSUE` | JOSUE | Ordinary | 55 / 56 | 27 / 28 | 66 / 67 | 77 / 78 |
| `TRAINER_CAMRON` | CAMRON | Ordinary | 56 | 28 | 67 | 78 |
| `TRAINER_CORY_1` | CORY | Ordinary | 55 / 55 / 56 | 27 / 27 / 28 | 66 / 66 / 67 | 77 / 77 / 78 |
| `TRAINER_CAROLINA` | CAROLINA | Ordinary | 55 / 55 / 56 | 27 / 27 / 28 | 66 / 66 / 67 | 77 / 77 / 78 |
| `TRAINER_ELIJAH` | ELIJAH | Ordinary | 35 / 36 | 17 / 18 | 42 / 43 | 49 / 50 |
| `TRAINER_CELIA` | CELIA | Ordinary | 45 / 46 | 22 / 23 | 54 / 55 | 63 / 64 |
| `TRAINER_BRYAN` | BRYAN | Ordinary | 45 / 46 | 22 / 23 | 54 / 55 | 63 / 64 |
| `TRAINER_BRANDEN` | BRANDEN | Ordinary | 45 / 46 | 22 / 23 | 54 / 55 | 63 / 64 |
| `TRAINER_BRYANT` | BRYANT | Ordinary | 42 / 43 | 21 / 22 | 50 / 51 | 58 / 60 |
| `TRAINER_SHAYLA` | SHAYLA | Ordinary | 42 / 43 | 21 / 22 | 50 / 51 | 58 / 60 |
| `TRAINER_KYRA` | KYRA | Ordinary | 56 / 57 | 28 / 29 | 67 / 68 | 78 / 79 |
| `TRAINER_JAIDEN` | JAIDEN | Ordinary | 56 / 57 | 28 / 29 | 67 / 68 | 78 / 79 |
| `TRAINER_ALIX` | ALIX | Ordinary | 56 / 57 | 28 / 29 | 67 / 68 | 78 / 79 |
| `TRAINER_HELENE` | HELENE | Ordinary | 56 / 57 | 28 / 29 | 67 / 68 | 78 / 79 |
| `TRAINER_MARLENE` | MARLENE | Ordinary | 43 / 44 | 21 / 22 | 51 / 52 | 60 / 61 |
| `TRAINER_DEVAN` | DEVAN | Ordinary | 20 / 21 | 10 / 11 | 24 / 25 | 28 / 29 |
| `TRAINER_JOHNSON` | JOHNSON | Ordinary | 20 / 21 | 10 / 11 | 24 / 25 | 28 / 29 |
| `TRAINER_MELINA` | MELINA | Ordinary | 39 | 20 | 46 | 54 |
| `TRAINER_BRANDI` | BRANDI | Ordinary | 39 | 20 | 46 | 54 |
| `TRAINER_AISHA` | AISHA | Ordinary | 39 | 20 | 46 | 54 |
| `TRAINER_MAKAYLA` | MAKAYLA | Ordinary | 80 / 82 | 40 / 41 | 96 / 98 | 98 / 100 |
| `TRAINER_FABIAN` | FABIAN | Ordinary | 58 | 29 | 69 | 81 |
| `TRAINER_DAYTON` | DAYTON | Ordinary | 57 / 58 | 28 / 29 | 68 / 69 | 79 / 81 |
| `TRAINER_RACHEL` | RACHEL | Ordinary | 58 | 29 | 69 | 81 |
| `TRAINER_LEONEL` | LEONEL | Ordinary | 61 | 31 | 73 | 85 |
| `TRAINER_CALLIE` | CALLIE | Ordinary | 59 / 61 | 30 / 31 | 70 / 73 | 82 / 85 |
| `TRAINER_CALE` | CALE | Ordinary | 66 / 67 | 33 / 34 | 79 / 80 | 92 / 93 |
| `TRAINER_MYLES` | Scotty | Protected | 67 | 34 | 80 | 87 |
| `TRAINER_PAT` | PAT | Ordinary | 66 / 66 / 66 / 66 / 66 / 67 | 33 / 33 / 33 / 33 / 33 / 34 | 79 / 79 / 79 / 79 / 79 / 80 | 92 / 92 / 92 / 92 / 92 / 93 |
| `TRAINER_CRISTIN_1` | CRISTIN | Ordinary | 66 / 67 | 33 / 34 | 79 / 80 | 92 / 93 |
| `TRAINER_MAY_RUSTBORO_TREECKO` | MAY | Story | 22 / 23 / 24 | 11 / 12 / 12 | 27 / 28 / 30 | 31 / 33 / 34 |
| `TRAINER_MAY_RUSTBORO_TORCHIC` | MAY | Story | 22 / 23 / 24 | 11 / 12 / 12 | 27 / 28 / 30 | 31 / 33 / 34 |
| `TRAINER_ROXANNE_2` | ROXANNE | Story | 56 / 57 / 58 / 59 / 60 | 28 / 29 / 29 / 30 / 30 | 70 / 71 / 72 / 73 / 75 | 81 / 82 / 84 / 85 / 87 |
| `TRAINER_ROXANNE_3` | ROXANNE | Story | 66 / 66 / 67 / 68 / 69 / 70 | 33 / 33 / 34 / 34 / 35 / 35 | 82 / 82 / 83 / 85 / 86 / 87 | 95 / 95 / 97 / 98 / 99 / 100 |
| `TRAINER_ROXANNE_4` | ROXANNE | Story | 76 / 76 / 77 / 78 / 79 / 80 | 38 / 38 / 39 / 39 / 40 / 40 | 95 / 95 / 96 / 97 / 98 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_ROXANNE_5` | ROXANNE | Story | 96 / 96 / 97 / 98 / 99 / 100 | 48 / 48 / 49 / 49 / 50 / 50 | 96 / 96 / 97 / 98 / 99 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_BRAWLY_2` | BRAWLY | Story | 67 / 68 / 69 / 70 | 34 / 34 / 35 / 35 | 83 / 85 / 86 / 87 | 97 / 98 / 99 / 100 |
| `TRAINER_BRAWLY_3` | BRAWLY | Story | 77 / 78 / 79 / 80 | 39 / 39 / 40 / 40 | 96 / 97 / 98 / 100 | 97 / 98 / 99 / 100 |
| `TRAINER_BRAWLY_4` | BRAWLY | Story | 86 / 87 / 88 / 89 / 90 | 43 / 44 / 44 / 45 / 45 | 96 / 97 / 98 / 99 / 100 | 96 / 97 / 98 / 99 / 100 |
| `TRAINER_BRAWLY_5` | BRAWLY | Story | 96 / 96 / 97 / 98 / 99 / 100 | 48 / 48 / 49 / 49 / 50 / 50 | 96 / 96 / 97 / 98 / 99 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_WATTSON_2` | WATTSON | Story | 77 / 78 / 79 / 80 | 39 / 39 / 40 / 40 | 96 / 97 / 98 / 100 | 97 / 98 / 99 / 100 |
| `TRAINER_WATTSON_3` | WATTSON | Story | 79 / 80 / 81 / 82 / 83 | 40 / 40 / 41 / 41 / 42 | 96 / 97 / 98 / 99 / 100 | 96 / 97 / 98 / 99 / 100 |
| `TRAINER_WATTSON_4` | WATTSON | Story | 86 / 87 / 88 / 89 / 90 | 43 / 44 / 44 / 45 / 45 | 96 / 97 / 98 / 99 / 100 | 96 / 97 / 98 / 99 / 100 |
| `TRAINER_WATTSON_5` | WATTSON | Story | 96 / 96 / 97 / 98 / 99 / 100 | 48 / 48 / 49 / 49 / 50 / 50 | 96 / 96 / 97 / 98 / 99 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_FLANNERY_2` | FLANNERY | Story | 76 / 77 / 78 / 79 / 80 | 38 / 39 / 39 / 40 / 40 | 95 / 96 / 97 / 98 / 100 | 96 / 97 / 98 / 99 / 100 |
| `TRAINER_FLANNERY_3` | FLANNERY | Story | 96 / 96 / 97 / 98 / 99 / 100 | 48 / 48 / 49 / 49 / 50 / 50 | 96 / 96 / 97 / 98 / 99 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_FLANNERY_4` | FLANNERY | Story | 96 / 96 / 97 / 98 / 99 / 100 | 48 / 48 / 49 / 49 / 50 / 50 | 96 / 96 / 97 / 98 / 99 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_FLANNERY_5` | FLANNERY | Story | 96 / 96 / 97 / 98 / 99 / 100 | 48 / 48 / 49 / 49 / 50 / 50 | 96 / 96 / 97 / 98 / 99 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_NORMAN_2` | NORMAN | Story | 96 / 97 / 98 / 99 / 100 | 48 / 49 / 49 / 50 / 50 | 96 / 97 / 98 / 99 / 100 | 96 / 97 / 98 / 99 / 100 |
| `TRAINER_NORMAN_3` | NORMAN | Story | 96 / 96 / 97 / 98 / 99 / 100 | 48 / 48 / 49 / 49 / 50 / 50 | 96 / 96 / 97 / 98 / 99 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_NORMAN_4` | NORMAN | Story | 96 / 96 / 97 / 98 / 99 / 100 | 48 / 48 / 49 / 49 / 50 / 50 | 96 / 96 / 97 / 98 / 99 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_NORMAN_5` | NORMAN | Story | 96 / 96 / 97 / 98 / 99 / 100 | 48 / 48 / 49 / 49 / 50 / 50 | 96 / 96 / 97 / 98 / 99 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_WINONA_2` | WINONA | Story | 86 / 86 / 87 / 88 / 89 / 90 | 43 / 43 / 44 / 44 / 45 / 45 | 96 / 96 / 97 / 98 / 99 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_WINONA_3` | WINONA | Story | 96 / 96 / 97 / 98 / 99 / 100 | 48 / 48 / 49 / 49 / 50 / 50 | 96 / 96 / 97 / 98 / 99 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_WINONA_4` | WINONA | Story | 96 / 96 / 97 / 98 / 99 / 100 | 48 / 48 / 49 / 49 / 50 / 50 | 96 / 96 / 97 / 98 / 99 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_WINONA_5` | WINONA | Story | 96 / 96 / 97 / 98 / 99 / 100 | 48 / 48 / 49 / 49 / 50 / 50 | 96 / 96 / 97 / 98 / 99 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_TATE_AND_LIZA_2` | TATE&LIZA | Story | 91 / 91 / 92 / 93 / 94 / 95 | 46 / 46 / 46 / 47 / 47 / 48 | 96 / 96 / 97 / 98 / 99 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_TATE_AND_LIZA_3` | TATE&LIZA | Story | 96 / 96 / 97 / 98 / 99 / 100 | 48 / 48 / 49 / 49 / 50 / 50 | 96 / 96 / 97 / 98 / 99 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_TATE_AND_LIZA_4` | TATE&LIZA | Story | 96 / 96 / 97 / 98 / 99 / 100 | 48 / 48 / 49 / 49 / 50 / 50 | 96 / 96 / 97 / 98 / 99 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_TATE_AND_LIZA_5` | TATE&LIZA | Story | 96 / 96 / 97 / 98 / 99 / 100 | 48 / 48 / 49 / 49 / 50 / 50 | 96 / 96 / 97 / 98 / 99 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_JUAN_2` | JUAN | Story | 96 / 96 / 97 / 98 / 99 / 100 | 48 / 48 / 49 / 49 / 50 / 50 | 96 / 96 / 97 / 98 / 99 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_JUAN_3` | JUAN | Story | 96 / 96 / 97 / 98 / 99 / 100 | 48 / 48 / 49 / 49 / 50 / 50 | 96 / 96 / 97 / 98 / 99 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_JUAN_4` | JUAN | Story | 96 / 96 / 97 / 98 / 99 / 100 | 48 / 48 / 49 / 49 / 50 / 50 | 96 / 96 / 97 / 98 / 99 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_JUAN_5` | JUAN | Story | 96 / 96 / 97 / 98 / 99 / 100 | 48 / 48 / 49 / 49 / 50 / 50 | 96 / 96 / 97 / 98 / 99 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_ANGELO` | ANGELO | Ordinary | 40 / 41 | 20 / 21 | 48 / 49 | 56 / 57 |
| `TRAINER_DARIUS` | DARIUS | Ordinary | 61 | 31 | 73 | 85 |
| `TRAINER_STEVEN` | STEVEN | Story | 96 / 96 / 97 / 98 / 99 / 100 | 48 / 48 / 49 / 49 / 50 / 50 | 96 / 96 / 97 / 98 / 99 / 100 | 96 / 96 / 97 / 98 / 99 / 100 |
| `TRAINER_ANABEL` | ANABEL | Metadata | 5 | 5 | 5 | 5 |
| `TRAINER_TUCKER` | TUCKER | Metadata | 5 | 5 | 5 | 5 |
| `TRAINER_SPENSER` | SPENSER | Metadata | 5 | 5 | 5 | 5 |
| `TRAINER_GRETA` | GRETA | Metadata | 5 | 5 | 5 | 5 |
| `TRAINER_NOLAND` | NOLAND | Metadata | 5 | 5 | 5 | 5 |
| `TRAINER_LUCY` | LUCY | Metadata | 5 | 5 | 5 | 5 |
| `TRAINER_BRANDON` | BRANDON | Metadata | 5 | 5 | 5 | 5 |
| `TRAINER_ANDRES_2` | ANDRES | Ordinary | 57 / 59 | 29 / 30 | 68 / 70 | 79 / 82 |
| `TRAINER_ANDRES_3` | ANDRES | Ordinary | 58 / 60 / 62 | 29 / 30 / 31 | 69 / 72 / 74 | 81 / 84 / 86 |
| `TRAINER_ANDRES_4` | ANDRES | Ordinary | 61 / 63 / 65 | 31 / 32 / 33 | 73 / 75 / 78 | 85 / 88 / 91 |
| `TRAINER_ANDRES_5` | ANDRES | Ordinary | 64 / 66 / 68 | 32 / 33 / 34 | 76 / 79 / 81 | 89 / 92 / 95 |
| `TRAINER_CORY_2` | CORY | Ordinary | 54 / 56 / 58 | 27 / 28 / 29 | 64 / 67 / 69 | 75 / 78 / 81 |
| `TRAINER_CORY_3` | CORY | Ordinary | 57 / 59 / 61 | 29 / 30 / 31 | 68 / 70 / 73 | 79 / 82 / 85 |
| `TRAINER_CORY_4` | CORY | Ordinary | 60 / 62 / 64 | 30 / 31 / 32 | 72 / 74 / 76 | 84 / 86 / 89 |
| `TRAINER_CORY_5` | CORY | Ordinary | 63 / 65 / 67 | 32 / 33 / 34 | 75 / 78 / 80 | 88 / 91 / 93 |
| `TRAINER_PABLO_2` | PABLO | Ordinary | 75 / 77 | 38 / 39 | 90 / 92 | 98 / 100 |
| `TRAINER_PABLO_3` | PABLO | Ordinary | 76 / 78 / 80 | 38 / 39 / 40 | 91 / 93 / 96 | 96 / 98 / 100 |
| `TRAINER_PABLO_4` | PABLO | Ordinary | 79 / 81 / 83 | 40 / 41 / 42 | 94 / 97 / 99 | 96 / 98 / 100 |
| `TRAINER_PABLO_5` | PABLO | Ordinary | 82 / 84 / 86 | 41 / 42 / 43 | 96 / 98 / 100 | 96 / 98 / 100 |
| `TRAINER_KOJI_2` | KOJI | Ordinary | 79 / 81 | 40 / 41 | 94 / 97 | 98 / 100 |
| `TRAINER_KOJI_3` | KOJI | Ordinary | 80 / 82 / 84 | 40 / 41 / 42 | 96 / 98 / 100 | 96 / 98 / 100 |
| `TRAINER_KOJI_4` | KOJI | Ordinary | 83 / 85 / 87 | 42 / 43 / 44 | 96 / 98 / 100 | 96 / 98 / 100 |
| `TRAINER_KOJI_5` | KOJI | Ordinary | 86 / 88 / 90 | 43 / 44 / 45 | 96 / 98 / 100 | 96 / 98 / 100 |
| `TRAINER_CRISTIN_2` | CRISTIN | Ordinary | 68 / 70 | 34 / 35 | 81 / 84 | 95 / 98 |
| `TRAINER_CRISTIN_3` | CRISTIN | Ordinary | 69 / 71 / 73 | 35 / 36 / 37 | 82 / 85 / 87 | 96 / 98 / 100 |
| `TRAINER_CRISTIN_4` | CRISTIN | Ordinary | 72 / 74 / 76 | 36 / 37 / 38 | 86 / 88 / 91 | 96 / 98 / 100 |
| `TRAINER_CRISTIN_5` | CRISTIN | Ordinary | 75 / 77 / 79 | 38 / 39 / 40 | 90 / 92 / 94 | 96 / 98 / 100 |
| `TRAINER_FERNANDO_2` | FERNANDO | Ordinary | 68 / 70 / 72 | 34 / 35 / 36 | 81 / 84 / 86 | 95 / 98 / 100 |
| `TRAINER_FERNANDO_3` | FERNANDO | Ordinary | 71 / 73 / 75 | 36 / 37 / 38 | 85 / 87 / 90 | 96 / 98 / 100 |
| `TRAINER_FERNANDO_4` | FERNANDO | Ordinary | 74 / 76 / 78 | 37 / 38 / 39 | 88 / 91 / 93 | 96 / 98 / 100 |
| `TRAINER_FERNANDO_5` | FERNANDO | Ordinary | 77 / 79 / 81 | 39 / 40 / 41 | 92 / 94 / 97 | 96 / 98 / 100 |
| `TRAINER_SAWYER_2` | SAWYER | Ordinary | 47 / 49 | 24 / 25 | 56 / 58 | 65 / 68 |
| `TRAINER_SAWYER_3` | SAWYER | Ordinary | 48 / 50 / 52 | 24 / 25 / 26 | 57 / 60 / 62 | 67 / 70 / 72 |
| `TRAINER_SAWYER_4` | SAWYER | Ordinary | 51 / 53 / 55 | 26 / 27 / 28 | 61 / 63 / 66 | 71 / 74 / 77 |
| `TRAINER_SAWYER_5` | SAWYER | Ordinary | 54 / 56 / 58 | 27 / 28 / 29 | 64 / 67 / 69 | 75 / 78 / 81 |
| `TRAINER_GABRIELLE_2` | GABRIELLE | Ordinary | 59 / 61 / 63 / 65 / 67 / 69 | 30 / 31 / 32 / 33 / 34 / 35 | 70 / 73 / 75 / 78 / 80 / 82 | 82 / 85 / 88 / 91 / 93 / 96 |
| `TRAINER_GABRIELLE_3` | GABRIELLE | Ordinary | 62 / 64 / 66 / 68 / 70 / 72 | 31 / 32 / 33 / 34 / 35 / 36 | 74 / 76 / 79 / 81 / 84 / 86 | 86 / 89 / 92 / 95 / 98 / 100 |
| `TRAINER_GABRIELLE_4` | GABRIELLE | Ordinary | 65 / 67 / 69 / 71 / 73 / 75 | 33 / 34 / 35 / 36 / 37 / 38 | 78 / 80 / 82 / 85 / 87 / 90 | 90 / 92 / 94 / 96 / 98 / 100 |
| `TRAINER_GABRIELLE_5` | GABRIELLE | Ordinary | 68 / 70 / 72 / 74 / 76 / 78 | 34 / 35 / 36 / 37 / 38 / 39 | 81 / 84 / 86 / 88 / 91 / 93 | 90 / 92 / 94 / 96 / 98 / 100 |
| `TRAINER_THALIA_2` | THALIA | Ordinary | 58 / 60 | 29 / 30 | 69 / 72 | 81 / 84 |
| `TRAINER_THALIA_3` | THALIA | Ordinary | 59 / 61 / 63 | 30 / 31 / 32 | 70 / 73 / 75 | 82 / 85 / 88 |
| `TRAINER_THALIA_4` | THALIA | Ordinary | 62 / 64 / 66 | 31 / 32 / 33 | 74 / 76 / 79 | 86 / 89 / 92 |
| `TRAINER_THALIA_5` | THALIA | Ordinary | 65 / 67 / 69 | 33 / 34 / 35 | 78 / 80 / 82 | 91 / 93 / 96 |
| `TRAINER_MARIELA` | MARIELA | Ordinary | 80 | 40 | 96 | 100 |
| `TRAINER_ALVARO` | ALVARO | Ordinary | 78 / 80 | 39 / 40 | 93 / 96 | 98 / 100 |
| `TRAINER_EVERETT` | EVERETT | Ordinary | 80 | 40 | 96 | 100 |
| `TRAINER_RED` | RED | Metadata | 5 | 5 | 5 | 5 |
| `TRAINER_LEAF` | LEAF | Metadata | 5 | 5 | 5 | 5 |
| `TRAINER_BRENDAN_PLACEHOLDER` | BRENDAN | Metadata | 5 | 5 | 5 | 5 |
| `TRAINER_MAY_PLACEHOLDER` | MAY | Metadata | 5 | 5 | 5 | 5 |
| `TRAINER_RED2` | RED | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_BLUE` | BLUE | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_CYNTHIA` | Cynthia | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_IRIS` | Iris | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_DIANTHA` | Diantha | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_LEON` | Leon | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_NEMONA` | Nemona | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_VOLO` | Volo | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_UNBORNE` | Unborne | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |

863 populated trainer entries; *Hard/Extreme values assume a cap of 100; earlier badge caps still apply.

Regenerate with `python tools/brisk/trainer_mode_plan.py`; validate with `--check`.
