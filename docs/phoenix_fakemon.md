# princess-phoenix Fakémon

All twelve species are implemented as independent species. No wild encounter,
map, gift, trainer team, or Adventure rental pool has been edited to place them.
Use their `SPECIES_` constants below in your own maps or encounter tables.
Existing species IDs and SaveBlock1/2 offsets are preserved. New seen/caught bits
use the four previously reserved bytes before externalEventData, with a nibble
signature to initialize old padding safely. Canonical Dex flags remain at their
original offsets and lengths.

Designs, names, front/back sprites, supplied shiny colors, evolution destinations,
and the five supplied Pokédex entries: **princess-phoenix**. Gameplay data,
including typing, stats, abilities, moves, and evolution levels: Brisk.

| Species | Constant | National Dex | Types | Evolution |
| --- | --- | --- | --- | --- |
| Hissiorite | `SPECIES_HISSIORITE` (1626) | 1026 | Fire | Lv. 16 → Cobarett |
| Cobarett | `SPECIES_COBARETT` (1627) | 1027 | Fire | Lv. 36 → Pythonova |
| Pythonova | `SPECIES_PYTHONOVA` (1628) | 1028 | Fire/Dragon | — |
| Baoby | `SPECIES_BAOBY` (1629) | 1029 | Grass/Water | Lv. 32 → Baobaraffe |
| Baobaraffe | `SPECIES_BAOBARAFFE` (1630) | 1030 | Grass/Water | — |
| Drashimi | `SPECIES_DRASHIMI` (1631) | 1031 | Water/Dragon | Lv. 30 → Tsushimi |
| Tsushimi | `SPECIES_TSUSHIMI` (1632) | 1032 | Water/Dragon | Lv. 55 → Tobishimi |
| Tobishimi | `SPECIES_TOBISHIMI` (1633) | 1033 | Water/Dragon | — |
| Merlicun | `SPECIES_MERLICUN` (1634) | 1034 | Bug/Dragon | Lv. 45 → Firomenis |
| Firomenis | `SPECIES_FIROMENIS` (1635) | 1035 | Bug/Dragon | — |
| Anu | `SPECIES_ANU` (1636) | 1036 | Psychic/Flying | — |
| Nuenflu | `SPECIES_NUENFLU` (1637) | 1037 | Ice/Poison | — |

The complete chosen stats and abilities are in `phoenix_fakemon.json`.
The stat array is HP, Attack, Defense, Speed, Special Attack, Special Defense.
The original wording of supplied entries is retained and wrapped for the screen.
Press **START** on an entry to cycle through its remaining text. The sushi and
four-species sheets do not provide Pokédex entries, so those seven descriptions
are intentionally blank rather than inventing replacement lore.

## Graphics

The sprite importer crops existing pixels from the supplied sheets; it does not
generate new designs. Normal and shiny sprites use a shared 15-color opaque GBA
palette index layout with a transparent index. Where the originals exceed that
limit, their closest supplied color pairs are used. Menu icons use the engine's
six shared icon palettes. Tsushimi, Tobishimi and Firomenis retain native 80×80
battle canvases with 64×64 versions for other screens. The supplied 64×64
variants are used for the five reference-card species.

Run `python tools/brisk/import_phoenix_fakemon.py <original-sheet-directory>`
to reproduce the graphics; crop coordinates are recorded in the script and
palette/native-size results in `tools/brisk/phoenix_sprite_import.json`.
No walking sprite sheets were supplied. Until they are provided, followers use
the engine's safe Substitute graphic, and footprints use its question-mark
fallback. Cries reuse suitable existing cries rather than claiming new audio.

## Sources and credit

- Creator: https://www.deviantart.com/princess-phoenix
- Hissiorite: https://www.deviantart.com/princess-phoenix/art/Hissiorite-892197800
- Cobarett: https://www.deviantart.com/princess-phoenix/art/Cobarett-892198611
- Baoby: https://www.deviantart.com/princess-phoenix/art/Baoby-890198113
- Baobaraffe: https://www.deviantart.com/princess-phoenix/art/Baobaraffe-890199408
- Merlicun: https://www.deviantart.com/princess-phoenix/art/Merlicun-340144534
- Firomenis: https://www.deviantart.com/princess-phoenix/art/Firomenis-340145234
- Sushi-line name assistance: Tenrio, as credited in the creator's Project Freemon collection.

The referenced snake sheets carry [CC BY 3.0](https://creativecommons.org/licenses/by/3.0/) attribution; Merlicun and Firomenis
carry [CC BY-SA 3.0](https://creativecommons.org/licenses/by-sa/3.0/) attribution. Retain the creator attribution and the applicable
license with redistributed source/derived sprite assets. Palette conversion,
transparent backgrounds, engine icon remapping and native tile packing are the
technical adaptations made here. No other creator's existing credit is replaced.
