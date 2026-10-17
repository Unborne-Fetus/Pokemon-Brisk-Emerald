#include "global.h"
#include "brisk_fakemon.h"
#include "pokemon.h"
#include "pokedex.h"
#include "string_util.h"
#include "constants/characters.h"
#include "constants/abilities.h"
#include "test/test.h"
#include "test/battle.h"

TEST("Phoenix Fakemon retain the previous save sizes, Dex offsets and existing species IDs")
{
    EXPECT_EQ(sizeof(struct SaveBlock1), 15572);
    EXPECT_EQ(sizeof(struct SaveBlock2), 3884);
    EXPECT_EQ(offsetof(struct SaveBlock1, dexSeen), 13724);
    EXPECT_EQ(offsetof(struct SaveBlock1, dexCaught), 13853);
    EXPECT_EQ(offsetof(struct SaveBlock1, recordMixingGift), 15004);
    EXPECT_EQ(NUM_DEX_FLAG_BYTES, 129);
    EXPECT_EQ(SPECIES_NINETALES_MEGA, 1625);
    EXPECT_EQ(SPECIES_HISSIORITE, 1626);
    EXPECT_EQ(NATIONAL_DEX_PECHARUNT, 1025);
}

TEST("Phoenix Fakemon create valid boxed monsters with attacks, abilities and distinct National Dex records")
{
    for (enum Species species = SPECIES_HISSIORITE; species <= SPECIES_NUENFLU; species++)
    {
        struct Pokemon mon, unboxed;
        CreateMonWithIVs(&mon, species, 70, 0, OTID_STRUCT_PLAYER_ID, 31);
        GiveMonInitialMoveset(&mon);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_SPECIES), species);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_LEVEL), 70);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_SANITY_IS_BAD_EGG), FALSE);
        EXPECT_GT(GetMonData(&mon, MON_DATA_MAX_HP), 0);
        for (u32 i = 0; i < MAX_MON_MOVES; i++)
            EXPECT_NE(GetMonData(&mon, MON_DATA_MOVE1 + i), MOVE_NONE);
        for (u32 i = 0; i < NUM_ABILITY_SLOTS; i++)
            EXPECT_NE(gSpeciesInfo[species].abilities[i], ABILITY_NONE);
        EXPECT(gSpeciesInfo[species].frontPic != NULL);
        EXPECT(gSpeciesInfo[species].backPic != NULL);
        EXPECT(gSpeciesInfo[species].iconSprite != NULL);
        EXPECT(gSpeciesInfo[species].palette != NULL);
        EXPECT(gSpeciesInfo[species].shinyPalette != NULL);
        EXPECT(gSpeciesInfo[species].teachableLearnset != NULL);
        EXPECT(gSpeciesInfo[species].eggMoveLearnset != NULL);
        EXPECT_EQ(NationalPokedexNumToSpecies(SpeciesToNationalPokedexNum(species)), species);
        BoxMonToMon(&mon.box, &unboxed);
        EXPECT_EQ(GetMonData(&unboxed, MON_DATA_SPECIES), species);
        EXPECT_EQ(GetMonData(&unboxed, MON_DATA_SANITY_IS_BAD_EGG), FALSE);
    }
}

TEST("Phoenix Fakemon new Dex flags initialize legacy padding and never overwrite canonical records")
{
    u8 seen[NUM_DEX_FLAG_BYTES], caught[NUM_DEX_FLAG_BYTES], extraSeen[2], extraCaught[2];
    memcpy(seen, gSaveBlock1Ptr->dexSeen, sizeof(seen));
    memcpy(caught, gSaveBlock1Ptr->dexCaught, sizeof(caught));
    memcpy(extraSeen, gSaveBlock1Ptr->customDexSeen, sizeof(extraSeen));
    memcpy(extraCaught, gSaveBlock1Ptr->customDexCaught, sizeof(extraCaught));
    memset(gSaveBlock1Ptr->customDexSeen, 0xFF, 2);
    memset(gSaveBlock1Ptr->customDexCaught, 0xFF, 2);
    for (u32 dex = NATIONAL_DEX_HISSIORITE; dex <= NATIONAL_DEX_NUENFLU; dex++)
    {
        EXPECT_EQ(GetSetPokedexFlag(dex, FLAG_GET_SEEN), FALSE);
        EXPECT_EQ(GetSetPokedexFlag(dex, FLAG_GET_CAUGHT), FALSE);
        GetSetPokedexFlag(dex, FLAG_SET_SEEN);
        GetSetPokedexFlag(dex, FLAG_SET_CAUGHT);
        EXPECT_EQ(GetSetPokedexFlag(dex, FLAG_GET_SEEN), TRUE);
        EXPECT_EQ(GetSetPokedexFlag(dex, FLAG_GET_CAUGHT), TRUE);
    }
    EXPECT_EQ(memcmp(seen, gSaveBlock1Ptr->dexSeen, sizeof(seen)), 0);
    EXPECT_EQ(memcmp(caught, gSaveBlock1Ptr->dexCaught, sizeof(caught)), 0);
    EXPECT_EQ(GetSetPokedexFlag(0, FLAG_GET_SEEN), FALSE);
    EXPECT_EQ(GetSetPokedexFlag(NATIONAL_DEX_COUNT + 1, FLAG_GET_SEEN), FALSE);
    memcpy(gSaveBlock1Ptr->customDexSeen, extraSeen, sizeof(extraSeen));
    memcpy(gSaveBlock1Ptr->customDexCaught, extraCaught, sizeof(extraCaught));
}

TEST("Phoenix Fakemon evolve into the supplied destinations at the selected levels")
{
    static const u16 routes[][3] = {
        {SPECIES_HISSIORITE, 16, SPECIES_COBARETT},
        {SPECIES_COBARETT, 36, SPECIES_PYTHONOVA},
        {SPECIES_BAOBY, 32, SPECIES_BAOBARAFFE},
        {SPECIES_DRASHIMI, 30, SPECIES_TSUSHIMI},
        {SPECIES_TSUSHIMI, 55, SPECIES_TOBISHIMI},
        {SPECIES_MERLICUN, 45, SPECIES_FIROMENIS},
    };
    for (u32 i = 0; i < ARRAY_COUNT(routes); i++)
    {
        struct Pokemon mon;
        bool32 canStop;
        CreateMonWithIVs(&mon, routes[i][0], routes[i][1] - 1, 0, OTID_STRUCT_PLAYER_ID, 31);
        EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_NORMAL, ITEM_NONE, NULL, &canStop, CHECK_EVO), SPECIES_NONE);
        CreateMonWithIVs(&mon, routes[i][0], routes[i][1], 0, OTID_STRUCT_PLAYER_ID, 31);
        EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_NORMAL, ITEM_NONE, NULL, &canStop, CHECK_EVO), routes[i][2]);
    }
}

TEST("Phoenix Fakemon long supplied entries are readable in bounded pages, and missing text stays blank")
{
    for (u32 species = SPECIES_HISSIORITE; species <= SPECIES_NUENFLU; species++)
    {
        u8 page[256];
        u32 pages = BriskFakemon_CopyDexPage(species, 0, page);
        if (species <= SPECIES_BAOBARAFFE)
        {
            EXPECT_GE(pages, 2);
            for (u32 i = 0; i < pages; i++)
            {
                EXPECT_EQ(BriskFakemon_CopyDexPage(species, i, page), pages);
                EXPECT_LT(StringLength(page), sizeof(page));
                EXPECT_NE(page[0], EOS);
                u32 lines = 1;
                for (u32 j = 0; page[j] != EOS; j++) lines += page[j] == CHAR_NEWLINE;
                EXPECT_LE(lines, 4);
            }
        }
        else
        {
            EXPECT_EQ(pages, 1);
            EXPECT_EQ(page[0], EOS);
        }
    }
}

SINGLE_BATTLE_TEST("Phoenix Fakemon native 80px sprites load in both battle positions")
{
    u32 species;
    PARAMETRIZE { species = SPECIES_TSUSHIMI; }
    PARAMETRIZE { species = SPECIES_TOBISHIMI; }
    PARAMETRIZE { species = SPECIES_FIROMENIS; }
    GIVEN {
        PLAYER(species) { Moves(MOVE_CELEBRATE); }
        OPPONENT(species) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_CELEBRATE); }
    } THEN {
        EXPECT(gSpeciesInfo[species].frontPic80 != NULL);
        EXPECT(gSpeciesInfo[species].backPic80 != NULL);
        gMain.inBattle = FALSE;
    }
}
