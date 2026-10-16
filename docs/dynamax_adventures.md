# Dynamax Adventures foundation

The initial backend is callable from C or event scripts. **No map, NPC, warp,
menu entry, or world location invokes it.** It does not use story flags or add
SaveBlock fields.

## Implemented run

- Three distinct level-65 rentals, selected from 16 curated four-move sets.
- Three ordinary encounters with two distinct route choices at each step,
  followed by a level-70 legendary encounter. The final route offers the same
  boss on both branches. Six legendary sets are available initially.
- Four lives. In this solo foundation, losing one battle spends one life,
  heals the rental, and permits a retry of that encounter. Four losses finish
  the run. Running away also finishes it.
- Every opponent automatically Dynamaxes before its first move and remains
  Dynamaxed for the encounter. Rentals may Dynamax for the usual three turns,
  without a Band or world Dynamax flag. Other gimmicks are excluded here.
  Normal battles retain Brisk's existing Dynamax restrictions.
- No battle EXP, bag items (including balls), or Pay Day money. Rentals obey
  regardless of badges and difficulty caps.
- Guaranteed optional catches after victory, without spending bag balls.
  Ordinary catches offer a rental swap. Keeping the existing rental preserves
  its HP, PP, and status; swapping gives a healed version of the encounter.
- Catch shininess rolls at 1/300 when accepted and is hidden on the battle
  opponent and the replacement rental. Reward previews can reveal it.
- At the end, choose exactly one caught Pokémon, including after a failed run.
  It goes to the real party or PC and receives seen/caught Pokédex registration.
  Full party and PC storage returns failure and retains the selectable reward.
  Quitting instead discards all temporary catches.

This version uses **one rental versus one boss**, through the existing wild
battle engine. Four-person teams, AI allies, networking, shields, a shared
team faint counter, a ten-turn limit, route graphics, reward UI, scientists,
backpackers, persistent legendary completion records, and adventure resumption
from a save are future work.

## Party and state safety

The real party remains in place between encounters. `PrepareBattle` backs up
all six party slots and the party count before installing the rental. The
battle return callback restores that exact party before resuming the script,
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

1. `Begin` → RENTAL; inspect offers and `ChooseRental` → PATH.
2. Inspect paths and `ChoosePath` → READY.
3. Start battle → BATTLE; return callback → CATCH on victory, READY for a
   surviving loss, or FINISHED for exhaustion/escape.
4. `Catch(FALSE)` advances directly. `Catch(TRUE)` → SWAP for an ordinary
   encounter; `Swap(TRUE/FALSE)` advances. The final catch decision finishes.
5. FINISHED → inspect caught Pokémon and `Claim(index)`, or `Quit()`.

## Script bridge

`special RunDynamaxAdventure` reads the operation from `VAR_0x8004` and its
argument from `VAR_0x8005`. `VAR_RESULT` reports success/failure.

| Operation | Argument | Result beyond VAR_RESULT |
| --- | --- | --- |
| `DA_OP_BEGIN` | None | Starts a run |
| `DA_OP_STATUS` | None | 8000=phase, 8001=stage, 8002=lives, 8003=catches, 8006=cleared |
| `DA_OP_RENTAL_INFO` | Offer 0–2 | STR_VAR_1=name, 8000=species, 8001=level |
| `DA_OP_CHOOSE_RENTAL` | Offer 0–2 | Selects rental and generates paths |
| `DA_OP_PATH_INFO` | Branch 0–1 | STR_VAR_1=name, 8000=species, 8001=level |
| `DA_OP_CHOOSE_PATH` | Branch 0–1 | Prepares that encounter |
| `DA_OP_START_BATTLE` | None | Launches battle if READY |
| `DA_OP_CATCH` | 0=skip, nonzero=catch | Catch or advance |
| `DA_OP_SWAP` | 0=keep rental, nonzero=swap | Swap or advance |
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
