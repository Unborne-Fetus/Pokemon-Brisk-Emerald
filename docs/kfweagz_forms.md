# KFWeagz forms and Orange City Mega Stones

Supplied designs, front/back sprites and shiny colors: [KFWeagz](https://www.deviantart.com/kfweagz).
Original uploaded sheets are retained in `resources/kfweagz/`; their DeviantArt
deviation IDs are encoded in the `d...` filenames. All fifteen supplied sheets
are used. Seviron, Losiatu and the leafy Quagsire design are adapted as Megas,
as requested. The artist calls Garganacl's design Iron Furnace, a future paradox;
Brisk deliberately implements **both paradox designs as past paradox Sun Stone
evolutions**, following the user's instruction, with Protosynthesis. Steam Shell
is Brisk's name for the Blastoise design. No new encounter or gift locations are
added.

Mega forms retain their base HP and gain 100 total base stats. Abilities are
existing engine abilities chosen for the artwork, with all ability slots agreeing.
Paradoxes have 570 total base stats and independent National Dex entries.
Blastoise retains its original Mega and Gigantamax forms; Lapras retains its
Gigantamax form and existing form index. The paradox evolutions are permanent.

All **67 custom Mega Stones**, including Dragoninite C, are sold exclusively by
Orange City Mart's stone counter. Existing official Mega Stone inventories and
rental held items remain supported. Each new stone has its own item ID, name,
and separate copies of an existing stone icon and palette. Prices follow the
existing difficulty multipliers. Neither paradox has a Mega Stone.

| Form | Types | Ability | Activation | HP / Atk / Def / SpA / SpD / Spe |
| --- | --- | --- | --- | --- |
| Mega Drifblim | Ghost/Flying | Fluffy | Drifblimite | 150 / 80 / 79 / 110 / 79 / 100 |
| Mega Lapras | Water/Dragon | Shell Armor | Laprasite | 130 / 95 / 110 / 115 / 115 / 70 |
| Mega Seviper | Poison/Steel | Sharpness | Seviprite | 73 / 135 / 80 / 100 / 80 / 90 |
| Mega Hydreigon | Dark/Dragon | Intimidate | Hydreigonite | 92 / 120 / 105 / 165 / 110 / 108 |
| Mega Regigigas | Normal/Rock | Solid Rock | Regigigite | 110 / 180 / 140 / 100 / 140 / 100 |
| Mega Noivern | Flying/Dragon | Punk Rock | Noivernite | 85 / 70 / 100 / 137 / 100 / 143 |
| Mega Kingdra | Water/Dragon | Multiscale | Kingdrite | 75 / 95 / 115 / 135 / 115 / 105 |
| Mega Fearow | Normal/Flying | Sniper | Fearowite | 65 / 130 / 85 / 61 / 71 / 130 |
| Mega Togekiss | Fairy/Flying | Serene Grace | Togekissite | 85 / 50 / 115 / 160 / 135 / 100 |
| Mega Volcarona | Bug/Fire | Drought | Volcaronite | 85 / 60 / 75 / 175 / 135 / 120 |
| Mega Zangoose | Normal/Steel | Sharpness | Zangoosite | 73 / 145 / 90 / 60 / 80 / 110 |
| Mega Quagsire | Water/Grass | Water Absorb | Quagsirite | 95 / 95 / 115 / 105 / 85 / 35 |
| Mega Xatu | Psychic/Flying | Magic Bounce | Xatunite | 65 / 75 / 95 / 125 / 95 / 115 |
| Iron Furnace | Fire/Steel | Protosynthesis | Sun Stone from Garganacl | 110 / 120 / 130 / 75 / 100 / 35 |
| Steam Shell | Water/Fire | Protosynthesis | Sun Stone from Blastoise | 100 / 75 / 110 / 130 / 90 / 65 |

The two paradoxes have their own level-up and teachable move lists, with both
STAB types and attacks appropriate to their stats. New Megas share their base
learnsets. Quagsire can relearn Magical Leaf, Giga Drain and Energy Ball;
Seviper gains Cross Poison, Night Slash and Iron Head, and Zangoose gains Iron
Head, so their new typings and bladed anatomy have usable attacks before Mega
Evolution. Party icons, cries, footprints and followers reuse their base species'
assets; the sheets supply battle sprites rather than walking animations.

`tools/brisk/import_kfweagz_forms.py` rebuilds normal/shiny front/back art from
explicit crop rectangles in the supplied PNG sheets. No replacement artwork is
generated. Native 80px art uses the existing composite renderer, with 64px
compatibility pictures. Oversize sprites are scaled together with nearest-neighbor
sampling rather than cropped; normal/shiny pairs share 15 visible GBA colors.
`tools/brisk/kfweagz_sprite_import.json` records the crops and palette counts.
`tools/brisk/kfweagz_species.json` records all gameplay data and stone icon sources.

Species and item IDs are appended, preserving existing values. The two new Dex
records use the final two reserved custom-Dex bits. Neither save block size nor
any established save offset changes. `test/kfweagz_forms.c` covers their evolution,
boxed data and form compatibility; `test/custom_mega_stones.c` exercises every
custom stone in battle, and `test/cdrx73_fakemon.c` verifies all 28 independent
custom Dex flags. `tools/brisk/check_custom_mega_stones.py` audits exclusive stock.
