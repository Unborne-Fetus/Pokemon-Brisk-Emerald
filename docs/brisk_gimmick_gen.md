# GIMMICK GEN

**Options page 7 → GIMMICK GEN → ON/OFF.** Press L from Options page 1 to
reach page 7. The preference is saved with the game and defaults to OFF.

When ON, an owned Tera Orb recharges after every completed battle, including
wins, losses, escapes, captures, draws and forfeits. Enabling the option also
recharges an already-spent Orb immediately. When OFF, normal healing/recharge
rules apply; switching OFF does not discard a charge already held.

Mega Evolution, Z-Moves and other battle-local gimmicks already reset between
battles. This option preserves their existing behavior and all per-battle usage
limits, required items, and location/Adventure restrictions. It does not grant
an Orb, heal Pokémon, restore PP or permit repeated Terastallization in a battle.
Recorded battle playback does not recharge the player's saved Orb.

Implementation: persistent flag `FLAG_BRISK_GIMMICK_GEN`, with the recharge hook
in completed battle resource cleanup. No save structure changes are required.
