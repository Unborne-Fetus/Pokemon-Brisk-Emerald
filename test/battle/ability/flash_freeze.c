#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Flash Freeze grants Ice Shard a 40 percent freeze chance")
{
    PASSES_RANDOMLY(40, 100, RNG_SECONDARY_EFFECT);
    GIVEN {
        ASSUME(GetMoveType(MOVE_ICE_SHARD) == TYPE_ICE);
        ASSUME(!MoveHasAdditionalEffect(MOVE_ICE_SHARD, MOVE_EFFECT_FREEZE));
        PLAYER(SPECIES_DELIBIRD) { Ability(ABILITY_FLASH_FREEZE); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(600); HP(600); }
    } WHEN {
        TURN { MOVE(player, MOVE_ICE_SHARD); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ICE_SHARD, player);
        HP_BAR(opponent);
        MESSAGE("The opposing Wobbuffet was frozen solid!");
    }
}

SINGLE_BATTLE_TEST("Flash Freeze changes Ice Beam's existing freeze chance to 40 percent, with no double roll")
{
    PASSES_RANDOMLY(40, 100, RNG_SECONDARY_EFFECT);
    GIVEN {
        ASSUME(GetMoveType(MOVE_ICE_BEAM) == TYPE_ICE);
        ASSUME(MoveHasAdditionalEffect(MOVE_ICE_BEAM, MOVE_EFFECT_FREEZE_OR_FROSTBITE));
        PLAYER(SPECIES_DELIBIRD) { Ability(ABILITY_FLASH_FREEZE); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(600); HP(600); }
    } WHEN {
        TURN { MOVE(player, MOVE_ICE_BEAM); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ICE_BEAM, player);
        HP_BAR(opponent);
        MESSAGE("The opposing Wobbuffet was frozen solid!");
    }
}

SINGLE_BATTLE_TEST("Flash Freeze doubles damage against frozen Pokémon", s16 damage)
{
    u32 status;
    PARAMETRIZE { status = 0; }
    PARAMETRIZE { status = STATUS1_FREEZE; }

    GIVEN {
        PLAYER(SPECIES_DELIBIRD) { Level(50); Ability(ABILITY_FLASH_FREEZE); Attack(100); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(600); HP(600); Defense(150); Status1(status); }
    } WHEN {
        TURN { MOVE(player, MOVE_ICE_SHARD, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[1].damage, results[0].damage * 2);
    }
}

SINGLE_BATTLE_TEST("Flash Freeze does not double non-frozen damage")
{
    GIVEN {
        PLAYER(SPECIES_DELIBIRD) { Ability(ABILITY_FLASH_FREEZE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_ICE_SHARD, WITH_RNG(RNG_SECONDARY_EFFECT, FALSE)); }
    } THEN {
        EXPECT_EQ(opponent->status1 & STATUS1_FREEZE, 0);
    }
}

SINGLE_BATTLE_TEST("Flash Freeze respects Ice-type immunity to freezing")
{
    GIVEN {
        PLAYER(SPECIES_DELIBIRD) { Ability(ABILITY_FLASH_FREEZE); }
        OPPONENT(SPECIES_SNORUNT);
    } WHEN {
        TURN { MOVE(player, MOVE_ICE_SHARD, WITH_RNG(RNG_SECONDARY_EFFECT, TRUE)); }
    } THEN {
        EXPECT_EQ(opponent->status1 & STATUS1_FREEZE, 0);
    }
}
