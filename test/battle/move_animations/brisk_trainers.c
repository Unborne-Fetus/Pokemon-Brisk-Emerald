#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Brisk trainer attack animations complete on normal and large Mega sprites")
{
    static const enum Species speciesList[] = {
        SPECIES_HITMONCHAN, SPECIES_HAUNTER, SPECIES_MUK, SPECIES_SNORLAX_MEGA,
    };
    static const enum Move moveList[] = {
        MOVE_ICE_PUNCH, MOVE_THUNDERBOLT, MOVE_SHADOW_BALL,
        MOVE_PSYCHIC, MOVE_SURF, MOVE_EARTHQUAKE,
    };
    enum Species species = SPECIES_HITMONCHAN;
    enum Move move = MOVE_ICE_PUNCH;
    for (u32 i = 0; i < ARRAY_COUNT(speciesList); i++)
        for (u32 j = 0; j < ARRAY_COUNT(moveList); j++)
            PARAMETRIZE { species = speciesList[i]; move = moveList[j]; }
    FORCE_MOVE_ANIM(TRUE);
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { HP(9999); MaxHP(9999); }
        OPPONENT(species) { Moves(move); }
    } WHEN {
        TURN { MOVE(opponent, move); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, opponent);
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        EXPECT_EQ(gLoadFail, FALSE);
        EXPECT_EQ(gSpriteAllocs, 0);
    }
}
