# Dynamax Adventures foundation

Enter the cave on Route 146 (entrance at 27, 12) and speak to the scientist
in the research chamber to start. The chamber has a return warp to Route 146.
Rental, route, catch, swap, and reward choices use field menus. Press B on a
rental/path menu or decline the next battle to pause; speak to the scientist
to resume. Runs and unclaimed catches are lost on reset. Full storage retains
the reward so you can make room and return before resetting.

The backend remains callable from C or event scripts. It does not use story
flags or add SaveBlock fields.

## Implemented run

- Choose **one rental** from six offers, shown with Pokémon icons. The rival, scientist,
  and Birch independently choose different remaining offers. One researcher
  fights beside the player; the other two are AI relay reserves. The lead
  researcher rotates each round.
  **May partners with a male player; Brendan partners with a female player.**
  Each encounter is a true **two-versus-one** wild multi battle, with exactly
  one opposing Pokémon and no opposing support. Names and trainer back sprites
  use dedicated rival partner identities.
- Rental offers are drawn from the Mega/G-Max-capable subset of **74 curated
  doubles sets**. Rentals, researchers, and intermediate encounters are **level 70**;
  the final legendary is **level 100**. The rival uses Basic + Doubles + Omniscient + Smart Mon Choices
  AI flags, independent of difficulty.
- Every set has four selected moves, an ability from its species' slots, a
  held item, 508 EVs, and a role-based nature. Special-only sets have 0 Attack
  IVs; slow sets have 0 Speed IVs. Other IVs are 31. Mega-capable species carry
  their matching Mega Stone. G-Max-only species carry beneficial items such
  as Life Orb, Sitrus Berry, Light Clay, Focus Sash, or a damage/type booster.
  Rental consumables are replenished between encounters.
- Three route choices offer a **Mega encounter versus a G-Max encounter**.
  The selection menu shows only each battle form's primary type, with no names or
  Pokémon icons. Rental and reward menus still show names and icons.
- The final pool contains **97 canonical legendary/mythical species and
  regional forms**, each with an explicit competitive set. Battle-only and
  duplicate alternate forms are excluded. Ultra Beasts and ordinary Paradox
  species are not classified as legendary by this pool.
- Opponents automatically Mega Evolve when carrying a matching stone, or
  Gigantamax if eligible. Final legendaries without either form use Dynamax,
  preserving the full legendary pool. Boss G-Max/Dynamax remains active for
  the encounter. Ordinary trainer/wild gimmick restrictions are unchanged.
- Both player and rival can use their rental's **Mega or G-Max** form. The
  player activates theirs normally; the rival activates automatically. Each
  participant has an independent gimmick allowance. Player/rival G-Max uses
  the usual three-turn duration; Terastallization remains unavailable.
- Difficulty is captured at the start of the run and scales **all four rental HP
  totals**, including through form changes: Easy 4×, Normal 2×, Hard 1×,
  Extreme ½× (minimum 1 HP). G-Max does not add another HP multiplier on top.
  The real party, offered/caught Pokémon, and final rewards are not scaled.
- Four lives. A whole-team loss spends one life and allows a retry. Four
  losses or running away finish the run. All four rentals recover PP after
  every battle. Easy restores full HP and cures status; Normal restores full
  HP without curing status; Hard adds half max HP; Extreme adds a quarter
  max HP. Healing caps at max HP and revives fainted rentals. Extreme bosses
  deal double direct move damage, including fixed-damage moves.
- No battle EXP, bag items (including balls), or Pay Day money. Rentals obey
  regardless of badges and difficulty caps.
- After victory, the primary encounter is an optional guaranteed catch without
  spending a ball. For ordinary catches, the player can replace their rental. If declined, a randomly chosen researcher
  has a 50% chance to take the encounter instead.
- Every generated rental or encounter rolls shiny odds once at **1/100**.
  That same roll survives battling, catching, and replacing a rental; no extra
  catch-time reroll occurs. Adventure odds are independent of the Options
  shiny setting. Reward labels identify Shiny/Regular catches.
- At the end, choose exactly one caught Pokémon, including after a failed run.
  It goes to the real party or PC with seen/caught Pokédex registration.
  Full party and PC storage retains the reward until space becomes available.
  Quitting instead discards all temporary catches.

This is a **two-versus-one** backend with an AI rival. Simultaneous four-allies-versus-one combat,
networking, shields, individual-faint life counting, a ten-turn limit, route graphics,
backpackers, legendary completion records, and adventure
resumption from a save remain future work.

Intermediate enemies have **5× their normal max HP**; the final boss has
**7×**. These replace the ordinary Dynamax HP multiplier rather than stacking
with it. Catches retain their normal stats; rental HP uses the difficulty rule above. Healthboxes use doubles
geometry throughout, matching the allocated graphics for the two-versus-one
battle. If the player rental faints, the rival continues against the boss;
a loss is counted only when neither player nor rival has a battle-ready rental.

After every battle (victory, loss, or escape), the caller moves to an enclosed
placeholder cave with the scientist, the correct rival, and Professor Birch. It has no
warp events, no doorway, and disallows Escape Rope/Dig. The scientist continues
catch, swap, and path choices there. Finishing, deferring a reward, or taking a
break returns the player to the Route 146 research cave. The opening-room rival explains the research behind the expeditions and their
goal of becoming a professor. The initial scientist
hint reveals only the final boss's primary type; the final path menu does not
name or show an icon of that legendary before battle.

Pool definitions are in `src/data/dynamax_adventure.h`. The C pool-query
functions expose the exact active species lists for a future UI or tooling.

## Party and state safety

The real party remains in place between encounters. `PrepareBattle` backs up
all six party slots and the party count before installing the player's rental.
The three researcher rentals live in the partner party, with a stable ownership mapping. The previous partner party,
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

1. `Begin` → RENTAL; `ChooseRental` picks the player rental, automatically
   gives the rival a distinct offer, and advances to PATH.
2. Inspect paths and `ChoosePath` → READY.
3. Start battle → BATTLE; return callback → CATCH on victory, READY for a
   surviving loss, or FINISHED for exhaustion/escape.
4. `Catch(FALSE)` advances directly. `Catch(TRUE)` → SWAP for an ordinary
   encounter; `SwapRental(slot)` advances, replacing only slot 0 or keeping the player rental with
   `DA_RENTAL_KEEP`. The compatibility `Swap(TRUE/FALSE)` replaces slot 0 or
   keeps the player rental. The final catch decision finishes.
5. FINISHED → inspect caught Pokémon and `Claim(index)`, or `Quit()`.

## Script bridge

`special RunDynamaxAdventure` reads the operation from `VAR_0x8004` and its
argument from `VAR_0x8005`. `VAR_RESULT` reports success/failure.

| Operation | Argument | Result beyond VAR_RESULT |
| --- | --- | --- |
| `DA_OP_BEGIN` | None | Starts a run |
| `DA_OP_STATUS` | None | 8000=phase, 8001=stage, 8002=lives, 8003=catches, 8006=cleared, 8007=rentals chosen |
| `DA_OP_RENTAL_INFO` | Offer 0–2 | STR_VAR_1=name, 8000=species, 8001=level |
| `DA_OP_CHOOSE_RENTAL` | Offer 0–2 | Selects player rental, assigns rival, generates paths |
| `DA_OP_PATH_INFO` | Branch 0–1 | STR_VAR_1=name, 8000=species, 8001=level |
| `DA_OP_CHOOSE_PATH` | Branch 0–1 | Prepares that encounter |
| `DA_OP_START_BATTLE` | None | Launches battle if READY |
| `DA_OP_CATCH` | 0=skip, nonzero=catch | Catch or advance |
| `DA_OP_SWAP` | 0=keep player rental, 1=replace player rental (other values rejected) | Swap or advance |
| `DA_OP_REWARD_INFO` | Caught index 0–count-1 | Name/species/level, STR_VAR_2=Shiny/Regular, only when FINISHED |
| `DA_OP_CLAIM` | Caught index | Gives one reward and closes run |
| `DA_OP_QUIT` | None | Discards the run outside battle |
| `DA_OP_BOSS_HINT` | None | STR_VAR_1=final boss primary type |
| `DA_OP_IN_REST_CAVE` | None | VAR_RESULT=whether player is in the staging cave |

Only `DA_OP_START_BATTLE` is asynchronous. The caller must `waitstate` after a
successful launch and then query status to decide what to display next. The installed caller is
`data/maps/DynamaxAdventureCave/scripts.inc`; a minimal battle example:

```asm
    setvar VAR_0x8004, DA_OP_START_BATTLE
    special RunDynamaxAdventure
    goto_if_eq VAR_RESULT, FALSE, YourCaller_InvalidState
    waitstate
    setvar VAR_0x8004, DA_OP_STATUS
    special RunDynamaxAdventure
    // Display catch/retry/end choices based on VAR_0x8000.
```

The scientist script supplies the rental, path, catch, swap, and reward menus
using these operations.

## Dormant four-player foundation

`include/dynamax_adventure_coop.h` and `src/dynamax_adventure_coop.c` define an
isolated four-player session with four rental snapshots, one shared boss,
per-player move/target submissions, duplicate/stale-round rejection, and a
barrier that collects one action from every living player. Fainted slots are
excluded from the barrier. The caller supplies storage; no persistent global
state is allocated.

`DA_COOP_AVAILABLE` is FALSE. Nothing in map scripts, menus, battle setup,
or networking invokes this layer. It does not execute damage or provide a
playable four-player fight. A future adapter must extend the four-battler
engine for four allies plus a boss, execute the collected actions, and add
transport, ownership/authentication, and disconnect handling before enabling it.

## Four-person combat limitation

The live expedition has four participants but still uses the engine's two
allied battle positions. It is a relay, not simultaneous 4v1 combat. The dormant
`dynamax_adventure_coop` command collector remains separate and unavailable;
it does not execute a five-battler battle. Supporting four allies on the field
requires expanding battler positions, targeting, controllers, animations, and
healthbox layouts beyond the engine's four total slots.

Defensive curated sets with fewer than three attacks are filled using Brisk's
legal move scorer (offensive stats, STAB, coverage, accuracy and secondary
effects). Existing sets with three attacks retain their curated moves.
