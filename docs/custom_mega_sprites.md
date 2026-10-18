# Custom Mega sprite import

Species definitions live beside their base families in
`src/data/pokemon/species_info/gen_*_families.h`; there is no separate custom
Mega species table. The palette helper reads these generation tables.

The supplied pack adds 24 custom Mega species (IDs 1573–1596). The existing
Meganium, Feraligatr, Excadrill, Pyroar and Darkrai Megas are preserved; their
custom counterparts use `SPECIES_*_MEGA_M` identifiers. Their distinct stone names are
**Meganiumite M**, **Feraligite M**, **Excadrite M**, **Pyroarite M**, and
**Darkranite M**. Each custom form has its own held Mega Stone and evolution trigger. All 24
stones are sold by Orange City's Poké Mart clerk for a base price of ₽500
each. Existing item IDs and original stones are preserved. New stone icons and palettes are copies of existing stone assets;
`manifest.json` records each source.

Stats and types match the supplied reference cards for all 24 forms. Brisk ability
changes and their design rationale are recorded in `custom_ability_review.md`;
manifest `source_ability` fields retain the original reference choices.
Their learnsets, icons and overworld graphics inherit Brisk's base species data.
All listed abilities use the existing battle implementations, including Dragonize,
Piercing Drill and Fire Mane. Flygon's six individual stats total 610; these values
are used even though its reference card labels the total as 620.
The original 24 forms now have shiny palettes adapted from their base forms'
normal-to-shiny color changes, preserving the Mega artwork's palette indexes. The forms revert to their base species
on fainting or after battle.

## Graphics

Each folder under `graphics/pokemon/custom_megas` contains:

- `front_80.png` and `back_80.png`: native 80×80 indexed battle artwork.
- `front_tiles.png` and `back_tiles.png`: the same pixels packed for the GBA.
- `front.png` and `back.png`: 64×64 compatibility graphics for menus, contests
  and moves that put the battler on a background layer. The front compatibility
  sheet includes two identical frames.
- `normal.pal`: one shared 16-entry palette, with transparency at index zero.

Front/back pairs are quantized together without dithering where necessary.
Native sprites retain their 80×80 dimensions. The supplied filenames
`AMOONGUS` and `TYPLHOSION` map to Amoonguss and Typhlosion respectively.
`manifest.json` lists every imported form and its asset path.

## Native battle renderer

One logical battler emits six hardware objects: a 64×64 square, two 16×32
rectangles on the right, two 32×16 rectangles at the bottom, and a 16×16 corner.
Their tile offsets are 0, 64, 72, 80, 88 and 96, using exactly 100 tiles
(3200 bytes). The composite keeps the normal battler's bottom baseline.
When native composites are used, the renderer enables all 128 hardware object
slots so that extra pieces do not consume the usual 64-object drawing budget.

The native images are read directly from ROM. Battler tile allocations reserve
100 tiles before any transformation, so a later Mega Evolution or Transform
does not overwrite adjacent sprite graphics. Normal decompression buffers keep
their original two-frame layout plus enough tail padding for the reserved copy
size. Menus reset image pointers to the compatibility canvas; battle restoration
then selects the native sprite, transformed species, or Substitute as needed.

`SUBSPRITES_MON_80x80` applies the inverse hardware affine matrix to each
piece's center. All pieces share the parent's palette, priority, animation
matrix and visibility. The faint animation copies the native data into writable
memory before clearing packed tile rows.

Run the emulator checks with:

```sh
make -j8 check TESTS='Native Mega sprites'
```

Full visual playtesting of battle animations is still needed before release.

## Additional sprite packs

Fourteen additional Mega forms (IDs 1597–1610) use the supplied 64×64
artwork from ThunderRainbow, SpiritsUndead and iriv24. Raichu and Milotic
from these packs are excluded; the previously installed Milotic is unchanged.
Nidoqueen uses the newer `mega_v2` revision. The two Empoleon designs are
separate X/Y forms, with separate stones. Original Rillaboom Gigantamax and
Samurott regional form tables are preserved.

Each form gains 100 base stat points with unchanged HP. All forms retain Brisk's existing base typing, including
Electric/Dark Luxray and Grass/Rock Rillaboom. Luxray's stats build on
Brisk's existing buffs. Slaking uses Slow Start to balance its 770 total stats.
All new backs sit four pixels lower. Original pixel indices are preserved,
with the supplied normal/shiny palettes applied to both views.

| Mega | HP / Atk / Def / SpA / SpD / Spe | Ability | Stone |
|---|---|---|---|
| Nidoqueen Mega | 90/112/127/95/105/76 | Sheer Force | Nidoqueenite |
| Nidoking Mega | 81/132/97/115/75/105 | Sheer Force | Nidokingite |
| Tropius Mega | 99/88/113/122/77/61 | Solar Power | Tropiusite |
| Rillaboom Mega | 100/155/120/60/100/95 | Grassy Surge | Rillaboomite |
| Samurott Hisui Mega | 90/148/100/100/75/115 | Sharpness | Samurottite H |
| Skeledirge Mega | 104/75/130/150/105/66 | Punk Rock | Skeledirgite |
| Dusknoir Mega | 45/140/165/65/165/45 | Levitate | Dusknoirite |
| Empoleon Mega X | 84/86/108/151/121/80 | Competitive | Empoleonite X |
| Empoleon Mega Y | 84/126/128/111/121/60 | Defiant | Empoleonite Y |
| Infernape Mega | 76/134/81/124/81/138 | Iron Fist | Infernite |
| Luxray Mega | 90/175/110/75/110/140 | Strong Jaw | Luxrayite |
| Roserade Mega | 60/70/85/155/125/120 | Aroma Veil | Roseradite |
| Slaking Mega | 150/180/140/105/95/100 | Slow Start | Slakingite |
| Torterra Mega | 95/139/145/85/115/46 | Solid Rock | Torterrite |

All fourteen stones have individually named copies of existing stone icon
and palette files, and are stocked by the Orange City Poké Mart clerk at
the same base price of ₽500. Difficulty price scaling still applies.
`added_manifest.json` records sources, stats, stones, and palette availability.
Roserade, Slaking and Torterra use shiny palettes adapted from their base
forms. All other additional forms retain the supplied shiny palettes.
`tools/brisk/mega_shiny_palettes.py` regenerates the 27 derived palettes.
Transparency is preserved and colors remain within the GBA palette format.
Artist credits from each pack are retained in each asset folder's `CREDITS.md`;
ThunderRainbow credits its spritework; SpiritsUndead credits Yogurcomics,
Max S and PhatmonMonstr. iriv24 credits camioftargon, IridescentMirage,
Rachel T, Pokets and X.

## SpectralMallow sprite sheets

Five forms (IDs 1611–1615) import the original sheets preserved under
`resources/spectralmallow/`. Credit: [SpectralMallow](https://www.deviantart.com/spectralmallow/gallery).
Normal and shiny front/back artwork is extracted from the sheets and fitted
proportionally to the native 80×80 renderer; compatibility fronts have two
identical 64×64 frames. The renderer applies the existing four-pixel lower
back alignment. Normal/shiny palette indexes are shared between both views.
All forms gain 100 base stat points without changing HP.

| Mega | Type | Ability | HP/Atk/Def/SpA/SpD/Spe | Stone |
|---|---|---|---|---|
| Clawitzer | Water/Dragon | Mega Launcher | 71/83/108/170/89/79 | Clawitzerite |
| Tinkaton | Fairy/Steel | Steelworker | 85/115/107/70/115/114 | Tinkatonite |
| Arboliva | Grass/Fairy | Seed Sower | 78/69/120/165/129/49 | Arbolivite |
| Cryogonal | Ice | Levitate | 80/50/90/125/145/125 | Cryogonalite |
| Haxorus | Dragon/Steel | Sharpness | 76/177/120/60/90/117 | Haxorite |

Typing follows the artist's design descriptions. Level-up/teachable moves
inherit the base species. Arboliva gains level-one Dazzling Gleam and Haxorus
gains level-one Iron Head in the active Gen 9 learnsets, so the summary
relearner can prepare them for their new secondary types before battle.
Each has its own stone, copied from an existing stone asset and sold by
the Orange City Poké Mart clerk at the standard base price of ₽500.
`spectralmallow_manifest.json` records source sheets and battle data.

### Additional SpectralMallow forms

Three more forms (IDs 1616–1618) follow the same native 80×80 normal/shiny
sprite import and duplicated Mega Stone setup. Design and sprite sheets:
[SpectralMallow](https://www.deviantart.com/spectralmallow/gallery).
The original sheets and individual source credits are preserved alongside
the first five imports. Each gains 100 base stat points, with unchanged HP.

| Mega | Type | Ability | HP/Atk/Def/SpA/SpD/Spe | Stone |
|---|---|---|---|---|
| Dodrio | Normal/Flying | Reckless | 60/140/90/60/80/140 | Dodrionite |
| Sigilyph | Psychic/Flying | Magic Guard | 72/58/100/143/100/117 | Sigilyphite |
| Reuniclus | Psychic/Fighting | Iron Fist | 110/125/105/125/95/30 | Reuniclusite |

Moves inherit each base species. Reuniclus additionally gains level-one
Drain Punch in the active Gen 9 learnset, available through the summary
relearner before Mega Evolution; Hammer Arm remains in its existing learnset.
All three stones are stocked by the Orange City Poké Mart clerk.

## EndlessTGX sheets

Seven forms (1619–1625) import Mega Rapidash, Vespiquen, Exploud, Mamoswine,
Tsareena, Breloom and Ninetales. Credit: EndlessTGX. Source JPEGs are retained
in `resources/endlesstgx/`; white backgrounds and JPEG noise were cleaned,
then art was fitted to the native 80×80 renderer and 16-color palettes.
Existing Megas (including Starmie, Froslass and Chimecho) and Golem were
skipped as requested. Normal and shiny views are supplied for every form.
Ninetales' orange/blue shiny colors were mapped by color family to avoid
the JPEG sheet's misaligned pixels corrupting white fur and outlines.

| Mega | Type | Ability | HP/Atk/Def/SpA/SpD/Spe |
|---|---|---|---|
| Rapidash | Fire/Flying | Gale Wings | 65/120/90/100/100/125 |
| Vespiquen | Bug/Flying | Queenly Majesty | 70/100/132/110/122/40 |
| Exploud | Normal/Steel | Punk Rock | 104/111/88/131/78/78 |
| Mamoswine | Ice/Ground | Thick Fat | 110/160/110/70/80/100 |
| Tsareena | Grass/Fighting | Dancer | 72/150/118/50/118/102 |
| Breloom | Grass/Fighting | Technician | 60/160/110/60/80/90 |
| Ninetales | Fire/Ghost | Drought | 73/76/95/131/110/120 |

Each gains 100 base stat points, with unchanged HP. Rapidash gains relearnable
Brave Bird and Ninetales gains Shadow Ball in the active Gen 9 learnsets.
Other moves inherit base learnsets. Each has a separate named stone with a
duplicated stone icon and palette, stocked at Orange City's Poké Mart.
