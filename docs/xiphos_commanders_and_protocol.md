# Xiphos Aegis — commanders and Containment Protocol

This feature adds four battle-ready commander identities and five separately authored six-member Doubles rosters for each, taken from the existing Xiphos blueprint. **They have not yet been placed in the Kanto map or story.**

## Commander battle IDs
- Fairy Commander Astra: TRAINER_XIPHOS_COMMANDER_FAIRY
- Fighting Commander Valor: TRAINER_XIPHOS_COMMANDER_FIGHTING
- Steel Commander Ferrum: TRAINER_XIPHOS_COMMANDER_STEEL
- Dragon Commander Drakon: TRAINER_XIPHOS_COMMANDER_DRAGON

The names are placeholders for the eventual organization story. All five modes are selected by the stable saved mode IDs Normal=0, Easy=1, Hard=2, Kaizo=3, Expert=4. No mode falls back to a different roster. Original Kanto trainer IDs are unchanged.

## Containment Protocol
Enemy-only battle modifier, not a real species or saved Pokemon ability. When a Xiphos commander Pokemon takes its first successful direct HP hit after entering battle, damage calculation halves that attack, then the surviving defender gains +2 Attack, +2 Special Attack, +2 Speed (respecting stage caps). Protect, Substitute-only hits, hazards and end-of-turn statuses do not consume the effect. Switching/fainting resets its consumed flag on that battler's battlefield slot. A displayed message announces activation.

The modifier is restricted to the opponent side of ordinary single-trainer NPC battles with Xiphos commander IDs. It cannot be copied via ability copying, disabled by ability suppression, traded, inherited, or stored in Pokemon save structures. Other trainers, rental modes, links, replay and partner modes are excluded.

## Director Jessie exceptions
- **Magearna and Mega Magearna keep Soul-Heart**, their normal innate ability. Neither gets the modifier. This has been checked against existing base and Mega Magearna species records.
- **Aegislash keeps Stance Change**, with no modifier.

Jessie's battle and storyline, which are not yet implemented, will need explicit registration as a qualifying Xiphos trainer for non-exempt members of her team. No Jessie encounter was added here.

## Validation
Run: python tools/brisk/validate_xiphos_commanders.py
Then perform a full ROM build and test Doubles: first hit, multihit, switching, Substitute, Protect, recoil, status, stat cap, Magearna/Aegislash exemptions, and all five modes.
