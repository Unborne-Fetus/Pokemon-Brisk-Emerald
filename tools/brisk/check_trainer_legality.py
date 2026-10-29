#!/usr/bin/env python3
"""Audit every fixed Hoenn/Kanto team for evolution legality and Easy balance.

Frontier identity templates are excluded: facilities generate their own parties.
Named dedication teams retain their difficulty exemption, but not an exemption
from evolution, ability or species/move compatibility checks.
"""
from collections import Counter
import re
import sys
import author_mode_teams as author
import trainer_legality as rules
from repair_trainer_legality import easy_stat_cap


def normalize(value,prefix):
    return value if value.startswith(prefix) else prefix+re.sub(r'\W+','_',value.upper()).strip('_')


def audit():
    errors=[];counts=Counter();entries=0
    for path in ('src/data/trainers.party','src/data/brisk_mode_teams.party','src/data/brisk_kanto_teams.party'):
        for tid,body in author.TEAM_RE.findall((author.ROOT/path).read_text()):
            if tid=='TRAINER_NONE' or tid in author.POLICY['metadata_only']:continue
            mode_match=re.search(r'^Difficulty:\s*(\w+)',body,re.M)
            mode=mode_match[1] if mode_match else 'Normal'
            blocks=re.split(r'\n[ \t]*\n',body.strip())[1:];entries+=1
            for block in blocks:
                if not block.strip() or block.lstrip().startswith('/*'):continue
                mon=author.parse_mon(block);sp=mon['species'];lv=mon['level'];key=f'{tid}/{mode}/{sp}'
                if rules.canonical(sp) not in rules.SPECIES:
                    errors.append(key+': missing compiled species rules');continue
                if lv<rules.minimum_level(sp):errors.append(key+f': level {lv} is below evolution minimum {rules.minimum_level(sp)}')
                data=rules.SPECIES[rules.canonical(sp)]
                ability=re.search(r'^Ability:\s*(.+)',block,re.M)
                if ability and normalize(ability[1],'ABILITY_') not in data['abilities']:errors.append(key+': incompatible ability '+ability[1])
                moves=[rules.canonical_move(normalize(m,'MOVE_')) for m in re.findall(r'^-\s*(.+)',block,re.M)]
                if not moves:
                    moves=list(dict.fromkeys(move for learn_level,move in data['level_moves'] if learn_level<=lv))[-4:]
                if not 1<=len(moves)<=4 or len(moves)!=len(set(moves)):errors.append(key+': invalid or duplicate move slots')
                legal=rules.legal_moves(sp,lv)
                for move in moves:
                    if move not in legal:errors.append(key+': incompatible move '+move)
                if not any(author.MOVES.get(m,{}).get('power',0)>1 for m in moves):errors.append(key+': no usable damaging attack')
                if mode=='Easy' and tid not in author.POLICY['protected'] and tid not in author.POLICY['kaizo_bosses']:
                    if sum(data['stats'])>easy_stat_cap(lv):errors.append(key+': too much early Easy stat power')
                    if any(int(n) for n in re.findall(r'(\d+)\s+(?:HP|Atk|Def|SpA|SpD|Spe)',re.search(r'^IVs:\s*(.*)',block,re.M)[1] if re.search(r'^IVs:\s*(.*)',block,re.M) else '')):errors.append(key+': nonzero Easy IVs')
                counts[mode]+=1
    for error in errors[:80]:print(error)
    print(f'Legality audit: {entries} fixed team records, {sum(counts.values())} Pokemon; {len(errors)} errors. {dict(counts)}')
    return bool(errors)

if __name__=='__main__':sys.exit(audit())
