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

Normal and Easy retain their rules and authored teams. Hard trainer levels gain
20% (minimum 3); Extreme gains 30% (minimum 4). Both still respect the badge cap.
Wild level scaling remains unchanged. Hard and Extreme now use smart switching,
HP-aware scoring, 2HKO checks, smarter Tera timing and PP-stall prevention.
Extreme retains full opponent knowledge; Hard uses its existing observed knowledge.

Ordinary trainers on Hard now get optimized legal move coverage within the existing
level-based move-power limits, plus stronger abilities. Important/dedication teams
retain their authored Hard moves. Hard training budgets grow from 128 EVs below
level 20, to 256 below 35, 384 below 50, and 510 thereafter; an authored higher total
is preserved as the budget floor. Investment follows the actual moves and species
strengths. IVs remain perfect on Hard/Extreme. Extreme keeps its 510-EV training.
No authored team species, party sizes, or special trainer definitions are edited.
