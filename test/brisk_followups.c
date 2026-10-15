#include "global.h"
#include "battle.h"
#include "ball_capsule.h"
#include "brisk_modes.h"
#include "caps.h"
#include "event_data.h"
#include "item.h"
#include "intro.h"
#include "main.h"
#include "palette.h"
#include "party_menu.h"
#include "pokemon.h"
#include "sprite.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "test/test.h"
#include "constants/rgb.h"

TEST("Brisk difficulty progression caps use full, regular and minus-ten limits")
{
    static const u16 flags[] = {FLAG_BADGE01_GET, FLAG_BADGE02_GET, FLAG_BADGE03_GET, FLAG_BADGE04_GET, FLAG_BADGE05_GET, FLAG_BADGE06_GET, FLAG_BADGE07_GET, FLAG_BADGE08_GET, FLAG_IS_CHAMPION};
    bool8 saved[ARRAY_COUNT(flags)];
    for (u32 i = 0; i < ARRAY_COUNT(flags); i++) { saved[i] = FlagGet(flags[i]); FlagClear(flags[i]); }
    VarSet(VAR_BRISK_NO_LEVEL_CAP, 0);
    static const u8 expected[] = {25, 35, 45, 50, 55, 65, 75, 100, 100};
    for (u32 stage = 0; stage < ARRAY_COUNT(expected); stage++)
    {
        for (u32 mode = 0; mode < 4; mode++)
        {
            VarSet(VAR_BRISK_DIFFICULTY, mode);
            u32 cap = mode == BRISK_MODE_EASY ? MAX_LEVEL : mode >= BRISK_MODE_HARD ? expected[stage] - 10 : expected[stage];
            EXPECT_EQ(GetProgressionLevelCap(), cap);
            EXPECT_EQ(GetCurrentLevelCap(), mode == BRISK_MODE_EXTREME ? cap : MAX_LEVEL);
        }
        FlagSet(flags[stage]);
    }
    for (u32 i = 0; i < ARRAY_COUNT(flags); i++) if (saved[i]) FlagSet(flags[i]); else FlagClear(flags[i]);
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
}

TEST("Brisk Extreme candies stop at the cap and never lower an existing over-cap Pokemon")
{
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_EXTREME);
    VarSet(VAR_BRISK_NO_LEVEL_CAP, 1); // Extreme enforces its cap even with the old override.
    u32 cap = GetCurrentLevelCap();
    struct Pokemon mon;
    CreateMonWithIVs(&mon, SPECIES_PIKACHU, cap - 1, 0, OTID_STRUCT_PRESET(0), 0);
    EXPECT(!ExecuteTableBasedItemEffect(&mon, ITEM_EXP_CANDY_XL, 0, 0, 1, 10));
    EXPECT_EQ(GetMonData(&mon, MON_DATA_LEVEL), cap);
    u32 exp = GetMonData(&mon, MON_DATA_EXP);
    EXPECT(ExecuteTableBasedItemEffect(&mon, ITEM_EXP_CANDY_XL, 0, 0, 1, 1));
    EXPECT(ExecuteTableBasedItemEffect(&mon, ITEM_RARE_CANDY, 0, 0, 1, 1));
    EXPECT_EQ(GetMonData(&mon, MON_DATA_EXP), exp);
    EXPECT_EQ(GetSoftLevelCapExpValue(cap, 100), 0);
    CreateMonWithIVs(&mon, SPECIES_PIKACHU, cap + 1, 0, OTID_STRUCT_PRESET(0), 0);
    exp = GetMonData(&mon, MON_DATA_EXP);
    EXPECT(ExecuteTableBasedItemEffect(&mon, ITEM_EXP_CANDY_XL, 0, 0, 1, 1));
    EXPECT_EQ(GetMonData(&mon, MON_DATA_EXP), exp);
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
    EXPECT_EQ(GetSoftLevelCapExpValue(cap + 1, 100), 100);
    VarSet(VAR_BRISK_NO_LEVEL_CAP, 0);
}

TEST("Brisk capsule text saves per Pokemon and survives boxing without damaging seals")
{
    struct Pokemon mon, restored;
    CreateMonWithIVs(&mon, SPECIES_PIKACHU, 20, 0, OTID_STRUCT_PRESET(0), 0);
    u8 capsule = 57;
    SetMonData(&mon, MON_DATA_BALL_CAPSULE, &capsule);
    u32 exp = GetMonData(&mon, MON_DATA_EXP);
    for (u8 text = 0; text < BALL_CAPSULE_TEXT_COUNT; text++)
    {
        SetMonData(&mon, MON_DATA_BALL_CAPSULE_TEXT, &text);
        BoxMonToMon(&mon.box, &restored);
        EXPECT_EQ(GetMonData(&restored, MON_DATA_BALL_CAPSULE_TEXT), text);
        EXPECT_EQ(GetMonData(&restored, MON_DATA_BALL_CAPSULE), capsule);
        EXPECT_EQ(GetMonData(&restored, MON_DATA_EXP), exp);
        EXPECT_EQ(GetMonData(&restored, MON_DATA_SPECIES), SPECIES_PIKACHU);
    }
}

TEST("Brisk capsule editor offers text choices and keeps them pending until Save")
{
    ResetTasks();
    CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][0], SPECIES_PIKACHU, 20, 0, OTID_STRUCT_PRESET(0), 0);
    u8 task = Test_OpenBriskCapsuleEditor();
    gMain.newKeys = 0;
    gMain.newAndRepeatedKeys = DPAD_UP;
    RunTasks();
    EXPECT_EQ(gTasks[task].data[13], BALL_CAPSULE_SLOTS);
    for (u32 text = 1; text < BALL_CAPSULE_TEXT_COUNT; text++)
    {
        gMain.newAndRepeatedKeys = DPAD_RIGHT;
        RunTasks();
        EXPECT_LE(GetStringWidth(FONT_NORMAL, gStringVar4, 0), 224);
    }
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_BALL_CAPSULE_TEXT), 0);
    gMain.newAndRepeatedKeys = 0;
    gMain.newKeys = A_BUTTON;
    RunTasks();
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_BALL_CAPSULE_TEXT), BALL_CAPSULE_TEXT_COUNT - 1);
    gMain.newKeys = 0;
    Test_CloseBriskCapsuleEditor();
}

TEST("Brisk fast intro palette clock completes a white fade with two logical frames per display")
{
    ResetPaletteFade();
    u16 color = RGB(4, 8, 12);
    LoadPalette(&color, 1, sizeof(color));
    BeginNormalPaletteFade(1, 0, 16, 0, RGB_WHITEALPHA);
    for (u32 frame = 0; frame < 30; frame++)
    {
        UpdatePaletteFadeForFastIntro();
        UpdatePaletteFadeForFastIntro();
        TransferPlttBuffer();
    }
    EXPECT(!gPaletteFade.active);
    EXPECT_EQ(gPlttBufferFaded[1], color);
    ResetPaletteFade();
}

TEST("Brisk double-speed intro reaches the bicycle scene with restored scenery colors")
{
    EXPECT(Test_BriskIntroSceneryColors());
}

TEST("Brisk item submenu prompt fits beside the action list")
{
    EXPECT_LE(GetStringWidth(FONT_NORMAL, COMPOUND_STRING("Do what with an item?"), 0), 128);
}
