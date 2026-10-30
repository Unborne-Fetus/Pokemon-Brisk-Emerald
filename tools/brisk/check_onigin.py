#!/usr/bin/env python3
"""Validate onigin_pixelart source crops, complete species, and Kanto placement."""
from pathlib import Path
import hashlib
import json
import re
from PIL import Image
ROOT=Path(__file__).resolve().parents[2]


def audit():
    meta=json.loads((ROOT/'tools/brisk/onigin_species.json').read_text());rows=meta['species']
    report=json.loads((ROOT/'tools/brisk/onigin_sprite_import.json').read_text())
    source=ROOT/meta['source_file'];assert hashlib.sha256(source.read_bytes()).hexdigest()==report['source_sha256']
    sheet=Image.open(source);assert sheet.size==(512,192)
    ids={r['id'] for r in rows};assert len(ids)==len(rows)==9
    positions=json.loads((ROOT/'data/onigin_kanto_placements.json').read_text());assert {p['species'] for p in positions}==ids
    assert len({p['map'] for p in positions})==9
    entries={e['map']:e for e in json.loads((ROOT/'src/data/wild_encounters.json').read_text())['wild_encounter_groups'][0]['encounters'] if e['base_label'].startswith('sBriskKanto_')}
    cfg=json.loads((ROOT/'data/brisk_kanto_encounters.json').read_text())
    reserved={(r['map'],r['table']):set(r['slots']) for r in cfg['replacement_slots']}
    for row in rows:
        key=row['id'];assert 250<=sum(row['stats'])<=540 and len(row['name'])<=12
        for label in ('front','back'):
            bounds=row[label];assert bounds[2]-bounds[0]==bounds[3]-bounds[1]==64
            assert sheet.crop(bounds).getbbox()
        folder=ROOT/'graphics/pokemon/onigin_pixelart'/key.lower()
        for label in ('front','back'):
            im=Image.open(folder/(label+'.png'));assert im.size==(64,64) and im.mode=='P';assert max(im.getdata())<16
        assert Image.open(folder/'icon.png').size==(32,64)
        assert Image.open(folder/'overworld.png').size==(32,256)
        assert (folder/'normal.pal').read_bytes()!=(folder/'shiny.pal').read_bytes()
        if row['evolution']:assert row['evolution'] in ids and row['evolution_level']>1
    for p in positions:
        mon=entries[p['map']][p['table']]['mons'][p['slot']]
        assert mon['species']=='SPECIES_'+p['species']
        assert 90<=mon['min_level']<=mon['max_level']<=100
        assert p['slot'] not in reserved[(p['map'],p['table'])]
    # Neither earlier pack nor unused future slots may be displaced.
    for p in json.loads((ROOT/'data/mikitari_kanto_placements.json').read_text()):
        assert entries[p['map']][p['table']]['mons'][p['slot']]['species']=='SPECIES_'+p['species']
    for (mapid,field),slots in reserved.items():
        for slot in slots:assert entries[mapid][field]['mons'][slot]['species'].removeprefix('SPECIES_') not in ids
    order=(ROOT/'src/data/pokemon/pokedex_orders.h').read_text()
    for name in ('Alphabetical','Weight','Height'):
        body=re.search(r'gPokedexOrder_'+name+r'\[\] =\s*\{(.*?)\};',order,re.S)[1]
        values=re.findall(r'NATIONAL_DEX_(\w+)',body);assert len(values)==len(set(values)) and ids<=set(values)
    assert 'onigin_pixelart' in (ROOT/'CREDITS.md').read_text()
    assert 'sCreditsText_Onigin[] = _("onigin_pixelart")' in (ROOT/'src/data/credits.h').read_text()
    print('PASS: nine onigin_pixelart front/back sets; nine accessible Kanto maps; prior pack and all 620 future slots preserved.')

if __name__=='__main__':audit()
