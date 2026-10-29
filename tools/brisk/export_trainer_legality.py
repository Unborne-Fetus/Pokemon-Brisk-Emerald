#!/usr/bin/env python3
"""Export evolution and learnset rules from a built Emerald ELF.

Requires pyelftools and arm-none-eabi-gcc. The snapshot uses compiled game data,
including conditional and custom evolution rules, rather than vanilla dex data.
"""
import argparse
import json
from pathlib import Path
import re
import struct
import subprocess
import tempfile
from elftools.elf.elffile import ELFFile
from trainer_rules_fingerprint import source_fingerprint

ROOT=Path(__file__).resolve().parents[2]


def names(path,prefix):
    text=(ROOT/path).read_text()
    return list(dict.fromkeys(re.findall(r'^\s*('+prefix+r'\w+)\s*(?:=|,)',text,re.M)+re.findall(r'^#define\s+('+prefix+r'\w+)\s+',text,re.M)))


def export(elf_path,compiler):
    species=names('include/constants/species.h','SPECIES_')
    moves=names('include/constants/moves.h','MOVE_')
    abilities=names('include/constants/abilities.h','ABILITY_')
    methods=names('include/constants/pokemon.h','EVO_')
    conditions=names('include/constants/pokemon.h','IF_')
    fields=['sizeof(struct SpeciesInfo)','offsetof(struct SpeciesInfo, abilities)',
        'sizeof(enum Ability)','offsetof(struct SpeciesInfo, levelUpLearnset)',
        'offsetof(struct SpeciesInfo, teachableLearnset)','offsetof(struct SpeciesInfo, eggMoveLearnset)',
        'offsetof(struct SpeciesInfo, evolutions)','offsetof(struct SpeciesInfo, formSpeciesIdTable)',
        'sizeof(struct Evolution)','offsetof(struct Evolution, targetSpecies)',
        'offsetof(struct Evolution, params)','sizeof(enum Species)',
        'sizeof(struct LevelUpMove)','offsetof(struct LevelUpMove, level)','sizeof(enum Move)']
    values=fields+species+moves+abilities+methods+conditions
    source='#include "global.h"\n#include "pokemon.h"\n#include <stddef.h>\nconst u32 layout[] = {'+','.join(values)+'};\n'
    with tempfile.TemporaryDirectory() as temp:
        src=Path(temp)/'layout.c';obj=Path(temp)/'layout.o';src.write_text(source)
        subprocess.run([compiler,'-DEMERALD','-DMODERN=1','-DTESTING=0','-mthumb','-mabi=apcs-gnu','-march=armv4t','-iquote',str(ROOT/'include'),'-I',str(ROOT),'-c',str(src),'-o',str(obj)],cwd=ROOT,check=True)
        with obj.open('rb') as stream:
            elf=ELFFile(stream);symbol=elf.get_section_by_name('.symtab').get_symbol_by_name('layout')[0]
            data=elf.get_section(symbol['st_shndx']).data()[symbol['st_value']:symbol['st_value']+symbol['st_size']]
            numbers=list(struct.unpack('<'+'I'*(len(data)//4),data))
    meta=numbers[:len(fields)];offset=len(fields);lookups=[]
    for group in (species,moves,abilities,methods,conditions):
        lookup={};
        for name,value in zip(group,numbers[offset:offset+len(group)]):lookup.setdefault(value,name)
        lookups.append(lookup);offset+=len(group)
    sn,mn,an,en,cn=lookups
    species_values=numbers[len(fields):len(fields)+len(species)]
    move_values=numbers[len(fields)+len(species):len(fields)+len(species)+len(moves)]
    size,ability_offset,ability_size,level_offset,teach_offset,egg_offset,evo_offset,form_offset,evo_size,target_offset,params_offset,species_size,level_size,lv_offset,move_size=meta
    result={}
    with Path(elf_path).open('rb') as stream:
        elf=ELFFile(stream);segments=[(s['p_vaddr'],s.data()) for s in elf.iter_segments() if s['p_filesz']]
        def read(address,count):
            for start,data in segments:
                if start<=address and address+count<=start+len(data):return data[address-start:address-start+count]
            raise ValueError(f'Unmapped ELF address {address:#x}')
        def number(address,width):return int.from_bytes(read(address,width),'little')
        def move_list(pointer):
            if not pointer:return []
            answer=[]
            for i in range(2048):
                m=number(pointer+i*2,2)
                if m==65535:return answer
                if m in mn and m:answer.append(mn[m])
            raise ValueError('Unterminated move list')
        symbol=elf.get_section_by_name('.symtab').get_symbol_by_name('gSpeciesInfo')[0]
        count=symbol['st_size']//size;base=symbol['st_value']
        for ident,name in sn.items():
            if not 0<ident<count:continue
            address=base+ident*size
            if not any(read(address,6)):continue
            levels=[];pointer=number(address+level_offset,4)
            if pointer:
                for i in range(1024):
                    m=number(pointer+i*level_size,move_size);lv=number(pointer+i*level_size+lv_offset,2)
                    if lv==65535 or m==65535:break
                    if m in mn and m:levels.append([lv,mn[m]])
            edges=[];pointer=number(address+evo_offset,4)
            if pointer:
                for i in range(128):
                    edge=pointer+i*evo_size;method=number(edge,2)
                    if method==65535:break
                    target=number(edge+target_offset,species_size)
                    conds=[];params=number(edge+params_offset,4)
                    if params:
                        for j in range(32):
                            condition,args1,args2,args3=struct.unpack('<4H',read(params+j*8,8))
                            if condition not in cn:break
                            conds.append([cn[condition],args1,args2,args3])
                    if target in sn:edges.append({'method':en.get(method,str(method)),'level':number(edge+2,2),'target':sn[target],'conditions':conds})
            forms=[];pointer=number(address+form_offset,4)
            if pointer:
                for i in range(128):
                    f=number(pointer+i*2,2)
                    if f==65535:break
                    if f in sn:forms.append(sn[f])
            result[name]={'stats':list(read(address,6)),
                'abilities':[an[a] for i in range(3) if (a:=number(address+ability_offset+i*ability_size,ability_size)) in an and a],
                'level_moves':levels,'teachable':move_list(number(address+teach_offset,4)),
                'egg_moves':move_list(number(address+egg_offset,4)),'evolutions':edges,'forms':forms}
    out={'source_fingerprint':source_fingerprint(),'description':'Compiled Brisk evolution/learnset rules; regenerate from a current Emerald ELF with export_trainer_legality.py. Evolution item values are not levels. Level-up moves, tutors/TMs and egg moves are recorded separately.',
         'move_ids':{str(k):v for k,v in mn.items()},'aliases':{name:sn[value] for name,value in zip(species,species_values) if value in sn and name!=sn[value]},'move_aliases':{name:mn[value] for name,value in zip(moves,move_values) if value in mn and name!=mn[value]},'species':result}
    path=ROOT/'tools/brisk/trainer_legality.json';path.write_text(json.dumps(out,separators=(',',':'))+'\n')
    print(f'Exported compiled rules for {len(result)} species to {path.relative_to(ROOT)}')

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('elf');parser.add_argument('--compiler',default='arm-none-eabi-gcc');args=parser.parse_args();export(args.elf,args.compiler)
