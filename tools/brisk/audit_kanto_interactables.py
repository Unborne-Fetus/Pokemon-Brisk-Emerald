#!/usr/bin/env python3
"""Classify every Kanto map item ball and physical interaction before ROM build."""
import json
from pathlib import Path
import re

ROOT=Path(__file__).resolve().parents[2]
item_scripts=dict(re.findall(r'^(\w+)::\s*\n\s*finditem\s+(ITEM_\w+)',
    (ROOT/'data/scripts/item_ball_scripts_frlg.inc').read_text(),re.M))
special={
    'PowerPlant_EventScript_Electrode1':'ELECTRODE',
    'PowerPlant_EventScript_Electrode2':'ELECTRODE',
    'SaffronCity_Dojo_EventScript_HitmonleeBall':'HITMONLEE',
    'SaffronCity_Dojo_EventScript_HitmonchanBall':'HITMONCHAN',
    'CeladonCity_Condominiums_RoofRoom_EventScript_EeveeBall':'EEVEE',
    'MtMoon_B2F_EventScript_DomeFossil':'DOME_FOSSIL',
    'MtMoon_B2F_EventScript_HelixFossil':'HELIX_FOSSIL',
}
frlg_flags=dict((name,int(value,16)) for name,value in re.findall(
    r'^#define\s+(FLAG_HIDE_\w+)\s+(0x[0-9A-Fa-f]+)',
    (ROOT/'include/constants/flags_frlg.h').read_text(),re.M))
unclassified=[]
balls=0
maps=0
for path in sorted((ROOT/'data/maps').glob('*/map.json')):
    m=json.loads(path.read_text())
    if m.get('region')!='REGION_KANTO' or m.get('name')=='OrangeStraitEast':
        continue
    maps+=1
    for i,o in enumerate(m.get('object_events',[]),1):
        if o.get('graphics_id') not in ('OBJ_EVENT_GFX_ITEM_BALL','OBJ_EVENT_GFX_FOSSIL_FRLG'):
            continue
        balls+=1
        script=o.get('script','')
        flag=o.get('flag','')
        if script not in item_scripts and script not in special:
            unclassified.append((m['name'],i,script,'unknown ball interaction'))
        if flag not in frlg_flags or frlg_flags.get(flag,512)>=511:
            unclassified.append((m['name'],i,flag,'missing isolated hide flag'))
print(f'Kanto physical object inventory: {maps} maps, {balls} balls/fossils.')
for entry in unclassified:
    print('UNCLASSIFIED:', *entry)
if unclassified:
    raise SystemExit(f'{len(unclassified)} Kanto physical objects need explicit gameplay behavior')
print('PASS: every Kanto item ball/fossil is a pickup, gift, or static battle with a unique FRLG flag.')
