#include "global.h"
#include "battle_main.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Solar Fleet temporarily grants Sunflora the equivalent of four Speed stages in sun")
{
    GIVEN {
        PLAYER(SPECIES_SUNFLORA) { Ability(ABILITY_SOLAR_FLEET); Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(200); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_CELEBRATE); MOVE(player, MOVE_SUNNY_DAY); }
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUNNY_DAY, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponent);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGE); // Temporary modifier, no permanent +4 stages
        EXPECT_EQ(GetBattlerTotalSpeedStat(B_BATTLER_0, ABILITY_SOLAR_FLEET, HOLD_EFFECT_NONE), 300);
    }
}

SINGLE_BATTLE_TEST("Solar Fleet boost ends immediately when weather changes away from sunlight")
{
    GIVEN {
        PLAYER(SPECIES_SUNFLORA) { Ability(ABILITY_SOLAR_FLEET); Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(200); }
    } WHEN {
        TURN { MOVE(player, MOVE_SUNNY_DAY); }
        TURN { MOVE(opponent, MOVE_RAIN_DANCE); }
    } THEN {
        EXPECT_EQ(GetBattlerTotalSpeedStat(B_BATTLER_0, ABILITY_SOLAR_FLEET, HOLD_EFFECT_NONE), 100);
    }
}

SINGLE_BATTLE_TEST("Solar Fleet does not boost Speed when Cloud Nine suppresses the sun")
{
    GIVEN {
        PLAYER(SPECIES_SUNFLORA) { Ability(ABILITY_SOLAR_FLEET); Speed(100); }
        OPPONENT(SPECIES_GOLDUCK) { Ability(ABILITY_CLOUD_NINE); }
    } WHEN {
        TURN { MOVE(player, MOVE_SUNNY_DAY); }
    } THEN {
        EXPECT_EQ(GetBattlerTotalSpeedStat(B_BATTLER_0, ABILITY_SOLAR_FLEET, HOLD_EFFECT_NONE), 100);
    }
}

SINGLE_BATTLE_TEST("Solar Fleet respects the maximum Speed stat stage")
{
    GIVEN {
        PLAYER(SPECIES_SUNFLORA) { Ability(ABILITY_SOLAR_FLEET); Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_AGILITY); }
        TURN { MOVE(player, MOVE_SUNNY_DAY); }
    } THEN {
        EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 2);
        EXPECT_EQ(GetBattlerTotalSpeedStat(B_BATTLER_0, ABILITY_SOLAR_FLEET, HOLD_EFFECT_NONE), 400);
    }
}
