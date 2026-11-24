#!/usr/bin/env python3
"""Regression checks for Brisk's Kanto elevators and emitted warp obstructions."""
from pathlib import Path
import json
import re
import struct
from collections import Counter

ROOT = Path(__file__).resolve().parents[2]
def load(path):
    return json.loads(path.read_text(encoding="utf-8-sig"))
maps = {m["id"]: m for path in (ROOT / "data/maps").glob("*/map.json")
        for m in [load(path)]}
layouts = {l["id"]: l for l in load(ROOT / "data/layouts/layouts.json")["layouts"]}
script = (ROOT / "data/scripts/brisk_kanto_access.inc").read_text(encoding="utf-8")
configs = {
    "RocketLift": ("MAP_ROCKET_HIDEOUT_ELEVATOR", 3),
    "CeladonLift": ("MAP_CELADON_CITY_DEPARTMENT_STORE_ELEVATOR", 5),
    "SilphLift": ("MAP_SILPH_CO_ELEVATOR", 11),
}
converter = (ROOT / "tools/mapjson/mapjson.cpp").read_text(encoding="utf-8")
checked = 0
for lift, (map_id, count) in configs.items():
    prefix = f"BriskKantoAccessOnly_EventScript_{lift}"
    assert prefix in converter, lift
    block = re.search(rf"^{prefix}::\n(.*?)(?=^\w+::|\Z)", script, re.M|re.S)[1]
    cases = re.findall(rf"case (\d+), ({prefix}_Floor\d+)", block)
    assert [int(index) for index, _ in cases] == list(range(count)), lift
    assert "goto BriskKantoAccessOnly_EventScript_LiftDone" in block
    m = maps[map_id]
    assert all(w["dest_map"] == "MAP_DYNAMIC" for w in m["warp_events"])
    targets = set()
    for _, label in cases:
        body = re.search(rf"^{label}::\n(.*?)(?=^\w+::|\Z)", script, re.M|re.S)[1]
        dest, x, y = re.search(r"setdynamicwarp (MAP_\w+), 255, (\d+), (\d+)", body).groups()
        x, y = int(x), int(y)
        target = maps[dest]
        layout = layouts[target["layout"]]
        assert 0 <= x < layout["width"] and 0 <= y < layout["height"], (lift, dest, x, y)
        assert any((w["x"], w["y"], w["dest_map"]) == (x, y, map_id)
                   for w in target["warp_events"]), (lift, dest, x, y)
        assert dest not in targets, (lift, dest)
        targets.add(dest)
        checked += 1
    print(f"PASS: {lift}, {len(targets)} distinct return destinations")
    event_file = ROOT / "data/maps" / m["name"] / "events.inc"
    if event_file.exists():
        generated = event_file.read_text(encoding="utf-8")
        if prefix in generated:
            print(f"PASS: {lift} emitted panel connected")
        else:
            print(f"STALE: regenerate {m['name']} events.inc before ROM release")
assert re.search(r"BriskKantoAccessOnly_EventScript_LiftDone::\s+closemessage\s+releaseall\s+end", script)
print(f"PASS: {checked} elevator floor destinations verified")

# Scan the actual generated event definitions, rather than raw FRLG cutscenes.
emitted, overlap, unhandled = 0, [], []
for m in maps.values():
    if m.get("region") != "REGION_KANTO":
        continue
    p = ROOT / "data/maps" / m["name"] / "events.inc"
    if not p.exists():
        continue
    events = p.read_text(encoding="utf-8")
    emitted += 1
    warps = {(w["x"],w["y"]) for w in m.get("warp_events",[])}
    for line in events.splitlines():
        if not line.strip().startswith("object_event "):
            continue
        fields = line.strip().removeprefix("object_event ").split(", ")
        if len(fields) != 12:
            continue
        _, gfx, x, y, elevation, movement, rx, ry, trainer, sight, label, flag = fields
        if not x.lstrip("-").isdigit() or not y.lstrip("-").isdigit():
            continue
        if (int(x),int(y)) in warps and flag == "0":
            overlap.append((m["name"],x,y,gfx,label))
    if any(w["dest_map"]=="MAP_DYNAMIC" for w in m.get("warp_events",[])) and m["id"] not in {v[0] for v in configs.values()}:
        unhandled.append(m["name"])
print(f"Generated Kanto maps checked: {emitted}")
print(f"Permanently occupied warp coordinates: {overlap}")
print(f"Other dynamic warp maps for review: {unhandled}")
assert not overlap, overlap
