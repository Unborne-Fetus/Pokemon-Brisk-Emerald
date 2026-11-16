# Team Xiphos Aegis — playable Kanto encounter wiring

Rocket overworld/battle art is temporary. New Xiphos scripts avoid re-enabling historical Rocket and rival quest events. Four existing commanders plus twelve newly authored trainers are fought with independent parties in Easy, Normal, Hard, Expert and Kaizo.

The authored trainer rosters are in `src/data/brisk_xiphos_teams.h` and `src/data/brisk_xiphos_extended_teams.h`; the five-mode loader is `src/data/brisk_xiphos_loader.h`.

Director Jessie waits in Rocket Hideout B4F and requires defeat of all four specialist commanders. Defeating them unlocks the Kanto Classic Red/Leaf outfit based on the player's selected character and awards a one-time level-70 Magearna (claimable later if storage is full). Their team opens with Aegislash and Archaludon in every mode and saves Mega Magearna as the final roster slot; Iron Valiant takes the fifth slot. Roster order guarantees the opening pair in a normal trainer doubles start, but the final slot does not guarantee Magearna will be the last surviving Pokémon.

The mapjson sanitizer has a strictly scoped exception for `XiphosKanto_EventScript_` actors so old Team Rocket/Rival content stays disabled. Running the Kanto postgame dialogue authoring script retains those actors.

**Validation required:** build ROM, verify all 16 entries, confirm tiles are reachable and not blocked by old map events, confirm flags in old and new saves, and test Jessie unlock in emulator.

## Temporary encounter placements

| Trainer | Kanto location |
| --- | --- |
| AGENT A | ViridianCity |
| AGENT B | PewterCity |
| AGENT C | MtMoon 1F |
| AGENT D | CeruleanCity |
| AGENT E | VermilionCity |
| AGENT F | CeladonCity |
| AGENT G | LavenderTown |
| SCIENTIST A | SaffronCity |
| SCIENTIST B | FuchsiaCity |
| SENIOR A | CinnabarIsland |
| SENIOR B | IndigoPlateau Exterior |
| COMMANDER FAIRY | CeladonCity GameCorner |
| COMMANDER FIGHTING | SaffronCity Dojo |
| COMMANDER STEEL | SilphCo 11F |
| COMMANDER DRAGON | VictoryRoad 2F |
| JESSIE | RocketHideout B4F |
