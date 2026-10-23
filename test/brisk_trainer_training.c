#include "global.h"
#include "battle.h"
#include "brisk_modes.h"
#include "data.h"
#include "event_data.h"
#include "pokemon.h"
#include "trainer_util.h"
#include "test/test.h"
#include "constants/trainers.h"
#include "constants/form_change_types.h"

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

TEST("Brisk trainer generation recovers from stale and out-of-range abilities")
{
    VarSet(VAR_BRISK_RANDOMIZER, 0);
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
    static const enum Ability abilities[] = {ABILITY_CURSED_BODY, ABILITIES_COUNT};
    for (u32 i = 0; i < ARRAY_COUNT(abilities); i++)
    {
        struct TrainerMon entry = sTrainingMon;
        struct Pokemon mon;
        entry.species = SPECIES_HAUNTER;
        entry.ability = abilities[i];
        entry.moves[0] = MOVE_SHADOW_BALL;
        entry.moves[1] = MOVE_THUNDERBOLT;
        GenerateTrainingMon(&mon, &entry);
        EXPECT_EQ(GetMonAbility(&mon), ABILITY_LEVITATE);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_MOVE1), MOVE_SHADOW_BALL);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_MOVE2), MOVE_THUNDERBOLT);
    }
}

TEST("Brisk trainer generation preserves valid authored abilities and moves")
{
    VarSet(VAR_BRISK_RANDOMIZER, 0);
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
    struct TrainerMon entry = sTrainingMon;
    struct Pokemon mon;
    entry.species = SPECIES_HITMONCHAN;
    entry.ability = ABILITY_IRON_FIST;
    entry.moves[0] = MOVE_ICE_PUNCH;
    GenerateTrainingMon(&mon, &entry);
    EXPECT_EQ(GetMonAbility(&mon), ABILITY_IRON_FIST);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_MOVE1), MOVE_ICE_PUNCH);
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
    EXPECT_LE(GetMonData(&mon, MON_DATA_SPATK_EV), 6); // New special coverage may use the spare EVs.
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
        CreateMonWithIVs(&mon, entry.species, 50, 0, OTID_STRUCT_PRESET(0), 0);
        SetMonMoveSlot(&mon, MOVE_TRICK_ROOM, 0);
        SetMonMoveSlot(&mon, MOVE_BODY_PRESS, 1);
        Test_ApplyBriskTrainerTraining(&mon);
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

TEST("Brisk every authored trainer ability belongs to its pre-battle species")
{
    for (u32 difficulty = 0; difficulty < DIFFICULTY_COUNT; difficulty++)
    {
        for (u32 id = 0; id < TRAINERS_COUNT; id++)
        {
            const struct Trainer *trainer = &gTrainers[difficulty][id];
            for (u32 slot = 0; slot < trainer->partySize; slot++)
            {
                const struct TrainerMon *entry = &trainer->party[slot];
                if (entry->ability == ABILITY_NONE)
                    continue;
                bool32 legal = FALSE;
                EXPECT_LT(entry->ability, ABILITIES_COUNT);
                for (u32 abilitySlot = 0; abilitySlot < NUM_ABILITY_SLOTS; abilitySlot++)
                    if (gSpeciesInfo[entry->species].abilities[abilitySlot] == entry->ability)
                        legal = TRUE;
                EXPECT(legal);
            }
        }
    }
}

TEST("Brisk Mega Scovillain gains Spicy Spray from every base ability slot")
{
    VarSet(VAR_BRISK_RANDOMIZER, 0);
    struct FormChangeContext ctx = {
        .currentSpecies = SPECIES_SCOVILLAIN, .heldItem = ITEM_SCOVILLAINITE,
        .method = FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM,
    };
    u32 mega = GetFormChangeTargetSpecies_Internal(ctx);
    EXPECT_EQ(mega, SPECIES_SCOVILLAIN_MEGA);
    for (u32 slot = 0; slot < NUM_ABILITY_SLOTS; slot++)
    {
        struct Pokemon mon;
        CreateMonWithIVs(&mon, SPECIES_SCOVILLAIN, 50, 0, OTID_STRUCT_PRESET(0), 31);
        SetMonData(&mon, MON_DATA_ABILITY_NUM, &slot);
        EXPECT_NE(GetMonAbility(&mon), ABILITY_SPICY_SPRAY);
        SetMonData(&mon, MON_DATA_SPECIES, &mega);
        EXPECT_EQ(GetMonAbility(&mon), ABILITY_SPICY_SPRAY);
    }
}

TEST("Brisk every Mega form resolves a valid ability from every inherited slot")
{
    VarSet(VAR_BRISK_RANDOMIZER, 0);
    for (u32 species = 1; species < NUM_SPECIES; species++)
    {
        if (!gSpeciesInfo[species].isMegaEvolution)
            continue;
        EXPECT_NE(gSpeciesInfo[species].abilities[0], ABILITY_NONE);
        for (u32 slot = 0; slot < NUM_ABILITY_SLOTS; slot++)
        {
            enum Ability ability = GetAbilityBySpecies(species, slot);
            EXPECT_NE(ability, ABILITY_NONE);
            EXPECT_LT(ability, ABILITIES_COUNT);
            bool32 belongsToMega = FALSE;
            for (u32 i = 0; i < NUM_ABILITY_SLOTS; i++)
                if (gSpeciesInfo[species].abilities[i] == ability)
                    belongsToMega = TRUE;
            EXPECT(belongsToMega);
        }
    }
}
