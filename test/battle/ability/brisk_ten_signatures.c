#include "global.h"
#include "battle_main.h"
#include "test/battle.h"

// Brisk Emerald's custom signature ability suite.

SINGLE_BATTLE_TEST("Jaw Snap punishes contact attackers for one sixth of their maximum HP")
{
    GIVEN {
        PLAYER(SPECIES_CARNIVINE) { Ability(ABILITY_JAW_SNAP); HP(500); MaxHP(500); Defense(150); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(180); MaxHP(180); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH); }
    } THEN {
        EXPECT_EQ(opponent->hp, 150);
    }
}

SINGLE_BATTLE_TEST("Jaw Snap punishes Bug-type contact attackers for one third of maximum HP")
{
    GIVEN {
        PLAYER(SPECIES_CARNIVINE) { Ability(ABILITY_JAW_SNAP); HP(500); MaxHP(500); Defense(150); }
        OPPONENT(SPECIES_SCYTHER) { HP(180); MaxHP(180); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH); }
    } THEN {
        EXPECT_EQ(opponent->hp, 120);
    }
}

SINGLE_BATTLE_TEST("Duran raises Watchog's Evasion three stages on entry")
{
    GIVEN {
        PLAYER(SPECIES_WATCHOG) { Ability(ABILITY_DURAN); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_DURAN);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_EVASION], DEFAULT_STAT_STAGE + 3);
    }
}

SINGLE_BATTLE_TEST("Clearwater gives all Wugtrio moves one extra priority stage")
{
    GIVEN {
        PLAYER(SPECIES_WUGTRIO) { Ability(ABILITY_CLEARWATER); Speed(10); HP(500); MaxHP(500); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(200); }
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); MOVE(opponent, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponent);
    }
}

SINGLE_BATTLE_TEST("Clearwater makes Wugtrio take twice the normal damage", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_RATTLED; }
    PARAMETRIZE { ability = ABILITY_CLEARWATER; }
    GIVEN {
        PLAYER(SPECIES_WUGTRIO) { Ability(ability); HP(600); MaxHP(600); Defense(150); }
        OPPONENT(SPECIES_WOBBUFFET) { Attack(130); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_TACKLE, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[1].damage, results[0].damage * 2);
    }
}

SINGLE_BATTLE_TEST("Build Me Up raises one Furret stat and lowers one opposing stat per turn")
{
    GIVEN {
        PLAYER(SPECIES_FURRET) { Ability(ABILITY_BUILD_ME_UP); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {}
    } THEN {
        s32 playerTotal = 0;
        s32 opponentTotal = 0;
        for (enum Stat stat = STAT_ATK; stat < NUM_BATTLE_STATS; stat++)
        {
            playerTotal += player->statStages[stat] - DEFAULT_STAT_STAGE;
            opponentTotal += opponent->statStages[stat] - DEFAULT_STAT_STAGE;
        }
        EXPECT_EQ(playerTotal, 1);
        EXPECT_EQ(opponentTotal, -1);
    }
}

SINGLE_BATTLE_TEST("Overdose immediately damages the freshly badly poisoned opponent for half max HP")
{
    GIVEN {
        PLAYER(SPECIES_ARIADOS) { Ability(ABILITY_OVERDOSE); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(200); MaxHP(200); }
    } WHEN {
        TURN { MOVE(player, MOVE_TOXIC); }
    } SCENE {
        MESSAGE("The opposing Wobbuffet was badly poisoned!");
        ABILITY_POPUP(player, ABILITY_OVERDOSE);
        HP_BAR(opponent, hp: 100);
    } THEN {
        EXPECT_NE(opponent->status1 & STATUS1_TOXIC_POISON, 0);
    }
}

SINGLE_BATTLE_TEST("Burning Crust protects Magcargo from Ground moves")
{
    GIVEN {
        PLAYER(SPECIES_MAGCARGO) { Ability(ABILITY_BURNING_CRUST); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_EARTHQUAKE); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_BURNING_CRUST);
        NONE_OF {
            HP_BAR(player);
        }
    }
}

SINGLE_BATTLE_TEST("Burning Crust halves Water-type move damage", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_MAGMA_ARMOR; }
    PARAMETRIZE { ability = ABILITY_BURNING_CRUST; }
    GIVEN {
        PLAYER(SPECIES_MAGCARGO) { Ability(ability); HP(700); MaxHP(700); SpDefense(200); }
        OPPONENT(SPECIES_WOBBUFFET) { SpAttack(100); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_WATER_GUN, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_NEAR(results[1].damage * 2, results[0].damage, 2);
    }
}

SINGLE_BATTLE_TEST("Ogooué Basin doubles Lumineon's Water-type move damage", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_SWIFT_SWIM; }
    PARAMETRIZE { ability = ABILITY_OGOOUE_BASIN; }
    GIVEN {
        PLAYER(SPECIES_LUMINEON) { Ability(ability); SpAttack(100); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(600); MaxHP(600); SpDefense(150); }
    } WHEN {
        TURN { MOVE(player, MOVE_WATER_GUN, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[1].damage, results[0].damage * 2);
    }
}

DOUBLE_BATTLE_TEST("Nopales redirects targeted Water attacks to Maractus and heals it")
{
    GIVEN {
        PLAYER(SPECIES_MARACTUS) { Ability(ABILITY_NOPALES); HP(200); MaxHP(400); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_WATER_GUN, target: playerRight); }
    } SCENE {
        ABILITY_POPUP(playerLeft, ABILITY_NOPALES);
        HP_BAR(playerLeft);
        NONE_OF {
            HP_BAR(playerRight);
        }
    } THEN {
        EXPECT_EQ(playerLeft->hp, 300);
        EXPECT_EQ(playerRight->hp, playerRight->maxHP);
    }
}

SINGLE_BATTLE_TEST("Nopales reduces Fire damage against Maractus to one quarter", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_WATER_ABSORB; }
    PARAMETRIZE { ability = ABILITY_NOPALES; }
    GIVEN {
        PLAYER(SPECIES_MARACTUS) { Ability(ability); HP(700); MaxHP(700); SpDefense(200); }
        OPPONENT(SPECIES_WOBBUFFET) { SpAttack(100); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_EMBER, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_NEAR(results[1].damage * 4, results[0].damage, 3);
    }
}

SINGLE_BATTLE_TEST("Torsion halves physical move damage against Swalot", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_LIQUID_OOZE; }
    PARAMETRIZE { ability = ABILITY_TORSION; }
    GIVEN {
        PLAYER(SPECIES_SWALOT) { Ability(ability); HP(600); MaxHP(600); Defense(150); }
        OPPONENT(SPECIES_WOBBUFFET) { Attack(100); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_TACKLE, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_NEAR(results[1].damage * 2, results[0].damage, 2);
    }
}

DOUBLE_BATTLE_TEST("Flame Tongue makes Heatmor's Fire moves hit both active opponents")
{
    GIVEN {
        PLAYER(SPECIES_HEATMOR) { Ability(ABILITY_FLAME_TONGUE); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { HP(500); MaxHP(500); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(500); MaxHP(500); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_EMBER, target: opponentLeft); }
    } SCENE {
        HP_BAR(opponentLeft);
        HP_BAR(opponentRight);
    }
}

SINGLE_BATTLE_TEST("Flame Tongue boosts Heatmor's damage against Bug types by 50 percent", s16 damage)
{
    enum Species species;
    PARAMETRIZE { species = SPECIES_WOBBUFFET; }
    PARAMETRIZE { species = SPECIES_SCYTHER; }
    GIVEN {
        PLAYER(SPECIES_HEATMOR) { Ability(ABILITY_FLAME_TONGUE); Attack(100); }
        OPPONENT(species) { HP(600); MaxHP(600); Defense(150); }
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_NEAR(results[1].damage * 2, results[0].damage * 3, 3);
    }
}
