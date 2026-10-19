#include "global.h"
#include "battle.h"
#include "brisk_modes.h"
#include "caps.h"
#include "data.h"
#include "event_data.h"
#include "item.h"
#include "main.h"
#include "party_menu.h"
#include "pokemon.h"
#include "script_pokemon_util.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "trainer_util.h"
#include "wild_encounter.h"
#include "window.h"
#include "test/test.h"
#include "constants/opponents.h"
#include "constants/trainers.h"

// The battle test runner substitutes gTrainers; use an actual generator fixture here.
static const struct TrainerMon sModeMon = {
    .species = SPECIES_GOLEM, .lvl = 40, .moves = {MOVE_TACKLE},
    .gender = TRAINER_MON_RANDOM_GENDER, .nature = NATURE_HARDY, .ball = BALL_POKE,
};
static const struct Trainer sModeTrainer = {
    .trainerClass = TRAINER_CLASS_LEADER, .party = &sModeMon, .partySize = 1,
    .trainerName = _("TEST"),
};

TEST("Brisk modes scale generated trainer teams once")
{
    const struct Trainer *trainer = &sModeTrainer;
    EXPECT_EQ(trainer->party[0].lvl, 40);
    for (u32 mode = 0; mode <= BRISK_MODE_EXTREME; mode++)
    {
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        VarSet(VAR_BRISK_RANDOMIZER, 0);
        struct TrainerGenerator generator = {0};
        struct Pokemon mon;
        MakeTrainerGenerator(&generator, trainer);
        GenerateMonFromTrainerMon(&mon, &trainer->party[0], &generator);
        u32 level = GetMonData(&mon, MON_DATA_LEVEL);
        if (mode == BRISK_MODE_NORMAL) EXPECT_EQ(level, 40);
        if (mode == BRISK_MODE_EASY) EXPECT_EQ(level, 20);
        if (mode == BRISK_MODE_HARD) EXPECT_EQ(level, min(44, GetProgressionLevelCap()));
        if (mode == BRISK_MODE_EXTREME) EXPECT_EQ(level, min(48, GetProgressionLevelCap()));
        CalculateMonStats(&mon);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_LEVEL), level);
    }
    VarSet(VAR_BRISK_DIFFICULTY, 0);
}

TEST("Brisk modes clamp levels and keep difficulty and randomizer defaults off")
{
    VarSet(VAR_BRISK_DIFFICULTY, 0);
    VarSet(VAR_BRISK_RANDOMIZER, 0);
    EXPECT_EQ(ScaleBriskLevel(27, 5), 27);
    EXPECT_EQ(BriskRandomSpecies(SPECIES_PIKACHU, BRISK_RAND_WILD), SPECIES_PIKACHU);
    EXPECT_EQ(BriskRandomItem(ITEM_POTION), ITEM_POTION);
    for (u32 mode = 1; mode < 4; mode++)
    {
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        for (u32 level = 1; level <= 100; level++)
        {
            EXPECT_GE(ScaleBriskLevel(level, level), 1);
            EXPECT_LE(ScaleBriskLevel(level, level), 100);
        }
    }
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_EASY);
    EXPECT_EQ(ScaleBriskLevel(1, 0), 1);
    EXPECT_EQ(ScaleBriskLevel(21, 0), 11);
    VarSet(VAR_BRISK_DIFFICULTY, 0);
}

TEST("Brisk modes ordinary trainer randomization leaves important teams intact")
{
    VarSet(VAR_BRISK_RANDOMIZER, BRISK_RAND_TRAINERS);
    struct Pokemon mon;
    struct Trainer ordinaryTrainer = sModeTrainer;
    ordinaryTrainer.trainerClass = TRAINER_CLASS_YOUNGSTER;
    const struct Trainer *boss = &sModeTrainer;
    const struct Trainer *ordinary = &ordinaryTrainer;
    struct TrainerGenerator generator = {0};
    MakeTrainerGenerator(&generator, boss);
    GenerateMonFromTrainerMon(&mon, &boss->party[0], &generator);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_SPECIES), boss->party[0].species);
    MakeTrainerGenerator(&generator, ordinary);
    GenerateMonFromTrainerMon(&mon, &ordinary->party[0], &generator);
    EXPECT_NE(GetMonData(&mon, MON_DATA_SPECIES), ordinary->party[0].species);
    VarSet(VAR_BRISK_RANDOMIZER, BRISK_RAND_IMPORTANT);
    MakeTrainerGenerator(&generator, boss);
    GenerateMonFromTrainerMon(&mon, &boss->party[0], &generator);
    EXPECT_NE(GetMonData(&mon, MON_DATA_SPECIES), boss->party[0].species);
    MakeTrainerGenerator(&generator, ordinary);
    GenerateMonFromTrainerMon(&mon, &ordinary->party[0], &generator);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_SPECIES), ordinary->party[0].species);
    VarSet(VAR_BRISK_RANDOMIZER, 0);
}

TEST("Brisk modes wild species and levels agree with overworld generated Pokemon")
{
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_HARD);
    VarSet(VAR_BRISK_RANDOMIZER, BRISK_RAND_WILD);
    CreateWildMon(SPECIES_PIKACHU, 20);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_SPECIES), BriskRandomSpecies(SPECIES_PIKACHU, BRISK_RAND_WILD));
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_LEVEL), ScaleBriskLevel(20, SPECIES_PIKACHU));
    VarSet(VAR_BRISK_DIFFICULTY, 0);
    VarSet(VAR_BRISK_RANDOMIZER, 0);
}

TEST("Brisk modes randomization never chooses disabled species or temporary forms")
{
    VarSet(VAR_BRISK_RANDOMIZER, BRISK_RAND_ALL);
    for (u32 species = 1; species < NUM_SPECIES; species++)
    {
        if (species == SPECIES_EGG || !IsSpeciesEnabled(species))
            continue;
        enum Species mapped = BriskRandomSpecies(species, BRISK_RAND_WILD);
        EXPECT(IsSpeciesEnabled(mapped));
        EXPECT_NE(mapped, SPECIES_EGG);
        EXPECT(!gSpeciesInfo[mapped].isMegaEvolution);
        EXPECT(!gSpeciesInfo[mapped].isGigantamax);
        EXPECT_EQ(BriskRandomSpecies(species, BRISK_RAND_WILD), mapped);
        EXPECT(GetSpeciesBaseHP(species) > 0);
        EXPECT(GetSpeciesType(species, 0) < NUMBER_OF_MON_TYPES);
        EXPECT(GetAbilityBySpecies(species, 0) != ABILITY_NONE);
    }
    VarSet(VAR_BRISK_RANDOMIZER, 0);
}

TEST("Brisk modes items preserve progression items and the actual gift matches its name")
{
    VarSet(VAR_BRISK_RANDOMIZER, BRISK_RAND_ITEMS);
    EXPECT_EQ(BriskRandomItem(ITEM_HM03), ITEM_HM03);
    EXPECT_EQ(BriskRandomItem(ITEM_MACH_BIKE), ITEM_MACH_BIKE);
    EXPECT_EQ(BriskRandomItem(ITEM_NONE), ITEM_NONE);
    for (u32 item = 1; item < ITEMS_COUNT; item++)
    {
        enum Item mapped = BriskRandomItem(item);
        EXPECT(mapped < ITEMS_COUNT);
        if (mapped != item)
        {
            EXPECT(!GetItemImportance(mapped));
            EXPECT_NE(GetItemPocket(mapped), POCKET_KEY_ITEMS);
        }
    }
    gSpecialVar_0x8000 = ITEM_POTION;
    BriskRandomizeScriptItem();
    EXPECT_EQ(gSpecialVar_0x8000, BriskRandomItem(ITEM_POTION));
    VarSet(VAR_BRISK_RANDOMIZER, 0);
}

TEST("Brisk multiuse candy starts evolution after accepting the last learned move")
{
    ResetTasks();
    VarSet(VAR_BRISK_RANDOMIZER, 0);
    CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][0], SPECIES_BULBASAUR, 17, 0, OTID_STRUCT_PRESET(0), 0);
    EXPECT(Test_ResumeBriskCandyAfterLearnedMove());
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_LEVEL), 17);
}

TEST("Brisk capsule editor text fits and stays visible while idle")
{
    ResetTasks();
    CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][0], SPECIES_PIKACHU, 20, 0, OTID_STRUCT_PRESET(0), 0);
    u8 taskId = Test_OpenBriskCapsuleEditor();
    EXPECT_LE(GetStringWidth(FONT_NORMAL, gStringVar4, 0), 224);
    u8 *pixels = (u8 *)GetWindowAttribute(PARTY_SIZE, WINDOW_TILE_DATA);
    u32 hash = Crc32B(pixels, 28 * 4 * 32);
    bool32 hasInk = FALSE;
    for (u32 i = 0; i < 28 * 4 * 32; i++)
        if (pixels[i] != 0x11)
            hasInk = TRUE;
    EXPECT(hasInk);
    gMain.newKeys = gMain.newAndRepeatedKeys = 0;
    for (u32 frame = 0; frame < 120; frame++)
        RunTasks();
    EXPECT_EQ(Crc32B(pixels, 28 * 4 * 32), hash);
    EXPECT(gTasks[taskId].isActive);
    EXPECT(!IsPartyMenuTextPrinterActive());
    gMain.newAndRepeatedKeys = DPAD_RIGHT;
    RunTasks();
    EXPECT_NE(Crc32B(pixels, 28 * 4 * 32), hash);
    gMain.newAndRepeatedKeys = 0;
    gMain.newKeys = A_BUTTON;
    RunTasks();
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_BALL_CAPSULE_SEALS), 1);
    gMain.newKeys = 0;
    Test_CloseBriskCapsuleEditor();
}
