ShoalCave_LowTideIceRoom_MapScripts::
	.byte 0



ShoalCave_LowTideIceRoom_EventScript_GLASTRIER::
    lockall
    waitse
    playmoncry SPECIES_GLASTRIER, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_GLASTRIER, 45
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, ShoalCave_LowTideIceRoom_EventScript_CaughtGLASTRIER

    releaseall
    end

ShoalCave_LowTideIceRoom_EventScript_CaughtGLASTRIER::
    setflag FLAG_HIDE_SC_GLASTRIER
    removeobject VAR_LAST_TALKED
    releaseall
    end