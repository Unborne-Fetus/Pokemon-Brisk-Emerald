#include "global.h"
#include "battle.h"
#include "battle_setup.h"
#include "battle_transition.h"
#include "brisk_modes.h"
#include "brisk_trainer_moves.h"
#include "data.h"
#include "event_data.h"
#include "item.h"
#include "move.h"
#include "pokemon.h"
#include "trainer_pools.h"
#include "trainer_util.h"
#include "test/test.h"
#include "constants/battle_ai.h"
#include "constants/battle_move_effects.h"
#include "constants/hold_effects.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/opponents.h"
#include "constants/trainers.h"

void Test_BriskTrainerSource(const struct Trainer *trainer, u32 id);

// The test runner replaces gTrainers. Include the real trainerproc output under
// a separate name so these tests validate authored ROM teams, not dummy fixtures.
static const struct Trainer sAuthoredSteven[DIFFICULTY_COUNT][TRAINERS_COUNT] =
{
#include "../src/data/trainers.h"
};
static const u16 sStevenIds[] = {TRAINER_STEVEN};

TEST("Brisk Steven champion all six authored Pokemon have valid abilities items and four usable distinct moves")
{
    bool32 allLegal = TRUE;
    for (u32 n = 0; n < ARRAY_COUNT(sStevenIds); n++)
    {
        const struct Trainer *trainer = &sAuthoredSteven[DIFFICULTY_NORMAL][sStevenIds[n]];
        EXPECT_EQ((u32)trainer->partySize, 6);
        EXPECT_EQ((u32)trainer->trainerClass, TRAINER_CLASS_CHAMPION);
        for (u32 slot = 0; slot < PARTY_SIZE; slot++)
        {
            const struct TrainerMon *entry = &trainer->party[slot];
            EXPECT_EQ(entry->lvl, 100);
            EXPECT(GetSpeciesType(entry->species, 0) == TYPE_STEEL || GetSpeciesType(entry->species, 1) == TYPE_STEEL);
            EXPECT(entry->heldItem != ITEM_NONE);
            bool32 abilityLegal = FALSE;
            for (u32 ability = 0; ability < NUM_ABILITY_SLOTS; ability++)
                if (entry->ability != ABILITY_NONE && GetAbilityBySpecies(entry->species, ability) == entry->ability)
                    abilityLegal = TRUE;
            if (!abilityLegal)
            {
                Test_MgbaPrintf("League trainer %d slot %d has illegal ability %d", sStevenIds[n], slot, entry->ability);
                allLegal = FALSE;
            }
            u32 attacks = 0;
            for (u32 moveSlot = 0; moveSlot < MAX_MON_MOVES; moveSlot++)
            {
                enum Move move = entry->moves[moveSlot];
                // Full cross-generation TM/tutor/transfer compatibility is
                // checked against all_learnables.json by league_teams.py.
                EXPECT(GetMoveEffect(move) != EFFECT_PLACEHOLDER);
                EXPECT(GetMovePP(move) > 0);
                EXPECT(move != MOVE_NONE);
                if (GetMoveCategory(move) != DAMAGE_CATEGORY_STATUS) attacks++;
                for (u32 previous = 0; previous < moveSlot; previous++) EXPECT_NE(move, entry->moves[previous]);
                if ((IsHoldEffectChoice(GetItemHoldEffect(entry->heldItem)) || GetItemHoldEffect(entry->heldItem) == HOLD_EFFECT_ASSAULT_VEST) && move != MOVE_TRICK)
                    EXPECT(GetMoveCategory(move) != DAMAGE_CATEGORY_STATUS);
                if (trainer->battleType == TRAINER_BATTLE_TYPE_DOUBLES)
                {
                    EXPECT(move != MOVE_EARTHQUAKE && move != MOVE_SURF && move != MOVE_DISCHARGE
                        && move != MOVE_LAVA_PLUME && move != MOVE_SLUDGE_WAVE && move != MOVE_BOOMBURST);
                }
            }
            EXPECT(attacks > 0);
        }
    }
    EXPECT(allLegal);
}

TEST("Brisk Steven champion preserves every authored move ability nature item and role EV spread across all modes")
{
    u16 oldMode = VarGet(VAR_BRISK_DIFFICULTY);
    u16 oldRandom = VarGet(VAR_BRISK_RANDOMIZER);
    VarSet(VAR_BRISK_RANDOMIZER, 0);
    const u8 evFields[] = {MON_DATA_HP_EV, MON_DATA_ATK_EV, MON_DATA_DEF_EV, MON_DATA_SPATK_EV, MON_DATA_SPDEF_EV, MON_DATA_SPEED_EV};
    for (u32 mode = 0; mode <= BRISK_MODE_EXTREME; mode++)
    {
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        for (u32 n = 0; n < ARRAY_COUNT(sStevenIds); n++)
        {
            const struct Trainer *trainer = &sAuthoredSteven[DIFFICULTY_NORMAL][sStevenIds[n]];
            struct TrainerGenerator generator = {0};
            Test_BriskTrainerSource(trainer, sStevenIds[n]);
            MakeTrainerGenerator(&generator, trainer);
            EXPECT(generator.championSet);

            generator.highestLevel = 100;
            for (u32 slot = 0; slot < PARTY_SIZE; slot++)
            {
                const struct TrainerMon *entry = &trainer->party[slot];
                struct Pokemon mon;
                GenerateMonFromTrainerMon(&mon, entry, &generator);
                EXPECT_EQ(GetMonData(&mon, MON_DATA_LEVEL), 100);
                EXPECT_EQ(GetMonData(&mon, MON_DATA_HELD_ITEM), entry->heldItem);
                EXPECT_EQ(GetMonAbility(&mon), entry->ability);
                EXPECT_EQ(GetNature(&mon), entry->nature);
                EXPECT_EQ(GetMonData(&mon, MON_DATA_IVS), entry->iv);
                for (u32 moveSlot = 0; moveSlot < MAX_MON_MOVES; moveSlot++)
                {
                    EXPECT_EQ(GetMonData(&mon, MON_DATA_MOVE1 + moveSlot), entry->moves[moveSlot]);
                    EXPECT_EQ(GetMonData(&mon, MON_DATA_PP1 + moveSlot), GetMovePP(entry->moves[moveSlot]));
                }
                for (u32 stat = 0; stat < NUM_STATS; stat++)
                    EXPECT_EQ(GetMonData(&mon, evFields[stat]), entry->ev[stat]);
            }
        }
    }
    Test_BriskTrainerSource(NULL, 0);
    VarSet(VAR_BRISK_DIFFICULTY, oldMode);
    VarSet(VAR_BRISK_RANDOMIZER, oldRandom);
}


TEST("Brisk Steven champion generates six level 100 Pokemon in every mode even below the final badge cap")
{
    u16 mode = VarGet(VAR_BRISK_DIFFICULTY), random = VarGet(VAR_BRISK_RANDOMIZER);
    u32 flags = gBattleTypeFlags;
    gBattleTypeFlags = BATTLE_TYPE_TRAINER;
    VarSet(VAR_BRISK_RANDOMIZER, 0);
    const struct Trainer *trainer = &sAuthoredSteven[DIFFICULTY_NORMAL][TRAINER_STEVEN];
    Test_BriskTrainerSource(trainer, TRAINER_STEVEN);
    for (u32 m=0;m<4;m++)
    {
        VarSet(VAR_BRISK_DIFFICULTY,m);
        struct Pokemon party[PARTY_SIZE] = {0};
        CreateNPCTrainerPartyFromTrainer(party,trainer);
        for(u32 i=0;i<PARTY_SIZE;i++)
        {
            EXPECT_EQ(GetMonData(&party[i],MON_DATA_SPECIES),trainer->party[i].species);
            EXPECT_EQ(GetMonData(&party[i],MON_DATA_LEVEL),100);
        }
    }
    Test_BriskTrainerSource(NULL,0);
    VarSet(VAR_BRISK_DIFFICULTY, mode); VarSet(VAR_BRISK_RANDOMIZER, random); gBattleTypeFlags = flags;
}
