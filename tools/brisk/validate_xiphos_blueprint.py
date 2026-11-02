#!/usr/bin/env python3
"""Check the Xiphos five-difficulty design matrix.

This intentionally validates the design blueprint only, NOT playable parties.
Once Xiphos trainer IDs and five-mode selection are implemented, add a
separate game-data check that enumerates every actual Xiphos trainer ID.
"""

from pathlib import Path
import sys


ROOT = Path(__file__).resolve().parents[2]
BLUEPRINT = ROOT / "docs" / "xiphos_aegis_implementation_blueprint.md"
HEADER = ["Trainer archetype", "Easy", "Normal", "Hard", "Expert", "Kaizo"]
REQUIRED = {
    "Patrol Agent Alpha",
    "Patrol Agent Beta",
    "Xiphos Scientist Alpha",
    "Senior Agent Alpha",
    "Fairy Commander",
    "Fighting Commander",
    "Steel Commander",
    "Dragon Commander",
    "Director Jessie",
}


def cells(line):
    return [cell.strip() for cell in line.strip().strip("|").split("|")]


def check(text):
    lines = text.splitlines()
    header = "| " + " | ".join(HEADER) + " |"
    if header not in lines:
        return ["Missing five-mode Xiphos roster table header"]

    rows = lines[lines.index(header) + 2 :]
    errors = []
    seen = set()
    count = 0
    for line in rows:
        if not line.startswith("|"):
            break
        fields = cells(line)
        if len(fields) != len(HEADER):
            errors.append(f"Expected 5 difficulty columns: {line[:65]}")
            continue
        name, *teams = fields
        if name in seen:
            errors.append(f"Duplicate trainer archetype: {name}")
        seen.add(name)
        count += 1
        parsed = []
        for mode, team in zip(HEADER[1:], teams):
            species = tuple(mon.strip() for mon in team.split(","))
            if not team or any(not mon for mon in species):
                errors.append(f"{name}/{mode}: missing Pokémon")
            if len(species) < 4 or len(species) > 6:
                errors.append(f"{name}/{mode}: expected 4-6 Pokémon, got {len(species)}")
            if len(set(species)) != len(species):
                errors.append(f"{name}/{mode}: repeated species")
            if ("Commander" in name or name == "Director Jessie") and len(species) != 6:
                errors.append(f"{name}/{mode}: boss must have six Pokémon")
            if name == "Director Jessie" and species[-2:] != ("Aegislash", "Archaludon"):
                errors.append(f"{name}/{mode}: intended final double aces changed")
            parsed.append(species)

        for i in range(len(parsed)):
            for j in range(i + 1, len(parsed)):
                if parsed[i] == parsed[j]:
                    errors.append(f"{name}: identical {HEADER[i+1]} and {HEADER[j+1]} teams")

    for name in sorted(REQUIRED - seen):
        errors.append(f"Missing archetype: {name}")
    if not count:
        errors.append("No Xiphos roster rows")
    return errors


def main():
    errors = check(BLUEPRINT.read_text(encoding="utf-8"))
    if errors:
        print("Xiphos blueprint validation failed:", file=sys.stderr)
        for err in errors:
            print(f"  - {err}", file=sys.stderr)
        return 1
    print("Xiphos blueprint OK: five nonempty, distinct teams per archetype.")
    print("NOTE: This does not verify in-game Xiphos parties or trainer IDs.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
