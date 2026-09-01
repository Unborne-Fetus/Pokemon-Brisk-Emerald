MeteorFalls_URSHIFU_MapScripts::
	.byte 0

MeteorFalls_URSHIFU_EventScript_URSHIFU::
	lockall
	waitse
	playmoncry SPECIES_KUBFU, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setwildbattle SPECIES_KUBFU, 40
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, MeteorFalls_URSHIFU_EventScript_DefeatedKubfu
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, MeteorFalls_URSHIFU_EventScript_RanFromKubfu
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, MeteorFalls_URSHIFU_EventScript_RanFromKubfu
	setflag FLAG_DEFEATED_KUBFU
	releaseall
	end

MeteorFalls_URSHIFU_EventScript_DefeatedKubfu::
	setflag FLAG_DEFEATED_KUBFU
	goto MeteorFalls_URSHIFU_EventScript_DefeatedKubfu2
	end

MeteorFalls_URSHIFU_EventScript_RanFromKubfu::
	setvar VAR_0x8004, SPECIES_KUBFU
	goto MeteorFalls_URSHIFU_EventScript_RanFromKubfu2
	end

MeteorFalls_URSHIFU_EventScript_DefeatedKubfu2::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject VAR_LAST_TALKED
	fadescreenswapbuffers FADE_FROM_BLACK
	releaseall
	end

MeteorFalls_URSHIFU_EventScript_RanFromKubfu2::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject VAR_LAST_TALKED
	fadescreenswapbuffers FADE_FROM_BLACK
	bufferspeciesname STR_VAR_1, VAR_0x8004
	msgbox gText_LegendaryFlewAway, MSGBOX_DEFAULT
	releaseall
	end