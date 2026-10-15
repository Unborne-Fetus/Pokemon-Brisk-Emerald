# Orange Coast

The Orange Islands primary tileset and its matching Valencia Island secondary tileset are used together on all five outdoor maps.

| Area | Dimensions | Features |
| --- | --- | --- |
| Route 143 | 40 × 36 | Forest-passage landing, palm groves, beach pier |
| Route 144 | 48 × 36 | Two islands joined by a tidal boardwalk |
| Orange City | 48 × 40 | Pokémon Center, mart, two homes, garden, plaza and harbor |
| Route 145 | 48 × 36 | Winding coastal trail and fishing pier |
| Route 146 | 40 × 44 | Crescent-shaped island, lagoon pier and lookout |

Access: follow Route 142's existing forest clearing north. The passage at (14, 12) exits onto Route 143. From there, the east/west map connections lead through Route 144 → Orange City → Route 145 → Route 146. The passage is reversible. Existing Route 142 encounters and story flags are retained.

Orange City's Center uses normal healing and a dedicated whiteout respawn location. Its upper floor, shop and homes have valid return warps. The mart uses Brisk's badge-scaled inventory. Routes have optional tall-grass detours, Surf/fishing encounters, signs and residents; no new trainer/story flags are required. Base wild levels range from 60–66 on Route 143 to 66–72 on Route 146 and follow the existing difficulty/randomizer options.

The map render shows small markers for NPC positions; these are not replacements for the game's normal NPC sprites.

Run `python tools/brisk/orange_coast.py --render` to reproduce the authored maps, metadata, encounter entries, connectivity audit and previews. This is an authoring tool: rerunning it overwrites manual edits to these new maps. Porymap can edit the generated maps normally.

Tileset credits remain in `resources/team_aqua/tilesets/orange_islands/README.md` and `resources/team_aqua/tilesets/valencia_island/README.md`.
