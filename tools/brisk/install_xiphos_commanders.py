from pathlib import Path
import re

R=Path(__file__).resolve().parents[2]
names={"Fairy Commander":"FAIRY","Fighting Commander":"FIGHTING","Steel Commander":"STEEL","Dragon Commander":"DRAGON"}
modes=["Normal","Easy","Hard","Kaizo","Expert"]
blue=(R/"docs/xiphos_aegis_implementation_blueprint.md").read_text()
rows={}
for line in blue.splitlines():
    v=[x.strip() for x in line.strip().strip("|").split("|")]
    if len(v)==6 and v[0] in names:
        rows[v[0]]={mode:[x.strip() for x in r.split(",")] for mode,r in zip(["Easy","Normal","Hard","Expert","Kaizo"],v[1:])}
assert set(rows)==set(names)
form={"Hisuian Goodra":"GOODRA_HISUI","Rotom-Wash":"ROTOM_WASH","Urshifu-Rapid-Strike":"URSHIFU_RAPID_STRIKE"}
def sp(x):return form.get(x,x.upper().replace(" ","_").replace("-","_"))
moves={
"Grimmsnarl":"REFLECT LIGHT_SCREEN SPIRIT_BREAK THUNDER_WAVE",
"Togekiss":"AIR_SLASH DAZZLING_GLEAM FOLLOW_ME HELPING_HAND",
"Tinkaton":"GIGATON_HAMMER PLAY_ROUGH FAKE_OUT KNOCK_OFF",
"Kingdra":"HYDRO_PUMP DRACO_METEOR ICE_BEAM PROTECT",
"Lucario":"AURA_SPHERE FLASH_CANNON EXTREME_SPEED PROTECT",
"Hatterene":"PSYCHIC DAZZLING_GLEAM TRICK_ROOM MYSTICAL_FIRE",
"Rotom-Wash":"HYDRO_PUMP THUNDERBOLT WILL_O_WISP VOLT_SWITCH",
"Metagross":"METEOR_MASH ZEN_HEADBUTT BULLET_PUNCH PROTECT",
"Iron Valiant":"MOONBLAST CLOSE_COMBAT KNOCK_OFF PROTECT",
"Whimsicott":"TAILWIND MOONBLAST ENCORE HELPING_HAND",
"Corviknight":"BRAVE_BIRD IRON_HEAD ROOST TAILWIND",
"Hitmontop":"FAKE_OUT CLOSE_COMBAT WIDE_GUARD FEINT",
"Gallade":"SACRED_SWORD PSYCHO_CUT SHADOW_SNEAK PROTECT",
"Kommo-o":"CLANGING_SCALES DRAIN_PUNCH PROTECT FLAMETHROWER",
"Dragapult":"DRAGON_DARTS SHADOW_BALL THUNDERBOLT PROTECT",
"Urshifu-Rapid-Strike":"SURGING_STRIKES CLOSE_COMBAT AQUA_JET PROTECT",
"Annihilape":"RAGE_FIST DRAIN_PUNCH BULK_UP PROTECT",
"Bronzong":"TRICK_ROOM GYRO_BALL BODY_PRESS PROTECT",
"Excadrill":"IRON_HEAD HIGH_HORSEPOWER ROCK_SLIDE PROTECT",
"Gholdengo":"MAKE_IT_RAIN SHADOW_BALL NASTY_PLOT PROTECT",
"Garchomp":"DRAGON_CLAW EARTHQUAKE ROCK_SLIDE PROTECT",
"Kingambit":"KOWTOW_CLEAVE SUCKER_PUNCH IRON_HEAD PROTECT",
"Flygon":"EARTH_POWER DRAGON_PULSE TAILWIND PROTECT",
"Dragalge":"SLUDGE_BOMB DRACO_METEOR HYDRO_PUMP PROTECT",
"Hisuian Goodra":"FLASH_CANNON DRACO_METEOR BODY_PRESS PROTECT",
"Baxcalibur":"GLAIVE_RUSH ICICLE_CRASH ICE_SHARD PROTECT",
"Duraludon":"FLASH_CANNON DRACO_METEOR THUNDERBOLT PROTECT"}
abilities={"Grimmsnarl":"PRANKSTER","Togekiss":"SERENE_GRACE","Tinkaton":"MOLD_BREAKER","Kingdra":"SWIFT_SWIM",
"Lucario":"INNER_FOCUS","Hatterene":"MAGIC_BOUNCE","Rotom-Wash":"LEVITATE","Metagross":"CLEAR_BODY",
"Iron Valiant":"QUARK_DRIVE","Whimsicott":"PRANKSTER","Corviknight":"MIRROR_ARMOR","Hitmontop":"INTIMIDATE",
"Gallade":"SHARPNESS","Kommo-o":"BULLETPROOF","Dragapult":"INFILTRATOR","Urshifu-Rapid-Strike":"UNSEEN_FIST",
"Annihilape":"DEFIANT","Bronzong":"LEVITATE","Excadrill":"MOLD_BREAKER","Gholdengo":"GOOD_AS_GOLD",
"Garchomp":"ROUGH_SKIN","Kingambit":"SUPREME_OVERLORD","Flygon":"LEVITATE","Dragalge":"ADAPTABILITY",
"Hisuian Goodra":"SHELL_ARMOR","Baxcalibur":"THERMAL_EXCHANGE","Duraludon":"STALWART"}
spatt={"Hatterene","Kingdra","Rotom-Wash","Dragalge","Gholdengo","Hisuian Goodra","Duraludon","Flygon"}
items={"Grimmsnarl":"LIGHT_CLAY","Hatterene":"LEFTOVERS","Togekiss":"SITRUS_BERRY","Hitmontop":"SITRUS_BERRY",
"Whimsicott":"FOCUS_SASH","Kingambit":"BLACK_GLASSES","Gholdengo":"LEFTOVERS","Bronzong":"MENTAL_HERB",
"Rotom-Wash":"SITRUS_BERRY","Iron Valiant":"BOOSTER_ENERGY","Corviknight":"ROCKY_HELMET","Urshifu-Rapid-Strike":"MYSTIC_WATER"}
extra={"Dragon Commander":{"Expert":("Kingdra","Duraludon")}}
gmax={"Fairy Commander":"Hatterene","Fighting Commander":"Urshifu-Rapid-Strike",
"Steel Commander":"Corviknight","Dragon Commander":"Duraludon"}
constants={p:(R/"include/constants"/p).read_text() for p in ("species.h","moves.h","abilities.h","items.h")}
for key in rows:
    for mode,mons in rows[key].items():
        oldnew=extra.get(key,{}).get(mode)
        if oldnew:mons[mons.index(oldnew[0])]=oldnew[1]
        assert len(mons)==6 and len(set(mons))==6
        for mon in mons:
            assert mon in moves and mon in abilities,mon
            for f,v in (("species.h","SPECIES_"+sp(mon)),("abilities.h","ABILITY_"+abilities[mon]),("items.h","ITEM_"+items.get(mon,"SITRUS_BERRY"))):
                assert re.search(r"\b"+v+r"\b",constants[f]),v
            for move in moves[mon].split():assert re.search(r"\bMOVE_"+move+r"\b",constants["moves.h"]),move
for name in rows:assert len({tuple(v) for v in rows[name].values()})==5,name
p=R/"include/constants/brisk_kanto_trainers.h";t=p.read_text()
assert "TRAINER_XIPHOS_COMMANDER_FAIRY" not in t
t=t.replace("#endif", "".join("#define TRAINER_XIPHOS_COMMANDER_%s (BRISK_KANTO_TRAINER_BASE + %d)\n"%(v,624+i) for i,v in enumerate(names.values()))+"\n#endif")
p.write_text(t)
p=R/"include/constants/opponents.h";t=p.read_text();assert "#define BRISK_KANTO_TRAINER_COUNT 624" in t
p.write_text(t.replace("#define BRISK_KANTO_TRAINER_COUNT 624","#define BRISK_KANTO_TRAINER_COUNT 628"))
level={"Easy":92,"Normal":95,"Hard":98,"Expert":99,"Kaizo":100}
ivs={"Easy":0,"Normal":24,"Hard":31,"Expert":28,"Kaizo":31}
def trainer(name):
    h=names[name]
    return "\n=== TRAINER_XIPHOS_COMMANDER_%s ===\nName: %s\nClass: %s\nPic: %s\nGender: Male\nMusic: Male\nDouble Battle: Yes\nAuthored Party: Yes\nAI: Basic Trainer / Smart Switching / HP Aware / Try To 2HKO\n\n"%(h,{"FAIRY":"ASTRA","FIGHTING":"VALOR","STEEL":"FERRUM","DRAGON":"DRAKON"}[h],{"FAIRY":"Aroma Lady","FIGHTING":"Black Belt","STEEL":"Expert","DRAGON":"Dragon Tamer"}[h],{"FAIRY":"Aroma Lady","FIGHTING":"Black Belt","STEEL":"Expert M","DRAGON":"Dragon Tamer"}[h])
def mons_party(name,mode):
    out=[]
    for mon in rows[name][mode]:
        out+=["SPECIES_"+sp(mon)+" @ "+items.get(mon,"SITRUS_BERRY").replace("_"," ").title(),
            "Level: "+str(level[mode]),"Ability: ABILITY_"+abilities[mon],
            "IVs: "+" / ".join(str(ivs[mode])+" "+s for s in ["HP","Atk","Def","SpA","SpD","Spe"]),
            "EVs: "+("0 HP" if mode=="Easy" else "252 HP / 252 SpA / 4 SpD" if mon in spatt else "252 Atk / 252 Spe / 4 HP"),
            "Nature: "+("Hardy" if mode=="Easy" else "Modest" if mon in spatt else "Jolly")]
        if mon==gmax[name] and mode in ("Hard","Expert","Kaizo"):
            out+=["Gigantamax: Yes","Dynamax Level: 10"]
        out+=["- MOVE_"+x for x in moves[mon].split()]
        out+=[""]
    return "\n".join(out)+"\n"
p=R/"src/data/trainers.party";assert "TRAINER_XIPHOS_COMMANDER_FAIRY" not in p.read_text()
with p.open("a",encoding="utf-8") as f:
    for name in names:f.write(trainer(name)+mons_party(name,"Normal"))
# Actual mode switch is independent of the four expansion Trainer rows.
code=["// Five separately authored battle-only team matrices: Easy/Normal/Hard/Kaizo/Expert.",
      "// Stored mode IDs: Normal 0, Easy 1, Hard 2, Kaizo 3, Expert 4.",
      "static const u8 sXiphosPhysicalEvs[NUM_STATS] = {4,252,0,0,0,252};",
      "static const u8 sXiphosSpecialEvs[NUM_STATS] = {252,0,0,252,4,0};"]
for name,short in names.items():
    for mode in modes:
        var="sXiphos"+short.title()+mode
        code+=["static const struct TrainerMon "+var+"[PARTY_SIZE] = {"]
        for mon in rows[name][mode]:
            entry=".species=SPECIES_"+sp(mon)+", .lvl="+str(level[mode])+", .iv="+str(sum(ivs[mode]<<(5*i) for i in range(6)))
            entry+=", .ability=ABILITY_"+abilities[mon]+", .heldItem=ITEM_"+items.get(mon,"SITRUS_BERRY")
            entry+=", .nature="+("NATURE_HARDY" if mode=="Easy" else "NATURE_MODEST" if mon in spatt else "NATURE_JOLLY")
            entry+=", .gender=TRAINER_MON_RANDOM_GENDER, .friendship=255, .ball=BALL_POKE"
            entry+=", .ev="+("NULL" if mode=="Easy" else "sXiphosSpecialEvs" if mon in spatt else "sXiphosPhysicalEvs")
            entry+=", .moves={"+", ".join("MOVE_"+x for x in moves[mon].split())+"}"
            if mon==gmax[name] and mode in ("Hard","Expert","Kaizo"):
                entry+=", .gigantamaxFactor=TRUE, .shouldUseDynamax=TRUE, .dynamaxLevel=10"
            code+=["    {"+entry+"},"]
        code+=["};"]
code+=["static bool32 BriskLoadXiphosCommanderParty(struct Pokemon *party, u16 trainerNum)",
       "{","    u32 id = trainerNum - TRAINER_XIPHOS_COMMANDER_FAIRY;",
       "    u32 mode = GetBriskDifficulty();",
       "    if (id >= 4 || mode > BRISK_MODE_MAX) return FALSE;",
       "    static const struct TrainerMon *const roster[4][5] = {"]
for name,short in names.items(): code+=["        {"+", ".join("sXiphos"+short.title()+mode for mode in modes)+"},"]
code+=["    };",
       "    struct Trainer trainer = gTrainers[DIFFICULTY_NORMAL][trainerNum];",
       "    trainer.party = roster[id][mode];",
       "    trainer.partySize = PARTY_SIZE;",
       "    trainer.authoredParty = TRUE;",
       "    CreateNPCTrainerPartyFromTrainer(party, &trainer);",
       "    return TRUE;",
       "}"]
(R/"src/data/brisk_xiphos_teams.h").write_text("\n".join(code)+"\n")
p=R/"src/battle_setup.c";t=p.read_text()
anchor='#include "data/brisk_expert_patch.h"'
assert t.count(anchor)==1;t=t.replace(anchor,anchor+'\n#include "data/brisk_xiphos_teams.h"')
anchor="static void CreateNPCTrainerParty(struct Pokemon *party, u16 trainerNum)\n{\n"
assert t.count(anchor)==1
t=t.replace(anchor,anchor+"    if (BriskLoadXiphosCommanderParty(party, trainerNum))\n        return; // No generic roster rewriting for these five-mode commanders.\n")
p.write_text(t)
print("Wrote 4 commander IDs and 20 independent six-member battle-mode rosters")
