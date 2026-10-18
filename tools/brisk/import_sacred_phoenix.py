#!/usr/bin/env python3
"""Convert the supplied Sacred Phoenix sheets; never generates replacement art.
Usage: python tools/brisk/import_sacred_phoenix.py /path/to/CuteCreature_Sprite
Gameplay definitions live in sacred_phoenix_species.json; species stay in Gen 3.
"""
from pathlib import Path
from collections import Counter
import argparse, json, hashlib
from PIL import Image, ImageDraw
from import_phoenix_fakemon import gba, distance, indexed, pack80, palette
ROOT=Path(__file__).resolve().parents[2]

def quantize_pairs(frames):
    counts=Counter((gba(n),gba(s)) for normal,shiny in frames for n,s in zip(normal.getdata(),shiny.getdata()) if n[3])
    selected=[p for p,_ in counts.most_common(15)]
    palettes=[[(0,0,0)]+[p[i] for p in selected]+[(0,0,0)]*(15-len(selected)) for i in range(2)]
    pictures=[]
    for normal,shiny in frames:
        data=[0 if not n[3] else 1+min(range(len(selected)),key=lambda j:4*distance(gba(n),selected[j][0])+distance(gba(s),selected[j][1])) for n,s in zip(normal.getdata(),shiny.getdata())]
        pictures.append(indexed(data,normal.size,palettes[0]))
    return pictures,palettes,len(counts)

def save_pal(path,colors):
    path.write_text('JASC-PAL\n0100\n16\n'+'\n'.join(' '.join(map(str,p)) for p in colors)+'\n')

def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('source',type=Path);args=ap.parse_args()
    rows=json.loads((ROOT/'tools/brisk/sacred_phoenix_species.json').read_text())
    graphics=['// Supplied Sacred Phoenix artwork. See docs/sacred_phoenix_fakemon.md.']
    pics=['// Eight native 32x32 follower frames, with separate shiny palettes.']
    species=[];learnsets=[];report={};cards=[]
    iconpals=[palette(ROOT/f'graphics/pokemon/icon_palettes/pal{i}.pal') for i in range(6)]
    for row in rows:
        name=row['name'];key=name.lower();enum=name.upper();paths=list(args.source.glob(key+'*.png'));assert len(paths)==1,name
        raw=paths[0].read_bytes();sheet=Image.open(paths[0]).convert('RGBA')
        # Strip sheet gutters, including opaque guide lines on indexed sheets.
        pixels=sheet.load()
        for yy in range(sheet.height):
            for xx in range(sheet.width):
                if pixels[xx,yy][3] != 255 or xx in (0,97,195,259) or yy in (0,97,195,323):
                    pixels[xx,yy]=(0,0,0,0)
        pairs=[];bounds=[]
        for y in (1,98):
            normal=sheet.crop((1,y,97,y+96));shiny=sheet.crop((98,y,194,y+96))
            # The grid occupies the gutters, outside these rectangles.
            mask=Image.new('L',normal.size);mask.putdata([max(a[3],b[3]) for a,b in zip(normal.getdata(),shiny.getdata())]);box=mask.getbbox();assert box
            bounds.append([box[2]-box[0],box[3]-box[1]])
            pair=[]
            for im in (normal,shiny):
                im=im.crop(im.getbbox());scale=min(1,80/im.width,80/im.height)
                if scale<1:im=im.resize((round(im.width*scale),round(im.height*scale)),Image.Resampling.NEAREST)
                canvas=Image.new('RGBA',(80,80));canvas.paste(im,((80-im.width)//2,80-im.height));pair.append(canvas)
            pairs.append(pair)
        battle,pals,count=quantize_pairs(pairs)
        folder=ROOT/'graphics/pokemon/sacred_phoenix'/key;folder.mkdir(parents=True,exist_ok=True)
        for pic,label in zip(battle,['front','back']):
            pic.save(folder/(label+'_80.png'));pack80(pic).save(folder/(label+'_tiles.png'))
            pic.resize((64,64),Image.Resampling.NEAREST).save(folder/(label+'.png'))
            shiny=pic.copy();shiny.putpalette([c for rgb in pals[1] for c in rgb]+[0]*720);shiny.save(folder/(label+'_shiny_80.png'))
        for label,pal in zip(['normal','shiny'],pals):save_pal(folder/(label+'.pal'),pal)
        # Use the actual supplied two-frame menu icon, mapped to a native bank.
        icons=[sheet.crop((x,0,x+32,32)) for x in (196,228)]
        colors=Counter(gba(p) for im in icons for p in im.getdata() if p[3])
        palid=min(range(6),key=lambda j:sum(c*min(distance(rgb,k) for k in iconpals[j][1:]) for rgb,c in colors.items()))
        data=[0 if not p[3] else 1+min(range(15),key=lambda j:distance(gba(p),iconpals[palid][j+1])) for im in icons for p in im.getdata()]
        indexed(data,(32,64),iconpals[palid]).save(folder/'icon.png')
        # Source direction rows: south, west, east, north. Native asymmetric
        # following uses south, north, west, east, two walking frames each.
        ow=[]
        for direction in (0,3,1,2):
            for frame in (0,1):
                y=196+direction*32;x=frame*32
                ow.append([sheet.crop((x,y,x+32,y+32)),sheet.crop((x+128,y,x+160,y+32))])
        owframes,owpals,_=quantize_pairs(ow)
        strip=indexed([v for im in owframes for v in im.getdata()],(32,256),owpals[0]);strip.save(folder/'overworld.png')
        for label,pal in zip(['normal','shiny'],owpals):save_pal(folder/('overworld_'+label+'.pal'),pal)
        base='graphics/pokemon/sacred_phoenix/'+key
        for label,ctype,suffix,asset in [('FrontPic','u32','.4bpp.smol','front.png'),('BackPic','u32','.4bpp.smol','back.png'),('FrontPic80','u8','.4bpp','front_tiles.png'),('BackPic80','u8','.4bpp','back_tiles.png'),('Palette','u16','.gbapal','normal.pal'),('ShinyPalette','u16','.gbapal','shiny.pal'),('Icon','u8','.4bpp','icon.png')]:
            graphics.append(f'const {ctype} gMon{label}_{name}[] = INCGFX_{ctype.upper()}("{base}/{asset}", "{suffix}");')
        graphics += ['#if OW_POKEMON_OBJECT_EVENTS',f'const u32 gObjectEventPic_{name}[] = INCGFX_COMP("{base}/overworld.png", ".4bpp", "-mwidth 4 -mheight 4");','#if OW_PKMN_OBJECTS_SHARE_PALETTES == FALSE',f'const u16 gOverworldPalette_{name}[] = INCGFX_U16("{base}/overworld_normal.pal", ".gbapal");',f'const u16 gShinyOverworldPalette_{name}[] = INCGFX_U16("{base}/overworld_shiny.pal", ".gbapal");','#endif','#endif']
        pics+=['#if OW_POKEMON_OBJECT_EVENTS',f'static const struct SpriteFrameImage sPicTable_{name}[] = {{',f'    overworld_ascending_frames(gObjectEventPic_{name}, 4, 4),','};','#endif']
        levels=[1,1,5,9,13,17,21,25,30,35,40,46,52,58]
        learnsets += [f'static const struct LevelUpMove s{name}LevelUpLearnset[] = {{']+[f'    LEVEL_UP_MOVE({level}, MOVE_{move}),' for level,move in zip(levels,row['moves'])]+['    LEVEL_UP_END','};']
        tm=sorted(set(row['moves']+['PROTECT','REST','SLEEP_TALK','SUBSTITUTE','FACADE','TERA_BLAST','HELPING_HAND']))
        learnsets += [f'static const u16 s{name}TeachableLearnset[] = {{']+[f'    MOVE_{m},' for m in tm]+['    MOVE_UNAVAILABLE','};']
        st=row['stats'];types=', '.join('TYPE_'+t for t in row['types']);abilities=', '.join('ABILITY_'+a for a in row['abilities'])
        desc=json.dumps(row['description']).replace('\\n','\\n"\n            "')
        species.append(f'''    [SPECIES_{enum}] =
    {{
        .baseHP = {st[0]}, .baseAttack = {st[1]}, .baseDefense = {st[2]},
        .baseSpeed = {st[3]}, .baseSpAttack = {st[4]}, .baseSpDefense = {st[5]},
        .types = MON_TYPES({types}),
        .catchRate = {190 if row['evolution'] else 45}, .expYield = {65 if sum(st)<300 else 150 if row['evolution'] else 240},
        .evYield_{'Attack' if st[1]>st[4] else 'SpAttack'} = {1 if row['evolution'] else 2},
        .genderRatio = PERCENT_FEMALE({0 if name=='Druichy' else 100 if name=='Duchefly' else 50}),
        .eggCycles = 20, .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_{'BUG' if 'BUG' in row['types'] else 'WATER_1' if 'WATER' in row['types'] else 'GRASS' if 'GRASS' in row['types'] else 'FIELD'}),
        .abilities = {{ {abilities} }},
        .bodyColor = BODY_COLOR_{'BLUE' if 'WATER' in row['types'] else 'GREEN' if 'GRASS' in row['types'] or 'BUG' in row['types'] else 'BROWN'},
        .speciesName = _("{name}"), .cryId = CRY_{row['cry']},
        .natDexNum = NATIONAL_DEX_{enum}, .categoryName = _("{row['category']}"),
        .height = {row['height']}, .weight = {row['weight']},
        .description = COMPOUND_STRING({desc}),
        .pokemonScale = 256, .trainerScale = 256,
        .frontPic = gMonFrontPic_{name}, .backPic = gMonBackPic_{name},
        .frontPic80 = gMonFrontPic80_{name}, .backPic80 = gMonBackPic80_{name},
        .frontPicSize = MON_COORDS_SIZE(64, 64), .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0, .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder, .backAnimId = BACK_ANIM_NONE,
        .palette = gMonPalette_{name}, .shinyPalette = gMonShinyPalette_{name},
        .iconSprite = gMonIcon_{name}, .iconPalIndex = {palid},
        .pokemonJumpType = PKMN_JUMP_TYPE_NORMAL,
        SHADOW(0, 0, SHADOW_SIZE_M)
        FOOTPRINT(QuestionMark)
        OVERWORLD(sPicTable_{name}, SIZE_32x32, SHADOW_SIZE_M, TRACKS_FOOT, sAnimTable_Following_Asym,
            gOverworldPalette_{name}, gShinyOverworldPalette_{name})
        .levelUpLearnset = s{name}LevelUpLearnset,
        .teachableLearnset = s{name}TeachableLearnset,''')
        if row['evolution']:
            if row['evolution_gender']=='split':
                species.append('        .evolutions = EVOLUTION({EVO_LEVEL, 36, SPECIES_DRUICHY, CONDITIONS({IF_GENDER, MON_MALE})},\n                                {EVO_LEVEL, 36, SPECIES_DUCHEFLY, CONDITIONS({IF_GENDER, MON_FEMALE})}),')
            else:species.append(f"        .evolutions = EVOLUTION({{EVO_LEVEL, {row['evolution_level']}, SPECIES_{row['evolution'].upper()}}}),")
        species.append('    },\n')
        report[name]=dict(source=paths[0].name,sha256=hashlib.sha256(raw).hexdigest(),source_bounds=bounds,palette_pairs=count,native_canvas=80,icon_palette=palid)
        card=Image.new('RGBA',(360,110),'#d9e6e0');d=ImageDraw.Draw(card);d.text((4,2),name,fill='#142b29')
        for i,pic in enumerate([Image.open(folder/'front_80.png'),Image.open(folder/'front_shiny_80.png'),Image.open(folder/'back_80.png'),Image.open(folder/'back_shiny_80.png')]):card.alpha_composite(pic.convert('RGBA'),(i*88+4,26))
        cards.append(card)
    (ROOT/'src/data/graphics/sacred_phoenix.h').write_text('\n'.join(graphics)+'\n')
    (ROOT/'src/data/pokemon/sacred_phoenix_pic_tables.h').write_text('\n'.join(pics)+'\n')
    (ROOT/'src/data/pokemon/sacred_phoenix_learnsets.h').write_text('\n'.join(learnsets)+'\n')
    p=ROOT/'src/data/pokemon/species_info/gen_3_families.h';s=p.read_text();marker='// Sacred Phoenix species (native Hoenn table).'
    if marker in s:s=s.split(marker)[0]+'#ifdef __INTELLISENSE__\n};\n#endif\n'
    at=s.rindex('#ifdef __INTELLISENSE__');s=s[:at]+marker+'\n'+'\n'.join(species)+'\n'+s[at:];p.write_text(s)
    (ROOT/'tools/brisk/sacred_phoenix_sprite_import.json').write_text(json.dumps(report,indent=2)+'\n')
    preview=Image.new('RGBA',(720,660))
    for i,card in enumerate(cards):preview.paste(card,((i%2)*360,(i//2)*110))
    (ROOT/'docs/sacred_phoenix').mkdir(exist_ok=True);preview.convert('RGB').save(ROOT/'docs/sacred_phoenix/sprites.png')
    print('Imported',len(rows),'complete sprite sets')
if __name__=='__main__':main()
