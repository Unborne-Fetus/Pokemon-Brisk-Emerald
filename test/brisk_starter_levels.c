#include "global.h"
#include "brisk_modes.h"
#include "event_data.h"
#include "pokemon.h"
#include "script_pokemon_util.h"
#include "wild_encounter.h"
#include "test/test.h"

TEST("Brisk starters retain level five and matching EXP in all modes and randomizers")
{
    static const u16 starters[] = {
        SPECIES_BULBASAUR, SPECIES_CHARMANDER, SPECIES_SQUIRTLE,
        SPECIES_TREECKO, SPECIES_TORCHIC, SPECIES_MUDKIP,
        SPECIES_SNIVY, SPECIES_CHIMCHAR, SPECIES_FROAKIE,
    };
    for (u32 mode = BRISK_MODE_NORMAL; mode <= BRISK_MODE_EXTREME; mode++)
    {
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        for (u32 randomized = 0; randomized < 2; randomized++)
        {
            VarSet(VAR_BRISK_RANDOMIZER, randomized ? BRISK_RAND_ALL : 0);
            for (u32 i = 0; i < ARRAY_COUNT(starters); i++)
            {
                ZeroPlayerPartyMons();
                struct PokemonTemplate starter = {
                    .species = starters[i], .level = 5, .origin = GIFTMON_ORIGIN,
                    .nature = NATURE_RANDOM, .gender = MON_GENDER_RANDOM,
                    .ivs = {USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS},
                };
                ScriptGiveStarterMon(&starter);
                struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][0];
                EXPECT_EQ(GetMonData(mon, MON_DATA_SPECIES), BriskRandomSpecies(starters[i], BRISK_RAND_ALL));
                EXPECT_EQ(GetMonData(mon, MON_DATA_LEVEL), 5);
                EXPECT_EQ(GetLevelFromMonExp(mon), 5);
                CalculateMonStats(mon);
                EXPECT_EQ(GetMonData(mon, MON_DATA_LEVEL), 5);
                EXPECT_EQ(starter.level, 5);
            }
        }
    }
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
    VarSet(VAR_BRISK_RANDOMIZER, 0);
    ZeroPlayerPartyMons();
}

TEST("Brisk opening Zigzagoon is level three without changing later encounter scaling")
{
    for (u32 mode = BRISK_MODE_NORMAL; mode <= BRISK_MODE_EXTREME; mode++)
    {
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        VarSet(VAR_BRISK_RANDOMIZER, 0);
        CreateBirchRescueMon();
        struct Pokemon *mon = &gParties[B_TRAINER_OPPONENT_A][0];
        EXPECT_EQ(GetMonData(mon, MON_DATA_SPECIES), SPECIES_ZIGZAGOON);
        EXPECT_EQ(GetMonData(mon, MON_DATA_LEVEL), 3);
        EXPECT_EQ(GetLevelFromMonExp(mon), 3);
        CalculateMonStats(mon);
        EXPECT_EQ(GetMonData(mon, MON_DATA_LEVEL), 3);
        CreateWildMon(SPECIES_ZIGZAGOON, 3);
        EXPECT_EQ(GetMonData(mon, MON_DATA_LEVEL), ScaleBriskLevel(3, SPECIES_ZIGZAGOON));
    }
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
}

TEST("Brisk level five gifts still scale outside starter selection")
{
    for (u32 mode = BRISK_MODE_NORMAL; mode <= BRISK_MODE_EXTREME; mode++)
    {
        ZeroPlayerPartyMons();
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        VarSet(VAR_BRISK_RANDOMIZER, 0);
        struct PokemonTemplate gift = {.species = SPECIES_BULBASAUR, .level = 5, .origin = GIFTMON_ORIGIN};
        ScriptGiveMonParameterized(B_SIDE_PLAYER, PARTY_SIZE, &gift);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_LEVEL), ScaleBriskLevel(5, SPECIES_BULBASAUR));
    }
    ZeroPlayerPartyMons();
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
}
