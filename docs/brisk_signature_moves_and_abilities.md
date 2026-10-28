# Brisk Emerald: signature additions

These are authored on `master` for the following Pokémon:

| Pokémon | Signature | Effect |
|---|---|---|
| Spidops | Web Excretant (Bug, Status, 10 PP) | Raises the user's Speed by two stages; lowers every *other active battler's* Evasion by one stage, including the user's doubles partner. Does not lower the user's Evasion. |
| Kricketune | Grand Maestro (ability) | When using damaging sound moves, the offensive calculation uses the highest currently stage-adjusted stat among Attack, Defense, Sp. Attack, Sp. Defense, and Speed instead of the normal attacking stat. HP is excluded. The move retains its normal physical/special category and targets the defender's corresponding defensive stat. |
| Luvdisc | Heartbreaker (ability) | When an opponent knocks Luvdisc out with a move, that attacking opponent takes damage equal to half its own maximum HP (minimum one). In doubles, the other opponent and the user's ally are not damaged. |

Web Excretant is learned by **Spidops at level 35** and can be relearned. The move reuses Sticky Web's battle animation; there is no separate art asset. It is a fieldwide stat move and does not create the normal entry-hazard Sticky Web.

Grand Maestro replaces Kricketune's first ability slot (Swarm). Technician remains its hidden ability. Heartbreaker replaces Luvdisc's first ability slot (Swift Swim). Hydration remains its hidden ability.

The Heartbreaker code currently uses the battle engine's move-end retaliation hook (the same class of hook used by Innards Out). Environmental fainting such as residual poison or entry hazards is **not** handled in that hook. It also respects the engine's normal passive-damage protections.

Regression test cases:
- `test/battle/move_effect/web_excretant.c`
- `test/battle/ability/grand_maestro.c`
- `test/battle/ability/heartbreaker.c`

Compile and execute the GBA battle test suite on the normal development environment; these tests have not been run in this connector environment.
