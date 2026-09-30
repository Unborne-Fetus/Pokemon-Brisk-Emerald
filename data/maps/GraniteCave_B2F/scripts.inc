GraniteCave_B2F_MapScripts::
    .byte 0

GraniteCave_B2F_EventScript_LATIOS::
    lockall
    waitse
    playmoncry SPECIES_LATIOS, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_LATIOS, 40
    setflag FLAG_SYS_CTRL_OBJ_DELETE
    special BattleSetup_StartLegendaryBattle
    clearflag FLAG_SYS_CTRL_OBJ_DELETE
    specialvar VAR_RESULT, GetBattleOutcome

    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, GraniteCave_B2F_EventScript_CaughtLATIOS
    releaseall
    end

GraniteCave_B2F_EventScript_CaughtLATIOS::
    setflag FLAG_DEFEATED_GRANITE_CAVE_LATIOS
    releaseall
    end
