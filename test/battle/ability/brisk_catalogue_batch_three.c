#include "global.h"
#include "test/battle.h"

// Catalogue signatures: battle-level regression checks for Alakazam through Jynx.
SINGLE_BATTLE_TEST("Spoon Bend makes Psychic damage super effective on Steel", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_SPOON_BEND; }
    PARAMETRIZE { ability = ABILITY_SYNCHRONIZE; }
    GIVEN {
        PLAYER(SPECIES_ALAKAZAM) { Ability(ability); SpAttack(135); }
        OPPONENT(SPECIES_REGISTEEL) { Ability(ABILITY_CLEAR_BODY); MaxHP(900); HP(900); SpDefense(160); }
    } WHEN {
        TURN { MOVE(player, MOVE_PSYCHIC, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_GT(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Operatic Voice strengthens sound-based attacks", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_OPERATIC_VOICE; }
    PARAMETRIZE { ability = ABILITY_OBLIVIOUS; }
    GIVEN {
        ASSUME(IsSoundMove(MOVE_HYPER_VOICE));
        PLAYER(SPECIES_JYNX) { Ability(ability); SpAttack(120); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(900); HP(900); SpDefense(160); }
    } WHEN {
        TURN { MOVE(player, MOVE_HYPER_VOICE, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_GT(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Bruce Force strengthens physical attacks", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_BRUCE_FORCE; }
    PARAMETRIZE { ability = ABILITY_LIMBER; }
    GIVEN {
        PLAYER(SPECIES_HITMONLEE) { Ability(ability); Attack(110); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(900); HP(900); Defense(150); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_GT(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Calcification reduces physical damage", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_CALCIFICATION; }
    PARAMETRIZE { ability = ABILITY_HYPER_CUTTER; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Attack(130); }
        OPPONENT(SPECIES_KINGLER) { Ability(ability); MaxHP(900); HP(900); Defense(100); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_LT(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Floating Wisp protects Gengar from Ground attacks")
{
    GIVEN {
        PLAYER(SPECIES_GENGAR) { Ability(ABILITY_FLOATING_WISP); MaxHP(400); HP(400); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_EARTHQUAKE); }
    } THEN {
        EXPECT_EQ(player->hp, 400);
    }
}

SINGLE_BATTLE_TEST("Megatherium absorbs a Grass-type move")
{
    GIVEN {
        PLAYER(SPECIES_SLOWBRO) { Ability(ABILITY_MEGATHERIUM); MaxHP(200); HP(100); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_VINE_WHIP); }
    } THEN {
        EXPECT_GT(player->hp, 100);
    }
}

SINGLE_BATTLE_TEST("Poison Gut poisons a Water move target")
{
    GIVEN {
        PLAYER(SPECIES_VICTREEBEL) { Ability(ABILITY_POISON_GUT); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(900); HP(900); }
    } WHEN {
        TURN { MOVE(player, MOVE_WATER_GUN); }
    } THEN {
        EXPECT(opponent->status1 & STATUS1_POISON);
    }
}

SINGLE_BATTLE_TEST("Sedimentary Buildup adds a third battle type")
{
    GIVEN {
        PLAYER(SPECIES_GOLEM) { Ability(ABILITY_SEDIMENTARY_BUILDUP); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {}
    } THEN {
        EXPECT_NE(player->types[2], TYPE_MYSTERY);
    }
}
