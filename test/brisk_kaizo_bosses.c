#include "global.h"
#include "battle.h"
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
static const struct Trainer sAuthoredKaizo[DIFFICULTY_COUNT][TRAINERS_COUNT] =
{
#include "../src/data/trainers.h"
};
static const u16 sKaizoIds[] = {TRAINER_HOODED_FIGURE, TRAINER_TANKERSON};

TEST("Brisk Kaizo bosses all 12 authored Pokemon have valid abilities items and four usable distinct moves")
{
    bool32 allLegal = TRUE;
    for (u32 n = 0; n < ARRAY_COUNT(sKaizoIds); n++)
    {
        const struct Trainer *trainer = &sAuthoredKaizo[DIFFICULTY_NORMAL][sKaizoIds[n]];
        EXPECT_EQ((u32)trainer->partySize, 6);
        for (u32 slot = 0; slot < PARTY_SIZE; slot++)
        {
            const struct TrainerMon *entry = &trainer->party[slot];
            EXPECT_EQ(entry->lvl, 100);
            EXPECT(entry->heldItem != ITEM_NONE);
            bool32 abilityLegal = FALSE;
            for (u32 ability = 0; ability < NUM_ABILITY_SLOTS; ability++)
                if (entry->ability != ABILITY_NONE && GetAbilityBySpecies(entry->species, ability) == entry->ability)
                    abilityLegal = TRUE;
            if (!abilityLegal)
            {
                Test_MgbaPrintf("League trainer %d slot %d has illegal ability %d", sKaizoIds[n], slot, entry->ability);
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
                if (IsHoldEffectChoice(GetItemHoldEffect(entry->heldItem)) || GetItemHoldEffect(entry->heldItem) == HOLD_EFFECT_ASSAULT_VEST)
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

TEST("Brisk Kaizo bosses preserves every authored move ability nature item and role EV spread across all modes")
{
    u16 oldMode = VarGet(VAR_BRISK_DIFFICULTY);
    u16 oldRandom = VarGet(VAR_BRISK_RANDOMIZER);
    VarSet(VAR_BRISK_RANDOMIZER, 0);
    const u8 evFields[] = {MON_DATA_HP_EV, MON_DATA_ATK_EV, MON_DATA_DEF_EV, MON_DATA_SPATK_EV, MON_DATA_SPDEF_EV, MON_DATA_SPEED_EV};
    for (u32 mode = 0; mode <= BRISK_MODE_EXTREME; mode++)
    {
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        for (u32 n = 0; n < ARRAY_COUNT(sKaizoIds); n++)
        {
            const struct Trainer *trainer = &sAuthoredKaizo[DIFFICULTY_NORMAL][sKaizoIds[n]];
            struct TrainerGenerator generator = {0};
            Test_BriskTrainerSource(trainer, sKaizoIds[n]);
            MakeTrainerGenerator(&generator, trainer);
            EXPECT(generator.kaizoBoss);

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

TEST("Brisk Kaizo saves retain mode 3 and both bosses are unavailable in all other modes")
{
    u16 mode = VarGet(VAR_BRISK_DIFFICULTY);
    EXPECT_EQ(BRISK_MODE_KAIZO, 3);
    EXPECT_EQ(BRISK_MODE_KAIZO, BRISK_MODE_EXTREME);
    EXPECT_EQ(MAX_TRAINERS_COUNT_EMERALD, 865);
    EXPECT(BriskIsKaizoBoss(TRAINER_HOODED_FIGURE));
    EXPECT(BriskIsKaizoBoss(TRAINER_TANKERSON));
    EXPECT(!BriskIsKaizoBoss(TRAINER_WALLACE));
    for (u32 m = 0; m < 4; m++)
    {
        VarSet(VAR_BRISK_DIFFICULTY, m);
        EXPECT_EQ(BriskKaizoBossAvailable(), m == BRISK_MODE_KAIZO);
    }
    VarSet(VAR_BRISK_DIFFICULTY, mode);
}

TEST("Brisk Kaizo bosses have native legal Crowned Mega and Primal transformations")
{
    struct FormChangeContext ctx = {.currentSpecies = SPECIES_ZACIAN, .heldItem = ITEM_RUSTED_SWORD, .method = FORM_CHANGE_BEGIN_BATTLE};
    EXPECT_EQ(GetFormChangeTargetSpecies_Internal(ctx), SPECIES_ZACIAN_CROWNED);
    ctx.currentSpecies = SPECIES_ZAMAZENTA; ctx.heldItem = ITEM_RUSTED_SHIELD;
    EXPECT_EQ(GetFormChangeTargetSpecies_Internal(ctx), SPECIES_ZAMAZENTA_CROWNED);
    ctx.currentSpecies = SPECIES_KYOGRE; ctx.heldItem = ITEM_BLUE_ORB; ctx.method = FORM_CHANGE_BATTLE_PRIMAL_REVERSION;
    EXPECT_EQ(GetFormChangeTargetSpecies_Internal(ctx), SPECIES_KYOGRE_PRIMAL);
    ctx.currentSpecies = SPECIES_KANGASKHAN; ctx.heldItem = ITEM_KANGASKHANITE; ctx.method = FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM;
    EXPECT_EQ(GetFormChangeTargetSpecies_Internal(ctx), SPECIES_KANGASKHAN_MEGA);
    ctx.currentSpecies = SPECIES_RAYQUAZA; ctx.heldItem = ITEM_LIFE_ORB; ctx.method = FORM_CHANGE_BATTLE_MEGA_EVOLUTION_MOVE; ctx.moves[0] = MOVE_DRAGON_ASCENT;
    EXPECT_EQ(GetFormChangeTargetSpecies_Internal(ctx), SPECIES_RAYQUAZA_MEGA);
}
