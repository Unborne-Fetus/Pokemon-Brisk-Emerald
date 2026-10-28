#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Nine Lives raises both defenses by two stages after surviving a super-effective hit")
{
    GIVEN {
        ASSUME(GetSpeciesType(SPECIES_DELCATTY, 1) == TYPE_FAIRY);
        PLAYER(SPECIES_DELCATTY) { Ability(ABILITY_NINE_LIVES); MaxHP(400); HP(400); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_POISON_STING); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_POISON_STING, opponent);
        HP_BAR(player);
        ABILITY_POPUP(player, ABILITY_NINE_LIVES);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_DEF], DEFAULT_STAT_STAGE + 2);
        EXPECT_EQ(player->statStages[STAT_SPDEF], DEFAULT_STAT_STAGE + 2);
    }
}

SINGLE_BATTLE_TEST("Nine Lives activates only once per switch-in")
{
    GIVEN {
        PLAYER(SPECIES_DELCATTY) { Ability(ABILITY_NINE_LIVES); MaxHP(400); HP(400); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_POISON_STING); }
        TURN { MOVE(opponent, MOVE_POISON_STING); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_NINE_LIVES);
        NONE_OF { ABILITY_POPUP(player, ABILITY_NINE_LIVES); }
    } THEN {
        EXPECT_EQ(player->statStages[STAT_DEF], DEFAULT_STAT_STAGE + 2);
        EXPECT_EQ(player->statStages[STAT_SPDEF], DEFAULT_STAT_STAGE + 2);
    }
}

SINGLE_BATTLE_TEST("Nine Lives does not activate on neutral damage")
{
    GIVEN {
        PLAYER(SPECIES_DELCATTY) { Ability(ABILITY_NINE_LIVES); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        NONE_OF { ABILITY_POPUP(player, ABILITY_NINE_LIVES); }
    } THEN {
        EXPECT_EQ(player->statStages[STAT_DEF], DEFAULT_STAT_STAGE);
        EXPECT_EQ(player->statStages[STAT_SPDEF], DEFAULT_STAT_STAGE);
    }
}

SINGLE_BATTLE_TEST("Nine Lives resets when Delcatty switches out and back in")
{
    GIVEN {
        PLAYER(SPECIES_DELCATTY) { Ability(ABILITY_NINE_LIVES); MaxHP(400); HP(400); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_POISON_STING); }
        TURN { SWITCH(player, 1); }
        TURN { SWITCH(player, 0); }
        TURN { MOVE(opponent, MOVE_POISON_STING); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_NINE_LIVES);
        ABILITY_POPUP(player, ABILITY_NINE_LIVES);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_DEF], DEFAULT_STAT_STAGE + 2);
        EXPECT_EQ(player->statStages[STAT_SPDEF], DEFAULT_STAT_STAGE + 2);
    }
}
