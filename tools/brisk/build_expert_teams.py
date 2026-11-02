#!/usr/bin/env python3
"""Materialize an independent, explicit Expert trainer party for every Emerald trainer.

Consumes the already-authored four-mode Hoenn and postgame Kanto parties.
The output is a fifth collection of actual trainerproc records, not a
runtime alias to Hard. Re-running it is deterministic and preserves manual
edits to the four existing modes.

Run: python3 tools/brisk/build_expert_teams.py --write
     python3 tools/brisk/build_expert_teams.py --check
"""
import argparse
from collections import Counter, OrderedDict
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[2]
SOURCES = (
    ROOT / "src/data/brisk_mode_teams.party",
    ROOT / "src/data/brisk_kanto_teams.party",
)
OUT = ROOT / "src/data/brisk_expert_teams.party"
MODES = ("Easy", "Normal", "Hard", "Kaizo")
RECORD = re.compile(r"^===[ \t]*(TRAINER_\w+)[ \t]*===[ \t]*\n(.*?)(?=^===|\Z)", re.M | re.S)
DIFFICULTY = re.compile(r"^Difficulty:\s*(\w+)\s*$", re.M)
LEVEL = re.compile(r"^Level:\s*(\d+)\s*$", re.M)
POLICY_FILE = ROOT / "tools/brisk/trainer_fairness_policy.json"


def split_record(body):
    blocks = re.split(r"\n[ \t]*\n", body.strip())
    return blocks[0], [b for b in blocks[1:] if b.strip()]


def pokemon_species(block):
    first = block.splitlines()[0].split("@", 1)[0].strip()
    match = re.search(r"\bSPECIES_[A-Z0-9_]+", first)
    return match.group() if match else first


def pokemon_level(block):
    match = LEVEL.search(block)
    return int(match.group(1)) if match else 100


def at_level(block, level):
    # A moved Normal-mode Pokemon is promoted, never devolved: moves remain legal.
    if LEVEL.search(block):
        return LEVEL.sub("Level: " + str(level), block, count=1)
    return block + "\nLevel: " + str(level)


def read_sources():
    trainers = OrderedDict()
    for path in SOURCES:
        if not path.exists():
            raise ValueError("Missing source: " + str(path))
        for tid, body in RECORD.findall(path.read_text(encoding="utf-8")):
            mode_match = DIFFICULTY.search(body)
            if not mode_match:
                raise ValueError(f"{path.name}/{tid}: no difficulty")
            mode = mode_match.group(1)
            if mode not in MODES:
                raise ValueError(f"{path.name}/{tid}: unexpected mode {mode}")
            modes = trainers.setdefault(tid, {})
            if mode in modes:
                raise ValueError(f"Duplicate record: {tid} {mode}")
            modes[mode] = split_record(body)
    for tid, modes in trainers.items():
        if set(modes) != set(MODES):
            raise ValueError(f"{tid}: missing modes {set(MODES) - set(modes)}")
    if not trainers:
        raise ValueError("No trainer records found")
    return trainers


def alternate_teams(hard, normal, kaizo, locked):
    if not hard or locked:
        return list(hard)
    team = list(hard)
    hard_high = max(map(pokemon_level, hard))
    desired = len(team) if hard_high < 50 else min(6, max(len(team), len(kaizo)))
    existing = {pokemon_species(block) for block in team}
    alternatives = list(normal[:-1]) + list(kaizo[:-1]) + list(normal[-1:]) + list(kaizo[-1:])
    # For late-game teams, add independently authored support members BEFORE the ace.
    for block in alternatives:
        if len(team) >= desired:
            break
        species = pokemon_species(block)
        if species in existing:
            continue
        team.insert(len(team) - 1, at_level(block, max(pokemon_level(block), hard_high - 1)))
        existing.add(species)

    # Make Expert's composition distinct whenever the existing source pool allows.
    original_signatures = {
        tuple(sorted(pokemon_species(b) for b in mode))
        for mode in (hard, normal, kaizo)
    }
    for block in alternatives:
        species = pokemon_species(block)
        if species in existing or len(team) < 2:
            continue
        for i in range(len(team) - 1):
            candidate = list(team)
            candidate[i] = at_level(block, max(pokemon_level(block), pokemon_level(team[i])))
            signature = tuple(sorted(pokemon_species(b) for b in candidate))
            if signature not in original_signatures and len(set(map(pokemon_species, candidate))) == len(candidate):
                return candidate
    return team


def render():
    records = read_sources()
    protected = set()
    if POLICY_FILE.exists():
        policy = json.loads(POLICY_FILE.read_text(encoding="utf-8"))
        protected = set(policy.get("protected", [])) | set(policy.get("metadata_only", [])) | set(policy.get("kaizo_bosses", []))
    output = [
        "/* Auto-generated FIFTH trainer mode. Do not edit directly.",
        "   Source: the existing authored Hoenn/Kanto mode records.",
        "   Regenerate: python3 tools/brisk/build_expert_teams.py --write",
        "   Expert is its own DIFFICULTY_EXPERT roster, never a Hard alias. */",
        "",
    ]
    count = 0
    late = 0
    distinct = 0
    for tid, modes in records.items():
        header, hard = modes["Hard"]
        normal = modes["Normal"][1]
        kaizo = modes["Kaizo"][1]
        locked = tid in protected or tid == "TRAINER_NONE" or "_ROUTE_103_" in tid
        team = alternate_teams(hard, normal, kaizo, locked)
        # Hard's species / ace may persist, but Expert has independent party data.
        if hard and max(map(pokemon_level, hard)) >= 50:
            late += 1
        if [pokemon_species(x) for x in team] != [pokemon_species(x) for x in hard]:
            distinct += 1
        header = re.sub(r"^Difficulty:.*(?:\n|$)", "", header, flags=re.M).rstrip()
        if not re.search(r"^Authored Party:\s*Yes\s*$", header, re.M):
            header += "\nAuthored Party: Yes"
        header += "\nDifficulty: Expert"
        if not team and tid != "TRAINER_NONE" and tid not in protected:
            # Empty nonbattle metadata records remain empty, like existing modes.
            pass
        output.append(f"=== {tid} ===\n{header}\n\n" + "\n\n".join(team) + "\n")
        count += 1
    return "\n".join(output).rstrip() + "\n", count, late, distinct


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--write", action="store_true")
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    if not args.write and not args.check:
        parser.error("specify --write or --check")
    result, count, late, distinct = render()
    if args.write:
        OUT.parent.mkdir(parents=True, exist_ok=True)
        if not OUT.exists() or OUT.read_text(encoding="utf-8") != result:
            OUT.write_text(result, encoding="utf-8")
    if args.check:
        if not OUT.exists() or OUT.read_text(encoding="utf-8") != result:
            raise SystemExit("Outdated/missing Expert parties: run --write")
    generated = RECORD.findall(result)
    if len(generated) != count:
        raise SystemExit("Expert party count mismatch")
    print(f"PASS: {count} independent Expert trainer records; {late} late-game; {distinct} species-roster changes from Hard")


if __name__ == "__main__":
    main()
