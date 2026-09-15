






TerraCave_Tera_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, TerraCave_Tera_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, TerraCave_Tera_OnTransition
	.byte 0

TerraCave_Tera_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, TerraCave_Tera_EventScript_TryRemoveTerapagos
	end

TerraCave_Tera_EventScript_TryRemoveTerapagos::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject LOCALID_TERRA_CAVE_TERAPAGOS
	return

TerraCave_Tera_OnTransition:
	call_if_unset FLAG_DEFEATED_TERAPAGOS, TerraCave_Tera_EventScript_ShowTerapagos
	end

TerraCave_Tera_EventScript_ShowTerapagos::
	clearflag FLAG_HIDE_TERRA_CAVE_TERAPAGOS
	setvar VAR_TEMP_1, 1
	return

TerraCave_Tera_EventScript_Terapagos::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	applymovement LOCALID_TERRA_CAVE_TERAPAGOS, TerraCave_Tera_Movement_TerapagosApproach
	waitmovement 0
	waitse
	playmoncry SPECIES_TERAPAGOS, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_LAST_TALKED, LOCALID_TERRA_CAVE_TERAPAGOS
	setwildbattle SPECIES_TERAPAGOS, 70
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	setvar VAR_TEMP_1, 0
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, TerraCave_Tera_EventScript_DefeatedTerapagos
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, TerraCave_Tera_EventScript_RanFromTerapagos
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, TerraCave_Tera_EventScript_RanFromTerapagos
	setvar VAR_SHOULD_Tera_ABNORMAL_WEATHER, 1
	setflag FLAG_DEFEATED_TERAPAGOS
	releaseall
	end

TerraCave_Tera_EventScript_DefeatedTerapagos::
	setvar VAR_SHOULD_Tera_ABNORMAL_WEATHER, 1
	setflag FLAG_DEFEATED_TERAPAGOS
	goto Common_EventScript_RemoveStaticPokemon
	end

TerraCave_Tera_EventScript_RanFromTerapagos::
	setvar VAR_0x8004, SPECIES_TERAPAGOS
	goto Common_EventScript_LegendaryFlewAway
	end

TerraCave_Tera_Movement_TerapagosApproach:
	init_affine_anim
	walk_down_start_affine
	delay_16
	delay_16
	walk_down_affine
	delay_16
	delay_16
	walk_down_affine
	step_end





