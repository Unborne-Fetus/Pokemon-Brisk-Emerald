# Team Xiphos Aegis: five-mode postgame battle foundation

This is a Kanto postgame trainer-roster and combat-systems patch, not the
complete Xiphos story or map event implementation.

## Trainer bank

Sixteen new trainer IDs use Kanto indices 624–639:
- Seven Xiphos Agents.
- Two Xiphos Scientists.
- Two Xiphos Senior Agents.
- Four specialist Commanders (Fairy, Fighting, Steel, Dragon).
- Director Jessie.

Every trainer has **six explicit, mode-distinct Pokémon** at levels 90–100
for Easy, Normal, Expert, Hard, Kaizo. All eighty teams reside in
`src/data/brisk_xiphos_teams.party`; the battle compiler includes this
file from `src/data/brisk_mode_teams.party`.

The Expert slot is a fifth `DIFFICULTY_EXPERT` table row. For regular
trainers with no separate Expert party, the previous Hard authored-party
fallback is retained. Sixty-three Aqua/Magma trainer IDs have unique
Expert records in `src/data/brisk_expert_villains.party`.

## Enemy-only Containment Protocol

Every Xiphos trainer has `Xiphos Protocol: Yes`. This is a trainer-level,
**battle-only modifier**, not a Pokémon species ability. On the first
positive damaging attack hit after a switch-in, the modifier halves
that hit's damage. If the Pokémon survives the attack, its Attack,
Special Attack, and Speed are raised two stages (capped at +6).

The modifier resets when the battler switches in again; attacks into
Substitute do not consume it. Xiphos Pokémon retain their natural
abilities, including Aegislash's Stance Change, Magearna's Soul-Heart
and Flamigo's Costar. The modifier does not enter caught Pokémon,
save files, breeding, Trace, or Skill Swap.

## Gimmicks

Regular Agents and higher ranks have selected Dynamax or Gigantamax
candidates per roster, rather than every Pokémon Dynamaxing.
The trainer-generation code blocks Xiphos Mega Stones, Z-Crystals and
Primal Orbs (substituting Leftovers) except the Director's Magearna
holding `ITEM_MAGEARNITE`. Jessie's five six-Pokémon rosters retain
Magearna, Aegislash Shield, and Archaludon; the latter two are kept
in the final two party positions. The battle AI may still choose to
switch Pokémon before the finale; a guaranteed synchronized dual-ace
entrance needs separate boss scripting.

## Save compatibility

Kanto defeat flags expanded from 624 to 640 bits within the existing
80-byte aligned storage. The old 78-byte array already occupied 80
bytes including alignment padding, so this uses the padding and
preserves subsequent SaveBlock3 offsets and older flag indexes.
Existing Kanto trainer IDs and all Hoenn mode IDs remain unchanged.

## Still to integrate

- Story NPCs, events, dialogue, maps, and facilities that invoke the
  sixteen trainer IDs. **No Xiphos encounters have been placed yet**.
- Unique Agent, commander and Jessie battle/overworld sprite art.
- Jessie's scripted final two-Pokémon entrance and postfight Magearna reward.
- Full ROM build and emulator battle test. These must pass before merging.

To evaluate: build Emerald via `make -j$(nproc)`, run
`python3 tools/brisk/check_trainer_legality.py` after refreshing the
legalities, and run the existing mGBA test suite. The GitHub Actions
Build Pokemon Brisk Emerald workflow runs on pull requests.
