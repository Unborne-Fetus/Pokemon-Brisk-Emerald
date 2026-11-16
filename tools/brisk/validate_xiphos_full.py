#!/usr/bin/env python3
"""Validate the additional Xiphos five-mode roster bank."""
from pathlib import Path
import re
R=Path(__file__).resolve().parents[2]
ranks=["AGENT_A","AGENT_B","AGENT_C","AGENT_D","AGENT_E","AGENT_F","AGENT_G","SCIENTIST_A","SCIENTIST_B","SENIOR_A","SENIOR_B","JESSIE"]
modes=["Normal","Easy","Hard","Kaizo","Expert"]
bank=(R/'src/data/brisk_xiphos_extended_teams.h').read_text()
meta=(R/'src/data/brisk_xiphos_trainer_info.h').read_text()
for rank in ranks:
    stem='sXiphos'+''.join(s.capitalize() for s in rank.lower().split('_'))
    sigs=set()
    for mode in modes:
        symbol=stem+mode
        marker='const struct TrainerMon '+symbol+'[PARTY_SIZE] = {'
        assert bank.count(marker)==1,(rank,mode,'missing')
        body=bank.split(marker,1)[1].split('\n};',1)[0]
        mons=re.findall(r'\.species=(SPECIES_[A-Z0-9_]+)',body)
        assert len(mons)==6 and len(set(mons))==6,(rank,mode)
        assert body.count('.moves={')==6
        assert tuple(mons) not in sigs,(rank,mode,'duplicate')
        sigs.add(tuple(mons))
        if mode!='Expert':
            assert f'[DIFFICULTY_{mode.upper()}][TRAINER_XIPHOS_{rank}]' in meta
    assert len(sigs)==5
assert 'roster[16][5]' in (R/'src/data/brisk_xiphos_loader.h').read_text()
assert 'TRAINER_XIPHOS_JESSIE' in (R/'src/battle_util.c').read_text()
assert 'kantoTrainerFlags[80]' in (R/'include/global.h').read_text()
print('PASS: all 16 Xiphos trainer IDs and 80 distinct six-Pokemon parties')
