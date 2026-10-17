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
    checkplayergender
    goto_if_eq VAR_RESULT, FEMALE, DynamaxAdventureRestCave_BirchBrendan
    msgbox DynamaxAdventureRestCave_Text_BirchMay, MSGBOX_NPC
    end
DynamaxAdventureRestCave_BirchBrendan:
    msgbox DynamaxAdventureRestCave_Text_BirchBrendan, MSGBOX_NPC
    end
DynamaxAdventureRestCave_Text_BirchMay:
    .string "I'm here to shadow MAY and see how\n"
    .string "she's doing with her research.\p"
    .string "She's working toward becoming a\n"
    .string "professor. I'm excited to see her grow!$"
DynamaxAdventureRestCave_Text_BirchBrendan:
    .string "I'm here to shadow BRENDAN and see how\n"
    .string "he's doing with his research.\p"
    .string "He's working toward becoming a\n"
    .string "professor. I'm excited to see him grow!$"
