# Menu appearance

Options page 3 has an **APPEARANCE** row with **LIGHT / DARK** choices.
Use Left/Right or A to switch. The Options palette updates immediately; B
closes and saves Options as usual. The choice persists through the existing
save-variable system (`VAR_BRISK_DARK_MODE`, formerly unused `0x40E5`).
Zero is Light, preserving the original appearance for new and existing saves.
No SaveBlock, Pokémon, or boxed-Pokémon layout changes are required.

Dark mode recolors neutral UI backgrounds and foreground text into slate
panels and light text. Colored accents, transparent palette entries, icons,
Pokémon sprites, overworld tiles, battle scenery, and PC wallpaper artwork
retain their original colors. Light restores the original source palettes.

Coverage includes Options and Help, shared menu and dialogue windows, Bag,
party panels, summary panels, PC party panels and shared menus, plus standard
menu palettes used by trading, Pokéblocks, and related interfaces. Custom
colored decorations stay visible as accents.

`BriskCopyMenuPalette` handles neutral UI colors without changing its source.
`LoadBriskMenuPalette` preserves each caller's exact palette range, including
partial ranges crossing a 16-color boundary. Dedicated Pokémon and item sprite
palette loaders are not routed through the theme. Window/frame getters keep
independent dark caches so simultaneously used frames do not alias each other.
