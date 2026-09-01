ArtisanCave_B1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ArtisanCave_B1F_OnTransition
	.byte 0

ArtisanCave_B1F_OnTransition:
	setflag FLAG_LANDMARK_ARTISAN_CAVE
	end


ArtisanCave_B1F_EventScript_MELOETTA::
	lockall
	waitse
	playmoncry SPECIES_MELOETTA, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setwildbattle SPECIES_MELOETTA, 40
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, ArtisanCave_B1F_EventScript_DefeatedMELOETTA
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, ArtisanCave_B1F_EventScript_RanFromMELOETTA
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, ArtisanCave_B1F_EventScript_RanFromMELOETTA
	setflag FLAG_DEFEATED_MELOETTA
	releaseall
	end

ArtisanCave_B1F_EventScript_DefeatedMELOETTA::
	setflag FLAG_DEFEATED_MELOETTA
	goto ArtisanCave_B1F_EventScript_DefeatedMELOETTA2
	end

ArtisanCave_B1F_EventScript_RanFromMELOETTA::
	setvar VAR_0x8004, SPECIES_MELOETTA
	goto ArtisanCave_B1F_EventScript_RanFromMELOETTA2
	end

ArtisanCave_B1F_EventScript_DefeatedMELOETTA2::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject VAR_LAST_TALKED
	fadescreenswapbuffers FADE_FROM_BLACK
	releaseall
	end

ArtisanCave_B1F_EventScript_RanFromMELOETTA2::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject VAR_LAST_TALKED
	fadescreenswapbuffers FADE_FROM_BLACK
	bufferspeciesname STR_VAR_1, VAR_0x8004
	msgbox gText_LegendaryFlewAway, MSGBOX_DEFAULT
	releaseall
	end