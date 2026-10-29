#!/usr/bin/env python3
"""Move selected existing legendary encounters to reachable Kanto ground."""
from collections import deque
import json
from pathlib import Path
import re
import struct
from check_kanto_connection import Map, ROOT

TARGETS = {
 'ARTICUNO':'SeafoamIslands_B4F_Frlg',
 'MOLTRES':'VictoryRoad_3F_Frlg',
 'ENTEI':'PokemonMansion_B1F_Frlg',
 'SUICUNE':'SeafoamIslands_1F_Frlg',
 'MOLTRES_GALAR':'PokemonTower_7F_Frlg',
 'ZAPDOS_GALAR':'Route23_Frlg',
}

def save(path,data):Path(path).write_text(json.dumps(data,indent=2,ensure_ascii=False)+'\n')

def main():
    all_maps={p.parent.name:(p,json.loads(p.read_text())) for p in (ROOT/'data/maps').glob('*/map.json')}
    placements=[]
    for species,target in TARGETS.items():
        label='BriskStatic_EventScript_'+species
        matches=[(name,p,d,o) for name,(p,d) in all_maps.items() for o in d.get('object_events',[]) if o.get('script')==label]
        assert len(matches)==1,(species,len(matches))
        oldname,oldpath,oldmap,obj=matches[0]
        if oldname==target:
            placements.append(dict(species=species,map=target,x=obj['x'],y=obj['y'],flag=obj['flag']));continue
        path,d=all_maps[target];terrain=Map(target)
        occupied={(o['x'],o['y']) for o in d['object_events']}
        triggers={(o['x'],o['y']) for o in d['warp_events']+d['coord_events']}
        safe={(x,y) for y in range(terrain.h) for x in range(terrain.w)
              if not terrain.block((x,y))&0xC00 and terrain.block((x,y))>>12 in (3,4)
              and terrain.behavior((x,y)) in (0,8,0x21) and (x,y) not in occupied}
        def neighbors(p):
            x,y=p;return [(x-1,y),(x+1,y),(x,y-1),(x,y+1)]
        seeds={n for p in triggers for n in [p]+neighbors(p) if n in safe}
        if d.get('connections'):seeds|={p for p in safe if p[0] in (0,terrain.w-1) or p[1] in (0,terrain.h-1)}
        reached=set(seeds);q=deque(seeds)
        while q:
            p=q.popleft()
            for n in neighbors(p):
                if n in safe and n not in reached:reached.add(n);q.append(n)
        candidates=[p for p in reached if sum(n in safe for n in neighbors(p))>=3
                    and all(abs(p[0]-x)+abs(p[1]-y)>3 for x,y in triggers)]
        assert candidates,(species,'no clear reachable ground')
        pos=max(candidates,key=lambda p:(min((abs(p[0]-x)+abs(p[1]-y) for x,y in triggers),default=1),-p[1],-p[0]))
        # Remove an inactive imported copy of this same legendary, if present.
        d['object_events']=[o for o in d['object_events'] if o.get('graphics_id')!='OBJ_EVENT_GFX_'+species]
        obj={**obj,'x':pos[0],'y':pos[1],'elevation':terrain.block(pos)>>12,'local_id':'LOCALID_BRISK_KANTO_'+species}
        d['object_events'].append(obj);oldmap['object_events'].remove(next(o for o in oldmap['object_events'] if o.get('script')==label))
        save(path,d);save(oldpath,oldmap)
        placements.append(dict(species=species,from_map=oldname,map=target,x=pos[0],y=pos[1],flag=obj['flag']))
    p=ROOT/'data/brisk_static_legendaries.inc';s=p.read_text()
    for species in TARGETS:
        s=re.sub(r'(BriskStatic_EventScript_'+species+r', SPECIES_'+species+r', )70(,)',r'\g<1>90\2',s)
    p.write_text(s)
    prior=ROOT/'data/brisk_kanto_legendaries.json'
    if prior.exists():
        previous={x['species']:x for x in json.loads(prior.read_text())}
        for entry in placements:
            if 'from_map' not in entry and entry['species'] in previous:entry['from_map']=previous[entry['species']]['from_map']
    save(prior,placements)
    print('Moved/verified',len(placements),'existing encounters; caught flags preserved.')

if __name__=='__main__':main()
