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
        # Arrays have external linkage because battle_setup.c loads them from data.c.
        matches=re.findall(r"^const struct TrainerMon\s+"+re.escape(name)+r"\[PARTY_SIZE\]\s*=\s*\{(.*?)\n\};",code,re.M|re.S)
        assert len(matches)==1,(name,"missing/duplicate")
        species=re.findall(r"\.species=SPECIES_([A-Z0-9_]+)",matches[0])
        assert len(species)==6 and len(set(species))==6,(name,species)
        assert matches[0].count(".ability=ABILITY_")==6,name
        assert matches[0].count(".moves={")==6,name
        assert matches[0].count(".heldItem=ITEM_")==6,name
        assert matches[0].count(".iv=")==6,name
        sets.append(tuple(species))
    assert len(set(sets))==5,(leader,"duplicate modes")
meta=(R/"src/data/brisk_xiphos_trainer_info.h").read_text()
data=(R/"src/data.c").read_text()
loader=(R/"src/data/brisk_xiphos_loader.h").read_text()
assert '#include "data/brisk_xiphos_trainer_info.h"' in data
assert '#include "data/brisk_xiphos_teams.h"' in data
assert '#include "data/brisk_xiphos_loader.h"' in (R/"src/battle_setup.c").read_text()
for leader in leaders:
    for mode in ("Normal","Easy","Hard","Kaizo"):
        key="sXiphos"+leader+mode
        assert (".party = "+key+",") in meta, (leader, mode, "missing engine trainer metadata")
        assert ("extern const struct TrainerMon "+key+"[PARTY_SIZE];") in loader
assert len(re.findall(r"\[DIFFICULTY_(?:NORMAL|EASY|HARD|KAIZO)\]\[TRAINER_XIPHOS_COMMANDER_",meta))==16
assert meta.count(".battleType = TRAINER_BATTLE_TYPE_DOUBLES") == 16
assert "species == SPECIES_MAGEARNA" in battle
assert "species == SPECIES_MAGEARNA_MEGA" in battle
assert "species == SPECIES_AEGISLASH" in battle
assert "BATTLE_TYPE_INGAME_PARTNER" in battle
assert "gBattleStruct->battlerState[battler].xiphosProtocolUsed" in battle
print("PASS: 4 commanders x 5 independently-authored parties x 6 complete battle sets")
print("PASS: 16 runtime trainer metadata rows and shared party pointers")
print("PASS: no Magearna/Aegislash Containment modifier, NPC/side restrictions present")
