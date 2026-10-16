# Custom Mega sprite import

The supplied pack adds 24 custom Mega species (IDs 1573–1596). The existing
Meganium, Feraligatr, Excadrill, Pyroar and Darkrai Megas are preserved; their
custom counterparts use `SPECIES_*_MEGA_M` identifiers. Their distinct stone names are
**Meganiumite M**, **Feraligite M**, **Excadrite M**, **Pyroarite M**, and
**Darkranite M**. Each custom form has its own held Mega Stone and evolution trigger. All 24
stones are sold with the existing Mega Stones at Lilycove Department Store
4F (top-right clerk) for ₽500 each. Existing item IDs and original stones are
preserved. New stone icons and palettes are copies of existing stone assets;
`manifest.json` records each source.

Stats, types and abilities match the supplied reference cards for all 24 forms.
Their learnsets, icons and overworld graphics inherit Brisk's base species data.
All listed abilities use the existing battle implementations, including Dragonize,
Piercing Drill and Fire Mane. Flygon's six individual stats total 610; these values
are used even though its reference card labels the total as 620.
Custom shiny palettes are pending. Both normal and shiny individuals currently
use the supplied artwork's normal palette. The forms revert to their base species
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
| Tropius Mega | 99/88/113/122/77/61 | Thick Fat | Tropiusite |
| Rillaboom Mega | 100/155/120/60/100/95 | Grassy Surge | Rillaboomite |
| Samurott Hisui Mega | 90/148/100/100/75/115 | Sharpness | Samurottite H |
| Skeledirge Mega | 104/75/130/150/105/66 | Punk Rock | Skeledirgite |
| Dusknoir Mega | 45/140/165/65/165/45 | Levitate | Dusknoirite |
| Empoleon Mega X | 84/86/108/151/121/80 | Competitive | Empoleonite X |
| Empoleon Mega Y | 84/126/128/111/121/60 | Defiant | Empoleonite Y |
| Infernape Mega | 76/134/81/124/81/138 | Iron Fist | Infernite |
| Luxray Mega | 90/175/110/75/110/140 | Strong Jaw | Luxrayite |
| Roserade Mega | 60/70/85/155/125/120 | Technician | Roseradite |
| Slaking Mega | 150/180/140/105/95/100 | Slow Start | Slakingite |
| Torterra Mega | 95/139/145/85/115/46 | Solid Rock | Torterrite |

All fourteen stones have individually named copies of existing stone icon
and palette files, and are stocked by the Lilycove 4F Mega Stone clerk at
the same base price of ₽500. Difficulty price scaling still applies.
`added_manifest.json` records sources, stats, stones, and palette availability.
Roserade, Slaking and Torterra lack separate shiny palettes in the uploads,
so they currently use their normal palette for shiny individuals.
Artist credits from each pack are retained in each asset folder's `CREDITS.md`;
ThunderRainbow credits its spritework; SpiritsUndead credits Yogurcomics,
Max S and PhatmonMonstr. iriv24 credits camioftargon, IridescentMirage,
Rachel T, Pokets and X.
