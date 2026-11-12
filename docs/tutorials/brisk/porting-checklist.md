# Before porting a Brisk Emerald feature

Brisk Emerald is **not a library** that can be dropped into every Pokémon hack. Its gameplay systems have dependencies on upstream pokeemerald-expansion, custom settings, and changes made over time. Follow this process for **each** feature.

## 1. Prepare a separate branch and a working baseline

1. Install a build environment using the repository's [installation guide](../../INSTALL.md). Have the original engine build working **before** changing gameplay code.
2. Note the exact source commit of your fork and the Brisk revision you want to study. Brisk's base is pokeemerald-expansion 1.17.0; not every API exists in older versions.
3. Create a branch (`git switch -c feature/my-brisk-port`), keep a clean baseline ROM, and back up a test save.
4. Identify every feature-specific function and its callers. GitHub's source search is useful; do not paste an entire `src/*.c` over your own fork.

For example, to find the caller and implementation of auto-battles:

~~~sh
git grep -n 'BriskAutoBattle_Update'
git grep -n 'TryBriskAutoBattleInput'
~~~

## 2. Classify what must be ported

| Concern | Typical places to inspect |
| --- | --- |
| Public API | `include/*.h` |
| Implementation | `src/*.c` |
| In-game options | [`src/option_menu.c`](../../../src/option_menu.c) |
| Save flags and variables | [`include/constants/flags.h`](../../../include/constants/flags.h), [`include/constants/vars.h`](../../../include/constants/vars.h) |
| Battle hooks | `src/battle_*.c` and `data/battle_scripts_*.s` |
| Encounter and field hooks | `src/overworld.c`, `src/wild_encounter_ow.c` |
| Config toggles | `include/config/*.h` |
| Tests | `test/*.c` |
| Documentation / attribution | `docs/*.md`, [`CREDITS.md`](../../../CREDITS.md) |

**Save IDs are not globally unused.** An unused variable or flag in Brisk may already be used by your fork. Search **both** codebases before allocating one; preserve existing values and default new features OFF for older saves.

## 3. Integrate in small changes

1. Add constants and function declarations.
2. Add the implementation, guarding conditions and error handling.
3. Connect the relevant field/battle/menu hook.
4. Add persistence only after checking save layout and ID collisions.
5. Add automated tests and update the help/Options page.
6. Compile before making further changes. If merging a feature changes Pokémon data layout, make a migration plan rather than assuming older saves will work.

Prefer copying a **small, auditable algorithm** to copying a whole source file. Some features rely on new structs, custom species, or custom engine patches; those require a porting project rather than simple code snippets.

## 4. Test both behavior and safety

~~~sh
make -j$(nproc)
# After configuring the project's supported test toolchain:
make check -j
~~~

Use the existing [testing-system tutorial](../how_to_testing_system.md) for test-runner setup; targets vary by fork. Then test the built ROM in a GBA emulator with a new save and a backed-up existing save.

At minimum: turn the option ON and OFF, change maps, enter/leave battles, save/reload, use a full party and full PC, reach level 100, and exercise exceptional encounters. Check normal gameplay with the feature OFF.

**ROM/build success does not prove emulator correctness.** Do not release a feature as validated without running it in-game.

## 5. Respect creators

The engine and documentation have their own licensing/credit obligations, and sprite/music/Fakémon packs may have different terms. Follow the repository's [credits](../../../CREDITS.md), upstream licenses, and the individual asset creator's permission. Avoid distributing prebuilt commercial game ROMs.
