#include "global.h"
#include "battle.h"
#include "battle_setup.h"
#include "brisk_difficulty.h"
#include "brisk_modes.h"
#include "caps.h"
#include "data.h"
#include "difficulty.h"
#include "event_data.h"
#include "pokemon.h"
#include "test/test.h"
#include "constants/battle_ai.h"
#include "constants/opponents.h"
#include "constants/trainers.h"

void Test_BriskTrainerSource(const struct Trainer *trainer, u32 id);

TEST("Expert is an accessible full-team mode with save-compatible ID")
{
    u16 previousMode = VarGet(VAR_BRISK_DIFFICULTY);
    u32 previousBattleFlags = gBattleTypeFlags;
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_EXPERT);
    gBattleTypeFlags = 0;

    EXPECT_EQ(GetBriskDifficulty(), BRISK_MODE_EXPERT);
    EXPECT_EQ(GetBriskDifficultyLevel(), DIFFICULTY_HARD);
    EXPECT_EQ(GetBriskPriceMultiplier(), 3);
    EXPECT(BriskBattleItemsAllowed());
    EXPECT_EQ(GetBriskBattleStyle(OPTIONS_BATTLE_STYLE_SHIFT), OPTIONS_BATTLE_STYLE_SET);
    EXPECT(!BriskUsesExpCap());
    EXPECT_EQ(ScaleBriskSharedExp(100), 100);

    u64 flags = GetBriskTrainerAiFlags(AI_FLAG_OMNISCIENT | AI_FLAG_PREDICTION);
    EXPECT(flags & AI_FLAG_SMART_MON_CHOICES);
    EXPECT(flags & AI_FLAG_SMART_SWITCHING);
    EXPECT(!(flags & AI_FLAG_OMNISCIENT));
    EXPECT(!(flags & AI_FLAG_PREDICTION));

    EXPECT_EQ(BriskDifficultyPartySize(2, 70, FALSE, FALSE, TRUE), 6);
    EXPECT_EQ(BriskDifficultyPartySize(2, 70, TRUE, FALSE, TRUE), 6);
    EXPECT_EQ(BriskDifficultyPartySize(2, 12, FALSE, FALSE, TRUE), 3);

    VarSet(VAR_BRISK_DIFFICULTY, previousMode);
    gBattleTypeFlags = previousBattleFlags;
}

static const struct TrainerMon sExpertTestMons[] =
{
    {.species = SPECIES_PIKACHU, .lvl = 66, .iv = 0x3FFFFFFF, .gender = TRAINER_MON_RANDOM_GENDER, .nature = NATURE_MODEST, .ball = BALL_POKE},
    {.species = SPECIES_RATICATE, .lvl = 70, .iv = 0x3FFFFFFF, .gender = TRAINER_MON_RANDOM_GENDER, .nature = NATURE_ADAMANT, .ball = BALL_POKE},
};

static const struct Trainer sExpertTestTrainer =
{
    .trainerClass = TRAINER_CLASS_YOUNGSTER,
    .party = sExpertTestMons,
    .partySize = 2,
    .authoredParty = TRUE,
    .trainerName = _("EXPERT"),
};

TEST("Expert preserves each authored party and keeps its original ace last")
{
    u16 previousMode = VarGet(VAR_BRISK_DIFFICULTY);
    u16 previousRandomizer = VarGet(VAR_BRISK_RANDOMIZER);
    u32 previousBattleFlags = gBattleTypeFlags;
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_EXPERT);
    VarSet(VAR_BRISK_RANDOMIZER, 0);
    gBattleTypeFlags = 0;
    // Neither Hoenn nor Kanto may receive random reserves after loading its
    // independent Expert-mode authored party.
    const u16 testIds[] = {TRAINER_SAWYER_1, BRISK_KANTO_TRAINER_BASE + 1};
    struct Pokemon party[PARTY_SIZE];
    for (u32 region = 0; region < ARRAY_COUNT(testIds); region++)
    {
        Test_BriskTrainerSource(&sExpertTestTrainer, testIds[region]);
        CreateNPCTrainerPartyFromTrainer(party, &sExpertTestTrainer);
        EXPECT_EQ(GetMonData(&party[0], MON_DATA_SPECIES), SPECIES_PIKACHU);
        EXPECT_EQ(GetMonData(&party[1], MON_DATA_SPECIES), SPECIES_RATICATE);
        for (u32 i = 2; i < PARTY_SIZE; i++)
            EXPECT_EQ(GetMonData(&party[i], MON_DATA_SPECIES), SPECIES_NONE);
        EXPECT_EQ(GetMonData(&party[1], MON_DATA_SPECIES), SPECIES_RATICATE);
        EXPECT_EQ(GetMonData(&party[1], MON_DATA_LEVEL), 67);
        Test_BriskTrainerSource(NULL, 0);
    }

    gBattleTypeFlags = previousBattleFlags;
    VarSet(VAR_BRISK_RANDOMIZER, previousRandomizer);
    VarSet(VAR_BRISK_DIFFICULTY, previousMode);
}
