#include "global.h"
#include "battle.h"
#include "battle_setup.h"
#include "battle_transition.h"
#include "brisk_difficulty.h"
#include "brisk_modes.h"
#include "caps.h"
#include "data.h"
#include "event_data.h"
#include "item.h"
#include "pokemon.h"
#include "trainer_pools.h"
#include "trainer_util.h"
#include "test/test.h"
#include "constants/battle_ai.h"
#include "constants/battle_partner.h"
#include "constants/opponents.h"
#include "constants/trainers.h"

void Test_BriskTrainerSource(const struct Trainer *trainer, u32 id);
static const struct Trainer sProfileSources[DIFFICULTY_COUNT][TRAINERS_COUNT] = {
#include "../src/data/trainers.h"
};

TEST("Brisk mode targets generate smaller Easy parties and distinct Hard Extreme reserves from real trainer sources")
{
    const u16 ids[] = {TRAINER_ALLEN, TRAINER_RICK, TRAINER_ROXANNE_1, TRAINER_MAY_ROUTE_119_TORCHIC,
        TRAINER_TABITHA_MT_CHIMNEY, TRAINER_MAXIE_MOSSDEEP, TRAINER_SIDNEY, TRAINER_DALTON_1};
    u32 id = TRAINER_ALLEN, mode = BRISK_MODE_NORMAL;
    for (u32 i = 0; i < ARRAY_COUNT(ids); i++)
        for (u32 m = 0; m < 4; m++)
            PARAMETRIZE_LABEL("trainer %d mode %d", ids[i], m) {id = ids[i]; mode = m;}
    u16 savedMode = VarGet(VAR_BRISK_DIFFICULTY), savedRandom = VarGet(VAR_BRISK_RANDOMIZER);
    u32 flags = gBattleTypeFlags;
    VarSet(VAR_BRISK_DIFFICULTY, mode); VarSet(VAR_BRISK_RANDOMIZER, 0); gBattleTypeFlags = 0;
    const struct Trainer *source = &sProfileSources[DIFFICULTY_NORMAL][id];
    Test_BriskTrainerSource(source, id);
    struct TrainerGenerator generator = {0};
    MakeTrainerGenerator(&generator, source);
    u32 expected = BriskDifficultyPartySize(source->partySize, generator.highestLevel,
        generator.storyLoadout, generator.leagueLevels, generator.fairLoadout || generator.storyLoadout);
    struct Pokemon party[PARTY_SIZE];
    CreateNPCTrainerPartyFromTrainer(party, source);
    u32 count = 0;
    for (u32 slot = 0; slot < PARTY_SIZE; slot++)
    {
        enum Species species = GetMonData(&party[slot], MON_DATA_SPECIES);
        if (species == SPECIES_NONE) continue;
        count++;
        EXPECT(GetMonData(&party[slot], MON_DATA_HP) > 0);
        if (mode == BRISK_MODE_EASY && !BriskTrainerDifficultyExempt(id))
        {
            EXPECT_EQ(GetMonData(&party[slot], MON_DATA_IVS), 0);
            EXPECT_EQ(BriskEasyTrainerSpecies(species, GetMonData(&party[slot],MON_DATA_LEVEL)),species);
        }
        if (mode == BRISK_MODE_NORMAL && !generator.leagueLevels && !BriskTrainerDifficultyExempt(id))
            for (u32 stat = 0; stat < NUM_STATS; stat++) EXPECT_GE(GetMonData(&party[slot],MON_DATA_HP_IV+stat),16);
        if (mode >= BRISK_MODE_HARD && !BriskTrainerDifficultyExempt(id))
        {
            EXPECT(GetMonData(&party[slot],MON_DATA_HELD_ITEM) != ITEM_NONE);
            EXPECT_EQ(GetMonData(&party[slot],MON_DATA_IVS), 0x3FFFFFFF);
            u32 evs = 0;
            for (u32 stat = 0; stat < NUM_STATS; stat++)
            {
                u32 ev = GetMonData(&party[slot],MON_DATA_HP_EV+stat);
                EXPECT_LE(ev,252); evs += ev;
            }
            EXPECT_LE(evs,510);
            if (!generator.leagueLevels) EXPECT_GE(evs,mode == BRISK_MODE_EXTREME ? 508 : 256);
        }
    }
    EXPECT_EQ(count,expected);
    EXPECT_LE(count,6);
    if (id == TRAINER_ALLEN)
        EXPECT_EQ(count, mode == BRISK_MODE_HARD ? 3 : mode == BRISK_MODE_EXTREME ? 4 : 2);
    if (id == TRAINER_MAY_ROUTE_119_TORCHIC)
        EXPECT_EQ(count, mode == BRISK_MODE_EASY ? 4 : mode == BRISK_MODE_NORMAL ? 5 : 6);
    if (id == TRAINER_SIDNEY) EXPECT_EQ(count,6);
    // The last authored slot remains last when reserves are inserted.
    if (generator.leagueLevels)
        EXPECT_EQ(GetMonData(&party[count-1],MON_DATA_LEVEL), mode == BRISK_MODE_EASY ? 70 : 100);
    Test_BriskTrainerSource(NULL,0);
    gBattleTypeFlags = flags;
    VarSet(VAR_BRISK_DIFFICULTY,savedMode); VarSet(VAR_BRISK_RANDOMIZER,savedRandom);
}

TEST("Brisk Easy early gym species devolve to level-appropriate stages and tutorials retain their party size")
{
    EXPECT_EQ(BriskEasyTrainerSpecies(SPECIES_GOLEM,12),SPECIES_GEODUDE);
    EXPECT_EQ(BriskEasyTrainerSpecies(SPECIES_SWAMPERT,33),SPECIES_MARSHTOMP);
    EXPECT_EQ(BriskEasyTrainerSpecies(SPECIES_SWAMPERT,36),SPECIES_SWAMPERT);
    u16 mode = VarGet(VAR_BRISK_DIFFICULTY); u32 flags = gBattleTypeFlags;
    for (u32 m = 0; m < 4; m++)
    {
        VarSet(VAR_BRISK_DIFFICULTY,m); gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_FIRST_BATTLE;
        EXPECT_EQ(BriskDifficultyPartySize(1,9,TRUE,FALSE,TRUE),1);
    }
    gBattleTypeFlags = flags; VarSet(VAR_BRISK_DIFFICULTY,mode);
}

TEST("Brisk Hard enforces EXP caps by default and Extreme always enforces them")
{
    u16 mode = VarGet(VAR_BRISK_DIFFICULTY), setting = VarGet(VAR_BRISK_NO_LEVEL_CAP);
    for (u32 m = 0; m < 4; m++)
    for (u32 off = 0; off < 2; off++)
    {
        VarSet(VAR_BRISK_DIFFICULTY,m); VarSet(VAR_BRISK_NO_LEVEL_CAP,off);
        bool32 enforced = m == BRISK_MODE_EXTREME || (m == BRISK_MODE_HARD && !off);
        EXPECT_EQ(BriskUsesExpCap(),enforced);
        if (enforced)
        {
            EXPECT_EQ(GetCurrentLevelCap(),GetProgressionLevelCap());
            EXPECT_EQ(GetSoftLevelCapExpValue(GetCurrentLevelCap(),100),0);
        }
    }
    VarSet(VAR_BRISK_DIFFICULTY,mode); VarSet(VAR_BRISK_NO_LEVEL_CAP,setting);
}

TEST("Brisk dedication and Easter egg AI keep their former behavior instead of the new predictive profiles")
{
    u16 mode = VarGet(VAR_BRISK_DIFFICULTY);
    VarSet(VAR_BRISK_DIFFICULTY,BRISK_MODE_NORMAL);
    EXPECT(GetBriskTrainerAiFlagsForTrainer(AI_FLAG_OMNISCIENT,TRAINER_DALTON_1) & AI_FLAG_OMNISCIENT);
    EXPECT(!(GetBriskTrainerAiFlagsForTrainer(AI_FLAG_OMNISCIENT,TRAINER_ALLEN) & AI_FLAG_OMNISCIENT));
    VarSet(VAR_BRISK_DIFFICULTY,BRISK_MODE_EXTREME);
    EXPECT(GetBriskTrainerAiFlagsForTrainer(0,TRAINER_ALLEN) & AI_FLAG_PREDICT_MOVE);
    EXPECT(!(GetBriskTrainerAiFlagsForTrainer(0,TRAINER_DALTON_1) & AI_FLAG_PREDICT_MOVE));
    VarSet(VAR_BRISK_DIFFICULTY,mode);
}

TEST("Brisk Normal is distinct from Easy and Hard caps also stop candy overlevelling")
{
    u16 mode = VarGet(VAR_BRISK_DIFFICULTY), setting = VarGet(VAR_BRISK_NO_LEVEL_CAP);
    u32 flags = gBattleTypeFlags;
    gBattleTypeFlags = 0;
    VarSet(VAR_BRISK_DIFFICULTY,BRISK_MODE_NORMAL);
    EXPECT_EQ(BriskDifficultyPartySize(1,45,FALSE,FALSE,TRUE),4);
    EXPECT_EQ(GetBriskBattleStyle(OPTIONS_BATTLE_STYLE_SHIFT),OPTIONS_BATTLE_STYLE_SET);
    VarSet(VAR_BRISK_DIFFICULTY,BRISK_MODE_HARD); VarSet(VAR_BRISK_NO_LEVEL_CAP,0);
    u32 cap = GetCurrentLevelCap();
    struct Pokemon mon;
    CreateMonWithIVs(&mon,SPECIES_PIKACHU,cap-1,0,OTID_STRUCT_PRESET(0),0);
    EXPECT(!ExecuteTableBasedItemEffect(&mon,ITEM_EXP_CANDY_XL,0,0,1,10));
    EXPECT_EQ(GetMonData(&mon,MON_DATA_LEVEL),cap);
    EXPECT(ExecuteTableBasedItemEffect(&mon,ITEM_RARE_CANDY,0,0,1,1));
    EXPECT_EQ(GetMonData(&mon,MON_DATA_LEVEL),cap);
    gBattleTypeFlags = flags; VarSet(VAR_BRISK_DIFFICULTY,mode); VarSet(VAR_BRISK_NO_LEVEL_CAP,setting);
}

static const struct Trainer sPartnerSources[DIFFICULTY_COUNT][PARTNER_COUNT] = {
#include "../src/data/battle_partners.h"
};
void Test_GenerateNamedPartnerParty(const struct Trainer *partner, u16 trainerId, u32 limit);

TEST("Brisk Steven partner matches the Mossdeep curve with three allies on Easy Normal and six on Hard Extreme")
{
    u16 oldMode = VarGet(VAR_BRISK_DIFFICULTY);
    struct Pokemon saved[PARTY_SIZE];
    memcpy(saved,gParties[B_TRAINER_PARTNER],sizeof(saved));
    const struct Trainer *source = &sPartnerSources[DIFFICULTY_NORMAL][PARTNER_STEVEN];
    for (u32 mode = 0; mode < 4; mode++)
    {
        VarSet(VAR_BRISK_DIFFICULTY,mode);
        Test_GenerateNamedPartnerParty(source,TRAINER_PARTNER(PARTNER_STEVEN),6);
        EXPECT_EQ(CalculatePartyCount(B_TRAINER_PARTNER),mode >= BRISK_MODE_HARD ? 6 : 3);
        for (u32 slot = 0; slot < (mode >= BRISK_MODE_HARD ? 6 : 3); slot++)
        {
            struct Pokemon *mon = &gParties[B_TRAINER_PARTNER][slot];
            EXPECT_EQ(GetMonData(mon,MON_DATA_LEVEL),ScaleBriskStoryTrainerLevel(source->party[slot].lvl,84));
            EXPECT_EQ(GetMonData(mon,MON_DATA_MOVE1),source->party[slot].moves[0]);
            EXPECT_EQ(GetMonAbility(mon),source->party[slot].ability);
            EXPECT_EQ(GetMonData(mon,MON_DATA_HELD_ITEM),source->party[slot].heldItem);
        }
        Test_GenerateNamedPartnerParty(source,TRAINER_PARTNER(PARTNER_STEVEN),3);
        EXPECT_EQ(CalculatePartyCount(B_TRAINER_PARTNER),3);
    }
    memcpy(gParties[B_TRAINER_PARTNER],saved,sizeof(saved));
    CalculatePartyCount(B_TRAINER_PARTNER);
    VarSet(VAR_BRISK_DIFFICULTY,oldMode);
}
