import json
import re
import zlib
from pathlib import Path

PARTY = Path('src/data/trainers.party')
LOW, HIGH = 12048, 18142
text = PARTY.read_text(encoding='utf-8')
lines = text.splitlines(keepends=True)
heads = [i for i, line in enumerate(lines) if line.startswith('=== TRAINER_')]

# Species pools preserve the trainer's class identity while bringing in species
# from across the supported generations.
POOLS = {
    'Battle Girl': 'HARIYAMA HITMONTOP HERACROSS MEDICHAM LUCARIO MIENSHAO HAWLUCHA TSAREENA GRAPPLOCT FALINKS IRON_VALIANT'.split(),
    'Black Belt': 'MACHAMP HARIYAMA HITMONTOP HERACROSS LUCARIO CONKELDURR MIENSHAO HAWLUCHA ANNIHILAPE FALINKS'.split(),
    'Hex Maniac': 'MISMAGIUS BANETTE CHANDELURE GENGAR FROSLASS GOURGEIST MIMIKYU SINISTCHA CERULEDGE SKELEDIRGE'.split(),
    'Psychic': 'GARDEVOIR ALAKAZAM ESPEON XATU METAGROSS REUNICLUS HATTERENE FARIGIRAF ESPATHRA BRONZONG'.split(),
    'Swimmer M': 'GYARADOS SHARPEDO TENTACRUEL PALAFIN QWILFISH WAILORD DREDNAW CLOYSTER VAPOREON KINGDRA'.split(),
    'Swimmer F': 'MILOTIC LANTURN STARMIE AZUMARILL GOREBYSS PRIMARINA LUMINEON VAPOREON QWILFISH BRUXISH'.split(),
    'Cooltrainer': 'GARDEVOIR GALLADE AGGRON FLYGON SALAMENCE MILOTIC METAGROSS LUCARIO VOLCARONA GARCHOMP HYDREIGON DRAGAPULT TINKATON GHOLDENGO'.split(),
    'Expert': 'LUCARIO GARDEVOIR FLYGON VOLCARONA METAGROSS HAXORUS HYDREIGON GARCHOMP DRAGAPULT KINGAMBIT ANNIHILAPE'.split(),
    'Magma': 'CAMERUPT HOUNDOOM NINETALES DARMANITAN TORKOAL VOLCARONA ARCANINE CERULEDGE SKELEDIRGE KROOKODILE'.split(),
    'Aqua': 'SHARPEDO GYARADOS CRAWDAUNT TENTACRUEL BARRASKEWDA DREDNAW PALAFIN WAILORD QWILFISH GOLISOPOD'.split(),
    'Lass': 'ALTARIA AZUMARILL LOPUNNY DELCATTTY GARDEVOIR MIMIKYU TOGEKISS SYLVEON FIDOUGH LILLIGANT WIGGLYTUFF'.split(),
    'Winstrate': 'ALTARIA AZUMARILL GARDEVOIR MANECTRIC CAMERUPT BRELOOM'.split(),
    'Bug Catcher': 'BUTTERFREE BEEDRILL NINJASK SHEDINJA SCIZOR HERACROSS VOLCARONA GALVANTULA LEAVANNY RIBOMBEE GOLISOPOD FALINKS'.split(),
    'Bug Maniac': 'SCIZOR HERACROSS VOLCARONA GALVANTULA LEAVANNY RIBOMBEE GOLISOPOD FROSMOTH ORBEETLE LOKIX'.split(),
    'Hiker': 'GOLEM RHYPERIOR STEELIX AGGRON CAMERUPT GLISCOR GIGALITH EXCADRILL GARGANACL KLAWF TYRANITAR'.split(),
    'Young Couple': 'PLUSLE MINUN VOLBEAT ILLUMISE DELCATTY MANECTRIC TOGETIC LUMINEON MAUSHOLD'.split(),
    'Twins': 'PLUSLE MINUN VOLBEAT ILLUMISE SPINDA SOLROCK LUNATONE TINKATON'.split(),
    'Sr And Jr': 'ALTARIA CAMERUPT ROSELIA DONPHAN AZUMARILL SUNFLORA'.split(),
    'Old Couple': 'MEDICHAM HARIYAMA GARDEVOIR SLOWKING LUCARIO ALTARIA'.split(),
    'Sis And Bro': 'LANTURN SHARPEDO AZUMARILL PELIPPER LUDICOLO PALAFIN'.split(),
    'Beauty': 'MILOTIC GARDEVOIR ALTARIA ROSERADE LOPUNNY PRIMARINA HATTERENE TSAREENA'.split(),
    'Fisherman': 'GYARADOS MILOTIC TENTACRUEL SHARPEDO KINGDRA WAILORD LANTURN DREDNAW PALAFIN'.split(),
    'Rich Boy': 'PERSIAN ARCANINE ABSOL AZUMARILL LOPUNNY LUXRAY TOGEKISS'.split(),
    'Lady': 'GARDEVOIR MILOTIC ALTARIA ROSERADE TOGEKISS NINETALES HATTERENE'.split(),
    'Tuber F': 'AZUMARILL LANTURN PRIMARINA LUMINEON QWILFISH VAPOREON'.split(),
    'Tuber M': 'TENTACRUEL SHARPEDO GYARADOS WAILORD DREDNAW BARRASKEWDA'.split(),
    'Pokefan': 'PIKACHU RAICHU EEVEE SNORLAX LUXRAY PACHIRISU PLUSLE MINUN'.split(),
    'Guitarist': 'MANECTRIC TOXTRICITY ELLectross AMPHAROS ROTOM LUXRAY MAGNEZONE'.split(),
    'Triathlete': 'STARMIE MANECTRIC SWELLOW PELIPPER DODRIO JOLTEON LANTURN LUCARIO'.split(),
    'Camper': 'ARCANINE DONPHAN SANDSLASH BRELOOM SWELLOW GOGOAT LYCANROC'.split(),
    'Picnicker': 'LILLIGANT AZUMARILL ROSERADE BRELOOM ALTARIA LOPUNNY TSAREENA'.split(),
    'Kindler': 'CAMERUPT TORKOAL NINETALES ARCANINE VOLCARONA CENTISKORCH CERULEDGE'.split(),
    'Aroma Lady': 'ROSERADE LILLIGANT BRELOOM TSAREENA FLORGES LURANTIS GOGOAT'.split(),
    'Bird Keeper': 'SWELLOW SKARMORY ALTARIA STARAPTOR CORVIKNIGHT TALONFLAME KILOWATTREL NOIVERN'.split(),
    'Parasol Lady': 'MILOTIC LANTURN GOREBYSS STARMIE LUDICOLO LUMINEON PRIMARINA'.split(),
    'Ninja Boy': 'NINJASK CROBAT TOXICROAK GRENINJA SNEASLER GENGAR GOLISOPOD'.split(),
    'Ruin Maniac': 'AERODACTYL CRADILY RAMPARDOS EXCADRILL GOLURK STEELIX CLAYDOL TYRANITAR DREDNAW'.split(),
    'Pkmn Breeder': 'DELCATTY LUXRAY LUDICOLO SHIFTRY SWELLOW LINOONE ARBOLIVA MAUSHOLD'.split(),
    'Pokemaniac': 'SNORLAX TYRANITAR AGGRON RHYPERIOR DRAGONITE URSALUNA'.split(),
    'Youngster': 'LINOONE LUXRAY BRELOOM DONPHAN MANECTRIC ARBOLIVA'.split(),
    'Gentleman': 'WOBBUFFET ALAKAZAM ARCANINE GALLADE SLOWKING KINGAMBIT'.split(),
    'Sailor': 'GYARADOS MACHAMP TENTACRUEL PELIPPER GOLISOPOD PALAFIN'.split(),
    'Team Aqua': 'SHARPEDO CRAWDAUNT GYARADOS GOLBAT QWILFISH GOLISOPOD'.split(),
    'Team Magma': 'CAMERUPT HOUNDOOM CROBAT KROOKODILE TORKOAL CLAYDOL'.split(),
}

# Correct a couple of display-name typos in the hand-curated pools.
POOLS['Lass'] = [x.replace('DELCATTTTY', 'DELCATTY') for x in POOLS['Lass']]
POOLS['Guitarist'] = [x.replace('ELLectross', 'EELEKTROSS') for x in POOLS['Guitarist']]

def read_utf8(path):
    return Path(path).read_text(encoding='utf-8')

# Read supported species, their typing/stats/abilities, and all legal learnable moves.
species = {}
for fp in Path('src/data/pokemon/species_info').glob('gen_*_families.h'):
    src = read_utf8(fp)
    for m in re.finditer(r'^    \[SPECIES_([A-Z0-9_]+)\]\s*=\s*\{(.*?)^    \},', src, re.M | re.S):
        key, body = m.group(1), m.group(2)
        abil = re.search(r'\.abilities\s*=\s*\{([^}]+)\}', body)
        typ = re.search(r'\.types\s*=\s*MON_TYPES\(([^)]+)\)', body)
        if not abil or not typ:
            continue
        abils = re.findall(r'ABILITY_([A-Z0-9_]+)', abil.group(1))
        types = re.findall(r'TYPE_([A-Z0-9_]+)', typ.group(1))
        stats = {}
        for prop, k in [('baseAttack','atk'),('baseSpAttack','spa'),('baseSpeed','spe'),('baseHP','hp')]:
            val = re.search(r'\.' + prop + r'\s*=\s*(\d+)', body)
            if val: stats[k] = int(val.group(1))
        species[key] = {'abilities': abils, 'types': types, 'stats': stats}

learnables = json.loads(read_utf8('src/data/pokemon/all_learnables.json'))
move_info = {}
moves_src = read_utf8('src/data/moves_info.h')
for m in re.finditer(r'^    \[MOVE_([A-Z0-9_]+)\]\s*=\s*\{(.*?)^    \},', moves_src, re.M | re.S):
    key, body = m.group(1), m.group(2)
    nm = re.search(r'\.name\s*=\s*COMPOUND_STRING\("([^"]+)"\)', body)
    power = re.search(r'\.power\s*=\s*(\d+)', body)
    typ = re.search(r'\.type\s*=\s*TYPE_([A-Z0-9_]+)', body)
    cat = re.search(r'\.category\s*=\s*DAMAGE_CATEGORY_([A-Z]+)', body)
    if nm:
        move_info[key] = {'name': nm.group(1), 'power': int(power.group(1)) if power else 0,
                          'type': typ.group(1) if typ else 'NORMAL', 'cat': cat.group(1) if cat else 'STATUS'}

ability_names = {}
abil_src = read_utf8('src/data/abilities.h')
for m in re.finditer(r'^    \[ABILITY_([A-Z0-9_]+)\]\s*=\s*\{(.*?)^    \},', abil_src, re.M | re.S):
    nm = re.search(r'\.name\s*=\s*_\("([^"]+)"\)', m.group(2))
    if nm: ability_names[m.group(1)] = nm.group(1)

# Class-specific Mega and Gigantamax forms. Mega takes priority if a Pokémon supports both.
MEGAS = {
 'VENUSAUR':'Venusaurite','CHARIZARD':'Charizardite X','BLASTOISE':'Blastoisinite','BEEDRILL':'Beedrillite',
 'PIDGEOT':'Pidgeotite','ALAKAZAM':'Alakazite','SLOWBRO':'Slowbronite','GENGAR':'Gengarite',
 'KANGASKHAN':'Kangaskhanite','PINSIR':'Pinsirite','GYARADOS':'Gyaradosite','AERODACTYL':'Aerodactylite',
 'AMPHAROS':'Ampharosite','STEELIX':'Steelixite','SCIZOR':'Scizorite','HERACROSS':'Heracronite',
 'HOUNDOOM':'Houndoominite','TYRANITAR':'Tyranitarite','SCEPTILE':'Sceptilite','BLAZIKEN':'Blazikenite',
 'SWAMPERT':'Swampertite','GARDEVOIR':'Gardevoirite','SABLEYE':'Sablenite','MAWILE':'Mawilite',
 'AGGRON':'Aggronite','MEDICHAM':'Medichamite','ALTARIA':'Altarianite','SHARPEDO':'Sharpedonite',
 'CAMERUPT':'Cameruptite','BANETTE':'Banettite','ABSOL':'Absolite','GLALIE':'Glalitite',
 'SALAMENCE':'Salamencite','METAGROSS':'Metagrossite','LOPUNNY':'Lopunnite','LUCARIO':'Lucarionite',
 'GARCHOMP':'Garchompite','ABOMASNOW':'Abomasite','GALLADE':'Galladite','DIANCIE':'Diancite'
}
GMAX = set('BUTTERFREE PIKACHU MACHAMP GENGAR KINGLER LAPRAS SNORLAX GARBODOR CORVIKNIGHT ORBEETLE DREDNAW COALOSSAL FLAPPLE APPLETUN SANDACONDA TOXTRICITY CENTISKORCH HATTERENE GRIMMSNARL ALCREMIE COPPERAJAH DURALUDON RILLABOOM CINDERACE INTELEON VENUSAUR CHARIZARD BLASTOISE'.split())

def class_pool(cls, ident):
    if cls == 'Team Aqua': return POOLS['Team Aqua']
    if cls == 'Team Magma': return POOLS['Team Magma']
    if cls in POOLS: return POOLS[cls]
    # Unknown classes receive a fitting balanced pool, preserving non-battle identity.
    return POOLS['Cooltrainer']

def moves_for(key):
    obj = species[key]
    keys = [x.removeprefix('MOVE_') for x in learnables.get(key, [])]
    candidates = [move_info[k] | {'key': k} for k in keys if k in move_info]
    atk = obj['stats'].get('atk', 70)
    spa = obj['stats'].get('spa', 70)
    stat = 'PHYSICAL' if atk >= spa else 'SPECIAL'
    stab = set(obj['types'])
    damage = [x for x in candidates if x['power'] >= 40 and x['cat'] == stat]
    ranked = sorted(damage, key=lambda x: (x['type'] in stab, x['power']), reverse=True)
    selected = []
    # Two strongest same-type attacks, then one coverage attack.
    for x in ranked:
        if x['type'] in stab and x['name'] not in [v['name'] for v in selected]:
            selected.append(x)
        if len(selected) == 2: break
    for x in ranked:
        if x['type'] not in stab and x['name'] not in [v['name'] for v in selected]:
            selected.append(x); break
    utility = [x for x in candidates if x['power'] == 0 and x['name'] in {
        'Swords Dance','Dragon Dance','Calm Mind','Bulk Up','Quiver Dance','Nasty Plot',
        'Stealth Rock','Spikes','Toxic','Will-O-Wisp','Thunder Wave','Recover','Roost',
        'Roar','Protect','Rain Dance','Sunny Day','Reflect','Light Screen','Agility','Leech Seed'
    }]
    if utility:
        selected.append(utility[zlib.crc32(key.encode()) % len(utility)])
    for x in ranked:
        if len(selected) >= 4: break
        if x['name'] not in [v['name'] for v in selected]: selected.append(x)
    return [x['name'] for x in selected[:4]], stat

def moves_to_showdown(moves):
    return '\n'.join('- ' + x for x in moves)

def iv_line():
    return 'IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe'

def ability_for(key):
    choices = species[key]['abilities']
    ability = next((x for x in choices if x not in ('NONE', 'NO_ABILITY')), choices[0])
    return ability_names.get(ability, ability.replace('_', ' ').title())

def stats_for(key):
    return species[key]['stats']

def build_mon(key, old_level, idx, ident, family):
    moves, offense = moves_for(key)
    st = stats_for(key)
    speed = st.get('spe', 60)
    if offense == 'PHYSICAL':
        evs = '252 Atk / 252 Spe / 4 HP' if speed >= 60 else '252 HP / 252 Atk / 4 SpD'
        nature = 'Jolly' if speed >= 60 else 'Adamant'
    else:
        evs = '252 SpA / 252 Spe / 4 HP' if speed >= 60 else '252 HP / 252 SpA / 4 SpD'
        nature = 'Timid' if speed >= 60 else 'Modest'
    lvl = 100 if ident.endswith('_5') else min(100, old_level + 15)
    # Mega stones outrank Dynamax; otherwise use a G-Max factor when available.
    item = MEGAS.get(key)
    gmax = key in GMAX and not item
    if not item:
        item = 'Life Orb' if speed >= 75 else 'Leftovers'
    # Terastallize non-Mega/G-Max members with an offensive STAB when beneficial.
    tera = None
    if not item in MEGAS.values() and not gmax and (zlib.crc32((family + str(idx)).encode()) % 4 == 0):
        tera = species[key]['types'][0].title()
    head = f'SPECIES_{key} @ {item}'
    out = [head, f'Level: {lvl}', f'Ability: {ability_for(key)}', f'EVs: {evs}', iv_line(), f'Nature: {nature}']
    if gmax: out.append('Gigantamax: Yes')
    if tera: out.append(f'Tera Type: {tera}')
    out.extend(moves_to_showdown(moves).splitlines())
    return '\n'.join(out)

def parse_party(block):
    # Party begins after the first blank line following the trainer metadata.
    blank = next((i for i, line in enumerate(block) if line.strip() == ''), None)
    if blank is None: return [], len(block)
    body = blank + 1
    while body < len(block) and not block[body].strip(): body += 1
    mons = []
    starts = []
    for i in range(body, len(block)):
        l = block[i].strip()
        if not l: continue
        if l.startswith('-') or re.match(r'^[A-Za-z][A-Za-z ]*:', l): continue
        starts.append(i)
    for j, start in enumerate(starts):
        end = starts[j+1] if j+1 < len(starts) else len(block)
        frag = block[start:end]
        level = next((int(re.search(r'Level:\s*(\d+)', x).group(1)) for x in frag if re.search(r'Level:\s*(\d+)', x)), 10)
        mons.append((start, level))
    return mons, body

# Locate the protected battles and eligible trainers in the corrected range.
blocks = []
for n, st in enumerate(heads):
    en = heads[n+1] if n+1 < len(heads) else len(lines)
    line_no = st + 1
    if line_no < LOW or line_no > HIGH:
        continue
    ident = re.search(r'TRAINER_(.+)', lines[st]).group(1).strip('= ')
    block = lines[st:en]
    cls = next((x.split(':', 1)[1].strip() for x in block if x.startswith('Class:')), '')
    protected = (
        any(x in ident for x in ('RIVAL', 'WALLY', 'STEVEN', 'ROXANNE', 'BRAWLY', 'WATTSON', 'FLANNERY', 'NORMAN', 'WINONA', 'TATE_AND_LIZA', 'JUAN', 'MAXIE', 'TABITHA'))
        or cls in ('Leader', 'Magma Leader', 'Aqua Leader', 'Magma Admin', 'Aqua Admin')
        or ident in {'ANABEL','TUCKER','SPENSER','GRETA','NOLAND','LUCY','BRANDON'}
    )
    if protected:
        continue
    parsed, body = parse_party(block)
    if not parsed:
        continue
    blocks.append({'st':st,'en':en,'id':ident,'class':cls,'party':parsed,'line':line_no})

# Assign stable rosters to rematch families. A team's later versions retain its
# selected species; party-size differences use the same lineup prefix.
def family_id(ident):
    m = re.match(r'^(.*)_([1-5])$', ident)
    if m and 'GRUNT' not in ident:
        return m.group(1)
    return ident

families = {}
for b in blocks:
    fam = family_id(b['id'])
    families.setdefault(fam, []).append(b)

replacements = {}
used = 0
stats = {'mega':0,'gmax':0,'tera':0,'mons':0}
for fam, members in families.items():
    # Revisit members in progression order so _1 provides the anchor class.
    members.sort(key=lambda b: (int(re.search(r'_(\d+)$', b['id']).group(1)) if re.search(r'_(\d+)$', b['id']) else 1, b['st']))
    cls = members[0]['class']
    pool = [k for k in class_pool(cls, members[0]['id']) if k in species and k in learnables and k not in MEGAS or False]
    # Include both Mega and G-Max species; filter malformed/missing aliases.
    pool = [k for k in class_pool(cls, members[0]['id']) if k in species and k in learnables]
    if not pool:
        continue
    max_party = max(len(x['party']) for x in members)
    shift = zlib.crc32(fam.encode()) % len(pool)
    lineup = []
    for i in range(max_party):
        key = pool[(shift + i) % len(pool)]
        # Avoid duplicate species whenever a class pool has enough options.
        while key in lineup and len(set(pool)) > len(lineup):
            key = pool[(pool.index(key) + 1) % len(pool)]
        lineup.append(key)
    for b in members:
        block = lines[b['st']:b['en']]
        _, body = parse_party(block)
        old_levels = [lvl for _, lvl in b['party']]
        generated = []
        for i, old_level in enumerate(old_levels):
            key = lineup[i]
            mon = build_mon(key, old_level, i, b['id'], fam)
            generated.append(mon)
            stats['mons'] += 1
            if key in MEGAS: stats['mega'] += 1
            elif key in GMAX: stats['gmax'] += 1
            else:
                if 'Tera Type:' in mon: stats['tera'] += 1
        prefix = block[:body]
        if prefix and not prefix[-1].endswith('\n'):
            prefix[-1] += '\n'
        new_block = prefix + ['\n'.join(generated) + '\n\n']
        replacements[b['st']] = new_block
        used += 1

if __import__('sys').argv[-1:] == ['--dry-run']:
    print(f'eligible trainers={used}; pokemon={stats["mons"]}; mega slots={stats["mega"]}; gmax slots={stats["gmax"]}; tera slots={stats["tera"]}')
    print('excluded protected trainers in range:')
    for st in heads:
        line_no=st+1
        if LOW <= line_no <= HIGH:
            ident=re.search(r'TRAINER_(.+)',lines[st]).group(1).strip('= ')
            if any(x in ident for x in ('RIVAL','WALLY','STEVEN','ROXANNE','BRAWLY','WATTSON','FLANNERY','NORMAN','WINONA','TATE_AND_LIZA','JUAN','MAXIE','TABITHA')):
                print(f'{line_no}: {ident}')
else:
    out=[]
    for i in range(len(lines)):
        if i in replacements:
            out.extend(replacements[i])
            i2=next(k for k in replacements if k==i)
            old_en=next(b['en'] for b in blocks if b['st']==i2)
            # The outer iteration skips replaced source lines by rebuilding below.
    # Rebuild sequentially with source block boundaries.
    out=[]
    cursor=0
    bmap={b['st']:b for b in blocks}
    while cursor < len(lines):
        if cursor in replacements:
            out.extend(replacements[cursor])
            cursor=bmap[cursor]['en']
        else:
            out.append(lines[cursor]); cursor+=1
    PARTY.write_text(''.join(out), encoding='utf-8', newline='')
    print(f'Updated {used} trainers / {stats["mons"]} Pokémon; Mega={stats["mega"]}, G-Max={stats["gmax"]}, Tera={stats["tera"]}.')
