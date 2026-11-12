# Add Let's Go-style follower auto-battles

**Difficulty:** Advanced. **Requires:** functioning player Pokémon followers and generated visible overworld encounters. Unlike ordinary encounters, these interactions never open the battle screen.

**Implementation:** [`src/wild_encounter_ow.c`](../../../src/wild_encounter_ow.c), [`include/wild_encounter_ow.h`](../../../include/wild_encounter_ow.h), [`src/overworld.c`](../../../src/overworld.c), [`src/brisk_options.c`](../../../src/brisk_options.c), [`src/option_menu.c`](../../../src/option_menu.c).  
**Tests:** [`test/brisk_encounter_balance.c`](../../../test/brisk_encounter_balance.c).

## 1. Enable the upstream foundations

- Read [visible overworld wild encounters](../how_to_overworld_wild_encounters.md) and enable `WE_OW_ENCOUNTERS` in [`include/config/wild_encounter.h`](../../../include/config/wild_encounter.h).
- Enable `OW_POKEMON_OBJECT_EVENTS` and `OW_FOLLOWERS_ENABLED` in [`include/config/overworld.h`](../../../include/config/overworld.h).
- Confirm [`GetFollowerObject()` and `GetFirstLiveMon()`](../../../src/event_object_movement.c) return the **player's Pokémon follower**, not a scripted partner NPC.
- Ensure the follower's displayed object is present and not hidden. Manually placed encounter objects are **not** automatically eligible.

## 2. Give players a persistent choice

Brisk defines `BRISK_AUTOBATTLE_OFF`, `BRISK_AUTOBATTLE_SEND` and `BRISK_AUTOBATTLE_PASSIVE` in [`include/brisk_options.h`](../../../include/brisk_options.h). Options page 7 lets the player select them. SEND mode uses **R+B** to toggle training; PASSIVE trains while the player stands still.

Instead of adding a SaveBlock field, Brisk packs the mode into bits 1–2 of its *existing* overworld-encounter preference. Bit 0 still means encounters OFF:

~~~c
u32 mode = (VarGet(VAR_BRISK_OW_ENCOUNTERS_DISABLED) >> 1) & 3;
// Keep the old encounter-toggle bit unchanged:
VarSet(VAR_BRISK_OW_ENCOUNTERS_DISABLED,
       (VarGet(VAR_BRISK_OW_ENCOUNTERS_DISABLED) & 1) | (mode << 1));
~~~

**Do not reuse this variable blindly.** Pick an unallocated variable/bit range in your project. Old saves using 0/1 load with auto-battles OFF.

## 3. Hook into the field loop

Brisk's [`OverworldBasic()`](../../../src/overworld.c) calls `UpdateOverworldWildEncounter()` followed by `BriskAutoBattle_Update()` once per field frame. Its overworld input handler calls `TryBriskAutoBattleInput(newKeys, heldKeys)` before normal player input. Use your fork's equivalent hook and **do not** run this during battle, menus, or link play.

The update function resets its queued attack on map changes and pauses for locked field controls, scripts, palette fades, DexNav, surfing, absent followers, low HP and capped Pokémon. A short windup and cooldown avoid instant repetitive knockouts.

## 4. Filter wild targets before rewarding EXP

Only [generated](../how_to_overworld_wild_encounters.md) `OWE_CATEGORY_WILD` encounters may be auto-defeated. Explicitly reject manual/story encounters, roamers, outbreaks, non-despawning spawns, shiny sprites, invalid species and encounters where the follower is too weak.

Brisk's mandatory level-difference rule is:

~~~c
static bool32 BriskAutoBattleLevelAllowed(u32 followerLevel, u32 wildLevel)
{
    return wildLevel <= followerLevel + 7;
}
~~~

A level-20 follower may defeat a level-27 wild Pokémon, **not** a level-28 one. Check eligibility again **after the attack windup**, not only at target selection; the object might have disappeared or changed. Brisk also checks the selected object's ID, local ID and species before removing it.

## 5. Award EXP without entering normal battle

The [`BriskAutoBattleReward()`](../../../src/wild_encounter_ow.c) implementation:

1. Computes reduced EXP using the wild species' yield and level.
2. Applies [`GetSoftLevelCapExpValue()`](../../../src/caps.c).
3. Clamps the total to the current cap's EXP threshold in `gExperienceTables`.
4. Writes `MON_DATA_EXP`, increments levels with `TryIncrementMonLevel()` and recalculates stats.
5. Learns level-up moves when a free slot exists; it **never silently replaces** a full move set.

Only after a successful reward does it `RemoveObjectEvent(owe)`. This route deliberately awards **no EVs** and does **not** trigger normal catch, evolution or full battle outcome processing. If you want those, design and test explicit handling instead of invoking regular battle callbacks from this loop.

## 6. Verify

- OFF / SEND / PASSIVE; R+B send/recall; changing maps.
- Follower absent, hidden, fainted, low HP and at the current EXP cap.
- Generated normal, shiny, roaming, mass-outbreak and scripted Pokémon.
- A 7-level disadvantage is eligible; 8 or more is blocked.
- No double EXP, duplicate knockout, enemy-party corruption or battle transition after a despawn.
- Save/reload preserves options without changing the legacy OW encounter toggle.

Run [`test/brisk_encounter_balance.c`](../../../test/brisk_encounter_balance.c) and manually test in an emulator. A build passing is necessary but not sufficient. For full details see [Brisk gameplay options](../../brisk/gameplay-options.md).
