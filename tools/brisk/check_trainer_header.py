#!/usr/bin/env python3
"""Exercise the real trainer build rule against a newer, stale cached header."""
import os
from pathlib import Path
import shutil
import subprocess
import tempfile
from check_trainer_progression import ROOT

with tempfile.TemporaryDirectory() as directory:
    temp=Path(directory);data=temp/'src/data';data.mkdir(parents=True)
    for name in ('brisk_mode_teams.party','brisk_kanto_teams.party'):
        shutil.copy2(ROOT/'src/data'/name,data/name)
    makefile=temp/'check.mk'
    makefile.write_text('TRAINERPROC := '+str(ROOT/'tools/trainerproc/trainerproc')+'\nCPP := cpp\nCPPFLAGS := -DMODERN=1 -DTESTING=0 -DEMERALD\ninclude '+str(ROOT/'trainer_rules.mk')+'\n.PHONY: FORCE\nFORCE:\n')
    def build():
        subprocess.run(['make','-f',str(makefile),'src/data/brisk_mode_teams.h'],cwd=temp,check=True,capture_output=True)
    build();header=data/'brisk_mode_teams.h';expected=header.read_bytes()
    header.write_text('/* stale trainer table from an older build */\n')
    newest=max((data/'brisk_mode_teams.party').stat().st_mtime,(data/'brisk_kanto_teams.party').stat().st_mtime)+600
    os.utime(header,(newest,newest))
    build();assert header.read_bytes()==expected,'Newer cached header was not repaired'
    before=header.stat().st_mtime_ns
    build();assert header.stat().st_mtime_ns==before,'Identical generated table forced an unnecessary rebuild'
print('PASS: actual trainerproc repairs newer stale headers and preserves timestamps for identical tables.')
