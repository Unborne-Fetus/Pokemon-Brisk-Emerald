#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Speed Boxer uses Speed rather than Attack for punching damage", s16 damage)
{
    u16 attack;
    u16 speed;
    PARAMETRIZE { attack = 30;  speed = 100; }
    PARAMETRIZE { attack = 200; speed = 100; }
    PARAMETRIZE { attack = 30;  speed = 200; }

    GIVEN {
        ASSUME(IsPunchingMove(MOVE_THUNDER_PUNCH));
        ASSUME(GetMoveCategory(MOVE_THUNDER_PUNCH) == DAMAGE_CATEGORY_PHYSICAL);
        PLAYER(SPECIES_LEDIAN) { Level(50); Ability(ABILITY_SPEED_BOXER); Attack(attack); Speed(speed); }
        OPPONENT(SPECIES_WOBBUFFET) { Defense(100); }
    } WHEN {
        TURN { MOVE(player, MOVE_THUNDER_PUNCH, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage); // Attack does not affect punching damage.
        EXPECT_GT(results[2].damage, results[0].damage); // Speed does.
    }
}

SINGLE_BATTLE_TEST("Speed Boxer does not change non-punching physical move damage", s16 damage)
{
    u16 attack;
    u16 speed;
    PARAMETRIZE { attack = 30;  speed = 100; }
    PARAMETRIZE { attack = 30;  speed = 200; }
    PARAMETRIZE { attack = 200; speed = 100; }

    GIVEN {
        ASSUME(!IsPunchingMove(MOVE_SCRATCH));
        ASSUME(GetMoveCategory(MOVE_SCRATCH) == DAMAGE_CATEGORY_PHYSICAL);
        PLAYER(SPECIES_LEDIAN) { Level(50); Ability(ABILITY_SPEED_BOXER); Attack(attack); Speed(speed); }
        OPPONENT(SPECIES_WOBBUFFET) { Defense(100); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage); // Speed has no effect.
        EXPECT_GT(results[2].damage, results[0].damage); // Attack still matters.
    }
}

SINGLE_BATTLE_TEST("Speed Boxer supports Fire Punch as well as Thunder Punch", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_SPEED_BOXER; }
    PARAMETRIZE { ability = ABILITY_SWARM; }

    GIVEN {
        ASSUME(IsPunchingMove(MOVE_FIRE_PUNCH));
        PLAYER(SPECIES_LEDIAN) { Level(50); Ability(ability); Attack(30); Speed(200); }
        OPPONENT(SPECIES_WOBBUFFET) { Defense(100); }
    } WHEN {
        TURN { MOVE(player, MOVE_FIRE_PUNCH, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_GT(results[0].damage, results[1].damage);
    }
}
