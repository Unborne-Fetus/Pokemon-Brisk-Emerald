#include "global.h"
#include "test/battle.h"
#include "battle_util.h"

DOUBLE_BATTLE_TEST("Brisk Mega Mode starts multiple Pokemon Mega on both sides without activation")
{
    GIVEN {
        FLAG_SET(FLAG_BRISK_MEGA_MODE);
        PLAYER(SPECIES_CHARIZARD) { Item(ITEM_CHARIZARDITE_X); Moves(MOVE_CELEBRATE); }
        PLAYER(SPECIES_BRELOOM) { Item(ITEM_BRELOOMITE); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_VENUSAUR) { Item(ITEM_VENUSAURITE); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_CELEBRATE); MOVE(playerRight, MOVE_CELEBRATE); }
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_CHARIZARD_MEGA_X);
        EXPECT_EQ(playerRight->species, SPECIES_BRELOOM_MEGA);
        EXPECT_EQ(opponentLeft->species, SPECIES_VENUSAUR_MEGA);
        EXPECT_EQ(opponentRight->species, SPECIES_WOBBUFFET);
        EXPECT(!CanMegaEvolve(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)));
    }
}

SINGLE_BATTLE_TEST("Brisk Mega Mode keeps the party Mega form when a Pokemon faints")
{
    GIVEN {
        FLAG_SET(FLAG_BRISK_MEGA_MODE);
        PLAYER(SPECIES_CHARIZARD) { Item(ITEM_CHARIZARDITE_X); HP(1); Moves(MOVE_CELEBRATE); }
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_TACKLE, MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_TACKLE); SEND_OUT(player, 1); }
    } THEN {
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SPECIES), SPECIES_CHARIZARD_MEGA_X);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP), 0);
    }
}
