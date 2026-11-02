#!/usr/bin/env python3
"""Deterministic habitat and progression correction for Brisk's wild tables.

Applied to the compiled encounter header *in memory*, preserving the source
JSON, all existing slot counts, legendary/static encounters, and custom-art
Fakemon placements. No global encounter RNG is involved.
"""
from collections import Counter
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]

# Fish, reef animals, and wholly aquatic Pokemon: NEVER walking encounters.
# Deliberately excludes amphibians (Lotad, Wooper, Mudkip, Slowpoke etc.).
WATER_ONLY = set("""
CORSOLA HORSEA SEADRA KINGDRA GOLDEEN SEAKING CHINCHOU LANTURN
TENTACOOL TENTACRUEL QWILFISH QWILFISH_HISUI REMORAID OCTILLERY
CLAMPERL HUNTAIL GOREBYSS RELICANTH WAILMER WAILORD LUVDISC
SHELLDER CLOYSTER KRABBY KINGLER STARYU STARMIE FEEBAS MILOTIC
MANTYKE MANTINE FINNEON LUMINEON FRILLISH JELLICENT ALOMOMOLA
BRUXISH DHELMISE WIGLETT WUGTRIO FINIZEN PALAFIN VELUZA
TATSUGIRI DONDONZO BASCULIN BASCULIN_RED_STRIPED BASCULIN_BLUE_STRIPED
BASCULIN_WHITE_STRIPED BASCULEGION BASCULEGION_F BASCULEGION_M
ARROKUDA BARRASKEWDA SKRELP DRAGALGE CLAUNCHER CLAWITZER
MAREANIE TOXAPEX WISHIWASHI WISHIWASHI_SOLO
""".split())
WATER_ONLY = {"SPECIES_" + mon for mon in WATER_ONLY}

EARLY = {
    "MAP_ROUTE101": (2, 5, "ZIGZAGOON POOCHYENA WURMPLE TAILLOW SENTRET HOOTHOOT LEDYBA BIDOOF STARLY FLETCHLING HOPPIP SEEDOT"),
    "MAP_ROUTE102": (3, 8, "ZIGZAGOON WURMPLE RALTS SEEDOT LOTAD SHROOMISH SENTRET NICKIT SKITTY POOCHYENA HOPPIP PIDOVE"),
    "MAP_ROUTE103": (3, 9, "POOCHYENA WINGULL ZIGZAGOON TAILLOW LOTAD PIDOVE FLETCHLING ELECTRIKE SKITTY SEEDOT SHROOMISH"),
    "MAP_ROUTE104": (5, 12, "WURMPLE TAILLOW SHROOMISH SEEDOT LOTAD HOOTHOOT BUDEW FLETCHLING COMBEE CATERPIE PIDOVE"),
    "MAP_PETALBURG_WOODS": (5, 13, "WURMPLE SILCOON CASCOON SHROOMISH SEEDOT ODDISH CATERPIE WEEDLE SEWADDLE VENIPEDE SPINARAK"),
    "MAP_ROUTE116": (9, 17, "NINCADA WHISMUR SKITTY TAILLOW POOCHYENA SHROOMISH NICKIT YAMPER LILLIPUP WOOPER"),
    "MAP_ROUTE117": (13, 24, "ROSELIA MARILL VOLBEAT ILLUMISE ODDISH ELECTRIKE SKITTY RALTS FLAAFFY HOPPIP"),
}
# A limited set of additional Fakemon from *already installed* packs.
# Baby-stage species were selected for low/medium-level Hoenn zones.
FAKEMON_PLACEMENTS = {
    "MAP_ROUTE104": "SPROUTSY",
    "MAP_PETALBURG_WOODS": "SHAGGOT",
    "MAP_ROUTE116": "BUDCHIRP",
    "MAP_ROUTE117": "CHUBBOINK",
    "MAP_ROUTE110": "GOOBOLT",
}
COASTAL_DESTINATIONS = (
    "MAP_ROUTE109", "MAP_ROUTE105", "MAP_ROUTE106",
    "MAP_ROUTE124", "MAP_ROUTE125", "MAP_ROUTE126",
    "MAP_ROUTE129", "MAP_ROUTE130", "MAP_ROUTE134", "MAP_PACIFIDLOG_TOWN",
)
INLAND_DESTINATIONS = ("MAP_ROUTE118", "MAP_ROUTE119", "MAP_ROUTE120", "MAP_ROUTE121", "MAP_ROUTE123", "MAP_ROUTE115")
FIELDS = ("land_mons", "water_mons", "fishing_mons", "rock_smash_mons")


def is_emerald(entry):
    label = entry.get("base_label", "")
    return "FireRed" not in label and "LeafGreen" not in label


def installed_fakemon():
    found = set()
    for name in ("mikitari_species.json", "onigin_species.json", "sacred_phoenix_species.json", "cdrx73_species.json"):
        path = ROOT / "tools" / "brisk" / name
        if not path.exists():
            continue
        data = json.loads(path.read_text(encoding="utf-8"))
        rows = data.get("species", data) if isinstance(data, dict) else data
        for row in rows:
            if isinstance(row, dict) and row.get("id"):
                found.add("SPECIES_" + row["id"])
    return found


def slots(data):
    for group in data["wild_encounter_groups"]:
        for entry in group.get("encounters", []):
            if not is_emerald(entry):
                continue
            for field in FIELDS:
                if field in entry:
                    for i, mon in enumerate(entry[field]["mons"]):
                        if mon.get("species") != "SPECIES_NONE":
                            yield (entry, field, i, mon)


def rebalance(data, strict=True):
    # Catch misspelled species before the GBA C compiler tries to parse a
    # many-megabyte generated wild header.
    import re
    species_header = (ROOT / "include/constants/species.h").read_text(encoding="utf-8")
    known = set(re.findall(r"\\bSPECIES_[A-Z0-9_]+\\b", species_header))
    referenced = {"SPECIES_" + name
                  for _map, (_low, _high, pool) in EARLY.items()
                  for name in pool.split()}
    referenced.update("SPECIES_" + name for name in FAKEMON_PLACEMENTS.values())
    missing = sorted(referenced - known)
    if missing and strict:
        raise ValueError("Undefined species in habitat pools: " + ", ".join(missing))
    all_slots = list(slots(data))
    freq = Counter(mon["species"] for _e, _f, _i, mon in all_slots)
    originals = set(freq)
    fakemon = installed_fakemon()
    bymap = {}
    for entry, field, i, mon in all_slots:
        bymap.setdefault(entry.get("map", ""), []).append((entry, field, i, mon))
    corrections = Counter()
    moved = set()
    pending = []

    def change(mon, new, reason):
        old = mon["species"]
        if old == new:
            return
        if freq[old] == 1:
            pending.append((old, reason))
        freq[old] -= 1
        freq[new] += 1
        mon["species"] = new
        corrections[reason] += 1

    def stable_slot(mapid, idx, size):
        key = f"{mapid}:{idx}".encode()
        return int.from_bytes(hashlib.sha256(key).digest()[:4], "little") % size

    # Biome correction applies even to Kanto, but never removes a custom Fakemon.
    for entry, field, i, mon in all_slots:
        if field != "land_mons" or mon["species"] not in WATER_ONLY:
            continue
        mapid = entry.get("map", "")
        local = [s["species"] for e, f, j, s in bymap.get(mapid, ())
                 if f == "land_mons" and s["species"] not in WATER_ONLY
                 and s["species"] != "SPECIES_NONE"]
        palette = EARLY.get(mapid)
        choices = ["SPECIES_" + name for name in palette[2].split()] if palette else local
        if not choices:
            choices = ["SPECIES_POOCHYENA", "SPECIES_ZIGZAGOON"] if mapid.startswith("MAP_ROUTE10") else ["SPECIES_ODDISH", "SPECIES_TAILLOW"]
        change(mon, choices[stable_slot(mapid, i, len(choices))], "aquatic_removed_from_land")

    # Early Hoenn should show plausible FIRST/STAGED forms, never late-game
    # evolved Water/Steel/legendary species on Route 101.
    for entry, field, i, mon in all_slots:
        if field != "land_mons":
            continue
        mapid = entry.get("map", "")
        if mapid not in EARLY or entry.get("base_label", "").startswith("sBriskKanto_"):
            continue
        low, high, names = EARLY[mapid]
        choices = ["SPECIES_" + name for name in names.split()]
        if mon["species"] not in choices and mon["species"] not in fakemon:
            change(mon, choices[stable_slot(mapid, i, len(choices))], "early_biome")
        # Keep levels in an encounter band, not a badge-scaled late-game band.
        old_min = mon.get("min_level", low)
        old_max = mon.get("max_level", high)
        mon["min_level"] = min(high, max(low, old_min))
        mon["max_level"] = min(high, max(mon["min_level"], old_max))

    # A handful of low-power Fakemon now appear outside their Kanto homes,
    # without evicting a rare/sole remaining species or an existing custom mon.
    for mapid, name in FAKEMON_PLACEMENTS.items():
        desired = "SPECIES_" + name
        if desired not in fakemon:
            if strict:
                raise ValueError(f"Fakemon {desired} is missing from installed species manifests")
            continue
        possible = [(e, f, i, mon) for e, f, i, mon in bymap.get(mapid, [])
                    if f == "land_mons" and freq[mon["species"]] > 1 and mon["species"] not in fakemon
                    and not e.get("base_label", "").startswith("sBriskKanto_")]
        if not possible:
            if strict:
                raise ValueError(f"No replaceable slot for {desired} in {mapid}")
            continue
        e, f, i, mon = possible[-3 if len(possible) >= 3 else -1]
        change(mon, desired, "fakemon_hoenn")

    # Preserve availability: if rebalancing removed the only Emerald wild
    # occurrence of a species, insert it into a plausible non-reserved slot.
    for species, reason in list(pending):
        if freq[species]:
            continue
        aquatic = species in WATER_ONLY
        destinations = COASTAL_DESTINATIONS if aquatic else INLAND_DESTINATIONS
        allowed_fields = ("water_mons", "fishing_mons") if aquatic else ("land_mons",)
        replacements = [(e, f, i, mon) for destination in destinations
                        for e, f, i, mon in bymap.get(destination, [])
                        if f in allowed_fields and freq[mon["species"]] > 1
                        and mon["species"] not in fakemon
                        and not e.get("base_label", "").startswith("sBriskKanto_")]
        if not replacements:
            if strict:
                raise ValueError(f"Cannot preserve wild availability of {species} ({reason})")
            continue
        # Deterministic local distribution and last-slot bias, preserving
        # ordinary prominent encounter species at the start of a table.
        pick = replacements[stable_slot(species, len(replacements), len(replacements))]
        change(pick[3], species, "availability_relocation")
        moved.add(species)

    lost = sorted(s for s in originals if not freq[s])
    # Do not report success if even one existing species is made unobtainable.
    if strict and lost:
        raise ValueError("Wild species made unavailable: " + ", ".join(lost[:20]))
    for entry, field, i, mon in all_slots:
        if field == "land_mons" and mon["species"] in WATER_ONLY:
            if strict:
                raise ValueError(f"Aquatic walking encounter left in {entry.get('map')}: {mon['species']}")
    return {
        "corrected": dict(corrections),
        "previous_species": len(originals),
        "available_species": len({sp for sp, n in freq.items() if n}),
        "lost_species": lost,
        "relocated": sorted(moved),
        "fakemon_added": list(FAKEMON_PLACEMENTS.values()),
        "maps_with_encounters": len(bymap),
    }


def load_and_rebalance():
    path = ROOT / "src/data/wild_encounters.json"
    data = json.loads(path.read_text(encoding="utf-8"))
    report = rebalance(data)
    return data, report


if __name__ == "__main__":
    data, report = load_and_rebalance()
    print(json.dumps(report, indent=2))
