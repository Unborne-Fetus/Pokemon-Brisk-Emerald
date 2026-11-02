# Brisk Emerald: trainer and wild encounter rebalance (2026-11-02)

## Scope

This pass checks all currently authored Hoenn and postgame Kanto trainer
identities in Easy, Normal, Expert, Hard and Kaizo. Expert uses a compact
per-trainer replacement plan so five different teams remain feasible inside
the 32MB GBA ROM limit. The original four mode team sets remain authored.

- Run `python3 tools/brisk/build_compact_expert.py --write` to generate the
  compact Expert patch used by battle_setup.c.
- Run `python3 tools/brisk/check_all_mode_balance.py` to audit every mode
  for missing rosters, invalid party sizes, severe mode-level reversals,
  the Lv90-100 Kanto postgame requirement, and repeated species.
- Run `python3 tools/brisk/check_trainer_legality.py`,
  `python3 tools/brisk/check_trainer_progression.py`, and the ROM's tests to
  verify moves, evolution legality and badge-appropriate progression.
  These checks complement, but cannot substitute for, playtesting each battle.

The protected dedication/Easter egg parties and Champions Archive are not
rewritten by the compact Expert patch. Bosses with fixed progression remain
protected.

## Wild Pokémon

`tools/brisk/biome_wild_encounters.py` applies habitat corrections to the
compiled `src/data/wild_encounters.h` *in memory*. The large canonical
`wild_encounters.json` remains unchanged, and is still the source for
Porymap and future manual authoring. The resulting **ROM** uses the corrected
tables; do not inspect the unprocessed JSON as if it were the active ROM table.

The first build audit reported:
- 51 inappropriate water-dependent walking slots replaced (e.g. Corsola).
- 160 early-route land slots adjusted to stage-appropriate habitat species.
- 56 relocations to preserve existing Emerald wild species accessibility.
- All 945 previously appearing wild species retained; 949 after the pass.
- Five additional mainland Hoenn wild Fakémon placements.

| Fakémon | Hoenn habitat | Encounter |
| --- | --- | --- |
| Sproutsy | Route 104 | Walking |
| Shaggot | Petalburg Woods | Walking |
| Budchirp | Route 116 | Walking |
| Chubboink | Route 117 | Walking |
| Goobolt | Route 110 | Walking |

The existing 114 Mikitari species/form placements in Kanto stay intact.
Selected ordinary postgame Kanto trainer classes can also showcase
battle-ready Mikitari Fakémon: Beelitant (Bug Catcher), Glyptoblock (Hiker),
Ravendler (Bird Keeper), Basstile (Swimmer/Fisherman), Lediastra
(Fighting specialists), and Platerigus (Hex Maniac). Only a subset of the
eligible ordinary NPCs are selected, keeping gym leaders and story bosses
untouched. Creator credits remain in the original import documentation.

### Safety invariants

- No new `SPECIES_NONE` active rolls.
- Existing Emerald wild species remain represented in some walk/Surf/fish table.
- The number of encounter slots on each map is unchanged.
- Fully aquatic species do not appear in walking encounter slots.
- Route 101 and its neighbouring early routes draw from suitable low-stage
  species pools, not late-game fully evolved water dwellers.
- Fakémon already placed in Kanto are not silently overwritten.
- No change is made to the player's save format.

## Validation state

The initial habitat compile revealed a misspelled `FLAAFFY` constant. This was
corrected and a compiler-independent species validation was added. The
five-mode table had also exceeded the ROM by about 129 KB; compact Expert
party deltas replace the oversize fifth table. The updated ROM build and
CI must pass before announcing this batch fully complete. Human balance
playtests are still needed for unusual terrain, opponent strategies and
game progression.
