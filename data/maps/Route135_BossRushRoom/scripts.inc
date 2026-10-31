Route135_BossRushRoom_MapScripts::
    map_script MAP_SCRIPT_ON_FRAME_TABLE, Route135_BossRushRoom_OnFrame
    .byte 0

Route135_BossRushRoom_OnFrame:
    map_script_2 VAR_TEMP_0, 0, Route135_BossRushRoom_EventScript_Run
    .2byte 0

Route135_BossRushRoom_EventScript_Run::
    lockall
    setvar VAR_TEMP_0, 1
    specialvar VAR_RESULT, BriskBossRushCheckActive
    goto_if_eq VAR_RESULT, 0, Route135_BossRushRoom_EventScript_Return
Route135_BossRushRoom_EventScript_Next::
    specialvar VAR_RESULT, BriskBossRushBufferNext
    goto_if_eq VAR_RESULT, 0, Route135_BossRushRoom_EventScript_Return
    msgbox Route135_BossRushRoom_Text_Next, MSGBOX_YESNO
    goto_if_eq VAR_RESULT, NO, Route135_BossRushRoom_EventScript_Forfeit
    special BriskBossRushFight
    specialvar VAR_RESULT, BriskBossRushEndRound
    goto_if_eq VAR_RESULT, 0, Route135_BossRushRoom_EventScript_Lost
    goto_if_eq VAR_RESULT, 2, Route135_BossRushRoom_EventScript_Clear
    goto_if_eq VAR_RESULT, 3, Route135_BossRushRoom_EventScript_Prize
    goto Route135_BossRushRoom_EventScript_Next

Route135_BossRushRoom_EventScript_Prize::
    special BriskBossRushBufferReward
    msgbox Route135_BossRushRoom_Text_Prize, MSGBOX_DEFAULT
Route135_BossRushRoom_EventScript_ClaimPrize::
    specialvar VAR_RESULT, BriskBossRushClaimReward
    goto_if_eq VAR_RESULT, 0, Route135_BossRushRoom_EventScript_NoPrizeSpace
    msgbox Route135_BossRushRoom_Text_PrizeReceived, MSGBOX_DEFAULT
    specialvar VAR_RESULT, BriskBossRushFinishReward
    goto_if_eq VAR_RESULT, 2, Route135_BossRushRoom_EventScript_Clear
    goto Route135_BossRushRoom_EventScript_Next

Route135_BossRushRoom_EventScript_NoPrizeSpace::
    msgbox Route135_BossRushRoom_Text_NoPrizeSpace, MSGBOX_DEFAULT
    specialvar VAR_RESULT, BriskBossRushRewardIsItem
    goto_if_eq VAR_RESULT, 1, Route135_BossRushRoom_EventScript_ItemPC
    special ShowPokemonStorageSystemPC
    goto Route135_BossRushRoom_EventScript_ClaimPrize

Route135_BossRushRoom_EventScript_ItemPC::
    special PlayerPC
    goto Route135_BossRushRoom_EventScript_ClaimPrize

Route135_BossRushRoom_EventScript_Forfeit::
    special BriskBossRushForfeit
    msgbox Route135_BossRushRoom_Text_Forfeit, MSGBOX_DEFAULT
    goto Route135_BossRushRoom_EventScript_Return

Route135_BossRushRoom_EventScript_Lost::
    msgbox Route135_BossRushRoom_Text_Lost, MSGBOX_DEFAULT
    goto Route135_BossRushRoom_EventScript_Return
Route135_BossRushRoom_EventScript_Clear::
    msgbox Route135_BossRushRoom_Text_Clear, MSGBOX_DEFAULT
Route135_BossRushRoom_EventScript_Return::
    closemessage
    warp MAP_ROUTE135_BOSS_RUSH_LOBBY, 6, 7
    waitstate
    releaseall
    end

Route135_BossRushRoom_Text_Prize:
    .string "STREAK: {STR_VAR_2} WINS!\n"
    .string "REWARD: {STR_VAR_3}\p"
    .string "SCOTT: Your prize is\n"
    .string "{STR_VAR_1}!$"

Route135_BossRushRoom_Text_PrizeReceived:
    .string "{STR_VAR_1} was sent to\n"
    .string "{STR_VAR_3}!$"

Route135_BossRushRoom_Text_NoPrizeSpace:
    .string "No room for this prize!\p"
    .string "Use the PC to make space,\n"
    .string "then claim your reward.$"

Route135_BossRushRoom_Text_Next:
    .string "ROUND {STR_VAR_2}/{STR_VAR_3}\p"
    .string "Face {STR_VAR_1} next?$"
Route135_BossRushRoom_Text_Forfeit:
    .string "You stepped out of the rush.\n"
    .string "SCOTT hopes to see you again!$"
Route135_BossRushRoom_Text_Lost:
    .string "The TRAINER SUMMIT is over!\n"
    .string "SCOTT will welcome you back.$"
Route135_BossRushRoom_Text_Clear:
    .string "AMAZING! You beat every\n"
    .string "invited TRAINER!\p"
    .string "SCOTT has never seen a run\n"
    .string "quite like this!$"
