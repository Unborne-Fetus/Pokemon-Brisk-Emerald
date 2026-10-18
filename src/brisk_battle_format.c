#include "global.h"
#include "brisk_options.h"
#include "brisk_battle_format.h"
#include "battle.h"
#include "battle_controllers.h"
#include "battle_ai_main.h"
#include "battle_util.h"
#include "dynamax_adventure.h"
#include "event_data.h"

static EWRAM_DATA bool8 sQueuedSecondTrainer = FALSE;
static EWRAM_DATA bool8 sFormatApplied = FALSE;
static EWRAM_DATA bool8 sOriginallyTwoTrainers = FALSE;
#if TESTING
static bool8 sTestConvertRecordedBattle;
void Test_BriskConvertRecordedBattle(bool32 enabled)
{
    sTestConvertRecordedBattle = enabled;
}
#endif

bool32 BriskBattleHadTwoOpponents(void)
{
    return sOriginallyTwoTrainers || (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS);
}

u32 GetBriskBattleFormat(void)
{
    if (gSaveBlock1Ptr == NULL) return BRISK_BATTLE_REGULAR;
    u32 mode = VarGet(VAR_BRISK_BATTLE_FORMAT);
    return mode <= BRISK_BATTLE_DOUBLES ? mode : BRISK_BATTLE_REGULAR;
}

bool32 BriskPokemonFollowersEnabled(void)
{
    return gSaveBlock1Ptr != NULL && VarGet(VAR_BRISK_FOLLOWERS_DISABLED) != 1;
}

void BriskPrepareBattleFormat(void)
{
    sQueuedSecondTrainer = FALSE;
    sFormatApplied = FALSE;
    sOriginallyTwoTrainers = FALSE;
    if (IsDynamaxAdventureBattle())
        return; // Rentals and the sole boss have a dedicated 2v1 format.
    // Network peers and saved recordings must agree on a format. Safari and
    // catching demonstrations use dedicated controllers rather than move combat.
    if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_SAFARI | BATTLE_TYPE_CATCH_TUTORIAL | BATTLE_TYPE_POKEDUDE)
     || ((gBattleTypeFlags & (BATTLE_TYPE_RECORDED | BATTLE_TYPE_RECORDED_LINK))
#if TESTING
      && !sTestConvertRecordedBattle
#endif
     ))
        return;
    u32 mode = GetBriskBattleFormat();
    if (mode == BRISK_BATTLE_REGULAR) return;
    sFormatApplied = TRUE;
    if (mode == BRISK_BATTLE_SINGLES)
    {
        sQueuedSecondTrainer = (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS) != 0;
        sOriginallyTwoTrainers = sQueuedSecondTrainer;
        gBattleTypeFlags &= ~(BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI | BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_TWO_OPPONENTS);
        // Both approaching trainers' return scripts and story flags are kept
        // by battle_setup; their generated parties are fought sequentially.
    }
    else
        gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;
}

void BriskPrepareBattleParties(void)
{
    if (!sFormatApplied) return;
    if (GetBriskBattleFormat() == BRISK_BATTLE_SINGLES && !(gBattleTypeFlags & BATTLE_TYPE_TRAINER))
    {
        for (u32 i = 1; i < PARTY_SIZE; i++)
            ZeroMonData(&gParties[B_TRAINER_OPPONENT_A][i]);
        CalculatePartyCount(B_TRAINER_OPPONENT_A);
    }
    if (GetBriskBattleFormat() == BRISK_BATTLE_DOUBLES
     && !(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && !IsDynamaxAdventureBattle()
     && IsValidForBattle(&gParties[B_TRAINER_OPPONENT_A][0])
     && !IsValidForBattle(&gParties[B_TRAINER_OPPONENT_A][1]))
    {
        struct Pokemon *first = &gParties[B_TRAINER_OPPONENT_A][0];
        BriskSetWildShinyContext(TRUE);
        CreateRandomMon(&gParties[B_TRAINER_OPPONENT_A][1], GetMonData(first, MON_DATA_SPECIES), GetMonData(first, MON_DATA_LEVEL));
        BriskSetWildShinyContext(FALSE);
        CalculatePartyCount(B_TRAINER_OPPONENT_A);
    }
}

bool32 BriskRefillSinglesOpponent(void)
{
    if (!sQueuedSecondTrainer) return FALSE;
    for (u32 i = 0; i < PARTY_SIZE; i++)
        if (IsValidForBattle(&gParties[B_TRAINER_OPPONENT_A][i])) return FALSE;
    u32 next = 0, added = 0;
    u32 current = gBattlerPartyIndexes[GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)];
    // Preserve the current fainted mon until its EXP/script processing ends.
    // At most five reserves move across; a sixth waits for the next refill.
    for (u32 n = 1; n < PARTY_SIZE; n++)
    {
        while (next < PARTY_SIZE && !IsValidForBattle(&gParties[B_TRAINER_OPPONENT_B][next])) next++;
        if (next == PARTY_SIZE) break;
        u32 slot = (current + n) % PARTY_SIZE;
        gParties[B_TRAINER_OPPONENT_A][slot] = gParties[B_TRAINER_OPPONENT_B][next];
        ZeroMonData(&gParties[B_TRAINER_OPPONENT_B][next++]);
        if (gBattleStruct != NULL)
        {
            memset(&gBattleStruct->partyState[B_TRAINER_OPPONENT_A][slot], 0, sizeof(struct PartyState));
            gBattleStruct->givenExpMons[0] &= ~(1u << slot);
        }
        added++;
    }
    sQueuedSecondTrainer = FALSE;
    for (u32 i = 0; i < PARTY_SIZE; i++)
        if (IsValidForBattle(&gParties[B_TRAINER_OPPONENT_B][i])) sQueuedSecondTrainer = TRUE;
    if (added)
    {
        CalculatePartyCount(B_TRAINER_OPPONENT_A);
        if (gAiPartyData != NULL) Ai_InitPartyStruct();
    }
    return added != 0;
}
