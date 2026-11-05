# Brisk Emerald Signature Abilities — Catalogue Batch 2

Implemented from the user's edited native Google Doc on November 5, 2026. This record preserves the original design descriptions before completed entries are removed from the catalogue.

## Assignments and intended effects

| Pokémon | New ability | In-game recipient | Implementation |
| --- | --- | --- | --- |
| Nidoqueen | Queens Gambit | Mega Nidoqueen | If another Pokémon on the same trainer's party has fainted, boosts Attack, Defense, Speed, Sp. Atk, and Sp. Def by two stages once per switch-in. Checks on entry and at end of turn. |
| Nidoking | Momentum Build | Mega Nidoking | End of turn has a 50% chance to raise Speed one stage (not on the turn it entered), and ignores breakable defending abilities like Mold Breaker. |
| Clefable | Lunar Power | Mega Clefable | Holding a Moon Stone grants +1 to the five battle stats once per appearance. Moon Stone also enables Mega Clefable through its existing Mega evolution. |
| Ninetales | Magic Tail | Mega Ninetales | All special damaging moves have an effectiveness multiplier of 2× regardless of ordinary type matchups, including type immunities. |
| Wigglytuff | Sweet Song | Wigglytuff hidden ability | Damaging sound moves have 50% chance to induce sleep; Wigglytuff itself cannot be put to sleep. |
| Vileplume | Holoparasite | Vileplume hidden ability | Water and Grass attacks are absorbed and heal 1/8 max HP; Bug damage is halved and hits raise Speed one stage. |
| Venomoth | Wedding Gift | Venomoth hidden ability | Physical contact in either direction inflicts poison upon the other Pokémon if the two are opposite genders and the victim is eligible. |
| Dugtrio | Deep Dive | Mega Dugtrio | Ground-type attack power ×2, and spread moves directed at the holder are blocked. |
| Persian | Double Coat | Persian hidden ability | Cannot receive nonvolatile status and takes half damage from Fighting attacks. |
| Golduck | Psychic Powers | Golduck hidden ability | Damaging Normal moves become Psychic-type moves. |
| Arcanine | Panhu's Lineage | Arcanine hidden ability | Attack +2 stages per foe knocked out by an attack; stacks normally up to +6. |
| Poliwrath | Silent Golaith | Mega Poliwrath | +2 Evasion upon entry; once it uses a sound move, Evasion is set to neutral and the boost cannot return until switching out. |

## Exact ability descriptions written by the user

- **Queens gambit:** "If a pokemon on the owner of Nidoqueens team has been knocked out, Nidoqueen gets a +2 stage boost to all stats"
- **Momentum Build:** "This pokemon has a 50% chance to get a   speed boost. The ability also has mold breaker built in"
- **Lunar Power:** "If this pokemon is holding a moon stone then it gets a +1 stage boost to all stats, it can mega evolve if it has the moon stone as well."
- **Magic Tail:** "All of this Pokemo’s Sp ATK moves deal supereffective damage regardless of typing"
- **Sweet Song:** "This pokemon’s sound based moves have a 50% to send the opposing pokemon to sleep, this pokemon cannot be put to sleep"
- **Holoparasite:** "Water and Grass type moves used against this pokemon heal 1/8th of this pokemon’s health. Bug type attacks deal half damage and raise this pokemon’s speed by +1"
- **Wedding Gift:** "Contact moves used by or against this pokemon will poison the other pokemon if they are the opposite gender."
- **Deep Dive:** "This pokemon deals 2x ground type moves, it’s unaffected by spread moves as well."
- **Double Coat:** "This pokemon is unaffected by status conditions, and fighting type moves deal ½ damage against it."
- **Psychic Powers:** "This pokemon’s normal type moves will deal psychic damage"
- **Panhu’s Lineage:** "When this pokemon causes another pokemon to faint, it’s attack is raised by +2 (stacks)"
- **Silent Golaith:** "This pokemon has +2 evasivness until it uses a sound based move, once that happens it will have +0 evasiveness regardless of its current stats"

## Implementation locations

- Ability IDs and descriptions: `include/constants/abilities.h`, `src/data/abilities.h`
- Species assignments and existing Mega forms: `src/data/pokemon/species_info/gen_1_families.h`
- Moon Stone Mega Clefable: `src/data/pokemon/form_change_tables.h`
- Switch-in, end-of-turn, contact, attack, damage, and effectiveness: `src/battle_util.c`
- Psychic Powers type transformation: `src/battle_main.c`
- Per-battle flags (reset on switching): `include/constants/battle.h`

Note: "all stats" is interpreted as the five core non-HP stats (Attack, Defense, Speed, Special Attack, Special Defense), not Accuracy and Evasion, except Silent Golaith where Evasion is explicitly named. The original wording is retained above.
