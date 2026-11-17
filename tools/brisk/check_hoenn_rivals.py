#!/usr/bin/env python3
"""Static regression checks for the original May/Brendan Hoenn battle overrides."""
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[2]
read = lambda p: (ROOT / p).read_text()
header = read("include/brisk_hoenn_rivals.h")
teams = read("src/data/brisk_hoenn_rivals_teams.h")
loader = read("src/data/brisk_hoenn_rivals_loader.h")
original = read("src/data/trainers.party")
kanto = read("src/data/brisk_kanto_rivals_teams.h")
jessie = read("src/data/brisk_xiphos_extended_teams.h")

# The two genders and the three original player-starter selections are distinct.
stages = ["RUSTBORO", "ROUTE_110", "ROUTE_119", "LILYCOVE"]
expected = {
    f"TRAINER_{who}_{stage}_{starter}"
    for who in ("MAY", "BRENDAN")
    for stage in stages
    for starter in ("TREECKO", "TORCHIC", "MUDKIP")
}
actual = set(re.findall(r"case (TRAINER_(?:MAY|BRENDAN)_[A-Z0-9_]+):", loader))
assert expected == actual, ("missing or unexpected rival ID", expected - actual, actual - expected)
assert all(f"=== {tid} ===" in original for tid in expected)
assert "_ROUTE_103_" not in " ".join(actual), "Do not override the Route 103 starter tutorial"

names = set(re.findall(r"^    (HOENN_RIVAL_MON_[A-Z0-9_]+) = \d+,", header, re.M))
templates = dict(re.findall(r"^    \[(HOENN_RIVAL_MON_[A-Z0-9_]+)\] = (.*)$", teams, re.M))
assert len(names) == len(templates) == 23 and names == templates.keys(), ("templates",len(names),len(templates))
for name, value in templates.items():
    assert value.count("MOVE_") == 4, ("four moves", name)
    assert ".species=SPECIES_" in value and ".ability=ABILITY_" in value
assert "HOENN_RIVAL_MON_TREECKO" in loader and "if (level < 16)" in loader
assert "if (level < 36)" in loader
assert all(f"HOENN_RIVAL_MON_{x}" in loader for x in ("VIBRAVA","TRAPINCH","WINGULL","SHROOMISH","ELECTRIKE","METANG"))

# Each stage/gender gets five support species, of which the selected mode
# chooses the desired count-1; the starter always occupies the last slot.
for who in ("MAY", "BRENDAN"):
    start = loader.index(f"[HOENN_RIVAL_{who}] =")
    for stage in ("RUSTBORO","ROUTE110","ROUTE119","LILYCOVE"):
        m = re.search(rf"\[HOENN_RIVAL_{stage}\] = \{{([^}}]+)\}}",loader[start:])
        assert m, (who,stage)
        slots = re.findall(r"HOENN_RIVAL_MON_[A-Z0-9_]+",m.group(1))
        assert len(slots)==5 and len(set(slots))==5, (who,stage,slots)
        assert set(slots) <= names, (who,stage,slots)
assert "if (i == count - 1)" in loader and "choice = ace" in loader

# The team gets stronger while keeping Hoenn-native recurring partners.
for mon in ("BRELOOM","MANECTRIC","MILOTIC","METAGROSS"):
    assert f"HOENN_RIVAL_MON_{mon}" in loader, mon
    assert f"SPECIES_{mon}" in kanto, ("missing Kanto continuity",mon)
assert "HOENN_RIVAL_MON_SALAMENCE" in loader
assert "HOENN_RIVAL_MON_FLYGON" in loader

jessie_species = set()
for mode in ("Normal","Easy","Hard","Expert","Kaizo"):
    marker = f"const struct TrainerMon sXiphosJessie{mode}[PARTY_SIZE] = " + "{"
    assert marker in jessie
    section = jessie[jessie.index(marker):]
    section = section[:section.index("\n};")]
    jessie_species.update(re.findall(r"\.species=(SPECIES_[A-Z0-9_]+)",section))
hoenn_species = set(re.findall(r"\.species=(SPECIES_[A-Z0-9_]+)",teams))
assert not (hoenn_species & jessie_species), ("Jessie overlap", sorted(hoenn_species & jessie_species))
for needle, path in [
    ("BriskLoadHoennRivalParty(party, trainerNum)", "src/battle_setup.c"),
    ('"data/brisk_hoenn_rivals_loader.h"', "src/battle_setup.c"),
    ('"data/brisk_hoenn_rivals_teams.h"', "src/data.c"),
]:
    assert needle in read(path), (path,needle)
assert loader.index("trainer.partySize = count") > loader.index("mons[i] = sBriskHoennRivalMons[choice]")
print("PASS: all 24 Hoenn May/Brendan IDs, 23 templates, difficulty and evolution guards, and Kanto continuity")
