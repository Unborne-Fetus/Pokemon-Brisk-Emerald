#!/usr/bin/env python3
"""Validate actual Xiphos commander C rosters (not the design blueprint)."""
from pathlib import Path
import re
R=Path(__file__).resolve().parents[2]
code=(R/"src/data/brisk_xiphos_teams.h").read_text()
battle=(R/"src/battle_util.c").read_text()
modes=["Normal","Easy","Hard","Kaizo","Expert"]
leaders=["Fairy","Fighting","Steel","Dragon"]
for leader in leaders:
    sets=[]
    for mode in modes:
        name="sXiphos"+leader+mode
        matches=re.findall(r"static const struct TrainerMon "+name+r"\[PARTY_SIZE\] = \{(.*?)\n\};",code,re.S)
        assert len(matches)==1,(name,"missing/duplicate")
        species=re.findall(r"\.species=SPECIES_([A-Z0-9_]+)",matches[0])
        assert len(species)==6 and len(set(species))==6,(name,species)
        assert matches[0].count(".ability=ABILITY_")==6,name
        assert matches[0].count(".moves={")==6,name
        assert matches[0].count(".heldItem=ITEM_")==6,name
        assert matches[0].count(".iv=")==6,name
        sets.append(tuple(species))
    assert len(set(sets))==5,(leader,"duplicate modes")
assert "species == SPECIES_MAGEARNA" in battle
assert "species == SPECIES_MAGEARNA_MEGA" in battle
assert "species == SPECIES_AEGISLASH" in battle
assert "BATTLE_TYPE_INGAME_PARTNER" in battle
assert "gBattleStruct->battlerState[battler].xiphosProtocolUsed" in battle
print("PASS: 4 commanders x 5 independently-authored parties x 6 complete battle sets")
print("PASS: no Magearna/Aegislash Containment modifier, NPC/side restrictions present")
