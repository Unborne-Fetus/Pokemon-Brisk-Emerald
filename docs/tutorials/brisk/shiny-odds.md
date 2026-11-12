# Add an adjustable shiny-odds setting

**Difficulty:** Intermediate. The setting must affect **the actual shiny check**, not only a displayed menu label.

**Implementation:** [`src/brisk_options.c`](../../../src/brisk_options.c), [`include/brisk_options.h`](../../../include/brisk_options.h), [`src/pokemon.c`](../../../src/pokemon.c), [`src/option_menu.c`](../../../src/option_menu.c), [`include/constants/vars.h`](../../../include/constants/vars.h).

## 1. Choose your menu values

Brisk offers `1/8192`, `1/4096`, `1/2048`, `1/1024`, `1/512`, `1/256`, `1/128`, `1/100` and `1/50`. The menu stores an **index**, not the denominator, in `VAR_BRISK_SHINY_ODDS`.

~~~c
static const u16 odds[] = {8192, 4096, 2048, 1024, 512, 256, 128, 100, 50};
return setting >= 1 && setting <= ARRAY_COUNT(odds) ? odds[setting - 1] : 512;
~~~

The default matters: zero in an old save means Brisk's previous `1/512` baseline. Your fork may need to default to different odds. Preserve your original save behavior before adding the menu.

## 2. Add an options control

Follow the `SHINY ODDS` selection flow in [`src/option_menu.c`](../../../src/option_menu.c): read saved selection, clamp it to the available range, display the current denominator, respond to Left/Right, then save the selected index. If your menu already fills all 16 task `data[]` slots, add storage safely or use a separate persistent variable. Do **not** blindly copy the Brisk task layout.

## 3. Connect the actual shiny roll

Brisk's [`GetBriskShinyThreshold()`](../../../src/brisk_options.c) translates the denominator into the engine's shiny-value threshold:

~~~c
return (65536 + denominator / 2) / denominator;
~~~

The shiny comparison and reroll logic in [`src/pokemon.c`](../../../src/pokemon.c) must use this threshold consistently. Search for every path that creates a shiny Pokémon; encounters from gifts, static scripts, DexNav, eggs or alternate battle modes may have intentional overrides.

**Do not reroll the personality/IVs of a Pokémon simply because the player changes the option.** Existing Pokémon should retain their shiny state.

## 4. Keep special encounter odds scoped

Brisk uses `BriskSetWildShinyContext()` and `GetBriskWildShinyDenominator()` to apply an optional `1/50` boost only to selected outdoor Orange Islands routes. Separate this **wild-only map exception** from the user's global shiny setting; indoor caves and dedicated battle systems retain their own rules.

In another ROM hack, use your actual map-section constants. Never assume Brisk's Orange Islands map IDs exist in your fork.

## 5. Test the probabilities and persistence

- Verify index 0, 1, the last index, and invalid/corrupted index behavior.
- Confirm the threshold is monotonic: rarer settings should not produce more shinies.
- Exercise overworld spawns, ordinary grass encounters, static encounters and your other Pokémon creation paths.
- Save/reload and ensure changing odds never retroactively changes already-owned Pokémon.
- Run a large **simulation of the pure threshold calculation** separately from the emulator; random samples will vary.

See [Brisk's gameplay options](../../brisk/gameplay-options.md) for intended player-facing behavior.
