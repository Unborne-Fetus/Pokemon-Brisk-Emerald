ShoalCave_LowTideIceRoom_2_MapScripts::
	.byte 0


ShoalCave_LowTideIceRoom_EventScript_CHIEN::
    lockall
    waitse
    playmoncry SPECIES_CHIEN_PAO, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_CHIEN_PAO, 45
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, ShoalCave_LowTideIceRoom_EventScript_CaughtCHIEN

    releaseall
    end

ShoalCave_LowTideIceRoom_EventScript_CaughtCHIEN::
    setflag FLAG_HIDE_SC_CHIEN
    removeobject VAR_LAST_TALKED
    releaseall
    end




	ShoalCave_LowTideIceRoom_EventScript_ARTICUNO::
    lockall
    waitse
    playmoncry SPECIES_ARTICUNO_GALAR, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_ARTICUNO_GALAR, 45
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, ShoalCave_LowTideIceRoom_EventScript_CaughtARTICUNO

    releaseall
    end

ShoalCave_LowTideIceRoom_EventScript_CaughtARTICUNO::
    setflag FLAG_HIDE_SC_ARTICUNO
    removeobject VAR_LAST_TALKED
    releaseall
    end