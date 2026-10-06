#!/usr/bin/env python3
"""Unpack the bundled Coffee Cup outfit source art without changing the game."""
from pathlib import Path
import tarfile


def main():
    root = Path(__file__).resolve().parents[1]
    destination = root / "resources/team_aqua/coffee_cup"
    destination.mkdir(parents=True, exist_ok=True)
    with tarfile.open(root / "resources/team_aqua/coffee_cup_customization.tar.xz", "r:xz") as archive:
        for member in archive.getmembers():
            target = (destination / member.name).resolve()
            if not target.is_relative_to(destination.resolve()) or not member.isfile():
                raise ValueError(f"Unexpected archive entry: {member.name}")
            target.parent.mkdir(parents=True, exist_ok=True)
            with archive.extractfile(member) as source, target.open("wb") as output:
                output.write(source.read())
    print(f"Extracted customization resources to {destination}")


if __name__ == "__main__":
    main()
