#!/usr/bin/env python3
"""Build 8x8 capsule seal tiles from existing battle graphics and game glyphs."""
from pathlib import Path
from PIL import Image, ImageDraw
ROOT = Path(__file__).resolve().parents[2]
FOLDER = ROOT / 'graphics/battle_anims/sprites'
COLORS = [(0,0,0),(48,40,64),(248,88,152),(248,216,56),(64,176,248),(168,224,248),(248,248,248),(144,144,160),(248,136,40),(232,64,48),(104,200,96),(160,96,224),(64,200,184),(160,112,80),(248,184,216),(192,192,216)]
PALETTE = sum(map(list,COLORS), []) + [0] * 720

def indexed(image):
    out = Image.new('P', image.size, 0)
    out.putpalette(PALETTE)
    out.putdata([0 if c[3] < 128 else min(range(1,16),key=lambda i:sum((c[j]-COLORS[i][j])**2 for j in range(3))) for c in image.convert('RGBA').getdata()])
    out.info['transparency'] = 0
    return out

def tile(name, size):
    im=Image.open(FOLDER / (name+'.png')).crop((0,0,*size))
    rgba=im.convert('RGBA');rgba.putalpha(Image.frombytes('L', im.size, bytes(0 if v==0 else 255 for v in im.getdata())))
    rgba=rgba.crop(rgba.getbbox());rgba.thumbnail((8,8),Image.Resampling.NEAREST)
    out=Image.new('RGBA',(8,8));out.paste(rgba,((8-rgba.width)//2,(8-rgba.height)//2))
    return indexed(out)

def main():
    # The first three slots preserve Heart, Star and Bubbles from the first version.
    old=Image.open(FOLDER/'ball_capsule_seals.png')
    tiles=[indexed(old.crop((0,i*8,8,i*8+8)).convert('RGBA')) for i in range(3)]
    line=Image.new('P',(8,8),0);line.putpalette(PALETTE);ImageDraw.Draw(line).line((1,6,6,1),fill=4,width=2);tiles.append(line)
    for name,size in [('gray_smoke',(32,32)),('electricity',(32,32)),('wisp_fire',(32,32)),('confetti',(8,8)),('flower_0',(16,16)),('music_notes',(16,16))]:tiles.append(tile(name,size))
    # Explicit small-pixel masks stay recognizable at the actual 8x8 display size.
    def blank():
        im=Image.new('P',(8,8),0);im.putpalette(PALETTE);return im
    star=blank();ImageDraw.Draw(star).polygon([(3,0),(4,2),(7,2),(5,4),(6,7),(3,5),(0,7),(1,4),(0,2),(2,2)],fill=3);tiles[1]=star
    bubble=blank();draw=ImageDraw.Draw(bubble);draw.ellipse((0,0,7,7),outline=4);draw.arc((1,1,6,6),180,280,fill=6);tiles[2]=bubble
    smoke=blank();draw=ImageDraw.Draw(smoke);draw.ellipse((0,3,5,7),fill=7);draw.ellipse((3,2,7,6),fill=15);draw.ellipse((1,0,5,5),fill=15);tiles[4]=smoke
    fire=blank();draw=ImageDraw.Draw(fire);draw.polygon([(3,0),(4,3),(6,2),(7,5),(6,7),(1,7),(0,5),(2,2)],fill=9);draw.polygon([(3,2),(4,4),(5,3),(5,6),(3,7),(2,6),(2,4)],fill=8);draw.polygon([(3,4),(4,6),(3,7),(2,6)],fill=3);tiles[6]=fire
    confetti=blank();draw=ImageDraw.Draw(confetti);draw.rectangle((0,0,1,2),fill=2);draw.rectangle((5,0,7,1),fill=3);draw.rectangle((3,3,4,4),fill=4);draw.rectangle((0,6,2,7),fill=10);draw.line((6,5,7,7),fill=11);tiles[7]=confetti
    music=blank();draw=ImageDraw.Draw(music);draw.line((4,0,4,5),fill=11,width=2);draw.line((4,0,7,2),fill=11,width=2);draw.ellipse((0,4,4,7),fill=11);tiles[9]=music
    font=Image.open(ROOT/'graphics/fonts/latin_small.png')
    for i,code in enumerate(list(range(0xBB,0xD5))+[0xAB,0xAC]):
        glyph=font.crop(((code%16)*16,(code//16)*16,(code%16+1)*16,(code//16+1)*16))
        mask=Image.frombytes('L', glyph.size, bytes(255 if v==1 else 0 for v in glyph.getdata()));mask=mask.crop(mask.getbbox());mask.thumbnail((6,7),Image.Resampling.NEAREST)
        out=Image.new('P',(8,8),0);out.putpalette(PALETTE);x=(8-mask.width)//2;y=(8-mask.height)//2
        for dx,dy in [(-1,0),(1,0),(0,-1),(0,1)]:out.paste(6,(x+dx,y+dy,x+dx+mask.width,y+dy+mask.height),mask)
        out.paste([2,3,4,10,11,8][i%6],(x,y,x+mask.width,y+mask.height),mask);tiles.append(out)
    assert len(tiles)==38
    sheet=Image.new('P',(8,38*8),0);sheet.putpalette(PALETTE)
    for i,im in enumerate(tiles):sheet.paste(im,(0,i*8))
    sheet.info['transparency']=0;sheet.save(FOLDER/'ball_capsule_seals.png')

if __name__=='__main__':main()
