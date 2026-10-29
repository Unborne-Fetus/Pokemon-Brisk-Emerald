"""Evolution lower bounds and learnability from Brisk's compiled game rules."""
from functools import lru_cache
import json
from trainer_rules_fingerprint import source_fingerprint
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
SNAPSHOT=json.loads((ROOT/'tools/brisk/trainer_legality.json').read_text())
if SNAPSHOT.get('source_fingerprint') != source_fingerprint():
    raise ValueError('Pokemon rules changed: rebuild Emerald and regenerate trainer_legality.json with export_trainer_legality.py before auditing or authoring teams.')
SPECIES=SNAPSHOT['species']
COMPATIBLE=json.loads((ROOT/'src/data/pokemon/all_learnables.json').read_text())
ALIASES=SNAPSHOT.get('aliases',{})
MOVE_ALIASES=SNAPSHOT.get('move_aliases',{})


def canonical(sp):return ALIASES.get(sp,sp)
def canonical_move(move):return MOVE_ALIASES.get(move,move)

PARENTS={sp:[] for sp in SPECIES}
EDGES=[]
for source,data in SPECIES.items():
    for edge in data['evolutions']:
        if edge['method']=='EVO_NONE':continue
        target=canonical(edge['target'])
        if target in SPECIES:
            EDGES.append((source,target,edge))
            PARENTS[target].append(source)
# Battle forms inherit their original species' evolution requirements. The
# first entry in a form table is its base; regional forms with actual incoming
# evolution edges keep those edges instead.
for sp,data in SPECIES.items():
    if not PARENTS[sp] and data['forms'] and canonical(data['forms'][0])!=sp:
        base=canonical(data['forms'][0])
        if base in SPECIES:
            edge={'method':'FORM','level':0,'conditions':[]}
            EDGES.append((base,sp,edge));PARENTS[sp].append(base)


def ancestors(sp):
    todo=[canonical(sp)];seen=set()
    while todo:
        node=todo.pop()
        if node in seen:continue
        seen.add(node);todo.extend(PARENTS.get(node,[]))
    return seen


@lru_cache(None)
def obtainable_moves(sp, level=100):
    """TM/tutor/egg moves plus level-up moves accessible by this level.

    Pre-evolutions may pass on their moves. A future level-up move alone does
    not make an underlevel set possible; a compatible TM or egg route can.
    """
    result=set()
    for node in ancestors(sp):
        data=SPECIES.get(node,{})
        result.update(data.get('teachable',[]));result.update(data.get('egg_moves',[]))
        result.update(move for lv,move in data.get('level_moves',[]) if lv<=level)
    return result


@lru_cache(None)
def legal_moves(sp, level=100):
    """Species compatibility, including legitimate cross-generation transfers.

    Brisk's teachable generator explicitly uses any supported game's methods.
    A trainer is not limited to the TMs currently sold in the player's region.
    Evolution conditions separately use obtainable_moves, so a future level-up
    move cannot lower a move-based evolution's minimum level.
    """
    result=set(obtainable_moves(sp,level))
    future={canonical_move(move) for node in ancestors(sp) for learn_level,move in SPECIES.get(node,{}).get('level_moves',[]) if learn_level>level}
    for node in ancestors(sp):
        name=node.removeprefix('SPECIES_')
        # Battle forms share their base species' transfer compatibility.
        while name and name not in COMPATIBLE:
            name=name.rsplit('_',1)[0] if '_' in name else ''
        # Transfer compatibility does not by itself prove access to a future
        # level-up-only move. Keep known TM/egg routes from obtainable_moves.
        result.update(canonical_move(m) for m in COMPATIBLE.get(name,[]) if canonical_move(m) not in future)
    return result


@lru_cache(None)
def level_moves(sp, level):
    return list(dict.fromkeys(move for node in sorted(ancestors(sp))
        for lv,move in SPECIES.get(node,{}).get('level_moves',[]) if lv<=level))


def move_minimum(sp,move):
    move=canonical_move(move)
    for level in range(1,101):
        if move in obtainable_moves(sp,level):return level
    return 101


def edge_minimum(source,edge,parent_level):
    method=edge['method']
    required=max(1,edge['level'],parent_level+1) if method in ('EVO_LEVEL','EVO_LEVEL_BATTLE_ONLY') else parent_level
    if method=='EVO_SPLIT_FROM_EVO':
        required=max(required,min((e['level'] for e in SPECIES[source]['evolutions'] if e['method']=='EVO_LEVEL'),default=1))
    for condition,arg1,arg2,arg3 in edge['conditions']:
        if condition in ('IF_KNOWS_MOVE','IF_USED_MOVE_X_TIMES'):
            move=SNAPSHOT['move_ids'].get(str(arg1))
            if move:required=max(required,move_minimum(source,move))
    return required


MINIMUM={sp:(101 if PARENTS[sp] else 1) for sp in SPECIES}
for _ in range(32):
    changed=False
    for source,target,edge in EDGES:
        value=edge_minimum(source,edge,MINIMUM[source])
        if value<MINIMUM[target]:MINIMUM[target]=value;changed=True
    if not changed:break


def minimum_level(sp):
    node=canonical(sp)
    if node not in MINIMUM:raise ValueError(f'{sp}: no compiled species rules')
    return MINIMUM[node]


def stage_species(sp, level):
    """Retain a legal family member without inventing a level-1 fallback."""
    if minimum_level(sp)<=level:return sp
    parents=PARENTS.get(canonical(sp),[])
    if not parents:raise ValueError(f'{sp} cannot exist at level {level}')
    return stage_species(parents[0],level)
