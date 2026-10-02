NewMauville_Inside_2_MapScripts::
	.byte 0


NewMauville_Inside_2_EventScript_RAIKOU::
    lockall
    waitse
    playmoncry SPECIES_RAIKOU, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_RAIKOU, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, NewMauville_Inside_2_EventScript_CaughtRAIKOU

    releaseall
    end

NewMauville_Inside_2_EventScript_CaughtRAIKOU::
    setflag FLAG_HIDE_NEWMAUVILLE_RAIKOU
    removeobject VAR_LAST_TALKED
    releaseall
    end