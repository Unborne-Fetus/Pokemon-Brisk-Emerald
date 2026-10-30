#!/usr/bin/env python3
"""Import Mikitari's supplied sprites and author repeatable Brisk definitions.

python tools/brisk/import_mikitari.py --assets '/path/to/Fakemon Pack'
Without --assets, regenerates definitions/encounters using the checked-in report.
Artwork is Mikitari's. Gameplay is Brisk-authored, not creator-supplied canon.
"""
from collections import Counter, defaultdict
from pathlib import Path
import argparse
import hashlib
import json
import re
from PIL import Image, ImageDraw, ImageOps
from import_phoenix_fakemon import gba, distance, indexed, pack80, palette
from import_sacred_phoenix import quantize_pairs, save_pal

ROOT = Path(__file__).resolve().parents[2]
ROWS = json.loads((ROOT/'tools/brisk/mikitari_species.json').read_text())['species']
EXISTING = {'BASCULIN','BUNEARY','CASTFORM','FALINKS','LOPUNNY','NOCTOWL','ORICORIO','SPRITZEE','YAMASK'}
TYPE_MOVES = {
 'NORMAL': ['TACKLE','QUICK_ATTACK','HEADBUTT','BODY_SLAM','DOUBLE_EDGE'],
 'FIRE': ['EMBER','FLAME_CHARGE','FIRE_FANG','FLAMETHROWER','FLARE_BLITZ'],
 'WATER': ['WATER_GUN','AQUA_JET','WATER_PULSE','SURF','LIQUIDATION'],
 'GRASS': ['ABSORB','RAZOR_LEAF','MEGA_DRAIN','GIGA_DRAIN','SEED_BOMB'],
 'ELECTRIC': ['THUNDER_SHOCK','SPARK','SHOCK_WAVE','THUNDERBOLT','WILD_CHARGE'],
 'ICE': ['POWDER_SNOW','ICE_SHARD','ICY_WIND','ICE_BEAM','ICICLE_CRASH'],
 'FIGHTING': ['ROCK_SMASH','MACH_PUNCH','BRICK_BREAK','DRAIN_PUNCH','CLOSE_COMBAT'],
 'POISON': ['POISON_STING','ACID','POISON_FANG','SLUDGE_BOMB','POISON_JAB'],
 'GROUND': ['MUD_SLAP','BULLDOZE','DIG','EARTH_POWER','EARTHQUAKE'],
 'FLYING': ['PECK','GUST','WING_ATTACK','AIR_SLASH','DRILL_PECK'],
 'PSYCHIC': ['CONFUSION','STORED_POWER','PSYBEAM','PSYCHIC','PSYSHOCK'],
 'BUG': ['STRUGGLE_BUG','FURY_CUTTER','BUG_BITE','BUG_BUZZ','X_SCISSOR'],
 'ROCK': ['ROCK_THROW','ROLLOUT','ROCK_TOMB','POWER_GEM','ROCK_SLIDE'],
 'GHOST': ['ASTONISH','SHADOW_SNEAK','HEX','SHADOW_BALL','SHADOW_CLAW'],
 'DRAGON': ['TWISTER','BREAKING_SWIPE','DRAGON_BREATH','DRAGON_PULSE','DRAGON_CLAW'],
 'DARK': ['BITE','PAYBACK','ASSURANCE','DARK_PULSE','CRUNCH'],
 'STEEL': ['METAL_CLAW','BULLET_PUNCH','METAL_SOUND','FLASH_CANNON','IRON_HEAD'],
 'FAIRY': ['FAIRY_WIND','BABY_DOLL_EYES','DRAINING_KISS','DAZZLING_GLEAM','PLAY_ROUGH'],
}
ALIASES = {('Icons shiny','CRISHY'):'CRUSHY',
 ('Icons','OCTOKINEPUS'):'OCTOKINEPIS',('Icons','SPRITZEE_1'):'SPRITZEE'}


def asset_path(source, directory, name):
    path = source/directory/(ALIASES.get((directory,name),name)+'.png')
    if not path.is_file():
        raise ValueError(f'Missing supplied art: {path}')
    return path


def native_pair(normal, shiny):
    """Undo the pack's 2x pixel enlargement; preserve relative front/back size."""
    pair=[]
    for im in (normal,shiny):
        factor = 2 if max(im.size) > 100 else 1
        pair.append(im.resize((round(im.width/factor),round(im.height/factor)),Image.Resampling.NEAREST))
    size=(max(im.width for im in pair),max(im.height for im in pair))
    aligned=[]
    for im in pair:
        canvas=Image.new('RGBA',size);canvas.paste(im,((size[0]-im.width)//2,size[1]-im.height));aligned.append(canvas)
    pair=aligned
    mask = Image.new('L',pair[0].size)
    mask.putdata([max(a[3],b[3]) for a,b in zip(*[im.getdata() for im in pair])])
    box = mask.getbbox(); assert box
    w,h = box[2]-box[0],box[3]-box[1]
    scale = min(1,80/w,80/h)
    output=[]
    for im in pair:
        im=im.crop(box)
        if scale<1: im=im.resize((max(1,round(w*scale)),max(1,round(h*scale))),Image.Resampling.NEAREST)
        canvas=Image.new('RGBA',(80,80));canvas.paste(im,((80-im.width)//2,80-im.height));output.append(canvas)
    return output


def import_assets(source):
    report={}; cards=[]
    iconpals=[palette(ROOT/f'graphics/pokemon/icon_palettes/pal{i}.pal') for i in range(6)]
    for row in ROWS:
        key=row['id']; folder=ROOT/'graphics/pokemon/mikitari'/key.lower();folder.mkdir(parents=True,exist_ok=True)
        sources=[(row['source'],'')]+([(row['female_source'],'Female')] if 'female_source' in row else [])
        rec={'source':row['source'],'files':{},'gender_art':bool(len(sources)>1)}
        for source_name,sex in sources:
            suffix='_female' if sex else ''
            pairs=[]
            for label in ('Front','Back'):
                paths=[asset_path(source,d,source_name) for d in (label,label+' shiny')]
                for p in paths:rec['files'][str(p.relative_to(source))]=hashlib.sha256(p.read_bytes()).hexdigest()
                pairs.append(native_pair(*[Image.open(p).convert('RGBA') for p in paths]))
            battle,pals,count=quantize_pairs(pairs)
            native80 = len(sources)==1 and any(max(im.getbbox()[2]-im.getbbox()[0], im.getbbox()[3]-im.getbbox()[1])>64 for im in battle)
            rec['native_size']=80 if native80 else 64
            for pic,label in zip(battle,['front','back']):
                pic.save(folder/(label+suffix+'_80.png'));pack80(pic).save(folder/(label+suffix+'_tiles.png'))
                if native80:
                    menu=pic.resize((64,64),Image.Resampling.NEAREST)
                else:
                    cropped=pic.crop(pic.getbbox())
                    if max(cropped.size)>64:
                        scale=64/max(cropped.size)
                        cropped=cropped.resize((round(cropped.width*scale),round(cropped.height*scale)),Image.Resampling.NEAREST)
                    menu=indexed([0]*4096,(64,64),pals[0])
                    menu.paste(cropped,((64-cropped.width)//2,64-cropped.height))
                menu.save(folder/(label+suffix+'.png'))
            for label,pal in zip(['normal','shiny'],pals):save_pal(folder/(label+suffix+'.pal'),pal)
            # Source icons are two horizontal frames enlarged 2x (some have a
            # one-pixel cropped border). Pad to 128x64 before reversing scale.
            ip=asset_path(source,'Icons',source_name);si=asset_path(source,'Icons shiny',source_name)
            for p in (ip,si):rec['files'][str(p.relative_to(source))]=hashlib.sha256(p.read_bytes()).hexdigest()
            raw=Image.open(ip).convert('RGBA');can=Image.new('RGBA',(128,64));can.paste(raw,(0,0));raw=can.resize((64,32),Image.Resampling.NEAREST)
            icons=[raw.crop((x,0,x+32,32)) for x in (0,32)]
            colors=Counter(gba(p) for im in icons for p in im.getdata() if p[3])
            palid=min(range(6),key=lambda j:sum(c*min(distance(rgb,k) for k in iconpals[j][1:]) for rgb,c in colors.items()))
            data=[0 if not p[3] else 1+min(range(15),key=lambda j:distance(gba(p),iconpals[palid][j+1])) for im in icons for p in im.getdata()]
            indexed(data,(32,64),iconpals[palid]).save(folder/('icon'+suffix+'.png'))
            rec['icon_palette'+suffix]=palid;rec['palette_pairs'+suffix]=count
            # The pack has no walking sheets. Use its front/back views as a
            # safe static follower fallback; never use unrelated Pokemon art.
            front,back=[p[0].resize((32,32),Image.Resampling.NEAREST) for p in pairs]
            directions=[front,back,ImageOps.mirror(front),front]
            shinyfront,shinyback=[p[1].resize((32,32),Image.Resampling.NEAREST) for p in pairs]
            shinydirections=[shinyfront,shinyback,ImageOps.mirror(shinyfront),shinyfront]
            frames,pals,_=quantize_pairs([(a,b) for a,b in zip(directions,shinydirections) for _ in range(2)])
            indexed([v for im in frames for v in im.getdata()],(32,256),pals[0]).save(folder/('overworld'+suffix+'.png'))
            for label,pal in zip(['normal','shiny'],pals):save_pal(folder/('overworld_'+label+suffix+'.pal'),pal)
        report[key]=rec
        card=Image.new('RGBA',(176,105),'#c6d5cd');ImageDraw.Draw(card).text((3,2),key,fill='#152521')
        pic=Image.open(folder/'front_80.png').convert('RGBA');card.alpha_composite(pic,(3,22))
        pic=Image.open(folder/'front_80.png');pic.putpalette([c for rgb in palette(folder/'shiny.pal') for c in rgb]+[0]*720);card.alpha_composite(pic.convert('RGBA'),(90,22));cards.append(card)
        # These are import intermediates, not additional source assets.
        for suffix in ('','_female'):
            for label in ('front','back'):
                (folder/(label+suffix+'_80.png')).unlink(missing_ok=True)
                if suffix or rec['native_size']==64:
                    (folder/(label+suffix+'_tiles.png')).unlink(missing_ok=True)
    (ROOT/'tools/brisk/mikitari_sprite_import.json').write_text(json.dumps(report,indent=2)+'\n')
    preview=Image.new('RGBA',(176*6,105*((len(cards)+5)//6)),'#c6d5cd')
    for i,card in enumerate(cards):preview.paste(card,((i%6)*176,(i//6)*105))
    p=ROOT/'docs/mikitari';p.mkdir(exist_ok=True);preview.convert('RGB').save(p/'sprites.png')
    print(f'Imported {len(ROWS)} species/forms from 120 complete sprite sets.')


def moves(row):
    types=row['types'];primary=TYPE_MOVES[types[0]];secondary=TYPE_MOVES[types[-1]]
    # Teach both STAB categories. Four final natural moves always include
    # meaningful attacks, even on newly caught level-90 babies.
    pool=['TACKLE',primary[0],secondary[0],'GROWL',primary[1],secondary[1],primary[2],secondary[2],'PROTECT','REST']
    pool+=['DRAIN_PUNCH','THUNDER_PUNCH','ICE_PUNCH','FIRE_PUNCH'] if 'IRON_FIST' in row['abilities'] or 'SPEED_BOXER' in row['abilities'] else []
    pool+=['WATER_PULSE','DRAGON_PULSE','AURA_SPHERE'] if 'MEGA_LAUNCHER' in row['abilities'] else []
    special=row['stats'][4]>row['stats'][1]
    pool += [primary[3],secondary[3],primary[4],secondary[4]] if not special else [primary[4],secondary[4],primary[3],secondary[3]]
    # Keep last occurrences of duplicate moves, in authored progression order.
    pool=[m for i,m in enumerate(pool) if m not in pool[i+1:]]
    levels=[1,1]+[5+4*i for i in range(len(pool)-2)]
    return list(zip(levels,pool))


def write_definitions():
    report=json.loads((ROOT/'tools/brisk/mikitari_sprite_import.json').read_text())
    gfx=['// Artwork by Mikitari; generated by tools/brisk/import_mikitari.py.'];pics=[];learn=[];species=[];forms=[]
    groups=defaultdict(list)
    for r in ROWS:groups[r['dex']].append(r['id'])
    form_names={}
    official_forms=(ROOT/'src/data/pokemon/form_species_tables.h').read_text()
    for dex,ids in groups.items():
        if dex in EXISTING:
            official='s'+dex.title()+'FormSpeciesIdTable'
            if 'static const u16 '+official+'[]' in official_forms:
                form_names[dex]=official
                continue
            form_names[dex]='sMikitari'+dex.title()+'Forms'
        elif len(ids)>1:form_names[dex]='sMikitari'+dex.title()+'Forms'
        if dex in form_names:
            table=form_names[dex];members=([dex] if dex in EXISTING else [])+ids
            forms.append(f'static const u16 {table}[] = {{ '+', '.join('SPECIES_'+x for x in members)+', FORM_SPECIES_END };')
    for row in ROWS:
        key=row['id'];name='Mikitari'+key.title().replace('_','');rec=report[key];base='graphics/pokemon/mikitari/'+key.lower()
        for sex in ('','Female') if 'female_source' in row else ('',):
            suffix='_female' if sex else '';symbol=name+sex
            if sex:gfx.append('#if P_GENDER_DIFFERENCES')
            for label,ctype,ext,asset in [('FrontPic','u32','.4bpp.smol','front'),('BackPic','u32','.4bpp.smol','back'),('Palette','u16','.gbapal','normal'),('ShinyPalette','u16','.gbapal','shiny'),('Icon','u8','.4bpp','icon')]:
                filename=asset+suffix+('.pal' if ext=='.gbapal' else '.png')
                gfx.append(f'const {ctype} gMon{label}_{symbol}[] = INCGFX_{ctype.upper()}("{base}/{filename}", "{ext}");')
            if not sex and rec['native_size']==80:
                for label in ('Front','Back'):gfx.append(f'const u8 gMon{label}Pic80_{symbol}[] = INCGFX_U8("{base}/{label.lower()}_tiles.png", ".4bpp");')
            gfx+=['#if OW_POKEMON_OBJECT_EVENTS',f'const u32 gObjectEventPic_{symbol}[] = INCGFX_COMP("{base}/overworld{suffix}.png", ".4bpp", "-mwidth 4 -mheight 4");','#if OW_PKMN_OBJECTS_SHARE_PALETTES == FALSE',f'const u16 gOverworldPalette_{symbol}[] = INCGFX_U16("{base}/overworld_normal{suffix}.pal", ".gbapal");',f'const u16 gShinyOverworldPalette_{symbol}[] = INCGFX_U16("{base}/overworld_shiny{suffix}.pal", ".gbapal");','#endif','#endif']
            pics+=['#if OW_POKEMON_OBJECT_EVENTS']+(['#if P_GENDER_DIFFERENCES'] if sex else [])+[f'static const struct SpriteFrameImage sPicTable_{symbol}[] = {{ overworld_ascending_frames(gObjectEventPic_{symbol}, 4, 4), }};']+(['#endif'] if sex else [])+['#endif']
            if sex:gfx.append('#endif')
        lm=moves(row);tm=sorted({m for _,m in lm}|{'PROTECT','REST','SLEEP_TALK','SUBSTITUTE','FACADE','TERA_BLAST','HELPING_HAND'})
        learn += [f'static const struct LevelUpMove s{name}LevelUpLearnset[] = {{']+[f'    LEVEL_UP_MOVE({lv}, MOVE_{m}),' for lv,m in lm]+['    LEVEL_UP_END','};',f'static const u16 s{name}TeachableLearnset[] = {{']+[f'    MOVE_{m},' for m in tm]+['    MOVE_UNAVAILABLE','};']
        st=row['stats'];dex=row['dex'];role=row['role'];color='BLUE' if 'WATER' in row['types'] else 'GREEN' if 'GRASS' in row['types'] or 'BUG' in row['types'] else 'RED' if 'FIRE' in row['types'] else 'GRAY'
        category={'forest':'Woodland','meadow':'Meadow','cave':'Cavern','mountain':'Highland','river':'River','coast':'Coastal','ice':'Frost','ghost':'Spectral','electric':'Charged','fire':'Ember'}[row['habitat']]
        height=5 if role=='baby' else 11 if role=='middle' else 18;weight=60 if role=='baby' else 240 if role=='middle' else 850
        row['height']=height;row['weight']=weight
        text=f'It makes its home in Kanto.\\nResearchers are still studying\\nits habits and unusual traits.'
        species.append(f'''    [SPECIES_{key}] =
    {{
        .baseHP = {st[0]}, .baseAttack = {st[1]}, .baseDefense = {st[2]},
        .baseSpeed = {st[3]}, .baseSpAttack = {st[4]}, .baseSpDefense = {st[5]},
        .types = MON_TYPES({', '.join('TYPE_'+t for t in row['types'])}),
        .catchRate = {150 if role=='baby' else 90 if role=='middle' else 45}, .expYield = {60 if role=='baby' else 140 if role=='middle' else 230},
        .evYield_{'Attack' if st[1]>st[4] else 'SpAttack'} = {1 if role=='baby' else 2},
        .genderRatio = PERCENT_FEMALE(50), .eggCycles = 20, .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_{'BUG' if 'BUG' in row['types'] else 'WATER_1' if 'WATER' in row['types'] else 'GRASS' if 'GRASS' in row['types'] else 'FIELD'}),
        .abilities = {{ {', '.join('ABILITY_'+a for a in row['abilities'])} }},
        .bodyColor = BODY_COLOR_{color},
        .speciesName = _("{row['name']}"), .cryId = CRY_{'BASCULIN' if 'WATER' in row['types'] else 'SCYTHER' if 'BUG' in row['types'] else 'EEVEE'},
        .natDexNum = NATIONAL_DEX_{dex}, .categoryName = _("{category}"),
        .height = {height}, .weight = {weight},
        .description = COMPOUND_STRING("{text}"),
        .pokemonScale = 256, .trainerScale = 256,
        .frontPic = gMonFrontPic_{name}, .backPic = gMonBackPic_{name},
        .frontPicSize = MON_COORDS_SIZE(64, 64), .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0, .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder, .backAnimId = BACK_ANIM_NONE,
        .palette = gMonPalette_{name}, .shinyPalette = gMonShinyPalette_{name},
        .iconSprite = gMonIcon_{name}, .iconPalIndex = {rec['icon_palette']},
        .pokemonJumpType = PKMN_JUMP_TYPE_NORMAL,
        SHADOW(0, 0, SHADOW_SIZE_M)
        FOOTPRINT(QuestionMark)
        OVERWORLD(sPicTable_{name}, SIZE_32x32, SHADOW_SIZE_M, TRACKS_FOOT, sAnimTable_Following_Asym,
            gOverworldPalette_{name}, gShinyOverworldPalette_{name})
        .levelUpLearnset = s{name}LevelUpLearnset,
        .teachableLearnset = s{name}TeachableLearnset,
        .eggMoveLearnset = sNoneEggMoveLearnset,''')
        if rec['native_size']==80:
            species.append(f'        .frontPic80 = gMonFrontPic80_{name}, .backPic80 = gMonBackPic80_{name},')
        if dex in form_names:species.append(f'        .formSpeciesIdTable = {form_names[dex]},')
        if 'female_source' in row:
            species+=['#if P_GENDER_DIFFERENCES',f'        .frontPicFemale = gMonFrontPic_{name}Female, .backPicFemale = gMonBackPic_{name}Female,',f'        .frontPicSizeFemale = MON_COORDS_SIZE(64, 64), .backPicSizeFemale = MON_COORDS_SIZE(64, 64),',f'        .paletteFemale = gMonPalette_{name}Female, .shinyPaletteFemale = gMonShinyPalette_{name}Female,',f'        .iconSpriteFemale = gMonIcon_{name}Female, .iconPalIndexFemale = {rec["icon_palette_female"]},',f'        OVERWORLD_FEMALE(sPicTable_{name}Female, SIZE_32x32, SHADOW_SIZE_M, TRACKS_FOOT, sAnimTable_Following_Asym, gOverworldPalette_{name}Female, gShinyOverworldPalette_{name}Female)','#endif']
        if row['evolutions']:
            ev=[]
            for e in row['evolutions']:
                cond=', CONDITIONS({IF_GENDER, MON_'+e['gender']+'})' if e.get('gender') else ''
                ev.append(f'{{EVO_LEVEL, {e["level"]}, SPECIES_{e["target"]}{cond}}}')
            species.append('        .evolutions = EVOLUTION('+', '.join(ev)+'),')
        species.append('    },')
    for path,lines in [('src/data/graphics/mikitari.h',gfx),('src/data/pokemon/mikitari_pic_tables.h',pics),('src/data/pokemon/mikitari_learnsets.h',learn),('src/data/pokemon/mikitari_forms.h',forms),('src/data/pokemon/species_info/mikitari.h',species)]:
        (ROOT/path).write_text('// Generated from tools/brisk/mikitari_species.json. Artwork: Mikitari.\n'+'\n'.join(lines)+'\n')
    return ROWS


def encounters():
    path=ROOT/'data/brisk_kanto_encounters.json';config=json.loads(path.read_text());overrides=config['overrides']
    # Remove only this pack's earlier placements; preserve other custom packs.
    ids={'SPECIES_'+r['id'] for r in ROWS}
    for tables in overrides.values():
        for changes in tables.values():
            for slot in list(changes):
                if changes[slot] in ids:del changes[slot]
    habitats={
      'forest':['VIRIDIAN_FOREST','ROUTE2','ROUTE12','ROUTE14','ROUTE21_NORTH','ROUTE21_SOUTH'],
      'meadow':['ROUTE1','ROUTE5','ROUTE7','ROUTE11','ROUTE13','ROUTE15','ROUTE24','ROUTE25'],
      'mountain':['ROUTE3','ROUTE4','ROUTE9','ROUTE16','ROUTE17','ROUTE18','ROUTE22','ROUTE23'],
      'cave':['MT_MOON_1F','MT_MOON_B1F','MT_MOON_B2F','DIGLETTS_CAVE_B1F','ROCK_TUNNEL_1F','ROCK_TUNNEL_B1F'],
      'ice':['SEAFOAM_ISLANDS_1F','SEAFOAM_ISLANDS_B1F','SEAFOAM_ISLANDS_B2F','SEAFOAM_ISLANDS_B3F','SEAFOAM_ISLANDS_B4F'],
      'fire':['POKEMON_MANSION_1F','POKEMON_MANSION_2F','POKEMON_MANSION_3F','POKEMON_MANSION_B1F'],
      'ghost':['ROUTE8','POKEMON_TOWER_3F','POKEMON_TOWER_4F','POKEMON_TOWER_5F','POKEMON_TOWER_6F','POKEMON_TOWER_7F'],
      'electric':['POWER_PLANT','ROUTE10','ROUTE7','ROUTE18'],
      'river':['ROUTE6','ROUTE10','ROUTE11','ROUTE12','ROUTE22','ROUTE24','ROUTE25','CERULEAN_CITY'],
      'coast':['ROUTE19','ROUTE20','ROUTE21_NORTH','ROUTE21_SOUTH','CINNABAR_ISLAND','VERMILION_CITY','PALLET_TOWN'],
    }
    counts=Counter();placements=[]
    # Evolved species occupy more common slots; babies stay in lower-rate slots.
    for row in ROWS:
        habitat=row['habitat'];options=habitats[habitat];field='water_mons' if habitat in ('river','coast') else 'land_mons'
        available=[m for m in options if len(overrides.get('MAP_'+m,{}).get(field,{})) < (6 if field=='water_mons' else 8)]
        assert available,(row['id'],'habitat full')
        m=min(available,key=lambda m:counts[(habitat,m)]);mapid='MAP_'+m
        changes=overrides.setdefault(mapid,{}).setdefault(field,{})
        preferred=([7,6,5,4,3,2,1,0] if row['role']=='baby' else [0,1,2,3,4,5,6,7]) if field=='water_mons' else ([19,18,17,16,15,14,13,12] if row['role']=='baby' else [12,13,14,15,16,17,18,19])
        slot=next(s for s in preferred if str(s) not in changes)
        changes[str(slot)]='SPECIES_'+row['id'];counts[(habitat,m)]+=1
        placements.append(dict(species=row['id'],map=mapid,table=field,slot=slot))
    config['overrides']=overrides;path.write_text(json.dumps(config,indent=2)+'\n')
    from author_kanto_encounters import generate
    generate()
    (ROOT/'data/mikitari_kanto_placements.json').write_text(json.dumps(placements,indent=2)+'\n')
    doc=['# Mikitari Fakémon pack','', '**Artwork and designs: Mikitari.**','',
         'Imported from the user-supplied `Fakemon Pack.7z`: 120 normal/shiny front, back and icon sets. Gender variants share a species; color/terrain variants share a National Dex entry. Existing species and Dex numbers are preserved.','',
         'The archive contains no gameplay specification. Types, stats, moves, evolution links, category labels and habitat assignments are Brisk-authored adaptations, not claims about Mikitari’s original canon. Final stages generally have 490–535 base-stat totals; early stages remain weaker.','',
         'No walking sheets were supplied. Followers use static directional fallbacks derived from the supplied front/back art. Native menu icons use the nearest supported icon palette; shiny battle and follower palettes use the supplied shiny art.','',
         'All 114 species/form entries are directly obtainable in mainland Kanto at its existing postgame levels. Gender art is selected by gender. Forest, meadow, mountain, cave, ice, fire, spectral, electrical, river and coastal habitats have distinct placements. Evolution is also available for the authored families.','',
         'The last four active land slots and last two active Surf/fishing slots remain available for future packs. They contain real Pokémon, never empty encounter rolls.','',
         '## Locations','', '| Pokémon / form | Type | BST | Location | Method |','| --- | --- | --- | --- | --- |']
    byid={r['id']:r for r in ROWS}
    for p in placements:
        r=byid[p['species']];doc.append(f'| {r["id"]} | {" / ".join(r["types"])} | {sum(r["stats"])} | {p["map"].removeprefix("MAP_")} | {"Surf" if p["table"]=="water_mons" else "Walking"} |')
    doc+=['','## Regeneration','','Run `python tools/brisk/import_mikitari.py --assets "/path/to/Fakemon Pack"` to reimport artwork, or omit `--assets` to regenerate definitions and encounters from the checked-in import report. Source file hashes are in `tools/brisk/mikitari_sprite_import.json`.','']
    (ROOT/'docs/mikitari_fakemon.md').write_text('\n'.join(doc))
    print(f'Placed {len(placements)} species/forms across {len({p["map"] for p in placements})} Kanto maps; future slots untouched.')


def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--assets',type=Path);args=ap.parse_args()
    if args.assets:import_assets(args.assets)
    write_definitions();encounters()

if __name__=='__main__':main()
