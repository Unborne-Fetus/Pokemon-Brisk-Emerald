#!/usr/bin/env python3
"""Inspect, polish and render the current Orange Islands maps.
--polish updates the town and tidies grass; --render and --check are read-only
with respect to map data. Existing encounter tables are never regenerated.
Run from the repository root. Pillow is only required for --render previews.
"""
from pathlib import Path
import argparse, collections, copy, json, math, struct
ROOT = Path(__file__).resolve().parents[2]
PRI = ROOT / 'data/tilesets/primary/aqua_orange_islands'
SEC = ROOT / 'data/tilesets/secondary/aqua_valencia_island'
WATER, SAND, GRASS = 0x12B, 0x115, 1
NAMES = ['Route143', 'Route144', 'OrangeCity', 'Route145', 'Route146', 'Route149', 'Route150']
DIMS = [(40,36), (48,36), (48,40), (48,36), (40,44)]
def load(path): return json.loads((ROOT/path).read_text())
def save(path, data):
    p=ROOT/path; p.parent.mkdir(parents=True, exist_ok=True); p.write_text(json.dumps(data,indent=2)+'\n')
def words(path):
    raw=Path(path).read_bytes(); return list(struct.unpack('<'+'H'*(len(raw)//2),raw))
def write_words(path, values):
    p=ROOT/path; p.parent.mkdir(parents=True,exist_ok=True); p.write_bytes(struct.pack('<'+'H'*len(values),*values))
def block(tile, solid=False, elevation=3): return tile | (int(solid)<<10) | (elevation<<12)
def rect(mask,x0,y0,x1,y1):
    for y in range(max(0,y0),min(len(mask),y1)):
        for x in range(max(0,x0),min(len(mask[0]),x1)): mask[y][x]=True
def ellipse(mask,cx,cy,rx,ry,value=True):
    for y,row in enumerate(mask):
        for x in range(len(row)):
            if ((x-cx)/rx)**2+((y-cy)/ry)**2<=1: row[x]=value

def person(gfx,x,y,script,local_id=None):
    obj=dict(graphics_id='OBJ_EVENT_GFX_'+gfx,x=x,y=y,elevation=3,movement_type='MOVEMENT_TYPE_FACE_DOWN',movement_range_x=0,movement_range_y=0,trainer_type='TRAINER_TYPE_NONE',trainer_sight_or_berry_tree_id='0',script=script,flag='0')
    if local_id: obj={'local_id':local_id,**obj}
    return obj

def exterior(name,w,h):
    return dict(id='MAP_'+name.upper().replace('ORANGECITY','ORANGE_CITY').replace('POKEMONCENTER','POKEMON_CENTER'),name=name,layout='LAYOUT_'+name.upper().replace('ORANGECITY','ORANGE_CITY').replace('POKEMONCENTER','POKEMON_CENTER'),music='MUS_DEWFORD' if name=='OrangeCity' else 'MUS_ROUTE110',region='REGION_HOENN',region_map_section='MAPSEC_ORANGE_CITY' if name=='OrangeCity' else 'MAPSEC_ROUTE_'+name[5:],requires_flash=False,weather='WEATHER_SUNNY',map_type='MAP_TYPE_CITY' if name=='OrangeCity' else 'MAP_TYPE_ROUTE',allow_cycling=True,allow_escaping=False,allow_running=True,show_map_name=True,battle_scene='MAP_BATTLE_SCENE_NORMAL',connections=[],object_events=[],warp_events=[],coord_events=[],bg_events=[])

class Area:
    def __init__(self,name,w,h):
        self.name,self.w,self.h=name,w,h; self.land=[[False]*w for _ in range(h)]
        self.green=[[False]*w for _ in range(h)]; self.grid=[[block(WATER,elevation=1)]*w for _ in range(h)]
        self.meta=exterior(name,w,h); self.messages=[]; self.doors=[]
    def has(self,mask,x,y): return 0<=x<self.w and 0<=y<self.h and mask[y][x]
    def path(self,pts,width=3):
        for (ax,ay),(bx,by) in zip(pts,pts[1:]):
            for i in range(max(abs(bx-ax),abs(by-ay))+1):
                n=max(abs(bx-ax),abs(by-ay),1);x=round(ax+(bx-ax)*i/n);y=round(ay+(by-ay)*i/n)
                for yy in range(y-width//2,y+width//2+1):
                    for xx in range(x-width//2,x+width//2+1):
                        if 0<=xx<self.w and 0<=yy<self.h: self.green[yy][xx]=False
    def ground(self):
        for y in range(self.h):
            for x in range(self.w):
                # Outside-map neighbors continue the connecting land strip.
                if not self.land[y][x]: continue
                sides=[not self.has(self.land,x+dx,y+dy) for dx,dy in [(0,-1),(1,0),(0,1),(-1,0)]]
                if x==0 and self.name!='Route143' and 16<=y<=22: sides[3]=False
                if x==self.w-1 and self.name!='Route146' and 16<=y<=22: sides[1]=False
                n,e,s,w=sides
                tile=0x10C if n and w else 0x10E if n and e else 0x11C if s and w else 0x11E if s and e else 0x10D if n else 0x11D if s else 0x114 if w else 0x116 if e else SAND
                self.grid[y][x]=block(tile)
                if self.green[y][x]:
                    n,e,s,w=[not self.has(self.green,x+dx,y+dy) for dx,dy in [(0,-1),(1,0),(0,1),(-1,0)]]
                    tile=0xE5 if n and w else 0xE3 if n and e else 0xD5 if s and w else 0xD3 if s and e else 0xE4 if n else 0xD4 if s else 0xDD if w else 0xDB if e else GRASS
                    self.grid[y][x]=block(tile)
    def stamp(self,x,y,pattern,solid=True,doors=()):
        for dy,row in enumerate(pattern):
            for dx,tile in enumerate(row):
                if 0<=x+dx<self.w and 0<=y+dy<self.h:
                    self.grid[y+dy][x+dx]=block(tile,solid and (dx,dy) not in doors)
    def palm(self,x,y):
        if all(self.has(self.green,x+dx,y+dy) for dy in range(2) for dx in range(2)):
            self.stamp(x,y,[[0x14,0x15],[0x24,0x25]])
    def pave(self,x,y,w,h):
        self.stamp(x,y,[[([0x138,0x139,0x13A] if yy==0 else [0x148,0x149,0x14A] if yy==h-1 else [0x140,0x141,0x142])[0 if xx==0 else 2 if xx==w-1 else 1] for xx in range(w)] for yy in range(h)],False)
    def boardwalk(self,x,y,w,h):
        self.stamp(x,y,[[0x205 if xx==0 else 0x207 if xx==w-1 else 0x206 for xx in range(w)] for yy in range(h)],False)
    def sign(self,x,y,label,text):
        self.grid[y][x]=block(4,True)
        script=self.name+'_EventScript_'+label
        self.meta['bg_events'].append(dict(type='sign',x=x,y=y,elevation=0,player_facing_dir='BG_EVENT_PLAYER_FACING_ANY',script=script))
        self.messages.append((label,text,'MSGBOX_SIGN'))
    def npc(self,gfx,x,y,label,text):
        self.meta['object_events'].append(person(gfx,x,y,self.name+'_EventScript_'+label)); self.messages.append((label,text,'MSGBOX_NPC'))
    def building(self,x,y,kind,room):
        if kind=='center': pat=[[0x48,0x49,0x4A,0x4B,0x4C],[0x50,0x51,0x52,0x53,0x54],[0x58,0x59,0x5A,0x5B,0x5C]]; door=(2,2)
        elif kind=='mart':pat=[[0x28,0x29,0x29,0x2B],[0x30,0x31,0x31,0x33],[0x38,0x39,0x3A,0x3B]];door=(1,2)
        else:pat=[[0x280,0x281,0x281,0x281,0x282],[0x288,0x289,0x289,0x289,0x28A],[0x290,0x291,0x291,0x291,0x294],[0x298,0x299,0x29A,0x29B,0x29C]];door=(1,3)
        self.stamp(x,y,pat,True,[door]); dx,dy=x+door[0],y+door[1]
        idx=len(self.meta['warp_events']); self.meta['warp_events'].append(dict(x=dx,y=dy,elevation=0,dest_map=room,dest_warp_id='0')); self.doors.append((dx,dy,room,idx))
    def emit(self):
        p='data/layouts/'+self.name
        write_words(p+'/map.bin',[v for row in self.grid for v in row]);write_words(p+'/border.bin',[block(WATER,elevation=1)]*4)
        save('data/maps/'+self.name+'/map.json',self.meta)
        scripts=self.name+'_MapScripts::\n\t.byte 0\n'
        for label,text,box in self.messages:
            scripts+='\n'+self.name+'_EventScript_'+label+'::\n\tmsgbox '+self.name+'_Text_'+label+', '+box+'\n\tend\n\n'+self.name+'_Text_'+label+':\n'
            lines=text.split('\n')
            for i,line in enumerate(lines): scripts+='\t.string '+json.dumps(line+('\\n' if i<len(lines)-1 else '$'),ensure_ascii=False).replace('\\\\n','\\n')+'\n'
        (ROOT/'data/maps'/self.name/'scripts.inc').write_text(scripts)

def current_areas():
    layouts=load('data/layouts/layouts.json')['layouts']
    areas=[]
    for name in NAMES:
        meta=load('data/maps/'+name+'/map.json')
        layout=next(row for row in layouts if row['id']==meta['layout'])
        a=Area(name,layout['width'],layout['height']);a.meta=meta
        data=words(ROOT/layout['blockdata_filepath'])
        a.grid=[data[y*a.w:(y+1)*a.w] for y in range(a.h)]
        areas.append(a)
    return areas

def polish():
    # A compact town square, service district, homes and harbor share one
    # connected street network. Retain map/layout IDs and the existing rooms.
    c=Area('OrangeCity',48,40)
    ellipse(c.land,24,20,23,18);rect(c.land,6,8,43,32)
    for y in range(c.h):
        for x in range(c.w):
            c.green[y][x]=all(c.has(c.land,x+dx,y+dy) for dx,dy in [(0,0),(0,2),(0,-2),(2,0),(-2,0)])
    c.ground()
    paving=[[False]*c.w for _ in range(c.h)]
    for box in [(0,18,48,21),(13,11,16,21),(33,12,36,21),
                (19,15,30,27),(9,28,38,31),(9,28,12,31),
                (34,27,37,31),(23,26,26,33)]:rect(paving,*box)
    for y in range(c.h):
        for x in range(c.w):
            if not paving[y][x]:continue
            n,e,s,w=[not c.has(paving,x+dx,y+dy) for dx,dy in [(0,-1),(1,0),(0,1),(-1,0)]]
            tile=0x138 if n and w else 0x13A if n and e else 0x148 if s and w else 0x14A if s and e else 0x139 if n else 0x149 if s else 0x140 if w else 0x142 if e else 0x141
            c.grid[y][x]=block(tile)
    # Wooden approaches join the neighboring island paths over the shore.
    c.boardwalk(0,18,6,3);c.boardwalk(42,18,6,3);c.boardwalk(21,32,6,8)
    center=[[0x40,0x41,0x42,0x43,GRASS],[0x48,0x49,0x4A,0x4B,0x4C],
            [0x50,0x51,0x52,0x53,0x54],[0x58,0x59,0x5A,0x5B,0x5C]]
    c.stamp(12,7,center,True,[(2,3),(4,0)])
    c.stamp(32,8,[[0x2C,0x2D,0x2E,0x2F],[0x34,0x35,0x36,0x37],
                   [0x3C,0x3D,0x3E,0x3F],[0x44,0x45,0x46,0x47]],True,[(2,3)])
    c.stamp(9,25,[[0x280,0x281,0x281,0x283,0x284],[0x288,0x289,0x28A,0x28B,0x28C],
                   [0x290,0x291,0x292,0x293,0x294],[0x298,0x299,0x29A,0x29B,0x29C]],True,[(1,3)])
    c.stamp(34,25,[[0x28,0x29,0x29,0x2B],[0x30,0x31,0x31,0x33],[0x38,0x39,0x3A,0x3B]],True,[(1,2)])
    # A planted pool sits below the through street, with room to walk around it.
    c.stamp(23,22,[[0x10C,0x10D,0x10E],[0x114,WATER,0x116],[0x11C,0x11D,0x11E]],False)
    c.grid[23][24]=block(WATER,elevation=1)
    for x,y in [(20,22),(28,22),(20,25),(28,25)]:c.grid[y][x]=block(0x10)
    for x,y in [(7,11),(19,8),(28,8),(40,12),(7,24),(40,25),
                (14,32),(32,32),(18,31),(28,31)]:c.palm(x,y)
    for x0,y0 in [(19,11),(27,11),(6,27),(39,28)]:
        for y in range(y0,y0+2):
            for x in range(x0,x0+2):
                if c.grid[y][x]&1023==GRASS:c.grid[y][x]=block(0x10)
    c.meta=load('data/maps/OrangeCity/map.json')
    c.meta['warp_events']=[dict(x=x,y=y,elevation=0,dest_map=dest,dest_warp_id='0')
        for x,y,dest in [(14,10,'MAP_ORANGE_CITY_POKEMON_CENTER_1F'),
                         (34,11,'MAP_ORANGE_CITY_MART'),(10,28,'MAP_ORANGE_CITY_HOUSE1'),
                         (35,27,'MAP_ORANGE_CITY_HOUSE2')]]
    for obj,pos in zip(c.meta['object_events'],[(18,18),(24,33),(31,21)]):obj['x'],obj['y']=pos
    c.meta['bg_events'][0]['x']=18;c.meta['bg_events'][0]['y']=16;c.grid[16][18]=block(4,True)
    save('data/maps/OrangeCity/map.json',c.meta)
    write_words('data/layouts/OrangeCity/map.bin',[v for row in c.grid for v in row])
    # Each room returns to its own entrance; the duplicate mart entrance is gone.
    for room,warp_id in [('OrangeCity_PokemonCenter_1F',0),('OrangeCity_Mart',1),('OrangeCity_House1',2),('OrangeCity_House2',3)]:
        meta=load('data/maps/'+room+'/map.json')
        for warp in meta['warp_events']:
            if warp['dest_map']=='MAP_ORANGE_CITY':warp['dest_warp_id']=str(warp_id)
        save('data/maps/'+room+'/map.json',meta)
    heal=load('src/data/heal_locations.json')
    for row in heal['heal_locations']:
        if row['id']=='HEAL_LOCATION_ORANGE_CITY':row['x']=14;row['y']=11
    save('src/data/heal_locations.json',heal)
    # Normalize the four grass quadrants and remove disconnected grass fragments
    # painted into open sea, retaining the hand-built rock lanes and shorelines.
    for a in current_areas():
        if a.name=='OrangeCity':continue
        grass={(x,y) for y,row in enumerate(a.grid) for x,v in enumerate(row) if 0xA<=v&1023<=0xD}
        for x,y in grass:
            a.grid[y][x]=block([[0xA,0xB],[0xC,0xD]][y%2][x%2])
        for x,y in grass:
            # A grass-only island separated from every beach is a stray sea fragment.
            todo=[(x,y)];seen=set();land=False
            while todo and len(seen)<64:
                xx,yy=todo.pop()
                if (xx,yy) in seen:continue
                seen.add((xx,yy))
                for dx,dy in [(0,-1),(1,0),(0,1),(-1,0)]:
                    nx,ny=xx+dx,yy+dy
                    if not (0<=nx<a.w and 0<=ny<a.h):continue
                    tile=a.grid[ny][nx]&1023
                    if (nx,ny) in grass and (nx,ny) not in seen:todo.append((nx,ny))
                    elif tile not in [WATER,0xA,0xB,0xC,0xD]:land=True
            if not land and len(seen)<64:
                for xx,yy in seen:a.grid[yy][xx]=block(WATER,elevation=1)
        if a.name=='Route149':
            for warp in a.meta['warp_events']:
                if warp['dest_map']=='MAP_ROUTE148':a.grid[warp['y']][warp['x']]=block(0x3FF)
        write_words('data/layouts/'+a.name+'/map.bin',[v for row in a.grid for v in row])
    # The lookout's cave was painted solid and lost its warp in a map upload.
    cave=load('data/maps/Route146/map.json');cave['warp_events']=[dict(x=27,y=12,elevation=0,dest_map='MAP_DYNAMAX_ADVENTURE_CAVE',dest_warp_id='0')]
    cave['object_events'][0]['x']=23;cave['object_events'][0]['y']=16
    save('data/maps/Route146/map.json',cave)
    grid=words(ROOT/'data/layouts/Route146/map.bin');grid[12*40+27]=block(0x66,elevation=4);write_words('data/layouts/Route146/map.bin',grid)

def render(areas):
    from PIL import Image,ImageDraw
    palettes=[]
    for n in range(13):
        lines=((PRI if n<6 else SEC)/f'palettes/{n:02}.pal').read_text().splitlines()[3:19]
        palettes.append([tuple(map(int,line.split())) for line in lines])
    tiles=[]
    for path in (PRI,SEC):
        im=Image.open(path/'tiles.png')
        for y in range(0,im.height,8):
            for x in range(0,im.width,8):tiles.append(im.crop((x,y,x+8,y+8)))
    metas=words(PRI/'metatiles.bin')+words(SEC/'metatiles.bin');cache={}
    def image(n):
        if n in cache:return cache[n]
        out=Image.new('RGBA',(16,16),(0,0,0,255))
        for layer in range(3):
            for j in range(4):
                v=metas[n*12+layer*4+j];sub=tiles[v&1023].copy()
                if v&1024:sub=sub.transpose(Image.Transpose.FLIP_LEFT_RIGHT)
                if v&2048:sub=sub.transpose(Image.Transpose.FLIP_TOP_BOTTOM)
                pixels=Image.new('RGBA',(8,8));pixels.putdata([(*palettes[v>>12][int(i)%16],255 if int(i)%16 else 0) for i in (sub.get_flattened_data() if hasattr(sub, 'get_flattened_data') else sub.getdata())])
                out.alpha_composite(pixels,((j%2)*8,(j//2)*8))
        cache[n]=out.convert('RGB');return cache[n]
    cards=[]
    for a in areas:
        im=Image.new('RGB',(a.w*16,a.h*16))
        for y,row in enumerate(a.grid):
            for x,v in enumerate(row):im.paste(image(v&1023),(x*16,y*16))
        d=ImageDraw.Draw(im)
        for obj in a.meta['object_events']:
            x,y=obj['x']*16,obj['y']*16;d.ellipse((x+4,y+1,x+12,y+9),fill='#ffd69e',outline='#493e34');d.rectangle((x+4,y+9,x+12,y+15),fill='#ba552d',outline='#493e34')
        im.convert('P',palette=Image.Palette.ADAPTIVE,colors=256).save(ROOT/f'docs/maps/{a.name}.png',optimize=True)
        card=Image.new('RGB',(a.w*16,a.h*16+32),'#193a41');card.paste(im,(0,32));draw=ImageDraw.Draw(card);draw.fontmode='1';draw.text((12,10),a.name.replace('OrangeCity','Orange City'),fill='white');cards.append(card)
    overview=Image.new('RGB',(max(c.width for c in cards)*3+32,max(c.height for c in cards)*math.ceil(len(cards)/3)+24),'#10272e')
    for i,c in enumerate(cards):overview.paste(c,((i%3)*(max(k.width for k in cards)+12),(i//3)*(max(k.height for k in cards)+12)))
    overview.convert('P',palette=Image.Palette.ADAPTIVE,colors=256).save(ROOT/'docs/maps/orange-islands.png',optimize=True)

def audit_current(areas):
    all_maps={load(str(p.relative_to(ROOT)))['id']:load(str(p.relative_to(ROOT))) for p in (ROOT/'data/maps').glob('*/map.json')}
    attrs=words(PRI/'metatile_attributes.bin')+words(SEC/'metatile_attributes.bin')
    for a in areas:
        assert len(a.grid)==a.h and all(len(row)==a.w for row in a.grid)
        for warp in a.meta['warp_events']:
            assert 0<=warp['x']<a.w and 0<=warp['y']<a.h
            dest=all_maps[warp['dest_map']];assert int(warp['dest_warp_id'])<len(dest['warp_events'])
            value=a.grid[warp['y']][warp['x']]
            assert (value>>10)&3==0,(a.name,'blocked warp',warp)
            assert attrs[value&1023]&0xFF in [0x60,0x65,0x69],(a.name,'wrong warp tile',warp)
    c=next(a for a in areas if a.name=='OrangeCity')
    occupied={(o['x'],o['y']) for o in c.meta['object_events']}
    def walk(x,y):
        return 0<=x<c.w and 0<=y<c.h and (c.grid[y][x]>>10)&3==0 and c.grid[y][x]>>12==3 and (x,y) not in occupied
    q=collections.deque([(0,19)]);seen={(0,19)}
    while q:
        x,y=q.popleft()
        for dx,dy in [(0,-1),(1,0),(0,1),(-1,0)]:
            pos=x+dx,y+dy
            if pos not in seen and walk(*pos):seen.add(pos);q.append(pos)
    targets=[(47,19),(14,11),(24,38)]+[(w['x'],w['y']) for w in c.meta['warp_events']]
    for target in targets:assert target in seen,('OrangeCity unreachable',target)
    for obj in c.meta['object_events']:
        assert (c.grid[obj['y']][obj['x']]>>10)&3==0,('NPC on solid tile',obj)
        assert any((obj['x']+dx,obj['y']+dy) in seen for dx,dy in [(0,-1),(1,0),(0,1),(-1,0)])
    for i,warp in enumerate(c.meta['warp_events']):
        dest=all_maps[warp['dest_map']]
        for back in dest['warp_events']:
            if back['dest_map']==c.meta['id']:assert int(back['dest_warp_id'])==i,(dest['name'],'wrong return door')
    print(f'PASS: {len(areas)} island maps; warp destinations/behaviors valid. City services, exits, residents, harbor and respawn reachable.')

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--render',action='store_true',help='Render the current maps without rebuilding them.')
    parser.add_argument('--polish',action='store_true',help='Apply the Orange City layout and route grass cleanup.')
    parser.add_argument('--check',action='store_true',help='Check current map warps and city access (the default).')
    args=parser.parse_args()
    if args.polish:polish()
    areas=current_areas();audit_current(areas)
    if args.render:render(areas)
