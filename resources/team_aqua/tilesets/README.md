# Selected Team Aqua tilesets

Source: https://github.com/TeamAquasHideout/Team-Aquas-Asset-Repo
Source revision: 92686187379d04d38bdf08b509f2db136cc25c99

All 16 selected sheets (plus a Hidden Grotto overflow companion) are registered in `src/data/tilesets/{graphics,metatiles,headers}.h`. Runtime/editor files live under `data/tilesets/{primary,secondary}/aqua_*`. Each includes tiles.png, all 16 numbered palette files, metatiles.bin and metatile_attributes.bin. Original author credits, permissions and source notes are preserved in the folders here; credit those authors and Rahtak where the source requests it. Brick Cafe assets are for non-commercial projects.

## Using them in Porymap

Reopen the project after updating. Porymap reads `NUM_TILES_PER_METATILE 12` in include/fieldmap.h and enables three-layer metatiles automatically. Use the `gTileset_Aqua...` names listed in manifest.json in a new map layout. Existing layouts, metatile IDs, behaviors and collisions have not changed.

The engine now renders three explicit layers. All 134 existing two-layer tileset files were migrated to this format, preserving their original BG3/BG2/BG1 placement (including normal-layer background fill). Shop rendering and Secret Base decoration graphics also understand the format. Door animation frames retain their original eight-tile format.

| Primary | Secondary / layout requirements |
| --- | --- |
| gTileset_AquaAlternativeGeneral | Emerald layout (`is_frlg: false`), up to 512 primary metatiles and 512 primary tiles. Source animations have a dedicated callback with their own offsets. Secondary packs referencing vanilla General or Rahtak greenery need their primary tile references checked when pairing with this alternative; it is not a replacement for General. |
| gTileset_AquaDesert | gTileset_AquaDesertVillage; Emerald layout. Some primary metatiles intentionally reference secondary tiles/palettes. |
| gTileset_AquaPyramidInterior | gTileset_AquaPyramidInteriorSecondary; Emerald layout. |
| gTileset_AquaHiddenGrotto | gTileset_AquaHiddenGrottoSecondary; Emerald layout. The companion supplies the final 128 source tiles and palette 6. Original metatile IDs and 16-bit attributes are retained. |
| gTileset_AquaOrangeIslands | gTileset_AquaValenciaIsland; Emerald layout. The primary supplies the first 512 tiles/metatiles; the secondary starts with the remaining 128 primary tiles/metatiles, followed by Valencia. Global tile/metatile IDs are preserved. The pair supplies palettes 0–12, including palette 6 transferred to the secondary. |

Other imported secondary packs: AutumnRuins, BrickCafe, DojoExterior, DojoInterior, LugiaAltar, ShadyForest, SmallTownLab and SpaceMeteor (all prefixed `gTileset_Aqua`). These retain the source's primary tile and palette references; choose the appropriate primary and inspect the result in Porymap before building a map. The Small Town source specifically expects Rahtak greenery grass tiles. No existing maps have been switched to imported tilesets.

The alternative General runtime uses the pack's supplied compiled Porytiles output. Its larger editable expanded sheet, layer PNGs, Aseprite sources and original mapping CSV are retained under alternative_general/porytiles_source_files; the expanded sheet's extra pieces need a new Porytiles compile before becoming additional metatiles. Space Meteor's sample star animation source frames are retained under space_meteor/anim; its source does not supply destination offsets, so its registered tileset uses the static tiles rather than guessing animation destinations.

Palette slots absent from a source pack are filled with the existing General palette 00 to keep each palette table a full 16 slots; supplied palettes are copied unchanged.

## Orange Islands adaptation

The source packs use FireRed's 640/384 tile and metatile split and 7/6 palette split. Their runtime versions use Emerald's 512/512 and 6/7 split so standard Porymap settings work across the project. Original source PNGs/binaries are retained in each pack's `original` directory. FireRed metatile behavior IDs were translated by name into Brisk behavior IDs, and layer types were converted to Emerald attributes. `orange_islands/behavior_conversion.json` lists the mappings. The source's nonstandard behavior 0x112 is treated as deep water (0x12); its two undefined 0xA4 entries are assigned normal behavior. Terrain/encounter bits in FireRed's 32-bit attributes are not present in Emerald's attributes. Map collision/elevation settings remain the responsibility of the new map's blocks.
