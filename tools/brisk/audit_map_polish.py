#!/usr/bin/env python3
"""Report structural and visual-review candidates across every Brisk Emerald map.

This is a source audit, not a substitute for checking rendered tile seams,
collision, NPC positioning, or dialogue presentation in an emulator.
"""
import argparse
import json
import re
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
MAP_DIR = ROOT / "data" / "maps"
LAYOUT_FILE = ROOT / "data" / "layouts" / "layouts.json"


def audit():
    issues = []
    stats = Counter()

    def report(level, category, path, detail):
        issues.append({
            "severity": level,
            "category": category,
            "path": str(path.relative_to(ROOT)),
            "detail": detail,
        })

    layouts_source = json.loads(LAYOUT_FILE.read_text(encoding="utf-8"))
    layouts = {layout["id"]: layout for layout in layouts_source["layouts"]}
    maps = {}
    map_paths = sorted(MAP_DIR.glob("*/map.json"))

    for path in map_paths:
        try:
            content = json.loads(path.read_text(encoding="utf-8"))
        except (ValueError, UnicodeError) as exc:
            report("error", "invalid-map-json", path, str(exc))
            continue
        map_id = content.get("id")
        if not map_id:
            report("error", "missing-map-id", path, "No map ID")
        elif map_id in maps:
            report("error", "duplicate-map-id", path,
                   f"{map_id} also found in {maps[map_id][0].relative_to(ROOT)}")
        else:
            maps[map_id] = (path, content)
        stats["maps"] += 1

    for layout_id, layout in layouts.items():
        blockfile = ROOT / layout.get("blockdata_filepath", "")
        if not blockfile.is_file():
            report("review", "missing-layout-data", LAYOUT_FILE,
                   f"{layout_id}: {blockfile.relative_to(ROOT)} not found")
            continue
        expected = layout.get("width", 0) * layout.get("height", 0) * 2
        if expected and blockfile.stat().st_size != expected:
            report("error", "layout-size", LAYOUT_FILE,
                   f"{layout_id}: expected {expected} map bytes, got {blockfile.stat().st_size}")

    for path, data in maps.values():
        layout_id = data.get("layout")
        layout = layouts.get(layout_id)
        if layout is None:
            report("error", "missing-layout", path, f"Unknown {layout_id}")
            continue

        width, height = layout["width"], layout["height"]
        events = {
            "object": data.get("object_events", []),
            "warp": data.get("warp_events", []),
            "coord": data.get("coord_events", []),
            "bg": data.get("bg_events", []),
        }
        for kind, items in events.items():
            stats[f"{kind}_events"] += len(items)
            for idx, item in enumerate(items):
                x, y = item.get("x"), item.get("y")
                if not isinstance(x, int) or not isinstance(y, int) or not (0 <= x < width and 0 <= y < height):
                    report("error", "event-out-of-bounds", path,
                           f"{kind} #{idx} at ({x}, {y}) outside {width}x{height}")

        positions = defaultdict(list)
        for idx, obj in enumerate(events["object"]):
            positions[(obj.get("x"), obj.get("y"))].append((idx, obj))
            graphic = str(obj.get("graphics_id", ""))
            script = str(obj.get("script", ""))
            if graphic == "OBJ_EVENT_GFX_ITEM_BALL":
                stats["item_balls"] += 1
                if script != "Common_EventScript_FindItem":
                    report("review", "item-ball-as-npc", path,
                           f"Object #{idx} at ({obj.get('x')}, {obj.get('y')}) "
                           f"uses {script}; check intended sprite and dialogue")
                if obj.get("flag", "0") in ("0", 0, None):
                    report("review", "unhidden-item-ball", path,
                           f"Object #{idx} has no removal flag")
            if script in ("0x0", "0", "") and graphic not in (
                "OBJ_EVENT_GFX_TRUCK", "OBJ_EVENT_GFX_NONE", "OBJ_EVENT_GFX_INVISIBLE"
            ):
                report("review", "noninteractive-object", path,
                       f"Object #{idx} ({graphic}) has no interaction script")
        for pos, occupants in positions.items():
            if len(occupants) > 1:
                report("review", "overlapping-objects", path,
                       f"{len(occupants)} objects at {pos}, indexes "
                       f"{[item[0] for item in occupants]} (may be flag-gated)")

        for idx, event in enumerate(events["warp"]):
            destination = str(event.get("dest_map", ""))
            raw_id = str(event.get("dest_warp_id", ""))
            if destination.startswith("MAP_") and destination not in maps and destination not in (
                "MAP_UNDEFINED", "MAP_DYNAMIC", "MAP_NONE"
            ):
                report("review", "unknown-warp-destination", path,
                       f"Warp #{idx}: {destination}")
            elif destination in maps and raw_id.isdecimal():
                target_warps = maps[destination][1].get("warp_events", [])
                if int(raw_id) >= len(target_warps):
                    report("review", "out-of-range-warp", path,
                           f"Warp #{idx} targets {destination} warp {raw_id}, "
                           f"but destination has {len(target_warps)}")
        for idx, connection in enumerate(data.get("connections", [])):
            target = str(connection.get("map", ""))
            if target.startswith("MAP_") and target not in maps and target != "MAP_UNDEFINED":
                report("review", "unknown-map-connection", path,
                       f"Connection #{idx}: {target}")

        script_file = path.with_name("scripts.inc")
        if script_file.is_file():
            content = script_file.read_text(encoding="utf-8")
            for idx, obj in enumerate(events["object"]):
                script = str(obj.get("script", ""))
                if script.startswith(path.parent.name + "_") and not re.search(
                    r"(?m)^" + re.escape(script) + r"::?\s*$", content
                ):
                    report("review", "unresolved-local-script", path,
                           f"Object #{idx} references {script}; check shared scripts")

    typo_patterns = {
        "stray-punctuation": re.compile(r"(?<!\.)[!?]\.(?!\.)"),
        "fathers-possessive": re.compile(r"\bfathers (?:journey|team|pokemon)\b", re.I),
        "common-typo": re.compile(r"\b(?:teh|recieve|seperate|definately)\b", re.I),
    }
    text_files = list(MAP_DIR.glob("*/scripts.inc"))
    text_files += list((ROOT / "data" / "text").rglob("*.inc"))
    for path in sorted(text_files):
        for line_no, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
            if ".string " not in line:
                continue
            for category, pattern in typo_patterns.items():
                if pattern.search(line):
                    report("review", category, path, f"Line {line_no}: {line.strip()[:140]}")
    stats["layouts"] = len(layouts)
    return dict(stats), issues


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", default="polish-audit.json")
    parser.add_argument("--fail-on-errors", action="store_true")
    args = parser.parse_args()
    stats, issues = audit()
    counts = Counter(item["severity"] for item in issues)
    report = {"stats": stats, "counts": dict(counts), "issues": issues}
    Path(args.output).write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"Audited {stats.get('maps', 0)} maps, {stats.get('layouts', 0)} layouts, "
          f"and {stats.get('object_events', 0)} overworld objects.")
    print(f"Found {counts['error']} structural errors and {counts['review']} review candidates.")
    for issue in issues[:30]:
        print(f"[{issue['severity']}] {issue['path']}: {issue['category']}: {issue['detail']}")
    if len(issues) > 30:
        print(f"...and {len(issues) - 30} further entries in {args.output}")
    if args.fail_on_errors and counts["error"]:
        raise SystemExit(1)


if __name__ == "__main__":
    main()
