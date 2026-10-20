#include "global.h"
#include "brisk_battle_format.h"
#include "brisk_modes.h"
#include "battle.h"
#include "event_data.h"
#include "pokemon.h"
#include "wild_encounter.h"
#include "test/test.h"

bool32 Test_TryGenerateSecondOWEMon(const struct WildPokemonInfo *, enum WildPokemonArea);

TEST("Overworld doubles retain the visible shiny Litten first, including when forced to singles")
{
    struct WildPokemon land[NUM_LAND_MONS_ENCOUNTER_SLOTS];
    for (u32 i = 0; i < ARRAY_COUNT(land); i++)
        land[i] = (struct WildPokemon){.species = SPECIES_MUDKIP, .minLevel = 50, .maxLevel = 50};
    const struct WildPokemonInfo info = {20, land};
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
    VarSet(VAR_BRISK_RANDOMIZER, 0);
    VarSet(VAR_REPEL_STEP_COUNT, 0);
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();
    CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][0], SPECIES_TORCHIC, 5, 0, OTID_STRUCT_PRESET(0), 0);
    CreateMonWithIVs(&gParties[B_TRAINER_OPPONENT_A][0], SPECIES_LITTEN, 7, 0, OTID_STRUCT_PRESET(0), 0);
    u32 shiny = TRUE;
    SetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_IS_SHINY, &shiny);
    struct Pokemon original = gParties[B_TRAINER_OPPONENT_A][0];
    EXPECT(Test_TryGenerateSecondOWEMon(&info, WILD_AREA_LAND));
    EXPECT_EQ(memcmp(&original, &gParties[B_TRAINER_OPPONENT_A][0], sizeof(original)), 0);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][1], MON_DATA_SPECIES), SPECIES_MUDKIP);
    VarSet(VAR_BRISK_BATTLE_FORMAT, BRISK_BATTLE_SINGLES);
    gBattleTypeFlags = BATTLE_TYPE_DOUBLE;
    BriskPrepareBattleFormat();
    BriskPrepareBattleParties();
    EXPECT_EQ(memcmp(&original, &gParties[B_TRAINER_OPPONENT_A][0], sizeof(original)), 0);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][1], MON_DATA_SPECIES), SPECIES_NONE);
    VarSet(VAR_BRISK_BATTLE_FORMAT, BRISK_BATTLE_REGULAR);
    gBattleTypeFlags = 0;
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();
}

TEST("An empty or absent second encounter table leaves the visible Pokemon intact")
{
    static const struct WildPokemon land[NUM_LAND_MONS_ENCOUNTER_SLOTS] = {0};
    const struct WildPokemonInfo info = {20, land};
    ZeroEnemyPartyMons();
    CreateMonWithIVs(&gParties[B_TRAINER_OPPONENT_A][0], SPECIES_LITTEN, 7, 0, OTID_STRUCT_PRESET(0), 0);
    struct Pokemon original = gParties[B_TRAINER_OPPONENT_A][0];
    EXPECT(!Test_TryGenerateSecondOWEMon(&info, WILD_AREA_LAND));
    EXPECT_EQ(memcmp(&original, &gParties[B_TRAINER_OPPONENT_A][0], sizeof(original)), 0);
    EXPECT(!Test_TryGenerateSecondOWEMon(NULL, WILD_AREA_LAND));
    EXPECT_EQ(memcmp(&original, &gParties[B_TRAINER_OPPONENT_A][0], sizeof(original)), 0);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][1], MON_DATA_SPECIES), SPECIES_NONE);
    ZeroEnemyPartyMons();
}
