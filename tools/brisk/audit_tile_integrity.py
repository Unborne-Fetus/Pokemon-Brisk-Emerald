#!/usr/bin/env python3
"""Audit all stored map metatile references for unavailable tileset entries.

This checks source block data only. Runtime setmetatile calls, animated tiles,
collision and visual seam continuity still need individual gameplay review.
"""
import argparse
import json
import re
import struct
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
METATILE_BYTES = 24
PRIMARY_CAPACITY = 512


def folder(symbol, primary):
    name = symbol.removeprefix("gTileset_")
    slug = re.sub(r"(?<!^)(?=[A-Z])", "_", name).lower()
    return ROOT / "data" / "tilesets" / ("primary" if primary else "secondary") / slug


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", default="tile-integrity-audit.json")
    args = parser.parse_args()
    layouts_path = ROOT / "data" / "layouts" / "layouts.json"
    layouts = json.loads(layouts_path.read_text(encoding="utf-8"))["layouts"]
    sizes = {}
    findings = []
    stats = Counter()
    for item in layouts:
        name = item["id"]
        width, height = item["width"], item["height"]
        dimensions = width * height
        block = ROOT / item["blockdata_filepath"]
        if not block.is_file():
            findings.append({"layout": name, "reason": "missing-map-binary"})
            continue
        raw = block.read_bytes()
        expected = dimensions * 2
        if len(raw) < expected:
            findings.append({"layout": name, "reason": "truncated-map-binary",
                             "actual": len(raw), "expected": expected})
            continue
        for source_primary, key in ((True, "primary_tileset"), (False, "secondary_tileset")):
            source = folder(item[key], source_primary)
            if source not in sizes:
                meta = source / "metatiles.bin"
                sizes[source] = meta.stat().st_size // METATILE_BYTES if meta.is_file() else None
            if sizes[source] is None:
                findings.append({"layout": name, "reason": "missing-tileset-binary",
                                 "path": str(source.relative_to(ROOT))})
        psource = folder(item["primary_tileset"], True)
        ssource = folder(item["secondary_tileset"], False)
        primary_max = sizes[psource]
        secondary_max = sizes[ssource]
        if primary_max is None or secondary_max is None:
            continue
        mismatches = defaultdict(list)
        for index in range(dimensions):
            word = struct.unpack_from("<H", raw, 2 * index)[0]
            tile = word & 0x3FF
            if (tile < PRIMARY_CAPACITY and tile >= primary_max) or (
                tile >= PRIMARY_CAPACITY and tile - PRIMARY_CAPACITY >= secondary_max
            ):
                mismatches[f"0x{tile:03X}"].append([index % width, index // width])
        if mismatches:
            for tile_id, locations in sorted(mismatches.items()):
                findings.append({
                    "layout": name, "reason": "out-of-range-metatile", "tile": tile_id,
                    "count": len(locations), "coordinates": locations[:25],
                    "primary_limit": primary_max, "secondary_limit": secondary_max,
                })
                stats["out_of_range_tiles"] += len(locations)
        if len(raw) not in (expected, expected + 2):
            findings.append({"layout": name, "reason": "unexpected-map-binary-size",
                             "actual": len(raw), "expected": expected})
        stats["layouts"] += 1
        stats["metatiles_checked"] += dimensions

    result = {"stats": dict(stats), "findings": findings,
              "counts": dict(Counter(f["reason"] for f in findings))}
    Path(args.output).write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(f"Audited {stats['layouts']} layouts and {stats['metatiles_checked']:,} stored metatiles.")
    print(f"Found {len(findings)} issues for manual review: {result['counts']}")
    for f in findings[:25]:
        print(json.dumps(f))


if __name__ == "__main__":
    main()
