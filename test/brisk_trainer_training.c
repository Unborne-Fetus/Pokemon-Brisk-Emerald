#include "global.h"
#include "battle.h"
#include "brisk_modes.h"
#include "data.h"
#include "event_data.h"
#include "pokemon.h"
#include "trainer_util.h"
#include "test/test.h"
#include "constants/trainers.h"

static const u8 sAuthoredEvs[] = {100, 80, 60, 40, 20, 10}; // Showdown order.
static const struct TrainerMon sTrainingMon = {
    .species = SPECIES_GOLEM, .lvl = 40, .moves = {MOVE_EARTHQUAKE, MOVE_ROCK_SLIDE},
    .gender = TRAINER_MON_RANDOM_GENDER, .nature = NATURE_HARDY, .ball = BALL_POKE,
    .iv = 3 | (7 << 5) | (11 << 10) | (13 << 15) | (17 << 20) | (23 << 25),
    .ev = sAuthoredEvs,
};
static const struct Trainer sTrainingTrainer = {
    .trainerClass = TRAINER_CLASS_LEADER, .party = &sTrainingMon, .partySize = 1,
    .trainerName = _("TEST"),
};

static void GenerateTrainingMon(struct Pokemon *mon, const struct TrainerMon *entry)
{
    struct TrainerGenerator generator = {0};
    MakeTrainerGenerator(&generator, &sTrainingTrainer);
    GenerateMonFromTrainerMon(mon, entry, &generator);
}

TEST("Brisk trainer training zeroes Easy, preserves Normal and perfects Hard IVs")
{
    static const u8 authored[] = {100, 80, 60, 10, 40, 20}; // Pokemon stat order.
    VarSet(VAR_BRISK_RANDOMIZER, 0);
    for (u32 mode = 0; mode < 3; mode++)
    {
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        struct Pokemon mon;
        GenerateTrainingMon(&mon, &sTrainingMon);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_IVS), mode == BRISK_MODE_EASY ? 0 : mode == BRISK_MODE_NORMAL ? sTrainingMon.iv : 0x3FFFFFFF);
        for (u32 stat = 0; stat < NUM_STATS; stat++)
            EXPECT_EQ(GetMonData(&mon, MON_DATA_HP_EV + stat), mode == BRISK_MODE_EASY ? 0 : authored[stat]);
    }
    EXPECT_EQ(sAuthoredEvs[0], 100);
    EXPECT_EQ(sTrainingMon.iv & 31, 3);
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
}

TEST("Brisk Extreme training invests in physical, special and defensive strengths")
{
    VarSet(VAR_BRISK_RANDOMIZER, 0);
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_EXTREME);
    struct Pokemon mon;
    GenerateTrainingMon(&mon, &sTrainingMon);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_ATK_EV), 252);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_DEF_EV), 252);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_HP_EV), 6);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_SPATK_EV), 0);
    struct TrainerMon entry = sTrainingMon;
    entry.species = SPECIES_ALAKAZAM;
    entry.moves[0] = MOVE_PSYCHIC;
    entry.moves[1] = MOVE_RECOVER;
    GenerateTrainingMon(&mon, &entry);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_SPATK_EV), 252);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_SPEED_EV), 252);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_ATK_EV), 0);
    entry.species = SPECIES_BLISSEY;
    entry.moves[0] = MOVE_SOFT_BOILED;
    entry.moves[1] = MOVE_TOXIC;
    GenerateTrainingMon(&mon, &entry);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_HP_EV), 252);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_SPDEF_EV), 252);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_ATK_EV), 0);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_SPATK_EV), 0);
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
}

TEST("Brisk Extreme training preserves slow strategies and keeps every spread legal")
{
    VarSet(VAR_BRISK_RANDOMIZER, 0);
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_EXTREME);
    struct TrainerMon entry = sTrainingMon;
    static const u16 species[] = {SPECIES_SLOWBRO, SPECIES_SHUCKLE, SPECIES_SHEDINJA, SPECIES_CHARIZARD, SPECIES_PIKACHU};
    for (u32 i = 0; i < ARRAY_COUNT(species); i++)
    {
        struct Pokemon mon;
        entry.species = species[i];
        entry.moves[0] = MOVE_TRICK_ROOM;
        entry.moves[1] = MOVE_BODY_PRESS;
        GenerateTrainingMon(&mon, &entry);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_IVS), 0x3FFFFFFF);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_SPEED_EV), 0);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_ATK_EV), 0);
        u32 total = 0;
        for (u32 stat = 0; stat < NUM_STATS; stat++)
        {
            u32 ev = GetMonData(&mon, MON_DATA_HP_EV + stat);
            EXPECT_LE(ev, 252);
            total += ev;
        }
        EXPECT_LE(total, 510);
        if (entry.species == SPECIES_SHEDINJA)
            EXPECT_EQ(GetMonData(&mon, MON_DATA_HP_EV), 0);
    }
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
}

TEST("Brisk training modifiers leave allied trainer Pokemon unchanged")
{
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_EASY);
    VarSet(VAR_BRISK_RANDOMIZER, 0);
    struct TrainerGenerator generator = {0};
    MakePartnerGenerator(&generator, &sTrainingTrainer);
    struct Pokemon mon;
    GenerateMonFromTrainerMon(&mon, &sTrainingMon, &generator);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_IVS), sTrainingMon.iv);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_HP_EV), 100);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_ATK_EV), 80);
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
}
