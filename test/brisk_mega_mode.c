#include "global.h"
#include "test/test.h"
#include "brisk_options.h"
#include "event_data.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "main.h"
#include "task.h"

u8 Test_OpenBriskBattleOptions(void);
void Test_CloseBriskThemeOption(u8 taskId);

TEST("Brisk Mega Mode supports every official and custom stone through fainting and battle end")
{
    FlagSet(FLAG_BRISK_MEGA_MODE);
    u32 count = 0;
    for (enum Species species = 1; species < NUM_SPECIES; species++)
    {
        if (!IsSpeciesEnabled(species) || gSpeciesInfo[species].isMegaEvolution
            || gSpeciesInfo[species].isGigantamax || gSpeciesInfo[species].isPrimalReversion
            || gSpeciesInfo[species].isUltraBurst || gSpeciesInfo[species].isTeraForm) continue;
        const struct FormChange *forms = GetSpeciesFormChanges(species);
        for (u32 i = 0; forms && forms[i].method != FORM_CHANGE_TERMINATOR; i++)
        {
            if (forms[i].method != FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM) continue;
            enum Species mega = forms[i].targetSpecies;
            enum Item stone = forms[i].param1;
            EXPECT_EQ(GetBriskMegaModeSpecies(species, stone), mega);
            EXPECT_EQ(GetBriskMegaModeSpecies(mega, stone), mega);
            EXPECT_EQ(GetBriskMegaModeSpecies(mega, ITEM_NONE), species);
            const enum FormChanges methods[] = {FORM_CHANGE_ITEM_HOLD, FORM_CHANGE_BEGIN_BATTLE,
                FORM_CHANGE_FAINT, FORM_CHANGE_END_BATTLE, FORM_CHANGE_DEPOSIT, FORM_CHANGE_WITHDRAW};
            for (u32 j = 0; j < ARRAY_COUNT(methods); j++)
            {
                struct FormChangeContext ctx = {.method = methods[j], .currentSpecies = mega, .heldItem = stone};
                EXPECT_EQ(GetFormChangeTargetSpecies_Internal(ctx), mega);
            }
            count++;
        }
    }
    EXPECT(count > 100);
    FlagClear(FLAG_BRISK_MEGA_MODE);
}

TEST("Brisk Mega Mode swaps X and Y forms, restores stats, and preserves fainted HP and identity")
{
    struct Pokemon mon;
    FlagClear(FLAG_BRISK_MEGA_MODE);
    CreateMon(&mon, SPECIES_CHARIZARD, 50, 31, OTID_STRUCT_PRESET(0));
    CalculateMonStats(&mon);
    u32 attack = GetMonData(&mon, MON_DATA_ATK);
    u32 personality = GetMonData(&mon, MON_DATA_PERSONALITY);
    u32 exp = GetMonData(&mon, MON_DATA_EXP);
    FlagSet(FLAG_BRISK_MEGA_MODE);
    u16 item = ITEM_CHARIZARDITE_X;
    SetMonData(&mon, MON_DATA_HELD_ITEM, &item);
    EXPECT(TryFormChange(&mon, FORM_CHANGE_ITEM_HOLD, B_TRAINER_PLAYER));
    EXPECT_EQ(GetMonData(&mon, MON_DATA_SPECIES), SPECIES_CHARIZARD_MEGA_X);
    EXPECT(GetMonData(&mon, MON_DATA_ATK) > attack);
    item = ITEM_CHARIZARDITE_Y;
    SetMonData(&mon, MON_DATA_HELD_ITEM, &item);
    EXPECT(TryFormChange(&mon, FORM_CHANGE_ITEM_HOLD, B_TRAINER_PLAYER));
    EXPECT_EQ(GetMonData(&mon, MON_DATA_SPECIES), SPECIES_CHARIZARD_MEGA_Y);
    u16 hp = 0;
    SetMonData(&mon, MON_DATA_HP, &hp);
    EXPECT(!TryFormChange(&mon, FORM_CHANGE_FAINT, B_TRAINER_PLAYER));
    EXPECT(!TryFormChange(&mon, FORM_CHANGE_END_BATTLE, B_TRAINER_PLAYER));
    EXPECT_EQ(GetMonData(&mon, MON_DATA_HP), 0);
    item = ITEM_NONE;
    SetMonData(&mon, MON_DATA_HELD_ITEM, &item);
    EXPECT(TryFormChange(&mon, FORM_CHANGE_ITEM_HOLD, B_TRAINER_PLAYER));
    EXPECT_EQ(GetMonData(&mon, MON_DATA_SPECIES), SPECIES_CHARIZARD);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_ATK), attack);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_HP), 0);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_PERSONALITY), personality);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_EXP), exp);
    FlagClear(FLAG_BRISK_MEGA_MODE);
}

TEST("Brisk Mega Mode toggles all owned parties and boxes while leaving eggs unchanged")
{
    FlagClear(FLAG_BRISK_MEGA_MODE);
    struct Pokemon mon;
    CreateMon(&mon, SPECIES_BRELOOM, 30, 31, OTID_STRUCT_PRESET(0));
    u16 item = ITEM_BRELOOMITE;
    SetMonData(&mon, MON_DATA_HELD_ITEM, &item);
    gParties[B_TRAINER_PLAYER][0] = mon;
    gParties[B_TRAINER_PLAYER][1] = mon;
    *GetBoxedMonPtr(0, 0) = mon.box;
    *GetBoxedMonPtr(TOTAL_BOXES_COUNT - 1, IN_BOX_COUNT - 1) = mon.box;
    bool32 egg = TRUE;
    SetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_IS_EGG, &egg);
    SetBriskMegaMode(TRUE);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SPECIES), SPECIES_BRELOOM_MEGA);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_SPECIES), SPECIES_BRELOOM);
    EXPECT_EQ(GetBoxMonData(GetBoxedMonPtr(0, 0), MON_DATA_SPECIES), SPECIES_BRELOOM_MEGA);
    EXPECT_EQ(GetBoxMonData(GetBoxedMonPtr(TOTAL_BOXES_COUNT - 1, IN_BOX_COUNT - 1), MON_DATA_SPECIES), SPECIES_BRELOOM_MEGA);
    SetBriskMegaMode(FALSE);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SPECIES), SPECIES_BRELOOM);
    EXPECT_EQ(GetBoxMonData(GetBoxedMonPtr(0, 0), MON_DATA_SPECIES), SPECIES_BRELOOM);
    EXPECT_EQ(GetBoxMonData(GetBoxedMonPtr(TOTAL_BOXES_COUNT - 1, IN_BOX_COUNT - 1), MON_DATA_SPECIES), SPECIES_BRELOOM);
}

TEST("Brisk Mega Mode option persists and does not toggle Gimmick Gen or DexNav")
{
    SetBriskMegaMode(FALSE);
    bool32 gimmick = BriskGimmickGenEnabled();
    u8 task = Test_OpenBriskBattleOptions();
    gMain.newKeys = 0;
    gMain.newAndRepeatedKeys = DPAD_DOWN;
    for (u32 i = 0; i < 6; i++) RunTasks();
    gMain.newAndRepeatedKeys = 0;
    gMain.newKeys = A_BUTTON;
    RunTasks();
    EXPECT(BriskMegaModeEnabled());
    // Last row is a setting, not Cancel. Repeated activation toggles it.
    RunTasks();
    EXPECT(!BriskMegaModeEnabled());
    RunTasks();
    EXPECT(BriskMegaModeEnabled());
    EXPECT_EQ(BriskGimmickGenEnabled(), gimmick);
    gMain.newKeys = 0;
    Test_CloseBriskThemeOption(task);
    task = Test_OpenBriskBattleOptions();
    EXPECT(BriskMegaModeEnabled());
    Test_CloseBriskThemeOption(task);
    SetBriskMegaMode(FALSE);
}
