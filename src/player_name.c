#include "global.h"
#include "player_name.h"
#include "dynamax_adventure.h"
#include "main.h"
#include "naming_screen.h"
#include "pokemon_storage_system.h"
#include "string_util.h"
#include "constants/characters.h"

static EWRAM_DATA u8 sPendingPlayerName[PLAYER_NAME_LENGTH + 1] = {0};
static EWRAM_DATA MainCallback sRenameReturnCallback = NULL;

bool32 RenamePlayerOwnedBoxMon(struct BoxPokemon *mon, const u8 *oldName, const u8 *newName)
{
    u8 otName[PLAYER_NAME_LENGTH + 1];
    if (!GetBoxMonData(mon, MON_DATA_SANITY_HAS_SPECIES)
     || GetBoxMonData(mon, MON_DATA_SANITY_IS_BAD_EGG)
     || GetBoxMonData(mon, MON_DATA_OT_ID) != READ_OTID_FROM_SAVE)
        return FALSE;
    GetBoxMonData(mon, MON_DATA_OT_NAME, otName);
    if (StringCompare(otName, oldName) != 0)
        return FALSE;
    SetBoxMonData(mon, MON_DATA_OT_NAME, newName);
    return TRUE;
}

bool32 ChangePlayerName(const u8 *name)
{
    u8 oldName[PLAYER_NAME_LENGTH + 1];
    u8 newName[PLAYER_NAME_LENGTH + 1];
    bool32 hasLetter = FALSE;
    u32 length;
    if (name == NULL)
        return FALSE;
    for (length = 0; length <= PLAYER_NAME_LENGTH && name[length] != EOS; length++)
        if (name[length] != CHAR_SPACE)
            hasLetter = TRUE;
    if (length > PLAYER_NAME_LENGTH || !hasLetter)
        return FALSE;
    StringCopyN(oldName, gSaveBlock2Ptr->playerName, PLAYER_NAME_LENGTH);
    oldName[PLAYER_NAME_LENGTH] = EOS;
    memset(newName, EOS, sizeof(newName));
    StringCopy(newName, name);
    if (StringCompare(oldName, newName) == 0)
        return TRUE;

    // Keep the Trainer ID, personality, IVs, and all battle/save state intact.
    for (u32 i = 0; i < PARTY_SIZE; i++)
        RenamePlayerOwnedBoxMon(&gParties[B_TRAINER_PLAYER][i].box, oldName, newName);
    for (u32 box = 0; box < TOTAL_BOXES_COUNT; box++)
        for (u32 slot = 0; slot < IN_BOX_COUNT; slot++)
            RenamePlayerOwnedBoxMon(GetBoxedMonPtr(box, slot), oldName, newName);
    for (u32 i = 0; i < DAYCARE_MON_COUNT; i++)
    {
        RenamePlayerOwnedBoxMon(&gSaveBlock1Ptr->daycare.mons[i].mon, oldName, newName);
        // This is the depositor's name, including when they deposited a trade.
        if (StringCompare(gSaveBlock1Ptr->daycare.mons[i].mail.otName, oldName) == 0)
            StringCopy(gSaveBlock1Ptr->daycare.mons[i].mail.otName, newName);
    }
    for (u32 i = 0; i < SECRET_BASES_COUNT; i++)
    {
        struct SecretBase *base = &gSaveBlock1Ptr->secretBases[i];
        if (base->secretBaseId != 0 && base->gender == gSaveBlock2Ptr->playerGender
         && memcmp(base->trainerId, gSaveBlock2Ptr->playerTrainerId, TRAINER_ID_LENGTH) == 0
         && StringCompareN(base->trainerName, oldName, PLAYER_NAME_LENGTH) == 0)
            memcpy(base->trainerName, newName, PLAYER_NAME_LENGTH);
    }
    #if IS_FRLG
    RenamePlayerOwnedBoxMon(&gSaveBlock1Ptr->route5DayCareMon.mon, oldName, newName);
    #endif
    DynamaxAdventure_UpdatePlayerName(oldName, newName);
    memcpy(gSaveBlock2Ptr->playerName, newName, sizeof(newName));
    return TRUE;
}

static void CB2_FinishPlayerRename(void)
{
    ChangePlayerName(sPendingPlayerName);
    SetMainCallback2(sRenameReturnCallback);
}

void OpenPlayerNameEditor(MainCallback returnCallback)
{
    StringCopyN(sPendingPlayerName, gSaveBlock2Ptr->playerName, PLAYER_NAME_LENGTH);
    sPendingPlayerName[PLAYER_NAME_LENGTH] = EOS;
    sRenameReturnCallback = returnCallback;
    // The new-game PLAYER template reseeds the Trainer ID on exit. Renaming
    // uses its own template, and edits a buffer until the player confirms.
    DoNamingScreen(NAMING_SCREEN_PLAYER_RENAME, sPendingPlayerName,
                   gSaveBlock2Ptr->playerGender, 0, 0, CB2_FinishPlayerRename);
}
