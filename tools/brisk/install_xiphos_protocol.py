from pathlib import Path
R=Path(__file__).resolve().parents[2]
def edit(path,old,new):
 p=R/path;s=p.read_text(encoding="utf-8")
 assert s.count(old)==1,(path,s.count(old),old[:65])
 p.write_text(s.replace(old,new),encoding="utf-8",newline="\n")

(R/"include/brisk_xiphos.h").write_text(
'''#ifndef GUARD_BRISK_XIPHOS_H
#define GUARD_BRISK_XIPHOS_H
// Enemy-only battlefield modifier. Not a species ability or saved Pokemon data.
bool32 BriskXiphosProtocolReady(enum BattlerId battler);
bool32 BriskXiphosProtocolTrigger(enum BattlerId battler);
#endif
''',encoding="utf-8")
edit("include/battle.h",
"    u16 isFirstTurn:2; // Starts at 2 on switch in and counts down during end turn\n    u16 padding:8;",
"    u16 isFirstTurn:2; // Starts at 2 on switch in and counts down during end turn\n    u16 xiphosProtocolUsed:1; // Per-battler, per-switch-in NPC technology.\n    u16 padding:7;")
edit("src/battle_main.c",
'#include "brisk_battle_format.h"',
'#include "brisk_battle_format.h"\n#include "brisk_xiphos.h"')
edit("src/battle_main.c",
"    gBattleStruct->battlerState[battler].isFirstTurn = 2;",
"    gBattleStruct->battlerState[battler].isFirstTurn = 2;\n    gBattleStruct->battlerState[battler].xiphosProtocolUsed = FALSE;")
edit("src/battle_util.c",
'#include "brisk_options.h"',
'#include "brisk_options.h"\n#include "brisk_xiphos.h"')
p=R/"src/battle_util.c";s=p.read_text(encoding="utf-8")
marker="s32 CalculateMoveDamage(struct DamageContext *ctx)\n{"
assert s.count(marker)==1
implementation='''// Xiphos containment hardware is NOT a real ability: no copied/suppressed/held
// ability or MON_DATA_ABILITY is ever changed.
bool32 BriskXiphosProtocolReady(enum BattlerId battler)
{
    u32 disallowed = BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED
                   | BATTLE_TYPE_RECORDED_LINK | BATTLE_TYPE_FRONTIER
                   | BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_TWO_OPPONENTS
                   | BATTLE_TYPE_MULTI | BATTLE_TYPE_TRAINER_HILL;
    enum Species species;

    if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) || (gBattleTypeFlags & disallowed)
        || IsOnPlayerSide(battler)
        || GetBattlerTrainer(battler) != B_TRAINER_OPPONENT_A
        || gTrainerBattleOpponent_A < TRAINER_XIPHOS_COMMANDER_FAIRY
        || gTrainerBattleOpponent_A > TRAINER_XIPHOS_COMMANDER_DRAGON
        || gBattleStruct->battlerState[battler].xiphosProtocolUsed)
        return FALSE;

    species = gBattleMons[battler].species;
    // Jessie exemptions stay explicit: Soul-Heart and Stance Change remain intact.
    if (species == SPECIES_MAGEARNA || species == SPECIES_MAGEARNA_ORIGINAL
     || species == SPECIES_MAGEARNA_MEGA || species == SPECIES_MAGEARNA_ORIGINAL_MEGA
     || species == SPECIES_AEGISLASH || species == SPECIES_AEGISLASH_BLADE)
        return FALSE;
    return TRUE;
}

bool32 BriskXiphosProtocolTrigger(enum BattlerId battler)
{
    static const enum Stat stages[] = {STAT_ATK, STAT_SPATK, STAT_SPEED};
    if (!BriskXiphosProtocolReady(battler))
        return FALSE;

    gBattleStruct->battlerState[battler].xiphosProtocolUsed = TRUE;
    // The first successful hit was already halved in CalculateMoveDamage.
    // There is no stat gain after a fatal blow.
    if (gBattleMons[battler].hp == 0)
        return FALSE;

    for (u32 i = 0; i < ARRAY_COUNT(stages); i++)
        gBattleMons[battler].statStages[stages[i]]
            = min(MAX_STAT_STAGE, gBattleMons[battler].statStages[stages[i]] + 2);
    return TRUE;
}

'''
s=s.replace(marker,implementation+marker)
old="    return GetAdjustedDamage(ctx, damage);\n}\n\n// for AI so that typeEffectivenessModifier"
assert s.count(old)==1
s=s.replace(old,'''    // Damage prediction and runtime resolve the same first-hit multiplier.
    // Do not consume the shield until a real HP hit (not a Substitute hit).
    if (damage > 0 && BriskXiphosProtocolReady(ctx->battlerDef)
     && !gSpecialStatuses[ctx->battlerAtk].attackerInParty)
        damage = max(1, damage / 2);
    return GetAdjustedDamage(ctx, damage);
}

// for AI so that typeEffectivenessModifier''')
old="    return damage;\n}\n\nstatic inline void MulByTypeEffectiveness"
assert s.count(old)==1
s=s.replace(old,'''    if (damage > 0 && BriskXiphosProtocolReady(ctx->battlerDef)
     && !gSpecialStatuses[ctx->battlerAtk].attackerInParty)
        damage = max(1, damage / 2);
    return damage;
}

static inline void MulByTypeEffectiveness''')
p.write_text(s,encoding="utf-8",newline="\n")
edit("src/battle_move_resolution.c",
'#include "battle_move_resolution.h"',
'#include "battle_move_resolution.h"\n#include "brisk_xiphos.h"')
edit("src/battle_move_resolution.c",
"static bool32 TryMoveDamageUpdate(struct BattleCalcValues *cv)\n{",
"static bool32 TryMoveDamageUpdate(struct BattleCalcValues *cv)\n{\n    bool8 xiphosActivated = FALSE;")
edit("src/battle_move_resolution.c",
"            if (hpLost != 0)\n                gBattleStruct->innardsOutHpLost[cv->battlerDef] += hpLost;",
"""            if (hpLost != 0)
            {
                gBattleStruct->innardsOutHpLost[cv->battlerDef] += hpLost;
                if (!gSpecialStatuses[cv->battlerAtk].attackerInParty)
                    xiphosActivated = BriskXiphosProtocolTrigger(cv->battlerDef);
            }""")
edit("src/battle_move_resolution.c",
"    gSpecialStatuses[cv->battlerDef].damagedByAttack = TRUE;\n    return FALSE;",
"""    gSpecialStatuses[cv->battlerDef].damagedByAttack = TRUE;
    if (xiphosActivated)
    {
        gBattleScripting.battler = cv->battlerDef;
        BattleScriptCall(BattleScript_XiphosContainmentProtocol);
        return TRUE;
    }
    return FALSE;""")
edit("include/constants/battle_string_ids.h",
"    STRINGID_BELCHCANTUSE,\n    STRINGID_COUNT",
"    STRINGID_BELCHCANTUSE,\n    STRINGID_XIPHOSCONTAINMENTPROTOCOL,\n    STRINGID_COUNT")
edit("src/battle_message.c",
"    [STRINGID_TRAINER1LOSETEXT]                     = COMPOUND_STRING(\"{B_TRAINER1_LOSE_TEXT}\"),",
"""    [STRINGID_XIPHOSCONTAINMENTPROTOCOL]            = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s Containment Protocol activated! Its offensive stats and Speed rose sharply!"),
    [STRINGID_TRAINER1LOSETEXT]                     = COMPOUND_STRING("{B_TRAINER1_LOSE_TEXT}"),""")
edit("include/battle_scripts.h",
"extern const u8 BattleScript_PlayMoveAnim[];",
"extern const u8 BattleScript_XiphosContainmentProtocol[];\nextern const u8 BattleScript_PlayMoveAnim[];")
edit("data/battle_scripts_1.s",
"BattleScript_FickleBeamMessage::",
"""BattleScript_XiphosContainmentProtocol::
    printstring STRINGID_XIPHOSCONTAINMENTPROTOCOL
    waitmessage B_WAIT_TIME_LONG
    return

BattleScript_FickleBeamMessage::""")
print("Containment Protocol integrated as a per-switch-in, enemy-only double-battle effect.")
