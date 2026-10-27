#include "global.h"
#include "battle.h"
#include "brisk_modes.h"
#include "caps.h"
#include "data.h"
#include "event_data.h"
#include "pokemon.h"
#include "trainer_util.h"
#include "test/test.h"
#include "constants/opponents.h"
#include "constants/trainers.h"

TEST("Brisk League policy applies only to the four Elite Four teams and Wallace")
{
    const u16 league[] = {TRAINER_SIDNEY, TRAINER_PHOEBE, TRAINER_GLACIA, TRAINER_DRAKE, TRAINER_WALLACE};
    for (u32 i = 0; i < ARRAY_COUNT(league); i++)
    {
        struct TrainerGenerator generator = {0};
        MakeTrainerGenerator(&generator, &gTrainers[DIFFICULTY_NORMAL][league[i]]);
        EXPECT(generator.leagueLevels);
    }
    const u16 others[] = {TRAINER_MAY_ROUTE_119_TORCHIC, TRAINER_ROXANNE_1,
        TRAINER_CYNTHIA, TRAINER_UNBORNE, TRAINER_JOEY, TRAINER_ALLEN};
    for (u32 i = 0; i < ARRAY_COUNT(others); i++)
    {
        struct TrainerGenerator generator = {0};
        MakeTrainerGenerator(&generator, &gTrainers[DIFFICULTY_NORMAL][others[i]]);
        EXPECT(!generator.leagueLevels);
    }
}

TEST("Brisk all six League slots generate at 70 on Easy and 100 in every other mode even under an early badge cap")
{
    u16 oldMode = VarGet(VAR_BRISK_DIFFICULTY);
    u16 oldRandom = VarGet(VAR_BRISK_RANDOMIZER);
    u16 oldCap = VarGet(VAR_BRISK_NO_LEVEL_CAP);
    bool32 badges[8];
    for (u32 i = 0; i < 8; i++)
    {
        badges[i] = FlagGet(FLAG_BADGE01_GET + i);
        FlagClear(FLAG_BADGE01_GET + i);
    }
    VarSet(VAR_BRISK_RANDOMIZER, 0);
    VarSet(VAR_BRISK_NO_LEVEL_CAP, 0);
    static const struct Trainer fixture = {.trainerClass = TRAINER_CLASS_CHAMPION, .trainerName = _("WALLACE")};
    struct TrainerMon entry = {.species = SPECIES_METAGROSS, .lvl = 100,
        .gender = TRAINER_MON_RANDOM_GENDER, .nature = NATURE_ADAMANT, .ball = BALL_POKE,
        .moves = {MOVE_METEOR_MASH, MOVE_ZEN_HEADBUTT}};
    for (u32 mode = 0; mode <= BRISK_MODE_EXTREME; mode++)
    {
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        if (mode == BRISK_MODE_HARD || mode == BRISK_MODE_EXTREME)
            EXPECT_EQ(GetProgressionLevelCap(), 25);
        struct TrainerGenerator generator = {0};
        MakeTrainerGenerator(&generator, &fixture);
        generator.storyLoadout = TRUE;
        generator.leagueLevels = TRUE;
        generator.highestLevel = 100;
        for (u32 slot = 0; slot < PARTY_SIZE; slot++)
        {
            struct Pokemon mon;
            GenerateMonFromTrainerMon(&mon, &entry, &generator);
            EXPECT_EQ(GetMonData(&mon, MON_DATA_LEVEL), mode == BRISK_MODE_EASY ? 70 : 100);
            CalculateMonStats(&mon);
            EXPECT_EQ(GetMonData(&mon, MON_DATA_LEVEL), mode == BRISK_MODE_EASY ? 70 : 100);
        }
    }
    for (u32 i = 0; i < 8; i++)
        if (badges[i]) FlagSet(FLAG_BADGE01_GET + i); else FlagClear(FLAG_BADGE01_GET + i);
    VarSet(VAR_BRISK_DIFFICULTY, oldMode);
    VarSet(VAR_BRISK_RANDOMIZER, oldRandom);
    VarSet(VAR_BRISK_NO_LEVEL_CAP, oldCap);
}
