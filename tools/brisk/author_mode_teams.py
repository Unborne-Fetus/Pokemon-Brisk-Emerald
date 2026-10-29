#!/usr/bin/env python3
"""Author explicit mode rosters from Brisk encounter data and curated archetypes.

Only this offline tool chooses species/sets. The game loads checked-in .party
records directly; it never expands or optimizes these teams during a battle.
Normal's original encounter roster remains the baseline in trainers.party.
"""
import argparse
from collections import Counter
from functools import lru_cache
import hashlib
import json
from pathlib import Path
import re

from check_trainer_progression import ROOT, TEAM_RE, LEAGUE_IDS
from trainer_mode_plan import profile_levels
from trainer_mode_archetypes import SET_ROWS, EARLY_POOLS, LATE_POOLS, BOSS_CORES, CLASS_THEMES, KAIZO_CORES

OUT = ROOT / 'src/data/brisk_mode_teams.party'
REPORT = ROOT / 'docs/brisk_authored_mode_teams.md'
MODES = ('Easy', 'Normal', 'Hard', 'Kaizo')
STATS = ('HP', 'Atk', 'Def', 'SpA', 'SpD', 'Spe')
TYPE_NAMES = ('NONE','NORMAL','FIGHTING','FLYING','POISON','GROUND','ROCK','BUG','GHOST','STEEL','MYSTERY','FIRE','WATER','GRASS','ELECTRIC','PSYCHIC','ICE','DRAGON','DARK','FAIRY')
POLICY = json.loads((ROOT / 'tools/brisk/trainer_fairness_policy.json').read_text())
LEARNABLES = json.loads((ROOT / 'src/data/pokemon/all_learnables.json').read_text())
SETS = {}
for row in SET_ROWS.strip().splitlines():
    if not row.strip(): continue
    sp, minimum, ability, item, nature, moves = row.split('|')
    SETS['SPECIES_' + sp] = dict(minimum=int(minimum), ability='ABILITY_'+ability,
        item='ITEM_'+item, nature=nature, moves=['MOVE_'+m for m in moves.split()])

RAW = {}
for path in sorted((ROOT / 'src/data/pokemon/species_info').glob('*families.h')):
    text = path.read_text()
    for sp, block in re.findall(r'\[(SPECIES_\w+)\]\s*=\s*\{(.*?)(?=\n\s*\[SPECIES_|\Z)', text, re.S):
        RAW[sp] = block

# Actual move data supplies type/category. Existing compiled fairness catalog
# takes precedence over raw conditional definitions where it has an entry.
MOVES = {}
for move, block in re.findall(r'\[(MOVE_\w+)\]\s*=\s*\{(.*?)(?=\n\s*\[MOVE_|\Z)', (ROOT/'src/data/moves_info.h').read_text(), re.S):
    power = re.findall(r'\.power\s*=\s*(\d+)', block)
    typ = re.findall(r'\.type\s*=\s*TYPE_(\w+)', block)
    cat = re.findall(r'\.category\s*=\s*DAMAGE_CATEGORY_(\w+)', block)
    if typ and cat and 'EFFECT_PLACEHOLDER' not in block:
        if not power: power=['0' if cat[-1]=='STATUS' else '40']
        MOVES[move] = dict(power=int(power[-1]), type=typ[-1], category=cat[-1])
for move, data in POLICY['moves'].items():
    MOVES[move] = dict(power=data['power'], type=TYPE_NAMES[data['type']],
                      category={0:'NONE',1:'PHYSICAL',2:'SPECIAL',3:'STATUS'}[data['category']])

PARENTS = POLICY['parents'] | POLICY['story_parents']
CATALOG = POLICY['species'] | POLICY['story_species']

@lru_cache(None)
def info(sp):
    raw = RAW.get(sp, '')
    cat = CATALOG.get(sp, {})
    stat_names = ('HP','Attack','Defense','Speed','SpAttack','SpDefense')
    stats = [int(m[1]) if (m := re.search(r'\.base'+k+r'\s*=\s*(\d+)', raw)) else 70 for k in stat_names]
    if 'stats' in cat: stats = cat['stats']
    types = re.search(r'\.types\s*=\s*MON_TYPES\(([^)]+)\)', raw)
    types = re.findall(r'TYPE_(\w+)', types[1]) if types else []
    if not types:
        types = [typ for typ, pool in LATE_POOLS.items() if sp.removeprefix('SPECIES_') in pool.split()][:2] or ['NORMAL']
    abilities = re.search(r'\.abilities\s*=\s*\{([^}]+)', raw)
    abilities = re.findall(r'ABILITY_\w+', abilities[1]) if abilities else []
    abilities = [a for a in abilities if a != 'ABILITY_NONE']
    return dict(stats=stats, types=types, abilities=abilities,
        minimum=SETS.get(sp, {}).get('minimum', POLICY['species'].get(sp, {}).get('minimum_level',1)))


def parse_mon(block):
    block=block.strip()
    first = block.splitlines()[0].split('@')[0].strip()
    named = re.findall(r'\(([^)]+)\)', first)
    species = next((x for x in named if x not in ('M','F')), None)
    species = species or re.sub(r'\s*\([MF]\)\s*$', '', first).strip()
    sp = species if species.startswith('SPECIES_') else 'SPECIES_'+re.sub(r'\W+', '_', species.upper()).strip('_')
    level = re.search(r'^Level:\s*(\d+)', block, re.M)
    return dict(species=sp, level=int(level[1]) if level else 100, original=block)


def stable(value):
    return int(hashlib.sha256(value.encode()).hexdigest()[:8],16)


@lru_cache(None)
def legal_moves(sp):
    name=sp.removeprefix('SPECIES_')
    if name in LEARNABLES: return set(LEARNABLES[name]) | set(level_moves(sp,100))
    # Battle/weather forms share the base species learnsets.
    while '_' in name:
        name=name.rsplit('_',1)[0]
        if name in LEARNABLES:return set(LEARNABLES[name]) | set(level_moves(sp,100))
    return set(level_moves(sp,100))


@lru_cache(None)
def level_moves(sp, level):
    todo, visited, result = [sp], set(), []
    while todo:
        name = todo.pop()
        if name in visited: continue
        visited.add(name)
        result += [move for lv,move in CATALOG.get(name, {}).get('learnset', []) if lv <= level]
        todo.extend(PARENTS.get(name, []))
    return list(dict.fromkeys(result))


def stage_species(sp, level):
    for _ in range(8):
        minimum = info(sp)['minimum']
        # Species outside the compiled catalog (not ordinary early candidates)
        # stay as the authored ace. Easy uses pre-evolutions when catalogued.
        if minimum <= level: return sp
        parents = PARENTS.get(sp, [])
        if not parents: return sp
        sp = parents[0]
    return sp


def theme(tid, header, original):
    for boss in BOSS_CORES:
        if tid == 'TRAINER_'+boss or tid.startswith('TRAINER_'+boss+'_'):
            return {'ROXANNE':'ROCK','BRAWLY':'FIGHTING','WATTSON':'ELECTRIC','FLANNERY':'FIRE','NORMAN':'NORMAL','WINONA':'FLYING','TATE_AND_LIZA':'PSYCHIC','JUAN':'WATER','SIDNEY':'DARK','PHOEBE':'GHOST','GLACIA':'ICE','DRAKE':'DRAGON','WALLACE':'WATER','STEVEN':'STEEL'}[boss]
    cls = re.search(r'^Class:\s*(.*)',header,re.M)
    if cls and cls[1] in CLASS_THEMES: return CLASS_THEMES[cls[1]]
    counts = Counter(t for m in original for t in dict.fromkeys(info(m['species'])['types']))
    return counts.most_common(1)[0][0] if counts else 'NORMAL'


def boss_core(tid, mode):
    for boss, cores in BOSS_CORES.items():
        if tid == 'TRAINER_'+boss or tid.startswith('TRAINER_'+boss+'_'):
            return ['SPECIES_'+s for s in cores[{'Easy':0,'Hard':1,'Kaizo':2}.get(mode,1)].split()]
    return []


def candidates(typ, level, mode, seed, core=()):
    early = ['SPECIES_'+s for s in EARLY_POOLS[typ].split()]
    late = ['SPECIES_'+s for s in LATE_POOLS[typ].split()]
    # A higher tier favors the coordinated role sets; Easy prefers familiar
    # early species until ordinary evolution levels, even in late-game areas.
    pools = [early] if mode == 'Easy' and level < 28 else [late, early]
    eligible=[]
    for pool in pools:
        group=[sp for sp in pool if info(sp)['minimum'] <= level and legal_moves(sp)]
        if group:
            offset=seed % len(group)
            eligible += group[offset:]+group[:offset]
    if not eligible: eligible = [sp for sp in early if legal_moves(sp)]
    return list(dict.fromkeys([sp for sp in core if info(sp)['minimum'] <= level and legal_moves(sp)] + eligible))


def select_moves(sp, level, mode, role, doubles):
    stats = info(sp)['stats']; physical = stats[1] >= stats[4]
    nature=SETS.get(sp,{}).get('nature')
    if nature in ('Modest','Timid','Quiet'):physical=False
    elif nature in ('Adamant','Jolly','Brave'):physical=True
    legal = legal_moves(sp)
    if mode == 'Easy':
        pool = level_moves(sp,level)
        # Complete catalog fallback for species outside the stage snapshot.
        if not pool: pool = sorted(legal)
        ceiling = 60 if level < 12 else 65 if level < 20 else 85 if level < 30 else 100
    else:
        pool = sorted(legal)
        ceiling = 60 if level < 12 else 75 if level < 20 else 90 if level < 30 else 150 if level>=40 and mode in ('Hard','Kaizo') else 120
    pool = [m for m in pool if m in MOVES and MOVES[m]['power'] <= ceiling]
    types = info(sp)['types']
    attacks = [m for m in pool if MOVES[m]['category'] != 'STATUS' and MOVES[m]['power'] > 1
               and m not in ('MOVE_SELF_DESTRUCT','MOVE_EXPLOSION','MOVE_DREAM_EATER','MOVE_SNORE','MOVE_LAST_RESORT','MOVE_BELCH','MOVE_FOCUS_PUNCH','MOVE_SYNCHRONOISE','MOVE_SOLAR_BEAM','MOVE_SOLAR_BLADE')]
    def score(m):
        d=MOVES[m]
        return d['power'] * (1.5 if d['type'] in types else 1) * (1 if (d['category']=='PHYSICAL') == physical else .55)
    attacks.sort(key=lambda m:(-score(m),m))
    if not attacks:
        # A catalog's Teleport-only Abra is unsuitable as a standalone team.
        raise ValueError(f'{sp} at {level} has no usable attack in {mode}')
    if mode == 'Easy':
        picked=[]
        for m in reversed(pool):
            if m in attacks and m not in picked: picked.append(m)
            if len(picked)==3: break
        if not picked: picked=[attacks[0]]
        support=next((m for m in reversed(pool) if MOVES[m]['category']=='STATUS' and m not in ('MOVE_SPLASH','MOVE_TELEPORT')),None)
        if support: picked.append(support)
        return picked[:4]
    preferred = SETS.get(sp,{}).get('moves',[])
    picked=[m for m in preferred if m in pool]
    # Normal has milder role sets; Hard/Kaizo keep curated support together.
    if mode == 'Normal': picked=[m for m in picked if m not in ('MOVE_SPORE','MOVE_QUIVER_DANCE','MOVE_DRAGON_DANCE')]
    if not any(m in attacks for m in picked): picked= [attacks[0]] + picked[:3]
    seen_types={MOVES[m]['type'] for m in picked if m in attacks}
    for m in attacks:
        if len(picked)>=4: break
        if m not in picked and MOVES[m]['type'] not in seen_types:
            picked.append(m);seen_types.add(MOVES[m]['type'])
    if len(picked)<4 and mode in ('Hard','Kaizo'):
        for m in ('MOVE_PROTECT','MOVE_TAILWIND','MOVE_THUNDER_WAVE','MOVE_HELPING_HAND','MOVE_RECOVER','MOVE_ROOST'):
            if m in pool and m not in picked:
                picked.append(m)
                if len(picked)==4:break
    for m in attacks:
        if len(picked)>=4:break
        if m not in picked:picked.append(m)
    # Earthquake + an unprotected grounded partner is unnecessary friendly fire.
    if doubles:
        replacements={
            'MOVE_EARTHQUAKE':('MOVE_HIGH_HORSEPOWER','MOVE_DRILL_RUN','MOVE_EARTH_POWER','MOVE_STOMPING_TANTRUM','MOVE_DIG'),
            'MOVE_SURF':('MOVE_MUDDY_WATER','MOVE_SCALD','MOVE_HYDRO_PUMP','MOVE_WATER_PULSE'),
            'MOVE_DISCHARGE':('MOVE_THUNDERBOLT','MOVE_THUNDER','MOVE_SHOCK_WAVE'),
            'MOVE_SLUDGE_WAVE':('MOVE_SLUDGE_BOMB','MOVE_ACID_SPRAY'),
            'MOVE_LAVA_PLUME':('MOVE_HEAT_WAVE','MOVE_FLAMETHROWER','MOVE_FIRE_BLAST'),
            'MOVE_BOOMBURST':('MOVE_HYPER_VOICE','MOVE_TRI_ATTACK'),
        }
        for move,options in replacements.items():
            if move in picked:
                replace=next((m for m in options if m in pool and m not in picked),None)
                if replace:picked[picked.index(move)]=replace
    return list(dict.fromkeys(picked))[:4]


def mon_block(sp, level, mode, slot, doubles, original=None, train_original=False):
    # Keep baseline personalities/nicknames/gimmicks on dedicated Normal sets.
    if original is not None:
        block=original.strip()
        block=re.sub(r'^Level:.*\n?', '',block,flags=re.M)
        if train_original:
            floor=16 if level<20 else 20 if level<35 else 24 if level<50 else 31
            old_ivs=re.search(r'^IVs: (.*)',block,re.M)
            ivs={s:31 for s in STATS}
            if old_ivs:ivs.update({s:max(floor,int(n)) for n,s in re.findall(r'(\d+) (HP|Atk|Def|SpA|SpD|Spe)',old_ivs[1])})
            old_evs=re.search(r'^EVs: (.*)',block,re.M)
            total=sum(map(int,re.findall(r'\d+',old_evs[1]))) if old_evs else 0
            budget=max(total,0 if level<15 else 64 if level<25 else 128 if level<40 else 256 if level<60 else 384)
            moves=re.findall(r'^- (MOVE_\w+)',block,re.M)
            scores=dict(zip(('HP','Atk','Def','Spe','SpA','SpD'),info(sp)['stats']))
            if not any(MOVES.get(m,{}).get('category')=='PHYSICAL' for m in moves):scores['Atk']=0
            if not any(MOVES.get(m,{}).get('category')=='SPECIAL' for m in moves):scores['SpA']=0
            if 'MOVE_TRICK_ROOM' in moves or 'MOVE_GYRO_BALL' in moves:scores['Spe']=0
            ranked=sorted(scores,key=lambda s:(-scores[s],STATS.index(s)))
            evs={ranked[0]:min(252,budget//2),ranked[1]:min(252,budget//2)}
            block=re.sub(r'^(IVs|EVs):.*\n?', '',block,flags=re.M)
            lines=block.splitlines()
            block='\n'.join(lines[:1]+['IVs: '+' / '.join(f'{n} {s}' for s,n in ivs.items()),'EVs: '+' / '.join(f'{n} {s}' for s,n in evs.items())]+lines[1:])
        return block.splitlines()[0]+'\nLevel: '+str(level)+'\n'+'\n'.join(block.splitlines()[1:])
    data=info(sp); stats=data['stats']; template=SETS.get(sp,{})
    moves=select_moves(sp,level,mode,slot,doubles)
    physical=sum(MOVES[m]['power'] for m in moves if MOVES[m]['category']=='PHYSICAL') >= sum(MOVES[m]['power'] for m in moves if MOVES[m]['category']=='SPECIAL')
    if template.get('nature') in ('Modest','Timid','Quiet'):physical=False
    elif template.get('nature') in ('Adamant','Jolly','Brave'):physical=True
    attack='Atk' if physical else 'SpA'
    nature='Hardy' if mode=='Easy' else template.get('nature','Adamant' if physical else 'Modest')
    iv=0 if mode=='Easy' else (16 if level<25 else 24 if level<50 else 31) if mode=='Normal' else 31
    total=0 if mode=='Easy' else (0 if level<15 else 128 if level<30 else 256 if level<50 else 384) if mode=='Normal' else (256 if level<20 else 384 if level<35 else 510) if mode=='Hard' else 510
    investment='HP' if stats[3]<70 or nature in ('Quiet','Brave','Sassy','Relaxed') else 'Spe'
    if total==510: evs={attack:252,investment:252,('SpD' if physical else 'Def'):4}
    else: evs={attack:total//2,investment:total-total//2} if total else {}
    item=None
    if mode=='Normal' and level>=12:item='ITEM_ORAN_BERRY' if level<35 else 'ITEM_SITRUS_BERRY'
    if mode in ('Hard','Kaizo'):
        item=template.get('item','ITEM_EVIOLITE' if PARENTS and sp in {p for ps in PARENTS.values() for p in ps} else 'ITEM_SITRUS_BERRY')
        if level<20:item='ITEM_ORAN_BERRY' if mode=='Hard' else 'ITEM_BERRY_JUICE'
        # Never choice-lock a support set, even after a legality fallback.
        if item.startswith('ITEM_CHOICE_') and any(MOVES[m]['category']=='STATUS' for m in moves):item='ITEM_LIFE_ORB'
        if item=='ITEM_ASSAULT_VEST' and any(MOVES[m]['category']=='STATUS' for m in moves):item='ITEM_SITRUS_BERRY'
        if item in ('ITEM_FLAME_ORB','ITEM_TOXIC_ORB') and template.get('ability') not in ('ABILITY_GUTS','ABILITY_POISON_HEAL','ABILITY_QUICK_FEET'):item='ITEM_SITRUS_BERRY'
        if item=='ITEM_THROAT_SPRAY' and not any(m in moves for m in ('MOVE_HYPER_VOICE','MOVE_CLANGING_SCALES','MOVE_BUG_BUZZ')):item='ITEM_LIFE_ORB'
    defensive={
        'SPECIES_INDEEDEE_F':{'HP':252,'Def':196,'SpD':60},
        'SPECIES_AMOONGUSS':{'HP':252,'Def':156,'SpD':100},
        'SPECIES_DUSCLOPS':{'HP':252,'Def':156,'SpD':100},
        'SPECIES_BRONZONG':{'HP':252,'Def':156,'SpD':100},
        'SPECIES_GRIMMSNARL':{'HP':252,'Def':4,'SpD':252},
        'SPECIES_MURKROW':{'HP':252,'Def':4,'SpD':252},
        'SPECIES_CORVIKNIGHT':{'HP':252,'Def':160,'SpD':96},
        'SPECIES_MILOTIC':{'HP':252,'Def':196,'SpD':60},
        'SPECIES_GASTRODON':{'HP':252,'Def':100,'SpD':156},
    }
    if total and sp in defensive:
        evs={stat:(amount*total//510)//4*4 for stat,amount in defensive[sp].items()}
    lines=[sp+(' @ '+item if item else ''),'Level: '+str(level),
        'IVs: '+' / '.join(f'{0 if mode!="Easy" and s=="Spe" and nature in ("Quiet","Brave","Sassy","Relaxed") else iv} {s}' for s in STATS),'EVs: '+' / '.join(f'{v} {s}' for s,v in evs.items()) if evs else 'EVs: 0 HP','Nature: '+nature]
    abilities=data['abilities']
    ability=template.get('ability')
    if ability and ability in abilities:lines.append('Ability: '+ability)
    elif abilities:lines.append('Ability: '+abilities[0])
    if mode=='Kaizo' and slot=='ace' and level>=40:lines.append('Tera Type: '+data['types'][0])
    return '\n'.join(lines+['- '+m for m in moves])


def render():
    sources=TEAM_RE.findall((ROOT/'src/data/trainers.party').read_text())
    output=['#include "src/data/brisk_kanto_teams.party"\n', '/* Explicit Brisk mode rosters. Edit these records to tune an encounter.\n'
            'Regeneration from curated archetypes: python tools/brisk/author_mode_teams.py --write\n'
            'Validation only (preserves manual edits): python tools/brisk/author_mode_teams.py --check\n*/\n']
    report=['# Brisk Emerald: explicit trainer rosters', '',
        'Every populated Hoenn trainer entry has an explicit roster for each mode. The game',
        'loads these records directly; battle-time reserve selection and set optimization',
        'are disabled for authored parties. Normal retains the original species as its',
        'baseline; the other modes use different themed rosters and curated role cores.', '',
        'All dedication/Easter egg trainers keep their original complete party in every',
        'mode, including levels, moves, items, training, nicknames and shiny settings.', '',
        'Opening Route 103 starter tutorials retain one starter. Metadata/Frontier identity',
        'templates are copied rather than rebalanced. Hooded Figure and Tankerson remain',
        'Kaizo-only; unreachable mode records preserve their original challenge sets.',
        'League members are 70 on Easy and 100 otherwise; Meteor Falls Steven is always 100.',
        'Encounter levels are fixed in these records, independent of the current badge count.',
        'Two-opponent half-team formats may use only three members from each trainer.', '',
        '| Trainer | Easy | Normal | Hard | Kaizo |', '| --- | --- | --- | --- | --- |']
    for tid,source in sources:
        blocks=re.split(r'\n[ \t]*\n', source.strip())
        header=blocks[0]
        original=[parse_mon(b) for b in blocks[1:] if b.strip() and not b.strip().startswith('/*')]
        if not original:
            for mode in MODES:output.append(f'=== {tid} ===\n'+header+'\nDifficulty: '+mode+'\n\n')
            continue
        role='Kaizo only' if tid in POLICY['kaizo_bosses'] else 'Protected' if tid in POLICY['protected'] else 'Story' if tid in POLICY['story'] else 'Metadata' if tid in POLICY['metadata_only'] else 'Ordinary'
        if role=='Protected':
            # Copy the original party verbatim. No archetype selection, reserve
            # slots, level changes or set/training rewrites apply in any mode.
            party='\n\n'.join('\n'.join(line.rstrip() for line in mon['original'].splitlines()) for mon in original)
            cell=', '.join(mon['species'].removeprefix('SPECIES_').replace('_',' ').title()+' '+str(mon['level']) for mon in original)
            for mode in MODES:
                output.append(f'=== {tid} ===\n'+header+'\nDifficulty: '+mode+'\nAuthored Party: Yes\n\n'+party+'\n\n')
            report.append('| `'+tid+'` | '+' | '.join([cell]*len(MODES))+' |')
            continue
        typ=theme(tid,header,original)
        doubles='Double Battle: Yes' in header or 'Battle Type: Doubles' in header
        cells=[]
        previous_signatures=[]
        for mode in MODES:
            tutorial='_ROUTE_103_' in tid
            untouched=role in ('Metadata','Kaizo only') or (mode=='Normal' and (role=='Protected' or tid in LEAGUE_IDS or tid=='TRAINER_STEVEN'))
            levels=profile_levels(tid,role,[m['level'] for m in original],mode)
            if not levels:levels=[100]*len(original)
            if role=='Protected' and mode!='Normal' and not tutorial:levels=levels+([max(1,levels[-1]-1)]*(max(0, (4 if mode=='Easy' else 6)-len(levels))))
            if untouched: levels=[m['level'] for m in original]
            # The tutorial is deliberately the only playable repeated species roster.
            if tutorial:levels=levels[:1]
            count=len(levels)
            if mode in ('Hard','Kaizo') and not untouched and tid not in LEAGUE_IDS and tid!='TRAINER_STEVEN':
                # Bake the encounter's intended badge band into the roster.
                # Replaying an early route after later badges must not change it.
                highest=max(m['level'] for m in original)
                cap=next(n for n in (25,35,45,50,55,65,75,90,100) if n>=highest)
                raw=[m['level'] for m in original]
                if count>len(raw):raw=raw[:-1]+[max(1,highest-1-i%2) for i in range(count-len(raw))]+raw[-1:]
                levels=[min(lv,max(1,cap-(highest-raw[min(i,len(raw)-1)]))) for i,lv in enumerate(levels)]
            chosen=[]
            core=boss_core(tid,mode) if mode!='Normal' else []
            if mode=='Kaizo' and not core and min(levels)>=30 and role not in ('Metadata','Kaizo only'):
                ace=stage_species(original[-1]['species'],levels[-1])
                core=[s for name in KAIZO_CORES[typ].split() if (s:='SPECIES_'+name)!=ace]+[ace]
            seed=stable(tid+mode)
            for i,level in enumerate(levels):
                slot='ace' if i==count-1 else 'support'
                original_index=(len(original)-1 if i==count-1 else i)
                keep=(untouched or tutorial or (mode=='Normal' and (i<len(original)-1 or i==count-1)))
                if keep:
                    old=original[min(original_index,len(original)-1)]
                    chosen.append((old['species'],mon_block(old['species'],level,mode,slot,doubles,old['original'],train_original=mode=='Normal' and not untouched and not tutorial)))
                    continue
                # Retain an ace's family for ordinary/rival/dedication identity.
                if slot=='ace' and core:
                    eligible=[s for s in core if info(s)['minimum']<=level and legal_moves(s)]
                    sp=eligible[-1] if eligible else stage_species(original[-1]['species'],level)
                elif slot=='ace' and not core:
                    sp=stage_species(original[-1]['species'],level)
                else:
                    pool=candidates(typ,level,mode,seed+i,core)
                    used={s for s,b in chosen}
                    # Core leads stay first. Supports avoid repeating a species.
                    ace=next((s for s in reversed(core) if info(s)['minimum']<=levels[-1] and legal_moves(s)),None) if core else stage_species(original[-1]['species'],levels[-1])
                    sp=next((s for s in pool if s not in used and s!=ace),pool[0])
                try:block=mon_block(sp,level,mode,slot,doubles,original[-1]['original'] if slot=='ace' and not legal_moves(sp) else None)
                except ValueError:
                    sp=next(s for s in candidates(typ,level,mode,seed+i) if any(MOVES.get(m,{}).get('power',0)>1 for m in level_moves(s,level)))
                    block=mon_block(sp,level,mode,slot,doubles)
                chosen.append((sp,block))
            # Guarantee different species compositions for non-tutorial, playable
            # variants, even when original parties have six members already.
            if not untouched and not tutorial and mode!='Normal':
                normal_species=[m['species'] for m in original]
                pool=candidates(typ,levels[0],mode,seed+17)
                if Counter(s for s,b in chosen)==Counter(normal_species) or Counter(s for s,b in chosen) in previous_signatures:
                    # Preserve the core lead whenever possible; change a support.
                    for index in range(len(chosen)-2, -1, -1):
                        for replacement in candidates(typ,levels[index],mode,seed+17):
                            others=[s for n,(s,b) in enumerate(chosen) if n!=index]
                            signature=Counter([replacement]+others)
                            if signature not in previous_signatures and signature!=Counter(normal_species) and replacement not in others:
                                try: candidate_block=mon_block(replacement,levels[index],mode,'support',doubles)
                                except ValueError: continue
                                chosen[index]=(replacement,candidate_block);break
                        if Counter(s for s,b in chosen) not in previous_signatures:break
            previous_signatures.append(Counter(s for s,b in chosen))
            mode_header=re.sub(r'^Difficulty:.*\n?', '',header,flags=re.M)+'\nDifficulty: '+mode+'\nAuthored Party: Yes'
            output.append(f'=== {tid} ===\n'+mode_header+'\n\n'+'\n\n'.join(b for s,b in chosen)+'\n\n')
            cells.append(', '.join(s.removeprefix('SPECIES_').replace('_',' ').title()+' '+str(lv) for (s,b),lv in zip(chosen,levels)))
        report.append('| `'+tid+'` | '+' | '.join(cells)+' |')
    return '\n'.join(output), '\n'.join(report)+'\n'


def audit(source):
    """Check actual checked-in records; no generator equality requirement."""
    errors=[];records={};base=dict(TEAM_RE.findall((ROOT/'src/data/trainers.party').read_text()))
    for tid,body in TEAM_RE.findall(source):
        mode_match=re.search(r'^Difficulty: (\w+)$',body,re.M)
        if not mode_match:errors.append(f'{tid}: no mode');continue
        mode=mode_match[1];key=(tid,mode)
        if key in records:errors.append(f'{key}: duplicate')
        blocks=re.split(r'\n\s*\n',body.strip());mons=[parse_mon(b) for b in blocks[1:] if b.strip()]
        records[key]=mons
        if tid not in base or mode not in MODES:errors.append(f'{key}: unknown entry')
        if tid!='TRAINER_NONE' and not 1<=len(mons)<=6:errors.append(f'{key}: invalid party size')
        if mode!='Normal' and tid not in POLICY['protected'] and tid not in POLICY['metadata_only'] and tid not in POLICY['kaizo_bosses'] and len({m['species'] for m in mons})!=len(mons):
            errors.append(f'{key}: duplicate species in mode roster')
        for mon in mons:
            sp=mon['species'];block=mon['original'];lv=mon['level']
            if not 1<=lv<=100:errors.append(f'{key}: bad level {lv}')
            if tid in LEAGUE_IDS and lv!=(70 if mode=='Easy' else 100):errors.append(f'{key}: League level changed')
            if tid=='TRAINER_STEVEN' and lv!=100:errors.append(f'{key}: Steven level changed')
            moves=re.findall(r'^- (MOVE_\w+)$',block,re.M)
            # Old dedication sets use display names; trainerproc validates those.
            if len(set(moves))!=len(moves):errors.append(f'{key}: duplicate move on {sp}')
            if tid not in POLICY['protected'] and tid not in POLICY['metadata_only'] and tid not in POLICY['kaizo_bosses']:
                for move in moves:
                    if legal_moves(sp) and move not in legal_moves(sp):errors.append(f'{key}: {sp} cannot learn {move}')
            ev=re.search(r'^EVs: (.*)',block,re.M)
            if ev and tid not in POLICY['protected']:
                evs=[int(v) for v in re.findall(r'\d+',ev[1])]
                if sum(evs)>510 or max(evs,default=0)>252:errors.append(f'{key}: invalid EVs')
            ability=re.search(r'^Ability: (ABILITY_\w+)',block,re.M)
            if ability and info(sp)['abilities'] and ability[1] not in info(sp)['abilities']:errors.append(f'{key}: invalid ability {ability[1]} for {sp}')
    expected={(tid,mode) for tid in base for mode in MODES}
    if set(records)!=expected:errors.append(f'Coverage mismatch: missing={expected-records.keys()}, extra={records.keys()-expected}')
    for tid in base:
        baseline=[parse_mon(b) for b in re.split(r'\n[ \t]*\n',base[tid].strip())[1:] if b.strip() and not b.strip().startswith('/*')]
        if tid in POLICY['protected']:
            for mode in MODES:
                if [[line.rstrip() for line in m['original'].splitlines()] for m in records.get((tid,mode),[])] != [[line.rstrip() for line in m['original'].splitlines()] for m in baseline]:
                    errors.append(f'{tid}: dedication/Easter egg party changed in {mode}')
            continue
        actual=iter(records.get((tid,'Normal'),[]))
        for mon in baseline:
            if not any(m['species']==mon['species'] and m['level']==mon['level'] for m in actual):
                errors.append(f'{tid}: Normal baseline species/level was lost: {mon["species"]} {mon["level"]}')
                break
        if tid in POLICY['metadata_only'] or tid in POLICY['kaizo_bosses'] or '_ROUTE_103_' in tid:continue
        signatures=[Counter(m['species'] for m in records.get((tid,mode),[])) for mode in MODES]
        for i in range(4):
            for j in range(i):
                if signatures[i]==signatures[j]:errors.append(f'{tid}: same composition in {MODES[i]} and {MODES[j]}')
    return errors,records


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--write',action='store_true',help='replace static rosters with archetype output')
    parser.add_argument('--check',action='store_true',help='audit current static rosters without changing them')
    args=parser.parse_args()
    if args.write:
        output,report=render();OUT.write_text(output);REPORT.write_text(report)
    if not OUT.exists():raise SystemExit('Run with --write to create explicit rosters')
    errors,records=audit(OUT.read_text())
    for error in errors[:100]:print(error)
    print(f'{len(records)} explicit mode entries; {sum(len(m) for m in records.values())} Pokemon; {len(errors)} errors')
    raise SystemExit(bool(errors))

if __name__=='__main__':main()
