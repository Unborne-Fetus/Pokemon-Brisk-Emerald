#include "global.h"
#include "test/battle.h"

// All abilities are tested against the established battle-engine damage
// and turn-order rules, not only against their species ability slots.
SINGLE_BATTLE_TEST("Brisk signature: Crasher Wake strengthens Water Pulse by using physical stats and halving Defense", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_SWIFT_SWIM; }
    PARAMETRIZE { ability = ABILITY_CRASHER_WAKE; }

    GIVEN {
        PLAYER(SPECIES_SEAKING) { Level(50); Ability(ability); Attack(120); SpAttack(120); }
        OPPONENT(SPECIES_WOBBUFFET) { Defense(120); SpDefense(120); MaxHP(500); HP(500); }
    } WHEN {
        TURN { MOVE(player, MOVE_WATER_PULSE, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_GT(results[1].damage, results[0].damage);
    }
}

SINGLE_BATTLE_TEST("Brisk signature: Rage Wings makes neutral attacks super effective regardless of type", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_SUPER_LUCK; }
    PARAMETRIZE { ability = ABILITY_RAGE_WINGS; }

    GIVEN {
        PLAYER(SPECIES_UNFEZANT) { Level(50); Ability(ability); Attack(100); }
        OPPONENT(SPECIES_WOBBUFFET) { Defense(100); MaxHP(500); HP(500); }
    } WHEN {
        TURN { MOVE(player, MOVE_POUND, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[1].damage, results[0].damage * 2);
    }
}

SINGLE_BATTLE_TEST("Brisk signature: Chromatophores grants STAB to non-Normal attacks", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_COLOR_CHANGE; }
    PARAMETRIZE { ability = ABILITY_CHROMATOPHORES; }

    GIVEN {
        PLAYER(SPECIES_KECLEON) { Level(50); Ability(ability); Attack(100); }
        OPPONENT(SPECIES_WOBBUFFET) { Defense(100); MaxHP(500); HP(500); }
    } WHEN {
        TURN { MOVE(player, MOVE_FIRE_PUNCH, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_GT(results[1].damage, results[0].damage);
    }
}

SINGLE_BATTLE_TEST("Brisk signature: Psychic Gravity makes super-effective moves hit after Beheeyem's move")
{
    GIVEN {
        PLAYER(SPECIES_BEHEEYEM) { Ability(ABILITY_PSYCHIC_GRAVITY); Speed(20); MaxHP(600); HP(600); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(200); }
    } WHEN {
        TURN { MOVE(player, MOVE_PSYCHIC); MOVE(opponent, MOVE_X_SCISSOR); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PSYCHIC, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_X_SCISSOR, opponent);
    }
}

SINGLE_BATTLE_TEST("Brisk signature: Master Thief conserves held items when redistributing them")
{
    GIVEN {
        PLAYER(SPECIES_THIEVUL) { Ability(ABILITY_MASTER_THIEF); Item(ITEM_LEFTOVERS); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_ORAN_BERRY); }
    } WHEN {
        TURN {}
    } THEN {
        EXPECT_EQ((player->item == ITEM_LEFTOVERS) + (opponent->item == ITEM_LEFTOVERS), 1);
        EXPECT_EQ((player->item == ITEM_ORAN_BERRY) + (opponent->item == ITEM_ORAN_BERRY), 1);
    }
}
