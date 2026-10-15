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
