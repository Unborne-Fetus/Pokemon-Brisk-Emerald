#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Grand Maestro uses Kricketune's strongest non-HP stat for sound attack damage", s16 damage)
{
    u16 attack;
    PARAMETRIZE { attack = 60; }
    PARAMETRIZE { attack = 220; }

    GIVEN {
        ASSUME(IsSoundMove(MOVE_BUG_BUZZ));
        PLAYER(SPECIES_KRICKETUNE) { Level(50); Ability(ABILITY_GRAND_MAESTRO); Attack(attack); SpAttack(30); Defense(50); SpDefense(50); Speed(65); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(500); HP(500); SpDefense(150); }
    } WHEN {
        TURN { MOVE(player, MOVE_BUG_BUZZ, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_GT(results[1].damage, results[0].damage);
    }
}

SINGLE_BATTLE_TEST("Grand Maestro does not replace Attack on non-sound moves", s16 damage)
{
    u16 special;
    PARAMETRIZE { special = 40; }
    PARAMETRIZE { special = 220; }

    GIVEN {
        ASSUME(!IsSoundMove(MOVE_X_SCISSOR));
        PLAYER(SPECIES_KRICKETUNE) { Level(50); Ability(ABILITY_GRAND_MAESTRO); Attack(90); SpAttack(special); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(500); HP(500); Defense(140); }
    } WHEN {
        TURN { MOVE(player, MOVE_X_SCISSOR, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[1].damage, results[0].damage);
    }
}
