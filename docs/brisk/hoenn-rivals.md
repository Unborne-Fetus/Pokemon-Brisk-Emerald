# May and Brendan — original Hoenn rival battles

Their Hoenn teams now **grow into their Kanto postgame identities**, instead of using completely unrelated rosters on each route. The executable battle loader is `src/data/brisk_hoenn_rivals_loader.h`, with 23 curated Pokémon templates in `src/data/brisk_hoenn_rivals_teams.h`.

The original Hoenn story remains intact: all 24 Rustboro/Route 110/Route 119/Lilycove trainer IDs, the battle dialogue and defeated flags, the player's starter choice, and the one-starter Route 103 introduction are preserved. The loader selects the team for the current encounter, gender, player-starter variant and all five difficulty modes before battle. Existing generated `brisk_mode_teams.party` rows remain as legacy source data but are **overridden when these Hoenn rival battles begin**. The difficulty-specific online story trainer generator is not used for these encounters.

## Evolution and progression

| Battle | Easy ace | Normal ace | Hard ace | Expert ace | Kaizo ace | Team size (Easy / Normal / Hard / Expert / Kaizo) |
| --- | ---: | ---: | ---: | ---: | ---: | --- |
| Rustboro | 12 | 24 | 25 | 25 | 25 | 2 / 3 / 4 / 4 / 5 |
| Route 110 | 22 | 44 | 45 | 45 | 45 | 3 / 4 / 5 / 5 / 6 |
| Route 119 | 33 | 65 | 65 | 65 | 65 | 4 / 5 / 6 / 6 / 6 |
| Lilycove | 37 | 74 | 75 | 75 | 75 | 4 / 6 / 6 / 6 / 6 |

The rival's starter always counters the player's own starter: Torchic → Combusken → Blaziken versus Treecko; Mudkip → Marshtomp → Swampert versus Torchic; Treecko → Grovyle → Sceptile versus Mudkip. It is always the last roster slot. Their late Hoenn Pokémon are not Mega-evolved yet; the starter later becomes their Mega ace at Kanto's Champion fight.

### Supporting Pokémon

* **May:** Wingull, Electrike, Shroomish and Marill at Rustboro; Pelipper, Breloom, Manectric and Vibrava at Route 110; adds Milotic and Metagross by Route 119–Lilycove. On harder late encounters she replaces Pelipper with a powerful Salamence, foreshadowing her Kanto League team.
* **Brendan:** Shroomish, Electrike, Wingull and Marill early; Breloom, Manectric and Vibrava at Route 110; Flygon, Milotic and Metagross become permanent late Hoenn members.

The loader keeps every evolutionary line within normal level requirements—Wingull before Pelipper, Shroomish before Breloom, Electrike before Manectric, Trapinch/Vibrava before Flygon, and Metang before Metagross. Easy retains no EV/IV investment and no held items; Rustboro uses modest berries in other modes. Later modes gain battle-ready moves, EVs and team variety.

This makes their Kanto teams feel earned. Metagross, Milotic, Breloom, Salamence and Flygon return in Kanto, where the rival expands their collection with Pokémon such as Volcarona and Tyranitar. The Hoenn rival sets do not reuse Jessie's Aegislash, Archaludon, Iron Valiant, Magearna, or other five-mode final-boss species.

## Test before release

Run `python3 tools/brisk/check_hoenn_rivals.py` and the existing Hoenn trainer legality/progression scripts after building. Test a male/female player and each starter choice in each stage, particularly Easy Rustboro Lv12 unevolved starters, Normal Route 110, and both Lilycove parties. Confirm the original Hoenn storyline's defeat flags, reward scenes, battle portraits, and transition into Kanto remain correct on old saves.

**Status:** Source changes are committed; there has not yet been a full ROM build or emulator playthrough of these overrides.
