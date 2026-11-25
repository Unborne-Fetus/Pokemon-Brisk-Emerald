#include "global.h"
#include "pokemon_storage_system.h"
#include "save.h"
#include "malloc.h"
#include "pokemon.h"
#include "string_util.h"
#include "test/test.h"

// If you would like to ensure save compatibility, update the values below with those for your hack. You can find these through the debug menu.
// Please note that this simple check is not 100% foolproof, but should be able to catch most unintended shifts.
#define T_SAVEBLOCK1_SIZE 15572
#define T_SAVEBLOCK2_SIZE 3884
#define T_SAVEBLOCK3_SIZE 524
#define T_POKEMONSTORAGE_SIZE 48604

TEST("SaveBlock1 preserves the Brisk save layout")
{
    EXPECT_EQ(sizeof(struct SaveBlock1), T_SAVEBLOCK1_SIZE);
}

TEST("SaveBlock2 preserves the Brisk save layout")
{
    EXPECT_EQ(sizeof(struct SaveBlock2), T_SAVEBLOCK2_SIZE);
}

TEST("SaveBlock3 preserves the Brisk save layout")
{
    EXPECT_EQ(sizeof(struct SaveBlock3), T_SAVEBLOCK3_SIZE);
}

TEST("PokemonStorage preserves the Brisk save layout")
{
    EXPECT_EQ(sizeof(struct PokemonStorage), T_POKEMONSTORAGE_SIZE);
}

#undef T_SAVEBLOCK1_SIZE
#undef T_SAVEBLOCK2_SIZE
#undef T_SAVEBLOCK3_SIZE
#undef T_POKEMONSTORAGE_SIZE

// Use the production migration path with a scratch storage buffer. No flash
// or existing user save is touched by these regression tests.
void Test_MigrateLegacyPokemonStorage(void);
u16 Test_GetPokemonStorageExtensionChunkSize(u32 chunk);
struct BriskLegacyStorageFixture
{
    u8 currentBox;
    struct BoxPokemon boxes[14][IN_BOX_COUNT];
    u8 boxNames[14][BOX_NAME_LENGTH + 1];
    u8 boxWallpapers[14];
    struct Pokemon fusions[MAX_FUSION_STORAGE];
};
TEST("Brisk save migration retains all fourteen legacy boxes and their metadata")
{
    struct PokemonStorage *original = gPokemonStoragePtr;
    struct PokemonStorage *storage = AllocZeroed(sizeof(*storage));
    EXPECT(storage != NULL);
    struct BriskLegacyStorageFixture *legacy = (void *)storage;
    struct Pokemon mon;
    CreateMonWithIVs(&mon, SPECIES_PIKACHU, 25, 0, OTID_STRUCT_PLAYER_ID, 20);
    for (u32 box = 0; box < 14; box++)
    {
        legacy->boxes[box][IN_BOX_COUNT - 1] = mon.box;
        StringCopy(legacy->boxNames[box], COMPOUND_STRING("LEGACY"));
        legacy->boxWallpapers[box] = box;
    }
    legacy->currentBox = 13;
    legacy->fusions[0] = mon;
    gPokemonStoragePtr = storage;
    Test_MigrateLegacyPokemonStorage();
    bool32 ok = storage->currentBox == 13;
    for (u32 box = 0; box < 14; box++)
    {
        ok &= memcmp(&storage->boxes[box][IN_BOX_COUNT - 1], &mon.box, sizeof(mon.box)) == 0;
        ok &= StringCompare(storage->boxNames[box], COMPOUND_STRING("LEGACY")) == 0;
        ok &= storage->boxWallpapers[box] == box;
    }
    for (u32 box = 14; box < TOTAL_BOXES_COUNT; box++)
    {
        for (u32 slot = 0; slot < IN_BOX_COUNT; slot++)
            ok &= GetBoxMonData(&storage->boxes[box][slot], MON_DATA_SPECIES) == SPECIES_NONE;
        ok &= storage->boxNames[box][0] != 0;
    }
    ok &= memcmp(&storage->fusions[0], &mon, sizeof(mon)) == 0;
    gPokemonStoragePtr = original;
    Free(storage);
    EXPECT(ok);
}
TEST("Brisk expanded PC storage is fully covered by its main and extension sectors")
{
    u32 total = (SECTOR_ID_PKMN_STORAGE_END - SECTOR_ID_PKMN_STORAGE_START + 1) * SECTOR_DATA_SIZE;
    for (u32 chunk = 0; chunk < SECTORS_COUNT - SECTOR_ID_HOF_1; chunk++)
    {
        u32 size = Test_GetPokemonStorageExtensionChunkSize(chunk);
        EXPECT_LE(size, SECTOR_DATA_SIZE);
        total += size;
    }
    EXPECT_EQ(total, sizeof(struct PokemonStorage));
    EXPECT_EQ(sizeof(struct SaveSector), SECTOR_SIZE);
    EXPECT_LE(sizeof(struct SaveBlock3), NUM_SECTORS_PER_SLOT * SAVE_BLOCK_3_CHUNK_SIZE);
}
