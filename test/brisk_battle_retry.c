#include "global.h"
#include "battle.h"
#include "brisk_battle_retry.h"
#include "brisk_modes.h"
#include "brisk_trainer_moves.h"
#include "event_data.h"
#include "item.h"
#include "move.h"
#include "pokemon.h"
#include "test/test.h"
#include "constants/battle_ai.h"

TEST("Brisk retry restores original parties and Bag, fully heals player, and rolls back EXP")
{
    struct Pokemon saved[MAX_BATTLE_TRAINERS][PARTY_SIZE];
    memcpy(saved, gParties, sizeof(saved));
    struct Bag bag = gSaveBlock1Ptr->bag;
    u32 flags = gBattleTypeFlags;
    bool32 charge = FlagGet(B_FLAG_TERA_ORB_CHARGED);
    memset(gParties, 0, sizeof(gParties));
    CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][0], SPECIES_PIKACHU, 20, 0, OTID_STRUCT_PRESET(0), 0);
    CreateMonWithIVs(&gParties[B_TRAINER_OPPONENT_A][0], SPECIES_GEODUDE, 22, 0, OTID_STRUCT_PRESET(0), 0);
    CreateMonWithIVs(&gParties[B_TRAINER_OPPONENT_B][0], SPECIES_ZUBAT, 21, 0, OTID_STRUCT_PRESET(0), 0);
    SetMonMoveSlot(&gParties[B_TRAINER_PLAYER][0], MOVE_THUNDER_SHOCK, 0);
    u32 zero = 0;
    SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP, &zero);
    SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_PP1, &zero);
    u32 originalExp = GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_EXP);
    struct Pokemon enemy = gParties[B_TRAINER_OPPONENT_B][0];
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_OPPONENTS;
    FlagSet(B_FLAG_TERA_ORB_CHARGED);
    BriskCaptureBattleRetry();
    EXPECT(BriskBattleRetryAvailable());
    memset(gParties, 0, sizeof(gParties));
    EXPECT(AddBagItem(ITEM_POTION, 1));
    FlagClear(B_FLAG_TERA_ORB_CHARGED);
    BriskRestoreBattleRetry();
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_EXP), originalExp);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP), GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_MAX_HP));
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_PP1), GetMovePP(MOVE_THUNDER_SHOCK));
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_STATUS), 0);
    EXPECT_EQ(memcmp(&enemy, &gParties[B_TRAINER_OPPONENT_B][0], sizeof(enemy)), 0);
    EXPECT_EQ(memcmp(&bag, &gSaveBlock1Ptr->bag, sizeof(bag)), 0);
    EXPECT(FlagGet(B_FLAG_TERA_ORB_CHARGED));
    BriskRestoreBattleRetry(); // Another attempt restores the same checkpoint.
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_EXP), originalExp);
    memcpy(gParties, saved, sizeof(saved));
    for (u32 t = 0; t < MAX_BATTLE_TRAINERS; t++) CalculatePartyCount(t);
    gBattleTypeFlags = flags;
    if (charge) FlagSet(B_FLAG_TERA_ORB_CHARGED); else FlagClear(B_FLAG_TERA_ORB_CHARGED);
}

TEST("Brisk retry rejects special battle lifecycles and invalidates a previous checkpoint")
{
    u32 saved = gBattleTypeFlags;
    const u32 excluded[] = {BATTLE_TYPE_LINK, BATTLE_TYPE_RECORDED, BATTLE_TYPE_FRONTIER,
        BATTLE_TYPE_SAFARI, BATTLE_TYPE_CATCH_TUTORIAL, BATTLE_TYPE_FIRST_BATTLE,
        BATTLE_TYPE_INGAME_PARTNER, BATTLE_TYPE_RAID, BATTLE_TYPE_ROAMER, BATTLE_TYPE_TRAINER_HILL};
    for (u32 i = 0; i < ARRAY_COUNT(excluded); i++)
    {
        gBattleTypeFlags = BATTLE_TYPE_TRAINER;
        BriskCaptureBattleRetry();
        EXPECT(BriskBattleRetryAvailable());
        gBattleTypeFlags |= excluded[i];
        BriskCaptureBattleRetry();
        EXPECT(!BriskBattleRetryAvailable());
    }
    gBattleTypeFlags = saved;
}

TEST("Brisk Hard ordinary loadouts improve weak coverage within the early power limit and enable switching")
{
    u32 old = VarGet(VAR_BRISK_DIFFICULTY);
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_HARD);
    struct Pokemon mon;
    CreateMonWithIVs(&mon, SPECIES_PIKACHU, 18, 0, OTID_STRUCT_PRESET(0), 0);
    SetMonMoveSlot(&mon, MOVE_SPLASH, 0);
    ApplyBriskFairTrainerLoadout(&mon);
    bool32 attack = FALSE;
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
    {
        enum Move move = GetMonData(&mon, MON_DATA_MOVE1 + i);
        if (move == MOVE_NONE) continue;
        EXPECT(Test_BriskTrainerMoveLegal(SPECIES_PIKACHU, 18, move));
        EXPECT_LE(GetMovePower(move), 60);
        if (GetMoveCategory(move) != DAMAGE_CATEGORY_STATUS) attack = TRUE;
    }
    EXPECT(attack);
    u64 ai = GetBriskTrainerAiFlags(0);
    EXPECT(ai & AI_FLAG_SMART_SWITCHING);
    EXPECT(ai & AI_FLAG_HP_AWARE);
    EXPECT(ai & AI_FLAG_TRY_TO_2HKO);
    EXPECT(ai & AI_FLAG_OMNISCIENT);
    VarSet(VAR_BRISK_DIFFICULTY, old);
}
