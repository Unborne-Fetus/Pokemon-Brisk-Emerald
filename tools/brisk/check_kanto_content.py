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
    # Kanto on Emerald compiles generated Brisk scripts, not the FRLG originals.
    # The fossils must be real one-time pickups, not ordinary dialogue NPCs.
    fossils=manifest['maps']['MtMoon_B2F_Frlg']['objects']
    for idx,item in (('1','DOME'),('2','HELIX')):
        label=f'BriskKanto_MtMoon_B2F_Frlg_{idx}'
        assert fossils[idx]['script']==label and fossils[idx].get('keep_flag') is True
        body=blocks.get(label,'')
        for command in (
            f'checkitemspace ITEM_{item}_FOSSIL, 1',
            f'giveitem_msg {label}_Obtained, ITEM_{item}_FOSSIL, 1',
            f'setflag FLAG_GOT_{item}_FOSSIL',
            'setflag FLAG_GOT_FOSSIL_FROM_MT_MOON',
            'setflag FLAG_HIDE_DOME_FOSSIL',
            'setflag FLAG_HIDE_HELIX_FOSSIL',
            'removeobject LOCALID_DOME_FOSSIL',
            'removeobject LOCALID_HELIX_FOSSIL',
        ):
            assert command in body,(label,command)
    assert 'Do you want the DOME FOSSIL?' not in scripts
    assert 'Do you want the HELIX FOSSIL?' not in scripts

    # The Hoenn rival is deliberately a VAR_0 actor in four Kanto maps so
    # the opposing May/Brendan sprite matches the player's gender. Reject
    # every other dynamic sprite, and require a working on-load initializer
    # for each intentional rival instead of treating it as uninitialized.
    rival_scripts=(ROOT/'data/scripts/brisk_kanto_rivals.inc').read_text()
    rival_graphics=(ROOT/'data/scripts/rival_graphics.inc').read_text()
    rival_map_scripts={
        'Route22_Frlg': 'BriskKantoRival_GfxMapScripts',
        'SSAnne_2F_Corridor_Frlg': 'BriskKantoRival_GfxMapScripts',
        'SilphCo_7F_Frlg': 'BriskKantoRival_GfxMapScripts',
        'PokemonLeague_ChampionsRoom_Frlg': 'BriskKantoRival_ChampionGfxMapScripts',
    }
    assert 'setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_RIVAL_MAY_NORMAL' in rival_graphics
    assert 'setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_RIVAL_BRENDAN_NORMAL' in rival_graphics

    # Check the actual distributed ROM, not just the source files. This
    # catches stale generated Kanto scripts being linked into Emerald again.
    rom_path=ROOT/'pokeemerald.gba'
    if rom_path.is_file():
        rom=rom_path.read_bytes()
        def gba_text(s):
            return bytes(0xBB+ord(c)-65 if 'A'<=c<='Z'
                         else 0xD5+ord(c)-97 if 'a'<=c<='z'
                         else 0 if c==' ' else 0xAC if c=='?'
                         else 0xFF for c in s)
        for item in ('DOME','HELIX'):
            assert gba_text(f'Do you want the {item} FOSSIL?') not in rom, f'Stale {item} fossil text in compiled ROM'
    with tempfile.TemporaryDirectory() as temp:
        for name,entry in manifest['maps'].items():
            subprocess.run([str(ROOT/'tools/mapjson/mapjson'),'map','emerald',str(ROOT/'data/maps'/name/'map.json'),str(ROOT/'data/layouts/layouts.json'),temp],cwd=ROOT,check=True,capture_output=True)
            emitted=(Path(temp)/'events.inc').read_text()
            dynamic_ids=re.findall(r'OBJ_EVENT_GFX_VAR_\\d+',emitted)
            if name in rival_map_scripts:
                assert dynamic_ids==['OBJ_EVENT_GFX_VAR_0'],(name,'rival graphic missing or extra dynamic actors',dynamic_ids)
                map_script=rival_map_scripts[name]
                assert entry.get('map_scripts')==map_script,(name,'rival map script disconnected')
                loader=re.search(rf'(?m)^{re.escape(map_script)}::\\s*\\n\\s*map_script MAP_SCRIPT_ON_LOAD, (\\w+)',rival_scripts)
                assert loader,(name,'rival graphic not initialized on map load')
                load_label=loader.group(1)
                assert re.search(rf'(?m)^{re.escape(load_label)}::\\s*\\n\\s*call Common_EventScript_SetupRivalGfxId',rival_scripts),(name,'missing rival graphics setup')
            else:
                assert not dynamic_ids,(name,'uninitialized sprite',dynamic_ids)
            assert 'OBJ_EVENT_GFX_SEAGALLOP' not in emitted,(name,'cutscene ferry')
            if name=='ViridianCity_Frlg':assert 'OBJ_EVENT_GFX_OLD_MAN_1' in emitted
            if name=='MtMoon_B2F_Frlg':
                assert 'FLAG_HIDE_DOME_FOSSIL' in emitted and 'FLAG_HIDE_HELIX_FOSSIL' in emitted
            if name=='CinnabarIsland_Frlg':assert 'OBJ_EVENT_GFX_BILL' in emitted
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
    print(f'PASS: {len(manifest["maps"])} emitted maps; no uninitialized actors or ferries; {len(speeches)} unique NPC speeches; {len(entries)} postgame encounter maps and {len(reserved)} future-Fakemon slot groups.')

if __name__=='__main__':audit()
