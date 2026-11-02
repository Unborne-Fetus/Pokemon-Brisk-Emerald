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
| Expert | 4 | Compact per-trainer Expert alternate, backed by Hard plus an authored source member |

This makes old single saves and the two-slot profiles forward-compatible without
rewriting the serialized Bag or party save data. A full fifth trainer-table
row exceeded the GBA's 32 MB ROM limit, so Expert now uses a compact authored
replacement for eligible trainer teams. The original four tables remain.

## Rules

- **Fuller teams:** early trainers ramp up progressively; ordinary trainers at
  higher levels and late story opponents can have six members. The
  Expert roster uses the authored Hard party as its base, substituting one
  battle-ready Pokemon from the trainer's Normal, Kaizo or Easy sets. The
  substitutions are stored explicitly in a compact per-trainer index; no
  arbitrary runtime random team expansion occurs.
  Tutorial battles and two-opponent half-team limits are respected.
- **Fairer opponents:** Expert removes all trainer AI omniscience and
  move prediction, retaining sensible switching, opponent selection,
  HP awareness, coverage and KO evaluation.
- **Reduced raw stat pressure:** standard authored Expert opponents use an
  intermediate IV ceiling of 28 and 75% of their assigned EVs. Ordinary
  and story opponents under level 100 lose a small number of levels.
  Dynamically generated opponents use 28 IVs and moderate EV budgets.
- **More accessible player rules:** Set battle style, battle consumables
  permitted, shop prices x3, normal EXP Share rate (100%), and optional
  level caps. This is not Kaizo's hard EXP lock.
- **Dynamax Adventures:** standard boss HP multiplier; recover 75% of
  maximum HP between rounds (less than Easy/Normal, more than Hard).
- **Scott's Trainer Summit:** Expert appears in its difficulty selector.
- **Kanto postgame:** all eligible Kanto trainer identities participate in
  compact Expert party selection and keep their six-member postgame format.
- **Protected encounters:** dedication/Easter egg trainers, the Champions
  Archive, the five Hoenn League teams, Meteor Falls Steven and Kaizo-exclusive
  trainers keep their protected roster behavior. League is still level 100
  outside Easy; Kaizo-only opponents are not unlocked.

## Verification

`test/brisk_expert_mode.c` checks the stored mode ID, trainer-row mapping,
battle settings and AI restrictions, party integrity and final-ace
preservation. `python3 tools/brisk/build_compact_expert.py --check` validates
that each compact Expert variant matches the four existing mode sources.
Review the Build Pokemon Brisk Emerald and CI GitHub Actions runs before
publishing a build. Runtime playtesting is still required to
confirm intended difficulty across different badges and save histories.
