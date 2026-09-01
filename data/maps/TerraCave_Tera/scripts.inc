TerraCave_Tera_MapScripts::
	.byte 0
TerraCave_Tera_EventScript_TERAPAGOS::
	lockall
	waitse
	playmoncry SPECIES_TERAPAGOS, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setwildbattle SPECIES_TERAPAGOS, 40
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, TerraCave_Tera_EventScript_DefeatedTERAPAGOS
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, TerraCave_Tera_EventScript_RanFromTERAPAGOS
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, TerraCave_Tera_EventScript_RanFromTERAPAGOS
	setflag FLAG_DEFEATED_TERAPAGOS
	releaseall
	end

TerraCave_Tera_EventScript_DefeatedTERAPAGOS::
	setflag FLAG_DEFEATED_TERAPAGOS
	goto TerraCave_Tera_EventScript_DefeatedTERAPAGOS2
	end

TerraCave_Tera_EventScript_RanFromTERAPAGOS::
	setvar VAR_0x8004, SPECIES_TERAPAGOS
	goto TerraCave_Tera_EventScript_RanFromTERAPAGOS2
	end

TerraCave_Tera_EventScript_DefeatedTERAPAGOS2::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject VAR_LAST_TALKED
	fadescreenswapbuffers FADE_FROM_BLACK
	releaseall
	end

TerraCave_Tera_EventScript_RanFromTERAPAGOS2::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject VAR_LAST_TALKED
	fadescreenswapbuffers FADE_FROM_BLACK
	bufferspeciesname STR_VAR_1, VAR_0x8004
	msgbox gText_LegendaryFlewAway, MSGBOX_DEFAULT
	releaseall
	end