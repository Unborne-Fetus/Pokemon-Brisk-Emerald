#!/usr/bin/env python3
"""Apply the Palm Cape trail and Hex Maniac nook to the current Route 145.
Never changes encounter tables, connections or the island outline.
"""
from pathlib import Path
from orange_islands import *
a=next(a for a in current_areas() if a.name=='Route145')
# Preserve the island outline, encounter banks, pier, map links and existing rest spots.
grasstiles={1,5,0x10,0xA,0xB,0xC,0xD,0xE5,0xE3,0xD5,0xD3,0xE4,0xD4,0xDD,0xDB}
terrain=grasstiles|{SAND}
green={(x,y) for y,row in enumerate(a.grid) for x,v in enumerate(row) if v&1023 in grasstiles|{0x14,0x15,0x24,0x25,0x22E,0x22F,0xF7,0xFF,3}}
points=[(0,19),(10,22),(18,21),(27,17),(38,17),(47,19)]
for (ax,ay),(bx,by) in zip(points,points[1:]):
 steps=max(abs(bx-ax),abs(by-ay))
 for i in range(steps+1):
  x=round(ax+(bx-ax)*i/max(1,steps));y=round(ay+(by-ay)*i/max(1,steps))
  for yy in range(y-2,y+3):
   for xx in range(x-2,x+3):
    if 0<=xx<a.w and 0<=yy<a.h and a.grid[yy][xx]&1023 not in {WATER,0x10C,0x10D,0x10E,0x11C,0x11D,0x11E,0x114,0x116,0x205,0x206,0x207}:green.add((xx,yy))
for yy in range(17,23):
 for xx in range(9,16):green.add((xx,yy))
for yy in range(17,23):
 for xx in range(21,29):
  if a.grid[yy][xx]&1023 in terrain:green.add((xx,yy))
for x,y in green:
 if a.grid[y][x]&1023 not in terrain:continue
 n,e,s,w=[(x+dx,y+dy) not in green for dx,dy in [(0,-1),(1,0),(0,1),(-1,0)]]
 tile=0xE5 if n and w else 0xE3 if n and e else 0xD5 if s and w else 0xD3 if s and e else 0xE4 if n else 0xD4 if s else 0xDD if w else 0xDB if e else GRASS
 if a.grid[y][x]&1023 not in {0xA,0xB,0xC,0xD}:a.grid[y][x]=block(tile)
# Native palms and tropical shrubs break up the open grass/sand, as on 144.
for x,y in [(7,24),(19,24),(31,12),(40,13),(43,21)]:
 if all(a.grid[y+dy][x+dx]&1023 in terrain for dy in range(2) for dx in range(2)):
  a.stamp(x,y,[[0x14,0x15],[0x24,0x25]],True)
for x,y in [(10,20),(15,22),(19,20),(23,19),(28,19),(32,17),(36,18),(40,17),(25,22),(22,22)]:
 if a.grid[y][x]&1023==GRASS:a.grid[y][x]=block(5 if (x+y)%2 else 0x10)
# New resting nook and a lamp beside the main trail.
a.stamp(23,22,[[0x22E,0x22F]],True)
a.grid[20][28]=block(0xF7);a.grid[21][28]=block(0xFF,True)
a.grid[20][26]=block(GRASS)
# A real sign tile (tile 4 is flowers in this tileset).
a.grid[23][17]=block(3,True)
if not any(o.get('script')=='Route145_EventScript_HexManiac' for o in a.meta['object_events']):
 a.meta['object_events'].append(person('HEX_MANIAC',26,20,'Route145_EventScript_HexManiac'))
# Small flower groups mark the winding trail and flank each resting bench.
for x,y in [(4,21),(6,22),(9,23),(12,24),(15,23),(19,23),(21,21),
            (24,20),(29,18),(33,18),(37,19),(41,19),(44,20),
            (10,18),(13,18),(29,19),(32,19),(22,22),(25,22),
            (19,18),(20,18),(19,19),(20,19),(34,15),(35,15),(34,16),(35,16)]:
 if a.grid[y][x]&1023 in (GRASS,5,0x10):a.grid[y][x]=block(5 if (x+y)%3 else 0x10)
write_words('data/layouts/Route145/map.bin',[v for row in a.grid for v in row]);save('data/maps/Route145/map.json',a.meta)
audit_current([a]);render(current_areas())
