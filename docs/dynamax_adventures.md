# Dynamax Adventures foundation

The initial backend is callable from C or event scripts. **No map, NPC, warp,
menu entry, or world location invokes it.** It does not use story flags or add
SaveBlock fields.

## Implemented run

- Choose **two distinct rentals** from three offers. The player controls the
  first; their rival controls the second through the normal partner AI.
  **May partners with a male player; Brendan partners with a female player.**
  Each encounter is a true **two-versus-one** wild multi battle, with exactly
  one opposing Pokémon and no opposing support. Names and trainer back sprites
  use dedicated rival partner identities.
- **48 shared rental/regular-encounter sets**, retaining the original species
  and adding competitive attackers and doubles supports. All rentals, regular
  encounters, rival rentals, and legendary bosses are **level 100**.
- Curated four-move doubles sets include Fake Out, redirection, Tailwind,
  screens, weather/terrain, priority, recovery, and Protect. Rentals have chosen
  species abilities, 31 IVs, 508 EVs, and matching offensive/support natures.
  These preparations are independent of the selected difficulty mode.
- Three ordinary encounters offer two distinct primary route choices each.
  The fourth encounter is a legendary boss, identical on both final branches.
- The final pool currently contains **97 species**, derived from all enabled species marked restricted
  legendary, sub-legendary, or mythical. It includes every canonical species
  in those categories plus regional forms, including the Galarian birds.
  Battle-only Mega, Primal, Ultra Burst, Gigantamax, Tera, and duplicate
  alternate forms are excluded. Ultra Beasts and ordinary Paradox species
  are not classified as legendary by this pool.
- Legendary moves and abilities use the existing strong loadout scorer without
  changing difficulty or global flags. Passive Cosmog/Cosmoem get an explicit
  Psychic/Shadow Ball/Light Screen/Protect boss set so every entry can fight.
- The sole opponent automatically **Gigantamaxes when its species has an
  enabled G-Max form**, or Dynamaxes otherwise, and keeps that transformation
  for the encounter. This also applies to bosses such as Zacian, Zamazenta,
  and Eternatus through an adventure-only exception to their ordinary ban.
- **Both the player and rival rentals remain regular Pokémon.** They cannot
  Dynamax, Gigantamax, Mega Evolve, or Terastallize during an adventure. Boss
  G-Max eligibility is installed only on the temporary enemy copy; catching
  and swapping that species still supplies its regular form. Other battle
  modes retain their existing gimmick restrictions.
- Four lives. A whole-team loss spends one life, heals both rentals, and allows
  a retry. Four losses or running away finish the run. A partner that faints
  during a victory is revived at half HP for the next double battle. Other
  surviving rental HP, PP, and status persist.
- No battle EXP, bag items (including balls), or Pay Day money. Rentals obey
  regardless of badges and difficulty caps.
- After victory, the primary encounter is an optional guaranteed catch without
  spending a ball. For ordinary catches, either the player's or rival's rental
  can be replaced with a healed version of the encounter.
- Catch shininess rolls at 1/300 when accepted and remains hidden on battle
  opponents and replacement rentals. Reward previews can reveal it.
- At the end, choose exactly one caught Pokémon, including after a failed run.
  It goes to the real party or PC with seen/caught Pokédex registration.
  Full party and PC storage retains the reward until space becomes available.
  Quitting instead discards all temporary catches.

This is a **two-versus-one** backend with an AI rival. Four-person teams,
networking, shields, individual-faint life counting, a ten-turn limit, route graphics,
menus, scientists, backpackers, legendary completion records, and adventure
resumption from a save remain future work. No world entrance is installed.

Pool definitions are in `src/data/dynamax_adventure.h`. The C pool-query
functions expose the exact active species lists for a future UI or tooling.

## Party and state safety

The real party remains in place between encounters. `PrepareBattle` backs up
all six party slots and the party count before installing the player's rental.
The rival rental lives in its own partner party. The previous partner party,
count, trainer identity, and opponent context are also backed up, and ordinary
follower setup cannot overwrite this temporary rival.
The battle return callback restores those parties and identities before resuming the script,
including on defeat or escape; it does not white out the player. A second
battle preparation, reward claim, or quit is rejected during battle.

Run data lives in dedicated EWRAM, because ordinary field/battle transitions
reset the heap. It is transient and is discarded by a reset. Normal saving is
rejected while the rental party is installed. Saving between encounters saves
the real party, but does not persist the run.

## C interface

See `include/dynamax_adventure.h` and `include/constants/dynamax_adventure.h`.
`DynamaxAdventure_Begin(seed)` creates a deterministic route/rental sequence.
The script entry uses a random seed. Guarded actions return `FALSE` for an
invalid phase or out-of-range choice and leave the run unchanged.

`GetRental`, `GetPath`, and `GetCaught` return borrowed views; do not modify or
retain these pointers across actions. Their types are mutable because this
engine's `GetMonData` decrypts/re-encrypts the Pokémon while reading it.
`GetStatus` returns a value copy. Choice and stage indices start at zero.

Use `BattleSetup_StartDynamaxAdventureBattle()` to launch an actual battle.
`PrepareBattle` and `FinishBattle` are the lower-level adapter methods and are
also used by the tests; calling `PrepareBattle` alone does not launch a battle.

Lifecycle:

1. `Begin` → RENTAL; choose two different offers with `ChooseRental`. The
   first selection remains in RENTAL; the second advances to PATH.
2. Inspect paths and `ChoosePath` → READY.
3. Start battle → BATTLE; return callback → CATCH on victory, READY for a
   surviving loss, or FINISHED for exhaustion/escape.
4. `Catch(FALSE)` advances directly. `Catch(TRUE)` → SWAP for an ordinary
   encounter; `SwapRental(slot)` advances, replacing slot 0/1 or keeping both with
   `DA_RENTAL_KEEP`. The compatibility `Swap(TRUE/FALSE)` replaces slot 0 or
   keeps both. The final catch decision finishes.
5. FINISHED → inspect caught Pokémon and `Claim(index)`, or `Quit()`.

## Script bridge

`special RunDynamaxAdventure` reads the operation from `VAR_0x8004` and its
argument from `VAR_0x8005`. `VAR_RESULT` reports success/failure.

| Operation | Argument | Result beyond VAR_RESULT |
| --- | --- | --- |
| `DA_OP_BEGIN` | None | Starts a run |
| `DA_OP_STATUS` | None | 8000=phase, 8001=stage, 8002=lives, 8003=catches, 8006=cleared, 8007=rentals chosen |
| `DA_OP_RENTAL_INFO` | Offer 0–2 | STR_VAR_1=name, 8000=species, 8001=level |
| `DA_OP_CHOOSE_RENTAL` | Offer 0–2 | Selects one rental; the second pick generates paths |
| `DA_OP_PATH_INFO` | Branch 0–1 | STR_VAR_1=name, 8000=species, 8001=level |
| `DA_OP_CHOOSE_PATH` | Branch 0–1 | Prepares that encounter |
| `DA_OP_START_BATTLE` | None | Launches battle if READY |
| `DA_OP_CATCH` | 0=skip, nonzero=catch | Catch or advance |
| `DA_OP_SWAP` | 0=keep both, 1=replace player rental, 2=replace rival rental | Swap or advance |
| `DA_OP_REWARD_INFO` | Caught index 0–count-1 | Name/species/level, only when FINISHED |
| `DA_OP_CLAIM` | Caught index | Gives one reward and closes run |
| `DA_OP_QUIT` | None | Discards the run outside battle |

Only `DA_OP_START_BATTLE` is asynchronous. The caller must `waitstate` after a
successful launch and then query status to decide what to display next. An
example **for a future caller**, deliberately not installed in the game:

```asm
    setvar VAR_0x8004, DA_OP_START_BATTLE
    special RunDynamaxAdventure
    goto_if_eq VAR_RESULT, FALSE, YourCaller_InvalidState
    waitstate
    setvar VAR_0x8004, DA_OP_STATUS
    special RunDynamaxAdventure
    // Display catch/retry/end choices based on VAR_0x8000.
```

No operation opens a rental, path, catch, or reward menu. A future UI supplies
those choices using these operations.
