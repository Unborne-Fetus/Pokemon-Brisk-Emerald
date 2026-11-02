#!/usr/bin/env python3
"""Audit every Hoenn/Kanto trainer across all five Brisk difficulty modes.

Counts actual authored teams plus the compact Expert variant (Hard roster
with a deterministic, legally authored replacement Pokemon). Prints actionable
warnings for balance judgment; fails for missing, malformed or unsafe parties.
"""
from collections import Counter
import re
from build_compact_expert import get_patches, SOURCES
from build_expert_teams import pokemon_level


def pokemon_species(block):
    """Read competitive-syntax headers, including nicknames and gender."""
    first = block.splitlines()[0].split("@", 1)[0].strip()
    explicit = re.search(r"SPECIES_[A-Z0-9_]+", first)
    if explicit:
        return explicit.group()
    names = re.findall(r"\\(([^)]+)\\)", first)
    species = next((value for value in names if value not in ("M", "F")), None)
    if species is None:
        species = re.sub(r"\\s*\\([MF]\\)\\s*$", "", first).strip()
    return "SPECIES_" + re.sub(r"[^A-Z0-9]+", "_", species.upper()).strip("_")


MODES = ("Easy", "Normal", "Expert", "Hard", "Kaizo")


def examine():
    trainers, patches, stats = get_patches()
    errors, reviews = [], []
    checked, species_sets = 0, Counter()
    for tid, modes in trainers.items():
        hard = modes["Hard"][1]
        if not hard and tid.endswith("_NONE"):
            continue
        if tid == "TRAINER_NONE":
            continue
        for mode in ("Easy", "Normal", "Hard", "Kaizo"):
            if mode not in modes:
                errors.append(f"{tid} missing {mode} team")
        expert = list(hard)
        if tid in patches:
            code = patches[tid] - 1
            row, source, target = (code >> 8) & 3, (code >> 4) & 7, code & 7
            if row >= len(SOURCES) or target >= len(expert) or source >= len(modes[SOURCES[row]][1]):
                errors.append(f"{tid} invalid compact Expert patch index")
            else:
                expert[target] = modes[SOURCES[row]][1][source]
        all_modes = {mode: modes[mode][1] for mode in ("Easy", "Normal", "Hard", "Kaizo")}
        all_modes["Expert"] = expert
        metadata = not hard or not modes["Normal"][1]
        if metadata:
            continue
        for mode in MODES:
            roster = all_modes[mode]
            checked += 1
            if not 1 <= len(roster) <= 6:
                errors.append(f"{tid}/{mode}: party has {len(roster)} Pokemon")
            for mon in roster:
                species = pokemon_species(mon)
                level = pokemon_level(mon)
                if not 1 <= level <= 100 or not species.startswith("SPECIES_"):
                    errors.append(f"{tid}/{mode}: invalid {species} Lv{level}")
                species_sets[mode] += 1
            if tid.startswith("TRAINER_BRISK_KANTO_") and (len(roster) != 6 or any(not 90 <= pokemon_level(m) <= 100 for m in roster)):
                errors.append(f"{tid}/{mode}: postgame Kanto must have six level 90-100 Pokemon")
            if len(roster) > 1 and len({pokemon_species(m) for m in roster}) != len(roster):
                reviews.append(f"{tid}/{mode}: repeated species in one party")
        if len(hard) > len(all_modes["Kaizo"]) and len(all_modes["Kaizo"]):
            reviews.append(f"{tid}: Hard has more team members than Kaizo")
        if max(map(pokemon_level, all_modes["Easy"])) > max(map(pokemon_level, all_modes["Normal"])) + 3:
            reviews.append(f"{tid}: Easy level spike above Normal")
        if max(map(pokemon_level, all_modes["Hard"])) < max(map(pokemon_level, all_modes["Normal"])) - 3:
            reviews.append(f"{tid}: Hard level lower than Normal")
        if tid not in patches and len(hard) > 1 and not tid.startswith(("TRAINER_BLUE", "TRAINER_RED2")):
            reviews.append(f"{tid}: no distinct Expert support replacement available")
    if checked == 0 or not stats["changed"]:
        errors.append("No five-mode trainer teams processed")
    print(f"Five-mode trainer audit: {len(trainers)} trainer identities, {checked} parties, "
          f"{sum(species_sets.values())} Pokemon.")
    print(f"Compact Expert: {stats}.")
    print(f"Balance review flags: {len(reviews)}; first 25:")
    for issue in reviews[:25]:
        print(" REVIEW:", issue)
    for issue in errors[:50]:
        print(" ERROR:", issue)
    return errors


if __name__ == "__main__":
    raise SystemExit(bool(examine()))
