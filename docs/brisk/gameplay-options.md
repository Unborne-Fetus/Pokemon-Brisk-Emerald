# Gameplay options and Ball Capsules

Use L/R in Options to change pages. Settings are stored with the save.

- Page 2: MOVE PROMPTS OFF skips the replacement question and opens move selection directly. Canceling move selection still lets you decline the move. LEVEL CAPS OFF removes progression caps up to level 100. REMATCHES ON lets you speak to defeated ordinary trainers to battle them repeatedly.
- Page 3: SHINY ODDS offers 1/8192, 1/4096, 1/2048, 1/1024, 1/512, 1/256, 1/128, 1/100 and 1/50. These are base odds per roll; existing charm/DexNav bonuses still apply. Existing Pokémon keep their shiny status. SHINY HUNT ON accelerates overworld spawn turnover while preserving protected shiny and static encounters.
- Full black screen fades and the battle intro flashes are faster. Partial color effects retain their original timing.
- Custom 80x80 Mega back sprites sit four pixels lower.

## Ball Capsules

Open Party → Pokémon → ITEM → BALL CAPSULE. Each Pokémon has three seal slots. Use Up/Down to select a slot and Left/Right to select NONE, HEARTS, STARS or BUBBLES. A saves; B discards edits. Slots combine when the Pokémon leaves its ball. Seals are reusable and free. Set all three slots to NONE to remove the effect. Capsules persist when boxing Pokémon and do not replace their held items.

## Save and story compatibility

The new options use previously unused save variables. Capsule data uses six unused bits inside the existing boxed Pokémon structure; its size and checksum behavior are unchanged. Old saves default to prompts and caps on, rematches and hunting off, and the existing 1/512 shiny baseline.

Rematches keep trainer defeat and story flags intact and end before the trainer's original post-battle continuation, preventing repeated story rewards or events. Trainers hidden or blocked by their map's story script remain governed by that script. First victories keep their original behavior.

Multi-use Exp Candy checks intermediate level-up moves without temporarily changing the Pokémon's saved level, so declining or accepting moves cannot leave it at an intermediate level.

## Difficulty and randomizer (page 4)

DIFFICULTY cycles through Normal, Easy, Hard and Extreme. The 48 Pokémon levels in the 26 trainer parties previously reduced through Roxanne have been restored exactly from the pre-reduction data. Difficulty is applied once when generating NPC trainer teams, wild/static encounters and scripted gifts; it does not rewrite owned Pokémon's levels or the source teams.

| Mode | Level adjustment |
| --- | --- |
| Normal (default) | Original level |
| Easy | Half, rounded up; minimum 1 |
| Hard | Original + 10–20 |
| Extreme | Original + 40–50 |

Levels stop at 100. Hard/Extreme offsets are deterministic for each generated entry in the save. The level-cap option remains independent.

Randomizer switches can be combined:

- RANDOM WILD changes wild and static species, including fishing and overworld spawns.
- TRAINERS changes ordinary NPC trainer species and gives them valid moves and abilities for the resulting species. Gym Leaders, rivals, Elite Four, Champions and villain bosses/admins are controlled separately by IMPORTANT.
- ITEMS changes ordinary field/hidden item pickups, standard item gifts, normal shop inventories and generated held items. Key items, HMs and other items marked important or unsellable remain available unchanged. Shop labels, descriptions, prices and purchases all use the randomized item.
- IMPORTANT changes important trainer teams without clearing story or defeat flags.
- EVERYTHING enables all of the above and also changes starter/gift/egg species, species types, abilities, base stats, level-up/TM/egg learnsets and evolution tables. Each trait uses a deterministic donor species. Existing Pokémon retain their identities and moves; their live trait data follows the switch, and party stats refresh when Options closes. Boxing/retrieving recalculates stats normally.

Results are repeatable for the save's trainer ID; toggling a category does not reroll it. Disabled Pokémon, Eggs and temporary battle forms cannot be selected as randomized species. Story scripts, flags, required items, map entrances and warps are unchanged. Switching Everything off restores original species trait tables; already acquired Pokémon and items remain acquired.

## Follow-up fixes

The capsule editor now renders two persistent, instant lines with D-pad icons, avoiding scrolling, truncation and automatic clearing. Accepting a candy level-up move continues processing remaining moves and then checks evolution. Opening animations run two animation ticks per display frame. Shiny Hunt replacement waits are reduced from 45 to 8 ticks, and ordinary refill waits from 6+3 per active spawn to 1+1 per active spawn. Protected shiny/static encounters are still retained.
