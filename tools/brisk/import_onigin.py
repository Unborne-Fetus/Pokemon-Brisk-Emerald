#!/usr/bin/env python3
"""Import the nine front/back pairs in onigin_pixelart.png.

Artwork: onigin_pixelart, credited from the supplied filename.
Names, gameplay, shiny palettes and reduced icon/follower fallbacks are Brisk
adaptations. No replacement art is generated. Run from any directory.
"""
from pathlib import Path
from collections import Counter
import colorsys
import hashlib
import json
import re
from PIL import Image, ImageOps
from import_phoenix_fakemon import gba, distance, indexed, palette
from import_sacred_phoenix import quantize_pairs, save_pal
from import_mikitari import moves
from author_kanto_encounters import generate

ROOT=Path(__file__).resolve().parents[2]
META=json.loads((ROOT/'tools/brisk/onigin_species.json').read_text())
ROWS=META['species']


def shiny(image, shift):
    data=[]
    for red,green,blue,alpha in image.getdata():
        h,s,v=colorsys.rgb_to_hsv(red/255,green/255,blue/255)
        rgb=colorsys.hsv_to_rgb((h+shift)%1,s,v) if s>.15 else (red/255,green/255,blue/255)
        data.append((*[round(c*255) for c in rgb],alpha))
    result=Image.new('RGBA',image.size);result.putdata(data);return result


def main():
    source=ROOT/META['source_file'];sheet=Image.open(source).convert('RGBA')
    assert sheet.size==(512,192)
    iconpals=[palette(ROOT/f'graphics/pokemon/icon_palettes/pal{i}.pal') for i in range(6)]
    graphics=['// Artwork: onigin_pixelart. Gameplay and palette adaptation: Brisk.'];pics=[];learn=[];species=[];report={}
    for row in ROWS:
        name=row['name'];key=row['id'];symbol='Onigin'+name
        folder=ROOT/'graphics/pokemon/onigin_pixelart'/key.lower();folder.mkdir(parents=True,exist_ok=True)
        normal=[sheet.crop(tuple(row[label])) for label in ('front','back')]
        assert all(im.size==(64,64) and im.getbbox() for im in normal)
        pairs=[(im,shiny(im,row['shiny_hue_shift'])) for im in normal]
        battle,pals,count=quantize_pairs(pairs)
        for pic,label in zip(battle,['front','back']):pic.save(folder/(label+'.png'))
        for label,pal in zip(['normal','shiny'],pals):save_pal(folder/(label+'.pal'),pal)
        # No menu icons or walking sheets were supplied. Reduce the creator's
        # existing views to safe native frames rather than borrowing other art.
        icon=normal[0].resize((32,32),Image.Resampling.NEAREST)
        colors=Counter(gba(p) for p in icon.getdata() if p[3])
        palid=min(range(6),key=lambda j:sum(c*min(distance(rgb,k) for k in iconpals[j][1:]) for rgb,c in colors.items()))
        data=[0 if not p[3] else 1+min(range(15),key=lambda j:distance(gba(p),iconpals[palid][j+1])) for p in icon.getdata()]
        indexed(data+data,(32,64),iconpals[palid]).save(folder/'icon.png')
        front,back=[im.resize((32,32),Image.Resampling.NEAREST) for im in battle]
        frames=[front,front,back,back,ImageOps.mirror(front),ImageOps.mirror(front),front,front]
        indexed([v for im in frames for v in im.getdata()],(32,256),pals[0]).save(folder/'overworld.png')
        base='graphics/pokemon/onigin_pixelart/'+key.lower()
        for label,ctype,ext,asset in [('FrontPic','u32','.4bpp.smol','front.png'),('BackPic','u32','.4bpp.smol','back.png'),('Palette','u16','.gbapal','normal.pal'),('ShinyPalette','u16','.gbapal','shiny.pal'),('Icon','u8','.4bpp','icon.png')]:
            graphics.append(f'const {ctype} gMon{label}_{symbol}[] = INCGFX_{ctype.upper()}("{base}/{asset}", "{ext}");')
        graphics+=['#if OW_POKEMON_OBJECT_EVENTS',f'const u32 gObjectEventPic_{symbol}[] = INCGFX_COMP("{base}/overworld.png", ".4bpp", "-mwidth 4 -mheight 4");','#endif']
        pics+=['#if OW_POKEMON_OBJECT_EVENTS',f'static const struct SpriteFrameImage sPicTable_{symbol}[] = {{ overworld_ascending_frames(gObjectEventPic_{symbol}, 4, 4), }};','#endif']
        lm=moves(row);tm=sorted({m for _,m in lm}|{'PROTECT','REST','SLEEP_TALK','SUBSTITUTE','FACADE','TERA_BLAST','HELPING_HAND'})
        if 'GRASS' in row['types']:tm+=['LEECH_SEED','SYNTHESIS','U_TURN']
        if 'FAIRY' in row['types']:tm+=['AQUA_RING','LIFE_DEW','CALM_MIND']
        if 'STEEL' in row['types']:tm+=['IRON_DEFENSE','BODY_PRESS','ROCK_SLIDE','EARTHQUAKE']
        if 'ELECTRIC' in row['types']:tm+=['THUNDER_WAVE','VOLT_SWITCH','DISCHARGE']
        tm=sorted(set(tm))
        learn+=[f'static const struct LevelUpMove s{symbol}LevelUpLearnset[] = {{']+[f'    LEVEL_UP_MOVE({lv}, MOVE_{m}),' for lv,m in lm]+['    LEVEL_UP_END','};',f'static const u16 s{symbol}TeachableLearnset[] = {{']+[f'    MOVE_{m},' for m in tm]+['    MOVE_UNAVAILABLE','};']
        st=row['stats'];early=row['evolution'] is not None;group='FLYING' if 'FLYING' in row['types'] else 'WATER_1' if 'WATER' in row['types'] else 'MONSTER'
        category='Leaf Bird' if 'GRASS' in row['types'] else 'Rose Fish' if 'FAIRY' in row['types'] else 'Iron Horn' if 'STEEL' in row['types'] else 'Glow Jelly'
        cry='TAILLOW' if 'GRASS' in row['types'] else 'GOLDEEN' if 'FAIRY' in row['types'] else 'RHYHORN' if 'STEEL' in row['types'] else 'TENTACOOL'
        color='GREEN' if 'GRASS' in row['types'] else 'PINK' if 'FAIRY' in row['types'] else 'GRAY' if 'STEEL' in row['types'] else 'BLUE'
        species.append(f'''    [SPECIES_{key}] =
    {{
        .baseHP = {st[0]}, .baseAttack = {st[1]}, .baseDefense = {st[2]},
        .baseSpeed = {st[3]}, .baseSpAttack = {st[4]}, .baseSpDefense = {st[5]},
        .types = MON_TYPES({', '.join('TYPE_'+t for t in row['types'])}),
        .catchRate = {150 if sum(st)<350 else 90 if early else 45}, .expYield = {65 if sum(st)<350 else 140 if early else 230},
        .evYield_{'Attack' if st[1]>st[4] else 'SpAttack'} = {1 if early else 2},
        .genderRatio = PERCENT_FEMALE(50), .eggCycles = 20, .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_{group}),
        .abilities = {{ {', '.join('ABILITY_'+a for a in row['abilities'])} }},
        .bodyColor = BODY_COLOR_{color},
        .speciesName = _("{name}"), .cryId = CRY_{cry},
        .natDexNum = NATIONAL_DEX_{key}, .categoryName = _("{category}"),
        .height = {row['height']}, .weight = {row['weight']},
        .description = COMPOUND_STRING("{row['description']}"),
        .pokemonScale = 256, .trainerScale = 256,
        .frontPic = gMonFrontPic_{symbol}, .backPic = gMonBackPic_{symbol},
        .frontPicSize = MON_COORDS_SIZE(64, 64), .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = {64-normal[0].getbbox()[3]}, .backPicYOffset = {64-normal[1].getbbox()[3]},
        .frontAnimFrames = sAnims_SingleFramePlaceHolder, .backAnimId = BACK_ANIM_NONE,
        .palette = gMonPalette_{symbol}, .shinyPalette = gMonShinyPalette_{symbol},
        .iconSprite = gMonIcon_{symbol}, .iconPalIndex = {palid},
        .pokemonJumpType = PKMN_JUMP_TYPE_NORMAL,
        SHADOW(0, 0, SHADOW_SIZE_M)
        FOOTPRINT(QuestionMark)
        OVERWORLD(sPicTable_{symbol}, SIZE_32x32, SHADOW_SIZE_M, TRACKS_FOOT, sAnimTable_Following_Asym,
            gMonPalette_{symbol}, gMonShinyPalette_{symbol})
        .levelUpLearnset = s{symbol}LevelUpLearnset,
        .teachableLearnset = s{symbol}TeachableLearnset,
        .eggMoveLearnset = sNoneEggMoveLearnset,''')
        if early:species.append(f'        .evolutions = EVOLUTION({{EVO_LEVEL, {row["evolution_level"]}, SPECIES_{row["evolution"]}}}),')
        species.append('    },')
        report[key]={'front':row['front'],'back':row['back'],'palette_pairs':count,'icon_palette':palid}
    for file,lines in [('src/data/graphics/onigin_pixelart.h',graphics),('src/data/pokemon/onigin_pic_tables.h',pics),('src/data/pokemon/onigin_learnsets.h',learn),('src/data/pokemon/species_info/onigin_pixelart.h',species)]:
        (ROOT/file).write_text('// Generated by tools/brisk/import_onigin.py; artwork: onigin_pixelart.\n'+'\n'.join(lines)+'\n')
    (ROOT/'tools/brisk/onigin_sprite_import.json').write_text(json.dumps({'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'sprites':report},indent=2)+'\n')
    # Assign previously unused non-reserved slots; keep other packs intact.
    path=ROOT/'data/brisk_kanto_encounters.json';config=json.loads(path.read_text());placements=[]
    for row in ROWS:
        mapid='MAP_'+row['map'];field=row['table'];changes=config['overrides'].setdefault(mapid,{}).setdefault(field,{})
        previous=next((int(k) for k,v in changes.items() if v=='SPECIES_'+row['id']),None)
        if previous is not None:
            slot=previous
        else:
            if field=='land_mons':
                preferred=[18,19,17,16,15,14,13,12,11,10] if row['evolution'] else [12,13,14,15,16,17,18,19,11,10]
            else:
                preferred=[7,6,5,4,3,2,1,0,8]
            slot=next(i for i in preferred if str(i) not in changes)
        changes[str(slot)]='SPECIES_'+row['id'];placements.append({'species':row['id'],'map':mapid,'table':field,'slot':slot})
    path.write_text(json.dumps(config,indent=2)+'\n');generate()
    (ROOT/'data/onigin_kanto_placements.json').write_text(json.dumps(placements,indent=2)+'\n')
    print('Imported nine onigin_pixelart species; distributed across nine Kanto maps; future slots untouched.')

if __name__=='__main__':main()
