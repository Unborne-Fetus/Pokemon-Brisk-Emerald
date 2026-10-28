#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(GetMoveEffect(MOVE_WEB_EXCRETANT) == EFFECT_STAT_CHANGE);
    ASSUME(GetMoveTarget(MOVE_WEB_EXCRETANT) == TARGET_FOES_AND_ALLY);
}

SINGLE_BATTLE_TEST("Web Excretant boosts Spidops's Speed twice and lowers only the foe's Evasion")
{
    GIVEN {
        PLAYER(SPECIES_SPIDOPS);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_WEB_EXCRETANT); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_WEB_EXCRETANT, player);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 2);
        EXPECT_EQ(player->statStages[STAT_EVASION], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponent->statStages[STAT_EVASION], DEFAULT_STAT_STAGE - 1);
        EXPECT_EQ(opponent->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
    }
}

DOUBLE_BATTLE_TEST("Web Excretant lowers Evasion of foes and ally without lowering its user's Evasion")
{
    GIVEN {
        PLAYER(SPECIES_SPIDOPS);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_WEB_EXCRETANT); MOVE(playerRight, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_WEB_EXCRETANT, playerLeft);
    } THEN {
        EXPECT_EQ(playerLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 2);
        EXPECT_EQ(playerLeft->statStages[STAT_EVASION], DEFAULT_STAT_STAGE);
        EXPECT_EQ(playerRight->statStages[STAT_EVASION], DEFAULT_STAT_STAGE - 1);
        EXPECT_EQ(opponentLeft->statStages[STAT_EVASION], DEFAULT_STAT_STAGE - 1);
        EXPECT_EQ(opponentRight->statStages[STAT_EVASION], DEFAULT_STAT_STAGE - 1);
        EXPECT_EQ(playerRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponentLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
    }
}
