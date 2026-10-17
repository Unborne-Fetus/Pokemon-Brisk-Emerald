# CDRX73 additions

Designs, front/back sprites, icons and supplied shiny colors: [CDRX73](https://www.reddit.com/user/CDRX73/).
Names, lore and gameplay data: Brisk. Sources: FAKEMON.zip, SPRITES 2.zip and HALLOWEEN.rar supplied by the user.

The 30 standalone designs are installed; the larger FRONT/BACK and H1–H6 images
are preview sheets. No maps, encounters, gifts, shops, teams or rental pools are
changed to place these additions. The alternate Dragonite Mega uses
`ITEM_DRAGONINITE_CDRX` (Dragoninite C); the existing Mega and its stone remain.

All battle art uses native 80×80 sprites. Normal/shiny front and back colors
share the GBA's 15 visible palette indices. Icons use the engine's shared icon
palettes. The Halloween archive supplies no separate shiny sprites, so those
forms deliberately use their supplied normal colors for both palettes.

| Species | Constant | Types | Evolution |
| --- | --- | --- | --- |
| Noctarch | `SPECIES_NOCTARCH` | Psychic/Flying | Lv. 42 from Noctowl |
| Alphayena | `SPECIES_ALPHAYENA` | Dark | Lv. 38 from Mightyena |
| Bruticate | `SPECIES_BRUTICATE` | Normal/Fighting | Lv. 36 from Raticate |
| Marrowak | `SPECIES_MARROWAK` | Ground/Fighting | Lv. 42 from Marowak |
| Rhyfortress | `SPECIES_RHYFORTRESS` | Ground/Steel | Lv. 60 from Rhyperior |
| Duneshred | `SPECIES_DUNESHRED` | Ground/Dark | Lv. 42 from Sandslash |
| Skarmagedn | `SPECIES_SKARMAGEDN` | Steel/Flying | Lv. 48 from Skarmory |
| Sharkaiser | `SPECIES_SHARKAISER` | Water/Dark | Lv. 48 from Sharpedo |
| Electitan | `SPECIES_ELECTITAN` | Electric/Fighting | Lv. 60 from Electivire |
| Grandbully | `SPECIES_GRANDBULLY` | Fairy/Fighting | Lv. 42 from Granbull |
| Sudosylvan | `SPECIES_SUDOSYLVAN` | Rock/Grass | Lv. 42 from Sudowoodo |
| Parasoul | `SPECIES_PARASOUL` | Bug/Ghost | Lv. 40 from Parasect |
| Linevolver | `SPECIES_LINEVOLVER` | Normal/Ground | Lv. 40 from Linoone |
| Tentacrown | `SPECIES_TENTACROWN` | Water/Poison | Lv. 50 from Tentacruel |
| Dragonite (Mega C) | `SPECIES_DRAGONITE_MEGA_CDRX` | Dragon/Water | Mega with Dragoninite C |
| Litwick (Halloween) | `SPECIES_LITWICK_HALLOWEEN` | Grass/Fire | Lv. 25 → Lampent (Halloween) |
| Lampent (Halloween) | `SPECIES_LAMPENT_HALLOWEEN` | Grass/Fire | Lv. 41 → Chandelure (Halloween) |
| Chandelure (Halloween) | `SPECIES_CHANDELURE_HALLOWEEN` | Grass/Fire | — |
| Sinistea (Halloween) | `SPECIES_SINISTEA_HALLOWEEN` | Ghost/Poison | Lv. 30 → Polteageist (Halloween) |
| Polteageist (Halloween) | `SPECIES_POLTEAGEIST_HALLOWEEN` | Ghost/Poison | — |
| Ponyta (Halloween) | `SPECIES_PONYTA_HALLOWEEN` | Fairy/Poison | Lv. 40 → Rapidash (Halloween) |
| Rapidash (Halloween) | `SPECIES_RAPIDASH_HALLOWEEN` | Fairy/Poison | — |
| Tangela (Halloween) | `SPECIES_TANGELA_HALLOWEEN` | Ghost/Normal | Lv. 36 → Tangrowth (Halloween) |
| Tangrowth (Halloween) | `SPECIES_TANGROWTH_HALLOWEEN` | Ghost/Normal | — |
| Timburr (Halloween) | `SPECIES_TIMBURR_HALLOWEEN` | Fighting/Ghost | Lv. 25 → Gurdurr (Halloween) |
| Gurdurr (Halloween) | `SPECIES_GURDURR_HALLOWEEN` | Fighting/Ghost | Lv. 40 → Conkeldurr (Halloween) |
| Conkeldurr (Halloween) | `SPECIES_CONKELDURR_HALLOWEEN` | Fighting/Ghost | — |
| Emolga (Halloween) | `SPECIES_EMOLGA_HALLOWEEN` | Electric/Dark | — |
| Snorunt (Halloween) | `SPECIES_SNORUNT_HALLOWEEN` | Ice/Ghost | Lv. 42 → Glalie (Halloween) |
| Glalie (Halloween) | `SPECIES_GLALIE_HALLOWEEN` | Ice/Ghost | — |

The fourteen new evolutions have independent National Dex records 1038–1051.
Halloween forms share their original species' records, as other regional forms do.
Existing species/item IDs and SaveBlock1/2 sizes and offsets are unchanged. The
first twelve custom Dex bits keep their existing storage; the next sixteen seen
and caught bits use the previously unused `pokedex.unknown3` word.
Full metadata is in `tools/brisk/cdrx73_species.json`; stats are HP/Atk/Def/Speed/SpA/SpD.

## New Options page

Use L/R to reach page 5 of Options. FORMAT has Regular, Singles and Doubles.
Regular preserves scripted formats. Singles converts ordinary combat, including
partner and two-trainer battles; two opposing parties are fought sequentially.
Doubles adds a second wild mon when necessary; a side with one usable mon fights
with one occupied position. Network battles, recordings, Safari and catching
demonstrations retain their dedicated formats.

FOLLOWERS defaults to ON. OFF removes the following Pokémon when returning to
the field; ON restores it when the usual map/story conditions permit. This
setting does not disable story NPC companions. Both options persist in saves.
