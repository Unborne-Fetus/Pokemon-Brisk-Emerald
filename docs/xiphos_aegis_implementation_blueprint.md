# Team Xiphos Aegis — implementation blueprint (2026-11-02)

Status: **design/data specification only; NOT integrated into trainer engine or battle engine**. Do not consider this feature shipped.

## Scope and distinction

These are **new postgame Kanto Team Xiphos Aegis trainers**. Do not apply the enemy-only ability to Hoenn's Team Aqua or Team Magma, wild encounters, player Pokémon, PvP, gifts, or rental Pokémon. Every Xiphos Pokémon gets Containment Protocol **as a battle-scoped modifier**, unless an explicit exception is listed. **Jessie's Aegislash keeps Stance Change**. The Mega Magearna request requires identifying its existing Mega evolution item, ability and actual supported trainer-gimmick logic before activation is implemented. Magearna should be Jessie's only Mega. Other Xiphos members use no Mega Evolutions or Z-Moves.

## Containment Protocol — battle-scoped rules

1. Applies to a Pokémon only while it is controlled by a qualifying Xiphos trainer **on the NPC/opposing team**, determined from the *trainer ID* and battler ownership, never from the species or data saved in Pokémon.
2. Increases protection: halve the damage of the **first successful, direct damaging hit** taken during each switch-in. Then apply +2 Attack, +2 Sp. Atk, +2 Speed to its user (subject to stat limits), once per switch-in.
3. No boost from status moves, entry hazards, end-of-turn weather, poison, self-inflicted/recoil damage, substitute-only hits, or unsuccessful/no-damage moves. Resolve hit/HP damage and activation in a consistent phase; define multi-hit attacks as first damaging hit only.
4. Reset the consumed flag when that specific battler Pokémon switches out, not every turn. Handle double battles and both opponent slots independently.
5. The modifier is **not a species ability and not a stored or inheritable MON_DATA_ABILITY**. Avoid modifying gEnemyParty or Pokémon save structures. It must not be copied with Trace/Role Play/Skill Swap, inherited by Receiver/Power of Alchemy, suppressed by Neutralizing Gas, retained by Transform, or transferred via trade/capture. Treat it as trainer technology, not an actual innate ability.
6. Present a visible text/activation notification for fairness. Test damage calculation and AI estimations against the modifier so they agree. In Double Battles, the first damage received must trigger only that recipient's protection.
7. **Exceptions:** Jessie Aegislash: Stance Change, no modifier. Other exceptions require an explicit ID/party-slot entry. Gimmicks must not clear/reset the flag mid-switch-in.
8. Do not apply overlay in battle replay, multiplayer, battle facilities, or battle-partner slots unless specifically enabled.

## Engine inspection already confirmed

- `src/data/trainers.party` has per-trainer party syntax including `Gigantamax: Yes`, `Dynamax Level: N`, and `Tera Type: ...`.
- `src/battle_gimmick.c` selects Dynamax only for explicitly eligible trainer Pokémon.
- `src/brisk_modes.c` exposes the five modes `BRISK_MODE_EASY, NORMAL, HARD, EXPERT, EXTREME` (the UI calls EXTREME Kaizo).
- `src/brisk_difficulty.c` currently scales ordinary parties by difficulty. New Xiphos authored teams must bypass any global level/party-size/evolution reshuffling that contradicts this dedicated postgame spec.
- `graphics/pokemon/magearna/mega` already exists. **Graphics alone do not prove an executable trainer Mega path.**

## Five-mode roster matrix — planning targets, not yet inserted in game

Every roster is postgame, with optimized legal moves/items/EVs and deliberate overlapping counters. Levels progress across Kanto, generally Easy 90–94, Normal 93–96, Hard 95–98, Expert 97–100, Kaizo 98–100. Jessie is Lv100 on every mode. Species listed in lead-to-ace order, but actual battle leads and doubles strategies should be authored.

| Trainer archetype | Easy | Normal | Hard | Expert | Kaizo |
|---|---|---|---|---|---|
| Patrol Agent Alpha | Lucario, Flygon, Tinkaton, Metagross | Grimmsnarl, Lucario, Flygon, Metagross, Tinkaton | Grimmsnarl, Excadrill, Gallade, Flygon, Metagross, Tinkaton | Klefki, Dragapult, Corviknight, Lucario, Garchomp, Kingambit | Grimmsnarl, Garchomp, Iron Valiant, Corviknight, Kingambit, Tinkaton |
| Patrol Agent Beta | Mawile, Kingdra, Hawlucha, Scizor | Klefki, Kingdra, Hawlucha, Scizor, Goodra | Grimmsnarl, Rotom-Wash, Hawlucha, Hisuian Goodra, Scizor, Dragonite | Grimmsnarl, Rotom-Wash, Dragapult, Hawlucha, Hisuian Goodra, Kingambit | Grimmsnarl, Rotom-Wash, Dragonite, Dragapult, Hisuian Goodra, Kingambit |
| Xiphos Scientist Alpha | Klefki, Bronzong, Magnezone, Porygon-Z | Klefki, Rotom-Wash, Bronzong, Metagross, Porygon-Z | Klefki, Rotom-Wash, Corviknight, Metagross, Porygon-Z, Tinkaton | Grimmsnarl, Rotom-Wash, Corviknight, Metagross, Iron Valiant, Porygon-Z | Grimmsnarl, Rotom-Wash, Corviknight, Metagross, Iron Valiant, Kingambit |
| Senior Agent Alpha | Gardevoir, Scizor, Kingdra, Kommo-o, Metagross | Grimmsnarl, Kingdra, Gallade, Corviknight, Kommo-o, Metagross | Grimmsnarl, Rotom-Wash, Dragapult, Corviknight, Lucario, Metagross | Grimmsnarl, Rotom-Wash, Dragapult, Iron Valiant, Corviknight, Kingambit | Grimmsnarl, Rotom-Wash, Garchomp, Dragapult, Corviknight, Kingambit |
| Fairy Commander | Grimmsnarl, Togekiss, Tinkaton, Kingdra, Lucario, Hatterene | Grimmsnarl, Togekiss, Tinkaton, Rotom-Wash, Metagross, Hatterene | Grimmsnarl, Togekiss, Iron Valiant, Rotom-Wash, Metagross, Hatterene | Grimmsnarl, Whimsicott, Iron Valiant, Corviknight, Metagross, Hatterene | Grimmsnarl, Whimsicott, Iron Valiant, Rotom-Wash, Metagross, Hatterene |
| Fighting Commander | Hitmontop, Gallade, Lucario, Kingdra, Corviknight, Kommo-o | Hitmontop, Gallade, Lucario, Rotom-Wash, Corviknight, Kommo-o | Hitmontop, Iron Valiant, Lucario, Dragapult, Corviknight, Kommo-o | Hitmontop, Iron Valiant, Urshifu-Rapid-Strike, Dragapult, Corviknight, Kommo-o | Hitmontop, Iron Valiant, Urshifu-Rapid-Strike, Dragapult, Corviknight, Annihilape |
| Steel Commander | Bronzong, Corviknight, Excadrill, Kingdra, Metagross, Kingambit | Bronzong, Corviknight, Rotom-Wash, Excadrill, Metagross, Kingambit | Bronzong, Corviknight, Rotom-Wash, Excadrill, Metagross, Kingambit | Bronzong, Corviknight, Rotom-Wash, Excadrill, Iron Valiant, Kingambit | Bronzong, Corviknight, Rotom-Wash, Garchomp, Metagross, Kingambit |
| Dragon Commander | Flygon, Kingdra, Dragalge, Hisuian Goodra, Corviknight, Baxcalibur | Flygon, Kingdra, Dragalge, Hisuian Goodra, Grimmsnarl, Dragapult | Garchomp, Kingdra, Dragalge, Hisuian Goodra, Grimmsnarl, Dragapult | Garchomp, Kingdra, Dragalge, Hisuian Goodra, Grimmsnarl, Dragapult | Garchomp, Kingdra, Dragalge, Hisuian Goodra, Grimmsnarl, Dragapult |
| Director Jessie | Grimmsnarl, Kingdra, Flamigo, Magearna, Aegislash, Archaludon | Grimmsnarl, Kingdra, Flamigo, Magearna, Aegislash, Archaludon | Grimmsnarl, Rotom-Wash, Flamigo, Magearna, Aegislash, Archaludon | Grimmsnarl, Kingdra, Landorus-Therian, Magearna, Aegislash, Archaludon | Grimmsnarl, Landorus-Therian, Incineroar, Magearna, Aegislash, Archaludon |

All Commander and Jessie encounters are **Doubles** with six Pokémon. Different species may repeat between archetypes but **each placed trainer must receive an individual party**, not an identical copy. Add further patrol and scientist templates/variants when actual trainer placement is decided. For Jessie preserve the narrative intention to have Aegislash and Archaludon be the final simultaneous pair; forcing this requires battle AI/party replacement coordination, not just party order.

## Battle gimmicks

- Xiphos grunts: occasional Dynamax or Gigantamax (selected members only; G-Max only when the species has an actual form). Sample G-Max-compatible members: Grimmsnarl, Corviknight, Kingler, Machamp, Hatterene, Duraludon (not Archaludon). Confirm engine support/form availability in project data.
- Scientists: a small number of marked Dynamax, mainly for prototype containment demonstrations.
- Commanders: one deliberately chosen G-Max-capable Pokémon per commander when available, and plain Dynamax in other encounters.
- Jessie: **only Mega on the team: Mega Magearna**. No other member may Mega; no Xiphos Z-Moves. Any Jessie Dynamax should be carefully tested against the same-battle gimmick limitation and never displace her signature Mega finale.
- If Mega Magearna requires a custom Mega Stone or trigger and it is not available to trainers, add it explicitly and test. Her mega should **not** result in the player gaining a transferable Containment Protocol ability.

## Acceptance tests (currently not executed)

- Each Xiphos ID loads distinct authored teams for all five modes, without global trainer rewriting.
- Every opposing Xiphos Pokémon has Containment overlay except Jessie Aegislash; every player-owned/caught/traded Pokémon does not.
- First damaging hit reduces damage; boosts occur once only, and re-arm on genuine switch-out/switch-in.
- Protect/immune/miss/hazard/indirect damage don't consume activation; multi-hit and doubles are deterministic.
- Natural abilities needed for gimmicks/form changes retain functionality. Aegislash's Stance Change works.
- The only Mega Evolution seen across Xiphos battles is Jessie's Magearna. Zero Xiphos Z-Moves.
- G-Max species with the configured factor actually enter G-Max; others Dynamax normally.
- Defeating/capturing/trading and reloading never transfers the battle-only effect into the player's save.
- Full `make` build and battle-engine tests pass before announcing completion.
