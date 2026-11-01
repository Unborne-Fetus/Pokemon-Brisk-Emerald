# Brisk Evolution AI (BEAI) — design and foundation

Status: **experimental**. This branch starts the work without modifying the game engine. An external Python proof of concept has been developed and locally tested; its simplified duel results are **not valid Brisk Emerald win rates**.

## Agreed evolutionary configuration

- 100 candidate policies per generation
- 1,400 generations = 140,000 agent evaluations
- 10% elitism = exactly 10 unchanged survivors and 90 offspring each generation
- Twelve seeded training episodes per candidate at the initial smoke-test stage
- Holdout evaluations every 25 generations; stable checkpoints with full population and RNG state every 10 generations
- Approximate new-generation distribution: 54 mutated descendants, 27 crossovers, 9 immigrants
- Champion selection must use reproducible scenarios and a separate never-trained-on benchmark

## Architecture

1. **Fast native battle backend.** Reuse Brisk Emerald/pokeemerald-expansion's actual battle logic and build-specific species, moves, items, trainers, abilities, and difficulty settings. The existing `test/test_runner_battle.c` and `include/test/battle.h` are useful foundations. Do not claim a hand-written toy battle model is exact.
2. **Evolution runner.** A fixed-size population of small scored-action policies is evaluated across diversified scenario seeds. Preserve elites; mutate/crossover/introduce random variants; checkpoint every generation interval.
3. **Real-ROM validation.** mGBA 0.10+ supports Lua scripting to read memory, send controller input, examine frame state and load savestates. Use it in a separate worker that loads a *matching* GBA build to replay policy decisions and compare against the native simulator.
4. **Parity gate.** Record same actions/seeds in native simulation and ROM; compare actual HP/status, move order, substitutions, item/ability outcomes and other battle mechanics. Do not promote simulation-only champions when parity fails.
5. **Full game completion later.** A battle-trained policy does not provide route navigation, item collection, story progression, long-term memory or Pokémon acquisition.

## Required invariants

- ROM/build hash, policy genome, seed, and difficulty mode recorded with every replay.
- Do not leak unobserved opponent information in Fair Play mode; optional separate omniscient testing mode.
- Legal-action masking to prevent nonexistent moves/switches/items.
- No save-state resets during an official fair-play completion attempt, although save-state snapshots are allowed for training/validation.
- Never mark a battle or game beaten without a real ROM trace and independently verified outcome.

## Initial milestone

A working evolutionary-loop smoke test followed by a verified bidirectional connection to mGBA. Those are prerequisites; full Brisk battle parity and autonomous gameplay are not yet implemented.

## Phases

P0: run evolution with deterministic toy combat to verify selection, checkpoint and resume logic.
P1: create host-native battle runner from exact Brisk game state.
P2: parity and mGBA integration.
P3: competitive battle training across all user-facing difficulty modes.
P4: team planning/navigation/full-game completion.
