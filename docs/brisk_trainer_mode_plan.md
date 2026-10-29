# Brisk trainer plan by difficulty

Hoenn now uses explicit Easy, Normal, Hard and Kaizo trainer records in
`src/data/brisk_mode_teams.party`. These records are authoritative: the game
does not expand, scale, devolve or optimize their parties at battle time.

See [every trainer and mode roster](brisk_authored_mode_teams.md) for the
complete species and level list, including rivals, gym rematches and bosses.

Dedication/Easter egg trainers retain their complete original teams in all
four modes: party size, species, levels, moves, items, training and cosmetics.

| Mode | Team design |
| --- | --- |
| Easy | Smaller traditional teams, modest moves, no EV investment |
| Normal | Original encounter species plus fixed themed supports |
| Hard | Stronger themed rosters, role sets, trained support and sweepers |
| Kaizo | Coordinated teams, full training, competitive items and support |

Hard/Kaizo levels are authored within each encounter’s intended badge band;
they do not change when an old route is revisited with additional badges.
The opening Route 103 starter tutorials retain one starter. Metadata and
Frontier identity templates retain their existing purpose. Hooded Figure and
Tankerson remain Kaizo-only. League levels are 70 on Easy and 100 otherwise;
Meteor Falls Steven is level 100 in every mode. Two-opponent half-team
formats may limit each trainer to three members.

Existing mode AI, battle style, EXP, item-use and randomizer settings remain
separate from the authored rosters. Randomizer options still apply when enabled.

Validate with `python tools/brisk/author_mode_teams.py --check`.
Regenerating with `--write` replaces manual roster edits with the curated
archetype output; ordinary builds and validation never regenerate the rosters.

## Champions Archive: one unchanged team, maximum AI

All nine Champions Archive fights on Route 135 — Blue, Red, Wallace, Cynthia,
Iris, Diantha, Leon, Nemona and Volo — load their existing Normal/base roster
in **every difficulty mode**. Species, party order, levels, moves, items,
abilities, EVs/IVs, natures, gimmicks, shinies, and other trainer attributes
are unchanged. The only adjustment is the AI: the strongest available
omniscient decision-making, strategic switching, prediction of moves and
switch-ins, full opponent-party knowledge, PP-stall awareness, HP/KO planning,
and smart Terastallization. Random party ordering, forced setup and ace-order
restrictions are deliberately excluded.

This exception applies only while battling these trainers **inside the
Archive**. Wallace retains his Hoenn League Easy-level-70 variant and the
normal difficulty rules when encountered elsewhere. Other dedication/Easter
egg trainers retain their existing AI behavior.
