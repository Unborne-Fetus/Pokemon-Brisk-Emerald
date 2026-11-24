#!/usr/bin/env python3
"""Check physical Kanto map seams and static access to postgame battle actors.

This is a conservative geometry scan, not a substitute for emulator gameplay.
Scripted tile replacements, doors, ledges, Strength and Surf can alter routes.
"""
import json
from pathlib import Path
import struct
from collections import deque

ROOT = Path(__file__).resolve().parents[2]
def load(p):
    return json.loads(p.read_text(encoding="utf-8-sig"))
maps = {m["id"]: m for p in (ROOT/"data/maps").glob("*/map.json") for m in [load(p)]}
layouts = {l["id"]: l for l in load(ROOT/"data/layouts/layouts.json")["layouts"]}
class Grid:
    def __init__(self,m):
        self.m=m
        l=layouts[m["layout"]]
        self.w,self.h=l["width"],l["height"]
        self.blocks=struct.unpack("<"+"H"*(self.w*self.h),(ROOT/l["blockdata_filepath"]).read_bytes())
    def inside(self,x,y):
        return 0<=x<self.w and 0<=y<self.h
    def walk(self,x,y):
        return self.inside(x,y) and not self.blocks[y*self.w+x]&0xC00
    def neighbors(self,x,y):
        return [(a,b) for a,b in [(x-1,y),(x+1,y),(x,y-1),(x,y+1)] if self.inside(a,b)]
    def border(self,side,i):
        return {"up":(i,0),"down":(i,self.h-1),"left":(0,i),"right":(self.w-1,i)}[side]
    def size(self,side):
        return self.w if side in ("up","down") else self.h

kanto=[m for m in maps.values() if m.get("region")=="REGION_KANTO"]
grids={}
def grid(m):
    if m["id"] not in grids:grids[m["id"]]=Grid(m)
    return grids[m["id"]]
opposite={"up":"down","down":"up","left":"right","right":"left"}
seams=[]; blocked=[]; asymmetric=[]; actor_review=[]; count=0
for m in kanto:
    g=grid(m); starts=set()
    for w in m.get("warp_events",[]):
        x,y=w["x"],w["y"]
        if g.inside(x,y):
            starts.update(p for p in [(x,y)]+g.neighbors(x,y) if g.walk(*p))
    for c in m.get("connections") or []:
        side=c["direction"]
        if side not in opposite or c["map"] not in maps:continue
        t=maps[c["map"]];h=grid(t);off=c["offset"];back=opposite[side]
        paired=[]
        for i in range(h.size(back)):
            j=i+off
            if not 0<=j<g.size(side):continue
            a,b=g.border(side,j),h.border(back,i)
            if g.walk(*a) and h.walk(*b):paired.append((a,b))
        starts.update(a for a,b in paired)
        seams.append((m["name"],side,t["name"],len(paired)))
        if not paired:blocked.append(seams[-1])
        if not any(z["map"]==m["id"] and z["direction"]==back and z["offset"]==-off for z in t.get("connections") or []):asymmetric.append(seams[-1])
    seen=set(starts);q=deque(starts)
    while q:
        x,y=q.popleft()
        for p in g.neighbors(x,y):
            if p not in seen and g.walk(*p):
                seen.add(p);q.append(p)
    for o in m.get("object_events",[]):
        script=o.get("script","")
        if not script.startswith(("XiphosKanto_EventScript_","BriskKantoRival_EventScript_")):continue
        count+=1
        x,y=o["x"],o["y"]
        if not g.walk(x,y) or not any(p in seen for p in g.neighbors(x,y)):
            actor_review.append((m["name"],script,x,y,"scripted or obstructed"))
print("KANTO MAPS",len(kanto),"DIRECTIONAL SEAMS",len(seams),"STORY ACTORS",count)
print("NO PHYSICAL SEAM:",blocked)
print("ASYMMETRIC SEAMS:",asymmetric)
print("STORY ACCESS REVIEW:",actor_review)
print("NOTE: no-physical-seam records include intentional route gate/border layouts.")
