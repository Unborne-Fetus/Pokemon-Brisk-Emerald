# Expert difficulty (fifth mode)

Expert is designed for players who find Easy and Normal too forgiving, but
find Hard and Kaizo too punishing even with carefully trained teams.

## Selection and compatibility

The Options difficulty selector cycles **Easy → Normal → Expert → Hard → Kaizo**.
The persistent game variable keeps the historical values untouched:

| Game mode | Saved value | Expansion trainer row |
|---|---:|---|
| Normal | 0 | Normal |
| Easy | 1 | Easy |
| Hard | 2 | Hard |
| Kaizo | 3 | Kaizo |
| Expert | 4 | Expert — independently compiled fifth trainer row |

This makes old single saves and the two-slot profiles forward-compatible without
rewriting the serialized Bag or party save data. The build now generates a
fifth trainer-row entry for every Hoenn and postgame Kanto trainer.

## Rules

- **Fuller teams:** early trainers ramp up progressively; ordinary trainers at
  higher levels and late story opponents can have six members. The existing
  Expert roster is generated deterministically from the authored Hard, Normal
  and Kaizo compositions as a full separate trainerproc record; postgame teams
  can be completed to six members while preserving the original ace last.
  Tutorial battles and two-opponent half-team limits are respected.
- **Fairer opponents:** Expert removes all trainer AI omniscience and
  move prediction, retaining sensible switching, opponent selection,
  HP awareness, coverage and KO evaluation.
- **Reduced raw stat pressure:** standard authored Hard opponents use an
  intermediate IV ceiling of 28 and 75% of their assigned EVs. Ordinary
  and story opponents under level 100 lose a small number of levels.
  Dynamically generated opponents use 28 IVs and moderate EV budgets.
- **More accessible player rules:** Set battle style, battle consumables
  permitted, shop prices x3, normal EXP Share rate (100%), and optional
  level caps. This is not Kaizo's hard EXP lock.
- **Dynamax Adventures:** standard boss HP multiplier; recover 75% of
  maximum HP between rounds (less than Easy/Normal, more than Hard).
- **Scott's Trainer Summit:** Expert appears in its difficulty selector.
- **Kanto postgame:** all 623 Kanto trainer identities receive their own
  compiled Expert party record, with six-member postgame parties.
- **Protected encounters:** dedication/Easter egg trainers, the Champions
  Archive, the five Hoenn League teams, Meteor Falls Steven and Kaizo-exclusive
  trainers keep their protected roster behavior. League is still level 100
  outside Easy; Kaizo-only opponents are not unlocked.

## Verification

`test/brisk_expert_mode.c` checks the stored mode ID, trainer-row mapping,
battle settings and AI restrictions, party integrity and final-ace
preservation. \`python3 tools/brisk/build_expert_teams.py --check\` validates
that the fifth-mode source exists and matches the four-mode source data. Review the Build Pokemon Brisk Emerald and CI GitHub Actions
runs before publishing a build. Runtime playtesting is still required to
confirm intended difficulty across different badges and save histories.
