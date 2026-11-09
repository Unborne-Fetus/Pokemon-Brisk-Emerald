#!/usr/bin/env python3
"""Check emitted Emerald maps, dialogue and real wild encounter records."""
from collections import Counter
import json
from pathlib import Path
import re
import subprocess
import tempfile

ROOT=Path(__file__).resolve().parents[2]


def audit():
    manifest=json.loads((ROOT/'data/brisk_kanto_postgame.json').read_text())
    scripts=(ROOT/'data/scripts/brisk_kanto_postgame.inc').read_text()
    blocks=dict(re.findall(r'^(\w+)::?\s*\n(.*?)(?=^\w+::?|\Z)',scripts,re.M|re.S))
    speeches=Counter(''.join(re.findall(r'\.string "(.*)"',body)) for label,body in blocks.items() if '.string' in body and '_Sign_' not in label)
    assert all(n==1 for n in speeches.values()),'Duplicate NPC dialogue'
    item_scripts=dict(re.findall(r'^(\\w+)::\\s*\\n\\s*finditem\\s+(ITEM_\\w+)',
        (ROOT/'data/scripts/item_ball_scripts_frlg.inc').read_text(),re.M))
    exceptional_balls={
        'PowerPlant_EventScript_Electrode1':'BriskKanto_EventScript_Electrode',
        'PowerPlant_EventScript_Electrode2':'BriskKanto_EventScript_Electrode',
        'CeladonCity_Condominiums_RoofRoom_EventScript_EeveeBall':'BriskKanto_EventScript_GiftEevee',
        'PalletTown_ProfessorOaksLab_EventScript_BulbasaurBall':'BriskKanto_EventScript_GiftBulbasaur',
        'PalletTown_ProfessorOaksLab_EventScript_SquirtleBall':'BriskKanto_EventScript_GiftSquirtle',
        'PalletTown_ProfessorOaksLab_EventScript_CharmanderBall':'BriskKanto_EventScript_GiftCharmander',
        'SaffronCity_Dojo_EventScript_HitmonleeBall':'BriskKanto_EventScript_GiftHitmonlee',
        'SaffronCity_Dojo_EventScript_HitmonchanBall':'BriskKanto_EventScript_GiftHitmonchan',
    }
    physical_count=0
    with tempfile.TemporaryDirectory() as temp:
        for name,entry in manifest['maps'].items():
            subprocess.run([str(ROOT/'tools/mapjson/mapjson'),'map','emerald',str(ROOT/'data/maps'/name/'map.json'),str(ROOT/'data/layouts/layouts.json'),temp],cwd=ROOT,check=True,capture_output=True)
            emitted=(Path(temp)/'events.inc').read_text()
            assert 'OBJ_EVENT_GFX_VAR_' not in emitted,(name,'uninitialized sprite')
            assert 'OBJ_EVENT_GFX_SEAGALLOP' not in emitted,(name,'cutscene ferry')
            if name=='ViridianCity_Frlg':assert 'OBJ_EVENT_GFX_OLD_MAN_1' in emitted
            if name=='CinnabarIsland_Frlg':assert 'OBJ_EVENT_GFX_BILL' in emitted
            original=json.loads((ROOT/'data/maps'/name/'map.json').read_text())
            for index,original_obj in enumerate(original['object_events'],1):
                if entry['objects'].get(str(index),{}).get('omit'):continue
                gfx=original_obj.get('graphics_id','')
                script=original_obj.get('script','')
                if gfx in ('OBJ_EVENT_GFX_ITEM_BALL','OBJ_EVENT_GFX_FOSSIL_FRLG'):
                    physical_count+=1
                    if script in item_scripts or script in ('MtMoon_B2F_EventScript_DomeFossil','MtMoon_B2F_EventScript_HelixFossil'):
                        target='Common_EventScript_FindItem'
                    else:
                        target=exceptional_balls.get(script)
                    assert target is not None,(name,script,'unsupported Kanto ball')
                    assert target in emitted,(name,script,target,'missing correct pickup/battle')
                    assert 'FLAG_BRISK_KANTO_OBJECT_START' in emitted,(name,script,'untracked pickup')
                elif gfx in ('OBJ_EVENT_GFX_PUSHABLE_BOULDER_FRLG','OBJ_EVENT_GFX_PUSHABLE_BOULDER','OBJ_EVENT_GFX_CUTTABLE_TREE_FRLG','OBJ_EVENT_GFX_BREAKABLE_ROCK_FRLG','OBJ_EVENT_GFX_BREAKABLE_ROCK'):
                    physical_count+=1
                    expected={'OBJ_EVENT_GFX_PUSHABLE_BOULDER_FRLG':'EventScript_StrengthBoulder',
                        'OBJ_EVENT_GFX_PUSHABLE_BOULDER':'EventScript_StrengthBoulder',
                        'OBJ_EVENT_GFX_CUTTABLE_TREE_FRLG':'EventScript_CutTree',
                        'OBJ_EVENT_GFX_BREAKABLE_ROCK_FRLG':'EventScript_RockSmash',
                        'OBJ_EVENT_GFX_BREAKABLE_ROCK':'EventScript_RockSmash'}[gfx]
                    assert expected in emitted,(name,gfx,'field obstacle became dialogue')
            for rec in entry['objects'].values():
                if rec.get('omit'):continue
                label=rec['script']
                if 'Nurse' not in label and label not in blocks:continue
                body=blocks.get(label,'')
                if 'HealPlayerParty' in body:
                    assert 'FLDEFF_POKECENTER_HEAL' in body
                    assert 'Movement_PkmnCenterNurse_Bow' in body
                    assert body.index('waitfieldeffect')<body.index('HealPlayerParty')
    wild=json.loads((ROOT/'src/data/wild_encounters.json').read_text())['wild_encounter_groups'][0]
    entries=[e for e in wild['encounters'] if e['base_label'].startswith('sBriskKanto_')]
    bymap={e['map']:e for e in entries};assert len(bymap)==len(entries)
    for n in range(1,26):
        ids=['MAP_ROUTE21_NORTH','MAP_ROUTE21_SOUTH'] if n==21 else [f'MAP_ROUTE{n}']
        for mapid in ids:assert mapid in bymap,mapid
    catalog='\n'.join(p.read_text() for p in (ROOT/'src/data/pokemon/species_info').glob('*.h'))
    catalog+='\n'+(ROOT/'include/constants/species.h').read_text()
    for e in entries:
        assert not any(x in e['base_label'] for x in ('FireRed','LeafGreen'))
        original=next((o for o in wild['encounters'] if o['map']==e['map'] and 'FireRed' in o['base_label']),None)
        for field,table in e.items():
            if not isinstance(table,dict):continue
            if original:assert len(table['mons'])==len(original[field]['mons'])
            for i,mon in enumerate(table['mons']):
                if mon['species']=='SPECIES_NONE':
                    assert field in ('land_mons','water_mons') and i==len(table['mons'])-1
                    continue
                assert 90<=mon['min_level']<=mon['max_level']<=100,(e['map'],mon)
                assert mon['species'] in catalog,mon['species']
    reserved=json.loads((ROOT/'data/brisk_kanto_encounters.json').read_text())['replacement_slots']
    for rec in reserved:
        table=bymap[rec['map']][rec['table']]['mons']
        assert len(rec['slots'])==(4 if rec['table']=='land_mons' else 2)
        for i in rec['slots']:assert table[i]['species']!='SPECIES_NONE'
    print(f'PASS: {len(manifest["maps"])} emitted maps; {physical_count} physical props checked for non-dialogue interactions; no uninitialized actors or ferries; {len(speeches)} unique NPC speeches; {len(entries)} postgame encounter maps and {len(reserved)} future-Fakemon slot groups.')

if __name__=='__main__':audit()
