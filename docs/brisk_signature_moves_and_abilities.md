# Brisk Emerald: signature abilities

These additions are assigned to Pokémon in the GBA version of Brisk Emerald.

| Pokémon | Signature ability | Effect |
|---|---|---|
| Spidops | Web Excretant | On entering battle, sharply raises Spidops's Speed (+2 stages) and lowers every *other active battler's* Evasion (-1 stage). In Double Battles this includes Spidops's partner; Spidops's own Evasion is untouched. |
| Kricketune | Grand Maestro | When using damaging sound moves, offensive damage calculation uses the highest stage-adjusted non-HP stat among Attack, Defense, Sp. Attack, Sp. Defense, and Speed. The move retains its normal physical/special category. |
| Luvdisc | Heartbreaker | When an opposing move knocks Luvdisc out, the attacking opponent takes damage equal to half its own maximum HP (minimum one). The other opponent and allies are not affected. |

**Web Excretant is an Ability, not a move.** It occupies Spidops's first ability slot (replacing Insomnia); its hidden ability remains Stakeout. It triggers on entry, including subsequent switches. It applies ordinary stat-change rules, including the stat stage cap and protective abilities. There is no custom move, learnset entry, or PP cost.

Grand Maestro replaces Kricketune's first ability slot (Swarm); Technician remains its hidden ability. Heartbreaker replaces Luvdisc's first ability slot (Swift Swim); Hydration remains its hidden ability.

Heartbreaker uses the engine's move-end retaliation hook. Indirect fainting from environmental effects (such as poison or weather) is not covered by that hook. The ability respects the engine's normal passive-damage protections.

Regression tests:
- `test/battle/ability/web_excretant.c`
- `test/battle/ability/grand_maestro.c`
- `test/battle/ability/heartbreaker.c`

Run the project's GBA battle tests in the native build environment; tests have not been executed in the GitHub connector session.
