#!/usr/bin/env python3
"""Repair illegal stages and moves while preserving authored levels and identities.

Only broken sets and overly strong early Easy species are touched. The ordinary
level curve, party sizes, nicknames and cosmetic fields remain authored data.
"""
from collections import Counter
import json
import re
import author_mode_teams as author
import trainer_legality as rules

ROOT=author.ROOT


def easy_stat_cap(level):
    return 400 if level<16 else 500 if level<25 else 550 if level<35 else 700


def replace_species(block,sp):
    lines=block.splitlines();first=lines[0]
    named=[m for m in re.finditer(r'\(([^)]+)\)',first) if m[1] not in ('M','F')]
    if named:
        m=named[0];first=first[:m.start(1)]+sp+first[m.end(1):]
    else:
        gender=re.search(r'\([MF]\)',first.partition('@')[0])
        first=sp+(' '+gender[0] if gender else '')+(' @ '+first.partition('@')[2].strip() if '@' in first else '')
    lines[0]=first
    return '\n'.join(lines)


def balanced_stage(sp,level,mode,seed):
    stage=rules.stage_species(sp,level)
    if mode!='Easy':return stage
    cap=easy_stat_cap(level)
    while sum(rules.SPECIES[rules.canonical(stage)]['stats'])>cap:
        parents=rules.PARENTS.get(rules.canonical(stage),[])
        if not parents:break
        stage=rules.stage_species(parents[0],level)
    if sum(rules.SPECIES[rules.canonical(stage)]['stats'])<=cap:return stage
    typ=author.info(sp)['types'][0]
    eligible=[s for s in author.candidates(typ,level,mode,seed) if rules.minimum_level(s)<=level and sum(rules.SPECIES[rules.canonical(s)]['stats'])<=cap]
    if not eligible:raise ValueError(f'{sp} at {level}: no balanced Easy family or themed substitute')
    return eligible[0]


def repair_mon(block,mode,tid,index):
    mon=author.parse_mon(block);sp=mon['species'];level=mon['level']
    # Dedications retain the same set across modes, but obey the same actual
    # evolution and learnability requirements as every other battle.
    effective='Normal' if tid in author.POLICY['protected'] or tid in author.POLICY['kaizo_bosses'] else mode
    if mode=='Easy' and '_ROUTE_103_' in tid:
        block=re.sub(r'^IVs:.*$', 'IVs: 0 HP / 0 Atk / 0 Def / 0 SpA / 0 SpD / 0 Spe',block,flags=re.M)
        block=re.sub(r'^EVs:.*$', 'EVs: 0 HP',block,flags=re.M)
    new=balanced_stage(sp,level,effective,author.stable(tid)+index)
    changed=new!=sp
    if changed:block=replace_species(block,new)
    info=rules.SPECIES[rules.canonical(new)]
    ability=re.search(r'^Ability:\s*(.+)',block,re.M)
    ability_name=(ability[1] if ability and ability[1].startswith('ABILITY_') else 'ABILITY_'+re.sub(r'\W+','_',ability[1].upper()).strip('_') if ability else None)
    if ability and ability_name not in info['abilities']:
        block=block[:ability.start(1)]+info['abilities'][0]+block[ability.end(1):]
    legal=rules.legal_moves(new,level)
    old=[]
    for move in re.findall(r'^-\s*(.+)$',block,re.M):
        normalized=move if move.startswith('MOVE_') else 'MOVE_'+re.sub(r'\W+','_',move.upper()).strip('_')
        old.append(rules.canonical_move(normalized))
    ceiling=60 if level<12 else 65 if level<20 else 85 if level<30 else 100 if level<50 else 150
    power_limited=effective=='Easy'
    valid=[m for m in old if m in legal and m in author.MOVES and (not power_limited or author.MOVES[m]['power']<=ceiling)]
    valid=list(dict.fromkeys(valid))
    if changed or len(valid)!=len(old):
        preferred=author.select_moves(new,level,effective,'ace' if index==5 else 'support',False)
        picked=[] if changed else valid
        for move in preferred:
            if move not in picked:picked.append(move)
            if len(picked)>=min(4,max(1,len(old))):break
        picked=picked[:4]
        if not any(author.MOVES[m]['power']>1 for m in picked):
            attack=next(m for m in preferred if author.MOVES[m]['power']>1)
            picked=([attack]+picked)[:4]
        block=re.sub(r'^-\s*.+\n?','',block,flags=re.M).rstrip()+'\n'+'\n'.join('- '+m for m in picked)
    return block


def repair_file(path,mode_default='Normal'):
    source=path.read_text();result=[];changes=0
    for match in author.TEAM_RE.finditer(source):
        tid,body=match.groups()
        if tid in author.POLICY['metadata_only'] or tid=='TRAINER_NONE':continue
        mode_match=re.search(r'^Difficulty: (\w+)',body,re.M);mode=mode_match[1] if mode_match else mode_default
        parts=re.split(r'(\n[ \t]*\n)',body)
        mon_index=0;changed=False
        for i in range(2,len(parts),2):
            block=parts[i]
            core=block.strip()
            if not core or core.startswith('/*'):continue
            fixed=repair_mon(core,mode,tid,mon_index);mon_index+=1
            if fixed!=core:
                leading=block[:len(block)-len(block.lstrip())]
                trailing=block[len(block.rstrip()):]
                parts[i]=leading+fixed+trailing
                changes+=1;changed=True
        if changed:result.append((match.start(2),match.end(2),''.join(parts)))
    for start,end,body in reversed(result):source=source[:start]+body+source[end:]
    path.write_text(source)
    print(f'{path.relative_to(ROOT)}: repaired {changes} sets')


def main():
    repair_file(ROOT/'src/data/trainers.party')
    repair_file(ROOT/'src/data/brisk_mode_teams.party')
    repair_file(ROOT/'src/data/brisk_kanto_teams.party')

if __name__=='__main__':main()
