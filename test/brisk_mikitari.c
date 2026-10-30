#include "global.h"
#include "event_data.h"
#include "malloc.h"
#include "pokedex.h"
#include "pokemon.h"
#include "save.h"
#include "test/test.h"
#include <stddef.h>

struct BriskLegacySaveBlock3
{
#if OW_USE_FAKE_RTC
    struct SiiRtcInfo fakeRTC;
#endif
#if FNPC_ENABLE_NPC_FOLLOWERS
    struct NPCFollower NPCfollower;
#endif
#if OW_SHOW_ITEM_DESCRIPTIONS == OW_ITEM_DESCRIPTIONS_FIRST_TIME
    u8 itemFlags[ITEM_FLAGS_COUNT];
#endif
#if USE_DEXNAV_SEARCH_LEVELS == TRUE
    u8 dexNavSearchLevels[1695];
#endif
    u8 dexNavChain;
#if APRICORN_TREE_COUNT > 0
    u8 apricornTrees[NUM_APRICORN_TREE_BYTES];
#endif
    u8 kantoTrainerFlags[78] ALIGNED(4); // 624 independent Kanto trainer defeat bits
};

STATIC_ASSERT(offsetof(struct SaveBlock3, kantoTrainerFlags) == offsetof(struct BriskLegacySaveBlock3, kantoTrainerFlags), MikitariPreservesKantoOffset);
STATIC_ASSERT(offsetof(struct SaveBlock3, dexNavChain) == offsetof(struct BriskLegacySaveBlock3, dexNavChain), MikitariPreservesChainOffset);

static const enum Species sMikitariSpecies[] = {
    SPECIES_BASCULIN_MIKITARI,
    SPECIES_BASSTILE,
    SPECIES_BASSTILE_1,
    SPECIES_BASSTILE_2,
    SPECIES_BEELITANT,
    SPECIES_BUNEARY_MIKITARI,
    SPECIES_CABAT,
    SPECIES_CASTFORM_SAND,
    SPECIES_CASTFORM_CLOUD,
    SPECIES_CASTFORM_GALE,
    SPECIES_CHARGOO,
    SPECIES_CHUBBOINK,
    SPECIES_COMMANT,
    SPECIES_CONFLEVOUR,
    SPECIES_CONTAMULLET,
    SPECIES_CRISHY,
    SPECIES_CROOBER,
    SPECIES_CRUNCHILL,
    SPECIES_DAMPYBARA,
    SPECIES_DOPERAMI,
    SPECIES_DOREWEE,
    SPECIES_DUNTESERT,
    SPECIES_DUNTESERT_1,
    SPECIES_ERUPTILE,
    SPECIES_FALINKS_MIKITARI,
    SPECIES_FLAMBAFEE,
    SPECIES_FLAMPY,
    SPECIES_FLAREPENT,
    SPECIES_FLORAMONA,
    SPECIES_FLORAMONA_1,
    SPECIES_FLORAMONA_2,
    SPECIES_FLORAMONA_3,
    SPECIES_FLORAMONA_4,
    SPECIES_FLORAMONA_5,
    SPECIES_FLORAMONA_6,
    SPECIES_FURNADON,
    SPECIES_FURNADON_1,
    SPECIES_GASARNET,
    SPECIES_GEOCKO,
    SPECIES_GEOLIDON,
    SPECIES_GERMEIT,
    SPECIES_GLANCHOG,
    SPECIES_GLOVET,
    SPECIES_GLYPTOBLOCK,
    SPECIES_GLYPTOBLOCK_1,
    SPECIES_GOOBOLT,
    SPECIES_GRANEBEE,
    SPECIES_GREXCLUB,
    SPECIES_HAUNTRAY,
    SPECIES_HOMURANT,
    SPECIES_HOPIDEY,
    SPECIES_INHALIS,
    SPECIES_KEERIN,
    SPECIES_KOCOMB,
    SPECIES_KOCOMB_1,
    SPECIES_LAMBY,
    SPECIES_LEDIASTRA,
    SPECIES_LILLEO,
    SPECIES_LOPUNNY_MIKITARI,
    SPECIES_MANTOON,
    SPECIES_MORTARINE,
    SPECIES_MOURIN,
    SPECIES_MUCHOG,
    SPECIES_MUNCHILL,
    SPECIES_MURASALEM,
    SPECIES_NOCTOWL_MIKITARI,
    SPECIES_OCTOKINEPUS,
    SPECIES_OCTOLECT,
    SPECIES_ORICORIO_NOCTURNE,
    SPECIES_ORICORIO_BALLET,
    SPECIES_ORICORIO_MARTIAL,
    SPECIES_ORICORIO_FOLK,
    SPECIES_PACKBATS,
    SPECIES_PACKBATS_1,
    SPECIES_PECLAY,
    SPECIES_PETALNIA,
    SPECIES_PETALNIA_1,
    SPECIES_PETALNIA_2,
    SPECIES_PETALNIA_3,
    SPECIES_PETALNIA_4,
    SPECIES_PETALNIA_5,
    SPECIES_PETALNIA_6,
    SPECIES_PETIDAW,
    SPECIES_PHANTABYS,
    SPECIES_PLASMATINA,
    SPECIES_PLATERIGUS,
    SPECIES_PORYGONA,
    SPECIES_PRANKMARE,
    SPECIES_RAKNALEAP,
    SPECIES_RAVENDLER,
    SPECIES_RHINPLINK,
    SPECIES_RHINTAIN,
    SPECIES_SEARPENT,
    SPECIES_SHAGGOT,
    SPECIES_SKINDLING,
    SPECIES_SKINGNITE,
    SPECIES_SLIDUNE,
    SPECIES_SLIDUNE_1,
    SPECIES_SNEEPY,
    SPECIES_SPHERTLE,
    SPECIES_SPRITZEE_MIKITARI,
    SPECIES_SPROUTSY,
    SPECIES_STALAD,
    SPECIES_STALASS,
    SPECIES_STORMANTA,
    SPECIES_SUFFERUB,
    SPECIES_TEKTALON,
    SPECIES_TEKTALON_1,
    SPECIES_TIMPOT,
    SPECIES_TRAUMATISSE,
    SPECIES_TROUTLE,
    SPECIES_TRUNKORN,
    SPECIES_WROOMTOISE,
    SPECIES_YAMASK_MIKITARI,
};

TEST("Brisk Mikitari every species has usable graphics abilities and level-90 moves")
{
    for (u32 i = 0; i < ARRAY_COUNT(sMikitariSpecies); i++)
    {
        enum Species species = sMikitariSpecies[i];
        const struct SpeciesInfo *info = &gSpeciesInfo[species];
        EXPECT(IsSpeciesEnabled(species));
        EXPECT(info->frontPic != NULL);
        EXPECT(info->backPic != NULL);
        EXPECT(info->palette != NULL);
        EXPECT(info->shinyPalette != NULL);
        EXPECT(info->iconSprite != NULL);
        EXPECT(info->abilities[0] != ABILITY_NONE);
        EXPECT(info->abilities[2] != ABILITY_NONE);
        EXPECT(info->natDexNum != NATIONAL_DEX_NONE);
        struct Pokemon mon;
        CreateMon(&mon, species, 90, 0, OTID_STRUCT_PRESET(0));
        GiveMonInitialMoveset(&mon);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_SPECIES), species);
        for (u32 m = 0; m < MAX_MON_MOVES; m++)
            EXPECT(GetMonData(&mon, MON_DATA_MOVE1 + m) != MOVE_NONE);
    }
}

TEST("Brisk Mikitari old-save extension migration preserves Kanto flags chain and search levels")
{
    memset(gSaveBlock3Ptr->kantoTrainerFlags, 0xA5, sizeof(gSaveBlock3Ptr->kantoTrainerFlags));
    gSaveBlock3Ptr->dexNavChain = 91;
#if USE_DEXNAV_SEARCH_LEVELS == TRUE
    gSaveBlock3Ptr->dexNavSearchLevels[SPECIES_PIKACHU] = 77;
    gSaveBlock3Ptr->dexNavSearchLevels[SPECIES_BOARHORN] = 88;
#endif
    memset(gSaveBlock3Ptr->mikitariDexSeen, 0xFF, sizeof(gSaveBlock3Ptr->mikitariDexSeen));
    memset(gSaveBlock3Ptr->mikitariDexCaught, 0xFF, sizeof(gSaveBlock3Ptr->mikitariDexCaught));
    memset(gSaveBlock3Ptr->extraDexNavSearchLevels, 0xFF, sizeof(gSaveBlock3Ptr->extraDexNavSearchLevels));
    gSaveBlock3Ptr->speciesExtensionMagic = 0xFFFFFFFF;
    InitBriskSpeciesExtension();
    EXPECT_EQ(gSaveBlock3Ptr->dexNavChain, 91);
#if USE_DEXNAV_SEARCH_LEVELS == TRUE
    EXPECT_EQ(*GetBriskDexNavSearchLevel(SPECIES_PIKACHU), 77);
    EXPECT_EQ(*GetBriskDexNavSearchLevel(SPECIES_BOARHORN), 88);
#endif
    for (u32 i = 0; i < sizeof(gSaveBlock3Ptr->kantoTrainerFlags); i++)
        EXPECT_EQ(gSaveBlock3Ptr->kantoTrainerFlags[i], 0xA5);
    for (u32 i = 0; i < sizeof(gSaveBlock3Ptr->mikitariDexSeen); i++)
    {
        EXPECT_EQ(gSaveBlock3Ptr->mikitariDexSeen[i], 0);
        EXPECT_EQ(gSaveBlock3Ptr->mikitariDexCaught[i], 0);
    }
    EXPECT_EQ(*GetBriskDexNavSearchLevel(SPECIES_BASCULIN_MIKITARI), 0);
    EXPECT_EQ(*GetBriskDexNavSearchLevel(SPECIES_YAMASK_MIKITARI), 0);
    *GetBriskDexNavSearchLevel(SPECIES_BASCULIN_MIKITARI) = 123;
    InitBriskSpeciesExtension();
    EXPECT_EQ(*GetBriskDexNavSearchLevel(SPECIES_BASCULIN_MIKITARI), 123);
    EXPECT(GetBriskDexNavSearchLevel(NUM_SPECIES) == NULL);
}

TEST("Brisk Mikitari all new Dex flags are independent of each other and older flags")
{
    gSaveBlock3Ptr->speciesExtensionMagic = 0;
    InitBriskSpeciesExtension();
    VarSet(VAR_BRISK_SACRED_DEX_SEEN, 0xA55A);
    VarSet(VAR_BRISK_SACRED_DEX_CAUGHT, 0x5AA5);
    for (u32 dex = NATIONAL_DEX_BOARHORN + 1; dex <= NATIONAL_DEX_COUNT; dex++)
    {
        EXPECT_EQ(GetSetPokedexFlag(dex, FLAG_GET_SEEN), 0);
        EXPECT_EQ(GetSetPokedexFlag(dex, FLAG_GET_CAUGHT), 0);
        GetSetPokedexFlag(dex, FLAG_SET_SEEN);
        EXPECT_EQ(GetSetPokedexFlag(dex, FLAG_GET_SEEN), 1);
        EXPECT_EQ(GetSetPokedexFlag(dex, FLAG_GET_CAUGHT), 0);
        GetSetPokedexFlag(dex, FLAG_SET_CAUGHT);
        EXPECT_EQ(GetSetPokedexFlag(dex, FLAG_GET_CAUGHT), 1);
    }
    for (u32 dex = NATIONAL_DEX_BOARHORN + 1; dex <= NATIONAL_DEX_COUNT; dex++)
    {
        EXPECT_EQ(GetSetPokedexFlag(dex, FLAG_GET_SEEN), 1);
        EXPECT_EQ(GetSetPokedexFlag(dex, FLAG_GET_CAUGHT), 1);
    }
    EXPECT_EQ(VarGet(VAR_BRISK_SACRED_DEX_SEEN), 0xA55A);
    EXPECT_EQ(VarGet(VAR_BRISK_SACRED_DEX_CAUGHT), 0x5AA5);
}

void Test_CopyKantoSaveExtension(u32 sectorId, struct SaveSector *sector, bool32 saving);
TEST("Brisk Mikitari expanded save data survives sector serialization")
{
    struct SaveSector *sector = AllocZeroed(sizeof(struct SaveSector));
    EXPECT(sector != NULL);
    InitBriskSpeciesExtension();
    GetSetPokedexFlag(NATIONAL_DEX_WROOMTOISE, FLAG_SET_CAUGHT);
    *GetBriskDexNavSearchLevel(SPECIES_WROOMTOISE) = 211;
    // Each test copy addresses one reserved SaveBlock3 chunk.
    for (u32 id = 0; id < NUM_SECTORS_PER_SLOT; id++)
    {
        memset(sector, 0, sizeof(*sector));
        Test_CopyKantoSaveExtension(id, sector, TRUE);
        u32 begin = id * SAVE_BLOCK_3_CHUNK_SIZE;
        u32 size = min(SAVE_BLOCK_3_CHUNK_SIZE, sizeof(struct SaveBlock3) > begin ? sizeof(struct SaveBlock3) - begin : 0);
        memset((u8 *)gSaveBlock3Ptr + min(begin, sizeof(struct SaveBlock3)), 0, size);
        Test_CopyKantoSaveExtension(id, sector, FALSE);
    }
    EXPECT_EQ(GetSetPokedexFlag(NATIONAL_DEX_WROOMTOISE, FLAG_GET_CAUGHT), 1);
    EXPECT_EQ(*GetBriskDexNavSearchLevel(SPECIES_WROOMTOISE), 211);
    Free(sector);
}
