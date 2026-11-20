#!/usr/bin/env python3
"""Render source metatile layouts for reviewing Brisk Emerald visual seams.

Map output is a source-data preview, not an emulated frame: animated tiles,
overworld actors, and runtime setmetatile changes are not represented.
"""
import argparse
import json
import struct
from functools import lru_cache
from pathlib import Path

from PIL import Image

ROOT = Path(__file__).resolve().parents[2]
PRIMARY_METATILES = 512
PRIMARY_TILES = 512
METATILE_BYTES = 24  # 12 packed tile entries, 3 compositing layers


def tileset_folder(identifier, primary):
    name = identifier.removeprefix("gTileset_")
    import re
    slug = re.sub(r"(?<!^)(?=[A-Z])", "_", name).lower()
    return ROOT / "data" / "tilesets" / ("primary" if primary else "secondary") / slug


def read_pal(path):
    lines = path.read_text(encoding="ascii").splitlines()
    if lines[0] != "JASC-PAL":
        raise ValueError(f"Unsupported palette: {path}")
    return [tuple(map(int, line.split())) for line in lines[3:19]]


class Tileset:
    def __init__(self, folder):
        self.folder = folder
        self.image = Image.open(folder / "tiles.png").convert("P")
        self.pixels = self.image.load()
        self.columns = self.image.width // 8
        self.count = self.columns * (self.image.height // 8)
        self.metatiles = (folder / "metatiles.bin").read_bytes()
        self.palettes = [read_pal(folder / "palettes" / f"{i:02d}.pal") for i in range(16)]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("maps", nargs="*", default=[
        "LittlerootTown", "Route101", "OldaleTown", "Route103", "Route102",
        "PetalburgCity", "Route104",
    ])
    parser.add_argument("--output", default="map-previews")
    args = parser.parse_args()
    out = Path(args.output)
    out.mkdir(parents=True, exist_ok=True)
    layouts_json = json.loads((ROOT / "data" / "layouts" / "layouts.json").read_text())
    layouts = {item["id"]: item for item in layouts_json["layouts"]}
    info = []
    tileset_cache = {}
    for name in args.maps:
        map_path = ROOT / "data" / "maps" / name / "map.json"
        if not map_path.exists():
            print(f"SKIP {name}: missing map")
            continue
        map_data = json.loads(map_path.read_text())
        layout = layouts[map_data["layout"]]
        width, height = layout["width"], layout["height"]
        raw = (ROOT / layout["blockdata_filepath"]).read_bytes()
        if len(raw) < width * height * 2:
            raise ValueError(f"{name}: layout data too short")
        sources = []
        for primary, key in ((True, "primary_tileset"), (False, "secondary_tileset")):
            folder = tileset_folder(layout[key], primary)
            if folder not in tileset_cache:
                tileset_cache[folder] = Tileset(folder)
            sources.append(tileset_cache[folder])
        # In Hoenn the primary tileset provides palette banks 0-5 and
        # the secondary tileset supplies 6-15.
        palettes = sources[0].palettes[:6] + sources[1].palettes[6:]
        missing = set()

        @lru_cache(maxsize=8192)
        def render_tile(tile_word, base):
            tile_id = tile_word & 0x3FF
            pal_bank = (tile_word >> 12) & 15
            flip_x = bool(tile_word & 0x400)
            flip_y = bool(tile_word & 0x800)
            source = sources[0] if tile_id < PRIMARY_TILES else sources[1]
            index = tile_id if tile_id < PRIMARY_TILES else tile_id - PRIMARY_TILES
            if index >= source.count:
                missing.add((tile_id, source.folder.name))
                return Image.new("RGBA", (8, 8))
            palette = palettes[pal_bank]
            sx = (index % source.columns) * 8
            sy = (index // source.columns) * 8
            points = []
            for py in range(8):
                for px in range(8):
                    idx = source.pixels[sx + px, sy + py] & 15
                    r, g, b = palette[idx]
                    points.append((r, g, b, 255 if base or idx else 0))
            tile = Image.new("RGBA", (8, 8))
            tile.putdata(points)
            if flip_x:
                tile = tile.transpose(Image.Transpose.FLIP_LEFT_RIGHT)
            if flip_y:
                tile = tile.transpose(Image.Transpose.FLIP_TOP_BOTTOM)
            return tile

        canvas = Image.new("RGBA", (width * 16, height * 16), (24, 41, 82, 255))
        for y in range(height):
            for x in range(width):
                word = struct.unpack_from("<H", raw, 2 * (y * width + x))[0]
                metatile_id = word & 0x3FF
                primary = metatile_id < PRIMARY_METATILES
                source = sources[0] if primary else sources[1]
                metatile_local = metatile_id if primary else metatile_id - PRIMARY_METATILES
                offset = metatile_local * METATILE_BYTES
                if offset + METATILE_BYTES > len(source.metatiles):
                    missing.add(("metatile", metatile_id))
                    continue
                for layer in range(3):
                    for cell in range(4):
                        tile_word = struct.unpack_from(
                            "<H", source.metatiles, offset + 2 * (layer * 4 + cell)
                        )[0]
                        tile = render_tile(tile_word, base=(layer == 0))
                        cx, cy = cell % 2, cell // 2
                        canvas.alpha_composite(tile, (x * 16 + cx * 8, y * 16 + cy * 8))
        png = out / f"{name}.png"
        canvas.convert("RGB").save(png)
        info.append({
            "name": name, "width": width, "height": height,
            "preview": str(png), "missing_tile_references": list(map(str, sorted(missing, key=str))),
        })
        print(f"{name}: {width}x{height} metatiles -> {png} ({len(missing)} missing references)")
    (out / "summary.json").write_text(json.dumps(info, indent=2) + "\n")


if __name__ == "__main__":
    main()
