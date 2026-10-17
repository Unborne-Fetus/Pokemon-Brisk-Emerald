#!/usr/bin/env python3
"""Import princess-phoenix's supplied sheets without generating replacement art.

Usage: python tools/brisk/import_phoenix_fakemon.py /path/to/original/sheets
The source sheet filenames and crop rectangles are retained here for repeatability.
"""
from pathlib import Path
from collections import Counter
import argparse
import json
from PIL import Image
ROOT = Path(__file__).resolve().parents[2]
BG = {(144,177,162), (155,196,180)}
CARDS = [('Hissiorite','der6w88'), ('Cobarett','der6wur'), ('Pythonova','der6x9y'), ('Baoby','deq019d'), ('Baobaraffe','deq029c')]
NATIVE = {'Tsushimi', 'Tobishimi', 'Firomenis'}

def remove_background(image):
    image = image.convert('RGBA')
    image.putdata([(0,0,0,0) if p[:3] in BG or not p[3] else p for p in image.getdata()])
    return image

def centered(image, size):
    box = image.getbbox()
    if box is None: raise ValueError('Empty sprite crop')
    obj = image.crop(box)
    if obj.width > size or obj.height > size: raise ValueError(f'Crop exceeds {size}px: {box}')
    result = Image.new('RGBA', (size,size))
    result.paste(obj, ((size-obj.width)//2,size-obj.height))
    return result

def gba(color): return tuple(v//8*8 for v in color[:3])
def distance(a,b): return sum((x-y)**2 for x,y in zip(a,b))
def palette(path): return [tuple(map(int,s.split())) for s in path.read_text().splitlines()[3:19]]
def indexed(data,size,colors):
    im=Image.new('P',size);im.putdata(data)
    im.putpalette([c for rgb in colors for c in rgb]+[0]*(768-len(colors)*3));im.info['transparency']=0
    return im

def pack80(image):
    result=Image.new('P',(8,800));result.putpalette(image.getpalette());result.info['transparency']=0
    # Must match sMonSubsprites80 in battle_gfx_sfx_util.c.
    n=0
    for x,y,w,h in [(0,0,64,64),(64,0,16,32),(64,32,16,32),(0,64,32,16),(32,64,32,16),(64,64,16,16)]:
        for ty in range(y,y+h,8):
            for tx in range(x,x+w,8):
                result.paste(image.crop((tx,ty,tx+8,ty+8)),(0,n*8));n+=1
    assert n==100
    return result

def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('source_dir',type=Path);args=parser.parse_args()
    def sheet(stem):
        matches=list(args.source_dir.glob(stem+'*.png'))
        if len(matches)!=1: raise ValueError(f'Expected one {stem} sheet, found {len(matches)}')
        return Image.open(matches[0]).convert('RGBA')
    mons={};icons={}
    for name,stem in CARDS:
        im=sheet(stem)
        mons[name]=[remove_background(im.crop((x,y,x+64,y+64))) for x,y in [(555,260),(619,260),(555,324),(619,324)]]
        icons[name]=[remove_background(im.crop((x,468,x+32,500))) for x in [555,587]]
    im=sheet('de9b5an')
    for i,name in enumerate(['Drashimi','Tsushimi','Tobishimi']):
        x=i*384;y=496 if i==2 else 512;h=80 if i==2 else 64
        mons[name]=[remove_background(im.crop((a,b,a+96,b+h))) for a,b in [(x,y),(x,y+96),(x+96,y),(x+96,y+96)]]
        icons[name]=[remove_background(im.crop((x+a,480,x+a+32,512))) for a in [288,320]]
    im=sheet('de8e75d')
    for i,name in enumerate(['Merlicun','Firomenis','Anu','Nuenflu']):
        y=i*96
        mons[name]=[remove_background(im.crop((x,y,x+96,y+96))) for x in [192,480,288,576]]
        icons[name]=[remove_background(im.crop((x,y,x+32,y+32))) for x in [128,160]]
    icon_pals=[palette(ROOT/f'graphics/pokemon/icon_palettes/pal{i}.pal') for i in range(6)]
    report={}
    for name,frames in mons.items():
        size=80 if name in NATIVE else 64
        frames=[centered(im,size) for im in frames]
        # Normal and shiny share tile indexes on GBA. Jointly quantize the
        # supplied pairs to 15 opaque colors, retaining their original colors.
        pairs=Counter((gba(n),gba(s)) for a,b in [(0,1),(2,3)] for n,s in zip(frames[a].getdata(),frames[b].getdata()) if n[3])
        selected=[p for p,_ in pairs.most_common(15)]
        norm=[(0,0,0)]+[p[0] for p in selected];shiny=[(0,0,0)]+[p[1] for p in selected]
        norm += [(0,0,0)]*(16-len(norm));shiny += [(0,0,0)]*(16-len(shiny))
        def match(pair): return 1+min(range(len(selected)),key=lambda j:4*distance(pair[0],selected[j][0])+distance(pair[1],selected[j][1]))
        folder=ROOT/'graphics/pokemon/fakemon'/name.lower();folder.mkdir(parents=True,exist_ok=True)
        previews=[]
        for a,b,label in [(0,1,'front'),(2,3,'back')]:
            data=[0 if not n[3] else match((gba(n),gba(s))) for n,s in zip(frames[a].getdata(),frames[b].getdata())]
            pic=indexed(data,(size,size),norm)
            if size==80:
                pack80(pic).save(folder/(label+'_tiles.png'))
                pic=pic.resize((64,64),Image.Resampling.NEAREST)
            pic.save(folder/(label+'.png'))
            preview=pic.copy();preview.putpalette([c for rgb in shiny for c in rgb]+[0]*720);preview.save(folder/(label+'_shiny.png'))
        for label,pal in [('normal',norm),('shiny',shiny)]:
            (folder/(label+'.pal')).write_text('JASC-PAL\n0100\n16\n'+'\n'.join(' '.join(map(str,p)) for p in pal)+'\n')
        colors=Counter(gba(p) for im in icons[name] for p in im.getdata() if p[3])
        palid=min(range(6),key=lambda j:sum(count*min(distance(c,k) for k in icon_pals[j][1:]) for c,count in colors.items()))
        data=[0 if not p[3] else 1+min(range(15),key=lambda j:distance(gba(p),icon_pals[palid][j+1])) for im in icons[name] for p in im.getdata()]
        indexed(data,(32,64),icon_pals[palid]).save(folder/'icon.png')
        report[name]={'icon_palette':palid,'native_size':size,'palette_pairs':len(pairs)}
    (ROOT/'tools/brisk/phoenix_sprite_import.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f'Imported {len(mons)} species')
if __name__=='__main__': main()
