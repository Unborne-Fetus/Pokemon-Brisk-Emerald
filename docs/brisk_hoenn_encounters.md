# Hoenn wild encounters and EXP Candy progression

Hoenn now has authored habitat cores rather than shuffled species. Each land
table has eight resident species occupying sixteen slots, followed by eight
visitor slots. Visitors preserve every species from the previous ordinary Hoenn
tables, using the game's evolution requirements, stat strength, and habitat.
Every active slot contains a Pokémon. Land and Surf retain their final sentinel.

| Area | Resident examples | Wild levels before mode scaling |
| --- | --- | --- |
| Route 101 | Poochyena, Zigzagoon, Wurmple, Taillow | 3–5 |
| Route 102 | Lotad, Seedot, Ralts, Surskit, Wooper | 5–8 |
| Route 103 | Wingull, Poochyena, Bunnelby, Hoppip | 5–8 |
| Route 104 / Petalburg Woods | Forest bugs, Shroomish, Pineco, Applin | 7–12 |
| Route 116 / Rusturf Tunnel | Nincada, Whismur, Skitty / cave residents | 10–15 |
| Granite Cave | Zubat, Aron, Makuhita, Nosepass | 13–18 |
| Route 110 | Electrike, Plusle, Minun, Gulpin | 15–19 |
| Route 117 | Roselia, Volbeat, Illumise, Petilil | 16–21 |
| Route 111 desert | Sandshrew, Trapinch, Cacnea, Baltoy | 22–28 |
| Fiery Path / Jagged Pass | Numel, Slugma, Torkoal, Houndour | 21–30 |
| Route 119–120 | Rainforest plants, bugs and ambush predators | 29–39 |
| Mt. Pyre | Ghost and spirit Pokémon | 33–41 |
| New Mauville | Electric and industrial Pokémon | 32–45 |
| Shoal Cave | Ice Pokémon, seals and cold-water residents | 35–44 |
| Deep Meteor Falls | Dragon families and compatible cave visitors | 42–54 |
| Sky Pillar | Dragon families and powerful late visitors | 48–58 |

Visitors can have higher minimum levels when their evolution requires it.
Rivers, oceans, underwater areas and icy water have separate Surf pools.
Old Rod encounters stay at 5–10, Good Rod at 18–25, and Super Rod starts at
32–42; a visitor's evolution minimum can raise its level. Early-route Surf
requires later progression and therefore uses later levels than that route's grass.

Kanto, Orange Islands, Route 135 onward, Evolution Cave's starter/form
collections, Altering Cave's Pikachu collection, Victory Road's Paradox
encounters, scripted rewards, and battle facilities retain their authored content.
Unused Ruby/Sapphire maps are excluded from availability guarantees.

## EXP Candy shops

| Candy | Required badges |
| --- | --- |
| XS | 3 |
| S | 4 |
| M | 5 |
| L | 7 |
| XL | 8 |

All difficulties use these purchase gates. Item criteria filter both automatic
badge-based stock and fixed inventories, including the Lilycove rooftop shop.
Candies already owned remain usable under the existing Hard/Kaizo EXP caps.
Kaizo's first badge cap remains 25; this change removes unlimited early candy
purchases without changing gym teams or lowering levels on existing saves.

Run `python3 tools/brisk/author_hoenn_encounters.py` to regenerate the tables;
run with `--check` to verify determinism, species coverage and protected content.
`data/brisk_hoenn_encounter_rebalance.json` records all 525 previous species
and a hash of protected tables. Edit the curated pools in the generator, not its
JSON output. The in-game Pokédex and DexNav consume the generated wild tables.
