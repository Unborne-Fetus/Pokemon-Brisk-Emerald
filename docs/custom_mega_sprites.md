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
