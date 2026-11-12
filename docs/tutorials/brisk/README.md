# Brisk Emerald community feature tutorials

These are **implementation guides for other ROM-hack developers**, based on the features maintained in [Pokémon Brisk Emerald](../../../README.md). They supplement — rather than replace — the upstream [pokeemerald-expansion tutorials](../quickstart.md).

**Target:** the current Brisk Emerald source (based on pokeemerald-expansion 1.17.0). Upstream projects and older forks differ. The guides show real integration points; **they are not one-file copy/paste patches**.

## Start here

1. [Before you port a Brisk feature](porting-checklist.md) — build environment, dependencies, save safety and test strategy.
2. [Let's Go-style follower auto-battles](overworld-auto-battles.md) — field-loop integration, eligible wild encounters, EXP and the 8-level safety rule.
3. [Five difficulty modes and optional level caps](difficulty-and-level-caps.md) — stable mode IDs, trainer scaling, progression and EXP caps.
4. [Configurable shiny odds](shiny-odds.md) — persistent settings, global generation hook and wild-only exceptions.
5. [Recharge the Tera Orb after battles](gimmick-recharge.md) — end-of-battle cleanup and protecting normal item requirements.
6. [Held-stone Mega Mode](mega-mode.md) — form-change events, held-item checks and save-safe conversion.
7. [Ball Capsules and send-out text](ball-capsules.md) — party UI, persistent per-Pokémon data and battle effects.

## More from upstream

- [Enable and configure visible overworld wild encounters](../how_to_overworld_wild_encounters.md)
- [Set up NPC followers](../how_to_follower_npc.md) (different from player Pokémon followers)
- [Add a Pokémon](../how_to_new_pokemon.md)
- [Add a new move](../how_to_new_move.md)
- [Use the testing system](../how_to_testing_system.md)

## How these guides are maintained

Every guide names **actual source files and test files**, notes any tight coupling, and separates a generic recipe from what Brisk implements. Feature descriptions should match `master`; do not describe proposed features as shipped.

If you improve a guide, open a documentation PR with your base expansion version, relevant commit, before/after behavior, emulator testing, and any credit/license requirements. Do not upload proprietary ROMs, commercial assets, personal save files, or third-party sprite packs without permission.
