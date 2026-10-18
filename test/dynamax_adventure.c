#include "global.h"
#include "dynamax_adventure.h"
#include "battle.h"
#include "battle_anim.h"
#include "item_use.h"
#include "type_icons.h"
#include "battle_interface.h"
#include "battle_main.h"
#include "battle_script_commands.h"
#include "battle_setup.h"
#include "battle_util2.h"
#include "reshow_battle_screen.h"
#include "battle_partner.h"
#include "battle_controllers.h"
#include "trainer_util.h"
#include "constants/opponents.h"
#include "constants/battle_ai.h"
#include "data.h"
#include "brisk_modes.h"
#include "event_data.h"
#include "main.h"
#include "move.h"
#include "pokemon_storage_system.h"
#include "save.h"
#include "string_util.h"
#include "overworld.h"
#include "constants/maps.h"
#include "constants/event_objects.h"
#include "test/test.h"
#include "script_menu.h"
#include "field_specials.h"
#include "pokemon_icon.h"
#include "malloc.h"
#include "sprite.h"
#include "task.h"
#include "window.h"
#include "dynamax_adventure_coop.h"
#include "battle_ai_main.h"
#include "constants/form_change_types.h"

static void StartRental(void)
{
    EXPECT(DynamaxAdventure_Begin(12345));
    EXPECT(DynamaxAdventure_ChooseRental(0));
    EXPECT_EQ(DynamaxAdventure_GetStatus().rentalsChosen, DA_TEAM_SIZE);
    EXPECT(!DynamaxAdventure_ChooseRental(0));
    EXPECT_EQ(DynamaxAdventure_GetStatus().phase, DA_PHASE_PATH);
}

TEST("Adventure: state guards reject invalid actions without changing the party")
{
    struct Pokemon original[PARTY_SIZE];
    memcpy(original, gParties[B_TRAINER_PLAYER], sizeof(original));
    EXPECT(!DynamaxAdventure_ChooseRental(0));
    EXPECT(!DynamaxAdventure_PrepareBattle());
    EXPECT(!DynamaxAdventure_Claim(0));
    EXPECT(DynamaxAdventure_Begin(1));
    EXPECT(!DynamaxAdventure_Begin(2));
    EXPECT(!DynamaxAdventure_ChooseRental(DA_RENTAL_CHOICES));
    EXPECT(!DynamaxAdventure_ChoosePath(0));
    EXPECT(!DynamaxAdventure_Catch(TRUE));
    EXPECT(!DynamaxAdventure_Swap(TRUE));
    EXPECT(!DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
    EXPECT_EQ(DynamaxAdventure_GetStatus().phase, DA_PHASE_RENTAL);
    EXPECT_EQ(memcmp(original, gParties[B_TRAINER_PLAYER], sizeof(original)), 0);
    EXPECT(DynamaxAdventure_Quit());
    EXPECT_EQ(DynamaxAdventure_GetStatus().phase, DA_PHASE_INACTIVE);
}

TEST("Adventure: seed creates distinct rentals and consistent route choices")
{
    u16 rentals[DA_RENTAL_CHOICES];
    u16 paths[DA_PATH_CHOICES];
    EXPECT(DynamaxAdventure_Begin(42));
    for (u32 i = 0; i < DA_RENTAL_CHOICES; i++)
    {
        struct Pokemon *mon = DynamaxAdventure_GetRental(i);
        rentals[i] = GetMonData(mon, MON_DATA_SPECIES);
        EXPECT_EQ(GetMonData(mon, MON_DATA_LEVEL), DA_RENTAL_LEVEL);

        for (u32 j = 0; j < MAX_MON_MOVES; j++)
            EXPECT_NE(GetMonData(mon, MON_DATA_MOVE1 + j), MOVE_NONE);
        for (u32 j = 0; j < i; j++)
            EXPECT_NE(rentals[i], rentals[j]);
    }
    EXPECT(DynamaxAdventure_ChooseRental(1));
    EXPECT(!DynamaxAdventure_ChooseRental(2));
    for (u32 i = 0; i < DA_PATH_CHOICES; i++)
        paths[i] = GetMonData(DynamaxAdventure_GetPath(i), MON_DATA_SPECIES);
    EXPECT_NE(paths[0], paths[1]);
    EXPECT(DynamaxAdventure_Quit());
    EXPECT(DynamaxAdventure_Begin(42));
    for (u32 i = 0; i < DA_RENTAL_CHOICES; i++)
        EXPECT_EQ(GetMonData(DynamaxAdventure_GetRental(i), MON_DATA_SPECIES), rentals[i]);
    EXPECT(DynamaxAdventure_ChooseRental(1));
    EXPECT(!DynamaxAdventure_ChooseRental(2));
    for (u32 i = 0; i < DA_PATH_CHOICES; i++)
        EXPECT_EQ(GetMonData(DynamaxAdventure_GetPath(i), MON_DATA_SPECIES), paths[i]);
    EXPECT(DynamaxAdventure_Quit());
}

TEST("Adventure: battle isolation restores the entire original party and blocks bag and saving")
{
    struct Pokemon original[PARTY_SIZE];
    u8 count;
    ZeroPlayerPartyMons();
    CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][0], SPECIES_PIKACHU, 12, 0, OTID_STRUCT_PLAYER_ID, 8);
    CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][1], SPECIES_EEVEE, 15, 0, OTID_STRUCT_PLAYER_ID, 9);
    CalculatePlayerPartyCount();
    count = gPartiesCount[B_TRAINER_PLAYER];
    memcpy(original, gParties[B_TRAINER_PLAYER], sizeof(original));
    StartRental();
    EXPECT(DynamaxAdventure_ChoosePath(0));
    EXPECT(DynamaxAdventure_PrepareBattle());
    EXPECT(IsDynamaxAdventureBattle());
    EXPECT_EQ(gPartiesCount[B_TRAINER_PLAYER], 1);
    EXPECT_EQ(gPartiesCount[B_TRAINER_PARTNER], DA_TEAM_SIZE - 1);
    EXPECT_EQ(gPartiesCount[B_TRAINER_OPPONENT_A], 1);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PARTNER][0], MON_DATA_LEVEL), DA_RENTAL_LEVEL);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][1], MON_DATA_SPECIES), SPECIES_NONE);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_LEVEL), DA_RENTAL_LEVEL);
    EXPECT(!BriskCanUseBattleItem(ITEM_POTION));
    EXPECT(!BriskCanUseBattleItem(ITEM_MASTER_BALL));
    EXPECT_EQ(TrySavingData(SAVE_NORMAL), SAVE_STATUS_ERROR);
    EXPECT(!DynamaxAdventure_PrepareBattle());
    EXPECT(!DynamaxAdventure_Quit());
    EXPECT(!DynamaxAdventure_Claim(0));
    EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
    EXPECT_EQ(gPartiesCount[B_TRAINER_PLAYER], count);
    EXPECT_EQ(memcmp(original, gParties[B_TRAINER_PLAYER], sizeof(original)), 0);
    EXPECT(!IsDynamaxAdventureBattle());
    EXPECT(DynamaxAdventure_Quit());
}

TEST("Adventure: four losses end the run without whiteout or modifying the party")
{
    struct Pokemon original[PARTY_SIZE];
    memcpy(original, gParties[B_TRAINER_PLAYER], sizeof(original));
    StartRental();
    EXPECT(DynamaxAdventure_ChoosePath(1));
    for (u32 i = 0; i < DA_STARTING_LIVES; i++)
    {
        EXPECT(DynamaxAdventure_PrepareBattle());
        EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_LOST));
        EXPECT_EQ(DynamaxAdventure_GetStatus().lives, DA_STARTING_LIVES - i - 1);
        EXPECT_EQ(DynamaxAdventure_GetStatus().stage, 0);
    }
    EXPECT_EQ(DynamaxAdventure_GetStatus().phase, DA_PHASE_FINISHED);
    EXPECT_EQ(DynamaxAdventure_GetStatus().cleared, FALSE);
    EXPECT(!DynamaxAdventure_PrepareBattle());
    EXPECT(!DynamaxAdventure_Claim(0));
    EXPECT_EQ(memcmp(original, gParties[B_TRAINER_PLAYER], sizeof(original)), 0);
    EXPECT(DynamaxAdventure_Quit());
}

TEST("Adventure: four catches allow swaps and exactly one final reward")
{
    u16 species[DA_ENCOUNTERS];
    ZeroPlayerPartyMons();
    StartRental();
    for (u32 stage = 0; stage < DA_ENCOUNTERS; stage++)
    {
        EXPECT_EQ(DynamaxAdventure_GetStatus().stage, stage);
        species[stage] = GetMonData(DynamaxAdventure_GetPath(0), MON_DATA_SPECIES);
        EXPECT_EQ(GetMonData(DynamaxAdventure_GetPath(0), MON_DATA_LEVEL), stage == 3 ? DA_BOSS_LEVEL : DA_RENTAL_LEVEL);
        EXPECT(DynamaxAdventure_ChoosePath(0));
        EXPECT(DynamaxAdventure_PrepareBattle());
        EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
        EXPECT(!DynamaxAdventure_Claim(0));
        EXPECT(DynamaxAdventure_Catch(TRUE));
        EXPECT(!DynamaxAdventure_Catch(TRUE));
        if (stage < DA_ENCOUNTERS - 1)
        {
            EXPECT(DynamaxAdventure_Swap(TRUE));
            EXPECT(!DynamaxAdventure_Swap(TRUE));
        }
    }
    EXPECT_EQ(DynamaxAdventure_GetStatus().phase, DA_PHASE_FINISHED);
    EXPECT_EQ(DynamaxAdventure_GetStatus().caughtCount, DA_ENCOUNTERS);
    EXPECT(DynamaxAdventure_GetStatus().cleared);
    for (u32 i = 0; i < DA_ENCOUNTERS; i++)
        EXPECT_EQ(GetMonData(DynamaxAdventure_GetCaught(i), MON_DATA_SPECIES), species[i]);
    EXPECT(DynamaxAdventure_Claim(3));
    EXPECT_EQ(gPartiesCount[B_TRAINER_PLAYER], 1);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SPECIES), species[3]);
    EXPECT(!DynamaxAdventure_Claim(0));
    EXPECT_EQ(DynamaxAdventure_GetStatus().phase, DA_PHASE_INACTIVE);
}

TEST("Adventure: declining catches and swaps advances correctly and escape ends a run")
{
    StartRental();
    EXPECT(DynamaxAdventure_ChoosePath(0));
    EXPECT(DynamaxAdventure_PrepareBattle());
    EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
    EXPECT(DynamaxAdventure_Catch(FALSE));
    EXPECT_EQ(DynamaxAdventure_GetStatus().caughtCount, 0);
    EXPECT_EQ(DynamaxAdventure_GetStatus().stage, 1);
    EXPECT_EQ(DynamaxAdventure_GetStatus().phase, DA_PHASE_PATH);
    EXPECT(DynamaxAdventure_ChoosePath(0));
    EXPECT(DynamaxAdventure_PrepareBattle());
    EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
    EXPECT(DynamaxAdventure_Catch(TRUE));
    EXPECT(DynamaxAdventure_Swap(FALSE));
    EXPECT_EQ(DynamaxAdventure_GetStatus().stage, 2);
    EXPECT(DynamaxAdventure_ChoosePath(0));
    EXPECT(DynamaxAdventure_PrepareBattle());
    EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_RAN));
    EXPECT_EQ(DynamaxAdventure_GetStatus().phase, DA_PHASE_FINISHED);
    EXPECT_EQ(DynamaxAdventure_GetStatus().cleared, FALSE);
    EXPECT(DynamaxAdventure_Quit());
}

TEST("Adventure: script bridge reports state and rejects invalid operations")
{
    gSpecialVar_0x8004 = DA_OP_BEGIN;
    RunDynamaxAdventure();
    EXPECT_EQ(gSpecialVar_Result, TRUE);
    gSpecialVar_0x8004 = DA_OP_STATUS;
    RunDynamaxAdventure();
    EXPECT_EQ(gSpecialVar_0x8000, DA_PHASE_RENTAL);
    EXPECT_EQ(gSpecialVar_0x8002, DA_STARTING_LIVES);
    gSpecialVar_0x8004 = DA_OP_RENTAL_INFO;
    gSpecialVar_0x8005 = 0;
    RunDynamaxAdventure();
    EXPECT_EQ(gSpecialVar_Result, TRUE);
    EXPECT_NE(gSpecialVar_0x8000, SPECIES_NONE);
    EXPECT_EQ(gSpecialVar_0x8001, DA_RENTAL_LEVEL);
    gSpecialVar_0x8004 = 0xFFFF;
    RunDynamaxAdventure();
    EXPECT_EQ(gSpecialVar_Result, FALSE);
    EXPECT(DynamaxAdventure_Quit());
}

TEST("Adventure: reward menu identifies regular and shiny catches without revealing rentals")
{
    static const u8 shinyText[] = _("Shiny");
    static const u8 regularText[] = _("Regular");
    StartRental();
    EXPECT(DynamaxAdventure_ChoosePath(0));
    EXPECT(DynamaxAdventure_PrepareBattle());
    EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
    EXPECT(DynamaxAdventure_Catch(TRUE));
    EXPECT(DynamaxAdventure_Swap(FALSE));
    EXPECT(DynamaxAdventure_ChoosePath(0));
    EXPECT(DynamaxAdventure_PrepareBattle());
    EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_RAN));
    struct Pokemon *mon = DynamaxAdventure_GetCaught(0);
    for (u8 shiny = 0; shiny <= 1; shiny++)
    {
        SetMonData(mon, MON_DATA_IS_SHINY, &shiny);
        gSpecialVar_0x8004 = DA_OP_REWARD_INFO;
        gSpecialVar_0x8005 = 0;
        RunDynamaxAdventure();
        EXPECT_EQ(gSpecialVar_Result, TRUE);
        EXPECT_EQ(StringCompare(gStringVar2, shiny ? shinyText : regularText), 0);
        EXPECT_EQ(GetMonData(mon, MON_DATA_IS_SHINY), shiny);
    }
    EXPECT(DynamaxAdventure_Quit());
}

#include "battle_gimmick.h"
#include "battle_dynamax.h"
#include "test/battle.h"

TWO_VS_ONE_BATTLE_TEST("Adventure: player Mega and rival G-Max activate independently with a G-Max boss")
{
    GIVEN {
        StartRental();
        EXPECT(DynamaxAdventure_ChoosePath(1));
        EXPECT(DynamaxAdventure_PrepareBattle());
        gBattleTestRunnerState->data.recordedBattle.battleFlags &= ~BATTLE_TYPE_TRAINER;
        gBattleTestRunnerState->data.recordedBattle.partnerId = DynamaxAdventure_GetRivalPartnerId();
        gBattleTypeFlags &= ~BATTLE_TYPE_TRAINER;
        PLAYER(SPECIES_CHARIZARD) { Item(ITEM_CHARIZARDITE_Y); Moves(MOVE_CELEBRATE); }
        PARTNER(SPECIES_LAPRAS) { MaxHP(200); HP(200); Item(ITEM_LIGHT_CLAY); Moves(MOVE_CELEBRATE); GigantamaxFactor(TRUE); }
        OPPONENT(SPECIES_VENUSAUR) { MaxHP(200); HP(200); Moves(MOVE_CELEBRATE); GigantamaxFactor(TRUE); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_CELEBRATE, gimmick: GIMMICK_MEGA); MOVE(playerRight, MOVE_CELEBRATE, gimmick: GIMMICK_DYNAMAX); MOVE(opponentLeft, MOVE_CELEBRATE); }
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_CHARIZARD_MEGA_Y);
        EXPECT_EQ(playerRight->species, SPECIES_LAPRAS_GMAX);
        EXPECT_EQ(opponentLeft->species, SPECIES_VENUSAUR_GMAX);
        EXPECT_EQ(opponentLeft->maxHP, 1000);
        EXPECT_EQ(playerRight->maxHP, 200);
        EXPECT_EQ(GetBattlerCoordsIndex(B_BATTLER_1), BATTLE_COORDS_DOUBLES);
        // Returning from the party/summary screen destroys and recreates all
        // windows. The dedicated boss bar must own a fresh buffer afterward.
        CloseMainBattleScreen();
        ReshowBattleScreenAfterMenu();
        for (u32 frame = 0; frame < 100 && gMain.callback2 != BattleMainCB2; frame++)
            gMain.callback2();
        EXPECT(gMain.callback2 == BattleMainCB2);
        DynamaxAdventure_UpdateRaidHud();
        bool32 foundBossBar = FALSE;
        for (u32 window = 0; window < WINDOWS_MAX; window++)
            if (gWindows[window].tileData != NULL && gWindows[window].window.baseBlock == 0x50)
            {
                EXPECT_EQ(gWindows[window].window.width, 28);
                EXPECT_EQ(gWindows[window].window.height, 2);
                foundBossBar = TRUE;
            }
        EXPECT(foundBossBar);
        // Damage-controller calls cannot resurrect any of the three native box sprites.
        SetHealthboxSpriteVisible(gHealthboxSpriteIds[B_BATTLER_1]);
        UpdateOamPriorityInAllHealthboxes(1, TRUE);
        struct Sprite *box = &gSprites[gHealthboxSpriteIds[B_BATTLER_1]];
        EXPECT(box->invisible);
        EXPECT(gSprites[box->data[5]].invisible);
        EXPECT(gSprites[box->oam.affineParam].invisible);
        EXPECT(!CanThrowBall());
        LoadTypeIcons(B_BATTLER_0);
        EXPECT_EQ(GetSpriteTileStartByTag(TYPE_ICON_TAG), 0xFFFF);
        EXPECT_EQ(GetBattlerSpriteCoord(B_BATTLER_1, BATTLER_COORD_X), 176);
        EXPECT_EQ(GetBattlerSpriteCoord(B_BATTLER_1, BATTLER_COORD_Y), 68);

        BattleAI_SetupFlags();
        EXPECT(gAiThinkingStruct->aiFlags[B_BATTLER_2] & AI_FLAG_OMNISCIENT);
        EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
        gMain.inBattle = FALSE;
        EXPECT(DynamaxAdventure_Quit());
    }
}

AI_TWO_VS_ONE_BATTLE_TEST("Adventure: AI partner attacks the sole boss and neither rental gains EXP")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_BASIC_TRAINER | AI_FLAG_DOUBLE_BATTLE);
        StartRental();
        EXPECT(DynamaxAdventure_ChoosePath(0));
        EXPECT(DynamaxAdventure_PrepareBattle());
        // Exercise the real wild-partner path rather than a trainer encounter.
        gBattleTestRunnerState->data.recordedBattle.battleFlags &= ~BATTLE_TYPE_TRAINER;
        gBattleTestRunnerState->data.recordedBattle.partnerId = DynamaxAdventure_GetRivalPartnerId();
        gBattleTypeFlags &= ~BATTLE_TYPE_TRAINER;
        PLAYER(SPECIES_WOBBUFFET) { Level(65); Speed(201); Moves(MOVE_CELEBRATE); }
        PARTNER(SPECIES_ZACIAN) { Level(65); Speed(200); Moves(MOVE_DRAGON_CLAW); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); Speed(100); Moves(MOVE_SCRATCH); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            EXPECT_MOVE(playerRight, MOVE_DRAGON_CLAW, target: opponentLeft);
        }
    } SCENE {
        MESSAGE("Wobbuffet used Celebrate!");
        MESSAGE("Zacian used Dragon Claw!");
        MESSAGE("The wild Wobbuffet fainted!");
    } THEN {
        EXPECT(gAbsentBattlerFlags & (1u << B_BATTLER_3));
        EXPECT_EQ(gBattlerBattleController[B_BATTLER_2], BATTLE_CONTROLLER_PLAYER_PARTNER);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_EXP), gExperienceTables[gSpeciesInfo[SPECIES_WOBBUFFET].growthRate][65]);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PARTNER][0], MON_DATA_EXP), gExperienceTables[gSpeciesInfo[SPECIES_ZACIAN].growthRate][65]);
        EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
        gMain.inBattle = FALSE;
        EXPECT(DynamaxAdventure_Quit());
    }
}

TEST("Adventure: full storage preserves the reward until space is available")
{
    struct Pokemon filler;
    CreateMonWithIVs(&filler, SPECIES_PIKACHU, 10, 0, OTID_STRUCT_PLAYER_ID, 10);
    for (u32 i = 0; i < PARTY_SIZE; i++)
        gParties[B_TRAINER_PLAYER][i] = filler;
    CalculatePlayerPartyCount();
    for (u32 box = 0; box < TOTAL_BOXES_COUNT; box++)
        for (u32 slot = 0; slot < IN_BOX_COUNT; slot++)
            *GetBoxedMonPtr(box, slot) = filler.box;
    StartRental();
    EXPECT(DynamaxAdventure_ChoosePath(0));
    EXPECT(DynamaxAdventure_PrepareBattle());
    EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
    EXPECT(DynamaxAdventure_Catch(TRUE));
    EXPECT(DynamaxAdventure_Swap(FALSE));
    EXPECT(DynamaxAdventure_ChoosePath(0));
    EXPECT(DynamaxAdventure_PrepareBattle());
    EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_RAN));
    EXPECT(!DynamaxAdventure_Claim(0));
    EXPECT_EQ(DynamaxAdventure_GetStatus().phase, DA_PHASE_FINISHED);
    EXPECT_EQ(DynamaxAdventure_GetStatus().caughtCount, 1);
    ZeroBoxMonData(GetBoxedMonPtr(0, 0));
    EXPECT(DynamaxAdventure_Claim(0));
    EXPECT_EQ(DynamaxAdventure_GetStatus().phase, DA_PHASE_INACTIVE);
    EXPECT_NE(GetBoxMonData(GetBoxedMonPtr(0, 0), MON_DATA_SPECIES), SPECIES_NONE);
    for (u32 box = 0; box < TOTAL_BOXES_COUNT; box++)
        for (u32 slot = 0; slot < IN_BOX_COUNT; slot++)
            ZeroBoxMonData(GetBoxedMonPtr(box, slot));
}

TEST("Adventure: every canonical legendary and mythical is in the boss pool")
{
    u32 count = DynamaxAdventure_GetBossPoolCount();
    bool8 seen[NUM_SPECIES] = {0};
    EXPECT_GE(count, 90);
    for (u32 i = 0; i < count; i++)
    {
        enum Species species = DynamaxAdventure_GetBossPoolSpecies(i);
        EXPECT_NE(species, SPECIES_NONE);
        EXPECT(!seen[species]);
        seen[species] = TRUE;
        EXPECT(!gSpeciesInfo[species].isMegaEvolution);
        EXPECT(!gSpeciesInfo[species].isPrimalReversion);
        EXPECT(!gSpeciesInfo[species].isGigantamax);
        EXPECT(!gSpeciesInfo[species].isTeraForm);
    }
    for (enum Species species = 1; species < NUM_SPECIES; species++)
        if (IsSpeciesEnabled(species) && GET_BASE_SPECIES_ID(species) == species
         && (gSpeciesInfo[species].isRestrictedLegendary || gSpeciesInfo[species].isSubLegendary || gSpeciesInfo[species].isMythical))
            EXPECT(seen[species]);
    EXPECT(seen[SPECIES_ARTICUNO_GALAR]);
    EXPECT(seen[SPECIES_ZAPDOS_GALAR]);
    EXPECT(seen[SPECIES_MOLTRES_GALAR]);
    EXPECT(seen[SPECIES_MEW]);
    EXPECT(seen[SPECIES_MEWTWO]);
    EXPECT(seen[SPECIES_ARCEUS]);
    EXPECT(seen[SPECIES_ZACIAN]);
    EXPECT(seen[SPECIES_ETERNATUS]);
    EXPECT(seen[SPECIES_KORAIDON]);
    EXPECT(seen[SPECIES_MIRAIDON]);
    EXPECT(seen[SPECIES_TERAPAGOS]);
    EXPECT(seen[SPECIES_PECHARUNT]);
    EXPECT(!seen[SPECIES_PIKACHU]);
    EXPECT(!seen[SPECIES_MEWTWO_MEGA_X]);
    EXPECT_EQ(DynamaxAdventure_GetBossPoolSpecies(count), SPECIES_NONE);
    Test_MgbaPrintf("Adventure boss pool: %d species", count);
}

TEST("Adventure: competitive rental pool has level 70 moves abilities and full training")
{
    bool8 seen[NUM_SPECIES] = {0};
    u32 found = 0;
    u32 count = DynamaxAdventure_GetRentalPoolCount();
    EXPECT_GE(count, 30);
    for (u32 seed = 0; seed < 1024 && found < count; seed++)
    {
        EXPECT(DynamaxAdventure_Begin(seed));
        for (u32 i = 0; i < DA_RENTAL_CHOICES; i++)
        {
            struct Pokemon *mon = DynamaxAdventure_GetRental(i);
            enum Species species = GetMonData(mon, MON_DATA_SPECIES);
            EXPECT_EQ(GetMonData(mon, MON_DATA_LEVEL), DA_RENTAL_LEVEL);
            EXPECT_NE(GetMonData(mon, MON_DATA_HELD_ITEM), ITEM_NONE);
            u32 evs = 0;
            for (u32 stat = 0; stat < NUM_STATS; stat++)
            {
                u32 physical = 0, special = 0;
                for (u32 slot = 0; slot < MAX_MON_MOVES; slot++)
                {
                    enum DamageCategory category = GetMoveCategory(GetMonData(mon, MON_DATA_MOVE1 + slot));
                    physical += category == DAMAGE_CATEGORY_PHYSICAL;
                    special += category == DAMAGE_CATEGORY_SPECIAL;
                }
                u32 expected = (stat == STAT_SPEED && GetSpeciesBaseSpeed(species) <= 60)
                    || (stat == STAT_ATK && physical == 0 && special != 0) ? 0 : 31;
                EXPECT_EQ(GetMonData(mon, MON_DATA_HP_IV + stat), expected);
                evs += GetMonData(mon, MON_DATA_HP_EV + stat);
            }
            EXPECT_EQ(evs, 508);
            for (u32 slot = 0; slot < MAX_MON_MOVES; slot++)
                EXPECT_NE(GetMonData(mon, MON_DATA_MOVE1 + slot), MOVE_NONE);
            switch (species)
            {
            case SPECIES_INCINEROAR: EXPECT_EQ(GetMonAbility(mon), ABILITY_INTIMIDATE); break;
            case SPECIES_RILLABOOM: EXPECT_EQ(GetMonAbility(mon), ABILITY_GRASSY_SURGE); break;
            case SPECIES_AMOONGUSS: EXPECT_EQ(GetMonAbility(mon), ABILITY_REGENERATOR); break;
            case SPECIES_WHIMSICOTT: EXPECT_EQ(GetMonAbility(mon), ABILITY_PRANKSTER); break;
            case SPECIES_GHOLDENGO: EXPECT_EQ(GetMonAbility(mon), ABILITY_GOOD_AS_GOLD); break;
            case SPECIES_AZUMARILL: EXPECT_EQ(GetMonAbility(mon), ABILITY_HUGE_POWER); break;
            default: break;
            }
            if (!seen[species]) { found++; seen[species] = TRUE; }
        }
        EXPECT(DynamaxAdventure_Quit());
    }
    EXPECT_EQ(found, count);
    for (u32 i = 0; i < count; i++)
        EXPECT(seen[DynamaxAdventure_GetRentalPoolSpecies(i)]);
    EXPECT_EQ(DynamaxAdventure_GetRentalPoolSpecies(count), SPECIES_NONE);
}

TEST("Adventure: only the player rental can be swapped and a fallen rental is revived")
{
    StartRental();
    EXPECT(DynamaxAdventure_ChoosePath(0));
    EXPECT(DynamaxAdventure_PrepareBattle());
    u16 first = GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SPECIES);
    u16 encounter = GetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_SPECIES);
    u16 hp = 0;
    SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP, &hp);
    EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
    EXPECT(DynamaxAdventure_Catch(TRUE));
    EXPECT(!DynamaxAdventure_SwapRental(DA_TEAM_SIZE));
    EXPECT(!DynamaxAdventure_SwapRental(1));
    EXPECT(DynamaxAdventure_SwapRental(0));
    EXPECT(DynamaxAdventure_ChoosePath(0));
    EXPECT(DynamaxAdventure_PrepareBattle());
    EXPECT_NE(first, SPECIES_NONE);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SPECIES), encounter);
    EXPECT_GT(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP), 0);
    EXPECT_NE(GetMonData(&gParties[B_TRAINER_PARTNER][0], MON_DATA_SPECIES), SPECIES_NONE);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PARTNER][0], MON_DATA_HP), GetMonData(&gParties[B_TRAINER_PARTNER][0], MON_DATA_MAX_HP));
    EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_RAN));
    EXPECT(DynamaxAdventure_Quit());
}

TEST("Adventure: every boss generates at level 100 with usable attacks")
{
    EXPECT(DynamaxAdventure_Begin(17));
    for (u32 index = 0; index < DynamaxAdventure_GetBossPoolCount(); index++)
    {
        struct Pokemon mon;
        enum Species species = DynamaxAdventure_GetBossPoolSpecies(index);
        EXPECT(Test_DynamaxAdventure_CreateBoss(&mon, species));
        EXPECT_EQ(GetMonData(&mon, MON_DATA_SPECIES), species);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_LEVEL), 100);
        u32 attacks = 0;
        for (u32 slot = 0; slot < MAX_MON_MOVES; slot++)
        {
            enum Move move = GetMonData(&mon, MON_DATA_MOVE1 + slot);
            if (move != MOVE_NONE && GetMoveCategory(move) != DAMAGE_CATEGORY_STATUS)
                attacks++;
        }
        if (attacks < 3) Test_MgbaPrintf("Boss lacking attacks: %d", species);
        EXPECT_GE(attacks, 3);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_HP), GetMonData(&mon, MON_DATA_MAX_HP));
    }
    EXPECT(DynamaxAdventure_Quit());
}

TEST("Adventure: rival identity follows player gender and has a real partner back sprite")
{
    u8 gender = gSaveBlock2Ptr->playerGender;
    gSaveBlock2Ptr->playerGender = MALE;
    EXPECT_EQ(DynamaxAdventure_GetRivalPartnerId(), TRAINER_PARTNER(PARTNER_MAY));
    const struct Trainer *may = GetTrainerStructFromId(DynamaxAdventure_GetRivalPartnerId());
    EXPECT_EQ(may->trainerPic, TRAINER_PIC_MAY);
    EXPECT(gTrainerPicInfo[may->trainerPic].backPic != NULL);
    gSaveBlock2Ptr->playerGender = FEMALE;
    EXPECT_EQ(DynamaxAdventure_GetRivalPartnerId(), TRAINER_PARTNER(PARTNER_BRENDAN));
    const struct Trainer *brendan = GetTrainerStructFromId(DynamaxAdventure_GetRivalPartnerId());
    EXPECT_EQ(brendan->trainerPic, TRAINER_PIC_BRENDAN);
    EXPECT(gTrainerPicInfo[brendan->trainerPic].backPic != NULL);
    gSaveBlock2Ptr->playerGender = gender;
}

TEST("Adventure: previous partner party identity and opponent context are restored")
{
    struct Pokemon original[PARTY_SIZE];
    ZeroPartyMons(gParties[B_TRAINER_PARTNER]);
    CreateMonWithIVs(&gParties[B_TRAINER_PARTNER][0], SPECIES_EEVEE, 23, 0, OTID_STRUCT_PLAYER_ID, 9);
    memcpy(original, gParties[B_TRAINER_PARTNER], sizeof(original));
    gPartiesCount[B_TRAINER_PARTNER] = 1;
    gPartnerTrainerId = TRAINER_PARTNER(PARTNER_STEVEN);
    TRAINER_BATTLE_PARAM.opponentB = 123;
    StartRental();
    EXPECT(DynamaxAdventure_ChoosePath(0));
    EXPECT(DynamaxAdventure_PrepareBattle());
    EXPECT_EQ(gPartiesCount[B_TRAINER_PLAYER], 1);
    EXPECT_EQ(gPartiesCount[B_TRAINER_PARTNER], DA_TEAM_SIZE - 1);
    EXPECT_EQ(gPartiesCount[B_TRAINER_OPPONENT_A], 1);
    EXPECT_EQ(gPartnerTrainerId, DynamaxAdventure_GetRivalPartnerId());
    EXPECT_EQ(TRAINER_BATTLE_PARAM.opponentB, 0xFFFF);
    EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_LOST));
    EXPECT_EQ(memcmp(original, gParties[B_TRAINER_PARTNER], sizeof(original)), 0);
    EXPECT_EQ(gPartiesCount[B_TRAINER_PARTNER], 1);
    EXPECT_EQ(gPartnerTrainerId, TRAINER_PARTNER(PARTNER_STEVEN));
    EXPECT_EQ(TRAINER_BATTLE_PARAM.opponentB, 123);
    EXPECT(DynamaxAdventure_Quit());
}

TEST("Adventure: route branches alternate stone-bearing Megas and beneficial-item G-Max encounters")
{
    StartRental();
    struct Pokemon *mega = DynamaxAdventure_GetPath(0), *gmax = DynamaxAdventure_GetPath(1);
    EXPECT_NE(GetMonData(mega, MON_DATA_HELD_ITEM), ITEM_NONE);
    EXPECT_EQ(GetMonData(mega, MON_DATA_GIGANTAMAX_FACTOR), FALSE);
    EXPECT_EQ(GetMonData(gmax, MON_DATA_GIGANTAMAX_FACTOR), TRUE);
    EXPECT_NE(GetMonData(gmax, MON_DATA_HELD_ITEM), ITEM_NONE);
    EXPECT(DynamaxAdventure_Quit());
}

extern const u8 DynamaxAdventureCave_EventScript_Scientist[];

TEST("Adventure: Route 146 cave has walkable reciprocal warps and an unconditional scientist")
{
    const struct MapHeader *route = Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(MAP_ROUTE146), MAP_NUM(MAP_ROUTE146));
    const struct MapHeader *cave = Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(MAP_DYNAMAX_ADVENTURE_CAVE), MAP_NUM(MAP_DYNAMAX_ADVENTURE_CAVE));
    EXPECT_EQ(route->events->warpCount, 1);
    EXPECT_EQ(cave->events->warpCount, 1);
    const struct WarpEvent *entrance = &route->events->warps[0];
    const struct WarpEvent *exit = &cave->events->warps[0];
    EXPECT_EQ(entrance->mapGroup, MAP_GROUP(MAP_DYNAMAX_ADVENTURE_CAVE));
    EXPECT_EQ(entrance->mapNum, MAP_NUM(MAP_DYNAMAX_ADVENTURE_CAVE));
    EXPECT_EQ(entrance->warpId, 0);
    EXPECT_EQ(exit->mapGroup, MAP_GROUP(MAP_ROUTE146));
    EXPECT_EQ(exit->mapNum, MAP_NUM(MAP_ROUTE146));
    EXPECT_EQ(exit->warpId, 0);
    EXPECT_EQ((route->mapLayout->map[entrance->y * route->mapLayout->width + entrance->x] >> 10) & 3, 0);
    EXPECT_EQ((cave->mapLayout->map[exit->y * cave->mapLayout->width + exit->x] >> 10) & 3, 0);
    EXPECT_EQ(cave->events->objectEventCount, 3);
    EXPECT_EQ(cave->events->objectEvents[0].graphicsId, OBJ_EVENT_GFX_SCIENTIST_1);
    EXPECT_EQ(cave->events->objectEvents[0].flagId, 0);
    EXPECT_EQ(cave->events->objectEvents[0].script, DynamaxAdventureCave_EventScript_Scientist);
}

TEST("Adventure: automatic rival choice is distinct and intermediate/final HP boosts stay off catches")
{
    EXPECT(DynamaxAdventure_Begin(73));
    u16 player = GetMonData(DynamaxAdventure_GetRental(2), MON_DATA_SPECIES);
    EXPECT(DynamaxAdventure_ChooseRental(2));
    EXPECT_EQ(DynamaxAdventure_GetStatus().phase, DA_PHASE_PATH);
    for (u32 stage = 0; stage < DA_ENCOUNTERS; stage++)
    {
        EXPECT(DynamaxAdventure_ChoosePath(0));
        EXPECT(DynamaxAdventure_PrepareBattle());
        if (stage == 0)
        {
            EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SPECIES), player);
            EXPECT_NE(GetMonData(&gParties[B_TRAINER_PARTNER][0], MON_DATA_SPECIES), player);
        }
        struct Pokemon *boss = &gParties[B_TRAINER_OPPONENT_A][0];
        u16 base = GetMonData(boss, MON_DATA_MAX_HP);
        u32 scale = stage == DA_ENCOUNTERS - 1 ? 7 : 5;
        EXPECT_EQ(DynamaxAdventure_GetBossHPMultiplier(), scale);
        ApplyDynamaxHPMultiplier(boss);
        EXPECT_EQ(GetMonData(boss, MON_DATA_MAX_HP), base * scale);
        EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
        EXPECT(DynamaxAdventure_Catch(TRUE));
        EXPECT_EQ(GetMonData(DynamaxAdventure_GetCaught(stage), MON_DATA_MAX_HP), base);
        if (stage < DA_ENCOUNTERS - 1)
            EXPECT(DynamaxAdventure_Swap(FALSE));
    }
    EXPECT(DynamaxAdventure_Quit());
    EXPECT_EQ(DynamaxAdventure_GetBossHPMultiplier(), 1);
}

TEST("Adventure: staging chamber has no warps, blocks escape and contains the rival and scientist")
{
    const struct MapHeader *room = Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(MAP_DYNAMAX_ADVENTURE_REST_CAVE), MAP_NUM(MAP_DYNAMAX_ADVENTURE_REST_CAVE));
    EXPECT_EQ(room->events->warpCount, 0);
    EXPECT(!room->allowEscaping);
    EXPECT_EQ(room->events->objectEventCount, 2);
    EXPECT_EQ(room->events->objectEvents[0].graphicsId, OBJ_EVENT_GFX_SCIENTIST_1);
    EXPECT_EQ(room->events->objectEvents[1].graphicsId, OBJ_EVENT_GFX_VAR_0);
    EXPECT_NE((room->mapLayout->map[3 * room->mapLayout->width + 7] >> 10) & 3, 0);
}

TWO_VS_ONE_BATTLE_TEST("Adventure: rival continues fighting after the player's rental faints")
{
    GIVEN {
        WITH_CONFIG(B_MULTI_BATTLE_WHITEOUT, GEN_3);
        StartRental();
        EXPECT(DynamaxAdventure_ChoosePath(0));
        EXPECT(DynamaxAdventure_PrepareBattle());
        gBattleTestRunnerState->data.recordedBattle.battleFlags &= ~BATTLE_TYPE_TRAINER;
        gBattleTestRunnerState->data.recordedBattle.partnerId = DynamaxAdventure_GetRivalPartnerId();
        gBattleTypeFlags &= ~BATTLE_TYPE_TRAINER;
        PLAYER(SPECIES_RATTATA) { Moves(MOVE_CELEBRATE); Speed(200); MaxHP(100); HP(1); Status1(STATUS1_POISON); }
        PARTNER(SPECIES_GARCHOMP) { Moves(MOVE_CELEBRATE, MOVE_DRAGON_CLAW); Attack(999); Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE, MOVE_TACKLE); MaxHP(100); HP(100); Defense(1); Speed(50); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_CELEBRATE); MOVE(playerRight, MOVE_CELEBRATE); MOVE(opponentLeft, MOVE_CELEBRATE); }
        TURN { MOVE(playerRight, MOVE_DRAGON_CLAW, target: opponentLeft); MOVE(opponentLeft, MOVE_TACKLE, target: playerRight); }
    } SCENE {
        MESSAGE("Rattata fainted!");
        MESSAGE("Garchomp used Dragon Claw!");
        MESSAGE("The wild Wobbuffet fainted!");
    } THEN {
        EXPECT_EQ(gBattleOutcome, B_OUTCOME_WON);
        EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
        gMain.inBattle = FALSE;
        EXPECT(DynamaxAdventure_Quit());
    }
}

TEST("Adventure: hint names only the final boss type")
{
    u8 hint[32];
    EXPECT(DynamaxAdventure_Begin(501));
    gSpecialVar_0x8004 = DA_OP_BOSS_HINT;
    RunDynamaxAdventure();
    EXPECT_EQ(gSpecialVar_Result, TRUE);
    StringCopy(hint, gStringVar1);
    EXPECT(DynamaxAdventure_ChooseRental(0));
    for (u32 stage = 0; stage < DA_ENCOUNTERS - 1; stage++)
    {
        EXPECT(DynamaxAdventure_ChoosePath(0));
        EXPECT(DynamaxAdventure_PrepareBattle());
        EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
        EXPECT(DynamaxAdventure_Catch(FALSE));
    }
    u32 species = GetMonData(DynamaxAdventure_GetPath(0), MON_DATA_SPECIES);
    EXPECT_EQ(StringCompare(hint, gTypesInfo[gSpeciesInfo[species].types[0]].name), 0);
    EXPECT(DynamaxAdventure_Quit());
}

static void ExpectAdventureIconRows(void)
{
    for (u32 row = 0; row < 6; row++)
    {
        u32 tile = GetSpriteTileStartByTag(0xDA10 + row);
        EXPECT_NE(tile, 0xFFFF);
        const u8 *actual = (const u8 *)(OBJ_VRAM0 + tile * TILE_SIZE_4BPP);
        struct Pokemon *mon = DynamaxAdventure_GetRental(gScrollableMultichoice_ScrollOffset + row);
        const u8 *tiles = GetMonIconTilesIsEgg(GetMonData(mon, MON_DATA_SPECIES), 0, FALSE);
        for (u32 py = 0; py < 16; py++)
            for (u32 px = 0; px < 16; px++)
            {
                u32 x = px * 2, y = py * 2;
                u32 src = ((y / 8) * 4 + x / 8) * 32 + (y % 8) * 4 + (x % 8) / 2;
                u32 dst = ((py / 8) * 2 + px / 8) * 32 + (py % 8) * 4 + (px % 8) / 2;
                EXPECT_EQ((actual[dst] >> ((px & 1) * 4)) & 15, (tiles[src] >> ((x & 1) * 4)) & 15);
            }
    }
}

TEST("Adventure: six rental rows keep every icon synchronized scrolling both directions and release graphics")
{
    static const struct WindowTemplate windows[] = {DUMMY_WIN_TEMPLATE};
    static const u8 name[] = _("Rental");
    EXPECT(DynamaxAdventure_Begin(42));
    ResetTasks(); ResetSpriteData(); FreeAllSpritePalettes(); InitWindows(windows);
    struct ListMenuItem *items = AllocZeroed(DA_RENTAL_CHOICES * sizeof(*items));
    for (u32 i = 0; i < DA_RENTAL_CHOICES; i++)
    {
        u8 *text = Alloc(sizeof(name));
        StringCopy(text, name);
        items[i].name = text;
        items[i].id = i;
    }
    EXPECT(ScriptMenu_MultichoiceDynamic(1, 1, DA_RENTAL_CHOICES, items, FALSE, 3, 0, DYN_MULTICHOICE_CB_ADVENTURE_PKMN));
    ExpectAdventureIconRows();
    for (u32 direction = 0; direction < 2; direction++)
    {
        for (u32 step = 0; step < DA_RENTAL_CHOICES - 1; step++)
        {
            gMain.newKeys = gMain.newAndRepeatedKeys = direction == 0 ? DPAD_DOWN : DPAD_UP;
            RunTasks();
            ExpectAdventureIconRows();
        }
        EXPECT_EQ(gScrollableMultichoice_ScrollOffset, direction == 0 ? DA_RENTAL_CHOICES - 6 : 0);
    }
    gMain.newKeys = gMain.newAndRepeatedKeys = B_BUTTON;
    RunTasks();
    gMain.newKeys = gMain.newAndRepeatedKeys = 0;
    EXPECT_EQ(gSpecialVar_Result, MULTI_B_PRESSED);
    for (u32 i = 0; i < 6; i++)
    {
        EXPECT_EQ(GetSpriteTileStartByTag(0xDA10 + i), 0xFFFF);
        EXPECT_EQ(IndexOfSpritePaletteTag(0xDA10 + i), 0xFF);
    }
    FreeAllWindowBuffers();
    EXPECT(DynamaxAdventure_Quit());
}

TEST("Adventure: difficulty scales both rental HP totals without changing the real party or catch stats")
{
    u32 modes[] = {BRISK_MODE_EASY, BRISK_MODE_NORMAL, BRISK_MODE_HARD, BRISK_MODE_EXTREME};
    u32 numerator[] = {4, 2, 1, 1}, denominator[] = {1, 1, 1, 2};
    u32 savedMode = VarGet(VAR_BRISK_DIFFICULTY);
    for (u32 i = 0; i < ARRAY_COUNT(modes); i++)
    {
        VarSet(VAR_BRISK_DIFFICULTY, modes[i]);
        EXPECT(DynamaxAdventure_Begin(17));
        u32 base = GetMonData(DynamaxAdventure_GetRental(0), MON_DATA_MAX_HP);
        EXPECT(DynamaxAdventure_ChooseRental(0));
        EXPECT(DynamaxAdventure_ChoosePath(0));
        EXPECT(DynamaxAdventure_PrepareBattle());
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_MAX_HP), max(1, base * numerator[i] / denominator[i]));
        struct Pokemon unscaled = gParties[B_TRAINER_PARTNER][0];
        CalculateMonStats(&unscaled);
        u32 partnerBase = GetMonData(&unscaled, MON_DATA_MAX_HP);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PARTNER][0], MON_DATA_MAX_HP), max(1, partnerBase * numerator[i] / denominator[i]));
        EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_RAN));
        EXPECT(DynamaxAdventure_Quit());
    }
    VarSet(VAR_BRISK_DIFFICULTY, savedMode);
}

TEST("Adventure: four-player foundation collects one command per living player and rejects stale rounds")
{
    struct AdventureCoopSession state;
    struct Pokemon rentals[DA_COOP_PLAYERS], boss;
    struct AdventureCoopAction actions[DA_COOP_PLAYERS];
    u32 count;
    CreateMonWithIVs(&boss, SPECIES_CHARIZARD, 100, 1, OTID_STRUCT_PLAYER_ID, 31);
    for (u32 i = 0; i < DA_COOP_PLAYERS; i++)
    {
        rentals[i] = boss;
        SetMonMoveSlot(&rentals[i], MOVE_FLAMETHROWER, 0);
    }
    EXPECT(!DA_COOP_AVAILABLE);
    EXPECT(AdventureCoop_Init(&state, rentals, &boss));
    for (u32 i = 0; i < DA_COOP_PLAYERS; i++)
    {
        EXPECT(!AdventureCoop_CollectRound(&state, actions, &count));
        EXPECT(AdventureCoop_Submit(&state, 1, i, 0, DA_COOP_TARGET_BOSS));
        EXPECT(!AdventureCoop_Submit(&state, 1, i, 0, DA_COOP_TARGET_BOSS));
    }
    EXPECT(AdventureCoop_CollectRound(&state, actions, &count));
    EXPECT_EQ(count, DA_COOP_PLAYERS);
    EXPECT_EQ(state.round, 2);
    EXPECT(!AdventureCoop_Submit(&state, 1, 0, 0, DA_COOP_TARGET_BOSS));
    u16 zero = 0;
    SetMonData(&state.rentals[3], MON_DATA_HP, &zero);
    for (u32 i = 0; i < 3; i++)
        EXPECT(AdventureCoop_Submit(&state, 2, i, 0, DA_COOP_TARGET_BOSS));
    EXPECT(!AdventureCoop_Submit(&state, 2, 3, 0, DA_COOP_TARGET_BOSS));
    EXPECT(AdventureCoop_CollectRound(&state, actions, &count));
    EXPECT_EQ(count, 3);
}

TWO_VS_ONE_BATTLE_TEST("Adventure: Mega enemies retain boosted HP through their actual form change")
{
    GIVEN {
        StartRental();
        EXPECT(DynamaxAdventure_ChoosePath(0));
        EXPECT(DynamaxAdventure_PrepareBattle());
        gBattleTestRunnerState->data.recordedBattle.battleFlags &= ~BATTLE_TYPE_TRAINER;
        gBattleTestRunnerState->data.recordedBattle.partnerId = DynamaxAdventure_GetRivalPartnerId();
        gBattleTypeFlags &= ~BATTLE_TYPE_TRAINER;
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
        PARTNER(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_GARCHOMP) { Item(ITEM_GARCHOMPITE); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_CELEBRATE); MOVE(playerRight, MOVE_CELEBRATE); MOVE(opponentLeft, MOVE_CELEBRATE); }
    } SCENE {
        MESSAGE("The wild Garchomp's Garchompite is radiating energy!");
    } THEN {
        EXPECT_EQ(opponentLeft->species, SPECIES_GARCHOMP_MEGA);
        struct Pokemon base = *GetBattlerMon(B_BATTLER_1);
        CalculateMonStats(&base);
        EXPECT_EQ(opponentLeft->maxHP, GetMonData(&base, MON_DATA_MAX_HP) * 5);
        EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
        gMain.inBattle = FALSE;
        EXPECT(DynamaxAdventure_Quit());
    }
}

TEST("Adventure: type-only route hints use the battle form and shiny rolls survive catches")
{
    bool32 sawShiny = FALSE;
    for (u32 seed = 0; seed < 4096 && !sawShiny; seed++)
    {
        EXPECT(DynamaxAdventure_Begin(seed));
        EXPECT(DynamaxAdventure_ChooseRental(0));
        struct Pokemon *mon = DynamaxAdventure_GetPath(0);
        u32 shiny = GetMonData(mon, MON_DATA_IS_SHINY);
        enum Species species = GetMonData(mon, MON_DATA_SPECIES);
        u32 item = GetMonData(mon, MON_DATA_HELD_ITEM);
        const struct FormChange *forms = GetSpeciesFormChanges(species);
        if (forms != NULL)
            for (u32 i = 0; forms[i].method != FORM_CHANGE_TERMINATOR; i++)
                if (forms[i].method == FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM && forms[i].param1 == item)
                { species = forms[i].targetSpecies; break; }
        gSpecialVar_0x8004 = DA_OP_PATH_INFO;
        gSpecialVar_0x8005 = 0;
        RunDynamaxAdventure();
        EXPECT_EQ(gSpecialVar_Result, TRUE);
        u8 expected[64];
        StringCopy(expected, gTypesInfo[gSpeciesInfo[species].types[0]].name);
        EXPECT_EQ(StringCompare(expected, gStringVar1), 0);
        EXPECT(DynamaxAdventure_ChoosePath(0));
        EXPECT(DynamaxAdventure_PrepareBattle());
        EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
        EXPECT(DynamaxAdventure_Catch(TRUE));
        EXPECT_EQ(GetMonData(DynamaxAdventure_GetCaught(0), MON_DATA_IS_SHINY), shiny);
        sawShiny |= shiny;
        EXPECT(DynamaxAdventure_Quit());
    }
    EXPECT(sawShiny);
}

TEST("Adventure: rental forms abilities and consumed held items reset between encounters")
{
    StartRental();
    EXPECT(DynamaxAdventure_ChoosePath(0));
    EXPECT(DynamaxAdventure_PrepareBattle());
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][0];
    u16 base = GetMonData(mon, MON_DATA_SPECIES), item = GetMonData(mon, MON_DATA_HELD_ITEM);
    u8 ability = GetMonData(mon, MON_DATA_ABILITY_NUM), zero = 0;
    enum Species form = GetGMaxTargetSpecies(base);
    const struct FormChange *forms = GetSpeciesFormChanges(base);
    for (u32 i = 0; forms != NULL && forms[i].method != FORM_CHANGE_TERMINATOR; i++)
        if (forms[i].method == FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM && forms[i].param1 == item)
        { form = forms[i].targetSpecies; break; }
    EXPECT_NE(form, base);
    SetMonData(mon, MON_DATA_SPECIES, &form);
    u16 none = ITEM_NONE;
    SetMonData(mon, MON_DATA_HELD_ITEM, &none);
    SetMonData(mon, MON_DATA_ABILITY_NUM, &zero);
    EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
    EXPECT(DynamaxAdventure_Catch(FALSE));
    EXPECT(DynamaxAdventure_ChoosePath(0));
    EXPECT(DynamaxAdventure_PrepareBattle());
    EXPECT_EQ(GetMonData(mon, MON_DATA_SPECIES), base);
    EXPECT_EQ(GetMonData(mon, MON_DATA_HELD_ITEM), item);
    EXPECT_EQ(GetMonData(mon, MON_DATA_ABILITY_NUM), ability);
    EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_RAN));
    EXPECT(DynamaxAdventure_Quit());
}

TEST("Adventure: both teammates recover HP and PP according to the starting difficulty")
{
    u32 savedMode = VarGet(VAR_BRISK_DIFFICULTY);
    for (u32 mode = BRISK_MODE_EASY; mode <= BRISK_MODE_EXTREME; mode++)
    {
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        StartRental();
        EXPECT(DynamaxAdventure_ChoosePath(0));
        EXPECT(DynamaxAdventure_PrepareBattle());
        EXPECT_EQ(gPartiesCount[B_TRAINER_PARTNER], 1);
        u16 maxima[DA_TEAM_SIZE];
        for (u32 i = 0; i < DA_TEAM_SIZE; i++)
        {
            struct Pokemon *mon = i == 0 ? &gParties[B_TRAINER_PLAYER][0] : &gParties[B_TRAINER_PARTNER][i - 1];
            maxima[i] = GetMonData(mon, MON_DATA_MAX_HP);
            u16 hp = 1;
            u32 status = STATUS1_POISON;
            u8 pp = 0;
            SetMonData(mon, MON_DATA_HP, &hp);
            SetMonData(mon, MON_DATA_STATUS, &status);
            SetMonData(mon, MON_DATA_PP1, &pp);
            for (u32 j = 0; j < i; j++)
            {
                struct Pokemon *other = j == 0 ? &gParties[B_TRAINER_PLAYER][0] : &gParties[B_TRAINER_PARTNER][j - 1];
                EXPECT_NE(GetMonData(mon, MON_DATA_SPECIES), GetMonData(other, MON_DATA_SPECIES));
            }
        }
        EXPECT_EQ(DynamaxAdventure_GetBossDamageMultiplier(), mode == BRISK_MODE_EXTREME ? 2 : 1);
        EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
        EXPECT(DynamaxAdventure_Catch(FALSE));
        EXPECT(DynamaxAdventure_ChoosePath(0));
        EXPECT(DynamaxAdventure_PrepareBattle());
        // Both teammates keep their own rentals across rounds.
        for (u32 i = 0; i < DA_TEAM_SIZE; i++)
        {
            u32 old = i;
            struct Pokemon *mon = i == 0 ? &gParties[B_TRAINER_PLAYER][0] : &gParties[B_TRAINER_PARTNER][i - 1];
            u32 maxHP = maxima[old];
            u32 heal = mode <= BRISK_MODE_NORMAL ? maxHP : maxHP / (mode == BRISK_MODE_HARD ? 2 : 4);
            EXPECT_EQ(GetMonData(mon, MON_DATA_HP), min(maxHP, 1 + heal));
            EXPECT_EQ(GetMonData(mon, MON_DATA_STATUS), mode == BRISK_MODE_EASY ? 0 : STATUS1_POISON);
            EXPECT_GT(GetMonData(mon, MON_DATA_PP1), 0);
        }
        EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_RAN));
        EXPECT(DynamaxAdventure_Quit());
    }
    VarSet(VAR_BRISK_DIFFICULTY, savedMode);
}

TEST("Adventure: twelve offers are selectable and rentals and routes have at least three damaging moves")
{
    EXPECT_EQ(DA_RENTAL_CHOICES, 12);
    for (u32 choice = 0; choice < DA_RENTAL_CHOICES; choice++)
    {
        EXPECT(DynamaxAdventure_Begin(17));
        struct Pokemon *mon = DynamaxAdventure_GetRental(choice);
        EXPECT(mon != NULL);
        EXPECT_EQ(GetMonData(mon, MON_DATA_LEVEL), 70);
        EXPECT(DynamaxAdventure_ChooseRental(choice));
        for (u32 branch = 0; branch < DA_PATH_CHOICES; branch++)
        {
            mon = DynamaxAdventure_GetPath(branch);
            u32 attacks = 0;
            for (u32 i = 0; i < MAX_MON_MOVES; i++)
            {
                enum Move move = GetMonData(mon, MON_DATA_MOVE1 + i);
                attacks += move != MOVE_NONE && GetMoveCategory(move) != DAMAGE_CATEGORY_STATUS;
            }
            EXPECT_GE(attacks, 3);
            EXPECT_EQ(GetMonData(mon, MON_DATA_LEVEL), 70);
        }
        EXPECT(DynamaxAdventure_Quit());
    }
}

TEST("Adventure: the rival sometimes accepts a declined catch without replacing the player's rental")
{
    bool32 sawSwap = FALSE, sawKeep = FALSE;
    for (u32 seed = 0; seed < 64 && !(sawSwap && sawKeep); seed++)
    {
        EXPECT(DynamaxAdventure_Begin(seed));
        EXPECT(DynamaxAdventure_ChooseRental(0));
        EXPECT(DynamaxAdventure_ChoosePath(0));
        EXPECT(DynamaxAdventure_PrepareBattle());
        struct Pokemon original[DA_TEAM_SIZE];
        original[0] = gParties[B_TRAINER_PLAYER][0];
        for (u32 i = 1; i < DA_TEAM_SIZE; i++)
            original[i] = gParties[B_TRAINER_PARTNER][i - 1];
        u32 encounter = GetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_PERSONALITY);
        EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
        EXPECT(DynamaxAdventure_Catch(TRUE));
        EXPECT(DynamaxAdventure_Swap(FALSE));
        EXPECT(DynamaxAdventure_ChoosePath(0));
        EXPECT(DynamaxAdventure_PrepareBattle());
        EXPECT_EQ(GetMonData(&original[0], MON_DATA_PERSONALITY), GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_PERSONALITY));
        u32 changed = 0;
        for (u32 i = 0; i < DA_TEAM_SIZE - 1; i++)
        {
            u32 old = 1 + i;
            u32 pid = GetMonData(&gParties[B_TRAINER_PARTNER][i], MON_DATA_PERSONALITY);
            if (pid != GetMonData(&original[old], MON_DATA_PERSONALITY))
            {
                EXPECT_EQ(pid, encounter);
                changed++;
            }
        }
        EXPECT_LE(changed, 1);
        sawSwap |= changed == 1;
        sawKeep |= changed == 0;
        EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_RAN));
        EXPECT(DynamaxAdventure_Quit());
    }
    EXPECT(sawSwap && sawKeep);
}

TWO_VS_ONE_BATTLE_TEST("Adventure: only Extreme doubles boss move damage")
{
    u32 mode;
    u32 savedMode = VarGet(VAR_BRISK_DIFFICULTY);
    PARAMETRIZE { mode = BRISK_MODE_NORMAL; }
    PARAMETRIZE { mode = BRISK_MODE_HARD; }
    PARAMETRIZE { mode = BRISK_MODE_EXTREME; }
    GIVEN {
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        StartRental();
        EXPECT(DynamaxAdventure_ChoosePath(0));
        EXPECT(DynamaxAdventure_PrepareBattle());
        gBattleTestRunnerState->data.recordedBattle.battleFlags &= ~BATTLE_TYPE_TRAINER;
        gBattleTestRunnerState->data.recordedBattle.partnerId = DynamaxAdventure_GetRivalPartnerId();
        gBattleTypeFlags &= ~BATTLE_TYPE_TRAINER;
        PLAYER(SPECIES_WOBBUFFET) { HP(500); MaxHP(500); Moves(MOVE_CELEBRATE); }
        PARTNER(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_DRAGON_RAGE); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_DRAGON_RAGE, target: playerLeft); }
    } THEN {
        EXPECT_EQ(playerLeft->hp, mode == BRISK_MODE_EXTREME ? 420 : 460);
        EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
        gMain.inBattle = FALSE;
        EXPECT(DynamaxAdventure_Quit());
        VarSet(VAR_BRISK_DIFFICULTY, savedMode);
    }
}

TWO_VS_ONE_BATTLE_TEST("Adventure: G-Max boss remains transformed beyond three turns without HP-loss text")
{
    GIVEN {
        StartRental();
        EXPECT(DynamaxAdventure_ChoosePath(0));
        EXPECT(DynamaxAdventure_PrepareBattle());
        gBattleTestRunnerState->data.recordedBattle.battleFlags &= ~BATTLE_TYPE_TRAINER;
        gBattleTestRunnerState->data.recordedBattle.partnerId = DynamaxAdventure_GetRivalPartnerId();
        gBattleTypeFlags &= ~BATTLE_TYPE_TRAINER;
        PLAYER(SPECIES_CHARIZARD) { Level(100); MaxHP(999); HP(999); Item(ITEM_CHARIZARDITE_Y); Moves(MOVE_CELEBRATE); }
        PARTNER(SPECIES_SHUCKLE) { Level(100); MaxHP(999); HP(999); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_VENUSAUR) { Item(ITEM_LIFE_ORB); MaxHP(200); HP(200); Moves(MOVE_TACKLE); GigantamaxFactor(TRUE); }
    } WHEN {
        for (u32 i = 0; i < 4; i++)
            TURN { MOVE(playerLeft, MOVE_CELEBRATE); MOVE(playerRight, MOVE_CELEBRATE); MOVE(opponentLeft, MOVE_TACKLE, target: playerLeft); }
    } SCENE {
        NONE_OF { MESSAGE("The wild Venusaur lost some of its HP!"); }
    } THEN {
        EXPECT_EQ(opponentLeft->species, SPECIES_VENUSAUR_GMAX);
        EXPECT_EQ(GetActiveGimmick(B_BATTLER_1), GIMMICK_DYNAMAX);
        EXPECT_LT(opponentLeft->hp, opponentLeft->maxHP);
        // The headless battle runner does not initialize the move-menu trigger.
        gBattleStruct->gimmick.triggerSpriteId = 0xFF;
        gBattleStruct->gimmick.usableGimmick[B_BATTLER_0] = GIMMICK_MEGA;
        CreateGimmickTriggerSprite(B_BATTLER_0);
        u32 trigger = gBattleStruct->gimmick.triggerSpriteId;
        EXPECT_LT(trigger, MAX_SPRITES);
        gSprites[trigger].callback(&gSprites[trigger]);
        EXPECT_EQ(gSprites[trigger].x, 12);
        EXPECT_EQ(gSprites[trigger].y, 72);
        HideGimmickTriggerSprite();
        gSprites[trigger].callback(&gSprites[trigger]);
        EXPECT_EQ(gBattleStruct->gimmick.triggerSpriteId, 0xFF);
        EXPECT_GT(opponentLeft->hp, 0);
        EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
        gMain.inBattle = FALSE;
        EXPECT(DynamaxAdventure_Quit());
    }
}
