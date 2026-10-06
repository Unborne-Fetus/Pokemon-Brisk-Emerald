# Team Aqua art resources for Brisk Emerald

Source: https://github.com/TeamAquasHideout/Team-Aquas-Asset-Repo
Source revision: 92686187379d04d38bdf08b509f2db136cc25c99

## Battle backgrounds

PurrfectDoodle's backgrounds are installed in `graphics/battle_environment`: grassy plains (tall_grass and Plain), long grass, sand, underwater, sea, pond, cave, sky, and snow. Snow and Ice now have full graphics; Soaring and Sky Pillar use the sky graphics. Matching entrance animations are imported where supplied; otherwise existing entrance images are recolored to the replacement palette. Legendary cave/water palettes match the new tiles. Building, stadium, and mountain art remains separate.

## Coffee Cup customization resources

The archive contains the complete editable customization pack: PNG components, layered GIMP projects, reference documents, examples, and the original README. Temporary Office lock files are excluded. Run `python tools/extract_coffee_cup.py` to unpack it into `resources/team_aqua/coffee_cup/`. It is source art, not a list of finished selectable outfits. Assemble complete outfits and register them in `src/data/outfit_tables.h`; provide all required player states before enabling an outfit.

## Individual tiles

`individual_tiles/` contains greenery from Rahtak (FM and Zeikaro), Oomer's plants and decorations, KyuZee's fence, and yoshord's furniture. Import selected tiles into a tileset in Porymap and create their metatiles/behaviors before placing them. Existing maps are not automatically modified.

- KyuZee's fence uses General palette 5.
- yoshord's furniture uses the Secret Base palettes specified in its original README.
- Rahtak's sheet and Oomer's individual images need palette matching to the destination tileset.

Original asset README files are retained. Credit the creators listed in `CREDITS.md` and in each pack.
