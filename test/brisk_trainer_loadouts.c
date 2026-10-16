#include "global.h"
#include "brisk_modes.h"
#include "brisk_trainer_moves.h"
#include "event_data.h"
#include "move.h"
#include "pokemon.h"
#include "random.h"
#include "test/test.h"

static void MakeLoadoutMon(struct Pokemon *mon, enum Species species)
{
    VarSet(VAR_BRISK_RANDOMIZER, 0);
    CreateMonWithIVs(mon, species, 50, 0, OTID_STRUCT_PRESET(0), 0);
    GiveMonInitialMoveset(mon);
}

TEST("Brisk trainer loadouts preserve Normal exactly and only change Hard abilities")
{
    struct Pokemon mon;
    MakeLoadoutMon(&mon, SPECIES_AZUMARILL);
    SetMonMoveSlot(&mon, MOVE_WATER_GUN, 0);
    SetMonMoveSlot(&mon, MOVE_PLAY_ROUGH, 1);
    u32 slot = 0;
    SetMonData(&mon, MON_DATA_ABILITY_NUM, &slot);
    struct Pokemon original = mon;
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
    ApplyBriskTrainerLoadout(&mon);
    EXPECT_EQ(memcmp(&mon, &original, sizeof(mon)), 0);
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_HARD);
    ApplyBriskTrainerLoadout(&mon);
    EXPECT_EQ(GetMonAbility(&mon), ABILITY_HUGE_POWER);
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
    {
        EXPECT_EQ(GetMonData(&mon, MON_DATA_MOVE1 + i), GetMonData(&original, MON_DATA_MOVE1 + i));
        EXPECT_EQ(GetMonData(&mon, MON_DATA_PP1 + i), GetMonData(&original, MON_DATA_PP1 + i));
    }
    EXPECT_EQ(GetMonData(&mon, MON_DATA_IVS), GetMonData(&original, MON_DATA_IVS));
    EXPECT_EQ(GetMonData(&mon, MON_DATA_EXP), GetMonData(&original, MON_DATA_EXP));
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
}

TEST("Brisk move viability accounts for STAB, power, accuracy, recharge and secondary effects")
{
    struct Pokemon mon;
    MakeLoadoutMon(&mon, SPECIES_CHARIZARD);
    EXPECT_GT(Test_BriskTrainerMoveScore(&mon, MOVE_FLAMETHROWER), Test_BriskTrainerMoveScore(&mon, MOVE_EMBER));
    EXPECT_GT(Test_BriskTrainerMoveScore(&mon, MOVE_FLAMETHROWER), Test_BriskTrainerMoveScore(&mon, MOVE_THUNDERBOLT));
    EXPECT_GT(Test_BriskTrainerMoveScore(&mon, MOVE_SURF), Test_BriskTrainerMoveScore(&mon, MOVE_HYDRO_PUMP));
    EXPECT_GT(Test_BriskTrainerMoveScore(&mon, MOVE_THUNDERBOLT), Test_BriskTrainerMoveScore(&mon, MOVE_HYPER_BEAM));
    EXPECT_GT(Test_BriskTrainerMoveScore(&mon, MOVE_BODY_SLAM), Test_BriskTrainerMoveScore(&mon, MOVE_STRENGTH));
    EXPECT_LT(Test_BriskTrainerMoveScore(&mon, MOVE_GROWL), 0);
    MakeLoadoutMon(&mon, SPECIES_SCIZOR);
    EXPECT_GT(Test_BriskTrainerMoveScore(&mon, MOVE_IRON_HEAD), Test_BriskTrainerMoveScore(&mon, MOVE_FLASH_CANNON));
    u32 ability = 0;
    for (u32 i = 0; i < NUM_ABILITY_SLOTS; i++)
        if (GetAbilityBySpecies(SPECIES_SCIZOR, i) == ABILITY_TECHNICIAN) ability = i;
    s32 before = Test_BriskTrainerMoveScore(&mon, MOVE_BULLET_PUNCH);
    SetMonData(&mon, MON_DATA_ABILITY_NUM, &ability);
    EXPECT_GT(Test_BriskTrainerMoveScore(&mon, MOVE_BULLET_PUNCH), before);
}

TEST("Brisk Easy and Extreme use legal unique moves and Extreme has stronger attacks")
{
    static const u16 species[] = {SPECIES_CHARIZARD, SPECIES_AZUMARILL, SPECIES_GOLEM, SPECIES_ALAKAZAM, SPECIES_SCIZOR, SPECIES_SHUCKLE, SPECIES_WOBBUFFET, SPECIES_MAGIKARP};
    for (u32 n = 0; n < ARRAY_COUNT(species); n++)
    {
        struct Pokemon original, easy, extreme;
        MakeLoadoutMon(&original, species[n]);
        easy = extreme = original;
        VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_EASY);
        ApplyBriskTrainerLoadout(&easy);
        VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_EXTREME);
        ApplyBriskTrainerLoadout(&extreme);
        s32 easyScore = 0, extremeScore = 0;
        u32 easyCount = 0, extremeCount = 0;
        for (u32 kind = 0; kind < 2; kind++)
        {
            struct Pokemon *mon = kind ? &extreme : &easy;
            bool32 abilityLegal = FALSE;
            for (u32 i = 0; i < NUM_ABILITY_SLOTS; i++)
                if (GetMonAbility(mon) == GetAbilityBySpecies(species[n], i)) abilityLegal = TRUE;
            EXPECT(abilityLegal);
            for (u32 i = 0; i < MAX_MON_MOVES; i++)
            {
                enum Move move = GetMonData(mon, MON_DATA_MOVE1 + i);
                if (move == MOVE_NONE) continue;
                EXPECT(Test_BriskTrainerMoveLegal(species[n], 50, move));
                EXPECT_EQ(GetMonData(mon, MON_DATA_PP1 + i), GetMovePP(move));
                for (u32 j = 0; j < i; j++) EXPECT_NE(move, GetMonData(mon, MON_DATA_MOVE1 + j));
                if (!kind) EXPECT_NE(GetMoveCategory(move), DAMAGE_CATEGORY_STATUS);
                s32 score = Test_BriskTrainerMoveScore(mon, move);
                if (score < 0) continue;
                if (kind) { extremeScore += score; extremeCount++; }
                else { easyScore += score; easyCount++; }
            }
        }
        EXPECT(easyCount > 0);
        EXPECT(extremeCount > 0);
        EXPECT_GE(extremeScore * easyCount, easyScore * extremeCount);
    }
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
}

TEST("Brisk loadouts keep Ditto functional and preserve useful authored support")
{
    struct Pokemon mon;
    MakeLoadoutMon(&mon, SPECIES_DITTO);
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_EASY);
    ApplyBriskTrainerLoadout(&mon);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_MOVE1), MOVE_TRANSFORM);
    MakeLoadoutMon(&mon, SPECIES_SLOWBRO);
    SetMonMoveSlot(&mon, MOVE_SLACK_OFF, 0);
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_EXTREME);
    ApplyBriskTrainerLoadout(&mon);
    bool32 hasRecovery = FALSE;
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
        if (GetMonData(&mon, MON_DATA_MOVE1 + i) == MOVE_SLACK_OFF) hasRecovery = TRUE;
    EXPECT(hasRecovery);
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
}

TEST("Brisk trainer loadouts are deterministic and do not consume the global RNG")
{
    struct Pokemon first, second;
    MakeLoadoutMon(&first, SPECIES_CHARIZARD);
    second = first;
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_EXTREME);
    SeedRng(1234);
    u32 expected = Random();
    SeedRng(1234);
    ApplyBriskTrainerLoadout(&first);
    EXPECT_EQ(Random(), expected);
    ApplyBriskTrainerLoadout(&second);
    EXPECT_EQ(memcmp(&first, &second, sizeof(first)), 0);
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
}
