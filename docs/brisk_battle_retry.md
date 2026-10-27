# Battle retry

After losing or drawing a normal trainer or wild battle, the battle message window
asks: **Retry with a healed party? A: Retry B: Continue**.

A restarts the same fight without the blackout trip, loss dialogue or money penalty.
Your starting party is restored and fully healed (HP, PP and status). Attempt-only
EXP, learned moves, party reordering and held-item changes are rolled back. The
original enemy parties and Bag contents are restored too, so repeated attempts do
not consume supplies or reroll a trainer's team. Tera charge returns to its entry
state; retry does not grant an Orb or charge one that was spent before the fight.
B follows the original loss sequence. Every new attempt offers the choice again.

Link/recorded battles, facilities, rentals, raids, roamers, tutorials, ghost battles,
sky battles and battles with partners use their existing result flows. A wild
battle in which a Pokemon was already caught also uses its normal result flow,
so retry cannot duplicate a catch already transferred to storage.

# Trainer difficulty

Ordinary and story trainers now have separate mode curves. See
`brisk_ordinary_level_spreads.md`, `brisk_story_and_balls.md` and the complete
`brisk_trainer_mode_plan.md` for the current authored teams, mode levels, badge-cap
rules and dedication/Easter egg exemptions. Retry always retains the generated
opponent teams for the selected mode.
