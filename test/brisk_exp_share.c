#include "global.h"
#include "brisk_modes.h"
#include "caps.h"
#include "event_data.h"
#include "item.h"
#include "test/battle.h"

WILD_BATTLE_TEST("Brisk EXP Share rewards use difficulty percentages without reducing active EXP")
{
    u32 mode = 0;
    bool32 modern = FALSE;
    for (u32 difficulty = 0; difficulty < 4; difficulty++)
    {
        PARAMETRIZE { mode = difficulty; modern = FALSE; }
        PARAMETRIZE { mode = difficulty; modern = TRUE; }
    }
    GIVEN {
        WITH_CONFIG(B_SCALED_EXP, GEN_3);
        WITH_CONFIG(B_UNEVOLVED_EXP_MULTIPLIER, GEN_3);
        WITH_CONFIG(B_AFFECTION_MECHANICS, FALSE);
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        VarSet(VAR_BRISK_NO_LEVEL_CAP, 1);
        VarSet(VAR_BRISK_EXP_SHARE, modern ? 1 : 0);
        GIVE_PLAYER_ITEM(ITEM_MODERN_EXP_SHARE, 1);
        PLAYER(SPECIES_WOBBUFFET) { Level(10); Moves(MOVE_SCRATCH); }
        PLAYER(SPECIES_WOBBUFFET) { Level(10); Item(modern ? ITEM_NONE : ITEM_EXP_SHARE); }
        OPPONENT(SPECIES_CATERPIE) { Level(7); HP(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        MESSAGE("Wobbuffet used Scratch!");
        MESSAGE("The wild Caterpie fainted!");
        EXPERIENCE_BAR(player);
    } THEN {
        u32 baseExp = gSpeciesInfo[SPECIES_CATERPIE].expYield * 7 / 7;
        u32 startExp = gExperienceTables[gSpeciesInfo[SPECIES_WOBBUFFET].growthRate][10];
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_EXP), startExp + baseExp);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_EXP), startExp + ScaleBriskSharedExp(baseExp));
        VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
        VarSet(VAR_BRISK_NO_LEVEL_CAP, 0);
        VarSet(VAR_BRISK_EXP_SHARE, 0);
    }
}

SINGLE_BATTLE_TEST("Brisk item restrictions keep opposing trainer item actions working")
{
    u32 mode = 0;
    PARAMETRIZE { mode = BRISK_MODE_HARD; }
    PARAMETRIZE { mode = BRISK_MODE_EXTREME; }
    GIVEN {
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); MaxHP(400); }
    } WHEN {
        TURN { USE_ITEM(opponent, ITEM_POTION, partyIndex: 0); }
    } SCENE {
        HP_BAR(opponent, damage: -20);
    } THEN {
        VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
    }
}

WILD_BATTLE_TEST("Brisk Hard and Extreme battle EXP stop exactly at their cap including boosted rewards")
{
    s32 offset = 0;
    u32 difficulty = BRISK_MODE_EXTREME;
    for (u32 m = BRISK_MODE_HARD; m <= BRISK_MODE_EXTREME; m++)
        for (s32 n = -1; n <= 1; n++)
            PARAMETRIZE {offset = n; difficulty = m;}
    u32 cap = 0;
    GIVEN {
        WITH_CONFIG(B_SCALED_EXP, GEN_3);
        VarSet(VAR_BRISK_DIFFICULTY, difficulty);
        VarSet(VAR_BRISK_NO_LEVEL_CAP, 0);
        cap = GetCurrentLevelCap();
        PLAYER(SPECIES_WOBBUFFET) { Level(cap + offset); Item(ITEM_LUCKY_EGG); }
        OPPONENT(SPECIES_BLISSEY) { Level(100); Moves(MOVE_MEMENTO); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_MEMENTO); }
    } SCENE {
        if (offset < 0) { EXPERIENCE_BAR(player); }
        else { NOT EXPERIENCE_BAR(player); }
    } THEN {
        u32 cap = GetCurrentLevelCap();
        u32 finalLevel = offset < 0 ? cap : cap + offset;
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_LEVEL), finalLevel);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_EXP), gExperienceTables[gSpeciesInfo[SPECIES_WOBBUFFET].growthRate][finalLevel]);
        VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
    }
}
