# Kanto's visiting Hoenn rivals

Their original Hoenn battles are now developed into the same team identity; see [the Hoenn rival progression](hoenn-rivals.md).

The FireRed/LeafGreen Blue, Oak and Rocket plot scenes remain **disabled**. In their place, the Hoenn rival (May for a male player, Brendan for a female player) and Wally offer high-level, **talk-to-battle** challenges. They do not demand badges, reset Hoenn rival story flags, force map cutscenes, or interfere with Team Xiphos. The Champion encounter is located after Lance, but the original Oak/Blue ending does not play.

## Encounters

| Character | Location | Battle |
| --- | --- | --- |
| May / Brendan | Route 22 | First Kanto challenge |
| Wally | Cerulean City | First Wally challenge |
| May / Brendan | S.S. Anne 2F corridor | Second rival challenge |
| Wally | Pokémon Tower 1F, Lavender | Second Wally challenge |
| May / Brendan | Silph Co. 7F | Third rival challenge |
| May / Brendan | Route 22, revisited after Wally in Lavender | Optional late rival challenge |
| Wally | Victory Road 2F | Final Wally challenge, Mega Gallade |
| May / Brendan | Pokémon League Champion's Room | Champion battle; one rematch |

All opponents have six fully evolved Pokémon and custom competitive sets. The visiting Hoenn rival's last Pokémon is the fully evolved starter effective against the player's original starter: Blaziken versus Treecko, Swampert versus Torchic and Sceptile versus Mudkip. In the Champion encounter their starter holds the matching Mega Stone. Wally keeps Gallade as their signature ace and Mega evolves it in Victory Road.

Kanto's original rival opponent IDs are reused under new aliases in `include/constants/brisk_kanto_rival_ids.h`. These are saved in the **Kanto** trainer-defeat extension, separately from existing Hoenn rival and Wally victories. The custom combat teams are in `src/data/brisk_kanto_rivals_teams.h`, loaded by `src/data/brisk_kanto_rivals_loader.h`; they override the legacy generated Blue battle parties at the moment battle starts, without changing other Kanto teams.

## May and Brendan's updated Hoenn-centered rosters

These teams deliberately **do not reuse any species from Jessie's five final-boss rosters**. May and Brendan share the same stage-specific team composition; their names, sprites and battle dialogue change based on the player character. Their sixth slot remains the rival's appropriate Hoenn starter (Sceptile, Blaziken or Swampert).

| Encounter | Pokémon 1–5, followed by their starter ace |
| --- | --- |
| First Route 22 | Flygon, Azumarill, Crobat, Lucario, Metagross |
| S.S. Anne | Metagross, Milotic, Garchomp, Manectric, Breloom |
| Silph Co. 7F | Metagross, Garchomp, Salamence, Milotic, Breloom |
| Route 22 revisited | Milotic, Tyranitar, Garchomp, Metagross, Salamence |
| Champion | Metagross, Milotic, Salamence, Volcarona, Breloom |
| Champion rematch | Garchomp, Tyranitar, Metagross, Volcarona, Milotic |

Milotic uses Marvel Scale, Leftovers, Recover and Haze for defensive stability; Breloom applies Spore and Technician-boosted attacks; Salamence can sweep with Dragon Dance; Volcarona uses Quiver Dance; and Tyranitar threatens with Sand Stream and Dragon Dance. Metagross remains the rival's recurring bulky attacker, and the Mega-capable starter is reserved for the Champion fight. This creates a more recognizable Hoenn rivalry without diluting Jessie's distinctive Xiphos lineup.

### Levels

| Stage | Easy | Normal | Hard | Expert | Kaizo |
| --- | ---: | ---: | ---: | ---: | ---: |
| Route 22 (first) | 90 | 94 | 97 | 99 | 100 |
| Cerulean Wally | 91 | 95 | 98 | 100 | 100 |
| S.S. Anne rival | 92 | 96 | 99 | 100 | 100 |
| Lavender Wally | 93 | 97 | 100 | 100 | 100 |
| Silph Co. rival | 94 | 98 | 100 | 100 | 100 |
| Route 22 (late), Victory Road Wally | 95 | 99 | 100 | 100 | 100 |
| Champion and rematch | 96 | 100 | 100 | 100 | 100 |

### Validation

Run `python3 tools/brisk/check_kanto_rivals.py` and the existing Kanto postgame validators, then build the ROM. In an emulator, verify that all seven actors are reachable, genders select correct overworld and battle sprites, battles save their independent defeats, the ship and Silph floors remain accessible without FRLG quest flags, the Champion can be reached after Lance, and the Hall of Fame door opens. Confirm fresh/old save behavior, each difficulty, Mega Evolution and Kanto/Hoenn flag isolation before publishing.

**Not verified yet:** a complete ROM build and emulator playthrough. Static checks do not establish that collision, scripted progression, Mega Evolution or save compatibility works in a running ROM.
