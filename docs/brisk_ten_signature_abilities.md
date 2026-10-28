# Ten new Pokémon signature abilities (Brisk Emerald)

These are implemented on the existing `master` branch. Each replaces the **first normal ability slot** for the named Pokémon; other normal / hidden ability slots remain unchanged.

| Pokémon | Signature ability | Behavior |
|---|---|---|
| Carnivine | **Jaw Snap** | A Pokémon that hits Carnivine with a contact move loses **1/6 of its own maximum HP**. If the attacker is Bug-type, it loses **1/3 instead**. Does not damage on missed/blocked moves; follows normal contact protection, and Magic Guard blocks recoil. |
| Watchog | **Duran** | Gains **+3 Evasion stages on entry**. This is a real stat-stage modification, capped by +6. Reapplies on switching back in after stage resets. |
| Wugtrio | **Clearwater** | Every selected move has **+1 priority**, including support moves. Wugtrio takes **2× ordinary attack damage**. Moves with negative native priority retain their relative order: a -6 move becomes -5 rather than +1. |
| Furret | **Build Me Up** | At the end of each turn, Furret gains **+1 random eligible stat stage**, and **each living opposing active Pokémon** loses **1 random eligible stat stage**. Includes accuracy and evasion in the pool; respects stage caps and normal ability-based stat protections. |
| Ariados | **Overdose** | When Ariados **successfully inflicts bad poison via its move**, the newly poisoned foe immediately takes damage equal to **50% of that foe's maximum HP**. Normal bad poison damage also applies on ensuing turns. Does not retrigger if the target was already badly poisoned, or on toxic damage ticks. |
| Magcargo | **Burning Crust** | Ground attacks have **no effect** on Magcargo. Water attacks deal **half ordinary damage** (instead of the original total damage). |
| Lumineon | **Ogooué Basin** | Lumineon's damaging **Water-type moves deal 2× normal damage**. |
| Maractus | **Nopales** | Fire-type moves aimed at Maractus inflict **1/4 their usual damage**. Water-type moves aimed at any active battler are redirected to Maractus and **heal Maractus by 1/4 of maximum HP**, leaving the original target unaffected. Spread Water-type moves are converted to one absorbed hit when Nopales is on the field. This does not absorb moves with no Pokémon target, such as Rain Dance. When already at full HP, the Water move is still absorbed without healing. |
| Swalot | **Torsion** | Receives **half damage from physical moves**. |
| Heatmor | **Flame Tongue** | Heatmor's damaging Fire-type moves hit **both active opponents** in Double Battles. Any of Heatmor's damaging attacks against Bug-type Pokémon deal **1.5× ordinary damage**. Reserve party Pokémon cannot be struck by regular battle attacks and are not affected. |

## Additional details

- Damage modifiers use the battle engine's current ability values and are added to the actual and preview damage paths.
- Evasion and end-turn stat boosts use the standard battle scripts and can be prevented by relevant stage caps and stat-change protection.
- Water absorption uses the existing healing animation and heals for one quarter of **Maractus's maximum HP**.
- Contact retaliation uses the existing ability-contact retaliation script.
- Overdose uses a custom battle script queued after the badly-poisoned animation and before the move continuation.
- Clearwater uses the engine's priority calculation for +1; it does not change the priority value encoded on move data or bypass Protect itself.

## Verification

Regression scenarios are in `test/battle/ability/brisk_ten_signatures.c` (contact, Bug contact, entry boosts, priority, received damage, turn-end stat changes, Toxic status, Ground immunity, Water reduction, Water offense, Water redirection, Fire reduction, physical reduction, spread Fire moves and Bug damage).

**The commits are pushed, but the ROM has not yet been compiled or emulator-tested in this session.** Passing the static source checks does not establish that all interactions work in gameplay.
