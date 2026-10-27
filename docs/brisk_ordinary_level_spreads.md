# Ordinary trainer level spreads

Allen now leads with Galarian Slowpoke and has a lower-level Rattata:

| Pokemon | Easy | Normal | Hard | Extreme |
| --- | --- | --- | --- | --- |
| Galarian Slowpoke | 7 | 13 | 15 | 18 |
| Rattata | 6 | 11 | 13 | 15 |

414 ordinary team definitions were adjusted. All 430 ordinary multi-Pokemon teams
now have a level spread; the 240 single-Pokemon teams use the same ordinary mode
curve. Route bands, species evolution/stat limits, legal moves and appropriate
IVs, EVs and held items are validated after the edits. The later story pass narrows exemptions to 27 dedication/Easter egg teams; their
fingerprints and previous runtime scaling remain unchanged.

Ordinary Hard levels add 20% (at least 2), and Extreme adds 40% (at least 4).
Easy halves levels, rounded up, with a one-level separation when a lower support
would otherwise round to the ace. When a Hard/Extreme ace exceeds the badge cap,
each support stays below the cap by its authored gap from the ace. This preserves
a level spread while ensuring Extreme never lowers a member below its Hard level. Levels never exceed 100 or fall below 1.

The complete per-team plan is in `brisk_trainer_mode_plan.md`. Its Hard/Extreme
columns assume cap 100; earlier badge caps still apply. The fairness audit checks
that every multi-Pokemon ordinary team stays varied in all modes and at all badge
caps. Story, rival, gym and League trainers now have their own stronger curve. Only
dedication and Easter egg trainers keep their previous definitions and scaling.
