#!/usr/bin/env python3
"""Author Brisk's Kanto postgame trainers and safe champion-visit dialogue."""
from collections import Counter
import argparse
from functools import lru_cache
import hashlib
import json
from pathlib import Path
import re
import textwrap
import author_mode_teams as teams
from kanto_dialogue import Dialogue, NAMED, source_words, place, battle_words

ROOT = Path(__file__).resolve().parents[2]
MODES = ('Easy','Normal','Hard','Kaizo')
LEVELS = {'Easy':90,'Normal':94,'Hard':97,'Kaizo':100}
BOSSES = {
 'BROCK': ('ROCK','TYRANITAR RHYPERIOR AERODACTYL CRADILY GASTRODON METAGROSS'),
 'MISTY': ('WATER','POLITOED KINGDRA LUDICOLO STARMIE GYARADOS FERROTHORN'),
 'LT_SURGE': ('ELECTRIC','PELIPPER RAICHU_ALOLA MAGNEZONE ROTOM_WASH ELECTIVIRE MANECTRIC'),
 'ERIKA': ('GRASS','TORKOAL VENUSAUR LILLIGANT AMOONGUSS FERROTHORN MEOWSCARADA'),
 'KOGA': ('POISON','CROBAT TOXAPEX AMOONGUSS SALAZZLE NIDOKING SLOWKING_GALAR'),
 'SABRINA': ('PSYCHIC','INDEEDEE_F HATTERENE METAGROSS GARDEVOIR ALAKAZAM SLOWBRO'),
 'BLAINE': ('FIRE','TORKOAL NINETALES CHARIZARD VOLCARONA ARCANINE ROTOM_HEAT'),
 'GIOVANNI': ('GROUND','HIPPOWDON EXCADRILL GARCHOMP GASTRODON MAMOSWINE LANDORUS_THERIAN'),
 'LORELEI': ('ICE','NINETALES_ALOLA BAXCALIBUR WEAVILE LAPRAS MAMOSWINE ROTOM_WASH'),
 'BRUNO': ('FIGHTING','HAWLUCHA ANNIHILAPE CONKELDURR LUCARIO IRON_VALIANT GREAT_TUSK'),
 'AGATHA': ('GHOST','GENGAR MIMIKYU AEGISLASH DRAGAPULT SKELEDIRGE GHOLDENGO'),
 'LANCE': ('DRAGON','DRAGONITE GARCHOMP SALAMENCE KINGDRA BAXCALIBUR HYDREIGON'),
}


def save(path,data):
    p=ROOT/path;p.parent.mkdir(parents=True,exist_ok=True)
    p.write_text(json.dumps(data,indent=2,ensure_ascii=False)+'\n')

def text(label,words):
    lines=textwrap.wrap(words,30,break_long_words=False,break_on_hyphens=False)
    return label+':\n'+''.join('\t.string '+json.dumps(line+('$' if i==len(lines)-1 else '\\p' if i%2 else '\\n'),ensure_ascii=False).replace('\\\\','\\')+'\n' for i,line in enumerate(lines))+'\n'


def trainer_source():
    return teams.TEAM_RE.findall((ROOT/'src/data/trainers_frlg.party').read_text())


def author_rosters():
    constants=['#ifndef GUARD_BRISK_KANTO_TRAINERS_H','#define GUARD_BRISK_KANTO_TRAINERS_H','']
    ids=dict(re.findall(r'^#define (TRAINER_\w+)\s+(\d+)\s*$',(ROOT/'include/constants/opponents_frlg.h').read_text(),re.M))
    output=['/* Kanto: explicit postgame parties. Generate with tools/brisk/author_kanto_postgame.py. */\n']
    summary=[];metadata={}
    for tid,body in trainer_source():
        new='TRAINER_BRISK_KANTO_'+tid.removeprefix('TRAINER_')
        constants.append(f'#define {new} (BRISK_KANTO_TRAINER_BASE + {ids[tid]})')
        header=body.split('\n\n',1)[0]
        name=re.search(r'^Name:\s*(.*)',header,re.M)[1]
        cls=re.search(r'^Class:\s*(.*)',header,re.M)[1]
        metadata[tid]={'id':new,'name':name,'class':cls,'double':'Double Battle: Yes' in header}
        if tid=='TRAINER_NONE':continue
        original=[teams.parse_mon(m) for m in body.split('\n\n')[1:] if m.strip()]
        typ=teams.theme(tid,header.replace(' Frlg',''),original) if original else 'NORMAL'
        boss=next((b for b in BOSSES if (tid.startswith('TRAINER_LEADER_') or tid.startswith('TRAINER_ELITE_FOUR_')) and tid.endswith('_'+b)),None)
        if boss:typ=BOSSES[boss][0]
        core=['SPECIES_'+s for s in BOSSES[boss][1].split()] if boss else []
        seed=int.from_bytes(hashlib.sha256(tid.encode()).digest()[:4],'little')
        signatures=[];rows=[]
        for mi,mode in enumerate(MODES):
            level=min(100,LEVELS[mode]+(2 if boss else 0))
            pool=teams.candidates(typ,level,mode,seed+mi*3)
            # Keep species mature and battle-ready in this postgame region.
            pool=[s for s in pool if sum(teams.info(s)['stats'])>=420 and s not in ('SPECIES_CHANSEY','SPECIES_DUSCLOPS')]
            support=teams.candidates(('STEEL','WATER','DARK','FAIRY')[mi],level,mode,seed+mi)
            pool=list(dict.fromkeys(pool+support))
            chosen=[]
            # Boss cores retain their type identity while changing partners in
            # every mode; the last two slots supply coverage or defensive pivots.
            preferred=core[mi:mi+3]+pool if boss else pool
            for sp in preferred:
                if sp in chosen or not teams.legal_moves(sp):continue
                try:block(sp,level,mode,'ace' if len(chosen)==5 else 'support',metadata[tid]['double'])
                except (ValueError,KeyError):continue
                chosen.append(sp)
                if len(chosen)==6:break
            assert len(chosen)==6,(tid,mode)
            if Counter(chosen) in signatures:
                for sp in support+pool:
                    if sp not in chosen:
                        chosen[-1]=sp
                        if Counter(chosen) not in signatures:break
            assert Counter(chosen) not in signatures,(tid,mode,'duplicate roster')
            signatures.append(Counter(chosen))
            lines=[x for x in header.splitlines() if not x.startswith(('AI:','Difficulty:','Items:'))]
            ai='Check Bad Move' if mode=='Easy' else 'Basic Trainer / Smart Switching / HP Aware / Try To 2HKO / Smart Tera'
            lines+=['AI: '+ai,'Difficulty: '+mode,'Authored Party: Yes']
            mons=[block(sp,level,mode,'ace' if i==5 else 'support',metadata[tid]['double']) for i,sp in enumerate(chosen)]
            output.append('=== '+new+' ===\n'+'\n'.join(lines)+'\n\n'+'\n\n'.join(mons)+'\n\n')
            rows.append(', '.join(s.removeprefix('SPECIES_') for s in chosen)+f' ({level})')
        summary.append('| '+name+' (`'+new+'`) | '+' | '.join(rows)+' |')
    constants+=['','#endif','']
    (ROOT/'include/constants/brisk_kanto_trainers.h').write_text('\n'.join(constants))
    (ROOT/'src/data/brisk_kanto_teams.party').write_text('\n'.join(output).rstrip()+'\n')
    (ROOT/'docs/brisk/kanto-trainer-teams.md').write_text('# Kanto postgame teams\n\nAll teams contain six Pokemon and differ across all four modes. Levels are\n90-92 Easy, 94-96 Normal, 97-99 Hard and 100 Kaizo. Brisk mode rules supply\nAI, prices, bag rules and EXP behavior; these parties own their actual sets.\n\n| Trainer | Easy | Normal | Hard | Kaizo |\n| --- | --- | --- | --- | --- |\n'+'\n'.join(summary)+'\n')
    return metadata

@lru_cache(None)
def block(species,level,mode,role,doubles):
    # Resolve species aliases before reading their native set data.
    canonical = {'SPECIES_GASTRODON':'SPECIES_GASTRODON_WEST',
                 'SPECIES_AEGISLASH':'SPECIES_AEGISLASH_SHIELD',
                 'SPECIES_MIMIKYU':'SPECIES_MIMIKYU_DISGUISED'}.get(species,species)
    result = teams.mon_block(canonical,level,mode,role,doubles)
    # Gengar's ability is defined through a generation-dependent C macro.
    if canonical == 'SPECIES_GENGAR' and 'Ability:' not in result:
        result = result.replace('Nature:', 'Ability: ABILITY_CURSED_BODY\nNature:')
    return result


def author_dialogue(metadata):
    # Resolve a script to its trainer encounter without executing any old
    # Rocket/rival quests, rewards, badges or shared Hoenn variables.
    source='\n'.join(p.read_text() for p in sorted((ROOT/'data').rglob('*.inc')) if not any(x in p.parts for x in ('layouts','tilesets')) and p.name not in ('events.inc','header.inc','connections.inc','brisk_kanto_postgame.inc'))
    scripts={k:v for k,v in re.findall(r'^(\w+)::?\s*\n(.*?)(?=^\w+::?|\Z)',source,re.M|re.S)}
    maps={json.loads(p.read_text())['id']:(p.parent.name,json.loads(p.read_text())) for p in (ROOT/'data/maps').glob('*/map.json')}
    heal=json.loads((ROOT/'src/data/heal_locations.json').read_text())['heal_locations']
    heals={h['respawn_map']:h['id'] for h in heal if 'respawn_map' in h}
    manifest={'maps':{},'trainers':metadata};out=['@ Authored dialogue for Hoenn Champion visits. No FRLG story state is executed.\n']
    dialogue=Dialogue()
    trainer_count=0
    for name,m in sorted(maps.values()):
        if m.get('region')!='REGION_KANTO' or name=='OrangeStraitEast':continue
        records={}
        for i,raw in enumerate(m.get('object_events',[]),1):
            obj=raw
            if raw.get('type')=='clone':
                target=maps.get(raw['target_map'])
                if target:
                    local=str(raw['target_local_id'])
                    target_obj=(target[1]['object_events'][int(local)-1] if local.isdigit() else next((o for o in target[1]['object_events'] if o.get('local_id')==local),None))
                    if target_obj:
                        obj={**target_obj,'type':'object','x':raw['x'],'y':raw['y']}
            # Original map-load scripts no longer run, so their graphics
            # variables must not inherit a Hoenn rival's sprite from the save.
            if obj.get('graphics_id','').startswith('OBJ_EVENT_GFX_VAR_'):
                fixed=('OBJ_EVENT_GFX_OLD_MAN_1' if name=='ViridianCity_Frlg' else
                       'OBJ_EVENT_GFX_OMANYTE' if name=='FuchsiaCity_Frlg' else None)
                if fixed:
                    obj={**obj,'graphics_id':fixed}
                else:
                    records[str(i)]={'omit':True,'script':'','trainer':''}
                    continue
            script=obj.get('script','');gfx=obj.get('graphics_id','')
            # Ferries, sleeping roadblocks and duplicate traveling cutscene
            # actors have no safe postgame event controller.
            if gfx in ('OBJ_EVENT_GFX_SEAGALLOP','OBJ_EVENT_GFX_SS_ANNE','OBJ_EVENT_GFX_SNORLAX') or (gfx=='OBJ_EVENT_GFX_BILL' and name!='CinnabarIsland_Frlg'):
                records[str(i)]={'omit':True,'script':'','trainer':''}
                continue
            body=scripts.get(script,'')
            match=re.search(r'trainerbattle_(?:single|double|rematch|continue)\s+(TRAINER_\w+)',body)
            tid=match[1] if match and match[1] in metadata else None
            # Leaders are sometimes dispatched by a jump before their battle.
            if tid is None:
                for boss in BOSSES:
                    if gfx=='OBJ_EVENT_GFX_'+boss:
                        key='TRAINER_'+('ELITE_FOUR_' if boss in ('LORELEI','BRUNO','AGATHA','LANCE') else 'LEADER_')+boss
                        if key in metadata:tid=key
            label=f'BriskKanto_{name}_{i}'
            rec={'script':label,'trainer':metadata[tid]['id'] if tid else '', 'trainer_type':'TRAINER_TYPE_NORMAL' if tid else 'TRAINER_TYPE_NONE'}
            if obj is not raw:rec['object']=obj
            # Preserve explicit Brisk legendary encounters and their own flags.
            if script.startswith('BriskStatic_EventScript_'):
                rec={'script':script,'trainer':'','trainer_type':'TRAINER_TYPE_NONE','keep_flag':True}
                records[str(i)]=rec;continue
            if script in ('EventScript_StrengthBoulder','EventScript_CutTree','EventScript_RockSmash'):
                rec['script']=script
                records[str(i)]=rec
                continue
            if tid:
                trainer_count+=1;t=metadata[tid]
                seed=int.from_bytes(hashlib.sha256((name+str(i)+tid).encode()).digest()[:4],'little')
                intro=battle_words(dialogue,scripts,body,script,name,t['name'],'intro',seed)
                defeat=battle_words(dialogue,scripts,body,script,name,t['name'],'defeat',seed)
                after=battle_words(dialogue,scripts,body,script,name,t['name'],'after',seed)
                if tid.startswith(('TRAINER_LEADER_','TRAINER_ELITE_FOUR_')):
                    intro=dialogue.unique(f"{t['name']}: Hoenn's Champion and savior! Your team protected a whole region. Let's see that bond in battle!",name,'boss')
                macro='trainerbattle_double' if t['double'] else 'trainerbattle_single'
                args=f"{t['id']}, {label}_Intro, {label}_Defeat"+(', BriskKanto_Text_TwoPokemon' if t['double'] else '')
                out+=[label+'::\n\t'+macro+' '+args+'\n\tmsgbox '+label+'_After, MSGBOX_AUTOCLOSE\n\tend\n\n',text(label+'_Intro',intro),text(label+'_Defeat',defeat),text(label+'_After',after)]
            elif 'NURSE' in gfx:
                respawn=heals.get(m['id'])
                welcome=dialogue.unique(f"Welcome to {place(name)}, Champion! Let's get your partners ready for the road ahead.",name,gfx)
                done=dialogue.unique(f"Your team is fully rested. Take care on your travels around {place(name)}!",name,gfx)
                out.append(label+'::\n\tlock\n\tfaceplayer\n\tmsgbox '+label+'_Welcome, MSGBOX_DEFAULT\n\tclosemessage\n\tapplymovement VAR_LAST_TALKED, Common_Movement_WalkInPlaceFasterLeft\n\twaitmovement 0\n\tdofieldeffect FLDEFF_POKECENTER_HEAL\n\twaitfieldeffect FLDEFF_POKECENTER_HEAL\n\tspecial HealPlayerParty\n'+('\tsetrespawn '+respawn+'\n' if respawn else '')+'\tapplymovement VAR_LAST_TALKED, Common_Movement_WalkInPlaceFasterDown\n\twaitmovement 0\n\tapplymovement VAR_LAST_TALKED, Movement_PkmnCenterNurse_Bow\n\twaitmovement 0\n\tmsgbox '+label+'_Done, MSGBOX_DEFAULT\n\trelease\n\tend\n\n')
                out.extend([text(label+'_Welcome',welcome),text(label+'_Done',done)])
            elif 'CLERK' in gfx:
                words=dialogue.unique(f"Welcome to {place(name)}! Stocking up before a long journey? We carry supplies for experienced Trainers.",name,gfx)
                out.append(label+'::\n\tlock\n\tfaceplayer\n\tmsgbox '+label+'_Text, MSGBOX_DEFAULT\n\tpokemart BriskKanto_PostgameStock\n\trelease\n\tend\n\n')
                out.append(text(label+'_Text',words))
            else:
                words=NAMED.get(gfx.removeprefix('OBJ_EVENT_GFX_')) or source_words(script,scripts)
                words=dialogue.unique(words,name,gfx)
                out.extend([label+'::\n\tmsgbox '+label+'_Text, MSGBOX_NPC\n\tend\n\n',text(label+'_Text',words)])
            records[str(i)]=rec
        geometry=[]
        if name=='CinnabarIsland_Gym_Frlg':
            geometry=[line.strip() for i in range(1,7) for line in scripts['CinnabarIsland_Gym_EventScript_OpenDoor'+str(i)].splitlines() if 'setmetatile' in line]
        elif name=='VermilionCity_Gym_Frlg':
            geometry=[line.strip() for line in scripts['VermilionCity_Gym_EventScript_SetBeamsOff'].splitlines() if 'setmetatile' in line]
        elif name.startswith('PokemonLeague_') and 'Room_Frlg' in name:
            geometry=['setmetatile 6, 1, METATILE_PokemonLeague_Door_Top_Open, 1','setmetatile 6, 2, METATILE_PokemonLeague_Door_Mid_Open, 0']
        elif name.startswith('PokemonMansion_') and name.endswith('_Frlg'):
            floor=name.removeprefix('PokemonMansion_').removesuffix('_Frlg')
            geometry=[line.strip() for action in ('PressSwitch','ResetSwitch')
                for line in scripts['PokemonMansion_EventScript_'+action+'_'+floor].splitlines()
                if 'setmetatile' in line and line.rstrip().endswith(', 0')]
        elif name in ('VictoryRoad_2F_Frlg','VictoryRoad_3F_Frlg'):
            raw=(ROOT/'data/maps'/name/'scripts.inc').read_text()
            geometry=[line.strip() for line in raw.splitlines() if 'setmetatile' in line and 'Floor_Ledge' in line]
        signs={}
        for i,event in enumerate(m.get('bg_events',[]),1):
            if event.get('type')!='sign':continue
            label=f'BriskKanto_{name}_Sign_{i}'
            words=source_words(event.get('script',''),scripts)
            # Direct sign scripts sometimes use message instead of msgbox.
            if not words:words=f"{place(name)} - Champion's visit. Travel safely and keep your Pokemon ready for the challenges ahead."
            out.extend([label+'::\n\tmsgbox '+label+'_Text, MSGBOX_SIGN\n\tend\n\n',text(label+'_Text',words)])
            signs[str(i)]=label
        entry={'objects':records,'signs':signs}
        if geometry:
            prefix='BriskKanto_'+name
            entry['map_scripts']=prefix
            out.append(prefix+'_MapScripts::\n\tmap_script MAP_SCRIPT_ON_LOAD, '+prefix+'_OnLoad\n\t.byte 0\n\n'+prefix+'_OnLoad::\n'+''.join('\t'+g+'\n' for g in geometry)+'\tend\n\n')
        manifest['maps'][name]=entry
    for label,words in {
        'BriskKanto_Text_TwoPokemon':'Bring at least two Pokemon for our double battle, Champion!',
        'BriskKanto_Text_NurseWelcome':'Welcome, Champion of Hoenn! Let us restore your team for the challenges ahead.',
        'BriskKanto_Text_NurseDone':'Your Pokemon are fully rested. Enjoy your journey through Kanto!',
        'BriskKanto_Text_Shop':'Welcome to Kanto, Champion! We stock supplies for experienced Trainers.',
    }.items():out.append(text(label,words))
    out.append('BriskKanto_PostgameStock:\n'+''.join('\t.2byte ITEM_'+item+'\n' for item in ('ULTRA_BALL','QUICK_BALL','DUSK_BALL','TIMER_BALL','FULL_RESTORE','MAX_POTION','REVIVE','FULL_HEAL','MAX_REPEL','ESCAPE_ROPE'))+'\tpokemartlistend\n')
    save('data/brisk_kanto_postgame.json',manifest)
    (ROOT/'data/scripts/brisk_kanto_postgame.inc').write_text(''.join(out))
    print(f'{len(metadata)-1} Kanto identities; {(len(metadata)-1)*4} mode teams; {trainer_count} placed trainer encounters; {len(manifest["maps"])} maps')


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--dialogue-only',action='store_true',help='Keep the checked-in trainer teams unchanged')
    args=parser.parse_args()
    metadata=(json.loads((ROOT/'data/brisk_kanto_postgame.json').read_text())['trainers']
              if args.dialogue_only else author_rosters())
    author_dialogue(metadata)

if __name__=='__main__': main()
