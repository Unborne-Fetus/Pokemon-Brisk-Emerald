# Brisk Emerald signature abilities — catalogue batch 1

Source: user-edited Google Doc, records Venusaur through Raichu (November 2026). Implemented as actual battle abilities rather than placeholders. The original catalogue text is preserved below because completed entries are removed from the source document.

## Species and assignment

| Source species | New ability | Recipient | Behaviour |
| --- | --- | --- | --- |
| Venusaur | Poison Wart | Mega Venusaur | Offensive and defensive contact can poison the other combatant (50% each qualifying hit). |
| Charizard | Flame Tail | Mega Charizard X and Y | Tail-based damaging moves deal 1.5× power and have a 50% burn chance. |
| Blastoise | Power Cannons | Mega Blastoise | Attacks that require a recharge turn have 2× power; the recharge remains. |
| Butterfree | Hemlock Wings | Mega Butterfree | Contact attackers receive paralysis with a 70% roll or sleep with a 30% roll, if eligible for the selected status. |
| Beedrill | Petiole | Mega Beedrill | +1 Speed stage after every successful damaging hit on a foe, including each hit of multi-hit attacks. |
| Pidgeot | Blind Dive | Mega Pidgeot | Recoil attacks have 1.5× power and do not inflict the user's normal recoil. |
| Fearow | Lightning Bill | Mega Fearow | Flying-type moves gain +1 priority, stacking with their existing move priority. |
| Arbok | Hood Flare | Arbok's hidden Ability slot | Damaging attacks have 2× power, and every move's priority is reduced by one bracket. |
| Raichu | Evolution Denial | Mega Raichu X and Y | Intrinsic 1.5× Defense and Special Defense, as if holding Eviolite even though fully evolved. |

Regular Venusaur, Charizard, Blastoise, Butterfree, Beedrill, Pidgeot, Fearow, and Raichu retain their preexisting abilities. Arbok does not have an implemented Mega, so it receives Hood Flare in its hidden slot, replacing Strong Jaw.

Charizard and Raichu each have two Mega forms. The signature applies to both until or unless the user chooses one specific Mega form.

## Original user-authored ability text

- **Poison Wart:** "contact moves used by or against this pokemon have a 50% chance to poison the pokemon that made contact which isn’t venusaur"
- **Flame tail:** "When using tail moves sucj as iron tail, deal +50% extra damage, and 50% to burn"
- **Power Cannons:** "Moves with a recharge turn like hyper beam will do double damage"
- **Hemlock wings:** "Contact moves against this pokemon (not from this pokemon) either cause paralysis or sleep at a 70/30 chance ratio"
- **Petiole:** "When this pokemon attacks a pokemon it’s speed is raised by one stage, for multihit moves, once for each hit"
- **Blind Dive:** "Moves that deal recoil damage to the user no longer deal recoil and do 1.5X damage"
- **Lightning Bill:** "This pokemons flying type moves have +1 priority, if the move already has +1 priority add +1 making it +2"
- **Hood Flare:** "This pokemon deals double damage, but it’s moves lose -1 priority"
- **Evolution Denial:** "This pokemon always has the buffs that an eviolite would give an unevolved pokemon whether its holding it or not"

## Engine integration

The ability IDs and UI descriptions are in `include/constants/abilities.h` and `src/data/abilities.h`. Species assignments are in `src/data/pokemon/species_info/gen_1_families.h`. On-contact and per-hit effects use `src/battle_util.c`. Move power, recoil and Eviolite-style defensive modifiers also use `src/battle_util.c` and `src/battle_move_resolution.c`. Move-order effects use `src/battle_main.c`.

The defensive status effects respect normal Pokémon status immunities and contact-prevention effects. Flame Tail's damaging tail-move set includes Iron Tail, Aqua Tail, Dragon Tail, Poison Tail, and Tail Slap; status moves such as Tail Whip are excluded. Blind Dive cancels actual damage-based recoil rather than missed-attack crash damage.

Test cases are in `test/battle/ability/brisk_catalogue_batch_one.c`.

## Important caveat

The abilities are intentionally strong because their values are from the user's design document. The implementation preserves those requested multipliers and odds rather than silently rebalancing them.
