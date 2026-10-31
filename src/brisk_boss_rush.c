#include "global.h"
#include "brisk_boss_rush.h"
#include "battle.h"
#include "battle_setup.h"
#include "data.h"
#include "event_data.h"
#include "pokemon.h"
#include "script_pokemon_util.h"
#include "random.h"
#include "string_util.h"
#include "constants/battle.h"
#include "constants/brisk_modes.h"
#include "constants/event_objects.h"
#include "constants/vars.h"
#include "constants/opponents.h"
#include "constants/trainers.h"

// Challenge settings remain in EWRAM; nothing new is added to the save.
// Each run shuffles the current save's defeated Hoenn and Kanto trainers.
enum { RUSH_HEAL_NONE, RUSH_HEAL_HALF, RUSH_HEAL_FULL };

static EWRAM_DATA struct
{
    u16 trainers[TRAINERS_COUNT];
    u16 total, current, previousDifficulty;
    u8 difficulty, healAmount, enemyMinLevel;
    bool8 initialized, active, allowBag, cureStatus, restorePP;
    bool8 allowShift, rechargeGimmicks;
} sBossRush;

static void EndChallenge(void)
{
    if (sBossRush.active)
        VarSet(VAR_BRISK_DIFFICULTY, sBossRush.previousDifficulty);
    sBossRush.active = FALSE;
}

bool32 BriskBossRushIsActive(void) { return sBossRush.active; }
bool32 BriskBossRushBagAllowed(void) { return sBossRush.allowBag; }
bool32 BriskBossRushShiftAllowed(void) { return sBossRush.allowShift; }
bool32 BriskBossRushGimmicksRecharge(void) { return sBossRush.rechargeGimmicks; }

u16 BriskBossRushInitRules(void)
{
    if (!sBossRush.initialized)
    {
        sBossRush.initialized = TRUE;
        sBossRush.difficulty = VarGet(VAR_BRISK_DIFFICULTY);
        if (sBossRush.difficulty > BRISK_MODE_KAIZO)
            sBossRush.difficulty = BRISK_MODE_NORMAL;
        sBossRush.healAmount = RUSH_HEAL_FULL;
        sBossRush.allowBag = FALSE;
        sBossRush.cureStatus = TRUE;
        sBossRush.restorePP = TRUE;
        sBossRush.allowShift = FALSE;
        sBossRush.rechargeGimmicks = FALSE;
        sBossRush.enemyMinLevel = 0;
    }
    return TRUE;
}

u16 BriskBossRushBufferRules(void)
{
    static const u8 *const sDifficultyNames[] = {
        [BRISK_MODE_NORMAL] = COMPOUND_STRING("NORMAL"),
        [BRISK_MODE_EASY] = COMPOUND_STRING("EASY"),
        [BRISK_MODE_HARD] = COMPOUND_STRING("HARD"),
        [BRISK_MODE_KAIZO] = COMPOUND_STRING("KAIZO"),
    };
    static const u8 *const sHealNames[] = {
        COMPOUND_STRING("NONE"),
        COMPOUND_STRING("HALF HP"),
        COMPOUND_STRING("FULL HP"),
    };
    BriskBossRushInitRules();
    StringCopy(gStringVar1, sHealNames[sBossRush.healAmount]);
    StringCopy(gStringVar2, sBossRush.allowBag ? COMPOUND_STRING("ON") : COMPOUND_STRING("OFF"));
    StringCopy(gStringVar3, sDifficultyNames[sBossRush.difficulty]);
    return TRUE;
}

u16 BriskBossRushBufferExtras(void)
{
    static const u8 *const sLevels[] = {
        COMPOUND_STRING("ORIGINAL"),
        COMPOUND_STRING("LV. 50+"),
        COMPOUND_STRING("LV. 100"),
    };
    BriskBossRushInitRules();
    StringCopy(gStringVar1, sBossRush.cureStatus ? COMPOUND_STRING("ON") : COMPOUND_STRING("OFF"));
    StringCopy(gStringVar2, sBossRush.restorePP ? COMPOUND_STRING("ON") : COMPOUND_STRING("OFF"));
    StringCopy(gStringVar3, sLevels[sBossRush.enemyMinLevel]);
    return TRUE;
}

u16 BriskBossRushBufferBattleRules(void)
{
    StringCopy(gStringVar1, sBossRush.allowShift ? COMPOUND_STRING("SHIFT") : COMPOUND_STRING("SET"));
    StringCopy(gStringVar2, sBossRush.rechargeGimmicks ? COMPOUND_STRING("ON") : COMPOUND_STRING("OFF"));
    return TRUE;
}

u16 BriskBossRushToggleHeal(void)
{
    sBossRush.healAmount = (sBossRush.healAmount + 1) % 3;
    return sBossRush.healAmount;
}

u16 BriskBossRushToggleBag(void)
{
    sBossRush.allowBag = !sBossRush.allowBag;
    return sBossRush.allowBag;
}

u16 BriskBossRushToggleStatus(void)
{
    sBossRush.cureStatus = !sBossRush.cureStatus;
    return sBossRush.cureStatus;
}

u16 BriskBossRushTogglePP(void)
{
    sBossRush.restorePP = !sBossRush.restorePP;
    return sBossRush.restorePP;
}

u16 BriskBossRushCycleDifficulty(void)
{
    sBossRush.difficulty = (sBossRush.difficulty + 1) % (BRISK_MODE_KAIZO + 1);
    return sBossRush.difficulty;
}

u16 BriskBossRushCycleEnemyLevels(void)
{
    sBossRush.enemyMinLevel = (sBossRush.enemyMinLevel + 1) % 3;
    return sBossRush.enemyMinLevel;
}

u16 BriskBossRushToggleBattleStyle(void)
{
    sBossRush.allowShift = !sBossRush.allowShift;
    return sBossRush.allowShift;
}

u16 BriskBossRushToggleGimmicks(void)
{
    sBossRush.rechargeGimmicks = !sBossRush.rechargeGimmicks;
    return sBossRush.rechargeGimmicks;
}

void BriskBossRushAdjustOpponentParty(void)
{
    u32 i;
    u8 minLevel = sBossRush.enemyMinLevel == 2 ? 100 : sBossRush.enemyMinLevel == 1 ? 50 : 0;
    if (!sBossRush.active || !minLevel)
        return;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_OPPONENT_A][i];
        enum Species species = GetMonData(mon, MON_DATA_SPECIES);
        u8 level = GetMonData(mon, MON_DATA_LEVEL);
        if (species == SPECIES_NONE || level >= minLevel)
            continue;
        // EXP (not just the cached level) controls CalculateMonStats().
        u32 exp = gExperienceTables[gSpeciesInfo[species].growthRate][minLevel];
        SetMonData(mon, MON_DATA_EXP, &exp);
        CalculateMonStats(mon);
    }
}

static void RestorePartyBetweenRounds(void)
{
    u32 i, moveIndex;
    for (i = 0; i < gPartiesCount[B_TRAINER_PLAYER]; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];
        u32 species = GetMonData(mon, MON_DATA_SPECIES);
        if (species == SPECIES_NONE || GetMonData(mon, MON_DATA_IS_EGG))
            continue;

        if (sBossRush.healAmount != RUSH_HEAL_NONE)
        {
            u16 hp = GetMonData(mon, MON_DATA_HP);
            u16 maxHP = GetMonData(mon, MON_DATA_MAX_HP);
            if (sBossRush.healAmount == RUSH_HEAL_FULL)
                hp = maxHP;
            else
                hp = min(maxHP, hp + max(1, maxHP / 2));
            SetMonData(mon, MON_DATA_HP, &hp);
        }
        if (sBossRush.cureStatus)
        {
            u32 status = 0;
            SetMonData(mon, MON_DATA_STATUS, &status);
        }
        if (sBossRush.restorePP)
        {
            u8 bonuses = GetMonData(mon, MON_DATA_PP_BONUSES);
            for (moveIndex = 0; moveIndex < MAX_MON_MOVES; moveIndex++)
            {
                enum Move move = GetMonData(mon, MON_DATA_MOVE1 + moveIndex);
                u8 pp = CalculatePPWithBonus(move, bonuses, moveIndex);
                SetMonData(mon, MON_DATA_PP1 + moveIndex, &pp);
            }
        }
    }
}

u16 BriskBossRushPrepare(void)
{
    u32 id, mode, i;
    BriskBossRushInitRules();
    EndChallenge();
    sBossRush.total = 0;
    sBossRush.current = 0;

    for (id = 1; id < TRAINERS_COUNT; id++)
    {
        bool32 valid = FALSE;
        if (!HasTrainerBeenFought(id))
            continue;
        for (mode = 0; mode < DIFFICULTY_COUNT; mode++)
        {
            if (gTrainers[mode][id].partySize || gTrainers[mode][id].poolSize)
            {
                valid = TRUE;
                break;
            }
        }
        if (valid)
            sBossRush.trainers[sBossRush.total++] = id;
    }
    if (sBossRush.total == 0)
        return 0;

    for (i = sBossRush.total; i > 1; i--)
    {
        u32 j = Random() % i;
        u16 temp = sBossRush.trainers[i - 1];
        sBossRush.trainers[i - 1] = sBossRush.trainers[j];
        sBossRush.trainers[j] = temp;
    }
    sBossRush.previousDifficulty = VarGet(VAR_BRISK_DIFFICULTY);
    VarSet(VAR_BRISK_DIFFICULTY, sBossRush.difficulty);
    sBossRush.active = TRUE;
    return sBossRush.total;
}

u16 BriskBossRushCheckActive(void)
{
    return sBossRush.active && sBossRush.current < sBossRush.total;
}

u16 BriskBossRushBufferNext(void)
{
    if (!BriskBossRushCheckActive())
        return FALSE;
    StringCopyN(gStringVar1, gTrainers[sBossRush.difficulty][sBossRush.trainers[sBossRush.current]].trainerName, TRAINER_NAME_LENGTH + 1);
    ConvertIntToDecimalStringN(gStringVar2, sBossRush.current + 1, STR_CONV_MODE_LEFT_ALIGN, 4);
    ConvertIntToDecimalStringN(gStringVar3, sBossRush.total, STR_CONV_MODE_LEFT_ALIGN, 4);
    return TRUE;
}

u16 BriskBossRushFight(void)
{
    if (!BriskBossRushCheckActive())
        return FALSE;
    InitTrainerBattleParameter();
    TRAINER_BATTLE_PARAM.opponentA = sBossRush.trainers[sBossRush.current];
    TRAINER_BATTLE_PARAM.objEventLocalIdA = LOCALID_NONE;
    TRAINER_BATTLE_PARAM.skipFlagCheck = TRUE;
    TRAINER_BATTLE_PARAM.playMusicA = TRUE;
    BattleSetup_StartTrainerBattle();
    return TRUE;
}

u16 BriskBossRushEndRound(void)
{
    if (!BriskBossRushCheckActive())
        return 0;
    if (gBattleOutcome != B_OUTCOME_WON)
    {
        HealPlayerParty();
        EndChallenge();
        return 0;
    }
    sBossRush.current++;
    if (sBossRush.current >= sBossRush.total)
    {
        EndChallenge();
        return 2;
    }
    RestorePartyBetweenRounds();
    return 1;
}

u16 BriskBossRushForfeit(void)
{
    EndChallenge();
    return TRUE;
}
