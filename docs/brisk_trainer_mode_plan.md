# Brisk trainer plan by difficulty

This is the complete Hoenn trainer plan, including ordinary trainers, rematches,
story fights, rivals, gyms, the League and dedication/Easter egg encounters.
Normal levels are authored per encounter; revisiting an old route does not
scale its trainers up to the player. Important teams retain their definitions.

## Progression targets

| Section | Normal | Easy | Hard, before badge cap | Extreme, before badge cap |
| --- | --- | --- | --- | --- |
| Route 102 | 10–15 | 5–8 | 13–18 | 14–19 |
| Route 104 | 14–21 | 7–11 | 17–25 | 18–27 |
| Petalburg Woods | 16–20 | 8–10 | 19–24 | 20–26 |
| Route 116 / Rusturf approach | 20–24 | 10–12 | 24–28 | 26–31 |
| Rustboro Gym trainers | 21–24 | 11–12 | 25–28 | 27–31 |
| Dewford / Brawly approach | 28–34 | 14–17 | 33–40 | 36–44 |
| Slateport / Route 110 | 35–42 | 18–21 | 42–50 | 45–54 |
| Wattson approach | 40–44 | 20–22 | 48–52 | 52–57 |
| Northern routes / Flannery approach | 43–49 | 22–25 | 51–58 | 55–63 |
| Norman approach | 51–54 | 26–27 | 61–64 | 66–70 |
| Route 119 / Fortree | 57–64 | 29–32 | 68–76 | 74–83 |
| Lilycove / hideouts | 66–73 | 33–37 | 79–87 | 85–94 |
| Mossdeep Gym approach | 71–74 | 36–37 | 85–88 | 92–96 |
| Space Center / eastern sea | 76–83 | 38–42 | 91–99 | 98–100 |
| Seafloor / Juan approach | 82–89 | 41–45 | 98–100 | 100–100 |
| Victory Road | 86–94 | 43–47 | 100–100 | 100–100 |
| League | 100–100 | 50–50 | 100–100 | 100–100 |

The section bands describe ordinary encounter targets, not blanket map scaling.
Individual teams and intentional protected exceptions are listed below.
The opening Route 103 tutorial rival and protected story encounters retain
their existing levels; they are separate from Route 102 ordinary trainers.

## Mode rules

| Rule | Easy | Normal | Hard | Extreme |
| --- | --- | --- | --- | --- |
| Trainer levels | Half, rounded up | Authored curve | +20%, at least +3 | +30%, at least +4 |
| IVs | 0 | 8 / 12 / 18 / 24 by stage | 31 | 31 |
| EVs | 0 | 0 / 128 / 256 / 384 by stage | 128 / 256 / 384 / 510 (at least authored total) | 510, chosen for the set |
| Ordinary moves | Weaker legal attacks | Legal authored coverage/support | Optimized ordinary moves and stronger abilities | Optimized legal moves within stage power limits |
| AI | Basic bad-move checks | Basic move scoring | Smart switching, HP and KO checks | Omniscient with smart switching |
| Battle style | Shift | Player preference | Set | Set |
| Player battle consumables | Allowed | Allowed | Blocked | Blocked |
| Shared EXP | 200% | 100% | 50% | Disabled |

Hard and Extreme trainer levels are additionally clamped to the save’s current
badge cap: **25 / 35 / 45 / 50 / 55 / 65 / 75 / 90 / 100**. The individual
table shows boosts before that clamp because the same rematch can be fought
at different progression stages. Extreme enforces the player EXP cap too.
Difficulty does not evolve an opponent into a different species at runtime.

Only the 670 ordinary trainer IDs use the new move-power guard. Important
and dedication/Easter egg teams keep their previous mode behavior. Ordinary
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
unchanged by this pass; their existing runtime difficulty modifiers remain.

| Trainer ID | Display name | Role | Normal | Easy | Hard* | Extreme* |
| --- | --- | --- | --- | --- | --- | --- |
| `TRAINER_SAWYER_1` | SAWYER | Ordinary | 46 | 23 | 55 | 59 |
| `TRAINER_GRUNT_AQUA_HIDEOUT_1` | GRUNT | Protected | 69 | 35 | 82 | 89 |
| `TRAINER_GRUNT_AQUA_HIDEOUT_2` | GRUNT | Protected | 69 / 69 | 35 / 35 | 82 / 82 | 89 / 89 |
| `TRAINER_GRUNT_AQUA_HIDEOUT_3` | GRUNT | Protected | 69 | 35 | 82 | 89 |
| `TRAINER_GRUNT_AQUA_HIDEOUT_4` | GRUNT | Protected | 69 | 35 | 82 | 89 |
| `TRAINER_GRUNT_SEAFLOOR_CAVERN_1` | GRUNT | Protected | 82 | 41 | 98 | 100 |
| `TRAINER_GRUNT_SEAFLOOR_CAVERN_2` | GRUNT | Protected | 82 | 41 | 98 | 100 |
| `TRAINER_GRUNT_SEAFLOOR_CAVERN_3` | GRUNT | Protected | 82 | 41 | 98 | 100 |
| `TRAINER_GABRIELLE_1` | GABRIELLE | Ordinary | 66 / 66 / 66 / 66 / 66 / 66 | 33 / 33 / 33 / 33 / 33 / 33 | 79 / 79 / 79 / 79 / 79 / 79 | 85 / 85 / 85 / 85 / 85 / 85 |
| `TRAINER_GRUNT_PETALBURG_WOODS` | GRUNT | Protected | 12 | 6 | 15 | 16 |
| `TRAINER_MARCEL` | MARCEL | Ordinary | 66 / 66 | 33 / 33 | 79 / 79 | 85 / 85 |
| `TRAINER_ALBERTO` | ALBERTO | Ordinary | 67 / 66 | 34 / 33 | 80 / 79 | 87 / 85 |
| `TRAINER_ED` | ED | Ordinary | 66 / 66 | 33 / 33 | 79 / 79 | 85 / 85 |
| `TRAINER_GRUNT_SEAFLOOR_CAVERN_4` | GRUNT | Protected | 82 | 41 | 98 | 100 |
| `TRAINER_DECLAN` | DECLAN | Ordinary | 68 | 34 | 81 | 88 |
| `TRAINER_GRUNT_RUSTURF_TUNNEL` | GRUNT | Protected | 20 | 10 | 24 | 26 |
| `TRAINER_GRUNT_WEATHER_INST_1` | GRUNT | Protected | 58 / 58 | 29 / 29 | 69 / 69 | 75 / 75 |
| `TRAINER_GRUNT_WEATHER_INST_2` | GRUNT | Protected | 59 / 59 | 30 / 30 | 70 / 70 | 76 / 76 |
| `TRAINER_GRUNT_WEATHER_INST_3` | GRUNT | Protected | 59 / 59 / 59 | 30 / 30 / 30 | 70 / 70 / 70 | 76 / 76 / 76 |
| `TRAINER_GRUNT_MUSEUM_1` | GRUNT | Protected | 40 | 20 | 48 | 52 |
| `TRAINER_GRUNT_MUSEUM_2` | GRUNT | Protected | 37 / 39 | 19 / 20 | 44 / 46 | 48 / 50 |
| `TRAINER_GRUNT_SPACE_CENTER_1` | GRUNT | Protected | 78 | 39 | 93 | 100 |
| `TRAINER_GRUNT_MT_PYRE_1` | GRUNT | Protected | 67 | 34 | 80 | 87 |
| `TRAINER_GRUNT_MT_PYRE_2` | GRUNT | Protected | 67 | 34 | 80 | 87 |
| `TRAINER_GRUNT_MT_PYRE_3` | GRUNT | Protected | 67 / 67 | 34 / 34 | 80 / 80 | 87 / 87 |
| `TRAINER_GRUNT_WEATHER_INST_4` | GRUNT | Protected | 61 | 31 | 73 | 79 |
| `TRAINER_GRUNT_AQUA_HIDEOUT_5` | GRUNT | Protected | 69 | 35 | 82 | 89 |
| `TRAINER_GRUNT_AQUA_HIDEOUT_6` | GRUNT | Protected | 69 | 35 | 82 | 89 |
| `TRAINER_FREDRICK` | FREDRICK | Ordinary | 67 / 67 | 34 / 34 | 80 / 80 | 87 / 87 |
| `TRAINER_MATT` | MATT | Protected | 69 / 69 | 35 / 35 | 82 / 82 | 89 / 89 |
| `TRAINER_ZANDER` | ZANDER | Ordinary | 67 | 34 | 80 | 87 |
| `TRAINER_SHELLY_WEATHER_INSTITUTE` | SHELLY | Protected | 61 / 61 | 31 / 31 | 73 / 73 | 79 / 79 |
| `TRAINER_SHELLY_SEAFLOOR_CAVERN` | SHELLY | Protected | 82 / 82 | 41 / 41 | 98 / 98 | 100 / 100 |
| `TRAINER_ARCHIE` | ARCHIE | Protected | 83 / 83 / 83 / 83 / 100 | 42 / 42 / 42 / 42 / 50 | 99 / 99 / 99 / 99 / 100 | 100 / 100 / 100 / 100 / 100 |
| `TRAINER_LEAH` | LEAH | Ordinary | 67 | 34 | 80 | 87 |
| `TRAINER_DAISY` | DAISY | Ordinary | 35 / 35 | 18 / 18 | 42 / 42 | 45 / 45 |
| `TRAINER_ROSE_1` | ROSE | Ordinary | 40 / 40 / 40 | 20 / 20 / 20 | 48 / 48 / 48 | 52 / 52 / 52 |
| `TRAINER_FELIX` | FELIX | Ordinary | 89 / 89 | 45 / 45 | 100 / 100 | 100 / 100 |
| `TRAINER_VIOLET` | VIOLET | Ordinary | 66 / 66 | 33 / 33 | 79 / 79 | 85 / 85 |
| `TRAINER_ROSE_2` | ROSE | Ordinary | 39 / 43 | 20 / 22 | 46 / 51 | 50 / 55 |
| `TRAINER_ROSE_3` | ROSE | Ordinary | 44 / 46 / 46 | 22 / 23 / 23 | 52 / 55 / 55 | 57 / 59 / 59 |
| `TRAINER_ROSE_4` | ROSE | Ordinary | 49 / 47 / 47 | 25 / 24 / 24 | 58 / 56 / 56 | 63 / 61 / 61 |
| `TRAINER_ROSE_5` | ROSE | Ordinary | 52 / 52 / 52 | 26 / 26 / 26 | 62 / 62 / 62 | 67 / 67 / 67 |
| `TRAINER_DUSTY_1` | DUSTY | Ordinary | 46 | 23 | 55 | 59 |
| `TRAINER_CHIP` | CHIP | Ordinary | 61 / 60 / 60 | 31 / 30 / 30 | 73 / 72 / 72 | 79 / 78 / 78 |
| `TRAINER_FOSTER` | FOSTER | Ordinary | 55 / 55 | 28 / 28 | 66 / 66 | 71 / 71 |
| `TRAINER_DUSTY_2` | DUSTY | Ordinary | 49 | 25 | 58 | 63 |
| `TRAINER_DUSTY_3` | DUSTY | Ordinary | 52 | 26 | 62 | 67 |
| `TRAINER_DUSTY_4` | DUSTY | Ordinary | 55 | 28 | 66 | 71 |
| `TRAINER_DUSTY_5` | DUSTY | Ordinary | 58 | 29 | 69 | 75 |
| `TRAINER_GABBY_AND_TY_1` | GABBY & TY | Protected | 30 / 25 | 15 / 13 | 36 / 30 | 39 / 32 |
| `TRAINER_GABBY_AND_TY_2` | GABBY & TY | Protected | 39 / 40 | 20 / 20 | 46 / 48 | 50 / 52 |
| `TRAINER_GABBY_AND_TY_3` | GABBY & TY | Protected | 39 / 40 | 20 / 20 | 46 / 48 | 50 / 52 |
| `TRAINER_GABBY_AND_TY_4` | GABBY & TY | Protected | 46 / 48 | 23 / 24 | 55 / 57 | 59 / 62 |
| `TRAINER_GABBY_AND_TY_5` | GABBY & TY | Protected | 50 / 54 | 25 / 27 | 60 / 64 | 65 / 70 |
| `TRAINER_GABBY_AND_TY_6` | GABBY & TY | Protected | 58 / 60 | 29 / 30 | 69 / 72 | 75 / 78 |
| `TRAINER_LOLA_1` | LOLA | Ordinary | 35 / 35 | 18 / 18 | 42 / 42 | 45 / 45 |
| `TRAINER_AUSTINA` | AUSTINA | Ordinary | 35 | 18 | 42 | 45 |
| `TRAINER_GWEN` | GWEN | Ordinary | 35 | 18 | 42 | 45 |
| `TRAINER_LOLA_2` | LOLA | Ordinary | 38 / 38 | 19 / 19 | 45 / 45 | 49 / 49 |
| `TRAINER_LOLA_3` | LOLA | Ordinary | 40 / 41 | 20 / 21 | 48 / 49 | 52 / 53 |
| `TRAINER_LOLA_4` | LOLA | Ordinary | 44 / 44 | 22 / 22 | 52 / 52 | 57 / 57 |
| `TRAINER_LOLA_5` | LOLA | Ordinary | 44 / 47 | 22 / 24 | 52 / 56 | 57 / 61 |
| `TRAINER_RICKY_1` | RICKY | Ordinary | 35 | 18 | 42 | 45 |
| `TRAINER_SIMON` | SIMON | Ordinary | 35 / 35 | 18 / 18 | 42 / 42 | 45 / 45 |
| `TRAINER_CHARLIE` | CHARLIE | Ordinary | 56 | 28 | 67 | 72 |
| `TRAINER_RICKY_2` | RICKY | Ordinary | 38 | 19 | 45 | 49 |
| `TRAINER_RICKY_3` | RICKY | Ordinary | 41 | 21 | 49 | 53 |
| `TRAINER_RICKY_4` | RICKY | Ordinary | 44 | 22 | 52 | 57 |
| `TRAINER_RICKY_5` | RICKY | Ordinary | 47 | 24 | 56 | 61 |
| `TRAINER_RANDALL` | RANDALL | Ordinary | 52 | 26 | 62 | 67 |
| `TRAINER_PARKER` | PARKER | Ordinary | 51 | 26 | 61 | 66 |
| `TRAINER_GEORGE` | GEORGE | Ordinary | 51 | 26 | 61 | 66 |
| `TRAINER_BERKE` | BERKE | Ordinary | 51 | 26 | 61 | 66 |
| `TRAINER_BRAXTON` | BRAXTON | Ordinary | 67 / 66 / 66 / 66 / 66 | 34 / 33 / 33 / 33 / 33 | 80 / 79 / 79 / 79 / 79 | 87 / 85 / 85 / 85 / 85 |
| `TRAINER_VINCENT` | VINCENT | Ordinary | 94 / 93 / 94 | 47 / 47 / 47 | 100 / 100 / 100 | 100 / 100 / 100 |
| `TRAINER_LEROY` | LEROY | Ordinary | 90 / 95 | 45 / 48 | 100 / 100 | 100 / 100 |
| `TRAINER_WILTON_1` | WILTON | Ordinary | 45 / 45 / 45 | 23 / 23 / 23 | 54 / 54 / 54 | 58 / 58 / 58 |
| `TRAINER_EDGAR` | EDGAR | Ordinary | 86 / 86 | 43 / 43 | 100 / 100 | 100 / 100 |
| `TRAINER_ALBERT` | ALBERT | Ordinary | 86 / 86 | 43 / 43 | 100 / 100 | 100 / 100 |
| `TRAINER_SAMUEL` | SAMUEL | Ordinary | 87 / 87 / 88 | 44 / 44 / 44 | 100 / 100 / 100 | 100 / 100 / 100 |
| `TRAINER_VITO` | VITO | Ordinary | 88 / 89 / 89 / 89 | 44 / 45 / 45 / 45 | 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 |
| `TRAINER_OWEN` | OWEN | Ordinary | 89 / 89 / 88 | 45 / 45 / 44 | 100 / 100 / 100 | 100 / 100 / 100 |
| `TRAINER_WILTON_2` | WILTON | Ordinary | 44 / 44 / 48 | 22 / 22 / 24 | 52 / 52 / 57 | 57 / 57 / 62 |
| `TRAINER_WILTON_3` | WILTON | Ordinary | 49 / 48 / 51 | 25 / 24 / 26 | 58 / 57 / 61 | 63 / 62 / 66 |
| `TRAINER_WILTON_4` | WILTON | Ordinary | 54 / 54 / 54 | 27 / 27 / 27 | 64 / 64 / 64 | 70 / 70 / 70 |
| `TRAINER_WILTON_5` | WILTON | Ordinary | 55 / 54 / 57 | 28 / 27 / 29 | 66 / 64 / 68 | 71 / 70 / 74 |
| `TRAINER_WARREN` | WARREN | Ordinary | 81 / 81 | 41 / 41 | 97 / 97 | 100 / 100 |
| `TRAINER_MARY` | MARY | Ordinary | 51 | 26 | 61 | 66 |
| `TRAINER_ALEXIA` | ALEXIA | Ordinary | 51 | 26 | 61 | 66 |
| `TRAINER_JODY` | JODY | Ordinary | 51 | 26 | 61 | 66 |
| `TRAINER_WENDY` | WENDY | Ordinary | 67 / 67 / 67 | 34 / 34 / 34 | 80 / 80 / 80 | 87 / 87 / 87 |
| `TRAINER_KEIRA` | KEIRA | Ordinary | 92 / 92 | 46 / 46 | 100 / 100 | 100 / 100 |
| `TRAINER_BROOKE_1` | BROOKE | Ordinary | 45 / 45 / 45 | 23 / 23 / 23 | 54 / 54 / 54 | 58 / 58 / 58 |
| `TRAINER_JENNIFER` | JENNIFER | Ordinary | 61 | 31 | 73 | 79 |
| `TRAINER_HOPE` | HOPE | Ordinary | 88 | 44 | 100 | 100 |
| `TRAINER_SHANNON` | SHANNON | Ordinary | 88 | 44 | 100 | 100 |
| `TRAINER_MICHELLE` | MICHELLE | Ordinary | 87 / 87 / 87 | 44 / 44 / 44 | 100 / 100 / 100 | 100 / 100 / 100 |
| `TRAINER_CAROLINE` | CAROLINE | Ordinary | 88 / 88 | 44 / 44 | 100 / 100 | 100 / 100 |
| `TRAINER_JULIE` | JULIE | Ordinary | 89 / 88 / 88 | 45 / 44 / 44 | 100 / 100 / 100 | 100 / 100 / 100 |
| `TRAINER_BROOKE_2` | BROOKE | Ordinary | 48 / 46 / 47 | 24 / 23 / 24 | 57 / 55 / 56 | 62 / 59 / 61 |
| `TRAINER_BROOKE_3` | BROOKE | Ordinary | 51 / 47 / 48 | 26 / 24 / 24 | 61 / 56 / 57 | 66 / 61 / 62 |
| `TRAINER_BROOKE_4` | BROOKE | Ordinary | 54 / 54 / 54 | 27 / 27 / 27 | 64 / 64 / 64 | 70 / 70 / 70 |
| `TRAINER_BROOKE_5` | BROOKE | Ordinary | 57 / 57 / 57 | 29 / 29 / 29 | 68 / 68 / 68 | 74 / 74 / 74 |
| `TRAINER_PATRICIA` | PATRICIA | Ordinary | 76 / 76 | 38 / 38 | 91 / 91 | 98 / 98 |
| `TRAINER_KINDRA` | KINDRA | Ordinary | 67 / 67 | 34 / 34 | 80 / 80 | 87 / 87 |
| `TRAINER_TAMMY` | TAMMY | Ordinary | 66 / 66 | 33 / 33 | 79 / 79 | 85 / 85 |
| `TRAINER_VALERIE_1` | VALERIE | Ordinary | 67 | 34 | 80 | 87 |
| `TRAINER_TASHA` | TASHA | Ordinary | 67 | 34 | 80 | 87 |
| `TRAINER_VALERIE_2` | VALERIE | Ordinary | 66 / 70 | 33 / 35 | 79 / 84 | 85 / 91 |
| `TRAINER_VALERIE_3` | VALERIE | Ordinary | 73 / 73 | 37 / 37 | 87 / 87 | 94 / 94 |
| `TRAINER_VALERIE_4` | VALERIE | Ordinary | 76 / 76 | 38 / 38 | 91 / 91 | 98 / 98 |
| `TRAINER_VALERIE_5` | VALERIE | Ordinary | 75 / 75 / 79 | 38 / 38 / 40 | 90 / 90 / 94 | 97 / 97 / 100 |
| `TRAINER_CINDY_1` | CINDY | Ordinary | 14 | 7 | 17 | 18 |
| `TRAINER_DAPHNE` | DAPHNE | Ordinary | 85 / 85 | 43 / 43 | 100 / 100 | 100 / 100 |
| `TRAINER_GRUNT_SPACE_CENTER_2` | GRUNT | Protected | 77 / 77 / 77 | 39 / 39 / 39 | 92 / 92 / 92 | 100 / 100 / 100 |
| `TRAINER_CINDY_2` | CINDY | Ordinary | 21 | 11 | 25 | 27 |
| `TRAINER_BRIANNA` | BRIANNA | Ordinary | 87 | 44 | 100 | 100 |
| `TRAINER_NAOMI` | NAOMI | Ordinary | 60 | 30 | 72 | 78 |
| `TRAINER_CINDY_3` | CINDY | Ordinary | 39 | 20 | 46 | 50 |
| `TRAINER_CINDY_4` | CINDY | Ordinary | 45 | 23 | 54 | 58 |
| `TRAINER_CINDY_5` | CINDY | Ordinary | 48 | 24 | 57 | 62 |
| `TRAINER_CINDY_6` | CINDY | Ordinary | 51 | 26 | 61 | 66 |
| `TRAINER_MELISSA` | MELISSA | Ordinary | 47 | 24 | 56 | 61 |
| `TRAINER_SHEILA` | SHEILA | Ordinary | 47 | 24 | 56 | 61 |
| `TRAINER_SHIRLEY` | SHIRLEY | Ordinary | 47 | 24 | 56 | 61 |
| `TRAINER_JESSICA_1` | JESSICA | Ordinary | 66 / 67 | 33 / 34 | 79 / 80 | 85 / 87 |
| `TRAINER_CONNIE` | CONNIE | Ordinary | 87 | 44 | 100 | 100 |
| `TRAINER_BRIDGET` | BRIDGET | Ordinary | 87 | 44 | 100 | 100 |
| `TRAINER_OLIVIA` | OLIVIA | Ordinary | 85 / 85 / 85 | 43 / 43 / 43 | 100 / 100 / 100 | 100 / 100 / 100 |
| `TRAINER_TIFFANY` | TIFFANY | Ordinary | 85 / 85 | 43 / 43 | 100 / 100 | 100 / 100 |
| `TRAINER_JESSICA_2` | JESSICA | Ordinary | 69 / 70 | 35 / 35 | 82 / 84 | 89 / 91 |
| `TRAINER_JESSICA_3` | JESSICA | Ordinary | 71 / 73 | 36 / 37 | 85 / 87 | 92 / 94 |
| `TRAINER_JESSICA_4` | JESSICA | Ordinary | 74 / 76 | 37 / 38 | 88 / 91 | 96 / 98 |
| `TRAINER_JESSICA_5` | JESSICA | Ordinary | 77 / 79 | 39 / 40 | 92 / 94 | 100 / 100 |
| `TRAINER_WINSTON_1` | WINSTON | Ordinary | 14 | 7 | 17 | 18 |
| `TRAINER_MOLLIE` | MOLLIE | Ordinary | 81 / 81 | 41 / 41 | 97 / 97 | 100 / 100 |
| `TRAINER_GARRET` | GARRET | Ordinary | 53 | 27 | 63 | 68 |
| `TRAINER_WINSTON_2` | WINSTON | Ordinary | 37 | 19 | 44 | 48 |
| `TRAINER_WINSTON_3` | WINSTON | Ordinary | 43 | 22 | 51 | 55 |
| `TRAINER_WINSTON_4` | WINSTON | Ordinary | 46 | 23 | 55 | 59 |
| `TRAINER_WINSTON_5` | WINSTON | Ordinary | 50 | 25 | 60 | 65 |
| `TRAINER_STEVE_1` | STEVE | Ordinary | 44 | 22 | 52 | 57 |
| `TRAINER_THALIA_1` | THALIA | Ordinary | 57 / 57 | 29 / 29 | 68 / 68 | 74 / 74 |
| `TRAINER_MARK` | MARK | Ordinary | 67 | 34 | 80 | 87 |
| `TRAINER_GRUNT_MT_CHIMNEY_1` | GRUNT | Protected | 47 | 24 | 56 | 61 |
| `TRAINER_STEVE_2` | STEVE | Ordinary | 47 | 24 | 56 | 61 |
| `TRAINER_STEVE_3` | STEVE | Ordinary | 50 / 49 | 25 / 25 | 60 / 58 | 65 / 63 |
| `TRAINER_STEVE_4` | STEVE | Ordinary | 53 / 52 | 27 / 26 | 63 / 62 | 68 / 67 |
| `TRAINER_STEVE_5` | STEVE | Ordinary | 56 / 56 | 28 / 28 | 67 / 67 | 72 / 72 |
| `TRAINER_LUIS` | LUIS | Ordinary | 56 | 28 | 67 | 72 |
| `TRAINER_DOMINIK` | DOMINIK | Ordinary | 55 | 28 | 66 | 71 |
| `TRAINER_DOUGLAS` | DOUGLAS | Ordinary | 55 / 55 | 28 / 28 | 66 / 66 | 71 / 71 |
| `TRAINER_DARRIN` | DARRIN | Ordinary | 55 / 55 / 55 | 28 / 28 / 28 | 66 / 66 / 66 | 71 / 71 / 71 |
| `TRAINER_TONY_1` | TONY | Ordinary | 55 | 28 | 66 | 71 |
| `TRAINER_JEROME` | JEROME | Ordinary | 56 | 28 | 67 | 72 |
| `TRAINER_MATTHEW` | MATTHEW | Ordinary | 55 | 28 | 66 | 71 |
| `TRAINER_DAVID` | DAVID | Ordinary | 55 / 55 | 28 / 28 | 66 / 66 | 71 / 71 |
| `TRAINER_SPENCER` | SPENCER | Ordinary | 70 / 69 | 35 / 35 | 84 / 82 | 91 / 89 |
| `TRAINER_ROLAND` | ROLAND | Ordinary | 70 | 35 | 84 | 91 |
| `TRAINER_NOLEN` | NOLEN | Ordinary | 70 | 35 | 84 | 91 |
| `TRAINER_STAN` | STAN | Ordinary | 69 / 70 | 35 / 35 | 82 / 84 | 89 / 91 |
| `TRAINER_BARRY` | BARRY | Ordinary | 73 | 37 | 87 | 94 |
| `TRAINER_DEAN` | DEAN | Ordinary | 72 / 72 / 72 | 36 / 36 / 36 | 86 / 86 / 86 | 93 / 93 / 93 |
| `TRAINER_RODNEY` | RODNEY | Ordinary | 81 | 41 | 97 | 100 |
| `TRAINER_RICHARD` | RICHARD | Ordinary | 81 | 41 | 97 | 100 |
| `TRAINER_HERMAN` | HERMAN | Ordinary | 81 / 81 | 41 / 41 | 97 / 97 | 100 / 100 |
| `TRAINER_SANTIAGO` | SANTIAGO | Ordinary | 80 / 81 | 40 / 41 | 96 / 97 | 100 / 100 |
| `TRAINER_GILBERT` | GILBERT | Ordinary | 82 | 41 | 98 | 100 |
| `TRAINER_FRANKLIN` | FRANKLIN | Ordinary | 84 | 42 | 100 | 100 |
| `TRAINER_KEVIN` | KEVIN | Ordinary | 82 | 41 | 98 | 100 |
| `TRAINER_JACK` | JACK | Ordinary | 83 | 42 | 99 | 100 |
| `TRAINER_DUDLEY` | DUDLEY | Ordinary | 45 / 47 / 42 | 23 / 24 / 21 | 54 / 56 / 50 | 58 / 61 / 54 |
| `TRAINER_CHAD` | CHAD | Ordinary | 71 / 70 | 36 / 35 | 85 / 84 | 92 / 91 |
| `TRAINER_TONY_2` | TONY | Ordinary | 58 | 29 | 69 | 75 |
| `TRAINER_TONY_3` | TONY | Ordinary | 61 | 31 | 73 | 79 |
| `TRAINER_TONY_4` | TONY | Ordinary | 64 / 64 | 32 / 32 | 76 / 76 | 83 / 83 |
| `TRAINER_TONY_5` | TONY | Ordinary | 70 / 70 | 35 / 35 | 84 / 84 | 91 / 91 |
| `TRAINER_TAKAO` | TAKAO | Ordinary | 32 | 16 | 38 | 41 |
| `TRAINER_HITOSHI` | HITOSHI | Ordinary | 83 / 83 | 42 / 42 | 99 / 99 | 100 / 100 |
| `TRAINER_KIYO` | KIYO | Ordinary | 82 | 41 | 98 | 100 |
| `TRAINER_KOICHI` | KOICHI | Ordinary | 61 / 58 | 31 / 29 | 73 / 69 | 79 / 75 |
| `TRAINER_NOB_1` | NOB | Ordinary | 48 | 24 | 57 | 62 |
| `TRAINER_NOB_2` | NOB | Ordinary | 51 | 26 | 61 | 66 |
| `TRAINER_NOB_3` | NOB | Ordinary | 54 / 54 | 27 / 27 | 64 / 64 | 70 / 70 |
| `TRAINER_NOB_4` | NOB | Ordinary | 60 / 60 / 60 | 30 / 30 / 30 | 72 / 72 / 72 | 78 / 78 / 78 |
| `TRAINER_NOB_5` | NOB | Ordinary | 63 / 63 / 63 / 63 | 32 / 32 / 32 / 32 | 75 / 75 / 75 / 75 | 81 / 81 / 81 / 81 |
| `TRAINER_YUJI` | YUJI | Ordinary | 53 / 53 | 27 / 27 | 63 / 63 | 68 / 68 |
| `TRAINER_DAISUKE` | Spro | Protected | 49 / 48 / 49 | 25 / 24 / 25 | 58 / 57 / 58 | 63 / 62 / 63 |
| `TRAINER_ATSUSHI` | ATSUSHI | Ordinary | 69 | 35 | 82 | 89 |
| `TRAINER_KIRK` | KIRK | Ordinary | 43 / 43 | 22 / 22 | 51 / 51 | 55 / 55 |
| `TRAINER_GRUNT_AQUA_HIDEOUT_7` | GRUNT | Protected | 70 / 71 | 35 / 36 | 84 / 85 | 91 / 92 |
| `TRAINER_GRUNT_AQUA_HIDEOUT_8` | GRUNT | Protected | 71 | 36 | 85 | 92 |
| `TRAINER_SHAWN` | SHAWN | Ordinary | 41 / 41 | 21 / 21 | 49 / 49 | 53 / 53 |
| `TRAINER_FERNANDO_1` | FERNANDO | Ordinary | 69 / 69 | 35 / 35 | 82 / 82 | 89 / 89 |
| `TRAINER_DALTON_1` | Queen | Protected | 44 / 43 | 22 / 22 | 52 / 51 | 57 / 55 |
| `TRAINER_DALTON_2` | Queen | Protected | 55 / 54 / 56 | 28 / 27 / 28 | 66 / 64 / 67 | 71 / 70 / 72 |
| `TRAINER_DALTON_3` | Queen | Protected | 65 / 63 / 64 / 64 | 33 / 32 / 32 / 32 | 78 / 75 / 76 / 76 | 84 / 81 / 83 / 83 |
| `TRAINER_DALTON_4` | Queen | Protected | 87 / 80 / 90 / 80 | 44 / 40 / 45 / 40 | 100 / 96 / 100 / 96 | 100 / 100 / 100 / 100 |
| `TRAINER_DALTON_5` | Queen | Protected | 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 |
| `TRAINER_COLE` | COLE | Ordinary | 47 | 24 | 56 | 61 |
| `TRAINER_JEFF` | JEFF | Ordinary | 47 / 47 | 24 / 24 | 56 / 56 | 61 / 61 |
| `TRAINER_AXLE` | AXLE | Ordinary | 47 | 24 | 56 | 61 |
| `TRAINER_JACE` | JACE | Ordinary | 46 | 23 | 55 | 59 |
| `TRAINER_KEEGAN` | KEEGAN | Ordinary | 47 | 24 | 56 | 61 |
| `TRAINER_BERNIE_1` | Sugimori | Protected | 45 / 45 | 23 / 23 | 54 / 54 | 58 / 58 |
| `TRAINER_BERNIE_2` | Sugimori | Protected | 50 / 50 | 25 / 25 | 60 / 60 | 65 / 65 |
| `TRAINER_BERNIE_3` | Sugimori | Protected | 60 / 60 | 30 / 30 | 72 / 72 | 78 / 78 |
| `TRAINER_BERNIE_4` | Sugimori | Protected | 70 / 70 | 35 / 35 | 84 / 84 | 91 / 91 |
| `TRAINER_BERNIE_5` | Sugimori | Protected | 100 / 100 | 50 / 50 | 100 / 100 | 100 / 100 |
| `TRAINER_DREW` | Manolete | Protected | 47 / 47 / 47 | 24 / 24 / 24 | 56 / 56 / 56 | 61 / 61 / 61 |
| `TRAINER_BEAU` | BEAU | Ordinary | 48 / 48 / 45 | 24 / 24 / 23 | 57 / 57 / 54 | 62 / 62 / 58 |
| `TRAINER_LARRY` | LARRY | Ordinary | 46 | 23 | 55 | 59 |
| `TRAINER_SHANE` | SHANE | Ordinary | 48 / 46 | 24 / 23 | 57 / 55 | 62 / 59 |
| `TRAINER_JUSTIN` | JUSTIN | Ordinary | 49 | 25 | 58 | 63 |
| `TRAINER_ETHAN_1` | ETHAN | Ordinary | 47 / 47 | 24 / 24 | 56 / 56 | 61 / 61 |
| `TRAINER_AUTUMN` | AUTUMN | Ordinary | 48 | 24 | 57 | 62 |
| `TRAINER_TRAVIS` | TRAVIS | Ordinary | 47 | 24 | 56 | 61 |
| `TRAINER_ETHAN_2` | ETHAN | Ordinary | 50 / 47 | 25 / 24 | 60 / 56 | 65 / 61 |
| `TRAINER_ETHAN_3` | ETHAN | Ordinary | 55 / 57 | 28 / 29 | 66 / 68 | 71 / 74 |
| `TRAINER_ETHAN_4` | ETHAN | Ordinary | 63 / 62 / 60 | 32 / 31 / 30 | 75 / 74 / 72 | 81 / 80 / 78 |
| `TRAINER_ETHAN_5` | ETHAN | Ordinary | 78 / 75 / 100 | 39 / 38 / 50 | 93 / 90 / 100 | 100 / 97 / 100 |
| `TRAINER_BRENT` | BRENT | Ordinary | 61 | 31 | 73 | 79 |
| `TRAINER_DONALD` | DONALD | Ordinary | 57 / 58 / 58 | 29 / 29 / 29 | 68 / 69 / 69 | 74 / 75 / 75 |
| `TRAINER_TAYLOR` | TAYLOR | Ordinary | 59 / 59 / 59 | 30 / 30 / 30 | 70 / 70 / 70 | 76 / 76 / 76 |
| `TRAINER_JEFFREY_1` | JEFFREY | Ordinary | 59 / 59 / 59 | 30 / 30 / 30 | 70 / 70 / 70 | 76 / 76 / 76 |
| `TRAINER_DEREK` | DEREK | Ordinary | 40 / 40 | 20 / 20 | 48 / 48 | 52 / 52 |
| `TRAINER_JEFFREY_2` | JEFFREY | Ordinary | 62 / 62 / 62 | 31 / 31 / 31 | 74 / 74 / 74 | 80 / 80 / 80 |
| `TRAINER_JEFFREY_3` | JEFFREY | Ordinary | 65 / 65 / 65 | 33 / 33 / 33 | 78 / 78 / 78 | 84 / 84 / 84 |
| `TRAINER_JEFFREY_4` | JEFFREY | Ordinary | 68 / 68 / 68 | 34 / 34 / 34 | 81 / 81 / 81 | 88 / 88 / 88 |
| `TRAINER_JEFFREY_5` | JEFFREY | Ordinary | 80 / 80 / 90 | 40 / 40 / 45 | 96 / 96 / 100 | 100 / 100 / 100 |
| `TRAINER_EDWARD` | EDWARD | Ordinary | 40 | 20 | 48 | 52 |
| `TRAINER_PRESTON` | PRESTON | Ordinary | 73 | 37 | 87 | 94 |
| `TRAINER_VIRGIL` | VIRGIL | Ordinary | 73 | 37 | 87 | 94 |
| `TRAINER_BLAKE` | BLAKE | Ordinary | 73 | 37 | 87 | 94 |
| `TRAINER_WILLIAM` | WILLIAM | Ordinary | 67 / 68 / 69 | 34 / 34 / 35 | 80 / 81 / 82 | 87 / 88 / 89 |
| `TRAINER_JOSHUA` | JOSHUA | Ordinary | 82 / 82 | 41 / 41 | 98 / 98 | 100 / 100 |
| `TRAINER_CAMERON_1` | CAMERON | Ordinary | 68 | 34 | 81 | 88 |
| `TRAINER_CAMERON_2` | CAMERON | Ordinary | 71 / 71 | 36 / 36 | 85 / 85 | 92 / 92 |
| `TRAINER_CAMERON_3` | CAMERON | Ordinary | 74 / 70 | 37 / 35 | 88 / 84 | 96 / 91 |
| `TRAINER_CAMERON_4` | CAMERON | Ordinary | 77 / 73 | 39 / 37 | 92 / 87 | 100 / 94 |
| `TRAINER_CAMERON_5` | CAMERON | Ordinary | 80 / 76 | 40 / 38 | 96 / 91 | 100 / 98 |
| `TRAINER_JACLYN` | JACLYN | Ordinary | 39 | 20 | 46 | 50 |
| `TRAINER_HANNAH` | HANNAH | Ordinary | 73 / 73 | 37 / 37 | 87 / 87 | 94 / 94 |
| `TRAINER_SAMANTHA` | SAMANTHA | Ordinary | 74 | 37 | 88 | 96 |
| `TRAINER_MAURA` | MAURA | Ordinary | 73 | 37 | 87 | 94 |
| `TRAINER_KAYLA` | KAYLA | Ordinary | 67 / 68 / 67 | 34 / 34 / 34 | 80 / 81 / 80 | 87 / 88 / 87 |
| `TRAINER_ALEXIS` | ALEXIS | Ordinary | 81 / 81 | 41 / 41 | 97 / 97 | 100 / 100 |
| `TRAINER_JACKI_1` | JACKI | Ordinary | 68 / 68 | 34 / 34 | 81 / 81 | 88 / 88 |
| `TRAINER_JACKI_2` | JACKI | Ordinary | 71 / 71 | 36 / 36 | 85 / 85 | 92 / 92 |
| `TRAINER_JACKI_3` | JACKI | Ordinary | 74 / 74 | 37 / 37 | 88 / 88 | 96 / 96 |
| `TRAINER_JACKI_4` | JACKI | Ordinary | 85 / 85 | 43 / 43 | 100 / 100 | 100 / 100 |
| `TRAINER_JACKI_5` | JACKI | Ordinary | 100 / 100 | 50 / 50 | 100 / 100 | 100 / 100 |
| `TRAINER_WALTER_1` | WALTER | Ordinary | 66 | 33 | 79 | 85 |
| `TRAINER_MICAH` | MICAH | Ordinary | 60 / 65 | 30 / 33 | 72 / 78 | 78 / 84 |
| `TRAINER_THOMAS` | Flo | Protected | 50 / 50 / 50 / 50 / 50 / 50 | 25 / 25 / 25 / 25 / 25 / 25 | 60 / 60 / 60 / 60 / 60 / 60 | 65 / 65 / 65 / 65 / 65 / 65 |
| `TRAINER_WALTER_2` | WALTER | Ordinary | 69 | 35 | 82 | 89 |
| `TRAINER_WALTER_3` | WALTER | Ordinary | 68 / 72 | 34 / 36 | 81 / 86 | 88 / 93 |
| `TRAINER_WALTER_4` | WALTER | Ordinary | 80 / 80 | 40 / 40 | 96 / 96 | 100 / 100 |
| `TRAINER_WALTER_5` | WALTER | Ordinary | 100 / 100 / 100 | 50 / 50 / 50 | 100 / 100 / 100 | 100 / 100 / 100 |
| `TRAINER_SIDNEY` | SIDNEY | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_PHOEBE` | PHOEBE | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_GLACIA` | GLACIA | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_DRAKE` | DRAKE | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_ROXANNE_1` | ROXANNE | Protected | 25 / 25 / 25 / 25 | 13 / 13 / 13 / 13 | 30 / 30 / 30 / 30 | 32 / 32 / 32 / 32 |
| `TRAINER_BRAWLY_1` | BRAWLY | Protected | 35 / 35 / 35 / 35 | 18 / 18 / 18 / 18 | 42 / 42 / 42 / 42 | 45 / 45 / 45 / 45 |
| `TRAINER_WATTSON_1` | WATTSON | Protected | 45 / 45 / 45 / 45 | 23 / 23 / 23 / 23 | 54 / 54 / 54 / 54 | 58 / 58 / 58 / 58 |
| `TRAINER_FLANNERY_1` | FLANNERY | Protected | 46 / 46 / 46 / 46 / 50 | 23 / 23 / 23 / 23 / 25 | 55 / 55 / 55 / 55 / 60 | 59 / 59 / 59 / 59 / 65 |
| `TRAINER_NORMAN_1` | NORMAN | Protected | 51 / 51 / 51 / 55 | 26 / 26 / 26 / 28 | 61 / 61 / 61 / 66 | 66 / 66 / 66 / 71 |
| `TRAINER_WINONA_1` | WINONA | Protected | 61 / 61 / 61 / 61 / 65 | 31 / 31 / 31 / 31 / 33 | 73 / 73 / 73 / 73 / 78 | 79 / 79 / 79 / 79 / 84 |
| `TRAINER_TATE_AND_LIZA_1` | TATE&LIZA | Protected | 71 / 71 / 71 / 71 / 75 | 36 / 36 / 36 / 36 / 38 | 85 / 85 / 85 / 85 / 90 | 92 / 92 / 92 / 92 / 97 |
| `TRAINER_JUAN_1` | JUAN | Protected | 86 / 86 / 86 / 86 / 86 / 90 | 43 / 43 / 43 / 43 / 43 / 45 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_JERRY_1` | JERRY | Ordinary | 20 | 10 | 24 | 26 |
| `TRAINER_TED` | TED | Ordinary | 44 | 22 | 52 | 57 |
| `TRAINER_PAUL` | PAUL | Ordinary | 44 / 44 / 44 | 22 / 22 / 22 | 52 / 52 / 52 | 57 / 57 / 57 |
| `TRAINER_JERRY_2` | JERRY | Ordinary | 40 / 40 | 20 / 20 | 48 / 48 | 52 / 52 |
| `TRAINER_JERRY_3` | JERRY | Ordinary | 50 / 50 | 25 / 25 | 60 / 60 | 65 / 65 |
| `TRAINER_JERRY_4` | JERRY | Ordinary | 60 / 60 | 30 / 30 | 72 / 72 | 78 / 78 |
| `TRAINER_JERRY_5` | JERRY | Ordinary | 70 / 70 / 80 | 35 / 35 / 40 | 84 / 84 / 96 | 91 / 91 / 100 |
| `TRAINER_KAREN_1` | KAREN | Ordinary | 20 | 10 | 24 | 26 |
| `TRAINER_GEORGIA` | GEORGIA | Ordinary | 42 / 42 | 21 / 21 | 50 / 50 | 54 / 54 |
| `TRAINER_KAREN_2` | KAREN | Ordinary | 40 / 40 | 20 / 20 | 48 / 48 | 52 / 52 |
| `TRAINER_KAREN_3` | KAREN | Ordinary | 50 / 50 | 25 / 25 | 60 / 60 | 65 / 65 |
| `TRAINER_KAREN_4` | KAREN | Ordinary | 60 / 60 | 30 / 30 | 72 / 72 | 78 / 78 |
| `TRAINER_KAREN_5` | KAREN | Ordinary | 70 / 70 | 35 / 35 | 84 / 84 | 91 / 91 |
| `TRAINER_KATE_AND_JOY` | KATE & JOY | Ordinary | 68 / 68 | 34 / 34 | 81 / 81 | 88 / 88 |
| `TRAINER_ANNA_AND_MEG_1` | ANNA & MEG | Ordinary | 40 / 40 | 20 / 20 | 48 / 48 | 52 / 52 |
| `TRAINER_ANNA_AND_MEG_2` | ANNA & MEG | Ordinary | 43 / 43 | 22 / 22 | 51 / 51 | 55 / 55 |
| `TRAINER_ANNA_AND_MEG_3` | ANNA & MEG | Ordinary | 46 / 46 | 23 / 23 | 55 / 55 | 59 / 59 |
| `TRAINER_ANNA_AND_MEG_4` | ANNA & MEG | Ordinary | 55 / 55 | 28 / 28 | 66 / 66 | 71 / 71 |
| `TRAINER_ANNA_AND_MEG_5` | ANNA & MEG | Ordinary | 70 / 70 | 35 / 35 | 84 / 84 | 91 / 91 |
| `TRAINER_VICTOR` | VICTOR | Protected | 45 / 45 | 23 / 23 | 54 / 54 | 58 / 58 |
| `TRAINER_MIGUEL_1` | MIGUEL | Ordinary | 40 | 20 | 48 | 52 |
| `TRAINER_COLTON` | COLTON | Ordinary | 40 / 55 / 65 / 70 / 80 / 90 | 20 / 28 / 33 / 35 / 40 / 45 | 48 / 66 / 78 / 84 / 96 / 100 | 52 / 71 / 84 / 91 / 100 / 100 |
| `TRAINER_MIGUEL_2` | MIGUEL | Ordinary | 60 | 30 | 72 | 78 |
| `TRAINER_MIGUEL_3` | MIGUEL | Ordinary | 70 | 35 | 84 | 91 |
| `TRAINER_MIGUEL_4` | MIGUEL | Ordinary | 80 | 40 | 96 | 100 |
| `TRAINER_MIGUEL_5` | MIGUEL | Ordinary | 100 | 50 | 100 | 100 |
| `TRAINER_VICTORIA` | VICTORIA | Protected | 45 | 23 | 54 | 58 |
| `TRAINER_VANESSA` | VANESSA | Ordinary | 69 | 35 | 82 | 89 |
| `TRAINER_BETHANY` | BETHANY | Ordinary | 88 / 88 / 88 | 44 / 44 / 44 | 100 / 100 / 100 | 100 / 100 / 100 |
| `TRAINER_ISABEL_1` | ISABEL | Ordinary | 39 / 39 | 20 / 20 | 46 / 46 | 50 / 50 |
| `TRAINER_ISABEL_2` | ISABEL | Ordinary | 45 / 45 | 23 / 23 | 54 / 54 | 58 / 58 |
| `TRAINER_ISABEL_3` | ISABEL | Ordinary | 60 / 60 | 30 / 30 | 72 / 72 | 78 / 78 |
| `TRAINER_ISABEL_4` | ISABEL | Ordinary | 70 / 70 | 35 / 35 | 84 / 84 | 91 / 91 |
| `TRAINER_ISABEL_5` | ISABEL | Ordinary | 90 / 90 | 45 / 45 | 100 / 100 | 100 / 100 |
| `TRAINER_TIMOTHY_1` | TIMOTHY | Ordinary | 61 | 31 | 73 | 79 |
| `TRAINER_TIMOTHY_2` | TIMOTHY | Ordinary | 64 | 32 | 76 | 83 |
| `TRAINER_TIMOTHY_3` | TIMOTHY | Ordinary | 67 | 34 | 80 | 87 |
| `TRAINER_TIMOTHY_4` | TIMOTHY | Ordinary | 70 | 35 | 84 | 91 |
| `TRAINER_TIMOTHY_5` | TIMOTHY | Ordinary | 100 | 50 | 100 | 100 |
| `TRAINER_VICKY` | VICKY | Protected | 46 | 23 | 55 | 59 |
| `TRAINER_SHELBY_1` | SHELBY | Ordinary | 48 / 48 | 24 / 24 | 57 / 57 | 62 / 62 |
| `TRAINER_SHELBY_2` | SHELBY | Ordinary | 55 / 55 | 28 / 28 | 66 / 66 | 71 / 71 |
| `TRAINER_SHELBY_3` | SHELBY | Ordinary | 65 / 65 | 33 / 33 | 78 / 78 | 84 / 84 |
| `TRAINER_SHELBY_4` | SHELBY | Ordinary | 80 / 80 | 40 / 40 | 96 / 96 | 100 / 100 |
| `TRAINER_SHELBY_5` | SHELBY | Ordinary | 100 / 100 | 50 / 50 | 100 / 100 | 100 / 100 |
| `TRAINER_CALVIN_1` | CALVIN | Ordinary | 10 | 5 | 13 | 14 |
| `TRAINER_BILLY` | BILLY | Ordinary | 17 / 16 | 9 / 8 | 20 / 19 | 22 / 20 |
| `TRAINER_JOSH` | JOSH | Ordinary | 23 | 12 | 27 | 29 |
| `TRAINER_TOMMY` | TOMMY | Ordinary | 24 / 24 | 12 / 12 | 28 / 28 | 31 / 31 |
| `TRAINER_JOEY` | JOEY | Protected | 23 | 12 | 27 | 29 |
| `TRAINER_BEN` | BEN | Ordinary | 42 / 44 | 21 / 22 | 50 / 52 | 54 / 57 |
| `TRAINER_QUINCY` | QUINCY | Ordinary | 88 / 88 | 44 / 44 | 100 / 100 | 100 / 100 |
| `TRAINER_KATELYNN` | KATELYNN | Ordinary | 90 / 88 | 45 / 44 | 100 / 100 | 100 / 100 |
| `TRAINER_JAYLEN` | JAYLEN | Ordinary | 44 | 22 | 52 | 57 |
| `TRAINER_DILLON` | DILLON | Ordinary | 44 | 22 | 52 | 57 |
| `TRAINER_CALVIN_2` | CALVIN | Ordinary | 50 | 25 | 60 | 65 |
| `TRAINER_CALVIN_3` | CALVIN | Ordinary | 50 / 55 | 25 / 28 | 60 / 66 | 65 / 71 |
| `TRAINER_CALVIN_4` | CALVIN | Ordinary | 50 / 55 / 60 | 25 / 28 / 30 | 60 / 66 / 72 | 65 / 71 / 78 |
| `TRAINER_CALVIN_5` | CALVIN | Ordinary | 100 / 100 / 100 | 50 / 50 / 50 | 100 / 100 / 100 | 100 / 100 / 100 |
| `TRAINER_EDDIE` | EDDIE | Ordinary | 41 / 41 | 21 / 21 | 49 / 49 | 53 / 53 |
| `TRAINER_ALLEN` | ALLEN | Ordinary | 10 / 10 | 5 / 5 | 13 / 13 | 14 / 14 |
| `TRAINER_TIMMY` | TIMMY | Ordinary | 39 / 40 | 20 / 20 | 46 / 48 | 50 / 52 |
| `TRAINER_WALLACE` | WALLACE | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_ANDREW` | ANDREW | Ordinary | 36 / 37 / 38 | 18 / 19 / 19 | 43 / 44 / 45 | 46 / 48 / 49 |
| `TRAINER_IVAN` | IVAN | Ordinary | 17 / 18 / 17 | 9 / 9 / 9 | 20 / 21 / 20 | 22 / 23 / 22 |
| `TRAINER_CLAUDE` | CLAUDE | Ordinary | 44 / 45 / 45 | 22 / 23 / 23 | 52 / 54 / 54 | 57 / 58 / 58 |
| `TRAINER_ELLIOT_1` | ELLIOT | Ordinary | 28 / 28 / 29 | 14 / 14 / 15 | 33 / 33 / 34 | 36 / 36 / 37 |
| `TRAINER_NED` | NED | Ordinary | 33 | 17 | 39 | 42 |
| `TRAINER_DALE` | DALE | Ordinary | 39 / 39 / 40 / 41 | 20 / 20 / 20 / 21 | 46 / 46 / 48 / 49 | 50 / 50 / 52 / 53 |
| `TRAINER_NOLAN` | NOLAN | Ordinary | 45 | 23 | 54 | 58 |
| `TRAINER_BARNY` | BARNY | Ordinary | 58 / 60 | 29 / 30 | 69 / 72 | 75 / 78 |
| `TRAINER_WADE` | WADE | Ordinary | 43 | 22 | 51 | 55 |
| `TRAINER_CARTER` | CARTER | Ordinary | 37 / 38 | 19 / 19 | 44 / 45 | 48 / 49 |
| `TRAINER_ELLIOT_2` | ELLIOT | Ordinary | 55 / 47 / 50 | 28 / 24 / 25 | 66 / 56 / 60 | 71 / 61 / 65 |
| `TRAINER_ELLIOT_3` | ELLIOT | Ordinary | 55 / 47 / 50 / 60 | 28 / 24 / 25 / 30 | 66 / 56 / 60 / 72 | 71 / 61 / 65 / 78 |
| `TRAINER_ELLIOT_4` | ELLIOT | Ordinary | 60 / 56 / 59 / 68 | 30 / 28 / 30 / 34 | 72 / 67 / 70 / 81 | 78 / 72 / 76 / 88 |
| `TRAINER_ELLIOT_5` | ELLIOT | Ordinary | 70 / 67 / 75 / 78 | 35 / 34 / 38 / 39 | 84 / 80 / 90 / 93 | 91 / 87 / 97 / 100 |
| `TRAINER_RONALD` | RONALD | Ordinary | 80 / 81 / 81 / 82 / 82 / 85 | 40 / 41 / 41 / 41 / 41 / 43 | 96 / 97 / 97 / 98 / 98 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_JACOB` | JACOB | Ordinary | 36 / 36 / 39 | 18 / 18 / 20 | 43 / 43 / 46 | 46 / 46 / 50 |
| `TRAINER_ANTHONY` | ANTHONY | Ordinary | 39 / 41 | 20 / 21 | 46 / 49 | 50 / 53 |
| `TRAINER_BENJAMIN_1` | BENJAMIN | Ordinary | 39 | 20 | 46 | 50 |
| `TRAINER_BENJAMIN_2` | BENJAMIN | Ordinary | 50 | 25 | 60 | 65 |
| `TRAINER_BENJAMIN_3` | BENJAMIN | Ordinary | 60 | 30 | 72 | 78 |
| `TRAINER_BENJAMIN_4` | BENJAMIN | Ordinary | 70 | 35 | 84 | 91 |
| `TRAINER_BENJAMIN_5` | BENJAMIN | Ordinary | 100 | 50 | 100 | 100 |
| `TRAINER_ABIGAIL_1` | ABIGAIL | Ordinary | 39 | 20 | 46 | 50 |
| `TRAINER_JASMINE` | JASMINE | Ordinary | 39 / 37 / 41 | 20 / 19 / 21 | 46 / 44 / 49 | 50 / 48 / 53 |
| `TRAINER_ABIGAIL_2` | ABIGAIL | Ordinary | 50 | 25 | 60 | 65 |
| `TRAINER_ABIGAIL_3` | ABIGAIL | Ordinary | 60 | 30 | 72 | 78 |
| `TRAINER_ABIGAIL_4` | ABIGAIL | Ordinary | 70 | 35 | 84 | 91 |
| `TRAINER_ABIGAIL_5` | ABIGAIL | Ordinary | 100 | 50 | 100 | 100 |
| `TRAINER_DYLAN_1` | DYLAN | Ordinary | 41 | 21 | 49 | 53 |
| `TRAINER_DYLAN_2` | DYLAN | Ordinary | 50 | 25 | 60 | 65 |
| `TRAINER_DYLAN_3` | DYLAN | Ordinary | 60 | 30 | 72 | 78 |
| `TRAINER_DYLAN_4` | DYLAN | Ordinary | 70 | 35 | 84 | 91 |
| `TRAINER_DYLAN_5` | DYLAN | Ordinary | 100 | 50 | 100 | 100 |
| `TRAINER_MARIA_1` | MARIA | Ordinary | 43 | 22 | 51 | 55 |
| `TRAINER_MARIA_2` | MARIA | Ordinary | 60 | 30 | 72 | 78 |
| `TRAINER_MARIA_3` | MARIA | Ordinary | 70 | 35 | 84 | 91 |
| `TRAINER_MARIA_4` | MARIA | Ordinary | 80 | 40 | 96 | 100 |
| `TRAINER_MARIA_5` | MARIA | Ordinary | 100 | 50 | 100 | 100 |
| `TRAINER_CAMDEN` | CAMDEN | Ordinary | 79 / 80 | 40 / 40 | 94 / 96 | 100 / 100 |
| `TRAINER_DEMETRIUS` | DEMETRIUS | Ordinary | 58 / 59 | 29 / 30 | 69 / 70 | 75 / 76 |
| `TRAINER_ISAIAH_1` | ISAIAH | Ordinary | 83 | 42 | 99 | 100 |
| `TRAINER_PABLO_1` | PABLO | Ordinary | 74 / 73 | 37 / 37 | 88 / 87 | 96 / 94 |
| `TRAINER_CHASE` | CHASE | Ordinary | 79 / 80 | 40 / 40 | 94 / 96 | 100 / 100 |
| `TRAINER_ISAIAH_2` | ISAIAH | Ordinary | 86 | 43 | 100 | 100 |
| `TRAINER_ISAIAH_3` | ISAIAH | Ordinary | 89 | 45 | 100 | 100 |
| `TRAINER_ISAIAH_4` | ISAIAH | Ordinary | 92 | 46 | 100 | 100 |
| `TRAINER_ISAIAH_5` | ISAIAH | Ordinary | 100 | 50 | 100 | 100 |
| `TRAINER_ISOBEL` | ISOBEL | Ordinary | 76 | 38 | 91 | 98 |
| `TRAINER_DONNY` | DONNY | Ordinary | 78 / 79 | 39 / 40 | 93 / 94 | 100 / 100 |
| `TRAINER_TALIA` | TALIA | Ordinary | 82 | 41 | 98 | 100 |
| `TRAINER_KATELYN_1` | KATELYN | Ordinary | 83 | 42 | 99 | 100 |
| `TRAINER_ALLISON` | ALLISON | Ordinary | 79 / 84 | 40 / 42 | 94 / 100 | 100 / 100 |
| `TRAINER_KATELYN_2` | KATELYN | Ordinary | 86 | 43 | 100 | 100 |
| `TRAINER_KATELYN_3` | KATELYN | Ordinary | 89 | 45 | 100 | 100 |
| `TRAINER_KATELYN_4` | KATELYN | Ordinary | 92 | 46 | 100 | 100 |
| `TRAINER_KATELYN_5` | KATELYN | Ordinary | 100 | 50 | 100 | 100 |
| `TRAINER_NICOLAS_1` | NICOLAS | Ordinary | 84 / 82 | 42 / 41 | 100 / 98 | 100 / 100 |
| `TRAINER_NICOLAS_2` | NICOLAS | Ordinary | 87 / 86 | 44 / 43 | 100 / 100 | 100 / 100 |
| `TRAINER_NICOLAS_3` | NICOLAS | Ordinary | 90 / 88 | 45 / 44 | 100 / 100 | 100 / 100 |
| `TRAINER_NICOLAS_4` | NICOLAS | Ordinary | 89 / 93 / 93 | 45 / 47 / 47 | 100 / 100 / 100 | 100 / 100 / 100 |
| `TRAINER_NICOLAS_5` | NICOLAS | Ordinary | 100 / 100 / 100 | 50 / 50 / 50 | 100 / 100 / 100 | 100 / 100 / 100 |
| `TRAINER_AARON` | AARON | Ordinary | 84 | 42 | 100 | 100 |
| `TRAINER_PERRY` | PERRY | Ordinary | 58 | 29 | 69 | 75 |
| `TRAINER_HUGH` | HUGH | Ordinary | 59 / 60 | 30 / 30 | 70 / 72 | 76 / 78 |
| `TRAINER_PHIL` | PHIL | Ordinary | 59 | 30 | 70 | 76 |
| `TRAINER_JARED` | JARED | Ordinary | 62 / 61 / 63 | 31 / 31 / 32 | 74 / 73 / 75 | 80 / 79 / 81 |
| `TRAINER_HUMBERTO` | HUMBERTO | Ordinary | 64 | 32 | 76 | 83 |
| `TRAINER_PRESLEY` | Elvis | Protected | 70 / 70 / 73 / 70 | 35 / 35 / 37 / 35 | 84 / 84 / 87 / 84 | 91 / 91 / 94 / 91 |
| `TRAINER_EDWARDO` | EDWARDO | Ordinary | 62 / 63 | 31 / 32 | 74 / 75 | 80 / 81 |
| `TRAINER_COLIN` | COLIN | Ordinary | 63 / 62 | 32 / 31 | 75 / 74 | 81 / 80 |
| `TRAINER_ROBERT_1` | ROBERT | Ordinary | 62 | 31 | 74 | 80 |
| `TRAINER_BENNY` | BENNY | Ordinary | 69 / 69 / 70 | 35 / 35 / 35 | 82 / 82 / 84 | 89 / 89 / 91 |
| `TRAINER_CHESTER` | CHESTER | Ordinary | 60 / 56 | 30 / 28 | 72 / 67 | 78 / 72 |
| `TRAINER_ROBERT_2` | ROBERT | Ordinary | 65 / 65 | 33 / 33 | 78 / 78 | 84 / 84 |
| `TRAINER_ROBERT_3` | ROBERT | Ordinary | 70 / 70 | 35 / 35 | 84 / 84 | 91 / 91 |
| `TRAINER_ROBERT_4` | ROBERT | Ordinary | 80 / 80 | 40 / 40 | 96 / 96 | 100 / 100 |
| `TRAINER_ROBERT_5` | ROBERT | Ordinary | 100 / 100 | 50 / 50 | 100 / 100 | 100 / 100 |
| `TRAINER_ALEX` | ALEX | Ordinary | 83 / 83 | 42 / 42 | 99 / 99 | 100 / 100 |
| `TRAINER_BECK` | BECK | Ordinary | 84 | 42 | 100 | 100 |
| `TRAINER_YASU` | YASU | Ordinary | 59 | 30 | 70 | 76 |
| `TRAINER_TAKASHI` | TAKASHI | Ordinary | 59 / 59 | 30 / 30 | 70 / 70 | 76 / 76 |
| `TRAINER_DIANNE` | DIANNE | Ordinary | 92 / 92 | 46 / 46 | 100 / 100 | 100 / 100 |
| `TRAINER_JANI` | JANI | Ordinary | 58 | 29 | 69 | 75 |
| `TRAINER_LAO_1` | LAO | Ordinary | 43 / 43 / 45 | 22 / 22 / 23 | 51 / 51 / 54 | 55 / 55 / 58 |
| `TRAINER_LUNG` | LUNG | Ordinary | 45 / 47 | 23 / 24 | 54 / 56 | 58 / 61 |
| `TRAINER_LAO_2` | LAO | Ordinary | 60 / 60 / 60 | 30 / 30 / 30 | 72 / 72 / 72 | 78 / 78 / 78 |
| `TRAINER_LAO_3` | LAO | Ordinary | 70 / 70 / 70 / 70 | 35 / 35 / 35 / 35 | 84 / 84 / 84 / 84 | 91 / 91 / 91 / 91 |
| `TRAINER_LAO_4` | LAO | Ordinary | 80 / 80 / 80 / 80 | 40 / 40 / 40 / 40 | 96 / 96 / 96 / 96 | 100 / 100 / 100 / 100 |
| `TRAINER_LAO_5` | LAO | Ordinary | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_JOCELYN` | JOCELYN | Ordinary | 30 | 15 | 36 | 39 |
| `TRAINER_LAURA` | LAURA | Ordinary | 34 | 17 | 40 | 44 |
| `TRAINER_CYNDY_1` | CYNDY | Ordinary | 48 / 48 | 24 / 24 | 57 / 57 | 62 / 62 |
| `TRAINER_CORA` | CORA | Ordinary | 54 | 27 | 64 | 70 |
| `TRAINER_PAULA` | PAULA | Ordinary | 53 | 27 | 63 | 68 |
| `TRAINER_CYNDY_2` | CYNDY | Ordinary | 55 / 55 | 28 / 28 | 66 / 66 | 71 / 71 |
| `TRAINER_CYNDY_3` | CYNDY | Ordinary | 65 / 65 | 33 / 33 | 78 / 78 | 84 / 84 |
| `TRAINER_CYNDY_4` | CYNDY | Ordinary | 80 / 80 | 40 / 40 | 96 / 96 | 100 / 100 |
| `TRAINER_CYNDY_5` | CYNDY | Ordinary | 100 / 100 / 100 | 50 / 50 / 50 | 100 / 100 / 100 | 100 / 100 / 100 |
| `TRAINER_MADELINE_1` | MADELINE | Ordinary | 44 | 22 | 52 | 57 |
| `TRAINER_CLARISSA` | CLARISSA | Ordinary | 62 / 62 | 31 / 31 | 74 / 74 | 80 / 80 |
| `TRAINER_ANGELICA` | ANGELICA | Ordinary | 62 | 31 | 74 | 80 |
| `TRAINER_MADELINE_2` | MADELINE | Ordinary | 55 | 28 | 66 | 71 |
| `TRAINER_MADELINE_3` | MADELINE | Ordinary | 60 | 30 | 72 | 78 |
| `TRAINER_MADELINE_4` | MADELINE | Ordinary | 70 / 70 | 35 / 35 | 84 / 84 | 91 / 91 |
| `TRAINER_MADELINE_5` | MADELINE | Ordinary | 100 / 100 | 50 / 50 | 100 / 100 | 100 / 100 |
| `TRAINER_BEVERLY` | BEVERLY | Ordinary | 58 / 58 | 29 / 29 | 69 / 69 | 75 / 75 |
| `TRAINER_IMANI` | IMANI | Ordinary | 58 | 29 | 69 | 75 |
| `TRAINER_KYLA` | KYLA | Ordinary | 58 | 29 | 69 | 75 |
| `TRAINER_DENISE` | DENISE | Ordinary | 57 / 60 | 29 / 30 | 68 / 72 | 74 / 78 |
| `TRAINER_BETH` | BETH | Ordinary | 58 | 29 | 69 | 75 |
| `TRAINER_TARA` | TARA | Ordinary | 58 / 58 | 29 / 29 | 69 / 69 | 75 / 75 |
| `TRAINER_MISSY` | MISSY | Ordinary | 56 | 28 | 67 | 72 |
| `TRAINER_ALICE` | ALICE | Ordinary | 55 / 55 / 55 | 28 / 28 / 28 | 66 / 66 / 66 | 71 / 71 / 71 |
| `TRAINER_JENNY_1` | JENNY | Ordinary | 72 | 36 | 86 | 93 |
| `TRAINER_GRACE` | GRACE | Ordinary | 71 | 36 | 85 | 92 |
| `TRAINER_TANYA` | TANYA | Ordinary | 70 | 35 | 84 | 91 |
| `TRAINER_SHARON` | SHARON | Ordinary | 70 | 35 | 84 | 91 |
| `TRAINER_NIKKI` | NIKKI | Ordinary | 74 / 73 | 37 / 37 | 88 / 87 | 96 / 94 |
| `TRAINER_BRENDA` | BRENDA | Ordinary | 74 | 37 | 88 | 96 |
| `TRAINER_KATIE` | KATIE | Ordinary | 81 / 84 | 41 / 42 | 97 / 100 | 100 / 100 |
| `TRAINER_SUSIE` | SUSIE | Ordinary | 82 | 41 | 98 | 100 |
| `TRAINER_KARA` | KARA | Ordinary | 84 | 42 | 100 | 100 |
| `TRAINER_DANA` | DANA | Ordinary | 81 | 41 | 97 | 100 |
| `TRAINER_SIENNA` | SIENNA | Ordinary | 74 / 74 | 37 / 37 | 88 / 88 | 96 / 96 |
| `TRAINER_DEBRA` | DEBRA | Ordinary | 86 | 43 | 100 | 100 |
| `TRAINER_LINDA` | LINDA | Ordinary | 85 / 84 | 43 / 42 | 100 / 100 | 100 / 100 |
| `TRAINER_KAYLEE` | KAYLEE | Ordinary | 50 / 48 | 25 / 24 | 60 / 57 | 65 / 62 |
| `TRAINER_LAUREL` | LAUREL | Ordinary | 82 / 83 | 41 / 42 | 98 / 99 | 100 / 100 |
| `TRAINER_CARLEE` | CARLEE | Ordinary | 83 | 42 | 99 | 100 |
| `TRAINER_JENNY_2` | JENNY | Ordinary | 75 | 38 | 90 | 97 |
| `TRAINER_JENNY_3` | JENNY | Ordinary | 78 | 39 | 93 | 100 |
| `TRAINER_JENNY_4` | JENNY | Ordinary | 81 / 81 | 41 / 41 | 97 / 97 | 100 / 100 |
| `TRAINER_JENNY_5` | JENNY | Ordinary | 100 / 100 / 100 | 50 / 50 / 50 | 100 / 100 / 100 | 100 / 100 / 100 |
| `TRAINER_HEIDI` | HEIDI | Ordinary | 47 / 48 | 24 / 24 | 56 / 57 | 61 / 62 |
| `TRAINER_BECKY` | BECKY | Ordinary | 47 / 47 | 24 / 24 | 56 / 56 | 61 / 61 |
| `TRAINER_CAROL` | CAROL | Ordinary | 43 / 44 | 22 / 22 | 51 / 52 | 55 / 57 |
| `TRAINER_NANCY` | NANCY | Ordinary | 44 / 44 | 22 / 22 | 52 / 52 | 57 / 57 |
| `TRAINER_MARTHA` | MARTHA | Ordinary | 47 / 48 | 24 / 24 | 56 / 57 | 61 / 62 |
| `TRAINER_DIANA_1` | DIANA | Ordinary | 47 / 46 / 47 | 24 / 23 / 24 | 56 / 55 / 56 | 61 / 59 / 61 |
| `TRAINER_CEDRIC` | CEDRIC | Ordinary | 67 | 34 | 80 | 87 |
| `TRAINER_IRENE` | IRENE | Ordinary | 46 / 47 | 23 / 24 | 55 / 56 | 59 / 61 |
| `TRAINER_DIANA_2` | DIANA | Ordinary | 48 / 46 / 50 | 24 / 23 / 25 | 57 / 55 / 60 | 62 / 59 / 65 |
| `TRAINER_DIANA_3` | DIANA | Ordinary | 59 / 55 / 50 | 30 / 28 / 25 | 70 / 66 / 60 | 76 / 71 / 65 |
| `TRAINER_DIANA_4` | DIANA | Ordinary | 70 / 70 / 70 | 35 / 35 / 35 | 84 / 84 / 84 | 91 / 91 / 91 |
| `TRAINER_DIANA_5` | DIANA | Ordinary | 100 / 100 / 100 | 50 / 50 / 50 | 100 / 100 / 100 | 100 / 100 / 100 |
| `TRAINER_AMY_AND_LIV_1` | AMY & LIV | Ordinary | 38 / 38 | 19 / 19 | 45 / 45 | 49 / 49 |
| `TRAINER_AMY_AND_LIV_2` | AMY & LIV | Ordinary | 50 / 45 | 25 / 23 | 60 / 54 | 65 / 58 |
| `TRAINER_GINA_AND_MIA_1` | GINA & MIA | Ordinary | 17 / 17 | 9 / 9 | 20 / 20 | 22 / 22 |
| `TRAINER_MIU_AND_YUKI` | MIU & YUKI | Ordinary | 66 / 66 | 33 / 33 | 79 / 79 | 85 / 85 |
| `TRAINER_AMY_AND_LIV_3` | AMY & LIV | Ordinary | 70 / 70 | 35 / 35 | 84 / 84 | 91 / 91 |
| `TRAINER_GINA_AND_MIA_2` | GINA & MIA | Ordinary | 38 / 37 | 19 / 19 | 45 / 44 | 49 / 48 |
| `TRAINER_AMY_AND_LIV_4` | AMY & LIV | Ordinary | 80 / 80 | 40 / 40 | 96 / 96 | 100 / 100 |
| `TRAINER_AMY_AND_LIV_5` | AMY & LIV | Ordinary | 90 / 90 | 45 / 45 | 100 / 100 | 100 / 100 |
| `TRAINER_AMY_AND_LIV_6` | AMY & LIV | Ordinary | 100 / 100 | 50 / 50 | 100 / 100 | 100 / 100 |
| `TRAINER_HUEY` | HUEY | Ordinary | 35 / 35 | 18 / 18 | 42 / 42 | 45 / 45 |
| `TRAINER_EDMOND` | EDMOND | Ordinary | 37 | 19 | 44 | 48 |
| `TRAINER_ERNEST_1` | ERNEST | Ordinary | 72 / 72 | 36 / 36 | 86 / 86 | 93 / 93 |
| `TRAINER_DWAYNE` | DWAYNE | Ordinary | 38 / 39 / 38 | 19 / 20 / 19 | 45 / 46 / 45 | 49 / 50 / 49 |
| `TRAINER_PHILLIP` | PHILLIP | Ordinary | 81 / 79 | 41 / 40 | 97 / 94 | 100 / 100 |
| `TRAINER_LEONARD` | LEONARD | Ordinary | 81 / 80 / 79 | 41 / 40 / 40 | 97 / 96 / 94 | 100 / 100 / 100 |
| `TRAINER_DUNCAN` | DUNCAN | Ordinary | 61 / 62 | 31 / 31 | 73 / 74 | 79 / 80 |
| `TRAINER_ERNEST_2` | ERNEST | Ordinary | 75 / 75 / 75 | 38 / 38 / 38 | 90 / 90 / 90 | 97 / 97 / 97 |
| `TRAINER_ERNEST_3` | ERNEST | Ordinary | 78 / 78 / 78 | 39 / 39 / 39 | 93 / 93 / 93 | 100 / 100 / 100 |
| `TRAINER_ERNEST_4` | ERNEST | Ordinary | 81 / 81 / 81 | 41 / 41 / 41 | 97 / 97 / 97 | 100 / 100 / 100 |
| `TRAINER_ERNEST_5` | ERNEST | Ordinary | 100 / 100 / 100 | 50 / 50 / 50 | 100 / 100 / 100 | 100 / 100 / 100 |
| `TRAINER_ELI` | ELI | Ordinary | 47 | 24 | 56 | 61 |
| `TRAINER_ANNIKA` | ANNIKA | Ordinary | 88 / 88 | 44 / 44 | 100 / 100 | 100 / 100 |
| `TRAINER_JAZMYN` | JAZMYN | Ordinary | 67 | 34 | 80 | 87 |
| `TRAINER_JONAS` | JONAS | Ordinary | 68 | 34 | 81 | 88 |
| `TRAINER_KAYLEY` | KAYLEY | Ordinary | 68 | 34 | 81 | 88 |
| `TRAINER_AURON` | AURON | Ordinary | 70 / 70 | 35 / 35 | 84 / 84 | 91 / 91 |
| `TRAINER_KELVIN` | KELVIN | Ordinary | 82 / 82 | 41 / 41 | 98 / 98 | 100 / 100 |
| `TRAINER_MARLEY` | MARLEY | Ordinary | 83 | 42 | 99 | 100 |
| `TRAINER_REYNA` | REYNA | Ordinary | 82 / 82 | 41 / 41 | 98 / 98 | 100 / 100 |
| `TRAINER_HUDSON` | HUDSON | Ordinary | 83 | 42 | 99 | 100 |
| `TRAINER_CONOR` | CONOR | Ordinary | 83 / 83 | 42 / 42 | 99 / 99 | 100 / 100 |
| `TRAINER_EDWIN_1` | EDWIN | Ordinary | 37 / 37 | 19 / 19 | 44 / 44 | 48 / 48 |
| `TRAINER_HECTOR` | HECTOR | Ordinary | 44 / 44 | 22 / 22 | 52 / 52 | 57 / 57 |
| `TRAINER_TABITHA_MOSSDEEP` | TABITHA | Protected | 78 / 78 / 79 | 39 / 39 / 40 | 93 / 93 / 94 | 100 / 100 / 100 |
| `TRAINER_EDWIN_2` | EDWIN | Ordinary | 41 / 41 | 21 / 21 | 49 / 49 | 53 / 53 |
| `TRAINER_EDWIN_3` | EDWIN | Ordinary | 44 / 44 | 22 / 22 | 52 / 52 | 57 / 57 |
| `TRAINER_EDWIN_4` | EDWIN | Ordinary | 47 / 47 | 24 / 24 | 56 / 56 | 61 / 61 |
| `TRAINER_EDWIN_5` | EDWIN | Ordinary | 100 / 100 | 50 / 50 | 100 / 100 | 100 / 100 |
| `TRAINER_WALLY_VR_1` | WALLY | Protected | 93 / 92 / 93 / 90 / 94 | 47 / 46 / 47 / 45 / 47 | 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 |
| `TRAINER_BRENDAN_ROUTE_103_MUDKIP` | BRENDAN | Protected | 7 | 4 | 10 | 11 |
| `TRAINER_BRENDAN_ROUTE_110_MUDKIP` | BRENDAN | Protected | 40 / 40 / 42 | 20 / 20 / 21 | 48 / 48 / 50 | 52 / 52 / 54 |
| `TRAINER_BRENDAN_ROUTE_119_MUDKIP` | BRENDAN | Protected | 61 / 61 / 63 | 31 / 31 / 32 | 73 / 73 / 75 | 79 / 79 / 81 |
| `TRAINER_BRENDAN_ROUTE_103_TREECKO` | BRENDAN | Protected | 7 | 4 | 10 | 11 |
| `TRAINER_BRENDAN_ROUTE_110_TREECKO` | BRENDAN | Protected | 40 / 40 / 42 | 20 / 20 / 21 | 48 / 48 / 50 | 52 / 52 / 54 |
| `TRAINER_BRENDAN_ROUTE_119_TREECKO` | BRENDAN | Protected | 61 / 61 / 63 | 31 / 31 / 32 | 73 / 73 / 75 | 79 / 79 / 81 |
| `TRAINER_BRENDAN_ROUTE_103_TORCHIC` | BRENDAN | Protected | 7 | 4 | 10 | 11 |
| `TRAINER_BRENDAN_ROUTE_110_TORCHIC` | BRENDAN | Protected | 40 / 40 / 42 | 20 / 20 / 21 | 48 / 48 / 50 | 52 / 52 / 54 |
| `TRAINER_BRENDAN_ROUTE_119_TORCHIC` | BRENDAN | Protected | 61 / 61 / 63 | 31 / 31 / 32 | 73 / 73 / 75 | 79 / 79 / 81 |
| `TRAINER_MAY_ROUTE_103_MUDKIP` | MAY | Protected | 7 | 4 | 10 | 11 |
| `TRAINER_MAY_ROUTE_110_MUDKIP` | MAY | Protected | 40 / 40 / 42 | 20 / 20 / 21 | 48 / 48 / 50 | 52 / 52 / 54 |
| `TRAINER_MAY_ROUTE_119_MUDKIP` | MAY | Protected | 61 / 61 / 63 | 31 / 31 / 32 | 73 / 73 / 75 | 79 / 79 / 81 |
| `TRAINER_MAY_ROUTE_103_TREECKO` | MAY | Protected | 7 | 4 | 10 | 11 |
| `TRAINER_MAY_ROUTE_110_TREECKO` | MAY | Protected | 40 / 40 / 42 | 20 / 20 / 21 | 48 / 48 / 50 | 52 / 52 / 54 |
| `TRAINER_MAY_ROUTE_119_TREECKO` | MAY | Protected | 61 / 61 / 63 | 31 / 31 / 32 | 73 / 73 / 75 | 79 / 79 / 81 |
| `TRAINER_MAY_ROUTE_103_TORCHIC` | MAY | Protected | 7 | 4 | 10 | 11 |
| `TRAINER_MAY_ROUTE_110_TORCHIC` | MAY | Protected | 40 / 40 / 42 | 20 / 20 / 21 | 48 / 48 / 50 | 52 / 52 / 54 |
| `TRAINER_MAY_ROUTE_119_TORCHIC` | MAY | Protected | 61 / 61 / 63 | 31 / 31 / 32 | 73 / 73 / 75 | 79 / 79 / 81 |
| `TRAINER_ISAAC_1` | ISAAC | Ordinary | 39 / 39 / 39 / 39 | 20 / 20 / 20 / 20 | 46 / 46 / 46 / 46 | 50 / 50 / 50 / 50 |
| `TRAINER_DAVIS` | DAVIS | Ordinary | 67 | 34 | 80 | 87 |
| `TRAINER_MITCHELL` | Neo | Protected | 91 / 91 / 91 | 46 / 46 / 46 | 100 / 100 / 100 | 100 / 100 / 100 |
| `TRAINER_ISAAC_2` | ISAAC | Ordinary | 42 / 42 / 42 / 42 | 21 / 21 / 21 / 21 | 50 / 50 / 50 / 50 | 54 / 54 / 54 / 54 |
| `TRAINER_ISAAC_3` | ISAAC | Ordinary | 45 / 45 / 45 / 45 / 45 | 23 / 23 / 23 / 23 / 23 | 54 / 54 / 54 / 54 / 54 | 58 / 58 / 58 / 58 / 58 |
| `TRAINER_ISAAC_4` | ISAAC | Ordinary | 48 / 48 / 48 / 48 / 48 | 24 / 24 / 24 / 24 / 24 | 57 / 57 / 57 / 57 / 57 | 62 / 62 / 62 / 62 / 62 |
| `TRAINER_ISAAC_5` | ISAAC | Ordinary | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_LYDIA_1` | LYDIA | Ordinary | 39 / 39 / 39 / 39 | 20 / 20 / 20 / 20 | 46 / 46 / 46 / 46 | 50 / 50 / 50 / 50 |
| `TRAINER_HALLE` | HALLE | Ordinary | 88 / 88 | 44 / 44 | 100 / 100 | 100 / 100 |
| `TRAINER_GARRISON` | GARRISON | Ordinary | 57 | 29 | 68 | 74 |
| `TRAINER_LYDIA_2` | LYDIA | Ordinary | 42 / 42 / 42 / 42 | 21 / 21 / 21 / 21 | 50 / 50 / 50 / 50 | 54 / 54 / 54 / 54 |
| `TRAINER_LYDIA_3` | LYDIA | Ordinary | 45 / 45 / 45 / 45 / 45 | 23 / 23 / 23 / 23 / 23 | 54 / 54 / 54 / 54 / 54 | 58 / 58 / 58 / 58 / 58 |
| `TRAINER_LYDIA_4` | LYDIA | Ordinary | 48 / 48 / 48 / 48 / 48 | 24 / 24 / 24 / 24 / 24 | 57 / 57 / 57 / 57 / 57 | 62 / 62 / 62 / 62 / 62 |
| `TRAINER_LYDIA_5` | LYDIA | Ordinary | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_JACKSON_1` | JACKSON | Ordinary | 58 | 29 | 69 | 75 |
| `TRAINER_LORENZO` | LORENZO | Ordinary | 61 / 61 / 61 | 31 / 31 / 31 | 73 / 73 / 73 | 79 / 79 / 79 |
| `TRAINER_SEBASTIAN` | SEBASTIAN | Ordinary | 68 | 34 | 81 | 88 |
| `TRAINER_JACKSON_2` | JACKSON | Ordinary | 61 | 31 | 73 | 79 |
| `TRAINER_JACKSON_3` | JACKSON | Ordinary | 64 | 32 | 76 | 83 |
| `TRAINER_JACKSON_4` | JACKSON | Ordinary | 67 | 34 | 80 | 87 |
| `TRAINER_JACKSON_5` | JACKSON | Ordinary | 100 / 100 | 50 / 50 | 100 / 100 | 100 / 100 |
| `TRAINER_CATHERINE_1` | CATHERINE | Ordinary | 58 / 58 | 29 / 29 | 69 / 69 | 75 / 75 |
| `TRAINER_JENNA` | JENNA | Ordinary | 61 / 61 / 61 | 31 / 31 / 31 | 73 / 73 / 73 | 79 / 79 / 79 |
| `TRAINER_SOPHIA` | SOPHIA | Ordinary | 68 / 68 | 34 / 34 | 81 / 81 | 88 / 88 |
| `TRAINER_CATHERINE_2` | CATHERINE | Ordinary | 61 / 61 | 31 / 31 | 73 / 73 | 79 / 79 |
| `TRAINER_CATHERINE_3` | CATHERINE | Ordinary | 64 / 64 | 32 / 32 | 76 / 76 | 83 / 83 |
| `TRAINER_CATHERINE_4` | CATHERINE | Ordinary | 67 / 67 | 34 / 34 | 80 / 80 | 87 / 87 |
| `TRAINER_CATHERINE_5` | CATHERINE | Ordinary | 100 / 100 | 50 / 50 | 100 / 100 | 100 / 100 |
| `TRAINER_JULIO` | JULIO | Ordinary | 47 | 24 | 56 | 61 |
| `TRAINER_GRUNT_SEAFLOOR_CAVERN_5` | GRUNT | Protected | 87 / 87 | 44 / 44 | 100 / 100 | 100 / 100 |
| `TRAINER_GRUNT_UNUSED` | GRUNT | Protected | 46 / 46 | 23 / 23 | 55 / 55 | 59 / 59 |
| `TRAINER_GRUNT_MT_PYRE_4` | GRUNT | Protected | 67 / 67 | 34 / 34 | 80 / 80 | 87 / 87 |
| `TRAINER_GRUNT_JAGGED_PASS` | GRUNT | Protected | 47 / 47 | 24 / 24 | 56 / 56 | 61 / 61 |
| `TRAINER_MARC` | MARC | Ordinary | 21 / 21 | 11 / 11 | 25 / 25 | 27 / 27 |
| `TRAINER_BRENDEN` | BRENDEN | Ordinary | 30 | 15 | 36 | 39 |
| `TRAINER_LILITH` | LILITH | Ordinary | 29 | 15 | 34 | 37 |
| `TRAINER_CRISTIAN` | CRISTIAN | Ordinary | 30 | 15 | 36 | 39 |
| `TRAINER_SYLVIA` | SYLVIA | Ordinary | 73 | 37 | 87 | 94 |
| `TRAINER_LEONARDO` | LEONARDO | Ordinary | 74 | 37 | 88 | 96 |
| `TRAINER_ATHENA` | ATHENA | Ordinary | 78 / 78 | 39 / 39 | 93 / 93 | 100 / 100 |
| `TRAINER_HARRISON` | HARRISON | Ordinary | 80 | 40 | 96 | 100 |
| `TRAINER_GRUNT_MT_CHIMNEY_2` | GRUNT | Protected | 47 | 24 | 56 | 61 |
| `TRAINER_CLARENCE` | CLARENCE | Ordinary | 80 | 40 | 96 | 100 |
| `TRAINER_TERRY` | TERRY | Ordinary | 54 | 27 | 64 | 70 |
| `TRAINER_NATE` | NATE | Ordinary | 73 | 37 | 87 | 94 |
| `TRAINER_KATHLEEN` | Sam | Protected | 71 / 71 / 71 / 71 | 36 / 36 / 36 / 36 | 85 / 85 / 85 / 85 | 92 / 92 / 92 / 92 |
| `TRAINER_CLIFFORD` | CLIFFORD | Ordinary | 73 | 37 | 87 | 94 |
| `TRAINER_NICHOLAS` | NICHOLAS | Ordinary | 73 | 37 | 87 | 94 |
| `TRAINER_GRUNT_SPACE_CENTER_3` | GRUNT | Protected | 79 / 79 | 40 / 40 | 94 / 94 | 100 / 100 |
| `TRAINER_GRUNT_SPACE_CENTER_4` | GRUNT | Protected | 80 | 40 | 96 | 100 |
| `TRAINER_GRUNT_SPACE_CENTER_5` | GRUNT | Protected | 82 | 41 | 98 | 100 |
| `TRAINER_GRUNT_SPACE_CENTER_6` | GRUNT | Protected | 82 | 41 | 98 | 100 |
| `TRAINER_GRUNT_SPACE_CENTER_7` | GRUNT | Protected | 82 | 41 | 98 | 100 |
| `TRAINER_MACEY` | MACEY | Ordinary | 73 | 37 | 87 | 94 |
| `TRAINER_BRENDAN_RUSTBORO_TREECKO` | BRENDAN | Protected | 22 / 24 | 11 / 12 | 26 / 28 | 28 / 31 |
| `TRAINER_BRENDAN_RUSTBORO_MUDKIP` | BRENDAN | Protected | 22 / 24 | 11 / 12 | 26 / 28 | 28 / 31 |
| `TRAINER_PAXTON` | PAXTON | Ordinary | 82 / 82 | 41 / 41 | 98 / 98 | 100 / 100 |
| `TRAINER_ISABELLA` | ISABELLA | Ordinary | 72 | 36 | 86 | 93 |
| `TRAINER_GRUNT_WEATHER_INST_5` | GRUNT | Protected | 62 / 62 | 31 / 31 | 74 / 74 | 80 / 80 |
| `TRAINER_TABITHA_MT_CHIMNEY` | TABITHA | Protected | 46 / 46 / 46 / 46 | 23 / 23 / 23 / 23 | 55 / 55 / 55 / 55 | 59 / 59 / 59 / 59 |
| `TRAINER_JONATHAN` | JONATHAN | Ordinary | 82 / 82 | 41 / 41 | 98 / 98 | 100 / 100 |
| `TRAINER_BRENDAN_RUSTBORO_TORCHIC` | BRENDAN | Protected | 22 / 24 | 11 / 12 | 26 / 28 | 28 / 31 |
| `TRAINER_MAY_RUSTBORO_MUDKIP` | MAY | Protected | 22 / 24 | 11 / 12 | 26 / 28 | 28 / 31 |
| `TRAINER_MAXIE_MAGMA_HIDEOUT` | MAXIE | Protected | 69 / 69 / 69 | 35 / 35 / 35 | 82 / 82 / 82 | 89 / 89 / 89 |
| `TRAINER_MAXIE_MT_CHIMNEY` | MAXIE | Protected | 46 / 46 / 46 | 23 / 23 / 23 | 55 / 55 / 55 | 59 / 59 / 59 |
| `TRAINER_TIANA` | TIANA | Ordinary | 12 / 15 | 6 / 8 | 15 / 18 | 16 / 19 |
| `TRAINER_HALEY_1` | HALEY | Ordinary | 14 / 14 | 7 / 7 | 17 / 17 | 18 / 18 |
| `TRAINER_JANICE` | JANICE | Ordinary | 20 | 10 | 24 | 26 |
| `TRAINER_VIVI` | VIVI | Protected | 41 / 41 / 41 | 21 / 21 / 21 | 49 / 49 / 49 | 53 / 53 / 53 |
| `TRAINER_HALEY_2` | HALEY | Ordinary | 41 / 41 | 21 / 21 | 49 / 49 | 53 / 53 |
| `TRAINER_HALEY_3` | HALEY | Ordinary | 44 / 44 | 22 / 22 | 52 / 52 | 57 / 57 |
| `TRAINER_HALEY_4` | HALEY | Ordinary | 48 / 48 | 24 / 24 | 57 / 57 | 62 / 62 |
| `TRAINER_HALEY_5` | HALEY | Ordinary | 51 / 51 / 51 | 26 / 26 / 26 | 61 / 61 / 61 | 66 / 66 / 66 |
| `TRAINER_SALLY` | SALLY | Ordinary | 38 | 19 | 45 | 49 |
| `TRAINER_ROBIN` | ROBIN | Ordinary | 38 / 38 / 38 | 19 / 19 / 19 | 45 / 45 / 45 | 49 / 49 / 49 |
| `TRAINER_ANDREA` | ANDREA | Ordinary | 88 | 44 | 100 | 100 |
| `TRAINER_CRISSY` | CRISSY | Ordinary | 89 / 88 | 45 / 44 | 100 / 100 | 100 / 100 |
| `TRAINER_RICK` | RICK | Ordinary | 12 / 13 | 6 / 7 | 15 / 16 | 16 / 17 |
| `TRAINER_LYLE` | LYLE | Ordinary | 18 / 17 / 18 | 9 / 9 / 9 | 21 / 20 / 21 | 23 / 22 / 23 |
| `TRAINER_JOSE` | JOSE | Ordinary | 20 / 20 | 10 / 10 | 24 / 24 | 26 / 26 |
| `TRAINER_DOUG` | DOUG | Ordinary | 58 / 58 | 29 / 29 | 69 / 69 | 75 / 75 |
| `TRAINER_GREG` | GREG | Ordinary | 58 / 58 | 29 / 29 | 69 / 69 | 75 / 75 |
| `TRAINER_KENT` | KENT | Ordinary | 58 | 29 | 69 | 75 |
| `TRAINER_JAMES_1` | JAMES | Ordinary | 20 / 18 | 10 / 9 | 24 / 21 | 26 / 23 |
| `TRAINER_JAMES_2` | JAMES | Ordinary | 42 / 42 | 21 / 21 | 50 / 50 | 54 / 54 |
| `TRAINER_JAMES_3` | JAMES | Ordinary | 45 / 45 | 23 / 23 | 54 / 54 | 58 / 58 |
| `TRAINER_JAMES_4` | JAMES | Ordinary | 48 / 48 / 48 | 24 / 24 / 24 | 57 / 57 / 57 | 62 / 62 / 62 |
| `TRAINER_JAMES_5` | JAMES | Ordinary | 51 / 51 / 51 / 51 | 26 / 26 / 26 / 26 | 61 / 61 / 61 / 61 | 66 / 66 / 66 / 66 |
| `TRAINER_BRICE` | BRICE | Ordinary | 42 / 42 | 21 / 21 | 50 / 50 | 54 / 54 |
| `TRAINER_TRENT_1` | TRENT | Ordinary | 42 / 42 / 42 | 21 / 21 / 21 | 50 / 50 / 50 | 54 / 54 / 54 |
| `TRAINER_LENNY` | LENNY | Ordinary | 44 / 44 | 22 / 22 | 52 / 52 | 57 / 57 |
| `TRAINER_LUCAS_1` | LUCAS | Ordinary | 44 / 44 | 22 / 22 | 52 / 52 | 57 / 57 |
| `TRAINER_ALAN` | ALAN | Ordinary | 47 / 47 / 47 | 24 / 24 / 24 | 56 / 56 / 56 | 61 / 61 / 61 |
| `TRAINER_CLARK` | CLARK | Ordinary | 20 | 10 | 24 | 26 |
| `TRAINER_ERIC` | ERIC | Ordinary | 47 / 47 | 24 / 24 | 56 / 56 | 61 / 61 |
| `TRAINER_LUCAS_2` | LUCAS | Ordinary | 21 | 11 | 25 | 27 |
| `TRAINER_MIKE_1` | MIKE | Ordinary | 22 / 22 | 11 / 11 | 26 / 26 | 28 / 28 |
| `TRAINER_MIKE_2` | MIKE | Ordinary | 23 / 23 / 23 | 12 / 12 / 12 | 27 / 27 / 27 | 29 / 29 / 29 |
| `TRAINER_TRENT_2` | TRENT | Ordinary | 45 / 45 / 45 / 45 | 23 / 23 / 23 / 23 | 54 / 54 / 54 / 54 | 58 / 58 / 58 / 58 |
| `TRAINER_TRENT_3` | TRENT | Ordinary | 48 / 48 / 48 / 48 | 24 / 24 / 24 / 24 | 57 / 57 / 57 / 57 | 62 / 62 / 62 / 62 |
| `TRAINER_TRENT_4` | TRENT | Ordinary | 51 / 51 / 51 / 51 | 26 / 26 / 26 / 26 | 61 / 61 / 61 / 61 | 66 / 66 / 66 / 66 |
| `TRAINER_TRENT_5` | TRENT | Ordinary | 54 / 54 / 54 / 54 | 27 / 27 / 27 / 27 | 64 / 64 / 64 / 64 | 70 / 70 / 70 / 70 |
| `TRAINER_DEZ_AND_LUKE` | DEZ & LUKE | Ordinary | 68 / 68 | 34 / 34 | 81 / 81 | 88 / 88 |
| `TRAINER_LEA_AND_JED` | LEA & JED | Ordinary | 63 / 63 | 32 / 32 | 75 / 75 | 81 / 81 |
| `TRAINER_KIRA_AND_DAN_1` | KIRA & DAN | Ordinary | 57 / 57 | 29 / 29 | 68 / 68 | 74 / 74 |
| `TRAINER_KIRA_AND_DAN_2` | KIRA & DAN | Ordinary | 60 / 60 | 30 / 30 | 72 / 72 | 78 / 78 |
| `TRAINER_KIRA_AND_DAN_3` | KIRA & DAN | Ordinary | 63 / 63 | 32 / 32 | 75 / 75 | 81 / 81 |
| `TRAINER_KIRA_AND_DAN_4` | KIRA & DAN | Ordinary | 66 / 66 | 33 / 33 | 79 / 79 | 85 / 85 |
| `TRAINER_KIRA_AND_DAN_5` | KIRA & DAN | Ordinary | 69 / 69 | 35 / 35 | 82 / 82 | 89 / 89 |
| `TRAINER_JOHANNA` | JOHANNA | Ordinary | 35 | 18 | 42 | 45 |
| `TRAINER_GERALD` | GERALD | Ordinary | 47 | 24 | 56 | 61 |
| `TRAINER_VIVIAN` | VIVIAN | Ordinary | 41 / 41 | 21 / 21 | 49 / 49 | 53 / 53 |
| `TRAINER_DANIELLE` | DANIELLE | Ordinary | 47 | 24 | 56 | 61 |
| `TRAINER_HIDEO` | HIDEO | Ordinary | 58 / 58 | 29 / 29 | 69 / 69 | 75 / 75 |
| `TRAINER_KEIGO` | KEIGO | Ordinary | 61 / 61 | 31 / 31 | 73 / 73 | 79 / 79 |
| `TRAINER_RILEY` | RILEY | Ordinary | 61 / 61 | 31 / 31 | 73 / 73 | 79 / 79 |
| `TRAINER_FLINT` | FLINT | Ordinary | 61 / 61 | 31 / 31 | 73 / 73 | 79 / 79 |
| `TRAINER_ASHLEY` | ASHLEY | Ordinary | 61 / 61 / 61 | 31 / 31 / 31 | 73 / 73 / 73 | 79 / 79 / 79 |
| `TRAINER_WALLY_MAUVILLE` | WALLY | Protected | 41 | 21 | 49 | 53 |
| `TRAINER_WALLY_VR_2` | WALLY | Protected | 96 / 95 / 96 / 93 / 97 | 48 / 48 / 48 / 47 / 49 | 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 |
| `TRAINER_WALLY_VR_3` | WALLY | Protected | 99 / 98 / 99 / 96 / 100 | 50 / 49 / 50 / 48 / 50 | 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 |
| `TRAINER_WALLY_VR_4` | WALLY | Protected | 99 / 98 / 99 / 96 / 100 | 50 / 49 / 50 / 48 / 50 | 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 |
| `TRAINER_WALLY_VR_5` | WALLY | Protected | 99 / 98 / 99 / 96 / 100 | 50 / 49 / 50 / 48 / 50 | 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 |
| `TRAINER_BRENDAN_LILYCOVE_MUDKIP` | BRENDAN | Protected | 69 / 70 / 70 / 72 | 35 / 35 / 35 / 36 | 82 / 84 / 84 / 86 | 89 / 91 / 91 / 93 |
| `TRAINER_BRENDAN_LILYCOVE_TREECKO` | BRENDAN | Protected | 69 / 70 / 70 / 72 | 35 / 35 / 35 / 36 | 82 / 84 / 84 / 86 | 89 / 91 / 91 / 93 |
| `TRAINER_BRENDAN_LILYCOVE_TORCHIC` | BRENDAN | Protected | 69 / 70 / 70 / 72 | 35 / 35 / 35 / 36 | 82 / 84 / 84 / 86 | 89 / 91 / 91 / 93 |
| `TRAINER_MAY_LILYCOVE_MUDKIP` | MAY | Protected | 69 / 70 / 70 / 72 | 35 / 35 / 35 / 36 | 82 / 84 / 84 / 86 | 89 / 91 / 91 / 93 |
| `TRAINER_MAY_LILYCOVE_TREECKO` | MAY | Protected | 69 / 70 / 70 / 72 | 35 / 35 / 35 / 36 | 82 / 84 / 84 / 86 | 89 / 91 / 91 / 93 |
| `TRAINER_MAY_LILYCOVE_TORCHIC` | MAY | Protected | 69 / 70 / 70 / 72 | 35 / 35 / 35 / 36 | 82 / 84 / 84 / 86 | 89 / 91 / 91 / 93 |
| `TRAINER_JONAH` | JONAH | Ordinary | 78 / 78 / 78 | 39 / 39 / 39 | 93 / 93 / 93 | 100 / 100 / 100 |
| `TRAINER_HENRY` | HENRY | Ordinary | 78 / 78 | 39 / 39 | 93 / 93 | 100 / 100 |
| `TRAINER_ROGER` | ROGER | Ordinary | 76 / 77 / 78 | 38 / 39 / 39 | 91 / 92 / 93 | 98 / 100 / 100 |
| `TRAINER_ALEXA` | ALEXA | Ordinary | 79 / 79 | 40 / 40 | 94 / 94 | 100 / 100 |
| `TRAINER_RUBEN` | RUBEN | Ordinary | 79 / 79 | 40 / 40 | 94 / 94 | 100 / 100 |
| `TRAINER_KOJI_1` | KOJI | Ordinary | 78 | 39 | 93 | 100 |
| `TRAINER_WAYNE` | WAYNE | Ordinary | 78 / 78 / 81 | 39 / 39 / 41 | 93 / 93 / 97 | 100 / 100 / 100 |
| `TRAINER_AIDAN` | AIDAN | Ordinary | 78 / 78 | 39 / 39 | 93 / 93 | 100 / 100 |
| `TRAINER_REED` | REED | Ordinary | 80 / 80 | 40 / 40 | 96 / 96 | 100 / 100 |
| `TRAINER_TISHA` | TISHA | Ordinary | 80 | 40 | 96 | 100 |
| `TRAINER_TORI_AND_TIA` | TORI & TIA | Ordinary | 43 / 43 | 22 / 22 | 51 / 51 | 55 / 55 |
| `TRAINER_KIM_AND_IRIS` | KIM & IRIS | Ordinary | 70 / 70 | 35 / 35 | 84 / 84 | 91 / 91 |
| `TRAINER_TYRA_AND_IVY` | TYRA & IVY | Ordinary | 44 / 44 | 22 / 22 | 52 / 52 | 57 / 57 |
| `TRAINER_MEL_AND_PAUL` | MEL & PAUL | Ordinary | 36 / 36 | 18 / 18 | 43 / 43 | 46 / 46 |
| `TRAINER_JOHN_AND_JAY_1` | JOHN & JAY | Ordinary | 78 / 78 | 39 / 39 | 93 / 93 | 100 / 100 |
| `TRAINER_JOHN_AND_JAY_2` | JOHN & JAY | Ordinary | 81 / 81 | 41 / 41 | 97 / 97 | 100 / 100 |
| `TRAINER_JOHN_AND_JAY_3` | JOHN & JAY | Ordinary | 84 / 84 | 42 / 42 | 100 / 100 | 100 / 100 |
| `TRAINER_JOHN_AND_JAY_4` | JOHN & JAY | Ordinary | 87 / 87 | 44 / 44 | 100 / 100 | 100 / 100 |
| `TRAINER_JOHN_AND_JAY_5` | JOHN & JAY | Ordinary | 90 / 90 | 45 / 45 | 100 / 100 | 100 / 100 |
| `TRAINER_RELI_AND_IAN` | RELI & IAN | Ordinary | 82 / 82 | 41 / 41 | 98 / 98 | 100 / 100 |
| `TRAINER_LILA_AND_ROY_1` | LILA & ROY | Ordinary | 72 / 71 | 36 / 36 | 86 / 85 | 93 / 92 |
| `TRAINER_LILA_AND_ROY_2` | LILA & ROY | Ordinary | 75 / 72 | 38 / 36 | 90 / 86 | 97 / 93 |
| `TRAINER_LILA_AND_ROY_3` | LILA & ROY | Ordinary | 78 / 76 | 39 / 38 | 93 / 91 | 100 / 98 |
| `TRAINER_LILA_AND_ROY_4` | LILA & ROY | Ordinary | 81 / 78 | 41 / 39 | 97 / 93 | 100 / 100 |
| `TRAINER_LILA_AND_ROY_5` | LILA & ROY | Ordinary | 84 / 82 | 42 / 41 | 100 / 98 | 100 / 100 |
| `TRAINER_LISA_AND_RAY` | LISA & RAY | Ordinary | 56 / 56 | 28 / 28 | 67 / 67 | 72 / 72 |
| `TRAINER_CHRIS` | CHRIS | Ordinary | 58 / 57 / 58 / 57 | 29 / 29 / 29 / 29 | 69 / 68 / 69 / 68 | 75 / 74 / 75 / 74 |
| `TRAINER_DAWSON` | DAWSON | Ordinary | 20 / 20 | 10 / 10 | 24 / 24 | 26 / 26 |
| `TRAINER_SARAH` | SARAH | Ordinary | 20 / 20 | 10 / 10 | 24 / 24 | 26 / 26 |
| `TRAINER_DARIAN` | DARIAN | Ordinary | 21 | 11 | 25 | 27 |
| `TRAINER_HAILEY` | HAILEY | Ordinary | 35 | 18 | 42 | 45 |
| `TRAINER_CHANDLER` | CHANDLER | Ordinary | 35 / 35 | 18 / 18 | 42 / 42 | 45 / 45 |
| `TRAINER_KALEB` | KALEB | Ordinary | 36 / 36 | 18 / 18 | 43 / 43 | 46 / 46 |
| `TRAINER_JOSEPH` | JOSEPH | Ordinary | 36 / 36 | 18 / 18 | 43 / 43 | 46 / 46 |
| `TRAINER_ALYSSA` | ALYSSA | Ordinary | 37 | 19 | 44 | 48 |
| `TRAINER_MARCOS` | MARCOS | Ordinary | 37 | 19 | 44 | 48 |
| `TRAINER_RHETT` | RHETT | Ordinary | 37 | 19 | 44 | 48 |
| `TRAINER_TYRON` | TYRON | Ordinary | 43 | 22 | 51 | 55 |
| `TRAINER_CELINA` | CELINA | Ordinary | 43 | 22 | 51 | 55 |
| `TRAINER_BIANCA` | BIANCA | Ordinary | 43 | 22 | 51 | 55 |
| `TRAINER_HAYDEN` | HAYDEN | Ordinary | 43 | 22 | 51 | 55 |
| `TRAINER_SOPHIE` | SOPHIE | Ordinary | 43 / 43 | 22 / 22 | 51 / 51 | 55 / 55 |
| `TRAINER_COBY` | COBY | Ordinary | 43 / 43 | 22 / 22 | 51 / 51 | 55 / 55 |
| `TRAINER_LAWRENCE` | LAWRENCE | Ordinary | 43 / 43 | 22 / 22 | 51 / 51 | 55 / 55 |
| `TRAINER_WYATT` | WYATT | Ordinary | 43 / 43 | 22 / 22 | 51 / 51 | 55 / 55 |
| `TRAINER_ANGELINA` | ANGELINA | Ordinary | 44 / 44 | 22 / 22 | 52 / 52 | 57 / 57 |
| `TRAINER_KAI` | KAI | Ordinary | 44 | 22 | 52 | 57 |
| `TRAINER_CHARLOTTE` | CHARLOTTE | Ordinary | 44 | 22 | 52 | 57 |
| `TRAINER_DEANDRE` | DEANDRE | Ordinary | 40 / 40 / 40 | 20 / 20 / 20 | 48 / 48 / 48 | 52 / 52 / 52 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_1` | GRUNT | Protected | 71 | 36 | 85 | 92 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_2` | GRUNT | Protected | 71 | 36 | 85 | 92 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_3` | GRUNT | Protected | 71 | 36 | 85 | 92 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_4` | GRUNT | Protected | 70 / 70 | 35 / 35 | 84 / 84 | 91 / 91 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_5` | GRUNT | Protected | 70 / 70 | 35 / 35 | 84 / 84 | 91 / 91 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_6` | GRUNT | Protected | 71 | 36 | 85 | 92 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_7` | GRUNT | Protected | 71 | 36 | 85 | 92 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_8` | GRUNT | Protected | 71 | 36 | 85 | 92 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_9` | GRUNT | Protected | 71 | 36 | 85 | 92 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_10` | GRUNT | Protected | 71 | 36 | 85 | 92 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_11` | GRUNT | Protected | 71 | 36 | 85 | 92 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_12` | GRUNT | Protected | 71 | 36 | 85 | 92 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_13` | GRUNT | Protected | 71 | 36 | 85 | 92 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_14` | GRUNT | Protected | 71 | 36 | 85 | 92 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_15` | GRUNT | Protected | 71 | 36 | 85 | 92 |
| `TRAINER_GRUNT_MAGMA_HIDEOUT_16` | GRUNT | Protected | 71 | 36 | 85 | 92 |
| `TRAINER_TABITHA_MAGMA_HIDEOUT` | TABITHA | Protected | 67 / 67 / 67 / 68 | 34 / 34 / 34 / 34 | 80 / 80 / 80 / 81 | 87 / 87 / 87 / 88 |
| `TRAINER_DARCY` | DARCY | Ordinary | 82 / 82 | 41 / 41 | 98 / 98 | 100 / 100 |
| `TRAINER_MAXIE_MOSSDEEP` | MAXIE | Protected | 80 / 80 / 80 | 40 / 40 / 40 | 96 / 96 / 96 | 100 / 100 / 100 |
| `TRAINER_PETE` | PETE | Ordinary | 37 | 19 | 44 | 48 |
| `TRAINER_ISABELLE` | ISABELLE | Ordinary | 37 | 19 | 44 | 48 |
| `TRAINER_ANDRES_1` | ANDRES | Ordinary | 56 / 56 | 28 / 28 | 67 / 67 | 72 / 72 |
| `TRAINER_JOSUE` | JOSUE | Ordinary | 56 / 56 | 28 / 28 | 67 / 67 | 72 / 72 |
| `TRAINER_CAMRON` | CAMRON | Ordinary | 56 | 28 | 67 | 72 |
| `TRAINER_CORY_1` | CORY | Ordinary | 55 / 55 / 55 | 28 / 28 / 28 | 66 / 66 / 66 | 71 / 71 / 71 |
| `TRAINER_CAROLINA` | CAROLINA | Ordinary | 55 / 55 / 55 | 28 / 28 / 28 | 66 / 66 / 66 | 71 / 71 / 71 |
| `TRAINER_ELIJAH` | ELIJAH | Ordinary | 36 / 36 | 18 / 18 | 43 / 43 | 46 / 46 |
| `TRAINER_CELIA` | CELIA | Ordinary | 46 / 46 | 23 / 23 | 55 / 55 | 59 / 59 |
| `TRAINER_BRYAN` | BRYAN | Ordinary | 46 / 46 | 23 / 23 | 55 / 55 | 59 / 59 |
| `TRAINER_BRANDEN` | BRANDEN | Ordinary | 46 / 46 | 23 / 23 | 55 / 55 | 59 / 59 |
| `TRAINER_BRYANT` | BRYANT | Ordinary | 42 / 42 | 21 / 21 | 50 / 50 | 54 / 54 |
| `TRAINER_SHAYLA` | SHAYLA | Ordinary | 42 / 42 | 21 / 21 | 50 / 50 | 54 / 54 |
| `TRAINER_KYRA` | KYRA | Ordinary | 56 / 56 | 28 / 28 | 67 / 67 | 72 / 72 |
| `TRAINER_JAIDEN` | JAIDEN | Ordinary | 56 / 56 | 28 / 28 | 67 / 67 | 72 / 72 |
| `TRAINER_ALIX` | ALIX | Ordinary | 56 / 56 | 28 / 28 | 67 / 67 | 72 / 72 |
| `TRAINER_HELENE` | HELENE | Ordinary | 56 / 56 | 28 / 28 | 67 / 67 | 72 / 72 |
| `TRAINER_MARLENE` | MARLENE | Ordinary | 43 / 43 | 22 / 22 | 51 / 51 | 55 / 55 |
| `TRAINER_DEVAN` | DEVAN | Ordinary | 20 / 20 | 10 / 10 | 24 / 24 | 26 / 26 |
| `TRAINER_JOHNSON` | JOHNSON | Ordinary | 20 / 20 | 10 / 10 | 24 / 24 | 26 / 26 |
| `TRAINER_MELINA` | MELINA | Ordinary | 39 | 20 | 46 | 50 |
| `TRAINER_BRANDI` | BRANDI | Ordinary | 39 | 20 | 46 | 50 |
| `TRAINER_AISHA` | AISHA | Ordinary | 39 | 20 | 46 | 50 |
| `TRAINER_MAKAYLA` | MAKAYLA | Ordinary | 82 / 82 | 41 / 41 | 98 / 98 | 100 / 100 |
| `TRAINER_FABIAN` | FABIAN | Ordinary | 58 | 29 | 69 | 75 |
| `TRAINER_DAYTON` | DAYTON | Ordinary | 58 / 58 | 29 / 29 | 69 / 69 | 75 / 75 |
| `TRAINER_RACHEL` | RACHEL | Ordinary | 58 | 29 | 69 | 75 |
| `TRAINER_LEONEL` | LEONEL | Ordinary | 61 | 31 | 73 | 79 |
| `TRAINER_CALLIE` | CALLIE | Ordinary | 61 / 61 | 31 / 31 | 73 / 73 | 79 / 79 |
| `TRAINER_CALE` | CALE | Ordinary | 67 / 67 | 34 / 34 | 80 / 80 | 87 / 87 |
| `TRAINER_MYLES` | Scotty | Protected | 67 | 34 | 80 | 87 |
| `TRAINER_PAT` | PAT | Ordinary | 66 / 66 / 66 / 66 / 66 / 66 | 33 / 33 / 33 / 33 / 33 / 33 | 79 / 79 / 79 / 79 / 79 / 79 | 85 / 85 / 85 / 85 / 85 / 85 |
| `TRAINER_CRISTIN_1` | CRISTIN | Ordinary | 67 / 67 | 34 / 34 | 80 / 80 | 87 / 87 |
| `TRAINER_MAY_RUSTBORO_TREECKO` | MAY | Protected | 22 / 24 | 11 / 12 | 26 / 28 | 28 / 31 |
| `TRAINER_MAY_RUSTBORO_TORCHIC` | MAY | Protected | 22 / 24 | 11 / 12 | 26 / 28 | 28 / 31 |
| `TRAINER_ROXANNE_2` | ROXANNE | Protected | 50 / 50 / 50 / 50 / 60 | 25 / 25 / 25 / 25 / 30 | 60 / 60 / 60 / 60 / 72 | 65 / 65 / 65 / 65 / 78 |
| `TRAINER_ROXANNE_3` | ROXANNE | Protected | 60 / 60 / 60 / 60 / 70 / 60 | 30 / 30 / 30 / 30 / 35 / 30 | 72 / 72 / 72 / 72 / 84 / 72 | 78 / 78 / 78 / 78 / 91 / 78 |
| `TRAINER_ROXANNE_4` | ROXANNE | Protected | 70 / 70 / 70 / 70 / 80 / 70 | 35 / 35 / 35 / 35 / 40 / 35 | 84 / 84 / 84 / 84 / 96 / 84 | 91 / 91 / 91 / 91 / 100 / 91 |
| `TRAINER_ROXANNE_5` | ROXANNE | Protected | 100 / 100 / 100 / 100 / 100 / 100 / 32 | 50 / 50 / 50 / 50 / 50 / 50 / 16 | 100 / 100 / 100 / 100 / 100 / 100 / 38 | 100 / 100 / 100 / 100 / 100 / 100 / 41 |
| `TRAINER_BRAWLY_2` | BRAWLY | Protected | 60 / 60 / 60 / 70 | 30 / 30 / 30 / 35 | 72 / 72 / 72 / 84 | 78 / 78 / 78 / 91 |
| `TRAINER_BRAWLY_3` | BRAWLY | Protected | 70 / 70 / 70 / 80 | 35 / 35 / 35 / 40 | 84 / 84 / 84 / 96 | 91 / 91 / 91 / 100 |
| `TRAINER_BRAWLY_4` | BRAWLY | Protected | 80 / 80 / 80 / 80 / 90 | 40 / 40 / 40 / 40 / 45 | 96 / 96 / 96 / 96 / 100 | 100 / 100 / 100 / 100 / 100 |
| `TRAINER_BRAWLY_5` | BRAWLY | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_WATTSON_2` | WATTSON | Protected | 70 / 70 / 70 / 80 | 35 / 35 / 35 / 40 | 84 / 84 / 84 / 96 | 91 / 91 / 91 / 100 |
| `TRAINER_WATTSON_3` | WATTSON | Protected | 83 / 83 / 83 / 83 / 83 | 42 / 42 / 42 / 42 / 42 | 99 / 99 / 99 / 99 / 99 | 100 / 100 / 100 / 100 / 100 |
| `TRAINER_WATTSON_4` | WATTSON | Protected | 90 / 90 / 90 / 90 / 90 | 45 / 45 / 45 / 45 / 45 | 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 |
| `TRAINER_WATTSON_5` | WATTSON | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_FLANNERY_2` | FLANNERY | Protected | 75 / 75 / 75 / 75 / 80 | 38 / 38 / 38 / 38 / 40 | 90 / 90 / 90 / 90 / 96 | 97 / 97 / 97 / 97 / 100 |
| `TRAINER_FLANNERY_3` | FLANNERY | Protected | 85 / 85 / 85 / 85 / 85 / 100 | 43 / 43 / 43 / 43 / 43 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_FLANNERY_4` | FLANNERY | Protected | 95 / 95 / 95 / 95 / 95 / 100 | 48 / 48 / 48 / 48 / 48 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_FLANNERY_5` | FLANNERY | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_NORMAN_2` | NORMAN | Protected | 80 / 80 / 80 / 80 / 100 | 40 / 40 / 40 / 40 / 50 | 96 / 96 / 96 / 96 / 100 | 100 / 100 / 100 / 100 / 100 |
| `TRAINER_NORMAN_3` | NORMAN | Protected | 90 / 90 / 90 / 90 / 90 / 100 | 45 / 45 / 45 / 45 / 45 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_NORMAN_4` | NORMAN | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_NORMAN_5` | NORMAN | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_WINONA_2` | WINONA | Protected | 90 / 90 / 90 / 90 / 90 / 90 | 45 / 45 / 45 / 45 / 45 / 45 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_WINONA_3` | WINONA | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_WINONA_4` | WINONA | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_WINONA_5` | WINONA | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_TATE_AND_LIZA_2` | TATE&LIZA | Protected | 95 / 95 / 95 / 95 / 95 / 95 | 48 / 48 / 48 / 48 / 48 / 48 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_TATE_AND_LIZA_3` | TATE&LIZA | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_TATE_AND_LIZA_4` | TATE&LIZA | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_TATE_AND_LIZA_5` | TATE&LIZA | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_JUAN_2` | JUAN | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_JUAN_3` | JUAN | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_JUAN_4` | JUAN | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_JUAN_5` | JUAN | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_ANGELO` | ANGELO | Ordinary | 41 / 41 | 21 / 21 | 49 / 49 | 53 / 53 |
| `TRAINER_DARIUS` | DARIUS | Ordinary | 61 | 31 | 73 | 79 |
| `TRAINER_STEVEN` | STEVEN | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_ANABEL` | ANABEL | Protected | 5 | 3 | 8 | 9 |
| `TRAINER_TUCKER` | TUCKER | Protected | 5 | 3 | 8 | 9 |
| `TRAINER_SPENSER` | SPENSER | Protected | 5 | 3 | 8 | 9 |
| `TRAINER_GRETA` | GRETA | Protected | 5 | 3 | 8 | 9 |
| `TRAINER_NOLAND` | NOLAND | Protected | 5 | 3 | 8 | 9 |
| `TRAINER_LUCY` | LUCY | Protected | 5 | 3 | 8 | 9 |
| `TRAINER_BRANDON` | BRANDON | Protected | 5 | 3 | 8 | 9 |
| `TRAINER_ANDRES_2` | ANDRES | Ordinary | 59 / 59 | 30 / 30 | 70 / 70 | 76 / 76 |
| `TRAINER_ANDRES_3` | ANDRES | Ordinary | 62 / 62 / 62 | 31 / 31 / 31 | 74 / 74 / 74 | 80 / 80 / 80 |
| `TRAINER_ANDRES_4` | ANDRES | Ordinary | 65 / 65 / 65 | 33 / 33 / 33 | 78 / 78 / 78 | 84 / 84 / 84 |
| `TRAINER_ANDRES_5` | ANDRES | Ordinary | 68 / 68 / 68 | 34 / 34 / 34 | 81 / 81 / 81 | 88 / 88 / 88 |
| `TRAINER_CORY_2` | CORY | Ordinary | 58 / 58 / 58 | 29 / 29 / 29 | 69 / 69 / 69 | 75 / 75 / 75 |
| `TRAINER_CORY_3` | CORY | Ordinary | 61 / 61 / 61 | 31 / 31 / 31 | 73 / 73 / 73 | 79 / 79 / 79 |
| `TRAINER_CORY_4` | CORY | Ordinary | 64 / 64 / 64 | 32 / 32 / 32 | 76 / 76 / 76 | 83 / 83 / 83 |
| `TRAINER_CORY_5` | CORY | Ordinary | 67 / 67 / 67 | 34 / 34 / 34 | 80 / 80 / 80 | 87 / 87 / 87 |
| `TRAINER_PABLO_2` | PABLO | Ordinary | 77 / 77 | 39 / 39 | 92 / 92 | 100 / 100 |
| `TRAINER_PABLO_3` | PABLO | Ordinary | 80 / 80 / 80 | 40 / 40 / 40 | 96 / 96 / 96 | 100 / 100 / 100 |
| `TRAINER_PABLO_4` | PABLO | Ordinary | 83 / 83 / 83 | 42 / 42 / 42 | 99 / 99 / 99 | 100 / 100 / 100 |
| `TRAINER_PABLO_5` | PABLO | Ordinary | 86 / 86 / 86 | 43 / 43 / 43 | 100 / 100 / 100 | 100 / 100 / 100 |
| `TRAINER_KOJI_2` | KOJI | Ordinary | 81 / 81 | 41 / 41 | 97 / 97 | 100 / 100 |
| `TRAINER_KOJI_3` | KOJI | Ordinary | 84 / 84 / 84 | 42 / 42 / 42 | 100 / 100 / 100 | 100 / 100 / 100 |
| `TRAINER_KOJI_4` | KOJI | Ordinary | 87 / 87 / 87 | 44 / 44 / 44 | 100 / 100 / 100 | 100 / 100 / 100 |
| `TRAINER_KOJI_5` | KOJI | Ordinary | 90 / 90 / 90 | 45 / 45 / 45 | 100 / 100 / 100 | 100 / 100 / 100 |
| `TRAINER_CRISTIN_2` | CRISTIN | Ordinary | 70 / 70 | 35 / 35 | 84 / 84 | 91 / 91 |
| `TRAINER_CRISTIN_3` | CRISTIN | Ordinary | 73 / 73 / 73 | 37 / 37 / 37 | 87 / 87 / 87 | 94 / 94 / 94 |
| `TRAINER_CRISTIN_4` | CRISTIN | Ordinary | 76 / 76 / 76 | 38 / 38 / 38 | 91 / 91 / 91 | 98 / 98 / 98 |
| `TRAINER_CRISTIN_5` | CRISTIN | Ordinary | 79 / 79 / 79 | 40 / 40 / 40 | 94 / 94 / 94 | 100 / 100 / 100 |
| `TRAINER_FERNANDO_2` | FERNANDO | Ordinary | 72 / 72 / 72 | 36 / 36 / 36 | 86 / 86 / 86 | 93 / 93 / 93 |
| `TRAINER_FERNANDO_3` | FERNANDO | Ordinary | 75 / 75 / 75 | 38 / 38 / 38 | 90 / 90 / 90 | 97 / 97 / 97 |
| `TRAINER_FERNANDO_4` | FERNANDO | Ordinary | 78 / 78 / 78 | 39 / 39 / 39 | 93 / 93 / 93 | 100 / 100 / 100 |
| `TRAINER_FERNANDO_5` | FERNANDO | Ordinary | 81 / 81 / 81 | 41 / 41 / 41 | 97 / 97 / 97 | 100 / 100 / 100 |
| `TRAINER_SAWYER_2` | SAWYER | Ordinary | 49 / 49 | 25 / 25 | 58 / 58 | 63 / 63 |
| `TRAINER_SAWYER_3` | SAWYER | Ordinary | 52 / 52 / 52 | 26 / 26 / 26 | 62 / 62 / 62 | 67 / 67 / 67 |
| `TRAINER_SAWYER_4` | SAWYER | Ordinary | 55 / 55 / 55 | 28 / 28 / 28 | 66 / 66 / 66 | 71 / 71 / 71 |
| `TRAINER_SAWYER_5` | SAWYER | Ordinary | 58 / 58 / 58 | 29 / 29 / 29 | 69 / 69 / 69 | 75 / 75 / 75 |
| `TRAINER_GABRIELLE_2` | GABRIELLE | Ordinary | 69 / 69 / 69 / 69 / 69 / 69 | 35 / 35 / 35 / 35 / 35 / 35 | 82 / 82 / 82 / 82 / 82 / 82 | 89 / 89 / 89 / 89 / 89 / 89 |
| `TRAINER_GABRIELLE_3` | GABRIELLE | Ordinary | 72 / 72 / 72 / 72 / 72 / 72 | 36 / 36 / 36 / 36 / 36 / 36 | 86 / 86 / 86 / 86 / 86 / 86 | 93 / 93 / 93 / 93 / 93 / 93 |
| `TRAINER_GABRIELLE_4` | GABRIELLE | Ordinary | 75 / 75 / 75 / 75 / 75 / 75 | 38 / 38 / 38 / 38 / 38 / 38 | 90 / 90 / 90 / 90 / 90 / 90 | 97 / 97 / 97 / 97 / 97 / 97 |
| `TRAINER_GABRIELLE_5` | GABRIELLE | Ordinary | 78 / 78 / 78 / 78 / 78 / 78 | 39 / 39 / 39 / 39 / 39 / 39 | 93 / 93 / 93 / 93 / 93 / 93 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_THALIA_2` | THALIA | Ordinary | 60 / 60 | 30 / 30 | 72 / 72 | 78 / 78 |
| `TRAINER_THALIA_3` | THALIA | Ordinary | 63 / 63 / 63 | 32 / 32 / 32 | 75 / 75 / 75 | 81 / 81 / 81 |
| `TRAINER_THALIA_4` | THALIA | Ordinary | 66 / 66 / 66 | 33 / 33 / 33 | 79 / 79 / 79 | 85 / 85 / 85 |
| `TRAINER_THALIA_5` | THALIA | Ordinary | 69 / 69 / 69 | 35 / 35 / 35 | 82 / 82 / 82 | 89 / 89 / 89 |
| `TRAINER_MARIELA` | MARIELA | Ordinary | 80 | 40 | 96 | 100 |
| `TRAINER_ALVARO` | ALVARO | Ordinary | 80 / 80 | 40 / 40 | 96 / 96 | 100 / 100 |
| `TRAINER_EVERETT` | EVERETT | Ordinary | 80 | 40 | 96 | 100 |
| `TRAINER_RED` | RED | Protected | 5 | 3 | 8 | 9 |
| `TRAINER_LEAF` | LEAF | Protected | 5 | 3 | 8 | 9 |
| `TRAINER_BRENDAN_PLACEHOLDER` | BRENDAN | Protected | 5 | 3 | 8 | 9 |
| `TRAINER_MAY_PLACEHOLDER` | MAY | Protected | 5 | 3 | 8 | 9 |
| `TRAINER_RED2` | RED | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_BLUE` | BLUE | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_CYNTHIA` | Cynthia | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_VOLO` | Volo | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |
| `TRAINER_UNBORNE` | Unborne | Protected | 100 / 100 / 100 / 100 / 100 / 100 | 50 / 50 / 50 / 50 / 50 / 50 | 100 / 100 / 100 / 100 / 100 / 100 | 100 / 100 / 100 / 100 / 100 / 100 |

859 populated trainer entries; *Hard/Extreme values are before the current badge cap.

Regenerate with `python tools/brisk/trainer_mode_plan.py`; validate with `--check`.
