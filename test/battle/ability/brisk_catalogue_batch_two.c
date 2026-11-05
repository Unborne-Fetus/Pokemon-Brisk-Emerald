#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Queen's Gambit boosts core stats after a teammate has fainted")
{
    GIVEN {
        PLAYER(SPECIES_NIDOQUEEN) { Ability(ABILITY_QUEENS_GAMBIT); }
        PLAYER(SPECIES_WOBBUFFET) { HP(0); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN { TURN {} }
    THEN {
        EXPECT_EQ(player->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 2);
        EXPECT_EQ(player->statStages[STAT_SPDEF], DEFAULT_STAT_STAGE + 2);
        EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 2);
    }
}

SINGLE_BATTLE_TEST("Momentum Build sometimes increases Speed at end of turn")
{
    PASSES_RANDOMLY(1, 2, RNG_HARVEST);
    GIVEN {
        PLAYER(SPECIES_NIDOKING) { Ability(ABILITY_MOMENTUM_BUILD); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {}
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_MOMENTUM_BUILD);
    }
}

SINGLE_BATTLE_TEST("Lunar Power boosts the five non-HP stats when holding a Moon Stone")
{
    GIVEN {
        PLAYER(SPECIES_CLEFABLE) { Ability(ABILITY_LUNAR_POWER); Item(ITEM_MOON_STONE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN { TURN {} }
    THEN {
        EXPECT_EQ(player->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(player->statStages[STAT_SPDEF], DEFAULT_STAT_STAGE + 1);
    }
}

SINGLE_BATTLE_TEST("Magic Tail turns special resisted hits super effective", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_MAGIC_TAIL; }
    PARAMETRIZE { ability = ABILITY_FLASH_FIRE; }
    GIVEN {
        PLAYER(SPECIES_NINETALES) { Ability(ability); SpAttack(105); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(900); HP(900); SpDefense(150); }
    } WHEN {
        TURN { MOVE(player, MOVE_PSYCHIC, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_GT(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Sweet Song can put foes to sleep with sound moves")
{
    PASSES_RANDOMLY(1, 2, RNG_EFFECT_SPORE);
    GIVEN {
        ASSUME(IsSoundMove(MOVE_HYPER_VOICE));
        PLAYER(SPECIES_WIGGLYTUFF) { Ability(ABILITY_SWEET_SONG); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(900); HP(900); }
    } WHEN { TURN { MOVE(player, MOVE_HYPER_VOICE); } }
    SCENE { STATUS_ICON(opponent, sleep: TRUE); }
}

SINGLE_BATTLE_TEST("Holoparasite absorbs Water moves to heal one eighth max HP")
{
    GIVEN {
        PLAYER(SPECIES_VILEPLUME) { Ability(ABILITY_HOLOPARASITE); MaxHP(200); HP(100); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN { TURN { MOVE(opponent, MOVE_WATER_GUN); } }
    THEN { EXPECT_EQ(player->hp, 125); }
}

SINGLE_BATTLE_TEST("Wedding Gift poisons an opposite-gender contact attacker")
{
    GIVEN {
        ASSUME(MoveMakesContact(MOVE_SCRATCH));
        PLAYER(SPECIES_VENOMOTH) { Ability(ABILITY_WEDDING_GIFT); Gender(MON_MALE); }
        OPPONENT(SPECIES_WOBBUFFET) { Gender(MON_FEMALE); }
    } WHEN { TURN { MOVE(opponent, MOVE_SCRATCH); } }
    THEN { EXPECT(opponent->status1 & STATUS1_POISON); }
}

SINGLE_BATTLE_TEST("Deep Dive doubles Ground move damage", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_DEEP_DIVE; }
    PARAMETRIZE { ability = ABILITY_ARENA_TRAP; }
    GIVEN {
        PLAYER(SPECIES_DUGTRIO) { Ability(ability); Attack(110); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(900); HP(900); Defense(150); }
    } WHEN {
        TURN { MOVE(player, MOVE_EARTHQUAKE, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE { HP_BAR(opponent, captureDamage: &results[i].damage); }
    FINALLY { EXPECT_GT(results[0].damage, results[1].damage); }
}

SINGLE_BATTLE_TEST("Double Coat prevents poison")
{
    GIVEN {
        PLAYER(SPECIES_PERSIAN) { Ability(ABILITY_DOUBLE_COAT); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN { TURN { MOVE(opponent, MOVE_TOXIC); } }
    THEN { EXPECT_EQ(player->status1 & STATUS1_PSN_ANY, 0); }
}

SINGLE_BATTLE_TEST("Psychic Powers changes Normal damaging moves to Psychic", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_PSYCHIC_POWERS; }
    PARAMETRIZE { ability = ABILITY_CLOUD_NINE; }
    GIVEN {
        PLAYER(SPECIES_GOLDUCK) { Ability(ability); Attack(100); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(900); HP(900); Defense(120); }
    } WHEN { TURN { MOVE(player, MOVE_SCRATCH, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); } }
    SCENE { HP_BAR(opponent, captureDamage: &results[i].damage); }
    FINALLY { EXPECT_LT(results[0].damage, results[1].damage); }
}

SINGLE_BATTLE_TEST("Panhu's Lineage raises Attack by two stages after a knockout")
{
    GIVEN {
        PLAYER(SPECIES_ARCANINE) { Ability(ABILITY_PANHUS_LINEAGE); Attack(180); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); MaxHP(100); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN { TURN { MOVE(player, MOVE_SCRATCH); } }
    THEN { EXPECT_EQ(player->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 2); }
}

SINGLE_BATTLE_TEST("Silent Golaith gains evasion then resets after using sound")
{
    GIVEN {
        PLAYER(SPECIES_POLIWRATH) { Ability(ABILITY_SILENT_GOLAITH); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(900); HP(900); }
    } WHEN {
        TURN {}
        TURN { MOVE(player, MOVE_HYPER_VOICE); }
    } THEN {
        EXPECT_EQ(player->statStages[STAT_EVASION], DEFAULT_STAT_STAGE);
    }
}
