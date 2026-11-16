#include "global.h"
#include "test/battle.h"

// Catalogue regression tests are named "Brisk signature" so the project's
// focused make check TESTS='Brisk signature' CI job includes this batch.

SINGLE_BATTLE_TEST("Brisk signature Hercules makes Normal hits super effective against Steel", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_HERCULES; }
    PARAMETRIZE { ability = ABILITY_HYPER_CUTTER; }
    GIVEN {
        PLAYER(SPECIES_PINSIR) { Ability(ability); Attack(160); }
        OPPONENT(SPECIES_AGGRON) { MaxHP(900); HP(900); Defense(150); }
    } WHEN { TURN { MOVE(player, MOVE_SCRATCH, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); } }
    SCENE { HP_BAR(opponent, captureDamage: &results[i].damage); }
    FINALLY { EXPECT_GT(results[0].damage, results[1].damage); }
}

SINGLE_BATTLE_TEST("Brisk signature Cretan doubles Tauros physical attack", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_CRETAN; }
    PARAMETRIZE { ability = ABILITY_ANGER_POINT; }
    GIVEN {
        PLAYER(SPECIES_TAUROS) { Ability(ability); Attack(120); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(900); HP(900); Defense(100); }
    } WHEN { TURN { MOVE(player, MOVE_SCRATCH, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); } }
    SCENE { HP_BAR(opponent, captureDamage: &results[i].damage); }
    FINALLY { EXPECT_GT(results[0].damage, results[1].damage); }
}

SINGLE_BATTLE_TEST("Brisk signature Cretan confuses Tauros after a Water hit")
{
    GIVEN {
        PLAYER(SPECIES_TAUROS) { Ability(ABILITY_CRETAN); MaxHP(300); HP(300); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN { TURN { MOVE(opponent, MOVE_WATER_GUN); } }
    THEN { EXPECT(player->volatiles.confusionTimer > 0); }
}

SINGLE_BATTLE_TEST("Brisk signature Jade Blessing occasionally raises one stat by two stages")
{
    PASSES_RANDOMLY(3, 40, RNG_MOODY_INCREASE);
    GIVEN {
        PLAYER(SPECIES_GYARADOS) { Ability(ABILITY_JADE_BLESSING); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN { TURN {} }
    SCENE { ABILITY_POPUP(player, ABILITY_JADE_BLESSING); }
    THEN {
        u32 total = 0;
        for (enum Stat stat = STAT_ATK; stat < NUM_BATTLE_STATS; stat++)
            total += player->statStages[stat] - DEFAULT_STAT_STAGE;
        EXPECT_EQ(total, 2);
    }
}

SINGLE_BATTLE_TEST("Brisk signature We Will Rock You blocks Ground and Electric attacks")
{
    enum Move move;
    PARAMETRIZE { move = MOVE_EARTHQUAKE; }
    PARAMETRIZE { move = MOVE_THUNDERBOLT; }
    GIVEN {
        PLAYER(SPECIES_LAPRAS) { Ability(ABILITY_WE_WILL_ROCK_YOU); MaxHP(300); HP(300); }
        OPPONENT(SPECIES_WOBBUFFET) { Attack(130); SpAttack(130); }
    } WHEN { TURN { MOVE(opponent, move); } }
    THEN { EXPECT_EQ(player->hp, 300); }
}

SINGLE_BATTLE_TEST("Brisk signature That's Gross raises Vaporeon's Speed two stages")
{
    GIVEN {
        PLAYER(SPECIES_VAPOREON) { Ability(ABILITY_THATS_GROSS); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN { TURN { MOVE(opponent, MOVE_WATER_GUN); } }
    THEN { EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 2); }
}

SINGLE_BATTLE_TEST("Brisk signature Plasma Discharge changes Electric to Water against Ground")
{
    GIVEN {
        PLAYER(SPECIES_JOLTEON) { Ability(ABILITY_PLASMA_DISCHARGE); SpAttack(160); }
        OPPONENT(SPECIES_RHYDON) { MaxHP(900); HP(900); SpDefense(120); }
    } WHEN { TURN { MOVE(player, MOVE_THUNDERBOLT); } }
    THEN { EXPECT_LT(opponent->hp, 900); }
}

SINGLE_BATTLE_TEST("Brisk signature Adrenaline Rage raises Flareon's Sp. Atk by two")
{
    GIVEN {
        PLAYER(SPECIES_FLAREON) { Ability(ABILITY_ADRENALINE_RAGE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN { TURN { MOVE(opponent, MOVE_EMBER); } }
    THEN { EXPECT_EQ(player->statStages[STAT_SPATK], DEFAULT_STAT_STAGE + 2); }
}

SINGLE_BATTLE_TEST("Brisk signature Extinction Event boosts hits on Bug egg group", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_EXTINCTION_EVENT; }
    PARAMETRIZE { ability = ABILITY_SWIFT_SWIM; }
    GIVEN {
        PLAYER(SPECIES_OMASTAR) { Ability(ability); SpAttack(110); }
        OPPONENT(SPECIES_METAPOD) { MaxHP(900); HP(900); SpDefense(100); }
    } WHEN { TURN { MOVE(player, MOVE_WATER_GUN, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); } }
    SCENE { HP_BAR(opponent, captureDamage: &results[i].damage); }
    FINALLY { EXPECT_GT(results[0].damage, results[1].damage); }
}

SINGLE_BATTLE_TEST("Brisk signature Chitin doubles both physical and special bulk", s16 damage)
{
    enum Ability ability;
    enum Move move;
    PARAMETRIZE { ability = ABILITY_CHITIN; move = MOVE_SCRATCH; }
    PARAMETRIZE { ability = ABILITY_SWIFT_SWIM; move = MOVE_SCRATCH; }
    PARAMETRIZE { ability = ABILITY_CHITIN; move = MOVE_PSYCHIC; }
    PARAMETRIZE { ability = ABILITY_SWIFT_SWIM; move = MOVE_PSYCHIC; }
    GIVEN {
        PLAYER(SPECIES_KABUTOPS) { Ability(ability); MaxHP(900); HP(900); Defense(120); SpDefense(120); }
        OPPONENT(SPECIES_WOBBUFFET) { Attack(140); SpAttack(140); }
    } WHEN { TURN { MOVE(opponent, move, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); } }
    SCENE { HP_BAR(player, captureDamage: &results[i].damage); }
    FINALLY {
        EXPECT_LT(results[0].damage, results[1].damage);
        EXPECT_LT(results[2].damage, results[3].damage);
    }
}

SINGLE_BATTLE_TEST("Brisk signature Sea Hunter heals one sixth max HP when hitting Water")
{
    GIVEN {
        PLAYER(SPECIES_AERODACTYL) { Ability(ABILITY_SEA_HUNTER); MaxHP(240); HP(100); }
        OPPONENT(SPECIES_BLASTOISE) { MaxHP(900); HP(900); }
    } WHEN { TURN { MOVE(player, MOVE_SCRATCH); } }
    THEN { EXPECT_EQ(player->hp, 140); }
}

SINGLE_BATTLE_TEST("Brisk signature Nerve Tickle blocks Fighting attacks")
{
    GIVEN {
        PLAYER(SPECIES_SNORLAX) { Ability(ABILITY_NERVE_TICKLE); MaxHP(250); HP(250); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN { TURN { MOVE(opponent, MOVE_BRICK_BREAK); } }
    THEN { EXPECT_EQ(player->hp, 250); }
}

SINGLE_BATTLE_TEST("Brisk signature Cold as Ice doubles damage in singles", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_COLD_AS_ICE; }
    PARAMETRIZE { ability = ABILITY_PRESSURE; }
    GIVEN {
        PLAYER(SPECIES_ARTICUNO) { Ability(ability); SpAttack(130); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(900); HP(900); SpDefense(120); }
    } WHEN { TURN { MOVE(player, MOVE_PSYCHIC, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); } }
    SCENE { HP_BAR(opponent, captureDamage: &results[i].damage); }
    FINALLY { EXPECT_GT(results[0].damage, results[1].damage); }
}

SINGLE_BATTLE_TEST("Brisk signature Electric Slide turns Normal moves into Electric")
{
    GIVEN {
        PLAYER(SPECIES_ZAPDOS) { Ability(ABILITY_ELECTRIC_SLIDE); }
        OPPONENT(SPECIES_DUGTRIO) { MaxHP(900); HP(900); }
    } WHEN { TURN { MOVE(player, MOVE_SCRATCH); } }
    THEN { EXPECT_EQ(opponent->hp, 900); }
}

SINGLE_BATTLE_TEST("Brisk signature bird Attract succeeds even with genderless targets")
{
    GIVEN {
        PLAYER(SPECIES_ARTICUNO) { Ability(ABILITY_COLD_AS_ICE); Gender(MON_GENDERLESS); }
        OPPONENT(SPECIES_WOBBUFFET) { Gender(MON_GENDERLESS); }
    } WHEN { TURN { MOVE(player, MOVE_ATTRACT); } }
    THEN { EXPECT(opponent->volatiles.infatuation); }
}

SINGLE_BATTLE_TEST("Brisk signature Burning Love burns when a contact attack connects")
{
    GIVEN {
        PLAYER(SPECIES_MOLTRES) { Ability(ABILITY_BURNING_LOVE); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(900); HP(900); }
    } WHEN { TURN { MOVE(player, MOVE_SCRATCH); } }
    THEN { EXPECT(opponent->status1 & STATUS1_BURN); }
}

SINGLE_BATTLE_TEST("Brisk signature Lonely Boy ignores spread Surf")
{
    GIVEN {
        PLAYER(SPECIES_DRAGONITE) { Ability(ABILITY_LONELY_BOY); MaxHP(300); HP(300); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN { TURN { MOVE(opponent, MOVE_SURF); } }
    THEN { EXPECT_EQ(player->hp, 300); }
}

SINGLE_BATTLE_TEST("Brisk signature Lonely Boy ignores Attract")
{
    GIVEN {
        PLAYER(SPECIES_DRAGONITE) { Ability(ABILITY_LONELY_BOY); Gender(MON_MALE); }
        OPPONENT(SPECIES_WOBBUFFET) { Gender(MON_FEMALE); }
    } WHEN { TURN { MOVE(opponent, MOVE_ATTRACT); } }
    THEN { EXPECT(player->volatiles.infatuation == 0); }
}

SINGLE_BATTLE_TEST("Brisk signature Lonely Boy raises singles damage 50 percent", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_LONELY_BOY; }
    PARAMETRIZE { ability = ABILITY_INNER_FOCUS; }
    GIVEN {
        PLAYER(SPECIES_DRAGONITE) { Ability(ability); Attack(140); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(900); HP(900); Defense(120); }
    } WHEN { TURN { MOVE(player, MOVE_SCRATCH, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); } }
    SCENE { HP_BAR(opponent, captureDamage: &results[i].damage); }
    FINALLY { EXPECT_GT(results[0].damage, results[1].damage); }
}
