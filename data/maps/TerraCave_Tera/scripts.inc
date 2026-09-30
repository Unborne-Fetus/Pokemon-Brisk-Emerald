TerraCave_Tera_MapScripts::
	.byte 0

TerraCave_Tera_EventScript_TERAPAGOS::
	lockall
	waitse
	playmoncry SPECIES_TERAPAGOS, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setwildbattle SPECIES_TERAPAGOS, 70
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, TerraCave_Tera_EventScript_CaughtTERAPAGOS
	releaseall
	end

TerraCave_Tera_EventScript_CaughtTERAPAGOS::
	setflag FLAG_HIDE_TERRA_CAVE_TERAPAGOS
	releaseall
	end

