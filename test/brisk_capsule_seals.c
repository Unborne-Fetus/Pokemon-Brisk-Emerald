#include "global.h"
#include "main.h"
#include "constants/characters.h"
#include "ball_capsule.h"
#include "pokemon.h"
#include "battle.h"
#include "sprite.h"
#include "string_util.h"
#include "party_menu.h"
#include "text.h"
#include "task.h"
#include "test/test.h"

TEST("Brisk expanded seals preserve boxed Pokemon data and all shape and letter values")
{
    struct Pokemon mon, restored;
    CreateMonWithIVs(&mon, SPECIES_PIKACHU, 40, 0, OTID_STRUCT_PRESET(0), 0);
    u16 item = ITEM_LEFTOVERS;
    SetMonData(&mon, MON_DATA_HELD_ITEM, &item);
    u8 phrase = 10;
    SetMonData(&mon, MON_DATA_BALL_CAPSULE_TEXT, &phrase);
    EXPECT_EQ(sizeof(struct PokemonSubstruct0), 12);
    EXPECT_EQ(sizeof(struct PokemonSubstruct1), 12);
    EXPECT_EQ(sizeof(struct PokemonSubstruct3), 12);
    for (u32 style = 0; style < BALL_CAPSULE_STYLES; style++)
    {
        u16 capsule = 0;
        for (u32 slot = 0; slot < BALL_CAPSULE_SLOTS; slot++)
            capsule = SetBallCapsuleSeal(capsule, slot, (style + slot) % BALL_CAPSULE_STYLES);
        SetMonData(&mon, MON_DATA_BALL_CAPSULE_SEALS, &capsule);
        BoxMonToMon(&mon.box, &restored);
        EXPECT_EQ(GetMonData(&restored, MON_DATA_BALL_CAPSULE_SEALS), capsule);
        for (u32 slot = 0; slot < BALL_CAPSULE_SLOTS; slot++)
            EXPECT_EQ(GetBallCapsuleSeal(capsule, slot), (style + slot) % BALL_CAPSULE_STYLES);
        EXPECT_EQ(GetMonData(&restored, MON_DATA_BALL_CAPSULE_TEXT), phrase);
        EXPECT_EQ(GetMonData(&restored, MON_DATA_HELD_ITEM), item);
        EXPECT_EQ(GetMonData(&restored, MON_DATA_SANITY_IS_BAD_EGG), FALSE);
    }
}

TEST("Brisk old capsule layouts migrate to expanded seals without rewriting old Pokemon")
{
    struct Pokemon mon;
    CreateMonWithIVs(&mon, SPECIES_PIKACHU, 20, 0, OTID_STRUCT_PRESET(0), 0);
    for (u8 old = 0; old < 64; old++)
    {
        SetMonData(&mon, MON_DATA_BALL_CAPSULE, &old);
        u16 upgraded = GetMonData(&mon, MON_DATA_BALL_CAPSULE_SEALS);
        for (u32 slot = 0; slot < BALL_CAPSULE_SLOTS; slot++)
            EXPECT_EQ(GetBallCapsuleSeal(upgraded, slot), (old >> (slot * 2)) & 3);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_BALL_CAPSULE), old);
        SetMonData(&mon, MON_DATA_BALL_CAPSULE_SEALS, &upgraded);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_BALL_CAPSULE_SEALS), upgraded);
    }
}

TEST("Brisk custom send-out phrases are separate and bounded to fifteen characters")
{
    memset(gSaveBlock1Ptr->filler1, 0xFF, sizeof(gSaveBlock1Ptr->filler1));
    EXPECT_EQ(GetCustomSendOutText(0)[0], EOS);
    StringCopy(GetCustomSendOutText(0), COMPOUND_STRING("Shine on!"));
    StringCopy(GetCustomSendOutText(1), COMPOUND_STRING("Let's battle!"));
    EXPECT_EQ(StringCompare(GetBallCapsuleText(8), COMPOUND_STRING("Shine on!")), 0);
    EXPECT_EQ(StringCompare(GetBallCapsuleText(9), COMPOUND_STRING("Let's battle!")), 0);
    EXPECT_EQ(GetBallCapsuleText(10)[0], EOS);
    GetCustomSendOutText(2)[15] = 0;
    EXPECT_EQ(GetCustomSendOutText(2)[15], EOS);
}

TEST("Brisk every seal particle including letters finishes and frees graphics")
{
    gMain.inBattle = TRUE;
    gBattleTypeFlags = 0;
    gBattlersCount = 2;
    gBattlerPositions[0] = B_POSITION_PLAYER_LEFT;
    gBattlerPartyIndexes[0] = 0;
    CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][0], SPECIES_PIKACHU, 20, 0, OTID_STRUCT_PRESET(0), 0);
    ResetSpriteData();
    FreeAllSpritePalettes();
    for (u16 style = 1; style < BALL_CAPSULE_STYLES; style++)
    {
        u16 capsule = SetBallCapsuleSeal(0, 0, style);
        SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_BALL_CAPSULE_SEALS, &capsule);
        PlayBallCapsule(0, 100, 80);
        u32 active = 0;
        for (u32 i = 0; i < MAX_SPRITES; i++)
            active += gSprites[i].inUse;
        EXPECT_EQ(active, 6);
        for (u32 frame = 0; frame < 50; frame++)
            AnimateSprites();
        EXPECT_EQ(GetSpriteTileStartByTag(0x5F20), 0xFFFF);
        EXPECT_EQ(IndexOfSpritePaletteTag(0x5F20), 0xFF);
    }
    ResetSpriteData();
    FreeAllSpritePalettes();
}

TEST("Brisk capsule shape and letter editor stays within its window and never changes send-out text")
{
    ResetTasks();
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][0];
    CreateMonWithIVs(mon, SPECIES_PIKACHU, 20, 0, OTID_STRUCT_PRESET(0), 0);
    u8 phrase = 5;
    SetMonData(mon, MON_DATA_BALL_CAPSULE_TEXT, &phrase);
    u8 task = Test_OpenBriskCapsuleEditor();
    gMain.newKeys = 0;
    for (u32 style = 1; style < BALL_CAPSULE_STYLES; style++)
    {
        gMain.newAndRepeatedKeys = DPAD_RIGHT;
        RunTasks();
        EXPECT_LE(GetStringWidth(FONT_NORMAL, gStringVar4, 0), 224);
        EXPECT_EQ(GetBallCapsuleSeal((u16)gTasks[task].data[12], 0), style);
        EXPECT_EQ(GetMonData(mon, MON_DATA_BALL_CAPSULE_TEXT), phrase);
    }
    gMain.newAndRepeatedKeys = 0;
    gMain.newKeys = A_BUTTON;
    RunTasks();
    EXPECT_EQ(GetBallCapsuleSeal(GetMonData(mon, MON_DATA_BALL_CAPSULE_SEALS), 0), 38);
    EXPECT_EQ(GetMonData(mon, MON_DATA_BALL_CAPSULE_TEXT), phrase);
    gMain.newKeys = 0;
    Test_CloseBriskCapsuleEditor();
}
