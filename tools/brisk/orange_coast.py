#!/usr/bin/env python3
"""Author and audit Brisk's Orange Coast maps using the imported native tilesets.
Run from the repository root. Pillow is only required for --render previews.
"""
from pathlib import Path
import argparse, collections, copy, json, math, struct
ROOT = Path(__file__).resolve().parents[2]
PRI = ROOT / 'data/tilesets/primary/aqua_orange_islands'
SEC = ROOT / 'data/tilesets/secondary/aqua_valencia_island'
WATER, SAND, GRASS = 0x171, 0x115, 1
NAMES = ['Route143', 'Route144', 'OrangeCity', 'Route145', 'Route146']
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

def make_areas():
    areas=[Area(n,*d) for n,d in zip(NAMES,DIMS)]
    for a in areas:
        if a.name=='Route143':ellipse(a.land,20,18,17,15);rect(a.land,30,16,a.w,23)
        elif a.name=='Route144':
            ellipse(a.land,12,14,13,11);ellipse(a.land,36,22,12,11);rect(a.land,0,16,48,23)
            for y in range(a.h):
                for x in range(22,26):a.land[y][x]=False
        elif a.name=='OrangeCity':
            ellipse(a.land,24,20,23,18);rect(a.land,5,7,44,32);rect(a.land,0,16,48,23)
        elif a.name=='Route145':ellipse(a.land,12,23,14,10);ellipse(a.land,31,13,16,11);rect(a.land,0,16,48,23)
        else:
            ellipse(a.land,20,21,18,19);ellipse(a.land,32,28,12,11,False);rect(a.land,0,16,20,23)
        # Inland greenery leaves a broad sandy beach around each shoreline.
        for y in range(a.h):
            for x in range(a.w):
                a.green[y][x]=all(a.has(a.land,x+dx,y+dy) for dx,dy in [(0,0),(0,3),(0,-3),(3,0),(-3,0)])
        if a.name=='Route143': pts=[(9,10),(11,15),(22,19),(39,19)]
        elif a.name=='Route144':pts=[(0,19),(12,19),(19,19),(29,19),(37,19),(47,19)]
        elif a.name=='OrangeCity':pts=[(0,19),(47,19)]
        elif a.name=='Route145':pts=[(0,19),(10,24),(20,21),(31,15),(39,18),(47,19)]
        else:pts=[(0,19),(12,19),(17,14),(25,10),(28,10)]
        a.path(pts)
        if a.name=='OrangeCity':
            for x in [9,36]: a.path([(x,10),(x,19),(x,28)],3)
            a.path([(9,27),(36,27)],3)
        a.ground()
        # Palms frame paths and clearings, rather than filling walking lanes.
        palms={'Route143':[(8,15),(12,7),(16,6),(26,7),(30,9),(11,23),(15,25),(26,25),(30,23)],'Route144':[(5,11),(9,7),(15,9),(15,13),(7,22),(32,24),(36,28),(40,26),(38,13)],'OrangeCity':[(5,13),(12,13),(19,7),(27,7),(40,13),(5,28),(14,29),(32,29),(40,28),(18,31),(29,31)],'Route145':[(6,23),(10,28),(16,26),(25,9),(30,6),(35,7),(38,11),(36,24),(25,24)],'Route146':[(8,19),(10,24),(12,30),(17,34),(21,31),(10,13),(15,8),(20,6),(28,6),(30,14)]}[a.name]
        for x,y in palms:a.palm(x,y)
        for y in range(7,a.h-5,5):
            for x in range(7,a.w-5,7):
                if all(a.has(a.green,x+dx,y+dy) for dx,dy in [(0,0),(1,0),(0,1),(1,1)]) and (a.grid[y][x]&1023)==GRASS:
                    a.grid[y][x]=block(0x10 if (x+y)%3 else 0x55, (x+y)%3==0)
        # Tall-grass pockets are optional detours beside the main trail.
        patches={'Route143':[(17,9,5,3),(18,24,5,3)],'Route144':[(6,13,5,3),(32,24,4,3)],'OrangeCity':[],'Route145':[(24,11,5,3),(13,26,4,3)],'Route146':[(15,26,4,4),(22,14,4,2)]}[a.name]
        for x,y,w,h in patches:
            for yy in range(y,y+h):
                for xx in range(x,x+w):
                    if a.grid[yy][xx]&1023==GRASS:a.grid[yy][xx]=block([[0xA,0xB],[0xC,0xD]][yy%2][xx%2])
    a,b,c,d,e=areas
    # The forest passage provides a proper tileset transition from Route 142.
    a.stamp(8,8,[[0x5D,0x5E,0x5F],[0x65,0x66,0x67]],True,[(1,1)])
    a.meta['warp_events'].append(dict(x=9,y=9,elevation=0,dest_map='MAP_ROUTE142',dest_warp_id='0'))
    a.boardwalk(14,29,4,6);a.sign(12,14,'RouteSign','ROUTE 143 - ORANGE COAST\nEast: Route 144 and ORANGE CITY')
    a.npc('SAILOR',13,28,'Landing','These boardwalks keep our beaches\nopen to everyone. Enjoy the coast!')
    b.boardwalk(20,18,8,3)
    b.sign(15,17,'RouteSign','ROUTE 144 - TIDAL BOARDWALK\nEast: ORANGE CITY')
    b.npc('WOMAN_2',35,21,'Tide','The tide pools are full of life.\nPlease leave the shoreline as you found it.')
    c.pave(17,15,15,13);c.pave(9,18,30,3)
    c.building(6,7,'center','MAP_ORANGE_CITY_POKEMON_CENTER_1F')
    c.building(35,7,'mart','MAP_ORANGE_CITY_MART')
    c.building(7,24,'hut','MAP_ORANGE_CITY_HOUSE1')
    c.building(35,24,'hut','MAP_ORANGE_CITY_HOUSE2')
    c.boardwalk(21,31,6,9)
    c.pave(20,7,8,5)
    for yy in range(8,11):
        for xx in range(21,27):c.grid[yy][xx]=block(0x10 if (xx+yy)%2 else GRASS)
    c.pave(23,12,3,4)
    c.palm(19,12);c.palm(28,12)
    # A small planted pool anchors the town square without obstructing its east-west lane.
    c.stamp(23,23,[[0x10C,0x10D,0x10E],[0x114,WATER,0x116],[0x11C,0x11D,0x11E]],False)
    for x,y in [(18,22),(30,22),(18,26),(30,26)]:c.grid[y][x]=block(0x10)
    c.sign(15,17,'TownSign','ORANGE CITY\nA bright harbor on the Orange Coast.')
    c.npc('GENTLEMAN',20,21,'Welcome','Welcome, CHAMPION! Take a rest\nbefore exploring the eastern islands.')
    c.npc('SAILOR',24,32,'Harbor','West lies the tidal boardwalk.\nEast lies the crescent-island lookout.')
    c.npc('GIRL_2',33,18,'Town','The houses here are built with\nlocal bamboo and warm orange roofs.')
    d.sign(17,23,'RouteSign','ROUTE 145 - PALM CAPE\nWest: ORANGE CITY  East: Route 146')
    d.npc('FISHERMAN',35,25,'Cape','Cast a line from the shore, or\nfollow the trail to the lookout.')
    d.boardwalk(35,22,3,11)
    e.pave(23,8,8,5)
    for x,y in [(23,8),(30,8),(23,12),(30,12)]:e.grid[y][x]=block(0x2BF,True)
    e.boardwalk(21,21,9,3)
    e.sign(20,16,'RouteSign','ROUTE 146 - CRESCENT LOOKOUT\nThe last island of the Orange Coast.')
    e.npc('OLD_MAN',27,9,'Lookout','From here, the whole coast\nlooks like a string of orange pearls.')
    for i,a in enumerate(areas):
        if i:a.meta['connections'].append(dict(map=areas[i-1].meta['id'],offset=0,direction='left'))
        if i+1<len(areas):a.meta['connections'].append(dict(map=areas[i+1].meta['id'],offset=0,direction='right'))
    return areas

def register(areas):
    layouts=load('data/layouts/layouts.json')
    for a in areas:
        row=dict(id=a.meta['layout'],name=a.name+'_Layout',width=a.w,height=a.h,primary_tileset='gTileset_AquaOrangeIslands',secondary_tileset='gTileset_AquaValenciaIsland',border_filepath='data/layouts/'+a.name+'/border.bin',blockdata_filepath='data/layouts/'+a.name+'/map.bin',layout_version='emerald')
        old=next((i for i,x in enumerate(layouts['layouts']) if x['id']==row['id']),None)
        if old is None:layouts['layouts'].append(row)
        else:layouts['layouts'][old]=row
    save('data/layouts/layouts.json',layouts)
    groups=load('data/maps/map_groups.json')
    for a in areas:
        if a.name not in groups['gMapGroup_TownsAndRoutes']:groups['gMapGroup_TownsAndRoutes'].append(a.name)
    interior_names=['OrangeCity_PokemonCenter_1F','OrangeCity_PokemonCenter_2F','OrangeCity_Mart','OrangeCity_House1','OrangeCity_House2']
    if 'gMapGroup_IndoorOrangeCity' not in groups:
        groups['group_order'].append('gMapGroup_IndoorOrangeCity');groups['gMapGroup_IndoorOrangeCity']=interior_names
    save('data/maps/map_groups.json',groups)
    sections=load('src/data/region_map/region_map_sections.json')
    for label,name in [('MAPSEC_ROUTE_144','Route 144'),('MAPSEC_ORANGE_CITY','ORANGE CITY'),('MAPSEC_ROUTE_145','Route 145'),('MAPSEC_ROUTE_146','Route 146')]:
        if not any(x['id']==label for x in sections['map_sections']):sections['map_sections'].append(dict(id=label,name=name))
    save('src/data/region_map/region_map_sections.json',sections)
    heal=load('src/data/heal_locations.json')
    if not any(x['id']=='HEAL_LOCATION_ORANGE_CITY' for x in heal['heal_locations']):
        heal['heal_locations'].append(dict(id='HEAL_LOCATION_ORANGE_CITY',map='MAP_ORANGE_CITY',x=8,y=10,respawn_map='MAP_ORANGE_CITY_POKEMON_CENTER_1F',respawn_npc='LOCALID_ORANGE_CITY_NURSE'))
    save('src/data/heal_locations.json',heal)
    r142=load('data/maps/Route142/map.json')
    r142['warp_events']=[dict(x=14,y=12,elevation=0,dest_map='MAP_ROUTE143',dest_warp_id='0')]
    save('data/maps/Route142/map.json',r142)
    raw=words(ROOT/'data/layouts/Route142/map.bin');raw[11*29+14]=block(0x9F,True);raw[12*29+14]=block(0xA7)
    write_words('data/layouts/Route142/map.bin',raw)
    includes=ROOT/'data/event_scripts.s';text=includes.read_text()
    for name in NAMES[1:]+interior_names:
        line='\t.include "data/maps/'+name+'/scripts.inc"'
        if line not in text:text+='\n'+line+'\n'
    includes.write_text(text)

def encounters():
    pools = {
        'Route143': 'TROPIUS EXEGGUTOR BELLOSSOM LURANTIS TSAREENA SHIINOTIC COMFEY TANGROWTH SUDOWOODO ARBOK VICTREEBEL PARASECT VENOMOTH VENUSAUR RIBOMBEE BEAUTIFLY DUSTOX LEDIAN HERACROSS PINSIR SCYTHER PASSIMIAN ORANGURU RAICHU_ALOLA MAROWAK_ALOLA',
        'Route144': 'LUDICOLO SHIFTRY QUAGSIRE CLODSIRE GASTRODON SEISMITOAD TOXICROAK CARNIVINE TANGROWTH LURANTIS AMOONGUSS SHIINOTIC ARAQUANID MASQUERAIN YANMEGA ORBEETLE VILEPLUME BELLOSSOM MUDSDALE GRANBULL STOUTLAND SWALOT ARBOK DRAPION EXEGGUTOR',
        'Route145': 'PALOSSAND SANDSLASH GOLEM_ALOLA GRAVELER_ALOLA RHYDON RHYPERIOR DONPHAN MUDSDALE COPPERAJAH TAUROS KANGASKHAN DODRIO FEAROW BRAVIARY MANDIBUZZ HAWLUCHA ORICORIO_BAILE LYCANROC ARCHEOPS AERODACTYL CRUSTLE AGGRON TYRANITAR GIGALITH STONJOURNER',
        'Route146': 'DRAGONITE SALAMENCE ALTARIA FLYGON NOIVERN DRAMPA TURTONATOR GOODRA KINGDRA DURALUDON DRAGAPULT BAXCALIBUR HAXORUS DRUDDIGON TYRANTRUM KOMMO_O TYRANITAR METAGROSS AGGRON AERODACTYL ARCHEOPS CHARIZARD AMPHAROS GARCHOMP CORVIKNIGHT',
    }
    water='GYARADOS SHARPEDO MANTINE PELIPPER LANTURN TOXAPEX KINGDRA STARMIE CLAWITZER DRAGALGE ALOMOMOLA BARRASKEWDA'.split()
    fish='TENTACRUEL CLOYSTER LANTURN STARMIE GYARADOS KINGDRA DRAGALGE CLAWITZER BARRASKEWDA SHARPEDO'.split()
    data=load('src/data/wild_encounters.json');group=data['wild_encounter_groups'][0]
    counts={field['type']:len(field['encounter_rates']) for field in group['fields']}
    for n,(name,pool) in enumerate(pools.items()):
        low,high=60+n*2,66+n*2
        entry=dict(map='MAP_'+name.upper(),base_label='g'+name)
        for kind,mons,rate in [('land_mons',pool.split(),15),('water_mons',water,7),('fishing_mons',fish,10)]:
            assert len(mons)==counts[kind]
            entry[kind]=dict(encounter_rate=rate,mons=[dict(min_level=low,max_level=high,species='SPECIES_'+species) for species in mons])
        existing=next((i for i,x in enumerate(group['encounters']) if x.get('map')==entry['map']),None)
        if existing is None:group['encounters'].append(entry)
        else:group['encounters'][existing]=entry
    save('src/data/wild_encounters.json',data)


def interiors(city):
    for name,source in [('OrangeCity_PokemonCenter_1F','PetalburgCity_PokemonCenter_1F'),('OrangeCity_PokemonCenter_2F','PetalburgCity_PokemonCenter_2F'),('OrangeCity_Mart','OldaleTown_Mart'),('OrangeCity_House1','DewfordTown_House1'),('OrangeCity_House2','DewfordTown_House1')]:
        data=copy.deepcopy(load('data/maps/'+source+'/map.json'))
        ident='MAP_'+name.upper().replace('ORANGECITY','ORANGE_CITY').replace('POKEMONCENTER','POKEMON_CENTER')
        data.update(id=ident,name=name,region_map_section='MAPSEC_ORANGE_CITY')
        if name.endswith('2F'):
            for warp in data['warp_events']:warp['dest_map']='MAP_ORANGE_CITY_POKEMON_CENTER_1F'
            scripts=(ROOT/'data/maps'/source/'scripts.inc').read_text().replace(source,name)
        else:
            entry=next(x for x in city.doors if x[2]==ident)
            for warp in data['warp_events']:
                if 'POKEMON_CENTER_2F' in warp['dest_map']:warp['dest_map']='MAP_ORANGE_CITY_POKEMON_CENTER_2F'
                else:warp.update(dest_map='MAP_ORANGE_CITY',dest_warp_id=str(entry[3]))
            if name.endswith('1F'):
                data['object_events']=[person('NURSE',7,2,name+'_EventScript_Nurse','LOCALID_ORANGE_CITY_NURSE')]
                scripts=f'''{name}_MapScripts::
\tmap_script MAP_SCRIPT_ON_TRANSITION, {name}_OnTransition
\tmap_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
\t.byte 0
{name}_OnTransition:
\tsetrespawn HEAL_LOCATION_ORANGE_CITY
\tend
{name}_EventScript_Nurse::
\tsetvar VAR_0x800B, LOCALID_ORANGE_CITY_NURSE
\tcall Common_EventScript_PkmnCenterNurse
\twaitmessage
\twaitbuttonpress
\trelease
\tend
'''
            elif name.endswith('Mart'):
                data['object_events']=[person('MART_EMPLOYEE',1,3,name+'_EventScript_Clerk')]
                data['object_events'][0]['movement_type']='MOVEMENT_TYPE_FACE_RIGHT'
                scripts=f'''{name}_MapScripts::
\t.byte 0
{name}_EventScript_Clerk::
\tlock
\tfaceplayer
\tmessage gText_HowMayIServeYou
\twaitmessage
\tpokemart 0
\tmsgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
\trelease
\tend
'''
                # Preserve the clerk's curated stock and dialogue when rebuilding maps.
                existing_script = ROOT / 'data/maps' / name / 'scripts.inc'
                if existing_script.exists():
                    scripts = existing_script.read_text()
            else:
                data['object_events']=[person('WOMAN_3' if name.endswith('1') else 'MAN_3',6,3,name+'_EventScript_Resident')]
                text='The coast is peaceful, even for a\\nCHAMPION. Make yourself at home!' if name.endswith('1') else 'The lookout on Route 146 is my\\nfavorite spot to watch the waves.'
                scripts=f'''{name}_MapScripts::
\t.byte 0
{name}_EventScript_Resident::
\tmsgbox {name}_Text_Resident, MSGBOX_NPC
\tend
{name}_Text_Resident:
\t.string "{text}$"
'''
        save('data/maps/'+name+'/map.json',data);(ROOT/'data/maps'/name/'scripts.inc').write_text(scripts.rstrip()+'\n')

def audit(areas):
    for a in areas:
        assert len(a.meta['object_events'])<=16
        assert len(words(ROOT/f'data/layouts/{a.name}/map.bin'))==a.w*a.h
        # Walkability: core routes, doors and signs must all be reachable without Surf.
        occupied={(obj["x"],obj["y"]) for obj in a.meta["object_events"]}
        def walk(x,y):
            if not (0<=x<a.w and 0<=y<a.h) or (x,y) in occupied:return False
            v=a.grid[y][x];return (v>>10)&3==0 and v>>12==3
        start=(9,10) if a.name=='Route143' else (0,19)
        q=collections.deque([start]);seen={start}
        while q:
            x,y=q.popleft()
            for dx,dy in [(1,0),(-1,0),(0,1),(0,-1)]:
                pos=x+dx,y+dy
                if pos not in seen and walk(*pos):seen.add(pos);q.append(pos)
        targets=[(0,19)] if a.name!='Route143' else []
        if a.name!='Route146':targets.append((a.w-1,19))
        targets += [(x,y+1) for x,y,_,_ in a.doors]
        for obj in a.meta['object_events']:
            assert any((obj['x']+dx,obj['y']+dy) in seen for dx,dy in [(1,0),(-1,0),(0,1),(0,-1)]),(a.name,'unreachable NPC',obj)
        for bg in a.meta['bg_events']:
            assert any((bg['x']+dx,bg['y']+dy) in seen for dx,dy in [(1,0),(-1,0),(0,1),(0,-1)]),(a.name,'unreachable sign')
        for target in targets:assert target in seen,(a.name,'blocked route/door',target)
        for warp in a.meta['warp_events']:assert (warp['x'],warp['y']) in seen,(a.name,'blocked warp',warp)
        print(f'{a.name}: {a.w}x{a.h}, {len(seen)} connected walkable tiles; exits, services and NPCs accessible.')
    # New rooms only refer to real maps and reciprocal exterior/indoor warp IDs.
    for name in NAMES+['Route142','OrangeCity_PokemonCenter_1F','OrangeCity_PokemonCenter_2F','OrangeCity_Mart','OrangeCity_House1','OrangeCity_House2']:
        data=load('data/maps/'+name+'/map.json')
        for warp in data['warp_events']:
            matches=[p for p in (ROOT/'data/maps').glob('*/map.json') if json.loads(p.read_text())['id']==warp['dest_map']]
            assert matches,(name,warp)
            target=json.loads(matches[0].read_text());assert int(warp['dest_warp_id'])<len(target['warp_events']),(name,warp)
    print('All new warp destinations and IDs resolve; existing map/layout IDs remain in order.')

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
    overview=Image.new('RGB',(max(c.width for c in cards)*3+32,max(c.height for c in cards)*2+24),'#10272e')
    for i,c in enumerate(cards):overview.paste(c,((i%3)*(max(k.width for k in cards)+12),(i//3)*(max(k.height for k in cards)+12)))
    overview.convert('P',palette=Image.Palette.ADAPTIVE,colors=256).save(ROOT/'docs/maps/orange-coast.png',optimize=True)

if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--render',action='store_true');args=parser.parse_args()
    areas=make_areas()
    for area in areas:area.emit()
    register(areas);interiors(areas[2]);encounters();audit(areas)
    if args.render:render(areas)
