#!/usr/bin/env python3
"""Audit real Hoenn trainer parties against their encounter's progression band.

Run from any directory. This reads the authored parties, not test-runner fixtures.
Surf-only areas and split routes deliberately have separate bands. Rematches
must never fall below their previous team; postgame dedication teams are exempt.
"""
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[2]
TEAM_RE = re.compile(r"^===[ \t]*(TRAINER_\w+)[ \t]*===[ \t]*\n(.*?)(?=^===|\Z)", re.M | re.S)
LEVEL_RE = re.compile(r"^Level: (\d+)[ \t]*$", re.M)

MAP_BANDS = {
    "Route102": (10, 15), "Route103": (35, 40), "Route104": (14, 21),
    "PetalburgWoods": (16, 20), "RusturfTunnel": (20, 24), "Route116": (20, 24),
    "RustboroCity_Gym": (21, 24), "DewfordTown_Gym": (29, 34),
    "Route105": (55, 60), "Route106": (28, 33), "Route107": (55, 60),
    "Route108": (55, 60), "Route109": (35, 38), "Route109_SeashoreHouse": (35, 39),
    "SlateportCity_OceanicMuseum_2F": (37, 40), "Route110": (36, 41),
    "MauvilleCity_Gym": (40, 44), "Route111": (41, 46), "Route112": (42, 46),
    "Route113": (43, 47), "Route114": (44, 48), "Route115": (43, 48),
    "Route117": (39, 43), "Route118": (40, 44), "MtChimney": (46, 49),
    "JaggedPass": (46, 49), "LavaridgeTown_Gym_1F": (46, 49),
    "LavaridgeTown_Gym_B1F": (46, 49), "PetalburgCity_Gym": (51, 54),
    "Route119": (57, 61), "Route119_WeatherInstitute_1F": (58, 61),
    "Route119_WeatherInstitute_2F": (59, 62), "Route120": (59, 63),
    "FortreeCity_Gym": (61, 64), "Route121": (66, 69), "Route123": (66, 70),
    "Route124": (68, 72), "Route125": (69, 73), "Route126": (72, 76),
    "Route127": (76, 80), "Route128": (78, 83), "Route129": (79, 84),
    "Route130": (80, 85), "Route131": (80, 85), "Route132": (80, 85),
    "Route133": (81, 86), "Route134": (82, 87), "MossdeepCity_Gym": (71, 74),
    "MossdeepCity_SpaceCenter_1F": (77, 80), "MossdeepCity_SpaceCenter_2F": (78, 82),
    "SootopolisCity_Gym_1F": (85, 89), "SootopolisCity_Gym_B1F": (85, 89),
    "VictoryRoad_1F": (86, 90), "VictoryRoad_B1F": (87, 91),
    "VictoryRoad_B2F": (88, 92), "MeteorFalls_1F_2R": (78, 84),
}
GYMS = dict(zip((f"TRAINER_{name}_1" for name in (
    "ROXANNE", "BRAWLY", "WATTSON", "FLANNERY", "NORMAN", "WINONA", "TATE_AND_LIZA", "JUAN")),
    (25, 35, 45, 50, 55, 65, 75, 90)))


LEAGUE_IDS = {f'TRAINER_{name}' for name in ('SIDNEY','PHOEBE','GLACIA','DRAKE','WALLACE')}

STORY_ACES = {
    'TRAINER_TABITHA_MT_CHIMNEY': 49, 'TRAINER_MAXIE_MT_CHIMNEY': 50,
    'TRAINER_TABITHA_MAGMA_HIDEOUT': 72, 'TRAINER_MAXIE_MAGMA_HIDEOUT': 74,
    'TRAINER_TABITHA_MOSSDEEP': 82, 'TRAINER_MAXIE_MOSSDEEP': 84,
    'TRAINER_SHELLY_WEATHER_INSTITUTE': 63, 'TRAINER_SHELLY_SEAFLOOR_CAVERN': 88,
    'TRAINER_MATT': 73, 'TRAINER_ARCHIE': 90,
    **{f'TRAINER_GABBY_AND_TY_{i}': ace for i, ace in enumerate((44,46,60,64,74,86),1)},
}

def levels(team):
    return list(map(int, LEVEL_RE.findall(team)))


def rematch_chains():
    source = (ROOT / "src/battle_setup.c").read_text()
    return [re.findall(r"TRAINER_\w+", match) for match in
            re.findall(r"\[REMATCH_\w+\] = REMATCH\(([^)]+)\)", source)]


def encounter_bands(teams):
    bands = {}
    for path in sorted((ROOT / "data/maps").glob("*/scripts.inc")):
        name = path.parent.name
        band = MAP_BANDS.get(name)
        if name.startswith("MtPyre_"): band = (66, 70)
        if name.startswith("MagmaHideout_"): band = (67, 71)
        if name.startswith("AquaHideout_"): band = (69, 73)
        if name.startswith("SeafloorCavern_"): band = (82, 87)
        if name.startswith("AbandonedShip_"): band = (56, 62)
        if name.startswith("Route110_TrickHousePuzzle"):
            band = {1: (38, 41), 2: (42, 44), 3: (46, 49), 4: (51, 54), 5: (61, 64),
                    6: (66, 70), 7: (76, 82), 8: (90, 95)}[int(name[-1])]
        if band is None:
            continue
        for trainer_id in sorted(set(re.findall(r"\bTRAINER_\w+", path.read_text())) & teams.keys()):
            # Leaders and rival variants have explicit encounter targets below.
            if trainer_id in GYMS or re.match(r"TRAINER_(?:BRENDAN|MAY|WALLY)_", trainer_id):
                continue
            bands[trainer_id] = band
        # Swimmers on the southern sea are only reachable after Norman.
        for obj in json.loads((path.parent / "map.json").read_text())["object_events"]:
            label = obj.get("script", "")
            match = re.search(r"^" + re.escape(label) + r"::?\s*\n(.*?)(?=\n\w+::|\Z)", path.read_text(), re.M | re.S)
            ids = re.findall(r"\bTRAINER_\w+", match[1]) if match else []
            for trainer_id in ids[:1]:
                if trainer_id not in bands:
                    continue
                if name in ("Route106", "Route109") and "Class: Swimmer" in teams[trainer_id]:
                    bands[trainer_id] = (55, 60)
                if name == "Route111" and obj["y"] < 85:
                    bands[trainer_id] = (45, 49)  # Northern road and desert.
                if name == "Route115" and obj["y"] < 30:
                    bands[trainer_id] = (56, 61)  # Surf-only northern beach.
                if name == "Route118" and obj["x"] > 30:
                    bands[trainer_id] = (56, 60)  # East of the Surf crossing.
    for chain in rematch_chains():
        for trainer_id in chain[1:]:
            bands.pop(trainer_id, None)
    return bands


def rival_aces(teams):
    aces = {}
    for trainer_id in teams:
        if re.match(r"TRAINER_(?:BRENDAN|MAY)_", trainer_id):
            for location, ace in (("ROUTE_103", 9), ("RUSTBORO", 24), ("ROUTE_110", 44),
                                  ("ROUTE_119", 65), ("LILYCOVE", 74)):
                if location in trainer_id:
                    aces[trainer_id] = ace
        if trainer_id == "TRAINER_WALLY_MAUVILLE": aces[trainer_id] = 43
        if trainer_id == "TRAINER_WALLY_VR_1": aces[trainer_id] = 96
    return aces


def authored_progression_errors(teams, bands, protected):
    """Audit the four-mode records actually compiled into the Emerald ROM."""
    records = {}
    errors = []
    for tid, body in TEAM_RE.findall((ROOT / 'src/data/brisk_mode_teams.party').read_text()):
        mode = re.search(r'^Difficulty: (\w+)$', body, re.M)
        if mode:
            records[tid, mode[1]] = levels(body)
    modes = ('Easy', 'Normal', 'Hard', 'Kaizo')
    for tid, (low, high) in bands.items():
        if tid in protected or tid in STORY_ACES:
            continue
        for mode in modes:
            # Allow the authored Hard/Kaizo badge ceilings, while rejecting
            # early-game leftovers among later route trainers in every mode.
            lower = max(1, (low + 1) // 2 - 1) if mode == 'Easy' else max(1, low - 2)
            upper = ((high + 1) // 2 if mode == 'Easy' else
                     min(100, high + max(2, high // 5)) if mode == 'Hard' else
                     min(100, high + max(4, high * 2 // 5)) if mode == 'Kaizo' else high)
            if mode in ('Hard', 'Kaizo'):
                upper = min(100, upper + 2)  # Story grunts use the stronger story profile.
            actual = records.get((tid, mode), [])
            if not actual or not all(lower <= lv <= upper for lv in actual):
                errors.append(f'{tid}/{mode}: compiled-source levels {actual}, expected route band {lower}-{upper}')
    # A rival after Dewford must not fall back to the opening tutorial levels.
    for mode in modes:
        brawly = records.get(('TRAINER_BRAWLY_1', mode), [])
        for tid in rival_aces(teams):
            if '_ROUTE_110_' not in tid:
                continue
            rival = records.get((tid, mode), [])
            if not rival or not brawly or max(rival) < max(brawly):
                errors.append(f'{tid}/{mode}: Route 110 rival regresses below Brawly')
    for tid, ace in (GYMS | rival_aces(teams) | STORY_ACES).items():
        actual = records.get((tid, 'Normal'), [])
        if not actual or max(actual) != ace or min(actual) < ace - 4:
            errors.append(f'{tid}/Normal: compiled-source levels {actual}, expected ace {ace}, spread <= 4')
    return errors


def audit():
    teams = dict(TEAM_RE.findall((ROOT / "src/data/trainers.party").read_text()))
    errors = []
    bands = encounter_bands(teams)
    protected = json.loads((ROOT / 'tools/brisk/trainer_fairness_policy.json').read_text())['protected']
    errors.extend(authored_progression_errors(teams, bands, protected))
    for trainer_id, (low, high) in bands.items():
        if trainer_id in protected or trainer_id in STORY_ACES:
            continue  # Named bosses have explicit targets; only dedication/Easter eggs are exempt.
        party_levels = levels(teams[trainer_id])
        if not party_levels or not all(low <= level <= high for level in party_levels):
            errors.append(f"{trainer_id}: {party_levels}, expected {low}-{high}")
    for trainer_id, ace in (GYMS | rival_aces(teams) | STORY_ACES).items():
        party_levels = levels(teams[trainer_id])
        if not party_levels or max(party_levels) != ace or min(party_levels) < ace - 4:
            errors.append(f"{trainer_id}: {party_levels}, expected ace {ace}, spread <= 4")
    for trainer_id in LEAGUE_IDS:
        party_levels = levels(teams[trainer_id])
        if party_levels != [100] * 6:
            errors.append(f'{trainer_id}: every League member must be authored at 100')
    # Use the actual rematch table: numbered grunts are different people.
    rematches = 0
    for chain in rematch_chains():
        if chain[0] == "TRAINER_WALLY_VR_2":
            chain = ["TRAINER_WALLY_VR_1"] + chain
        for previous_id, trainer_id in zip(chain, chain[1:]):
            if previous_id in teams and trainer_id in teams and previous_id != trainer_id:
                rematches += 1
                if max(levels(teams[trainer_id])) < max(levels(teams[previous_id])):
                    errors.append(f"{trainer_id}: rematch weaker than previous team")
    for trainer_id in rival_aces(teams):
        if "ROUTE_119" in trainer_id or "LILYCOVE" in trainer_id:
            if re.search(r"^(?:SPECIES_)?(?:GROVYLE|COMBUSKEN|MARSHTOMP|SLUGMA|LOMBRE)\s*(?:@.*)?$", teams[trainer_id], re.M | re.I):
                errors.append(f"{trainer_id}: late rival still has an unevolved team member")
    for error in errors:
        print(error)
    print(f"Audited {len(bands)} encounter teams, {len(GYMS)} gyms, "
          f"{len(rival_aces(teams))} rivals and {rematches} rematch transitions; {len(errors)} errors.")
    return bool(errors)


if __name__ == "__main__":
    raise SystemExit(audit())
