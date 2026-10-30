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
    msgbox Route135_BossRushRoom_Text_Next, MSGBOX_DEFAULT
    special BriskBossRushFight
    specialvar VAR_RESULT, BriskBossRushEndRound
    goto_if_eq VAR_RESULT, 0, Route135_BossRushRoom_EventScript_Lost
    goto_if_eq VAR_RESULT, 2, Route135_BossRushRoom_EventScript_Clear
    goto Route135_BossRushRoom_EventScript_Next

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

Route135_BossRushRoom_Text_Next:
    .string "ROUND {STR_VAR_2}/{STR_VAR_3}\p"
    .string "Next challenger: {STR_VAR_1}!$"
Route135_BossRushRoom_Text_Lost:
    .string "The TRAINER SUMMIT is over!\n"
    .string "SCOTT will welcome you back.$"
Route135_BossRushRoom_Text_Clear:
    .string "AMAZING! You beat every\n"
    .string "invited TRAINER!\p"
    .string "SCOTT has never seen a run\n"
    .string "quite like this!$"
