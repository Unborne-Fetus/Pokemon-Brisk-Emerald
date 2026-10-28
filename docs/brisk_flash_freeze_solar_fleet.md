# Brisk Emerald: Flash Freeze and Solar Fleet

## Delibird — Flash Freeze (signature Ability)

**First ability slot:** Flash Freeze replaces Vital Spirit; Hustle and hidden Insomnia remain available.

- Whenever Delibird lands a *damaging Ice-type move*, the hit has exactly a **40% chance to freeze** the target.
- The 40% replaces the existing 10% freeze/frostbite chance on moves such as Ice Beam and Blizzard. It is not an additional roll.
- Damaging Ice moves that have no built-in freeze chance, such as Ice Shard, gain a 40% freeze roll.
- A Flash Freeze Ice hit results in **freeze**, not frostbite, even when the game is configured to use frostbite for ordinary moves.
- When Delibird attacks an **already frozen** target, the attack deals **2× damage**. The multiplier is applied before Sturdy/Endure/Focus Sash processing and is included in AI damage calculation.
- Normal immunity and protection rules apply (e.g., already statused Pokémon, Ice-type freeze immunity, sunlight freeze prevention, Substitute).
- Applies to damaging attacks only, not Ice-type status moves such as Haze or Snowscape. It does not double damage dealt by other party members.
- In Double Battles each affected target rolls independently.

## Sunflora — Solar Fleet (signature Ability)

**First ability slot:** Solar Fleet replaces Chlorophyll; Solar Power and hidden Early Bird remain available.

- While effective sunny weather is active, Sunflora's effective Speed stat stage is **four stages higher**, capped at the normal +6 stage maximum.
- At a neutral Speed stage, this is equivalent to +4 Speed, i.e. **3× effective Speed**.
- The ability is an automatically calculated, **temporary modifier** to Speed, rather than permanent increments of the stat-stage field. It therefore drops immediately if the sun ends or is suppressed, or if Sunflora loses the ability, without corrupting other boosts/drops.
- No stacking each turn. The displayed stat-stage field does not include the virtual +4 boost.
- As with Chlorophyll, Utility Umbrella blocks the weather-based boost.

## Verification

Regression tests: `test/battle/ability/flash_freeze.c`, `test/battle/ability/solar_fleet.c`.

The change is committed to `master` but the battle tests / full ROM build have not been run in a native compilation environment.
