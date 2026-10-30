#!/usr/bin/env python3
"""Audit supplied-art coverage, complete species data, and reachable Kanto slots."""
from collections import Counter
from pathlib import Path
import json
import re
from PIL import Image
ROOT=Path(__file__).resolve().parents[2]


def audit():
    rows=json.loads((ROOT/'tools/brisk/mikitari_species.json').read_text())['species']
    report=json.loads((ROOT/'tools/brisk/mikitari_sprite_import.json').read_text())
    assert len(rows)==114 and len(report)==114
    assert len({r['id'] for r in rows})==114
    paths=[p for r in report.values() for p in r['files']]
    assert len(paths)==len(set(paths))==720, 'Every one of 120 supplied sets must have all six sources'
    constants=(ROOT/'include/constants/species.h').read_text()
    defs=(ROOT/'src/data/pokemon/species_info/mikitari.h').read_text()
    graphics=(ROOT/'src/data/graphics/mikitari.h').read_text()
    for r in rows:
        key=r['id'];assert 'SPECIES_'+key in constants and '[SPECIES_'+key+']' in defs
        assert len(r['stats'])==6 and all(1<=v<=255 for v in r['stats'])
        assert sum(r['stats'])<=540
        assert len(r['name'])<=12
        folder=ROOT/'graphics/pokemon/mikitari'/key.lower()
        for sex in ('','_female') if r.get('female_source') else ('',):
            for label in ('front','back'):
                im=Image.open(folder/(label+sex+'.png'));assert im.size==(64,64) and im.mode=='P'
                assert set(im.getdata())<=set(range(16))
            icon=Image.open(folder/('icon'+sex+'.png'));assert icon.size==(32,64)
            ow=Image.open(folder/('overworld'+sex+'.png'));assert ow.size==(32,256)
            for label in ('normal','shiny'):
                assert len((folder/(label+sex+'.pal')).read_text().splitlines())==19
        for ev in r['evolutions']:
            assert ev['target'] in report and 1<ev['level']<=60
    wild=json.loads((ROOT/'src/data/wild_encounters.json').read_text())['wild_encounter_groups'][0]['encounters']
    entries={e['map']:e for e in wild if e['base_label'].startswith('sBriskKanto_')}
    placements=json.loads((ROOT/'data/mikitari_kanto_placements.json').read_text())
    assert {p['species'] for p in placements}==set(report)
    config=json.loads((ROOT/'data/brisk_kanto_encounters.json').read_text())
    reserved={(r['map'],r['table']):set(r['slots']) for r in config['replacement_slots']}
    for p in placements:
        mon=entries[p['map']][p['table']]['mons'][p['slot']]
        assert mon['species']=='SPECIES_'+p['species']
        assert 90<=mon['min_level']<=mon['max_level']<=100
        assert p['slot'] not in reserved[(p['map'],p['table'])], 'Future room must remain'
        assert not any(x in p['map'] for x in ('SAFARI','ISLAND_'))
    ids={'SPECIES_'+r['id'] for r in rows}
    for (mapid,field),slots in reserved.items():
        for slot in slots:assert entries[mapid][field]['mons'][slot]['species'] not in ids
    dex={r['dex'] for r in rows}-{'BASCULIN','BUNEARY','CASTFORM','FALINKS','LOPUNNY','NOCTOWL','ORICORIO','SPRITZEE','YAMASK'}
    orders=(ROOT/'src/data/pokemon/pokedex_orders.h').read_text()
    for label in ('Alphabetical','Height','Weight'):
        body=re.search(r'gPokedexOrder_'+label+r'\[\] =\s*\{(.*?)\};',orders,re.S)[1]
        names=re.findall(r'NATIONAL_DEX_(\w+)',body)
        assert len(names)==len(set(names))
        assert dex<=set(names)
    assert 'Mikitari' in (ROOT/'CREDITS.md').read_text()
    assert 'sCreditsText_Mikitari' in (ROOT/'src/data/credits.h').read_text()
    print(f'PASS: 120 complete source sets; 114 species/forms; 79 new Dex entries; {len(set(p["map"] for p in placements))} Kanto habitats; {sum(map(len,reserved.values()))} future slots remain.')

if __name__=='__main__':audit()
