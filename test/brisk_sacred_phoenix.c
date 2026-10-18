#include "global.h"
#include "pokemon.h"
#include "pokedex.h"
#include "event_data.h"
#include "brisk_options.h"
#include "wild_encounter.h"
#include "main.h"
#include "task.h"
#include "constants/map_types.h"
#include "constants/region_map_sections.h"
#include "test/test.h"

extern u8 Test_OpenBriskBattleOptions(void);
extern void Test_CloseBriskThemeOption(u8 taskId);

TEST("Island shiny odds are local, optional and limited to wild outdoor encounters")
{
    u16 savedOdds = VarGet(VAR_BRISK_SHINY_ODDS);
    u16 savedToggle = VarGet(VAR_BRISK_ISLAND_SHINIES_DISABLED);
    struct MapHeader header = gMapHeader;
    static const u16 islands[] = {MAPSEC_ROUTE_143, MAPSEC_ROUTE_144, MAPSEC_ORANGE_CITY, MAPSEC_ROUTE_145, MAPSEC_ROUTE_146, MAPSEC_ROUTE_149, MAPSEC_ROUTE_150};
    VarSet(VAR_BRISK_SHINY_ODDS, 1);
    VarSet(VAR_BRISK_ISLAND_SHINIES_DISABLED, 0);
    for (u32 i = 0; i < ARRAY_COUNT(islands); i++)
    {
        EXPECT_EQ(GetBriskWildShinyDenominator(islands[i]), 50);
        gMapHeader.regionMapSectionId = islands[i];
        gMapHeader.mapType = MAP_TYPE_ROUTE;
        BriskSetWildShinyContext(TRUE);
        EXPECT_EQ(GetBriskShinyThreshold(), 1311);
        BriskSetWildShinyContext(FALSE);
        EXPECT_EQ(GetBriskShinyThreshold(), 8); // Gifts/eggs keep global odds.
        BriskSetWildShinyContext(TRUE);
        gMapHeader.mapType = MAP_TYPE_UNDERGROUND;
        EXPECT_EQ(GetBriskShinyThreshold(), 8);
        BriskSetWildShinyContext(FALSE);
    }
    EXPECT_EQ(GetBriskWildShinyDenominator(MAPSEC_ROUTE_147), 8192);
    EXPECT_EQ(GetBriskWildShinyDenominator(MAPSEC_ROUTE_148), 8192);
    EXPECT_EQ(GetBriskWildShinyDenominator(MAPSEC_ROUTE_101), 8192);
    VarSet(VAR_BRISK_ISLAND_SHINIES_DISABLED, 1);
    EXPECT_EQ(GetBriskWildShinyDenominator(MAPSEC_ROUTE_145), 8192);
    VarSet(VAR_BRISK_SHINY_ODDS, 7);
    EXPECT_EQ(GetBriskWildShinyDenominator(MAPSEC_ROUTE_145), 128);
    VarSet(VAR_BRISK_ISLAND_SHINIES_DISABLED, savedToggle);
    VarSet(VAR_BRISK_SHINY_ODDS, savedOdds);
    gMapHeader = header;
}

TEST("The island shiny option toggles from its own row and persists when reopened")
{
    u16 saved = VarGet(VAR_BRISK_ISLAND_SHINIES_DISABLED);
    VarSet(VAR_BRISK_ISLAND_SHINIES_DISABLED, 0);
    u8 task = Test_OpenBriskBattleOptions();
    for (u32 i = 0; i < 2; i++)
    {
        gMain.newKeys = 0;
        gMain.newAndRepeatedKeys = DPAD_DOWN;
        RunTasks();
    }
    gMain.newAndRepeatedKeys = 0;
    gMain.newKeys = A_BUTTON;
    RunTasks();
    EXPECT(!BriskIslandShiniesEnabled());
    gMain.newKeys = 0;
    Test_CloseBriskThemeOption(task);
    task = Test_OpenBriskBattleOptions();
    EXPECT(!BriskIslandShiniesEnabled());
    Test_CloseBriskThemeOption(task);
    VarSet(VAR_BRISK_ISLAND_SHINIES_DISABLED, saved);
}

TEST("All twelve Sacred Phoenix species have valid boxed data, moves, art and distinct Dex entries")
{
    for (u32 species = SPECIES_SLUGAQUA; species <= SPECIES_BOARHORN; species++)
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
        EXPECT(gSpeciesInfo[species].palette != gSpeciesInfo[species].shinyPalette);
        EXPECT(gSpeciesInfo[species].iconSprite != NULL);
        EXPECT(gSpeciesInfo[species].overworldData.images != NULL);
        EXPECT(gSpeciesInfo[species].overworldPalette != gSpeciesInfo[species].overworldShinyPalette);
        EXPECT_EQ(NationalPokedexNumToSpecies(SpeciesToNationalPokedexNum(species)), species);
        BoxMonToMon(&mon.box, &restored);
        EXPECT_EQ(GetMonData(&restored, MON_DATA_SPECIES), species);
        EXPECT_EQ(GetMonData(&restored, MON_DATA_SANITY_IS_BAD_EGG), FALSE);
    }
}

TEST("Sacred Phoenix Dex bits are independent and preserve all older Dex flags and save sizes")
{
    u16 savedSeen = VarGet(VAR_BRISK_SACRED_DEX_SEEN), savedCaught = VarGet(VAR_BRISK_SACRED_DEX_CAUGHT);
    u32 oldExtra = gSaveBlock2Ptr->pokedex.unknown3;
    u8 seen[NUM_DEX_FLAG_BYTES], caught[NUM_DEX_FLAG_BYTES], custom[4];
    memcpy(seen, gSaveBlock1Ptr->dexSeen, sizeof(seen));
    memcpy(caught, gSaveBlock1Ptr->dexCaught, sizeof(caught));
    memcpy(custom, gSaveBlock1Ptr->customDexSeen, 2);
    memcpy(custom + 2, gSaveBlock1Ptr->customDexCaught, 2);
    for (u32 dex = NATIONAL_DEX_SLUGAQUA; dex <= NATIONAL_DEX_BOARHORN; dex++)
    {
        VarSet(VAR_BRISK_SACRED_DEX_SEEN, 0);
        VarSet(VAR_BRISK_SACRED_DEX_CAUGHT, 0);
        GetSetPokedexFlag(dex, FLAG_SET_SEEN);
        for (u32 other = NATIONAL_DEX_SLUGAQUA; other <= NATIONAL_DEX_BOARHORN; other++)
        {
            EXPECT_EQ(GetSetPokedexFlag(other, FLAG_GET_SEEN), other == dex);
            EXPECT_EQ(GetSetPokedexFlag(other, FLAG_GET_CAUGHT), FALSE);
        }
        GetSetPokedexFlag(dex, FLAG_SET_CAUGHT);
        EXPECT_EQ(GetSetPokedexFlag(dex, FLAG_GET_CAUGHT), TRUE);
    }
    EXPECT_EQ(memcmp(seen, gSaveBlock1Ptr->dexSeen, sizeof(seen)), 0);
    EXPECT_EQ(memcmp(caught, gSaveBlock1Ptr->dexCaught, sizeof(caught)), 0);
    EXPECT_EQ(memcmp(custom, gSaveBlock1Ptr->customDexSeen, 2), 0);
    EXPECT_EQ(memcmp(custom + 2, gSaveBlock1Ptr->customDexCaught, 2), 0);
    EXPECT_EQ(gSaveBlock2Ptr->pokedex.unknown3, oldExtra);
    EXPECT_EQ(sizeof(struct SaveBlock1), 15572);
    EXPECT_EQ(sizeof(struct SaveBlock2), 3884);
    VarSet(VAR_BRISK_SACRED_DEX_SEEN, savedSeen);
    VarSet(VAR_BRISK_SACRED_DEX_CAUGHT, savedCaught);
}

TEST("Sacred Phoenix level evolutions and the gender branch reach the correct species")
{
    static const u16 lines[][3] = {
        {SPECIES_SLUGAQUA, 20, SPECIES_FLABAQUA},
        {SPECIES_FLABAQUA, 40, SPECIES_GLAUTILUS},
        {SPECIES_BULBENT, 18, SPECIES_MAGUORN},
        {SPECIES_MAGUORN, 38, SPECIES_TREEWISE},
        {SPECIES_LARFAE, 12, SPECIES_CHRYSANTLE},
        {SPECIES_FURFANG, 34, SPECIES_BOARHORN},
    };
    bool32 canStop;
    for (u32 i = 0; i < ARRAY_COUNT(lines); i++)
    {
        struct Pokemon mon;
        CreateMonWithIVs(&mon, lines[i][0], lines[i][1] - 1, 0, OTID_STRUCT_PLAYER_ID, 31);
        EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_NORMAL, ITEM_NONE, NULL, &canStop, CHECK_EVO), SPECIES_NONE);
        u8 level = lines[i][1];
        SetMonData(&mon, MON_DATA_LEVEL, &level);
        EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_NORMAL, ITEM_NONE, NULL, &canStop, CHECK_EVO), lines[i][2]);
    }
    for (u32 female = 0; female < 2; female++)
    {
        struct Pokemon mon;
        CreateMonWithIVs(&mon, SPECIES_CHRYSANTLE, 36, female ? 0 : 255, OTID_STRUCT_PLAYER_ID, 31);
        EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_NORMAL, ITEM_NONE, NULL, &canStop, CHECK_EVO), female ? SPECIES_DUCHEFLY : SPECIES_DRUICHY);
    }
}
