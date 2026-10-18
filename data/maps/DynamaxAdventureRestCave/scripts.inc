DynamaxAdventureRestCave_MapScripts::
    map_script MAP_SCRIPT_ON_TRANSITION, DynamaxAdventureRestCave_SetRival
    .byte 0

DynamaxAdventureRestCave_SetRival:
    checkplayergender
    goto_if_eq VAR_RESULT, FEMALE, DynamaxAdventureRestCave_Brendan
    setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_RIVAL_MAY_NORMAL
    end
DynamaxAdventureRestCave_Brendan:
    setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_RIVAL_BRENDAN_NORMAL
    end

DynamaxAdventureRestCave_EventScript_Rival::
    msgbox DynamaxAdventureRestCave_Text_Rival, MSGBOX_NPC
    end
DynamaxAdventureRestCave_Text_Rival:
    .string "I will pick my own rental!\n"
    .string "If you go down, I will keep fighting.$"

DynamaxAdventureRestCave_EventScript_Birch::
    lock
    faceplayer
    setvar VAR_0x8004, DA_OP_STATUS
    special RunDynamaxAdventure
    goto_if_eq VAR_0x8000, DA_PHASE_PATH, DynamaxAdventureRestCave_BirchSwap
    goto_if_eq VAR_0x8000, DA_PHASE_READY, DynamaxAdventureRestCave_BirchSwap
    checkplayergender
    goto_if_eq VAR_RESULT, FEMALE, DynamaxAdventureRestCave_BirchBrendan
    msgbox DynamaxAdventureRestCave_Text_BirchMay, MSGBOX_DEFAULT
    goto DynamaxAdventureRestCave_BirchDone
DynamaxAdventureRestCave_BirchBrendan:
    msgbox DynamaxAdventureRestCave_Text_BirchBrendan, MSGBOX_DEFAULT
    goto DynamaxAdventureRestCave_BirchDone
DynamaxAdventureRestCave_BirchSwap:
    msgbox DynamaxAdventureRestCave_Text_SwapOffer, MSGBOX_DEFAULT
    setvar VAR_0x8008, 0
    setvar VAR_0x8004, DA_OP_BENCH_INFO
    setvar VAR_0x8005, 0
    special RunDynamaxAdventure
    goto_if_eq VAR_RESULT, FALSE, DynamaxAdventureRestCave_BenchNext0
    dynmultipush DynamaxAdventureCave_Text_Mon, 0
    addvar VAR_0x8008, 1
DynamaxAdventureRestCave_BenchNext0:
    setvar VAR_0x8004, DA_OP_BENCH_INFO
    setvar VAR_0x8005, 1
    special RunDynamaxAdventure
    goto_if_eq VAR_RESULT, FALSE, DynamaxAdventureRestCave_BenchNext1
    dynmultipush DynamaxAdventureCave_Text_Mon, 1
    addvar VAR_0x8008, 1
DynamaxAdventureRestCave_BenchNext1:
    setvar VAR_0x8004, DA_OP_BENCH_INFO
    setvar VAR_0x8005, 2
    special RunDynamaxAdventure
    goto_if_eq VAR_RESULT, FALSE, DynamaxAdventureRestCave_BenchNext2
    dynmultipush DynamaxAdventureCave_Text_Mon, 2
    addvar VAR_0x8008, 1
DynamaxAdventureRestCave_BenchNext2:
    setvar VAR_0x8004, DA_OP_BENCH_INFO
    setvar VAR_0x8005, 3
    special RunDynamaxAdventure
    goto_if_eq VAR_RESULT, FALSE, DynamaxAdventureRestCave_BenchNext3
    dynmultipush DynamaxAdventureCave_Text_Mon, 3
    addvar VAR_0x8008, 1
DynamaxAdventureRestCave_BenchNext3:
    goto_if_eq VAR_0x8008, 0, DynamaxAdventureRestCave_BirchNoCatches
    dynmultistack 1, 1, FALSE, 3, FALSE, 0, DYN_MULTICHOICE_CB_ADVENTURE_CAUGHT
    goto_if_eq VAR_RESULT, MULTI_B_PRESSED, DynamaxAdventureRestCave_BirchDone
    copyvar VAR_0x8005, VAR_RESULT
    setvar VAR_0x8004, DA_OP_SWAP_CAUGHT
    special RunDynamaxAdventure
    goto_if_eq VAR_RESULT, FALSE, DynamaxAdventureRestCave_BirchNoCatches
    msgbox DynamaxAdventureRestCave_Text_Swapped, MSGBOX_DEFAULT
    goto DynamaxAdventureRestCave_BirchDone
DynamaxAdventureRestCave_BirchNoCatches:
    msgbox DynamaxAdventureRestCave_Text_NoBench, MSGBOX_DEFAULT
DynamaxAdventureRestCave_BirchDone:
    release
    end
DynamaxAdventureRestCave_Text_SwapOffer:
    .string "I've brought our caught POKéMON along.\n"
    .string "Want to switch your current rental?\p"
    .string "Choose one that neither you nor\n"
    .string "{RIVAL} is already using.$"
DynamaxAdventureRestCave_Text_NoBench:
    .string "There's nobody available to switch to.\n"
    .string "Catch more POKéMON on this adventure!$"
DynamaxAdventureRestCave_Text_Swapped:
    .string "All set! Your chosen POKéMON will\n"
    .string "join you in the next battle.$"
DynamaxAdventureRestCave_Text_BirchMay:
    .string "I'm here to shadow MAY and see how\n"
    .string "she's doing with her research.\p"
    .string "She's working toward becoming a\n"
    .string "professor. I'm excited to see her grow!\p"
    .string "I'll accompany you between battles.\n"
    .string "I can help switch your caught team!$"
DynamaxAdventureRestCave_Text_BirchBrendan:
    .string "I'm here to shadow BRENDAN and see how\n"
    .string "he's doing with his research.\p"
    .string "He's working toward becoming a\n"
    .string "professor. I'm excited to see him grow!\p"
    .string "I'll accompany you between battles.\n"
    .string "I can help switch your caught team!$"
