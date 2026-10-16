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
