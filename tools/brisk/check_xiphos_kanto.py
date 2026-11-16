#!/usr/bin/env python3
"""Audit Xiphos actors, exported-map manifest, Rocket placeholders and Jessie outfit hook."""
import json
from pathlib import Path
R = Path(__file__).resolve().parents[2]
placements = [["AGENT_A","ViridianCity_Frlg"],["AGENT_B","PewterCity_Frlg"],["AGENT_C","MtMoon_1F_Frlg"],["AGENT_D","CeruleanCity_Frlg"],["AGENT_E","VermilionCity_Frlg"],["AGENT_F","CeladonCity_Frlg"],["AGENT_G","LavenderTown_Frlg"],["SCIENTIST_A","SaffronCity_Frlg"],["SCIENTIST_B","FuchsiaCity_Frlg"],["SENIOR_A","CinnabarIsland_Frlg"],["SENIOR_B","IndigoPlateau_Exterior_Frlg"],["COMMANDER_FAIRY","CeladonCity_GameCorner_Frlg"],["COMMANDER_FIGHTING","SaffronCity_Dojo_Frlg"],["COMMANDER_STEEL","SilphCo_11F_Frlg"],["COMMANDER_DRAGON","VictoryRoad_2F_Frlg"],["JESSIE","RocketHideout_B4F_Frlg"]]
manifest = json.loads((R/"data/brisk_kanto_postgame.json").read_text())
script = (R/"data/scripts/xiphos_kanto.inc").read_text()
names = set()
for trainer, map_name in placements:
    path = R/"data/maps"/map_name/"map.json"
    map_data = json.loads(path.read_text())
    event_name = "XiphosKanto_EventScript_" + trainer
    found = [(i + 1, ev) for i,ev in enumerate(map_data["object_events"]) if ev.get("script")==event_name]
    assert len(found)==1,(trainer,map_name,"missing or duplicate")
    number,ev = found[0]
    assert ev["graphics_id"] in ("OBJ_EVENT_GFX_ROCKET_M","OBJ_EVENT_GFX_ROCKET_F"),trainer
    assert ev["trainer_type"]=="TRAINER_TYPE_NORMAL"
    record=manifest["maps"][map_name]["objects"][str(number)]
    assert record["script"]==event_name
    assert record["trainer"]=="TRAINER_XIPHOS_"+trainer
    assert event_name+"::" in script
    assert "trainerbattle_double TRAINER_XIPHOS_"+trainer+"," in script
    assert trainer not in names
    names.add(trainer)
assert len(names)==16
assert script.count("call Common_EventScript_JessieKantoClassicReward")==1
for commander in ("FAIRY","FIGHTING","STEEL","DRAGON"):
    assert "goto_if_unset FLAG_XIPHOS_COMMANDER_"+commander+"_DEFEATED" in script
assert "XiphosKanto_EventScript_JESSIE" in script
assert 'include "data/scripts/xiphos_kanto.inc"' in (R/"data/event_scripts.s").read_text()
assert "xiphosActor" in (R/"tools/mapjson/mapjson.cpp").read_text()
assert "script.startswith('XiphosKanto_EventScript_')" in (R/"tools/brisk/author_kanto_postgame.py").read_text()
assert "OUTFIT_KANTO_CLASSIC 3" in (R/"include/constants/outfits.h").read_text()
assert "void UnlockJessieKantoClassicOutfit(void)" in (R/"src/field_specials.c").read_text()
assert "\\\\n" not in script and "\\\\p" not in script, "double-escaped dialogue text"
print("PASS: 16 Team Xiphos Aegis battles placed, exported, and wired to outfit reward")
