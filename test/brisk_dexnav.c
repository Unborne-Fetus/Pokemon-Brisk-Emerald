#include "global.h"
#include "dexnav.h"
#include "event_data.h"
#include "pokedex.h"
#include "pokemon.h"
#include "sprite.h"
#include "wild_encounter.h"
#include "test/test.h"

u8 Test_OpenBriskDexNav(const struct WildPokemonInfo *, const struct WildPokemonInfo *, const struct WildPokemonInfo *);
void Test_BriskDexNavInput(u8 task, u16 keys);
enum Species Test_BriskDexNavSpecies(void);
u32 Test_BriskDexNavCount(u32 section);
u32 Test_BriskDexNavPages(u32 section);
u32 Test_BriskDexNavPage(u32 section);
u32 Test_BriskDexNavRow(void);
u32 Test_BriskDexNavCol(void);
bool32 Test_BriskDexNavCaught(const struct WildPokemonInfo *info, u32 slots);
void Test_CloseBriskDexNav(u8 task);

static void FillTable(struct WildPokemon *mons, u32 slots, u32 start)
{
    for (u32 i = 0; i < slots; i++)
    {
        mons[i] = (struct WildPokemon){.species = start + i, .minLevel = 5, .maxLevel = 8};
        GetSetPokedexFlag(SpeciesToNationalPokedexNum(start + i), FLAG_SET_SEEN);
    }
}

TEST("DexNav pages expose all 25 land and 12 water species with correct selection and registration")
{
    struct WildPokemon land[NUM_LAND_MONS_ENCOUNTER_SLOTS], water[NUM_WATER_MONS_ENCOUNTER_SLOTS];
    FillTable(land, ARRAY_COUNT(land), SPECIES_BULBASAUR);
    FillTable(water, ARRAY_COUNT(water), SPECIES_PSYDUCK);
    const struct WildPokemonInfo landInfo = {20, land}, waterInfo = {10, water};
    u8 task = Test_OpenBriskDexNav(&landInfo, &waterInfo, NULL);
    EXPECT_EQ(Test_BriskDexNavPages(ENCOUNTER_TYPE_LAND), 3);
    EXPECT_EQ(Test_BriskDexNavPages(ENCOUNTER_TYPE_WATER), 3);
    EXPECT_EQ(Test_BriskDexNavSpecies(), land[0].species);
    Test_BriskDexNavInput(task, R_BUTTON);
    EXPECT_EQ(Test_BriskDexNavSpecies(), land[12].species);
    Test_BriskDexNavInput(task, DPAD_DOWN);
    EXPECT_EQ(Test_BriskDexNavSpecies(), land[18].species);
    Test_BriskDexNavInput(task, R_BUTTON); // Last page has just one land species.
    EXPECT_EQ(Test_BriskDexNavRow(), ROW_LAND_TOP);
    EXPECT_EQ(Test_BriskDexNavSpecies(), land[24].species);
    Test_BriskDexNavInput(task, DPAD_RIGHT);
    EXPECT_EQ(Test_BriskDexNavCol(), 0);
    Test_BriskDexNavInput(task, SELECT_BUTTON);
    EXPECT_EQ(VarGet(DN_VAR_SPECIES) & DEXNAV_MASK_SPECIES, land[24].species);
    Test_BriskDexNavInput(task, DPAD_DOWN); // Skip empty land-bottom and hidden rows.
    EXPECT_EQ(Test_BriskDexNavRow(), ROW_WATER);
    EXPECT_EQ(Test_BriskDexNavSpecies(), water[0].species);
    Test_BriskDexNavInput(task, R_BUTTON);
    EXPECT_EQ(Test_BriskDexNavSpecies(), water[5].species);
    Test_BriskDexNavInput(task, R_BUTTON);
    Test_BriskDexNavInput(task, DPAD_RIGHT);
    EXPECT_EQ(Test_BriskDexNavSpecies(), water[11].species);
    Test_BriskDexNavInput(task, A_BUTTON);
    EXPECT_EQ(gSpecialVar_0x8000, water[11].species);
    EXPECT_EQ(gSpecialVar_0x8001, ENCOUNTER_TYPE_WATER);
    Test_CloseBriskDexNav(task);
}

TEST("DexNav discards empty and duplicate slots and paging does not leak icon sprites")
{
    struct WildPokemon land[NUM_LAND_MONS_ENCOUNTER_SLOTS] = {0};
    land[0].species = land[1].species = SPECIES_BULBASAUR;
    land[24].species = SPECIES_CHARMANDER;
    GetSetPokedexFlag(NATIONAL_DEX_BULBASAUR, FLAG_SET_SEEN);
    GetSetPokedexFlag(NATIONAL_DEX_CHARMANDER, FLAG_SET_SEEN);
    const struct WildPokemonInfo info = {20, land};
    u8 task = Test_OpenBriskDexNav(&info, NULL, NULL);
    EXPECT_EQ(Test_BriskDexNavCount(ENCOUNTER_TYPE_LAND), 2);
    EXPECT_EQ(Test_BriskDexNavCount(ENCOUNTER_TYPE_WATER), 0);
    EXPECT_EQ(Test_BriskDexNavPages(ENCOUNTER_TYPE_LAND), 1);
    for (u32 i = 0; i < 100; i++)
    {
        Test_BriskDexNavInput(task, R_BUTTON);
        u32 sprites = 0;
        for (u32 j = 0; j < MAX_SPRITES; j++)
            if (gSprites[j].inUse) sprites++;
        EXPECT_EQ(sprites, DEXNAV_VISIBLE_ICONS + 3);
    }
    Test_BriskDexNavInput(task, DPAD_LEFT);
    EXPECT_EQ(Test_BriskDexNavSpecies(), SPECIES_CHARMANDER);
    Test_CloseBriskDexNav(task);
}

TEST("DexNav empty maps are safe and completion includes species on the final page")
{
    u8 task = Test_OpenBriskDexNav(NULL, NULL, NULL);
    Test_BriskDexNavInput(task, DPAD_DOWN);
    Test_BriskDexNavInput(task, L_BUTTON);
    Test_BriskDexNavInput(task, SELECT_BUTTON);
    EXPECT_EQ(Test_BriskDexNavSpecies(), SPECIES_NONE);
    EXPECT_EQ(Test_BriskDexNavPages(ENCOUNTER_TYPE_LAND), 1);
    Test_CloseBriskDexNav(task);
    struct WildPokemon land[NUM_LAND_MONS_ENCOUNTER_SLOTS];
    FillTable(land, ARRAY_COUNT(land), SPECIES_BULBASAUR);
    const struct WildPokemonInfo info = {20, land};
    // Use fresh caught flags: only the final species has not been caught.
    for (u32 i = 0; i + 1 < ARRAY_COUNT(land); i++)
        GetSetPokedexFlag(SpeciesToNationalPokedexNum(land[i].species), FLAG_SET_CAUGHT);
    EXPECT(!Test_BriskDexNavCaught(&info, ARRAY_COUNT(land)));
    GetSetPokedexFlag(SpeciesToNationalPokedexNum(land[24].species), FLAG_SET_CAUGHT);
    EXPECT(Test_BriskDexNavCaught(&info, ARRAY_COUNT(land)));
    EXPECT(!Test_BriskDexNavCaught(NULL, ARRAY_COUNT(land)));
}

TEST("DexNav hidden entries stay gated by the detector and searches keep hidden context")
{
    struct WildPokemon hidden[NUM_HIDDEN_MONS_ENCOUNTER_SLOTS];
    FillTable(hidden, ARRAY_COUNT(hidden), SPECIES_BULBASAUR);
    const struct WildPokemonInfo info = {0, hidden}; // Zero means land, not disabled.
    FlagClear(DN_FLAG_DETECTOR_MODE);
    u8 task = Test_OpenBriskDexNav(NULL, NULL, &info);
    Test_BriskDexNavInput(task, DPAD_UP);
    EXPECT_EQ(Test_BriskDexNavRow(), ROW_HIDDEN);
    EXPECT_EQ(Test_BriskDexNavCount(ENCOUNTER_TYPE_HIDDEN), 3);
    EXPECT_EQ(Test_BriskDexNavSpecies(), SPECIES_NONE);
    FlagSet(DN_FLAG_DETECTOR_MODE);
    EXPECT_EQ(Test_BriskDexNavSpecies(), hidden[0].species);
    Test_BriskDexNavInput(task, A_BUTTON);
    EXPECT_EQ(gSpecialVar_0x8000, hidden[0].species);
    EXPECT_EQ(gSpecialVar_0x8002, TRUE);
    Test_CloseBriskDexNav(task);
    FlagClear(DN_FLAG_DETECTOR_MODE);
}

TEST("DexNav registration persists in a real save variable and stale search flags never touch trainers")
{
    VarSet(DN_VAR_SPECIES, SPECIES_PIKACHU | (ENCOUNTER_TYPE_WATER << 14));
    EXPECT_EQ(VarGet(DN_VAR_SPECIES) & DEXNAV_MASK_SPECIES, SPECIES_PIKACHU);
    EXPECT_EQ(VarGet(DN_VAR_SPECIES) >> 14, ENCOUNTER_TYPE_WATER);
    VarSet(DN_VAR_SPECIES, SPECIES_NONE);
    FlagSet(TRAINER_FLAGS_START);
    FlagSet(TRAINER_FLAGS_START + 1);
    FlagSet(TRAINER_FLAGS_START + 2);
    FlagSet(DN_FLAG_SEARCHING);
    EXPECT(!TryStartDexNavSearch()); // No active search object after loading a save.
    EXPECT(!FlagGet(DN_FLAG_SEARCHING));
    for (u32 i = 0; i < 3; i++) EXPECT(FlagGet(TRAINER_FLAGS_START + i));
}
