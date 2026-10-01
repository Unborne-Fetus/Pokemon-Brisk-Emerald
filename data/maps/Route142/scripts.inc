Route142_MapScripts::
	.byte 0

Route142_EventScript_URSALUNA::
    lockall
    waitse
    playmoncry SPECIES_URSALUNA_BLOODMOON, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_URSALUNA_BLOODMOON, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Route142_EventScript_CaughtURSALUNA

    releaseall
    end

Route142_EventScript_CaughtURSALUNA::
    setflag FLAG_DEFEATED_URSALUNA
    removeobject VAR_LAST_TALKED
    releaseall
    end