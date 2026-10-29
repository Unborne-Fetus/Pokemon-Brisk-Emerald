#!/usr/bin/env python3
"""Build Emerald-only postgame tables without altering FRLG's campaign tables.

The last four active land slots and last two water/fishing slots are documented
replacement points for future Fakemon. They contain valid ordinary Pokemon now;
SPECIES_NONE is only used for the existing land/water terminators.
"""
from copy import deepcopy
import json
from pathlib import Path
import re

ROOT=Path(__file__).resolve().parents[2]
ROUTES={
 1:'PIDGEOT STARAPTOR CORVIKNIGHT TALONFLAME DUBWOOL STOUTLAND LOPUNNY SNORLAX ARBOLIVA ROSERADE TOGEKISS FARIGIRAF',
 2:'SCIZOR HERACROSS PINSIR VIKAVOLT LEAVANNY BRELOOM AMOONGUSS LOKIX VESPIQUEN RIBOMBEE SHIFTRY TREVENANT',
 3:'TYRANITAR GARGANACL RHYPERIOR LYCANROC_MIDDAY FLYGON EXCADRILL BRAVIARY MANDIBUZZ CONKELDURR LUCARIO DODRIO TAUROS',
 4:'CLEFABLE WIGGLYTUFF NIDOKING NIDOQUEEN GLISCOR CROBAT KANGASKHAN GARCHOMP NOIVERN SANDSLASH LYCANROC_MIDNIGHT BRONZONG',
 5:'ARCANINE NINETALES ALAKAZAM ESPEON GARDEVOIR GALLADE LILLIGANT TSAREENA FLORGES AMPHAROS BOLTUND MIENSHAO',
 6:'POLIWRATH POLITOED TOXICROAK YANMEGA LUDICOLO ROSERADE SCIZOR GOODRA QUAGSIRE HERACROSS CLODSIRE KILOWATTREL',
 7:'PORYGON_Z PERSIAN LUXRAY ARCANINE GRUMPIG TANGROWTH TOGEKISS HATTERENE RAICHU MANECTRIC MUSHARNA GOTHITELLE',
 8:'NINETALES CHANDELURE GENGAR DUSKNOIR ABSOL HONCHKROW UMBREON HYPNO HOUNDOOM MISMAGIUS REUNICLUS COFAGRIGUS',
 9:'RHYPERIOR GIGALITH GOLEM FLYGON KROOKODILE SCRAFTY LUCARIO GLISCOR BRAVIARY HAWLUCHA DONPHAN GARCHOMP',
 10:'ELECTIVIRE MAGNEZONE AMPHAROS LUXRAY TOGEDEMARU RAICHU HELIOLISK MANECTRIC BELLIBOLT VIKAVOLT PORYGON_Z METAGROSS',
 11:'SNORLAX TAUROS DODRIO RAPIDASH MUDSDALE STOUTLAND SAWSBUCK EXEGGUTOR BRELOOM MEOWSCARADA KILOWATTREL BOUFFALANT',
 12:'ESCAVALIER AMOONGUSS TANGROWTH SCIZOR TOXICROAK DRAPION SLOWBRO SLOWKING GOODRA FERROTHORN LUDICOLO YANMEGA',
 13:'PELIPPER KILOWATTREL BRAVIARY CORVIKNIGHT TALONFLAME ALTARIA DRAGALGE GASTRODON TANGROWTH HERACROSS MANDIBUZZ TSAREENA',
 14:'DRAGONITE SALAMENCE NOIVERN ALTARIA FLYGON HAXORUS TROPIUS SCYTHER SCIZOR VIKAVOLT NIDOKING NIDOQUEEN',
 15:'KANGASKHAN DODRIO TAUROS BOUFFALANT LOPUNNY BRAVIARY GALLADE GARDEVOIR HATTERENE REUNICLUS FLORGES TOGEKISS',
 16:'DODRIO CYCLIZAR RAPIDASH REVAVROOM MUDSDALE VAROOM CORVIKNIGHT SNORLAX BOUFFALANT SCOLIPEDE ARCANINE KROOKODILE',
 17:'REVAVROOM RAPIDASH MUDSDALE CYCLIZAR SCOLIPEDE TAUROS BRAVIARY KILOWATTREL LUXRAY BOLTUND LYCANROC_DUSK ORTHWORM',
 18:'SKARMORY CORVIKNIGHT EXCADRILL DURANT LUCARIO SCIZOR FERROTHORN BISHARP KINGAMBIT MAGNEZONE COPPERAJAH ORTHWORM',
 21:'TANGROWTH VENUSAUR SERPERIOR TSAREENA ROSERADE AMOONGUSS ARBOLIVA EXEGGUTOR LUDICOLO GOODRA POLIWRATH CLODSIRE',
 22:'ANNIHILAPE LUCARIO GALLADE HERACROSS HAWLUCHA MIENSHAO CONKELDURR KOMMO_O SCRAFTY BEWEAR RHYPERIOR URSALUNA',
 23:'TYRANITAR DRAGONITE SALAMENCE METAGROSS GARCHOMP HYDREIGON GOODRA KOMMO_O DRAGAPULT BAXCALIBUR KINGAMBIT VOLCARONA',
 24:'ALAKAZAM ESPEON GARDEVOIR REUNICLUS GOTHITELLE GALLADE HATTERENE FARIGIRAF BRONZONG METAGROSS ORBEETLE EXEGGUTOR',
 25:'VENUSAUR CHARIZARD BLASTOISE DRAGONITE GYARADOS LAPRAS MILOTIC KINGDRA SLOWKING CLEFABLE NINETALES ARCANINE',
}
# VAROOM is a pre-evolution; the cycling road should be entirely battle-ready.
ROUTES[16]=ROUTES[16].replace('VAROOM','MAGNEZONE')
OCEAN='GYARADOS KINGDRA LAPRAS TENTACRUEL STARMIE CLOYSTER MILOTIC MANTINE BARRASKEWDA PALAFIN DHELMISE'
RIVER='POLIWRATH POLITOED SLOWBRO SLOWKING AZUMARILL LUDICOLO GASTRODON QUAGSIRE MILOTIC GYARADOS DREDNAW'
FISH='GYARADOS KINGDRA MILOTIC BARRASKEWDA SHARPEDO CLOYSTER CRAWDAUNT WHISCASH LANTURN RELICANTH'
PROFILES={
 'VIRIDIAN_FOREST':'SCIZOR HERACROSS PINSIR VOLCARONA VIKAVOLT LEAVANNY RIBOMBEE GALVANTULA AMOONGUSS BRELOOM TREVENANT LOKIX',
 'MT_MOON':'CLEFABLE LUNATONE SOLROCK CARBINK GIGALITH CROBAT ALAKAZAM REUNICLUS MUSHARNA BRONZONG SABLEYE MAWILE',
 'DIGLETTS_CAVE':'DUGTRIO DUGTRIO_ALOLA EXCADRILL STEELIX SANDSLASH KROOKODILE ORTHWORM GLISCOR RHYPERIOR DONPHAN',
 'ROCK_TUNNEL':'GOLEM GIGALITH RHYPERIOR STEELIX CROBAT AGGRON SABLEYE MAWILE GLISCOR ANNIHILAPE LUCARIO TYRANITAR',
 'VICTORY_ROAD':'TYRANITAR DRAGONITE SALAMENCE METAGROSS GARCHOMP HYDREIGON GOODRA KOMMO_O DRAGAPULT BAXCALIBUR VOLCARONA KINGAMBIT',
 'POKEMON_MANSION':'ARCANINE NINETALES WEEZING MUK MAGMORTAR CHANDELURE GENGAR HOUNDOOM DITTO REUNICLUS PORYGON_Z SALAZZLE',
 'POKEMON_TOWER':'GENGAR MISMAGIUS DUSKNOIR CHANDELURE BANETTE COFAGRIGUS RUNERIGUS POLTEAGEIST TREVENANT SPIRITOMB MIMIKYU HOUNDSTONE',
 'SEAFOAM':'LAPRAS CLOYSTER DEWGONG WALREIN FROSLASS GLALIE AVALUGG BEARTIC ABOMASNOW MAMOSWINE CRABOMINABLE SANDSLASH_ALOLA',
 'POWER_PLANT':'ELECTIVIRE MAGNEZONE AMPHAROS RAICHU ELECTRODE MANECTRIC TOGEDEMARU VIKAVOLT LUXRAY PORYGON_Z BELLIBOLT KLINKLANG',
 'CERULEAN_CAVE':'DRAGAPULT METAGROSS TYRANITAR GARCHOMP HYDREIGON SALAMENCE BAXCALIBUR VOLCARONA KINGAMBIT URSALUNA ALAKAZAM SNORLAX',
 'SAFARI_ZONE':'KANGASKHAN TAUROS CHANSEY BLISSEY SCIZOR PINSIR HERACROSS RHYPERIOR DRAGONITE EXEGGUTOR TANGROWTH KROOKODILE',
 'MT_EMBER':'MAGMORTAR ARCANINE NINETALES COALOSSAL CENTISKORCH SALAZZLE TORKOAL VOLCARONA CAMERUPT TURTONATOR TALONFLAME ARMAROUGE',
 'ICEFALL':'LAPRAS CLOYSTER DEWGONG WALREIN FROSLASS GLALIE AVALUGG BEARTIC ABOMASNOW MAMOSWINE',
 'LOST_CAVE':'GENGAR MISMAGIUS DUSKNOIR CHANDELURE SPIRITOMB MIMIKYU COFAGRIGUS CROBAT SABLEYE UMBREON',
}
DEFAULT='HERACROSS SCIZOR BRELOOM ROSERADE TSAREENA TANGROWTH ARCANINE KANGASKHAN TOGEKISS LUCARIO GOODRA BRAVIARY'


def generate():
    path=ROOT/'src/data/wild_encounters.json';data=json.loads(path.read_text())
    config_path=ROOT/'data/brisk_kanto_encounters.json'
    config=json.loads(config_path.read_text()) if config_path.exists() else {}
    overrides=config.get('overrides',{})
    group=data['wild_encounter_groups'][0]
    entries=[e for e in group['encounters'] if not e['base_label'].startswith('sBriskKanto_')]
    generated=[];slots=[];seen=set()
    for original in entries:
        if 'FireRed' not in original['base_label'] or original['map'] in seen:continue
        seen.add(original['map'])
        e=deepcopy(original);mapid=e['map'];route=re.fullmatch(r'MAP_ROUTE(\d+)(?:_NORTH|_SOUTH)?',mapid)
        n=int(route[1]) if route else None
        land=ROUTES.get(n, next((v for k,v in PROFILES.items() if k in mapid),DEFAULT))
        if 'TANOBY_RUINS_' in mapid:land='UNOWN'
        e['base_label']='sBriskKanto_'+mapid.removeprefix('MAP_')
        high=95 if any(k in mapid for k in ('VICTORY_ROAD','CERULEAN_CAVE')) else 90
        for field,table in e.items():
            if not isinstance(table,dict):continue
            pool=(land if field=='land_mons' else 'RHYPERIOR GOLEM GIGALITH SHUCKLE CRUSTLE BARBARACLE' if field=='rock_smash_mons' else FISH if field=='fishing_mons' else OCEAN if n in (19,20,21) or any(k in mapid for k in ('ISLAND','SEAFOAM','SSANNE')) else RIVER).split()
            old=table['mons'];sentinel=bool(old and old[-1]['species']=='SPECIES_NONE');count=len(old)-sentinel
            reserve=4 if field=='land_mons' else 2 if field in ('water_mons','fishing_mons') else 0
            # Fill reserved slots with the first habitat species until custom
            # species are authored. Do not create empty encounter rolls.
            table['mons']=[{'min_level':high+(i%3),'max_level':min(100,high+5),'species':'SPECIES_'+pool[(i+((n or 0)%len(pool)))%len(pool)]} for i in range(count)]
            if sentinel:table['mons'].append({'min_level':5,'max_level':5,'species':'SPECIES_NONE'})
            table['encounter_rate']=15 if field=='land_mons' else min(10,max(5,table['encounter_rate']))
            if reserve:slots.append({'map':mapid,'table':field,'label':e['base_label'],'slots':list(range(max(0,count-reserve),count)),'min_level':high,'max_level':min(100,high+5)})
        for field,changes in overrides.get(mapid,{}).items():
            for slot,species in changes.items():
                index=int(slot)
                assert field in e and 0<=index<len(e[field]['mons']), (mapid,field,slot)
                assert e[field]['mons'][index]['species']!='SPECIES_NONE' and species!='SPECIES_NONE'
                e[field]['mons'][index]['species']=species
        generated.append(e)
    # Orange Strait has no FRLG source table but should share its coastal fauna.
    source=next(e for e in generated if e['map']=='MAP_ROUTE20')
    for mapid in ('MAP_ORANGE_STRAIT_WEST','MAP_ORANGE_STRAIT_EAST'):
        e=deepcopy(source);e['map']=mapid;e['base_label']='sBriskKanto_'+mapid.removeprefix('MAP_');generated.append(e)
    group['encounters']=entries+generated
    path.write_text(json.dumps(data,indent=2)+'\n')
    (ROOT/'data/brisk_kanto_encounters.json').write_text(json.dumps({'description':'Zero-based active slots reserved for replacement with future Fakemon. Valid habitat species occupy them until replaced; do not use SPECIES_NONE. Author overrides below, then rerun this generator.','overrides':overrides,'replacement_slots':slots},indent=2)+'\n')
    print(f'Authored {len(generated)} Emerald Kanto encounter tables; {len(slots)} habitat tables have future Fakemon replacement slots.')

if __name__=='__main__':generate()
