# Team Xiphos Aegis — playable Kanto encounter wiring

Rocket overworld/battle art is temporary. New Xiphos scripts avoid re-enabling historical Rocket and rival quest events. Four existing commanders plus twelve newly authored trainers are fought with independent parties in Easy, Normal, Hard, Expert and Kaizo.

The authored trainer rosters are in `src/data/brisk_xiphos_teams.h` and `src/data/brisk_xiphos_extended_teams.h`; the five-mode loader is `src/data/brisk_xiphos_loader.h`.

Director Jessie waits in Rocket Hideout B4F and requires defeat of all four specialist commanders. A first victory unlocks the Kanto Classic Red/Leaf outfits through the existing idempotent reward script. The intended synchronized Aegislash/Archaludon finale still requires a dedicated battle-engine sequence; roster ordering alone does not force it.

The mapjson sanitizer has a strictly scoped exception for `XiphosKanto_EventScript_` actors so old Team Rocket/Rival content stays disabled. Running the Kanto postgame dialogue authoring script retains those actors.

**Validation required:** build ROM, verify all 16 entries, confirm tiles are reachable and not blocked by old map events, confirm flags in old and new saves, and test Jessie unlock in emulator.
