# onigin_pixelart Fakémon

**Artwork credit: onigin_pixelart**, from the supplied filename `onigin_pixelart.png`.

The supplied 512×192 sheet contains nine complete front/back pairs. Original pixels are retained at native 64×64 resolution, using a shared GBA palette for each pair.

The sheet does not supply names, stats, types, evolutions, learnsets, shiny palettes, icons, or walking frames. Those gameplay definitions and shiny recolors are Brisk adaptations, not claims about the artist’s canon. Icons are reduced from the supplied front view; followers use static directional front/back fallbacks.

## Kanto locations

| Pokémon | Type | Evolution | Location | Method |
| --- | --- | --- | --- | --- |
| Budchirp | GRASS / FLYING | Verdwing at level 32 | VIRIDIAN_FOREST | Walking |
| Verdwing | GRASS / FLYING | Final stage | ROUTE14 | Walking |
| Rosplash | WATER / FAIRY | Rosafin at level 30 | ROUTE6 | Surf |
| Rosafin | WATER / FAIRY | Final stage | ROUTE25 | Surf |
| Ferrunt | STEEL / DRAGON | Ferradon at level 28 | ROCK_TUNNEL_1F | Walking |
| Ferradon | STEEL / DRAGON | Ferragon at level 48 | ROCK_TUNNEL_B1F | Walking |
| Ferragon | STEEL / DRAGON | Final stage | VICTORY_ROAD_2F | Walking |
| Jellume | WATER / ELECTRIC | Voltijelly at level 34 | ROUTE19 | Surf |
| Voltijelly | WATER / ELECTRIC | Final stage | ROUTE20 | Surf |

All nine are directly obtainable at the existing Kanto postgame levels. Existing Mikitari placements remain intact, and the reserved future encounter slots remain available.

Run `python tools/brisk/import_onigin.py` to regenerate sprites, gameplay definitions and encounters from the checked-in source sheet and `tools/brisk/onigin_species.json`.
