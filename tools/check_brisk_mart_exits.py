#!/usr/bin/env python3
"""Check every map using the shared Mart layout against its actual exit tiles."""
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]


def main():
    layouts = json.loads((ROOT / "data/layouts/layouts.json").read_text())["layouts"]
    layout = next(row for row in layouts if row["id"] == "LAYOUT_MART")
    width, height = layout["width"], layout["height"]
    blocks = struct.unpack(f"<{width * height}H", (ROOT / layout["blockdata_filepath"]).read_bytes())
    attributes = (ROOT / "data/tilesets/secondary/shop/metatile_attributes.bin").read_bytes()
    # Emerald secondary metatiles start at 512; MB_SOUTH_ARROW_WARP is 0x65.
    exits = set()
    for y in range(height):
        for x in range(width):
            block = blocks[y * width + x]
            tile = block & 0x3FF
            if tile >= 512 and struct.unpack_from("<H", attributes, (tile - 512) * 2)[0] & 0xFF == 0x65:
                assert not block & 0xC00, f"Blocked exit tile at {x}, {y}"
                assert y > 0 and not blocks[(y - 1) * width + x] & 0xC00, f"Blocked approach at {x}, {y}"
                exits.add((x, y))
    assert exits, "Mart layout has no south-arrow exits"
    maps = {}
    for path in (ROOT / "data/maps").glob("*/map.json"):
        data = json.loads(path.read_text())
        maps[data["id"]] = data
    checked = 0
    for data in maps.values():
        if data["layout"] != layout["id"]:
            continue
        warps = data["warp_events"]
        assert {(warp["x"], warp["y"]) for warp in warps} == exits, data["name"]
        for warp in warps:
            target = maps[warp["dest_map"]]
            assert 0 <= int(warp["dest_warp_id"]) < len(target["warp_events"]), data["name"]
            assert warp["elevation"] in (0, blocks[warp["y"] * width + warp["x"]] >> 12), data["name"]
        checked += 1
    print(f"PASS: {checked} Mart maps have usable exits and valid destination warps.")


if __name__ == "__main__":
    main()
