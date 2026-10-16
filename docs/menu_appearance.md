# Menu appearance

Options page 3 has an **APPEARANCE** row with **LIGHT / DARK** choices.
Use Left/Right or A to switch. The Options palette updates immediately; B
closes and saves Options as usual. The choice persists through the existing
save-variable system (`VAR_BRISK_DARK_MODE`, formerly unused `0x40E5`).
Zero is Light, preserving the original appearance for new and existing saves.
No SaveBlock, Pokémon, or boxed-Pokémon layout changes are required.

Dark mode uses screen-specific palette roles: charcoal panels, raised slate
cards, soft white foregrounds, darker text shadows, and teal selections. It is
not a global color inversion. The same source white can remain white lettering
on a party card or become a dark dialogue surface. Options use muted unselected
choices and a teal active value, with gentler row dimming in Dark mode.

Party cards distinguish ordinary, selected, fainted, and action/switch states.
HP bars, gender symbols, EXP bars, and low-PP warnings keep their original
colors. All 20 frame designs retain their shapes, border hues, and gradient
order; their fills and outer shadows use the shared dark surface roles.
Bag, summary, and PC party panels use their own palette layouts. Icons,
Pokémon sprites, overworld tiles, battle scenery, and PC wallpaper artwork
retain their original colors. Light restores every original source palette.

Coverage includes Options and Help, shared menu and dialogue windows, Bag,
party panels, summary panels, PC party panels and shared menus, plus standard
menu palettes used by trading, Pokéblocks, and related interfaces. Custom
colored decorations stay visible as accents.

`BriskCopyMenuPaletteStyled` uses explicit screen/element roles without changing
its source. `BriskCopyMenuPalette` remains a conservative neutral-color fallback
for unclassified palettes. Standard menu palettes are recognized by source
identity; other known screens request a style explicitly.
`LoadBriskMenuPalette` preserves each caller's exact palette range, including
partial ranges crossing a 16-color boundary. Dedicated Pokémon and item sprite
palette loaders are not routed through the theme. Window/frame getters keep
independent dark caches so simultaneously used frames do not alias each other.
