# Add multiple difficulty modes and level caps

**Difficulty:** Advanced. The basic switcher is straightforward; **creating a fifth trainer roster without exceeding GBA ROM space is not**.

**Implementation:** [`src/brisk_modes.c`](../../../src/brisk_modes.c), [`src/brisk_difficulty.c`](../../../src/brisk_difficulty.c), [`src/trainer_util.c`](../../../src/trainer_util.c), [`src/caps.c`](../../../src/caps.c), [`src/option_menu.c`](../../../src/option_menu.c).  
**Tests:** [`test/brisk_mode_targets.c`](../../../test/brisk_mode_targets.c), [`test/brisk_expert_mode.c`](../../../test/brisk_expert_mode.c), [`test/brisk_difficulty_balance.c`](../../../test/brisk_difficulty_balance.c).

## 1. Keep mode IDs stable

Brisk's saved IDs are **Normal=0, Easy=1, Hard=2, Kaizo=3, Expert=4**. The visible menu order is **Easy → Normal → Expert → Hard → Kaizo**. Never renumber existing saved IDs simply to change the display order.

The selected mode lives in `VAR_BRISK_DIFFICULTY`. [`GetBriskDifficulty()`](../../../src/brisk_modes.c) validates the value and falls back to Normal for invalid saves. When porting, reserve a free variable in your fork, select a safe default for old saves, and have every system consume a single validated accessor.

## 2. Separate the knobs

Do not equate "difficulty" with only higher levels. Brisk changes:

- Trainer party size and available members in [`src/brisk_difficulty.c`](../../../src/brisk_difficulty.c).
- IVs, EVs and dynamically generated trainer composition in [`src/trainer_util.c`](../../../src/trainer_util.c).
- AI and rules via [`src/brisk_modes.c`](../../../src/brisk_modes.c) and battle hooks.
- Shared EXP rates: Easy x2, Hard x0.5 (rounded up), Kaizo 0; Normal/Expert unchanged.
- Prices, battle items and battle style in their respective callers.

For portability, begin with only one adjustment (for example trainer size), test it, and then add AI/EXP/economy rules separately.

## 3. Implement progression-based caps

Brisk centralizes enforcement in [`src/caps.c`](../../../src/caps.c), instead of making every EXP source guess:

~~~c
bool32 BriskUsesExpCap(void)
{
    u32 mode = GetBriskDifficulty();
    if (mode == BRISK_MODE_EASY)
        return FALSE;
    return mode == BRISK_MODE_EXTREME || !VarGet(VAR_BRISK_NO_LEVEL_CAP);
}
~~~

Normal, Expert and Hard honor the player's cap preference; Easy is uncapped; Kaizo always enforces the cap. `GetProgressionLevelCap()` reads badge/story flags. `GetCurrentLevelCap()` returns the effective cap. Call `GetSoftLevelCapExpValue()` from EXP-awarding paths and check cap logic from level-up and candy/daycare paths.

**Do not only block the level counter.** EXP, item use, daycare, shared EXP and auto-battles should converge on the same threshold.

## 4. Add a fifth mode without a fifth full trainer table

Brisk's [Expert-mode design](../../brisk_expert_mode.md) explains why a full fifth table was too large for the 32 MB GBA ROM budget. It uses a **compact authored per-trainer substitution** on the existing rosters instead. Study the actual team selector and its data generation before attempting to copy the strategy; a fifth enum alone does not create fifth-mode parties.

To port incrementally, first make every switch statement handle your new mode, then use your existing trainer data for a temporary fallback, then add explicit per-trainer differences. Test major trainers, wild battles, special/double battles and level-100 postgame encounters.

## 5. Tests and edge cases

Use the cited tests to verify the save value, cap preference, badge transitions and difficulty behavior. In an emulator, check a Pokémon one EXP away from the cap, a Pokémon already above it, Rare Candy/EXP Candy, daycare EXP, shared EXP, rematches and a freshly loaded save. Ensure turning caps OFF works only in modes where it is intended.

**Avoid copied team data without verifying species IDs and trainer IDs:** expansion versions and other hacks do not necessarily share the same generated tables.
