#include "global.h"
#include "test/battle.h"

// Brisk Emerald signature abilities assigned from the catalogue through Raichu.

SINGLE_BATTLE_TEST("Poison Wart can poison a contact attacker")
{
    PASSES_RANDOMLY(1, 2, RNG_POISON_POINT);
    GIVEN {
        ASSUME(MoveMakesContact(MOVE_SCRATCH));
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_VENUSAUR) { Ability(ABILITY_POISON_WART); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_POISON_WART);
        STATUS_ICON(player, poison: TRUE);
    } THEN {
        EXPECT(player->status1 & STATUS1_POISON);
    }
}

SINGLE_BATTLE_TEST("Poison Wart can poison the target of its own contact attack")
{
    PASSES_RANDOMLY(1, 2, RNG_POISON_POINT);
    GIVEN {
        ASSUME(MoveMakesContact(MOVE_SCRATCH));
        PLAYER(SPECIES_VENUSAUR) { Ability(ABILITY_POISON_WART); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_POISON_WART);
        STATUS_ICON(opponent, poison: TRUE);
    } THEN {
        EXPECT(opponent->status1 & STATUS1_POISON);
    }
}

SINGLE_BATTLE_TEST("Flame Tail strengthens damaging tail moves", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_FLAME_TAIL; }
    PARAMETRIZE { ability = ABILITY_BLAZE; }
    GIVEN {
        ASSUME(GetMovePower(MOVE_IRON_TAIL) > 0);
        PLAYER(SPECIES_CHARIZARD) { Ability(ability); Attack(105); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(750); HP(750); Defense(110); }
    } WHEN {
        TURN { MOVE(player, MOVE_IRON_TAIL, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_GT(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Power Cannons strengthens moves that force recharge", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_POWER_CANNONS; }
    PARAMETRIZE { ability = ABILITY_TORRENT; }
    GIVEN {
        ASSUME(MoveHasAdditionalEffectSelf(MOVE_HYPER_BEAM, MOVE_EFFECT_RECHARGE));
        PLAYER(SPECIES_BLASTOISE) { Ability(ability); SpAttack(110); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(900); HP(900); SpDefense(150); }
    } WHEN {
        TURN { MOVE(player, MOVE_HYPER_BEAM, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_GT(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Hemlock Wings paralyzes contact attackers 70 percent of the time")
{
    PASSES_RANDOMLY(7, 10, RNG_EFFECT_SPORE);
    GIVEN {
        ASSUME(MoveMakesContact(MOVE_SCRATCH));
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_BUTTERFREE) { Ability(ABILITY_HEMLOCK_WINGS); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_HEMLOCK_WINGS);
        STATUS_ICON(player, paralysis: TRUE);
    } THEN {
        EXPECT(player->status1 & STATUS1_PARALYSIS);
    }
}

SINGLE_BATTLE_TEST("Petiole raises speed on every strike of a two-hit move")
{
    GIVEN {
        ASSUME(IsMultiHitMove(MOVE_DOUBLE_HIT));
        PLAYER(SPECIES_BEEDRILL) { Ability(ABILITY_PETIOLE); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(900); HP(900); }
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_HIT); }
    } THEN {
        EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 2);
    }
}

SINGLE_BATTLE_TEST("Blind Dive strengthens recoil attacks", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_BLIND_DIVE; }
    PARAMETRIZE { ability = ABILITY_KEEN_EYE; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_DOUBLE_EDGE) == EFFECT_RECOIL);
        PLAYER(SPECIES_PIDGEOT) { Ability(ability); Attack(110); MaxHP(500); HP(500); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(900); HP(900); Defense(150); }
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_EDGE, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_GT(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Lightning Bill gives Flying moves extra priority")
{
    GIVEN {
        ASSUME(GetMoveType(MOVE_AERIAL_ACE) == TYPE_FLYING);
        PLAYER(SPECIES_FEAROW) { Ability(ABILITY_LIGHTNING_BILL); Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(100); }
    } WHEN {
        TURN { MOVE(player, MOVE_AERIAL_ACE); }
    } SCENE {
        MESSAGE("Fearow used Aerial Ace!");
        MESSAGE("The opposing Wobbuffet used Celebrate!");
    }
}

SINGLE_BATTLE_TEST("Hood Flare lowers move priority")
{
    GIVEN {
        PLAYER(SPECIES_ARBOK) { Ability(ABILITY_HOOD_FLARE); Speed(200); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        MESSAGE("The opposing Wobbuffet used Celebrate!");
        MESSAGE("Arbok used Scratch!");
    }
}

SINGLE_BATTLE_TEST("Hood Flare doubles damaging move power", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_HOOD_FLARE; }
    PARAMETRIZE { ability = ABILITY_INTIMIDATE; }
    GIVEN {
        PLAYER(SPECIES_ARBOK) { Ability(ability); Attack(100); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(900); HP(900); Defense(140); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_GT(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Evolution Denial grants an intrinsic Eviolite-like defense", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_EVOLUTION_DENIAL; }
    PARAMETRIZE { ability = ABILITY_STATIC; }
    GIVEN {
        PLAYER(SPECIES_RAICHU) { Ability(ability); Defense(90); MaxHP(900); HP(900); }
        OPPONENT(SPECIES_WOBBUFFET) { Attack(100); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_LT(results[0].damage, results[1].damage);
    }
}
