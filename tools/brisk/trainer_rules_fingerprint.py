"""Detect when the compiled legality snapshot needs to be regenerated."""
import hashlib
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]


def source_fingerprint():
    paths=set((ROOT/'src/data/pokemon/species_info').rglob('*.h'))
    paths.update((ROOT/'src/data/pokemon/level_up_learnsets').rglob('*.h'))
    paths.update(p for p in (ROOT/'src/data/pokemon').glob('*_learnsets.h') if p.name!='teachable_learnsets.h')
    paths.update(ROOT/p for p in ('src/data/pokemon/species_info.h','src/data/pokemon/egg_moves.h'))
    paths.update((ROOT/'include/config').glob('pokemon*.h'))
    paths.update(ROOT/p for p in ('include/pokemon.h','include/constants/pokemon.h',
        'include/constants/species.h','include/constants/moves.h','include/constants/abilities.h',
        'src/data/pokemon/all_learnables.json','src/data/pokemon/special_movesets.json'))
    digest=hashlib.sha256()
    for path in sorted(paths):
        if path.exists():
            digest.update(str(path.relative_to(ROOT)).encode());digest.update(b'\0');digest.update(path.read_bytes())
    return digest.hexdigest()
