# Add optional held-stone Mega Mode

**Difficulty:** Advanced. This is **not** normal Mega Evolution: when the preference is enabled, a Pokémon with the appropriate Mega Stone can remain in its Mega form outside battle.

**Implementation:** [`src/brisk_options.c`](../../../src/brisk_options.c), [`src/pokemon.c`](../../../src/pokemon.c), [`src/option_menu.c`](../../../src/option_menu.c), [`include/constants/flags.h`](../../../include/constants/flags.h).  
**Tests:** [`test/brisk_mega_mode.c`](../../../test/brisk_mega_mode.c).

## 1. Require supported forms and stones

Begin with the normal pokeemerald-expansion form-change system and species definitions. Brisk's `GetBriskMegaModeSpecies(species, heldItem)` checks a base species and a held item against the form-change table. It can also recover a base form when a Mega form must revert.

Your fork must have a valid species ID for each Mega form and a matching held-item rule. Do **not** assume every custom Fakémon with "Mega" in its name is already registered as an engine Mega Evolution.

## 2. Add a saved OFF/ON preference

Brisk uses `FLAG_BRISK_MEGA_MODE` and two accessors: `BriskMegaModeEnabled()` and `SetBriskMegaMode(enabled)`. Add a dedicated Options toggle, defaulting OFF in old saves. Audit your fork's flag table before copying the flag ID.

When the player toggles the option, Brisk updates **all owned Pokémon** rather than only the active party:

- Player party via `TryFormChange()`.
- Every PC box slot via `TryBoxMonFormChange()`.
- Daycare Pokémon (including the FRLG daycare where compiled).

The goal is for the party, box summary and future battle data to agree about the current form.

## 3. Hook form-changing events

In [`src/pokemon.c`](../../../src/pokemon.c), `GetFormChangeTargetSpecies()` applies the special `FORM_CHANGE_BRISK_MEGA_MODE` method. While Mega Mode is enabled, it also considers held-item changes, battle start/end, wild encounter start, deposit/withdraw and fainting.

**Why so many hooks?** If you update the form only when the option changes, exchanging a stone, boxing a Pokémon or ending a battle can leave its stored species and battle species inconsistent.

Port the accessor, the method enum, and all relevant callbacks **together**. Ensure disabling the mode correctly reverses a supported Mega form. A stone should never transform an unrelated species.

## 4. Test save compatibility and edge cases

Start with one official Mega line and a matching stone. Test OFF → ON → OFF, giving/removing the stone, boxing/withdrawing, daycare, fainting and battle end. Save/reload after each operation. Then test custom Mega lines and other form systems (Primal, Gigantamax, Ultra Burst, Tera forms).

Read [`test/brisk_mega_mode.c`](../../../test/brisk_mega_mode.c) for the broader species scan and regressions. This is a **high-coupling feature**; prefer adapting the form-change hooks to your engine over copying whole files.
