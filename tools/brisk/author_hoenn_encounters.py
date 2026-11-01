#!/usr/bin/env python3
"""Author habitat cores and place every previous Hoenn species in reachable tables.

No random shuffling: common residents occupy 16/24 land slots; the remaining
slots hold compatible visitors. Coverage uses actual evolution minimums and BST
floors, so a leftover powerful species cannot leak into an opening route.
"""
import argparse
from collections import Counter
from copy import deepcopy
import hashlib
import json
from pathlib import Path
import re
import trainer_legality as rules

ROOT = Path(__file__).resolve().parents[2]
PATH = ROOT / 'src/data/wild_encounters.json'
MANIFEST = ROOT / 'data/brisk_hoenn_encounter_rebalance.json'
NONE = 'SPECIES_NONE'

def names(text):
    return ['SPECIES_' + s for s in text.split()]

# Eight recognisable residents, each occupying two of the 24 land slots.
ROUTES = {
101: ('meadow', 3, 5, 'POOCHYENA ZIGZAGOON WURMPLE TAILLOW LILLIPUP FLETCHLING SENTRET SUNKERN'),
102: ('wetland', 5, 8, 'LOTAD SEEDOT RALTS SURSKIT WOOPER AZURILL BUDEW NYMBLE'),
103: ('meadow', 5, 8, 'WINGULL POOCHYENA BUNNELBY PIDOVE HOPPIP SKWOVET NIDORAN_F NIDORAN_M'),
104: ('forest', 7, 11, 'TAILLOW WURMPLE SHROOMISH LEDYBA SPINARAK SEWADDLE COTTONEE DEERLING'),
110: ('urban', 15, 19, 'ELECTRIKE PLUSLE MINUN GULPIN MAGNEMITE VOLTORB BLITZLE TRUBBISH'),
111: ('desert', 22, 28, 'SANDSHREW TRAPINCH CACNEA BALTOY DWEBBLE SILICOBRA HIPPOPOTAS HELIOPTILE'),
112: ('mountain', 20, 25, 'MACHOP NUMEL GEODUDE ROGGENROLA MUDBRAY TEDDIURSA SPOINK ROCKRUFF'),
113: ('ash', 23, 28, 'SPINDA SKARMORY SLUGMA SANDSHREW KOFFING VULPIX MINCCINO SWABLU'),
114: ('mountain', 24, 29, 'SWABLU ZANGOOSE SEVIPER LOTAD SEEDOT SKIDDO NUZLEAF LOMBRE'),
115: ('mountain', 28, 34, 'SWABLU JIGGLYPUFF TAILLOW MACHOKE SOLROCK ROCKRUFF MIENFOO NOSEPASS'),
116: ('meadow', 10, 14, 'NINCADA WHISMUR SKITTY TAILLOW ABRA MACHOP BELLSPROUT YAMPER'),
117: ('meadow', 16, 21, 'ROSELIA ODDISH MARILL VOLBEAT ILLUMISE CUTIEFLY PETILIL FLABEBE'),
118: ('meadow', 27, 33, 'ELECTRIKE LINOONE KECLEON TROPIUS ZEBSTRIKA FURFROU SKIPLOOM DODUO'),
119: ('rainforest', 29, 36, 'TROPIUS KECLEON ODDISH GLOOM SHROOMISH FOONGUS MORELULL GOOMY'),
120: ('rainforest', 32, 39, 'ABSOL KECLEON NUZLEAF MIGHTYENA TANGELA PASSIMIAN ORANGURU IMPIDIMP'),
121: ('forest', 34, 41, 'SHUPPET DUSKULL GLOOM LINOONE HERACROSS PINSIR PHANTUMP MURKROW'),
123: ('wetland', 34, 42, 'GLOOM WEEPINBELL TROPIUS YANMA CARNIVINE AMOONGUSS LOMBRE CROAGUNK'),
130: ('meadow', 38, 45, 'WYNAUT CHIMECHO TOGETIC DUNSPARCE AUDINO CHANSEY INDEEDEE MAWILE'),
}
PROFILES = {
'forest': 'SHROOMISH WURMPLE SILCOON CASCOON PARAS VENONAT PINECO APPLIN',
'cave': 'ZUBAT WHISMUR GEODUDE ARON MAKUHITA NOSEPASS ROGGENROLA WOOBAT',
'fire': 'NUMEL SLUGMA TORKOAL KOFFING GROWLITHE HOUNDOUR ROLYCOLY SALANDIT',
'ghost': 'SHUPPET DUSKULL GASTLY MISDREAVUS LITWICK YAMASK SINISTEA PHANTUMP',
'electric': 'MAGNEMITE VOLTORB ELECTRIKE TOGEDEMARU DEDENNE JOLTIK TYNAMO CHARJABUG',
'desert': 'SANDSHREW TRAPINCH CACNEA BALTOY DWEBBLE SILICOBRA HIPPOPOTAS HELIOPTILE',
'ice': 'SNORUNT SPHEAL SEEL SWINUB CUBCHOO BERGMITE SMOOCHUM SNEASEL',
'dragon': 'BAGON DRATINI DEINO AXEW DREEPY GIBLE JANGMO_O FRIGIBAX',
'safari': 'GIRAFARIG NATU DODUO EXEGGCUTE RHYHORN SCYTHER PINSIR KANGASKHAN',
'ruins': 'BALTOY CLAYDOL BRONZOR BRONZONG SIGILYPH UNOWN SABLEYE GOLETT',
'meadow': 'LINOONE MIGHTYENA DODUO AUDINO DEERLING SKIDDO FURFROU BOUFFALANT',
'wetland': 'LOTAD WOOPER MARILL PSYDUCK SURSKIT CROAGUNK SHELLOS WOOPER_PALDEA',
}
# Explicit pools prevent non-aquatic birds and land mammals appearing in Surf.
WATER = {
'river': 'PSYDUCK MARILL WOOPER LOTAD SURSKIT POLIWAG BUIZEL SHELLOS DUCKLETT BASCULIN_RED_STRIPED BARBOACH',
'ocean': 'TENTACOOL WINGULL PELIPPER MANTYKE WAILMER FINNEON FRILLISH HORSEA CLAMPERL CHINCHOU REMORAID',
'deep': 'CLAMPERL CHINCHOU RELICANTH LANTURN STARYU SKRELP CLAUNCHER FINNEON FRILLISH TYNAMO SHELLDER',
'ice': 'SPHEAL SEEL SHELLDER LAPRAS DEWGONG SEALEO CLOYSTER WALREIN MANTYKE FRILLISH WAILMER',
}
FISH = {
'river': ('MAGIKARP GOLDEEN', 'BARBOACH CORPHISH REMORAID', 'FEEBAS CARVANHA BASCULIN_RED_STRIPED WHISCASH CRAWDAUNT'),
'ocean': ('MAGIKARP TENTACOOL', 'HORSEA KRABBY REMORAID', 'CHINCHOU WAILMER CARVANHA STARYU CLAMPERL'),
'deep': ('MAGIKARP TENTACOOL', 'CHINCHOU HORSEA SHELLDER', 'RELICANTH CLAMPERL SKRELP CLAUNCHER LANTURN'),
'ice': ('MAGIKARP TENTACOOL', 'SHELLDER REMORAID CHINCHOU', 'CLOYSTER SEAKING SEALEO LANTURN RELICANTH'),
}
TYPES = {
'meadow': {'NORMAL','FLYING','GRASS','FAIRY','BUG'},
'forest': {'BUG','GRASS','FAIRY','NORMAL','POISON'},
'wetland': {'GRASS','BUG','POISON','FAIRY','NORMAL'},
'rainforest': {'GRASS','BUG','POISON','DARK','FAIRY'},
'urban': {'ELECTRIC','STEEL','POISON','NORMAL','PSYCHIC'},
'mountain': {'ROCK','GROUND','FIGHTING','FLYING','NORMAL'},
'desert': {'GROUND','ROCK','FIRE','BUG','DARK'},
'ash': {'FIRE','STEEL','NORMAL','POISON','GROUND'},
'fire': {'FIRE','POISON','ROCK','GROUND'},
'ghost': {'GHOST','DARK','PSYCHIC'},
'electric': {'ELECTRIC','STEEL'},
'cave': {'ROCK','GROUND','STEEL','POISON','FIGHTING','DARK','PSYCHIC'},
'ice': {'ICE'},
'dragon': {'DRAGON','ROCK','STEEL','FLYING'},
'safari': {'NORMAL','BUG','GRASS','GROUND','FIGHTING','FAIRY','PSYCHIC','FLYING'},
'ruins': {'PSYCHIC','GHOST','GROUND','ROCK','STEEL'},
}


def selected(e):
    label, mapid = e['base_label'], e['map']
    if not label.startswith('g') or any(x in label for x in ('FireRed','LeafGreen')):
        return False
    if any(x in mapid for x in ('UNUSED','VICTORY_ROAD','EVOLUTION_CAVE','ALTERING_CAVE','ORANGE_CITY')):
        return False
    route = re.fullmatch(r'MAP_ROUTE(\d+)', mapid)
    return not route or 101 <= int(route[1]) <= 134


def profile(e):
    m = e['map']
    route = re.fullmatch(r'MAP_ROUTE(\d+)', m)
    if route and int(route[1]) in ROUTES:
        return ROUTES[int(route[1])]
    if 'PETALBURG_WOODS' in m: return 'forest', 8, 12, PROFILES['forest']
    if 'PETALBURG_CITY' in m: return 'wetland', 5, 8, 'LOTAD SEEDOT RALTS SURSKIT WOOPER AZURILL BUDEW NYMBLE'
    if 'RUSTURF' in m: return 'cave', 11, 15, 'WHISMUR ZUBAT GEODUDE ROGGENROLA WOOBAT NOIBAT DIGLETT MAKUHITA'
    if 'GRANITE' in m: return 'cave', 13, 18, PROFILES['cave']
    if 'FIERY' in m: return 'fire', 21, 27, PROFILES['fire']
    if 'JAGGED' in m: return 'fire', 25, 30, PROFILES['fire']
    if 'MAGMA' in m: return 'fire', 35, 43, PROFILES['fire']
    if 'MIRAGE_TOWER' in m: return 'desert', 25, 32, PROFILES['desert']
    if 'DESERT_UNDERPASS' in m: return 'desert', 43, 52, PROFILES['desert']
    if 'NEW_MAUVILLE' in m: return 'electric', 32, 45, PROFILES['electric']
    if 'MT_PYRE' in m: return 'ghost', 33, 41, PROFILES['ghost']
    if 'METEOR_FALLS' in m:
        if any(x in m for x in ('1F_2R','B1F','STEVENS')):
            return 'dragon', 42, 54, PROFILES['dragon']
        return 'cave', 25, 31, 'ZUBAT SOLROCK LUNATONE CLEFAIRY BRONZOR NOSEPASS SABLEYE MAWILE'
    if 'SHOAL' in m: return 'ice', 35, 44, PROFILES['ice']
    if 'SEAFLOOR' in m: return 'cave', 40, 48, PROFILES['cave']
    if 'CAVE_OF_ORIGIN' in m: return 'ruins', 43, 51, PROFILES['ruins']
    if 'SKY_PILLAR' in m: return 'dragon', 48, 58, PROFILES['dragon']
    if 'SAFARI' in m: return 'safari', 30, 40, PROFILES['safari']
    if 'ARTISAN' in m: return 'meadow', 50, 60, PROFILES['meadow']
    # Water-only maps: Surf comes after Norman; Old Rod is handled separately.
    return 'wetland', 30, 38, PROFILES['wetland']


def aquatic_profile(e):
    m = e['map']
    if 'SHOAL' in m: return 'ice'
    if 'UNDERWATER' in m: return 'deep'
    n = re.fullmatch(r'MAP_ROUTE(\d+)', m)
    if n and (int(n[1]) in (104,105,106,107,108,109,115,122) or 124 <= int(n[1]) <= 134):
        return 'ocean'
    if any(x in m for x in ('SHIP','DEWFORD','SLATEPORT','LILYCOVE','MOSSDEEP','PACIFIDLOG','EVER_GRANDE','SEAFLOOR')):
        return 'ocean'
    return 'river'


def metadata():
    result = {}
    for path in sorted((ROOT/'src/data/pokemon/species_info').glob('*.h')):
        text = path.read_text().replace('\\\n', ' ')
        definitions = {}
        for key,value in re.findall(r'^\s*#define\s+(\w+)([^\n]*)',text,re.M):
            definitions.setdefault(key,value)
        def parse_types(expression):
            for key, value in definitions.items():
                if key in expression:
                    expression = expression.replace(key,value)
            found = re.findall(r'TYPE_(\w+)',expression)
            return set(found[:1] if '?' in expression else found)
        macros = {}
        for definition in re.finditer(r'^\s*#define\s+(\w+)([^\n]*(?:\\\n[^\n]*)*)', text, re.M):
            match = re.search(r'\.types\s*=\s*MON_TYPES\(([^)]+)\)', definition[2])
            if match: macros[definition[1]] = parse_types(match[1])
        for _ in range(4):
            for key, body in definitions.items():
                if key not in macros:
                    inherited = next((ts for macro,ts in macros.items() if re.search(r'\b'+macro+r'\b',body)),None)
                    if inherited: macros[key] = inherited
        starts = list(re.finditer(r'^\s*\[(SPECIES_\w+)\]\s*=', text, re.M))
        for i, match in enumerate(starts):
            block = text[match.end():starts[i+1].start() if i+1<len(starts) else len(text)]
            types = re.search(r'\.types\s*=\s*MON_TYPES\(([^)]+)\)', block)
            if types: result[match[1]] = parse_types(types[1])
            else:
                type_alias = re.search(r'\.types\s*=\s*(\w+)',block)
                if type_alias and type_alias[1] in definitions:
                    result[match[1]] = parse_types(definitions[type_alias[1]])
                    continue
                for macro, ts in macros.items():
                    if re.search(r'\b'+macro+r'\b',block):
                        result[match[1]] = ts
                        break
    for sp, data in rules.SPECIES.items():
        if sp not in result and data['forms']:
            result[sp] = result.get(rules.canonical(data['forms'][0]), set())
    return result

META = metadata()

def types(sp): return META[rules.canonical(sp)]
def bst(sp): return sum(rules.SPECIES[rules.canonical(sp)]['stats'])

def floor(sp):
    # Direct wild evolutions are deliberately later than their theoretical
    # stone/trade/friendship minimum. Early routes favour unevolved partners.
    s = rules.canonical(sp)
    evolved = bool(rules.PARENTS.get(s))
    stat_floor = 40 if bst(sp)>=570 else 35 if bst(sp)>=520 else 28 if bst(sp)>=480 else 20 if bst(sp)>=430 else 1
    return max(rules.minimum_level(s), 18 if evolved and bst(sp)>=400 else 1, stat_floor)


def land_allowed(sp, biome):
    habitat_overrides = {
        'SPECIES_BURMY_SANDY': {'desert','mountain'},
        'SPECIES_BURMY_TRASH': {'urban'},
        'SPECIES_CASTFORM': {'rainforest'},
        'SPECIES_DITTO': {'ruins','cave','safari'},
    }
    if sp in habitat_overrides: return biome in habitat_overrides[sp]
    ts = types(sp)
    # Water residents use Surf/fishing; explicit cores include amphibians.
    if 'WATER' in ts: return False
    if 'ICE' in ts: return biome=='ice'
    if 'ROCK' in ts or 'GROUND' in ts:
        return biome in ('cave','mountain','desert','dragon','ruins','ash')
    if 'FIRE' in ts: return biome in ('fire','ash','desert','mountain')
    if 'GHOST' in ts: return biome in ('ghost','ruins','forest','rainforest')
    if 'BUG' in ts: return biome in ('forest','rainforest','wetland','safari','meadow')
    return bool(ts & TYPES[biome])


def slots(e):
    for field, table in e.items():
        if isinstance(table,dict) and 'mons' in table:
            yield field,table


def digest(value):
    return hashlib.sha256(json.dumps(value,sort_keys=True).encode()).hexdigest()


def generate(check=False):
    data = json.loads(PATH.read_text())
    es = data['wild_encounter_groups'][0]['encounters']
    original = deepcopy(data)
    previous = json.loads(MANIFEST.read_text())
    if previous.get('version') == 2:
        required = previous['required_species']
        protected_hash = previous['protected_sha256']
    else:
        required = sorted({m['species'] for e in es if selected(e) for _,t in slots(e) for m in t['mons'] if m['species']!=NONE})
        protected_hash = digest([e for e in es if not selected(e)] + data['wild_encounter_groups'][1:])
    candidates = []
    for e in es:
        if not selected(e): continue
        biome,low,high,core = profile(e)
        for field,t in slots(e):
            aquatic = aquatic_profile(e)
            if field == 'land_mons':
                pool = names(core)
                # Cores are anchors, not coverage filler; their legal minimum
                # is enforced too (e.g. Spinda's buffed stats).
                mons = [dict(species=sp,min_level=max(low,floor(sp)),max_level=max(high,floor(sp))) for sp in pool for _ in range(2)]
                visitor_start, count = 16, 24
                pool = [sp for sp in pool if floor(sp)<=high] or pool
            elif field == 'water_mons':
                pool = names(WATER[aquatic])
                offset = sum(map(ord,e['map'])) % len(pool)
                pool = pool[offset:] + pool[:offset]
                count=11; visitor_start=7
                water_low = max(28,low)
                mons = [dict(species=sp,min_level=max(water_low,floor(sp)),max_level=max(water_low+6,high,floor(sp))) for sp in pool[:7]]
            elif field == 'fishing_mons':
                rod = FISH[aquatic];pool=names(' '.join(rod));count=10;visitor_start=7
                mons=[]
                for i,sp in enumerate(pool):
                    lo,hi = (5,10) if i<2 else (18,25) if i<5 else (32,42)
                    mons.append(dict(species=sp,min_level=max(lo,floor(sp)),max_level=max(hi,floor(sp))))
                mons=mons[:visitor_start]
            else:
                pool=names('GEODUDE NOSEPASS SHUCKLE DWEBBLE BINACLE');count=5;visitor_start=5
                mons=[dict(species=sp,min_level=max(15,low,floor(sp)),max_level=max(20,high,floor(sp))) for sp in pool]
            while len(mons)<count:
                sp=pool[(len(mons)-visitor_start)%len(pool)]
                lo = max(low,floor(sp)) if field=='land_mons' else max(32,floor(sp))
                hi = max(high,lo)
                mons.append(dict(species=sp,min_level=lo,max_level=hi))
            if field in ('land_mons','water_mons'):
                mons.append(dict(species=NONE,min_level=5,max_level=5))
            t['mons']=mons
            for i in range(visitor_start,count):
                candidates.append((e,field,i,biome,low,high))
    present = {m['species'] for e in es if selected(e) for f,t in slots(e) for i,m in enumerate(t['mons']) if i < (16 if f=='land_mons' else 7 if f in ('water_mons','fishing_mons') else 5)}
    missing = set(required)-present
    used = Counter()
    def choices(sp):
        out=[]
        for index,(e,field,i,biome,low,high) in enumerate(candidates):
            if field=='land_mons':
                if floor(sp)>high or not land_allowed(sp,biome): continue
            else:
                if 'WATER' not in types(sp): continue
                # Fishing visitors occupy Super Rod slots only.
                if field=='fishing_mons' and floor(sp)>42: continue
            out.append(index)
        return out
    options = {sp: choices(sp) for sp in missing}
    # Most constrained species first; ties stable across machines and runs.
    occupied=set()
    for sp in sorted(missing,key=lambda sp:(len(options[sp]),-floor(sp),sp)):
        available=[i for i in options[sp] if i not in occupied]
        if not available: raise ValueError('No reachable habitat slot for '+sp)
        def rank(i):
            e,field,slot,biome,low,high=candidates[i]
            affinity=len(types(sp)&TYPES[biome]) if field=='land_mons' else 1
            # Spread visitors over floors/routes instead of filling one map.
            return (used[e['base_label']],-affinity,abs(max(low, floor(sp))-max(15,floor(sp))),e['base_label'],slot)
        i=min(available,key=rank);occupied.add(i)
        e,field,slot,biome,low,high=candidates[i]
        lo=max(low,floor(sp)) if field=='land_mons' else max(32,floor(sp))
        hi=max(high,lo)
        e[field]['mons'][slot]=dict(species=sp,min_level=lo,max_level=hi)
        used[e['base_label']]+=1
    # The source dimensions and global probabilities are unchanged: 24+sentinel,
    # 11+sentinel, 2/3/5 fishing slots, five Rock Smash slots.
    assert digest([e for e in es if not selected(e)]+data['wild_encounter_groups'][1:])==protected_hash
    for e in es:
        if not selected(e): continue
        biome,low,high,core = profile(e)
        for field,t in slots(e):
            t['mons'] = [dict(min_level=m['min_level'],max_level=m['max_level'],species=m['species']) for m in t['mons']]
            count = {'land_mons':24,'water_mons':11,'fishing_mons':10,'rock_smash_mons':5}[field]
            assert len(t['mons']) == count + (field in ('land_mons','water_mons')), (e['map'],field)
            assert len(data['wild_encounter_groups'][0]['fields'][next(i for i,f in enumerate(data['wild_encounter_groups'][0]['fields']) if f['type']==field)]['encounter_rates']) == len(t['mons'])
            for i,m in enumerate(t['mons'][:count]):
                assert m['species'] != NONE, (e['map'],field,i)
                assert floor(m['species']) <= m['min_level'] <= m['max_level'] <= 100, (e['map'],field,m)
                if field=='land_mons': assert m['max_level']<=high, (e['map'],m)
                if field=='fishing_mons' and i<2: assert m['max_level']<=10
            if field in ('land_mons','water_mons'): assert t['mons'][-1]['species']==NONE
    available={m['species'] for e in es if selected(e) for _,t in slots(e) for m in t['mons']}
    assert set(required)<=available
    report={
        'version':2,
        'description':'Authored Hoenn habitat cores, progression level ranges and deterministic coverage visitors. Every previous ordinary Hoenn species remains directly catchable in a reachable table.',
        'required_species':required,'protected_sha256':protected_hash,
        'affected_tables':sum(1 for e in es if selected(e)),
        'visitor_placements':len(occupied),
        'excluded':'Kanto, Orange Islands and Route135+, Evolution Cave starter/form collections, Victory Road Paradox tables, Altering Cave Pikachu/event collection, unused Ruby/Sapphire maps, Battle facilities and scripted rewards.',
        'candy_badges':{'XS':3,'S':4,'M':5,'L':7,'XL':8},
    }
    if check:
        assert data==original, 'Encounter tables differ from authored output; run author_hoenn_encounters.py'
        assert report==previous, 'Coverage manifest is stale'
    else:
        PATH.write_text(json.dumps(data,indent=2)+'\n')
        MANIFEST.write_text(json.dumps(report,indent=2)+'\n')
    print(f'PASS: {report["affected_tables"]} Hoenn tables; {len(required)} previous species retained; {len(occupied)} habitat visitors; protected content unchanged.')

if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--check',action='store_true')
    generate(parser.parse_args().check)
