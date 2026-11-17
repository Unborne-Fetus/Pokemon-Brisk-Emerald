#!/usr/bin/env python3
"""Static regression test for Brisk Kanto's Hoenn rival / Wally encounters."""
from pathlib import Path
import json
import re

R = Path(__file__).resolve().parents[2]
get = lambda path: (R / path).read_text()
manifest = json.loads(get("data/brisk_kanto_postgame.json"))["maps"]
expected = {
    "Route22_Frlg": (1, "BriskKantoRival_EventScript_Route22", "OBJ_EVENT_GFX_VAR_0"),
    "SSAnne_2F_Corridor_Frlg": (1, "BriskKantoRival_EventScript_SSAnne", "OBJ_EVENT_GFX_VAR_0"),
    "SilphCo_7F_Frlg": (1, "BriskKantoRival_EventScript_Silph", "OBJ_EVENT_GFX_VAR_0"),
    "PokemonLeague_ChampionsRoom_Frlg": (1, "BriskKantoRival_EventScript_Champion", "OBJ_EVENT_GFX_VAR_0"),
    "CeruleanCity_Frlg": (8, "BriskKantoRival_EventScript_WallyCerulean", "OBJ_EVENT_GFX_WALLY"),
    "PokemonTower_1F_Frlg": (6, "BriskKantoRival_EventScript_WallyLavender", "OBJ_EVENT_GFX_WALLY"),
    "VictoryRoad_2F_Frlg": (15, "BriskKantoRival_EventScript_WallyVictory", "OBJ_EVENT_GFX_WALLY"),
}
scripts = get("data/scripts/brisk_kanto_rivals.inc")
for name, (idx, label, sprite) in expected.items():
    m = json.loads(get(f"data/maps/{name}/map.json"))
    obj = m["object_events"][idx - 1]
    assert (obj["script"], obj["graphics_id"]) == (label, sprite), (name, "object")
    assert manifest[name]["objects"][str(idx)]["script"] == label, (name, "manifest")
    assert label + "::" in scripts, (name, "missing battle script")
    assert obj["flag"] == "0", (name, "FRLG plot flag retained")
for name in ("Route22_Frlg", "SSAnne_2F_Corridor_Frlg", "SilphCo_7F_Frlg"):
    assert manifest[name]["map_scripts"] == "BriskKantoRival_GfxMapScripts"
assert manifest["PokemonLeague_ChampionsRoom_Frlg"]["map_scripts"] == "BriskKantoRival_ChampionGfxMapScripts"
assert r"\\n" not in scripts and r"\\p" not in scripts, "doubled dialogue escapes"
assert "call Common_EventScript_SetupRivalGfxId" in scripts

team_bank = get("src/data/brisk_kanto_rivals_teams.h")
teams = re.findall(r"const struct TrainerMon (sKantoRival_[A-Z0-9_]+)\[PARTY_SIZE\] = \{(.*?)\n\};",team_bank,re.S)
assert len(teams) == 9, ("team definitions", len(teams))
for name,body in teams:
    species = re.findall(r"\.species=(SPECIES_[A-Z_]+)",body)
    assert len(species)==6 and len(set(species))==6,(name,species)
    assert body.count(".moves={")==6
assert "ITEM_GALLADITE" in dict(teams)["sKantoRival_WALLY_VICTORY"]
ids = re.findall(r"^#define (TRAINER_KANTO_[A-Z_0-9]+) (TRAINER_BRISK_KANTO_[A-Z_0-9]+)",get("include/constants/brisk_kanto_rival_ids.h"),re.M)
assert len(ids)==15 and len({x[1] for x in ids})==15, ids
loader = get("src/data/brisk_kanto_rivals_loader.h")
for alias,_ in ids:
    assert "case " + alias + ":" in loader,(alias,"not wired to battle loader")
for anchor,path in (
    ("BriskLoadKantoRivalParty(party, trainerNum)","src/battle_setup.c"),
    ("data/brisk_kanto_rivals_teams.h","src/data.c"),
    ("data/scripts/brisk_kanto_rivals.inc","data/event_scripts.s"),
    ("BriskKantoRival_EventScript_","tools/mapjson/mapjson.cpp"),
    ("BriskKantoRival_EventScript_","tools/brisk/author_kanto_postgame.py"),
):
    assert anchor in get(path),(path,anchor)
assert "goto_if_defeated TRAINER_KANTO_MAY_CHAMPION" in scripts
assert "goto_if_defeated TRAINER_KANTO_BRENDAN_CHAMPION" in scripts
assert "VarGet(VAR_STARTER_MON)" in loader
assert "TRAINER_PIC_WALLY" in get("include/data.h")
print("PASS: Hoenn rival and Wally Kanto script/map/roster hooks; build/emulator still required")
