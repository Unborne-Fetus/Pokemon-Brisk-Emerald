#!/usr/bin/env python3
"""Import Pawkkie's FRLG-style Red/Leaf wardrobe into Brisk Emerald."""
from pathlib import Path
from PIL import Image
import re

R=Path(__file__).resolve().parents[2]
GENDERS=[("Red","red",2),("Leaf","leaf",4)]
POSES=[("Walking","walking"),("Running","running"),("MachBike","mach_bike"),("AcroBike","acro_bike"),("Surfing","surfing"),("Underwater","underwater"),("FieldMove","field_move"),("Fishing","fishing"),("Watering","watering"),("Decorating","decorating")]
STATES=[("Normal","normal")]+POSES[2:]

def edit(path,old,new):
    p=R/path; s=p.read_text(encoding="utf-8")
    assert s.count(old)==1,(path,s.count(old),old[:50])
    p.write_text(s.replace(old,new),encoding="utf-8",newline="\n")
def extend(path,extra):
    p=R/path;s=p.read_text(encoding="utf-8")
    assert "KANTO CLASSIC: Pawkkie" not in s,path
    p.write_text(s.rstrip()+"\n\n// KANTO CLASSIC: Pawkkie FRLG-style player costumes.\n"+extra+"\n",encoding="utf-8",newline="\n")
def palette_read(p):
    s=p.read_text().splitlines();assert s[0]=="JASC-PAL" and s[2]=="16"
    return [tuple(map(int,l.split())) for l in s[3:19]]
def palette_write(p,pal):
    p.write_text("JASC-PAL\n0100\n16\n"+"\n".join("%d %d %d"%x for x in pal)+"\n")

def icons():
    mapped={};pals={}
    for _,key,_ in GENDERS:
        pal=palette_read(R/f"graphics/object_events/palettes/kanto_classic_{key}.pal");pals[key]=pal
        source=Image.open(R/f"graphics/pokenav/region_map/{key}_icon.png")
        target=Image.new("P",(16,16));target.putpalette([c for color in pal for c in color]+[0]*720)
        rgb=source.convert("RGB"); transparent=source.getpixel((0,0))
        for y in range(16):
            for x in range(16):
                if source.getpixel((x,y))==transparent: idx=0
                else:
                    color=rgb.getpixel((x,y))
                    idx=min(range(1,16),key=lambda i:sum((color[c]-pal[i][c])**2 for c in range(3)))
                target.putpixel((x,y),idx)
        target.save(R/f"graphics/pokenav/region_map/kanto_classic_{key}_icon.png")
        mapped[key]=target
        refl=[pal[0]]+[tuple(min(255,int(c*0.65+(7 if i==2 else 0))) for i,c in enumerate(rgb)) for rgb in pal[1:]]
        palette_write(R/f"graphics/object_events/palettes/kanto_classic_{key}_reflection.pal",refl)
    both=Image.new("P",(16,32));both.putpalette([c for color in pals["red"] for c in color]+[0]*720)
    both.paste(mapped["red"],(0,0));both.paste(mapped["leaf"],(0,16))
    both.save(R/"graphics/frontier_pass/kanto_classic_heads.png")

def modify():
    edit("include/constants/outfits.h","#define OUTFIT_UNUSUAL_RED 2\n#define OUTFIT_COUNT 3",
         "#define OUTFIT_UNUSUAL_RED 2\n#define OUTFIT_KANTO_CLASSIC 3\n#define OUTFIT_COUNT 4")
    edit("include/constants/outfits.h","#define OUTFIT_END   OUTFIT_UNUSUAL_RED","#define OUTFIT_END   OUTFIT_KANTO_CLASSIC")
    edit("src/outfit_menu.c","if (id > OUTFIT_COUNT)","if (id == OUTFIT_NONE || id >= OUTFIT_COUNT)")
    ids=[];infos=[];pointers=[];definitions=[];tables=[];structs=[];palette_entries=[]
    for name,key,normal_width in GENDERS:
        for pose,fn in POSES:
            w=2 if fn=="decorating" else normal_width if fn in ("walking","running") else 4
            f=R/f"graphics/object_events/pics/people/kanto_classic_{key}/{fn}.png"
            im=Image.open(f)
            assert im.height==32 and im.width%(8*w)==0,(f,im.size,w)
            definitions.append('const u32 gObjectEventPic_KantoClassic%s%s[] = INCGFX_U32("graphics/object_events/pics/people/kanto_classic_%s/%s.png", ".4bpp", "-mwidth %d -mheight 4");'%(name,pose,key,fn,w))
        for v,suff in (("",""),("Reflection","_reflection")):
            definitions.append('const u16 gObjectEventPal_KantoClassic%s%s[] = INCGFX_U16("graphics/object_events/palettes/kanto_classic_%s%s.pal", ".gbapal");'%(name,v,key,suff))
            palette_entries.append("    {gObjectEventPal_KantoClassic%s%s, OBJ_EVENT_PAL_TAG_KANTO_CLASSIC_%s%s},"%(name,v,key.upper(),"_REFLECTION" if v else ""))
        for pose,fn in STATES:
            snake=re.sub(r"(?<!^)(?=[A-Z])","_",pose).upper()
            ids.append("    OBJ_EVENT_GFX_KANTO_CLASSIC_%s_%s,"%(key.upper(),snake))
            sym="KantoClassic"+name+pose
            infos.append("extern const struct ObjectEventGraphicsInfo gObjectEventGraphicsInfo_%s;"%sym)
            pointers.append("    [OBJ_EVENT_GFX_KANTO_CLASSIC_%s_%s] = &gObjectEventGraphicsInfo_%s,"%(key.upper(),snake,sym))
            base="Brendan"+pose
            original=(R/"src/data/object_events/object_event_graphics_info.h").read_text()
            m=re.search(r"const struct ObjectEventGraphicsInfo gObjectEventGraphicsInfo_%s = \{.*?\n\};"%base,original,re.S)
            assert m,base
            code=m.group().replace("gObjectEventGraphicsInfo_"+base,"gObjectEventGraphicsInfo_"+sym).replace("sPicTable_"+base,"sPicTable_"+sym)
            code=re.sub(r"(\.paletteTag\s*=\s*)OBJ_EVENT_PAL_TAG_[A-Z0-9_]+,",r"\g<1>OBJ_EVENT_PAL_TAG_KANTO_CLASSIC_"+key.upper()+",",code)
            if pose=="Normal" and normal_width==4:
                code=code.replace(".size = 512,", ".size = 1024,").replace(".width = 16,", ".width = 32,").replace("gObjectEventBaseOam_16x32","gObjectEventBaseOam_32x32").replace("sOamTables_16x32","sOamTables_32x32")
            structs.append(code)
            pic=["static const struct SpriteFrameImage sPicTable_%s[] = {"%sym]
            if pose=="Normal":
                for sub in ("Walking","Running"):
                    pic += ["    overworld_frame(gObjectEventPic_KantoClassic%s%s, %d, 4, %d),"%(name,sub,normal_width,i) for i in range(9)]
            elif pose=="Surfing":
                pic += ["    overworld_frame(gObjectEventPic_%s, 4, 4, %d),"%(sym,i) for i in [0,2,4,0,0,2,2,4,4,1,3,5]]
            elif pose=="Underwater":
                pic += ["    overworld_frame(gObjectEventPic_%s, 4, 4, %d),"%(sym,i) for i in [0,1,2,0,0,1,1,2,2]]
            elif pose=="Watering":
                pic += ["    overworld_frame(gObjectEventPic_%s, 4, 4, %d),"%(sym,i) for i in [0,2,4,1,1,3,3,5,5]]
            elif pose=="Decorating":pic += ["    obj_frame_tiles(gObjectEventPic_%s),"%sym]
            else:pic += ["    overworld_ascending_frames(gObjectEventPic_%s, 4, 4),"%sym]
            pic.append("};");tables.append("\n".join(pic))
    edit("include/constants/event_objects.h","    OBJ_EVENT_GFX_VOLO,\n    NUM_OBJ_EVENT_GFX,","    OBJ_EVENT_GFX_VOLO,\n"+"\n".join(ids)+"\n    NUM_OBJ_EVENT_GFX,")
    tags="\n".join("#define OBJ_EVENT_PAL_TAG_KANTO_CLASSIC_%s%s 0x%04X"%(key.upper(),suf,0x11D0+2*j+k) for j,(_,key,_) in enumerate(GENDERS) for k,suf in enumerate(("","_REFLECTION")))+"\n"
    edit("include/constants/event_objects.h","#define OBJ_EVENT_PAL_TAG_NONE                    0x11FF",tags+"#define OBJ_EVENT_PAL_TAG_NONE                    0x11FF")
    extend("src/data/object_events/object_event_graphics.h","\n".join(definitions))
    extend("src/data/object_events/object_event_pic_tables.h","\n\n".join(tables))
    extend("src/data/object_events/object_event_graphics_info.h","\n\n".join(structs))
    ptr="src/data/object_events/object_event_graphics_info_pointers.h"
    edit(ptr,"const struct ObjectEventGraphicsInfo *const gObjectEventGraphicsInfoPointers[NUM_OBJ_EVENT_GFX] = {","\n".join(infos)+"\nconst struct ObjectEventGraphicsInfo *const gObjectEventGraphicsInfoPointers[NUM_OBJ_EVENT_GFX] = {")
    edit(ptr,"    [OBJ_EVENT_GFX_DIANTHA] = &gObjectEventGraphicsInfo_Diantha,","\n".join(pointers)+"\n    [OBJ_EVENT_GFX_DIANTHA] = &gObjectEventGraphicsInfo_Diantha,")
    event="src/event_object_movement.c"
    edit(event,"    {gObjectEventPal_RubySapphireMay,       OBJ_EVENT_PAL_TAG_RS_MAY},","    {gObjectEventPal_RubySapphireMay,       OBJ_EVENT_PAL_TAG_RS_MAY},\n"+"\n".join(palette_entries))
    reflection=[]
    for name,key,_ in GENDERS:
        reflection.append("static const u16 sReflectionPaletteTags_KantoClassic%s[] = {\n"%name + ("    OBJ_EVENT_PAL_TAG_KANTO_CLASSIC_%s_REFLECTION,\n"%key.upper())*4+"};")
    edit(event,"static const struct PairedPalettes sPlayerReflectionPaletteSets[] = {","\n\n".join(reflection)+"\n\nstatic const struct PairedPalettes sPlayerReflectionPaletteSets[] = {")
    paired="\n".join("    {OBJ_EVENT_PAL_TAG_KANTO_CLASSIC_%s, sReflectionPaletteTags_KantoClassic%s},"%(key.upper(),name) for name,key,_ in GENDERS)
    edit(event,"    {OBJ_EVENT_PAL_TAG_PLAYER_UNDERWATER, sReflectionPaletteTags_PlayerUnderwater},","    {OBJ_EVENT_PAL_TAG_PLAYER_UNDERWATER, sReflectionPaletteTags_PlayerUnderwater},\n"+paired)
    edit(event,"    {OBJ_EVENT_PAL_TAG_RED_LEAF,         sReflectionPaletteTags_RedLeaf},","    {OBJ_EVENT_PAL_TAG_RED_LEAF,         sReflectionPaletteTags_RedLeaf},\n"+paired)
    outf="src/data/outfit_tables.h"
    edit(outf,"#if MODERN == 0\nstatic const u8 sText_OutfitName_UsualGreen",
         '#if MODERN == 0\nstatic const u8 sText_OutfitName_KantoClassic[] = _("KANTO CLASSIC");\nstatic const u8 sText_OutfitDesc_KantoClassic[] = _("Red and Leaf styles from Kanto.");\n\nstatic const u8 sText_OutfitName_UsualGreen')
    icons_defs="\n".join('static const u16 sRegionMapPlayerIcon_KantoClassic%sGfx[] = INCGFX_U16("graphics/pokenav/region_map/kanto_classic_%s_icon.png", ".4bpp", "-mwidth 2 -mheight 2");'%(name,key) for name,key,_ in GENDERS)
    icons_defs+='\nstatic const u8 sFrontierPassPlayerIcons_KantoClassicGfx[] = INCGFX_U8("graphics/frontier_pass/kanto_classic_heads.png", ".4bpp", "-mwidth 2 -mheight 2");\n\n'
    edit(outf,"#define REGION_MAP_GFX(m, f)",icons_defs+"#define REGION_MAP_GFX(m, f)")
    entry=["    [OUTFIT_KANTO_CLASSIC] = {","        .isHidden = TRUE, // Jessie reward only; cannot buy","        .prices = {0, 0},",
           "#if MODERN == 1",'        .name = COMPOUND_STRING("KANTO CLASSIC"),','        .desc = COMPOUND_STRING("Red and Leaf styles from Kanto."),',
           "#else","        .name = sText_OutfitName_KantoClassic,","        .desc = sText_OutfitDesc_KantoClassic,","#endif",
           "        .trainerPics = { [MALE] = {TRAINER_PIC_RED, TRAINER_PIC_RED}, [FEMALE] = {TRAINER_PIC_LEAF, TRAINER_PIC_LEAF} },",
           "        .avatarGfxIds = {"]
    for gender,key in (("MALE","RED"),("FEMALE","LEAF")):
        entry+=["            [%s] = {"%gender]
        entry+=["                [PLAYER_AVATAR_STATE_%s] = OBJ_EVENT_GFX_KANTO_CLASSIC_%s_%s,"%(state,key,state) for state in ("NORMAL","MACH_BIKE","ACRO_BIKE","SURFING","UNDERWATER")]
        entry+=["            },"]
    entry+=["        },","        .animGfxIds = {"]
    for gender,key in (("MALE","RED"),("FEMALE","LEAF")):
        entry+=["            [%s] = {"%gender]
        entry+=["                [PLAYER_AVATAR_GFX_%s] = OBJ_EVENT_GFX_KANTO_CLASSIC_%s_%s,"%(anim,key,"FIELD_MOVE" if anim=="VSSEEKER" else anim) for anim in ("FIELD_MOVE","FISHING","WATERING","DECORATING","VSSEEKER")]
        entry+=["            },"]
    entry+=["        },","        .iconsRM = { sRegionMapPlayerIcon_KantoClassicRedGfx, sRegionMapPlayerIcon_KantoClassicLeafGfx },","        .iconsFP = sFrontierPassPlayerIcons_KantoClassicGfx,","    },"]
    edit(outf,"    },\n};","    },\n"+"\n".join(entry)+"\n};")
    edit("src/field_specials.c","void UnlockWallaceOutfit(void)\n{\n    UnlockOutfit(OUTFIT_UNUSUAL_RED);\n}","void UnlockWallaceOutfit(void)\n{\n    UnlockOutfit(OUTFIT_UNUSUAL_RED);\n}\n\nvoid UnlockJessieKantoClassicOutfit(void)\n{\n    UnlockOutfit(OUTFIT_KANTO_CLASSIC);\n}")
    with (R/"data/specials.inc").open("a") as f:f.write("\n\t@ Jessie victory secondary reward; append after the main prize.\n\tdef_special UnlockJessieKantoClassicOutfit\n")
    p=R/"data/scripts/xiphos_jessie_outfit.inc"
    p.write_text('@ Call this after Jessie awards the primary prize.\nCommon_EventScript_JessieKantoClassicReward::\n\tspecial UnlockJessieKantoClassicOutfit\n\tmsgbox Common_Text_JessieKantoClassicReward, MSGBOX_DEFAULT\n\treturn\n\nCommon_Text_JessieKantoClassicReward:\n\t.string "KANTO CLASSIC was unlocked!\\n"\n\t.string "Red and Leaf styles are now\\n"\n\t.string "available in your wardrobe!$"\n')
    p=R/"data/event_scripts.s";p.write_bytes(p.read_bytes()+b'\n\t.include "data/scripts/xiphos_jessie_outfit.inc"\n')
    doc=R/"docs/brisk/kanto-classic-jessie-outfit.md"
    doc.write_text("# Kanto Classic outfit: Jessie secondary reward\n\nNew outfit ID 3: Red for male player, Leaf for female player. Previous outfit IDs stay unchanged. Full walking, running, biking, surfing, diving, field move, fishing, watering and decorating sprites are provided.\n\nSource: Team Aqua Asset Repo, Pawkkie FRLG-style player sets. Credit Pawkkie and Poffin_Case for underlying art and Pawkkie's conversion/recolor and dive art.\n\nJessie battle scripts do not yet exist in-game. After awarding the PRIMARY Jessie prize, the future Jessie win script must execute:\n\n    call Common_EventScript_JessieKantoClassicReward\n\nThis special unlocks the hidden Kanto Classic outfit (idempotent). Do not call it on a loss or mere interaction. Full end-to-end battle verification awaits Jessie's implementation.\n")
    test=R/"test/brisk_kanto_classic_outfit.c"
    test.write_text('#include "global.h"\n#include "outfit_menu.h"\n#include "test/test.h"\nvoid UnlockJessieKantoClassicOutfit(void);\nTEST("Jessie Kanto Classic outfit unlock is idempotent")\n{\n    gSaveBlock2Ptr->outfits[0]=0;\n    UnlockOutfit(OUTFIT_USUAL_GREEN);\n    EXPECT(!GetOutfitStatus(OUTFIT_KANTO_CLASSIC));\n    UnlockJessieKantoClassicOutfit();\n    EXPECT(GetOutfitStatus(OUTFIT_KANTO_CLASSIC));\n    EXPECT(GetOutfitStatus(OUTFIT_USUAL_GREEN));\n    UnlockJessieKantoClassicOutfit();\n    EXPECT(GetOutfitStatus(OUTFIT_KANTO_CLASSIC));\n}\nTEST("Jessie outfit ID guard rejects invalid indices")\n{\n    gSaveBlock2Ptr->outfits[0]=0;\n    UnlockOutfit(OUTFIT_COUNT);\n    UnlockOutfit(OUTFIT_NONE);\n    EXPECT(!GetOutfitStatus(OUTFIT_KANTO_CLASSIC));\n}\n')

if __name__=="__main__":
    for _,key,_ in GENDERS:
        for _,fn in POSES:assert (R/f"graphics/object_events/pics/people/kanto_classic_{key}/{fn}.png").exists()
    icons()
    modify()
    print("Imported Red/Leaf player graphics and Jessie outfit reward hook.")
