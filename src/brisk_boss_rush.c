#include "global.h"
#include "brisk_boss_rush.h"
#include "battle.h"
#include "battle_setup.h"
#include "data.h"
#include "event_data.h"
#include "pokemon.h"
#include "random.h"
#include "string_util.h"
#include "constants/battle.h"
#include "constants/brisk_modes.h"
#include "constants/event_objects.h"
#include "constants/vars.h"
#include "constants/opponents.h"
#include "constants/trainers.h"

// A new shuffle is built from existing Hoenn and Kanto defeat flags on each
// entry. This EWRAM state does not enlarge the save file.
static EWRAM_DATA struct
{
    u16 trainers[TRAINERS_COUNT];
    u16 total, current, previousDifficulty;
    u8 difficulty;
    bool8 initialized, active, healBetween, allowBag;
} sBossRush;

static void EndChallenge(void)
{
    if (sBossRush.active)
        VarSet(VAR_BRISK_DIFFICULTY, sBossRush.previousDifficulty);
    sBossRush.active = FALSE;
}

bool32 BriskBossRushIsActive(void) { return sBossRush.active; }
bool32 BriskBossRushBagAllowed(void) { return sBossRush.allowBag; }

u16 BriskBossRushInitRules(void)
{
    if (!sBossRush.initialized)
    {
        sBossRush.initialized = TRUE;
        sBossRush.difficulty = VarGet(VAR_BRISK_DIFFICULTY);
        if (sBossRush.difficulty > BRISK_MODE_KAIZO)
            sBossRush.difficulty = BRISK_MODE_NORMAL;
        sBossRush.healBetween = TRUE;
        sBossRush.allowBag = FALSE;
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
    BriskBossRushInitRules();
    StringCopy(gStringVar1, sBossRush.healBetween ? COMPOUND_STRING("ON") : COMPOUND_STRING("OFF"));
    StringCopy(gStringVar2, sBossRush.allowBag ? COMPOUND_STRING("ON") : COMPOUND_STRING("OFF"));
    StringCopy(gStringVar3, sDifficultyNames[sBossRush.difficulty]);
    return TRUE;
}

u16 BriskBossRushToggleHeal(void)
{
    sBossRush.healBetween = !sBossRush.healBetween;
    return sBossRush.healBetween;
}

u16 BriskBossRushToggleBag(void)
{
    sBossRush.allowBag = !sBossRush.allowBag;
    return sBossRush.allowBag;
}

u16 BriskBossRushCycleDifficulty(void)
{
    sBossRush.difficulty = (sBossRush.difficulty + 1) % (BRISK_MODE_KAIZO + 1);
    return sBossRush.difficulty;
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
    if (sBossRush.healBetween)
        HealPlayerParty();
    return 1;
}

u16 BriskBossRushForfeit(void)
{
    EndChallenge();
    return TRUE;
}
