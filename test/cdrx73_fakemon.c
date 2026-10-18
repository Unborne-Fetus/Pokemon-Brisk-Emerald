#include "global.h"
#include "pokemon.h"
#include "pokedex.h"
#include "constants/abilities.h"
#include "constants/form_change_types.h"
#include "test/test.h"

TEST("CDRX73 imports create valid mons with native 80px sprites, palettes and full movesets")
{
    for (u32 species = SPECIES_NOCTARCH; species <= SPECIES_GLALIE_HALLOWEEN; species++)
    {
        struct Pokemon mon, restored;
        CreateMonWithIVs(&mon, species, 70, 0, OTID_STRUCT_PLAYER_ID, 31);
        GiveMonInitialMoveset(&mon);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_SPECIES), species);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_SANITY_IS_BAD_EGG), FALSE);
        EXPECT_GT(GetMonData(&mon, MON_DATA_MAX_HP), 0);
        for (u32 i = 0; i < MAX_MON_MOVES; i++)
            EXPECT_NE(GetMonData(&mon, MON_DATA_MOVE1 + i), MOVE_NONE);
        EXPECT(gSpeciesInfo[species].frontPic80 != NULL);
        EXPECT(gSpeciesInfo[species].backPic80 != NULL);
        EXPECT(gSpeciesInfo[species].iconSprite != NULL);
        EXPECT(gSpeciesInfo[species].palette != NULL);
        EXPECT(gSpeciesInfo[species].shinyPalette != NULL);
        EXPECT_NE(gSpeciesInfo[species].abilities[0], ABILITY_NONE);
        BoxMonToMon(&mon.box, &restored);
        EXPECT_EQ(GetMonData(&restored, MON_DATA_SPECIES), species);
    }
}

TEST("CDRX73 and Phoenix Dex flags remain independent without changing save layout")
{
    u8 seen[NUM_DEX_FLAG_BYTES], caught[NUM_DEX_FLAG_BYTES], customSeen[2], customCaught[2];
    u32 extra = gSaveBlock2Ptr->pokedex.unknown3;
    memcpy(seen, gSaveBlock1Ptr->dexSeen, sizeof(seen));
    memcpy(caught, gSaveBlock1Ptr->dexCaught, sizeof(caught));
    memcpy(customSeen, gSaveBlock1Ptr->customDexSeen, 2);
    memcpy(customCaught, gSaveBlock1Ptr->customDexCaught, 2);
    memset(gSaveBlock1Ptr->customDexSeen, 0xFF, 2);
    memset(gSaveBlock1Ptr->customDexCaught, 0xFF, 2);
    gSaveBlock2Ptr->pokedex.unknown3 = 0xFFFFFFFF;
    for (u32 dex = NATIONAL_DEX_HISSIORITE; dex <= NATIONAL_DEX_STEAM_SHELL; dex++)
    {
        EXPECT_EQ(GetSetPokedexFlag(dex, FLAG_GET_SEEN), FALSE);
        EXPECT_EQ(GetSetPokedexFlag(dex, FLAG_GET_CAUGHT), FALSE);
        GetSetPokedexFlag(dex, FLAG_SET_SEEN);
        EXPECT_EQ(GetSetPokedexFlag(dex, FLAG_GET_SEEN), TRUE);
        EXPECT_EQ(GetSetPokedexFlag(dex, FLAG_GET_CAUGHT), FALSE);
        GetSetPokedexFlag(dex, FLAG_SET_CAUGHT);
    }
    for (u32 dex = NATIONAL_DEX_HISSIORITE; dex <= NATIONAL_DEX_STEAM_SHELL; dex++)
    {
        EXPECT_EQ(GetSetPokedexFlag(dex, FLAG_GET_SEEN), TRUE);
        EXPECT_EQ(GetSetPokedexFlag(dex, FLAG_GET_CAUGHT), TRUE);
        EXPECT_EQ(SpeciesToNationalPokedexNum(NationalPokedexNumToSpecies(dex)), dex);
    }
    EXPECT_EQ(memcmp(seen, gSaveBlock1Ptr->dexSeen, sizeof(seen)), 0);
    EXPECT_EQ(memcmp(caught, gSaveBlock1Ptr->dexCaught, sizeof(caught)), 0);
    EXPECT_EQ(sizeof(struct SaveBlock1), 15572);
    EXPECT_EQ(sizeof(struct SaveBlock2), 3884);
    memcpy(gSaveBlock1Ptr->customDexSeen, customSeen, 2);
    memcpy(gSaveBlock1Ptr->customDexCaught, customCaught, 2);
    gSaveBlock2Ptr->pokedex.unknown3 = extra;
}

TEST("CDRX73 evolution chains reach their authored destinations")
{
    static const u16 routes[][3] = {
        {SPECIES_NOCTOWL, 42, SPECIES_NOCTARCH},
        {SPECIES_MIGHTYENA, 38, SPECIES_ALPHAYENA},
        {SPECIES_RATICATE, 36, SPECIES_BRUTICATE},
        {SPECIES_MAROWAK, 42, SPECIES_MARROWAK},
        {SPECIES_RHYDON, 60, SPECIES_RHYFORTRESS},
        {SPECIES_SANDSLASH, 42, SPECIES_DUNESHRED},
        {SPECIES_SKARMORY, 48, SPECIES_SKARMAGEDN},
        {SPECIES_SHARPEDO, 48, SPECIES_SHARKAISER},
        {SPECIES_ELECTIVIRE, 60, SPECIES_ELECTITAN},
        {SPECIES_GRANBULL, 42, SPECIES_GRANDBULLY},
        {SPECIES_SUDOWOODO, 42, SPECIES_SUDOSYLVAN},
        {SPECIES_PARASECT, 40, SPECIES_PARASOUL},
        {SPECIES_LINOONE, 40, SPECIES_LINEVOLVER},
        {SPECIES_TENTACRUEL, 50, SPECIES_TENTACROWN},
        {SPECIES_LITWICK_HALLOWEEN, 25, SPECIES_LAMPENT_HALLOWEEN},
        {SPECIES_LAMPENT_HALLOWEEN, 41, SPECIES_CHANDELURE_HALLOWEEN},
        {SPECIES_SINISTEA_HALLOWEEN, 30, SPECIES_POLTEAGEIST_HALLOWEEN},
        {SPECIES_PONYTA_HALLOWEEN, 40, SPECIES_RAPIDASH_HALLOWEEN},
        {SPECIES_TANGELA_HALLOWEEN, 36, SPECIES_TANGROWTH_HALLOWEEN},
        {SPECIES_TIMBURR_HALLOWEEN, 25, SPECIES_GURDURR_HALLOWEEN},
        {SPECIES_GURDURR_HALLOWEEN, 40, SPECIES_CONKELDURR_HALLOWEEN},
        {SPECIES_SNORUNT_HALLOWEEN, 42, SPECIES_GLALIE_HALLOWEEN},
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

#include "battle.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("CDRX73 Dragoninite C and original Dragoninite select distinct Mega forms and revert correctly")
{
    enum Item stone;
    enum Species mega;
    PARAMETRIZE { stone = ITEM_DRAGONINITE_CDRX; mega = SPECIES_DRAGONITE_MEGA_CDRX; }
    PARAMETRIZE { stone = ITEM_DRAGONINITE; mega = SPECIES_DRAGONITE_MEGA; }
    GIVEN {
        PLAYER(SPECIES_DRAGONITE) { Item(stone); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE, gimmick: GIMMICK_MEGA); MOVE(opponent, MOVE_CELEBRATE); }
    } THEN {
        EXPECT_EQ(player->species, mega);
        struct FormChangeContext ctx = {.currentSpecies = mega, .heldItem = stone, .method = FORM_CHANGE_END_BATTLE};
        EXPECT_EQ(GetFormChangeTargetSpecies_Internal(ctx), SPECIES_DRAGONITE);
        gMain.inBattle = FALSE;
    }
}

TEST("Rhydon branches to Rhyfortress by level and Rhyperior by Protector")
{
    const struct Evolution *evolutions = gSpeciesInfo[SPECIES_RHYDON].evolutions;
    bool32 fortress = FALSE, perior = FALSE;
    for (u32 i = 0; evolutions[i].method != EVOLUTIONS_END; i++)
    {
        if (evolutions[i].targetSpecies == SPECIES_RHYFORTRESS)
            fortress = evolutions[i].method == EVO_LEVEL && evolutions[i].param == 60;
        if (evolutions[i].targetSpecies == SPECIES_RHYPERIOR)
            perior = evolutions[i].method == EVO_ITEM && evolutions[i].param == ITEM_PROTECTOR;
    }
    EXPECT(fortress && perior);
    EXPECT(gSpeciesInfo[SPECIES_RHYPERIOR].evolutions == NULL);
}
