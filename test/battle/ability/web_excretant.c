#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Web Excretant sharply boosts Spidops's Speed and lowers foe Evasion on entry")
{
    GIVEN {
        PLAYER(SPECIES_SPIDOPS) { Ability(ABILITY_WEB_EXCRETANT); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_WEB_EXCRETANT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 2);
        EXPECT_EQ(player->statStages[STAT_EVASION], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponent->statStages[STAT_EVASION], DEFAULT_STAT_STAGE - 1);
        EXPECT_EQ(opponent->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
    }
}

DOUBLE_BATTLE_TEST("Web Excretant lowers every other battler's Evasion, including its doubles ally")
{
    GIVEN {
        PLAYER(SPECIES_SPIDOPS) { Ability(ABILITY_WEB_EXCRETANT); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(playerLeft, ABILITY_WEB_EXCRETANT);
    } THEN {
        EXPECT_EQ(playerLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 2);
        EXPECT_EQ(playerLeft->statStages[STAT_EVASION], DEFAULT_STAT_STAGE);
        EXPECT_EQ(playerRight->statStages[STAT_EVASION], DEFAULT_STAT_STAGE - 1);
        EXPECT_EQ(opponentLeft->statStages[STAT_EVASION], DEFAULT_STAT_STAGE - 1);
        EXPECT_EQ(opponentRight->statStages[STAT_EVASION], DEFAULT_STAT_STAGE - 1);
        EXPECT_EQ(playerRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
    }
}

SINGLE_BATTLE_TEST("Web Excretant triggers again on switching in, without lowering Spidops's own Evasion")
{
    GIVEN {
        PLAYER(SPECIES_SPIDOPS) { Ability(ABILITY_WEB_EXCRETANT); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { SWITCH(player, 1); }
        TURN { SWITCH(player, 0); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_WEB_EXCRETANT);
        ABILITY_POPUP(player, ABILITY_WEB_EXCRETANT);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 2);
        EXPECT_EQ(player->statStages[STAT_EVASION], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponent->statStages[STAT_EVASION], DEFAULT_STAT_STAGE - 2);
    }
}

SINGLE_BATTLE_TEST("Web Excretant does not lower Evasion of opponents with Clear Body")
{
    GIVEN {
        PLAYER(SPECIES_SPIDOPS) { Ability(ABILITY_WEB_EXCRETANT); }
        OPPONENT(SPECIES_REGISTEEL) { Ability(ABILITY_CLEAR_BODY); }
    } WHEN {
        TURN {}
    } THEN {
        EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 2);
        EXPECT_EQ(opponent->statStages[STAT_EVASION], DEFAULT_STAT_STAGE);
    }
}
