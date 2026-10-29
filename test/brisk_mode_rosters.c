#include "global.h"
#include "battle.h"
#include "battle_setup.h"
#include "brisk_modes.h"
#include "data.h"
#include "difficulty.h"
#include "event_data.h"
#include "item.h"
#include "move.h"
#include "pokemon.h"
#include "string_util.h"
#include "trainer_util.h"
#include "test/test.h"
#include "constants/battle_ai.h"
#include "constants/battle_move_effects.h"
#include "constants/opponents.h"
#include "constants/trainers.h"
#include "constants/layouts.h"

// Test fixtures replace gTrainers. Validate the actual ROM roster separately.
static const struct Trainer sModeTeams[DIFFICULTY_COUNT][TRAINERS_COUNT] =
{
#include "../src/data/brisk_mode_teams.h"
};

static const struct Trainer sOriginalTeams[DIFFICULTY_COUNT][TRAINERS_COUNT] =
{
#include "../src/data/trainers.h"
};
#include "../src/data/brisk_exempt_trainers.h"

TEST("Brisk mode rosters preserve every dedication and Easter egg party field in all modes")
{
    u32 protectedCount = 0;
    for (u32 id = 1; id < TRAINERS_COUNT; id++)
    {
        if (!sBriskExemptTrainer[id])
            continue;
        protectedCount++;
        const struct Trainer *baseline = &sOriginalTeams[DIFFICULTY_NORMAL][id];
        for (u32 row = DIFFICULTY_EASY; row <= DIFFICULTY_KAIZO; row++)
        {
            const struct Trainer *trainer = &sModeTeams[row][id];
            EXPECT_EQ((u32)trainer->partySize, (u32)baseline->partySize);
            EXPECT(trainer->authoredParty);
            for (u32 slot = 0; slot < baseline->partySize; slot++)
            {
                const struct TrainerMon *actual = &trainer->party[slot];
                const struct TrainerMon *expected = &baseline->party[slot];
#define CHECK_DEDICATION_FIELD(field) EXPECT_EQ((u32)actual->field, (u32)expected->field)
                CHECK_DEDICATION_FIELD(species);
                CHECK_DEDICATION_FIELD(lvl);
                CHECK_DEDICATION_FIELD(heldItem);
                CHECK_DEDICATION_FIELD(ability);
                CHECK_DEDICATION_FIELD(iv);
                CHECK_DEDICATION_FIELD(nature);
                CHECK_DEDICATION_FIELD(gender);
                CHECK_DEDICATION_FIELD(isShiny);
                CHECK_DEDICATION_FIELD(ball);
                CHECK_DEDICATION_FIELD(friendship);
                CHECK_DEDICATION_FIELD(teraType);
                CHECK_DEDICATION_FIELD(gigantamaxFactor);
                CHECK_DEDICATION_FIELD(shouldUseDynamax);
                CHECK_DEDICATION_FIELD(dynamaxLevel);
                CHECK_DEDICATION_FIELD(tags);
#undef CHECK_DEDICATION_FIELD
                for (u32 move = 0; move < MAX_MON_MOVES; move++)
                    EXPECT_EQ(actual->moves[move], expected->moves[move]);
                for (u32 stat = 0; stat < NUM_STATS; stat++)
                    EXPECT_EQ(actual->ev ? actual->ev[stat] : 0, expected->ev ? expected->ev[stat] : 0);
                EXPECT_EQ(actual->nickname == NULL, expected->nickname == NULL);
                if (actual->nickname && expected->nickname)
                    EXPECT_EQ(StringCompare(actual->nickname, expected->nickname), 0);
            }
        }
    }
    EXPECT_EQ(protectedCount, 27);
}

TEST("Brisk mode rosters map saved Normal Easy Hard Kaizo values to Expansion rows")
{
    u16 oldMode = VarGet(VAR_BRISK_DIFFICULTY);
    const u8 rows[] = {DIFFICULTY_NORMAL, DIFFICULTY_EASY, DIFFICULTY_HARD, DIFFICULTY_KAIZO};
    for (u32 mode = 0; mode < ARRAY_COUNT(rows); mode++)
    {
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        EXPECT_EQ(GetBriskDifficultyLevel(), rows[mode]);
    }
    VarSet(VAR_BRISK_DIFFICULTY, 0xFFFF);
    EXPECT_EQ(GetBriskDifficultyLevel(), DIFFICULTY_NORMAL);
    VarSet(VAR_BRISK_DIFFICULTY, oldMode);
}

TEST("Brisk mode rosters cover every populated trainer with valid species and usable attacks")
{
    bool32 valid = TRUE;
    for (u32 row = DIFFICULTY_EASY; row <= DIFFICULTY_KAIZO; row++)
    {
        for (u32 id = 1; id < TRAINERS_COUNT; id++)
        {
            const struct Trainer *trainer = &sModeTeams[row][id];
            if (id == BRISK_KANTO_TRAINER_BASE) continue; // Reserved zero opponent in the Kanto bank.
            if (!trainer->authoredParty || trainer->party == NULL || !trainer->partySize || trainer->partySize > PARTY_SIZE)
            {
                Test_MgbaPrintf("Missing roster row %d trainer %d", row, id);
                valid = FALSE;
                continue;
            }
            for (u32 slot = 0; slot < trainer->partySize; slot++)
            {
                const struct TrainerMon *entry = &trainer->party[slot];
                if (entry->species <= SPECIES_NONE || entry->species >= NUM_SPECIES || !entry->lvl || entry->lvl > MAX_LEVEL)
                {
                    Test_MgbaPrintf("Bad roster entry row %d trainer %d slot %d", row, id, slot);
                    valid = FALSE;
                }
                // Omitted moves intentionally mean a stock level-up set.
                for (u32 moveSlot = 0; moveSlot < MAX_MON_MOVES; moveSlot++)
                {
                    enum Move move = entry->moves[moveSlot];
                    if (move != MOVE_NONE && (GetMoveEffect(move) == EFFECT_PLACEHOLDER || GetMovePP(move) == 0))
                    {
                        Test_MgbaPrintf("Unusable move row %d trainer %d slot %d move %d", row, id, slot, move);
                        valid = FALSE;
                    }
                }
            }
        }
    }
    EXPECT(valid);
}

TEST("Brisk mode rosters generate exact authored species levels moves items and training")
{
    const u16 ids[] = {TRAINER_CALVIN_1, TRAINER_ROXANNE_1, TRAINER_FLANNERY_1,
        TRAINER_TATE_AND_LIZA_1, TRAINER_WALLACE, TRAINER_STEVEN, TRAINER_HOODED_FIGURE};
    const u8 evFields[] = {MON_DATA_HP_EV, MON_DATA_ATK_EV, MON_DATA_DEF_EV, MON_DATA_SPATK_EV, MON_DATA_SPDEF_EV, MON_DATA_SPEED_EV};
    u16 oldMode = VarGet(VAR_BRISK_DIFFICULTY);
    u16 oldRandom = VarGet(VAR_BRISK_RANDOMIZER);
    u32 oldFlags = gBattleTypeFlags;
    VarSet(VAR_BRISK_RANDOMIZER, 0);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER;
    struct Pokemon party[PARTY_SIZE];
    for (u32 mode = 0; mode <= BRISK_MODE_KAIZO; mode++)
    {
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        for (u32 n = 0; n < ARRAY_COUNT(ids) + TRAINERS_COUNT; n++)
        {
            u32 id = n < ARRAY_COUNT(ids) ? ids[n] : n - ARRAY_COUNT(ids);
            if (n >= ARRAY_COUNT(ids) && !sBriskExemptTrainer[id])
                continue;
            const struct Trainer *trainer = &sModeTeams[GetBriskDifficultyLevel()][id];
            struct TrainerGenerator generator = {0};
            MakeTrainerGenerator(&generator, trainer);
            EXPECT(generator.authoredParty);
            EXPECT(!generator.smartTera);
            CreateNPCTrainerPartyFromTrainer(party, trainer);
            for (u32 slot = 0; slot < PARTY_SIZE; slot++)
            {
                if (slot >= trainer->partySize)
                {
                    EXPECT_EQ(GetMonData(&party[slot], MON_DATA_SPECIES), SPECIES_NONE);
                    continue;
                }
                const struct TrainerMon *entry = &trainer->party[slot];
                EXPECT_EQ(GetMonData(&party[slot], MON_DATA_SPECIES), entry->species);
                EXPECT_EQ(GetMonData(&party[slot], MON_DATA_LEVEL), entry->lvl);
                EXPECT_EQ(GetMonData(&party[slot], MON_DATA_HELD_ITEM), entry->heldItem);
                EXPECT_EQ(GetMonData(&party[slot], MON_DATA_IVS), entry->iv);
                EXPECT_EQ(GetNature(&party[slot]), entry->nature);
                for (u32 stat = 0; stat < NUM_STATS; stat++)
                    EXPECT_EQ(GetMonData(&party[slot], evFields[stat]), entry->ev ? entry->ev[stat] : 0);
                if (entry->moves[0] != MOVE_NONE)
                    for (u32 moveSlot = 0; moveSlot < MAX_MON_MOVES; moveSlot++)
                        EXPECT_EQ(GetMonData(&party[slot], MON_DATA_MOVE1 + moveSlot), entry->moves[moveSlot]);
            }
        }
    }
    VarSet(VAR_BRISK_DIFFICULTY, oldMode);
    VarSet(VAR_BRISK_RANDOMIZER, oldRandom);
    gBattleTypeFlags = oldFlags;
}

TEST("Brisk mode rosters preserve League and Meteor Falls Steven fixed levels")
{
    const u16 league[] = {TRAINER_SIDNEY,TRAINER_PHOEBE,TRAINER_GLACIA,TRAINER_DRAKE,TRAINER_WALLACE};
    for (u32 row = DIFFICULTY_EASY; row <= DIFFICULTY_KAIZO; row++)
    {
        for (u32 i = 0; i < ARRAY_COUNT(league); i++)
            for (u32 slot = 0; slot < sModeTeams[row][league[i]].partySize; slot++)
                EXPECT_EQ(sModeTeams[row][league[i]].party[slot].lvl, row == DIFFICULTY_EASY ? 70 : 100);
        for (u32 slot = 0; slot < sModeTeams[row][TRAINER_STEVEN].partySize; slot++)
            EXPECT_EQ(sModeTeams[row][TRAINER_STEVEN].party[slot].lvl, 100);
    }
}

TEST("Brisk Champions Archive uses fixed champion teams and maximal AI in every mode")
{
    const u16 archive[] = {TRAINER_BLUE, TRAINER_RED2, TRAINER_WALLACE,
        TRAINER_CYNTHIA, TRAINER_IRIS, TRAINER_DIANTHA, TRAINER_LEON,
        TRAINER_NEMONA, TRAINER_VOLO};
    const u64 strongAI = AI_FLAG_BASIC_TRAINER | AI_FLAG_OMNISCIENT
        | AI_FLAG_SMART_SWITCHING | AI_FLAG_SMART_MON_CHOICES
        | AI_FLAG_HP_AWARE | AI_FLAG_TRY_TO_2HKO
        | AI_FLAG_PP_STALL_PREVENTION | AI_FLAG_SMART_TERA
        | AI_FLAG_PREDICTION | AI_FLAG_KNOW_OPPONENT_PARTY
        | AI_FLAG_CONSERVATIVE;
    struct MapHeader savedHeader = gMapHeader;
    u16 savedDifficulty = VarGet(VAR_BRISK_DIFFICULTY);
    gMapHeader.mapLayoutId = LAYOUT_ROUTE135_ARCHIVES;
    for (u32 mode = BRISK_MODE_NORMAL; mode <= BRISK_MODE_KAIZO; mode++)
    {
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        for (u32 i = 0; i < ARRAY_COUNT(archive); i++)
        {
            const u16 id = archive[i];
            EXPECT(BriskIsChampionsArchiveTrainer(id));
            EXPECT_EQ(GetTrainerDifficultyLevel(id), DIFFICULTY_NORMAL);
            EXPECT_EQ(GetBriskTrainerAiFlagsForTrainer(0, id), strongAI);
            const struct Trainer *fixed = &sModeTeams[GetTrainerDifficultyLevel(id)][id];
            const struct Trainer *normal = &sModeTeams[DIFFICULTY_NORMAL][id];
            EXPECT_EQ((u32)fixed->partySize, (u32)normal->partySize);
            EXPECT_EQ(fixed->party, normal->party);
            EXPECT_EQ(fixed->trainerPic, normal->trainerPic);
            EXPECT_EQ(fixed->trainerClass, normal->trainerClass);
            EXPECT_EQ((u32)fixed->encounterMusic, (u32)normal->encounterMusic);
        }
        EXPECT(!BriskIsChampionsArchiveTrainer(TRAINER_STEVEN));
        EXPECT(!BriskIsChampionsArchiveTrainer(TRAINER_ALLEN));
    }
    // Wallace's League fight still uses Easy levels and its usual AI.
    gMapHeader.mapLayoutId = LAYOUT_EVER_GRANDE_CITY_CHAMPIONS_ROOM;
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_EASY);
    EXPECT(!BriskIsChampionsArchiveTrainer(TRAINER_WALLACE));
    // Battle fixtures replace gTrainers and omit Wallace. Validate the saved
    // mode and real roster here instead of the fixture table fallback.
    EXPECT_EQ(GetBriskDifficultyLevel(), DIFFICULTY_EASY);
    EXPECT_EQ(sModeTeams[GetBriskDifficultyLevel()][TRAINER_WALLACE].party[0].lvl, 70);
    EXPECT_EQ(GetBriskTrainerAiFlagsForTrainer(0, TRAINER_WALLACE), AI_FLAG_CHECK_BAD_MOVE);
    gMapHeader = savedHeader;
    VarSet(VAR_BRISK_DIFFICULTY, savedDifficulty);
}

TEST("Brisk Kaizo boss appearances stay consistent across authored mode teams")
{
    for (u32 row = DIFFICULTY_EASY; row <= DIFFICULTY_KAIZO; row++)
    {
        EXPECT_EQ(sModeTeams[row][TRAINER_HOODED_FIGURE].trainerPic, TRAINER_PIC_HEX_MANIAC);
        EXPECT_EQ(sModeTeams[row][TRAINER_HOODED_FIGURE].trainerClass, TRAINER_CLASS_HEX_MANIAC);
        EXPECT_EQ((u32)sModeTeams[row][TRAINER_HOODED_FIGURE].gender, FEMALE);
        EXPECT_EQ(sModeTeams[row][TRAINER_TANKERSON].trainerPic, TRAINER_PIC_VOLO);
        EXPECT_EQ(sModeTeams[row][TRAINER_TANKERSON].trainerClass, TRAINER_CLASS_CHAMPION);
    }
}
