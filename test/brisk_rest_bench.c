#include "global.h"
#include "pokemon.h"
#include "script.h"
#include "test/test.h"
#include "test/overworld_script.h"
#include "constants/battle.h"

TEST("Rest benches revive and fully heal all six Pokemon without changing the whiteout destination")
{
    struct Pokemon backup[PARTY_SIZE];
    u8 oldCount = gPartiesCount[B_TRAINER_PLAYER];
    struct WarpData oldHeal = gSaveBlock1Ptr->lastHealLocation;
    memcpy(backup, gParties[B_TRAINER_PLAYER], sizeof(backup));
    gPartiesCount[B_TRAINER_PLAYER] = PARTY_SIZE;
    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];
        u16 hp = i == 0 ? 0 : 1;
        u32 status = i % 2 ? STATUS1_TOXIC_POISON | STATUS1_TOXIC_TURN(4) : STATUS1_PARALYSIS;
        u8 zero = 0, bonuses = 0xFF;
        CreateMonWithIVs(mon, SPECIES_FLYGON, 50, 0, OTID_STRUCT_PLAYER_ID, 31);
        SetMonMoveSlot(mon, MOVE_TACKLE, 0);
        SetMonMoveSlot(mon, MOVE_EMBER, 1);
        SetMonMoveSlot(mon, MOVE_WATER_GUN, 2);
        SetMonMoveSlot(mon, MOVE_RECOVER, 3);
        SetMonData(mon, MON_DATA_HP, &hp);
        SetMonData(mon, MON_DATA_STATUS, &status);
        SetMonData(mon, MON_DATA_PP_BONUSES, &bonuses);
        for (u32 j = 0; j < MAX_MON_MOVES; j++)
            SetMonData(mon, MON_DATA_PP1 + j, &zero);
    }

    // Exercise the exact subroutine called by the interactive bench script.
    RUN_OVERWORLD_SCRIPT(call EventScript_RestBench_HealParty;);

    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];
        EXPECT_EQ(GetMonData(mon, MON_DATA_HP), GetMonData(mon, MON_DATA_MAX_HP));
        EXPECT_EQ(GetMonData(mon, MON_DATA_STATUS), 0);
        for (u32 j = 0; j < MAX_MON_MOVES; j++)
            EXPECT_EQ(GetMonData(mon, MON_DATA_PP1 + j), CalculatePPWithBonus(GetMonData(mon, MON_DATA_MOVE1 + j), 0xFF, j));
    }
    EXPECT_EQ(memcmp(&oldHeal, &gSaveBlock1Ptr->lastHealLocation, sizeof(oldHeal)), 0);
    memcpy(gParties[B_TRAINER_PLAYER], backup, sizeof(backup));
    gPartiesCount[B_TRAINER_PLAYER] = oldCount;
}
