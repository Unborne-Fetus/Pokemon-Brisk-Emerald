#!/usr/bin/env python3
"""Audit the checked-in postgame rosters, dialogue and relocated encounters."""
from collections import Counter
import json
import re
from pathlib import Path
from check_trainer_progression import ROOT, TEAM_RE
from check_kanto_connection import Map


def audit():
    source=(ROOT/'src/data/brisk_kanto_teams.party').read_text()
    records={}
    for tid,body in TEAM_RE.findall(source):
        mode=re.search(r'^Difficulty: (\w+)',body,re.M)[1]
        assert (tid,mode) not in records,(tid,mode)
        assert 'Authored Party: Yes' in body
        levels=list(map(int,re.findall(r'^Level: (\d+)',body,re.M)))
        species=re.findall(r'^(SPECIES_\w+)',body,re.M)
        assert len(levels)==len(species)==6,(tid,mode)
        assert all(90<=level<=100 for level in levels),(tid,mode,levels)
        assert len(set(species))==6,(tid,mode,'duplicate species')
        assert len(re.findall(r'^- MOVE_',body,re.M))==24,(tid,mode,'moves')
        assert len(re.findall(r'^Ability: ABILITY_',body,re.M))==6,(tid,mode,'abilities')
        records[tid,mode]=Counter(species)
    trainers={tid for tid,mode in records}
    assert len(trainers)==623
    for tid in trainers:
        parties=[records[tid,mode] for mode in ('Easy','Normal','Hard','Kaizo')]
        assert all(p!=q for i,p in enumerate(parties) for q in parties[i+1:]),tid
    script=(ROOT/'data/scripts/brisk_kanto_postgame.inc').read_text()
    assert '\\\\n' not in script and '\\\\p' not in script,'Doubled dialogue escape'
    assert 'setflag FLAG_BADGE' not in script and 'setvar VAR_MAP_SCENE' not in script
    assert 'Hoenn' in script and 'Champion' in script and 'savior' in script
    labels=set(re.findall(r'^(\w+)::?',script,re.M))
    manifest=json.loads((ROOT/'data/brisk_kanto_postgame.json').read_text())
    for mapname,entry in manifest['maps'].items():
        for obj in entry['objects'].values():
            label=obj['script']
            if label.startswith('BriskKanto_'):assert label in labels,(mapname,label)
            if obj['trainer']:assert obj['trainer'] in trainers,(mapname,obj)
    placements=json.loads((ROOT/'data/brisk_kanto_legendaries.json').read_text())
    assert len(placements)==6
    maps=[json.loads(p.read_text()) for p in (ROOT/'data/maps').glob('*/map.json')]
    for entry in placements:
        label='BriskStatic_EventScript_'+entry['species']
        matches=[(m,o) for m in maps for o in m.get('object_events',[]) if o.get('script')==label]
        assert len(matches)==1,(entry['species'],'duplicate/missing encounter')
        m,o=matches[0];assert m['name']==entry['map'] and o['flag']==entry['flag'],entry
        t=Map(entry['map']);p=(o['x'],o['y']);b=t.block(p)
        assert not b&0xC00 and b>>12==o['elevation'] and t.behavior(p) in (0,8,0x21),entry
        assert sum((x['x'],x['y'])==p for x in m['object_events'])==1,entry
        assert any(r['script']==label and r.get('keep_flag') for r in manifest['maps'][entry['map']]['objects'].values()),entry
    print(f'PASS: {len(trainers)} identities, {len(records)} distinct four-mode six-Pokemon rosters at levels 90-100; champion dialogue and six unique relocated encounters.')

if __name__=='__main__':audit()
