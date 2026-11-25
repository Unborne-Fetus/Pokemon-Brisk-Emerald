#include "global.h"
#include "battle_setup.h"
#include "brisk_modes.h"
#include "data.h"
#include "difficulty.h"
#include "event_data.h"
#include "malloc.h"
#include "save.h"
#include "battle.h"
#include "pokemon.h"
#include "trainer_util.h"
#include "test/test.h"
#include "constants/battle_ai.h"

extern const struct TrainerMon sXiphosFairyNormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosFairyEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosFairyHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosFairyKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosFairyExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosFightingNormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosFightingEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosFightingHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosFightingKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosFightingExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosSteelNormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosSteelEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosSteelHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosSteelKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosSteelExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosDragonNormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosDragonEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosDragonHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosDragonKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosDragonExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentANormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentAEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentAHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentAKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentAExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentBNormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentBEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentBHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentBKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentBExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentCNormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentCEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentCHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentCKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentCExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentDNormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentDEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentDHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentDKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentDExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentENormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentEEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentEHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentEKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentEExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentFNormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentFEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentFHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentFKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentFExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentGNormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentGEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentGHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentGKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosAgentGExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosScientistANormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosScientistAEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosScientistAHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosScientistAKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosScientistAExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosScientistBNormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosScientistBEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosScientistBHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosScientistBKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosScientistBExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosSeniorANormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosSeniorAEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosSeniorAHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosSeniorAKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosSeniorAExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosSeniorBNormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosSeniorBEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosSeniorBHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosSeniorBKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosSeniorBExpert[PARTY_SIZE];
extern const struct TrainerMon sXiphosJessieNormal[PARTY_SIZE];
extern const struct TrainerMon sXiphosJessieEasy[PARTY_SIZE];
extern const struct TrainerMon sXiphosJessieHard[PARTY_SIZE];
extern const struct TrainerMon sXiphosJessieKaizo[PARTY_SIZE];
extern const struct TrainerMon sXiphosJessieExpert[PARTY_SIZE];

static const struct Trainer sActualTeams[DIFFICULTY_COUNT][TRAINERS_COUNT] = {
#include "../src/data/brisk_mode_teams.h"
#include "../src/data/brisk_xiphos_trainer_info.h"
};
void Test_CopyKantoSaveExtension(u32 sectorId, struct SaveSector *sector, bool32 saving);

TEST("Brisk Kanto every trainer has six distinct authored mode teams at levels 90 through 100")
{
    const u32 modes[] = {DIFFICULTY_EASY, DIFFICULTY_NORMAL, DIFFICULTY_HARD, DIFFICULTY_KAIZO};
    for (u32 tid = BRISK_KANTO_TRAINER_BASE+1; tid < TRAINERS_COUNT; tid++)
        for (u32 m = 0; m < ARRAY_COUNT(modes); m++)
        {
            const struct Trainer *t = &sActualTeams[modes[m]][tid];
            EXPECT(t->authoredParty);
            EXPECT_EQ((u32)t->partySize, PARTY_SIZE);
            EXPECT(t->party != NULL);
            EXPECT(gTrainerPicInfo[t->trainerPic].frontPic != NULL);
            EXPECT(gTrainerPicInfo[t->trainerPic].frontPic->imageData != NULL);
            EXPECT(gTrainerPicInfo[t->trainerPic].frontPic->paletteData != NULL);
            for (u32 p = 0; p < PARTY_SIZE; p++)
            {
                const struct TrainerMon *mon = &t->party[p];
                EXPECT(mon->lvl >= 90 && mon->lvl <= 100);
                EXPECT(IsSpeciesEnabled(mon->species));
                bool32 nativeAbility = FALSE;
                for (u32 a = 0; a < NUM_ABILITY_SLOTS; a++)
                    if (mon->ability == gSpeciesInfo[mon->species].abilities[a]) nativeAbility = TRUE;
                if (!nativeAbility) Test_MgbaPrintf("Trainer %d mode %d species %d ability %d",tid,m,mon->species,mon->ability);
                EXPECT(nativeAbility);
                for (u32 q = 0; q < MAX_MON_MOVES; q++) EXPECT(mon->moves[q] != MOVE_NONE);
            }
            for (u32 previous = 0; previous < m; previous++)
            {
                const struct Trainer *other = &sActualTeams[modes[previous]][tid];
                bool32 different = FALSE;
                for (u32 p = 0; p < PARTY_SIZE; p++)
                    if (t->party[p].species != other->party[p].species) different = TRUE;
                EXPECT(different);
            }
        }
}

TEST("Brisk Kanto defeated-trainer flags are isolated from every Hoenn story and badge flag")
{
    u8 flags[NUM_FLAG_BYTES];
    memcpy(flags,gSaveBlock1Ptr->flags,sizeof(flags));
    u32 ids[] = {BRISK_KANTO_TRAINER_BASE+1, TRAINER_BRISK_KANTO_LEADER_BLAINE,
        TRAINER_BRISK_KANTO_ELITE_FOUR_LANCE, TRAINERS_COUNT-1};
    for (u32 i = 0; i < ARRAY_COUNT(ids); i++)
    {
        u16 flag = GetTrainerDefeatFlag(ids[i]);
        EXPECT(flag >= FLAG_BRISK_KANTO_TRAINER_START && flag < FLAG_BRISK_KANTO_TRAINER_END);
        ClearTrainerFlag(ids[i]);
        EXPECT(!HasTrainerBeenFought(ids[i]));
        SetTrainerFlag(ids[i]);
        EXPECT(HasTrainerBeenFought(ids[i]));
        EXPECT_EQ(memcmp(flags,gSaveBlock1Ptr->flags,sizeof(flags)),0);
        ClearTrainerFlag(ids[i]);
    }
    EXPECT_EQ(GetTrainerDefeatFlag(TRAINER_WALLACE),TRAINER_FLAGS_START+TRAINER_WALLACE);
    EXPECT_EQ(SYSTEM_FLAGS,0x861);
}

TEST("Brisk Kanto defeat records round-trip through save extension sectors and old saves default clear")
{
    EXPECT_EQ(sizeof(struct SaveBlock1),15572);
    EXPECT_EQ(sizeof(struct SaveBlock2),3884);
    EXPECT_EQ(offsetof(struct SaveBlock3,kantoTrainerFlags),120);
    struct SaveBlock3 original = *gSaveBlock3Ptr;
    const u32 sectorCount = (sizeof(struct SaveBlock3)+SAVE_BLOCK_3_CHUNK_SIZE-1)/SAVE_BLOCK_3_CHUNK_SIZE;
    struct SaveSector *sectors = AllocZeroed(sizeof(struct SaveSector)*sectorCount);
    EXPECT(sectors != NULL);
    memset(gSaveBlock3Ptr->kantoTrainerFlags,0xA5,sizeof(gSaveBlock3Ptr->kantoTrainerFlags));
    for (u32 i = 0; i < sectorCount; i++) Test_CopyKantoSaveExtension(i,&sectors[i],TRUE);
    memset(gSaveBlock3Ptr->kantoTrainerFlags,0,sizeof(gSaveBlock3Ptr->kantoTrainerFlags));
    for (u32 i = 0; i < sectorCount; i++) Test_CopyKantoSaveExtension(i,&sectors[i],FALSE);
    for (u32 i = 0; i < sizeof(gSaveBlock3Ptr->kantoTrainerFlags); i++) EXPECT_EQ(gSaveBlock3Ptr->kantoTrainerFlags[i],0xA5);
    EXPECT_EQ(memcmp(gSaveBlock3Ptr,&original,120),0);
    // Previous builds zeroed all unused extension-sector bytes on write.
    for (u32 i = 0; i < sectorCount; i++) memset(sectors[i].saveBlock3Chunk,0,SAVE_BLOCK_3_CHUNK_SIZE);
    for (u32 i = 0; i < 120; i++) sectors[i/SAVE_BLOCK_3_CHUNK_SIZE].saveBlock3Chunk[i%SAVE_BLOCK_3_CHUNK_SIZE] = ((u8 *)&original)[i];
    for (u32 i = 0; i < sectorCount; i++) Test_CopyKantoSaveExtension(i,&sectors[i],FALSE);
    for (u32 i = 0; i < sizeof(gSaveBlock3Ptr->kantoTrainerFlags); i++) EXPECT_EQ(gSaveBlock3Ptr->kantoTrainerFlags[i],0);
    *gSaveBlock3Ptr = original;
    Free(sectors);
}

TEST("Brisk Kanto authored trainer generation keeps postgame levels moves items and abilities in every mode")
{
    const u16 ids[] = {TRAINER_BRISK_KANTO_YOUNGSTER_BEN,TRAINER_BRISK_KANTO_LEADER_MISTY,
        TRAINER_BRISK_KANTO_LEADER_BLAINE,TRAINER_BRISK_KANTO_ELITE_FOUR_LANCE};
    u16 mode = VarGet(VAR_BRISK_DIFFICULTY), random = VarGet(VAR_BRISK_RANDOMIZER);
    u32 flags = gBattleTypeFlags;
    VarSet(VAR_BRISK_RANDOMIZER,0);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER;
    struct Pokemon party[PARTY_SIZE];
    for (u32 m = 0; m <= BRISK_MODE_KAIZO; m++)
    {
        VarSet(VAR_BRISK_DIFFICULTY,m);
        for (u32 i = 0; i < ARRAY_COUNT(ids); i++)
        {
            const struct Trainer *t = &sActualTeams[GetBriskDifficultyLevel()][ids[i]];
            CreateNPCTrainerPartyFromTrainer(party,t);
            for (u32 p = 0; p < PARTY_SIZE; p++)
            {
                EXPECT_EQ(GetMonData(&party[p],MON_DATA_SPECIES),t->party[p].species);
                EXPECT_EQ(GetMonData(&party[p],MON_DATA_LEVEL),t->party[p].lvl);
                EXPECT_EQ(GetMonData(&party[p],MON_DATA_HELD_ITEM),t->party[p].heldItem);
                EXPECT_EQ(GetMonAbility(&party[p]),t->party[p].ability);
                for (u32 q = 0; q < MAX_MON_MOVES; q++) EXPECT_EQ(GetMonData(&party[p],MON_DATA_MOVE1+q),t->party[p].moves[q]);
            }
        }
    }
    VarSet(VAR_BRISK_DIFFICULTY,mode);
    VarSet(VAR_BRISK_RANDOMIZER,random);
    gBattleTypeFlags = flags;
}
