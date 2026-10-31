Route135_BossRushLobby_MapScripts::
    .byte 0

Route135_BossRushLobby_EventScript_Nurse::
    lock
    faceplayer
    msgbox Route135_BossRushLobby_Text_NurseOffer, MSGBOX_YESNO
    goto_if_eq VAR_RESULT, NO, Route135_BossRushLobby_EventScript_NurseCancel
    special HealPlayerParty
    callnative UpdateFollowingPokemon
    msgbox Route135_BossRushLobby_Text_NurseHealed, MSGBOX_DEFAULT
Route135_BossRushLobby_EventScript_NurseCancel::
    release
    end

Route135_BossRushLobby_Text_NurseOffer:
    .string "Welcome to the TRAINER SUMMIT!\p"
    .string "Shall I restore your POKéMON?$"

Route135_BossRushLobby_Text_NurseHealed:
    .string "All your POKéMON are ready!\n"
    .string "Good luck in the arena!$"

Route135_BossRushLobby_EventScript_PC::
    lockall
    msgbox gText_StorageSystemOpened, MSGBOX_DEFAULT
    special ShowPokemonStorageSystemPC
    releaseall
    end

Route135_BossRushLobby_EventScript_Door::
    msgbox Route135_BossRushLobby_Text_GuardedDoor, MSGBOX_SIGN
    end

Route135_BossRushLobby_EventScript_Scott::
    lock
    faceplayer
    special BriskBossRushInitRules
    msgbox Route135_BossRushLobby_Text_ScottIntro, MSGBOX_DEFAULT
Route135_BossRushLobby_EventScript_MainMenu::
    multichoice 0, 0, MULTI_BRISK_BOSS_RUSH_MAIN, FALSE
    switch VAR_RESULT
    case 0, Route135_BossRushLobby_EventScript_Start
    case 1, Route135_BossRushLobby_EventScript_Modifiers
    case 2, Route135_BossRushLobby_EventScript_Explain
    release
    end

Route135_BossRushLobby_EventScript_Explain::
    msgbox Route135_BossRushLobby_Text_Explain, MSGBOX_DEFAULT
    msgbox Route135_BossRushLobby_Text_Prizes, MSGBOX_DEFAULT
    goto Route135_BossRushLobby_EventScript_MainMenu

Route135_BossRushLobby_EventScript_Modifiers::
    special BriskBossRushBufferRules
    msgbox Route135_BossRushLobby_Text_Rules, MSGBOX_DEFAULT
    special BriskBossRushBufferExtras
    msgbox Route135_BossRushLobby_Text_Extras, MSGBOX_DEFAULT
    special BriskBossRushBufferBattleRules
    msgbox Route135_BossRushLobby_Text_BattleRules, MSGBOX_DEFAULT
    multichoice 0, 0, MULTI_BRISK_BOSS_RUSH_RULES, FALSE
    switch VAR_RESULT
    case 0, Route135_BossRushLobby_EventScript_ToggleHeal
    case 1, Route135_BossRushLobby_EventScript_ToggleBag
    case 2, Route135_BossRushLobby_EventScript_CycleMode
    case 3, Route135_BossRushLobby_EventScript_ToggleStatus
    case 4, Route135_BossRushLobby_EventScript_TogglePP
    case 5, Route135_BossRushLobby_EventScript_CycleEnemyLevels
    case 6, Route135_BossRushLobby_EventScript_ToggleBattleStyle
    case 7, Route135_BossRushLobby_EventScript_ToggleGimmicks
    goto Route135_BossRushLobby_EventScript_MainMenu

Route135_BossRushLobby_EventScript_ToggleHeal::
    special BriskBossRushToggleHeal
    goto Route135_BossRushLobby_EventScript_Modifiers
Route135_BossRushLobby_EventScript_ToggleBag::
    special BriskBossRushToggleBag
    goto Route135_BossRushLobby_EventScript_Modifiers
Route135_BossRushLobby_EventScript_CycleMode::
    special BriskBossRushCycleDifficulty
    goto Route135_BossRushLobby_EventScript_Modifiers

Route135_BossRushLobby_EventScript_ToggleStatus::
    special BriskBossRushToggleStatus
    goto Route135_BossRushLobby_EventScript_Modifiers
Route135_BossRushLobby_EventScript_TogglePP::
    special BriskBossRushTogglePP
    goto Route135_BossRushLobby_EventScript_Modifiers
Route135_BossRushLobby_EventScript_CycleEnemyLevels::
    special BriskBossRushCycleEnemyLevels
    goto Route135_BossRushLobby_EventScript_Modifiers
Route135_BossRushLobby_EventScript_ToggleBattleStyle::
    special BriskBossRushToggleBattleStyle
    goto Route135_BossRushLobby_EventScript_Modifiers
Route135_BossRushLobby_EventScript_ToggleGimmicks::
    special BriskBossRushToggleGimmicks
    goto Route135_BossRushLobby_EventScript_Modifiers

Route135_BossRushLobby_EventScript_Start::
    msgbox Route135_BossRushLobby_Text_Confirm, MSGBOX_YESNO
    goto_if_eq VAR_RESULT, NO, Route135_BossRushLobby_EventScript_MainMenu
    specialvar VAR_RESULT, BriskBossRushPrepare
    goto_if_eq VAR_RESULT, 0, Route135_BossRushLobby_EventScript_NoTrainers
    msgbox Route135_BossRushLobby_Text_Begin, MSGBOX_DEFAULT
    closemessage
    warp MAP_ROUTE135_BOSS_RUSH_ROOM, 5, 7
    waitstate
    release
    end

Route135_BossRushLobby_EventScript_NoTrainers::
    msgbox Route135_BossRushLobby_Text_NoTrainers, MSGBOX_DEFAULT
    goto Route135_BossRushLobby_EventScript_MainMenu

Route135_BossRushLobby_Text_ScottIntro:
    .string "SCOTT: Hi, {PLAYER}!\p"
    .string "I've gathered all the TRAINERS\n"
    .string "you've beaten in your travels!\p"
    .string "They want another chance.\n"
    .string "Welcome to my TRAINER SUMMIT!$"

Route135_BossRushLobby_Text_Explain:
    .string "Every beaten TRAINER is invited\n"
    .string "to battle you again!\p"
    .string "The order is random each time.\n"
    .string "One loss ends the challenge.\p"
    .string "No story gifts are repeated.$"

Route135_BossRushLobby_Text_Prizes:
    .string "SCOTT: Each win earns its place!\p"
    .string "WINS 1-9: A random MEGA STONE\p"
    .string "WIN 10: A random POKéMON\p"
    .string "WIN 25: A random shiny POKéMON\p"
    .string "WIN 50: A random LEGENDARY\p"
    .string "WIN 75: A shiny LEGENDARY\p"
    .string "Every 25 more wins earns\l"
    .string "another shiny LEGENDARY!$"

Route135_BossRushLobby_Text_Rules:
    .string "SCOTT: Set your modifiers!\p"
    .string "Healing: {STR_VAR_1}\n"
    .string "Battle Bag: {STR_VAR_2}\n"
    .string "Difficulty: {STR_VAR_3}$"

Route135_BossRushLobby_Text_Extras:
    .string "Cure status: {STR_VAR_1}\n"
    .string "Refill PP: {STR_VAR_2}\n"
    .string "Enemy levels: {STR_VAR_3}$"

Route135_BossRushLobby_Text_BattleRules:
    .string "Battle style: {STR_VAR_1}\n"
    .string "Gimmick recharge: {STR_VAR_2}$"

Route135_BossRushLobby_Text_Confirm:
    .string "SCOTT: Battle everyone you've\n"
    .string "defeated? Ready for the rush?$"

Route135_BossRushLobby_Text_Begin:
    .string "SCOTT: The crowd is waiting!\n"
    .string "Good luck out there!$"

Route135_BossRushLobby_Text_NoTrainers:
    .string "SCOTT: No defeated TRAINERS\n"
    .string "are eligible yet!$"

Route135_BossRushLobby_Text_GuardedDoor:
    .string "TRAINER SUMMIT - Arena Access\n"
    .string "Please check in with SCOTT.$"
