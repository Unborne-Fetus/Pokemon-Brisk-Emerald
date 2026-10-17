#include "global.h"
#include "battle.h"
#include "dynamax_adventure.h"
#include "main.h"
#include "player_name.h"
#include "pokemon_storage_system.h"
#include "string_util.h"
#include "constants/characters.h"
#include "test/test.h"
#include "task.h"

static const u8 sOldName[] = _("OLD");
static const u8 sNewName[] = _("SPENCER");
static const u8 sForeignName[] = _("TRADE");

static void ExpectOT(struct BoxPokemon *mon, const u8 *name)
{
    u8 actual[PLAYER_NAME_LENGTH + 1];
    GetBoxMonData(mon, MON_DATA_OT_NAME, actual);
    EXPECT_EQ(StringCompare(actual, name), 0);
}

TEST("Player rename preserves ownership in party, last PC box and daycare without changing traded Pokemon")
{
    struct Pokemon party[PARTY_SIZE];
    struct BoxPokemon pc[3];
    struct DaycareMon daycare[DAYCARE_MON_COUNT];
    u8 name[PLAYER_NAME_LENGTH + 1], trainerId[4];
    memcpy(party, gParties[B_TRAINER_PLAYER], sizeof(party));
    memcpy(daycare, gSaveBlock1Ptr->daycare.mons, sizeof(daycare));
    memcpy(name, gSaveBlock2Ptr->playerName, sizeof(name));
    memcpy(trainerId, gSaveBlock2Ptr->playerTrainerId, sizeof(trainerId));
    pc[0] = *GetBoxedMonPtr(0, 0); pc[1] = *GetBoxedMonPtr(0, 1);
    pc[2] = *GetBoxedMonPtr(TOTAL_BOXES_COUNT - 1, IN_BOX_COUNT - 1);
    StringCopy(gSaveBlock2Ptr->playerName, sOldName);
    CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][0], SPECIES_FLYGON, 50, 1234, OTID_STRUCT_PLAYER_ID, 31);
    struct Pokemon *owned = &gParties[B_TRAINER_PLAYER][0];
    gParties[B_TRAINER_PLAYER][1] = *owned;
    u32 otherId = READ_OTID_FROM_SAVE ^ 1;
    SetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_OT_ID, &otherId);
    gParties[B_TRAINER_PLAYER][2] = *owned;
    SetMonData(&gParties[B_TRAINER_PLAYER][2], MON_DATA_OT_NAME, sForeignName);
    *GetBoxedMonPtr(0, 0) = owned->box;
    *GetBoxedMonPtr(0, 1) = gParties[B_TRAINER_PLAYER][2].box;
    *GetBoxedMonPtr(TOTAL_BOXES_COUNT - 1, IN_BOX_COUNT - 1) = owned->box;
    gSaveBlock1Ptr->daycare.mons[0].mon = owned->box;
    gSaveBlock1Ptr->daycare.mons[1].mon = gParties[B_TRAINER_PLAYER][1].box;
    StringCopy(gSaveBlock1Ptr->daycare.mons[1].mail.otName, sOldName);
    u32 personality = GetMonData(owned, MON_DATA_PERSONALITY);
    u32 attack = GetMonData(owned, MON_DATA_ATK);
    u32 shiny = GetMonData(owned, MON_DATA_IS_SHINY);
    u32 id = READ_OTID_FROM_SAVE;
    EXPECT(ChangePlayerName(sNewName));
    EXPECT_EQ(StringCompare(gSaveBlock2Ptr->playerName, sNewName), 0);
    EXPECT_EQ(READ_OTID_FROM_SAVE, id);
    EXPECT(!IsTradedMon(owned));
    EXPECT(IsTradedMon(&gParties[B_TRAINER_PLAYER][1]));
    EXPECT(IsTradedMon(&gParties[B_TRAINER_PLAYER][2]));
    ExpectOT(&owned->box, sNewName);
    ExpectOT(&gParties[B_TRAINER_PLAYER][1].box, sOldName);
    ExpectOT(&gParties[B_TRAINER_PLAYER][2].box, sForeignName);
    ExpectOT(GetBoxedMonPtr(0, 0), sNewName);
    ExpectOT(GetBoxedMonPtr(0, 1), sForeignName);
    ExpectOT(GetBoxedMonPtr(TOTAL_BOXES_COUNT - 1, IN_BOX_COUNT - 1), sNewName);
    ExpectOT(&gSaveBlock1Ptr->daycare.mons[0].mon, sNewName);
    ExpectOT(&gSaveBlock1Ptr->daycare.mons[1].mon, sOldName);
    EXPECT_EQ(StringCompare(gSaveBlock1Ptr->daycare.mons[1].mail.otName, sNewName), 0);
    EXPECT_EQ(GetMonData(owned, MON_DATA_PERSONALITY), personality);
    EXPECT_EQ(GetMonData(owned, MON_DATA_ATK), attack);
    EXPECT_EQ(GetMonData(owned, MON_DATA_IS_SHINY), shiny);
    EXPECT_EQ(GetMonData(owned, MON_DATA_SANITY_IS_BAD_EGG), FALSE);
    EXPECT(ChangePlayerName(COMPOUND_STRING("A")));
    EXPECT(!IsTradedMon(owned));
    EXPECT_EQ(gSaveBlock2Ptr->playerName[1], EOS);
    memcpy(gParties[B_TRAINER_PLAYER], party, sizeof(party));
    memcpy(gSaveBlock1Ptr->daycare.mons, daycare, sizeof(daycare));
    memcpy(gSaveBlock2Ptr->playerName, name, sizeof(name));
    memcpy(gSaveBlock2Ptr->playerTrainerId, trainerId, sizeof(trainerId));
    *GetBoxedMonPtr(0, 0) = pc[0]; *GetBoxedMonPtr(0, 1) = pc[1];
    *GetBoxedMonPtr(TOTAL_BOXES_COUNT - 1, IN_BOX_COUNT - 1) = pc[2];
}

TEST("Player rename rejects empty or overlong names and leaves the Trainer ID alone")
{
    u8 name[PLAYER_NAME_LENGTH + 1];
    u32 id = READ_OTID_FROM_SAVE;
    memcpy(name, gSaveBlock2Ptr->playerName, sizeof(name));
    EXPECT(!ChangePlayerName(NULL));
    EXPECT(!ChangePlayerName(COMPOUND_STRING("")));
    EXPECT(!ChangePlayerName(COMPOUND_STRING("       ")));
    EXPECT(!ChangePlayerName(COMPOUND_STRING("TOOLONG!")));
    EXPECT_EQ(memcmp(name, gSaveBlock2Ptr->playerName, sizeof(name)), 0);
    EXPECT_EQ(READ_OTID_FROM_SAVE, id);
}

TEST("Player rename preserves rentals and rewards when resuming an adventure")
{
    u8 name[PLAYER_NAME_LENGTH + 1];
    memcpy(name, gSaveBlock2Ptr->playerName, sizeof(name));
    StringCopy(gSaveBlock2Ptr->playerName, sOldName);
    EXPECT(DynamaxAdventure_Begin(777));
    EXPECT(ChangePlayerName(sNewName));
    for (u32 i = 0; i < DA_RENTAL_CHOICES; i++)
        EXPECT(!IsTradedMon(DynamaxAdventure_GetRental(i)));
    EXPECT(DynamaxAdventure_ChooseRental(0));
    EXPECT(DynamaxAdventure_ChoosePath(0)); EXPECT(DynamaxAdventure_PrepareBattle());
    EXPECT(!IsTradedMon(&gParties[B_TRAINER_PLAYER][0]));
    EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_WON));
    EXPECT(DynamaxAdventure_Catch(TRUE));
    EXPECT(DynamaxAdventure_Swap(FALSE));
    EXPECT(ChangePlayerName(COMPOUND_STRING("ASH")));
    EXPECT(!IsTradedMon(DynamaxAdventure_GetCaught(0)));
    EXPECT(DynamaxAdventure_ChoosePath(0)); EXPECT(DynamaxAdventure_PrepareBattle());
    EXPECT(!IsTradedMon(&gParties[B_TRAINER_PLAYER][0]));
    EXPECT(DynamaxAdventure_FinishBattle(B_OUTCOME_RAN)); EXPECT(DynamaxAdventure_Quit());
    memcpy(gSaveBlock2Ptr->playerName, name, sizeof(name));
}

static void RenameReturnMarker(void) {}

TEST("Player rename keyboard accepts an edited existing name without reseeding the Trainer ID")
{
    u8 oldName[PLAYER_NAME_LENGTH + 1];
    memcpy(oldName, gSaveBlock2Ptr->playerName, sizeof(oldName));
    StringCopy(gSaveBlock2Ptr->playerName, sOldName);
    u32 id = READ_OTID_FROM_SAVE;
    MainCallback previousCallback = gMain.callback2;
    IntrCallback previousVBlank = gMain.vblankCallback;
    OpenPlayerNameEditor(RenameReturnMarker);
    for (u32 frame = 0; frame < 500 && gMain.callback2 != RenameReturnMarker; frame++)
    {
        // Delete D from OLD, move to OK, then confirm with the real keyboard.
        gMain.newKeys = frame == 150 ? B_BUTTON : frame == 180 ? START_BUTTON : frame == 210 ? A_BUTTON : 0;
        gMain.heldKeys = gMain.newKeys;
        gMain.newAndRepeatedKeys = gMain.newKeys;
        gMain.callback2();
        if (gMain.vblankCallback != NULL)
            gMain.vblankCallback();
    }
    EXPECT(gMain.callback2 == RenameReturnMarker);
    EXPECT_EQ(StringCompare(gSaveBlock2Ptr->playerName, COMPOUND_STRING("OL")), 0);
    EXPECT_EQ(READ_OTID_FROM_SAVE, id);
    memcpy(gSaveBlock2Ptr->playerName, oldName, sizeof(oldName));
    ResetTasks();
    SetMainCallback2(previousCallback);
    SetVBlankCallback(previousVBlank);
    gMain.newKeys = gMain.heldKeys = gMain.newAndRepeatedKeys = 0;
}

TEST("Player rename keeps the player's secret base registered without renaming other owners")
{
    struct SecretBase originals[3];
    u8 name[PLAYER_NAME_LENGTH + 1];
    memcpy(name, gSaveBlock2Ptr->playerName, sizeof(name));
    memcpy(originals, gSaveBlock1Ptr->secretBases, sizeof(originals));
    StringCopy(gSaveBlock2Ptr->playerName, sOldName);
    struct SecretBase *bases = gSaveBlock1Ptr->secretBases;
    memset(&bases[0], 0, sizeof(bases[0]));
    bases[0].secretBaseId = 1;
    bases[0].gender = gSaveBlock2Ptr->playerGender;
    memcpy(bases[0].trainerId, gSaveBlock2Ptr->playerTrainerId, TRAINER_ID_LENGTH);
    memset(bases[0].trainerName, EOS, PLAYER_NAME_LENGTH);
    StringCopyN(bases[0].trainerName, sOldName, PLAYER_NAME_LENGTH);
    bases[1] = bases[0];
    StringCopyN(bases[1].trainerName, sForeignName, PLAYER_NAME_LENGTH);
    bases[2] = bases[0];
    bases[2].gender ^= 1;
    EXPECT(ChangePlayerName(sNewName));
    EXPECT_EQ(StringCompareN(bases[0].trainerName, sNewName, PLAYER_NAME_LENGTH), 0);
    EXPECT_EQ(StringCompareN(bases[1].trainerName, sForeignName, PLAYER_NAME_LENGTH), 0);
    EXPECT_EQ(StringCompareN(bases[2].trainerName, sOldName, PLAYER_NAME_LENGTH), 0);
    memcpy(gSaveBlock1Ptr->secretBases, originals, sizeof(originals));
    memcpy(gSaveBlock2Ptr->playerName, name, sizeof(name));
}
