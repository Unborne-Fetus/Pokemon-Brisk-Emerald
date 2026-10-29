# Orange Islands to Kanto

From Orange City, travel east through Routes 145 and 146. Surf from Route
146's east coast into Orange Strait, then south into its Kanto junction.
Continue west to Cinnabar Island or east toward Route 20 and Seafoam Islands.
Return by Surfing east from Cinnabar, north through Orange Strait, then west
to Route 146. The route uses seamless map connections in both directions.

The regional boundary reloads both tilesets, their palettes and animations,
and redraws the visible map when switching between Emerald and FRLG formats.
Kanto NPC graphics and palettes are available in the Emerald ROM, including
Cinnabar's residents and the trainers on the onward routes.

## Town Map and Fly

Press **L** in the Town Map, Fly screen, or PokeNav map to browse Hoenn and
Kanto. The player marker appears on the player's current region. Kanto uses
its existing FRLG map artwork, city names and route labels. Orange Strait's
Kanto junction shares Route 20's map square.

Each of Kanto's eleven cities becomes a Fly destination after visiting it.
Browsing the map does not unlock cities. Visit flags occupy previously unused
Hoenn flags and do not grant badges or start story scenes. Fly still requires
the normal Hoenn travel ability. Hoenn destinations retain their existing rules.

## Champion's visit

Kanto welcomes the player as Hoenn's Champion and savior. Residents, trainers,
Gym Leaders and the Elite Four acknowledge that achievement. Pokemon Centers
heal the party and update the local return point; marts sell postgame supplies.
Giovanni appears as Viridian's Gym Leader. Rocket, rival and Oak plot scenes
remain excluded. This is a postgame challenge region rather than the original
FRLG badge campaign: original badge rewards, forced scenes and colliding quest
variables stay inactive. Gym and League passage doors are opened by independent
map-load scripts.

Every Kanto trainer identity has four explicit parties of six Pokemon, with
different species lineups in each mode. See [the complete teams](kanto-trainer-teams.md).

| Mode | Ordinary trainers | Gym Leaders and Elite Four |
| --- | --- | --- |
| Easy | 90 | 92 |
| Normal | 94 | 96 |
| Hard | 97 | 99 |
| Kaizo | 100 | 100 |

Parties use mature species, legal moves, abilities and held items, with the
existing mode rules for IVs, EVs, AI, bag use, prices and experience. Explicit
parties retain their authored levels instead of being rebuilt by generic
trainer scaling. Hoenn's dedication, Easter egg and Champions Archive parties
are unchanged.

Kanto trainers use their own opponent IDs and saved defeat bits. Hoenn trainer,
badge and story flag positions remain fixed. The defeat bank extends SaveBlock3
inside its existing reserved sectors; previously unused bytes in old saves
initialize the new records as undefeated.

## Relocated legendary encounters

These existing Brisk encounters have moved to Kanto at level 90:

| Pokemon | Location |
| --- | --- |
| Articuno | Seafoam Islands B4F |
| Moltres | Victory Road 3F |
| Entei | Pokemon Mansion B1F |
| Suicune | Seafoam Islands 1F |
| Galarian Moltres | Pokemon Tower 7F |
| Galarian Zapdos | Route 23 |

Each keeps its original caught flag, so an already caught Pokemon does not
reappear after updating. Uncaught encounters remain retryable after a knockout,
flee or loss. Other legendary locations remain unchanged.

## Verification

Static audits check the bidirectional Surf corridor, every authored roster,
champion dialogue, unique encounter placement and accessible encounter tiles.
Emulator regression tests cover NPC resources, Surf collision, map selection,
independent defeat flags and save-extension round trips. These checks do not
replace a complete playthrough of every Kanto route and interior.
