#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Heartbreaker takes half of the knockout attacker's maximum HP", s16 damage)
{
    GIVEN {
        PLAYER(SPECIES_LUVDISC) { HP(1); Ability(ABILITY_HEARTBREAKER); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(200); HP(200); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH); SEND_OUT(player, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponent);
        ABILITY_POPUP(player, ABILITY_HEARTBREAKER);
        HP_BAR(opponent, captureDamage: &damage);
    } THEN {
        EXPECT_EQ(damage, 100);
    }
}

DOUBLE_BATTLE_TEST("Heartbreaker retaliates against the opposing attacker but not its partner or Luvdisc's ally")
{
    GIVEN {
        PLAYER(SPECIES_LUVDISC) { HP(1); Ability(ABILITY_HEARTBREAKER); }
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(200); HP(200); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(200); HP(200); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_SCRATCH, target: playerLeft); SEND_OUT(playerLeft, 2); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponentLeft);
        ABILITY_POPUP(playerLeft, ABILITY_HEARTBREAKER);
        HP_BAR(opponentLeft);
        NOT HP_BAR(opponentRight);
        NOT HP_BAR(playerRight);
    } THEN {
        EXPECT_EQ(opponentLeft->hp, 100);
        EXPECT_EQ(opponentRight->hp, 200);
    }
}
