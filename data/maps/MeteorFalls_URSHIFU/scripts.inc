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
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, MeteorFalls_URSHIFU_EventScript_CaughtKubfu
	releaseall
	end

MeteorFalls_URSHIFU_EventScript_CaughtKubfu::
	goto_if_set FLAG_CAUGHT_KUBFU_ONCE, MeteorFalls_URSHIFU_EventScript_CaughtKubfuTwice
	setflag FLAG_CAUGHT_KUBFU_ONCE
	releaseall
	end

MeteorFalls_URSHIFU_EventScript_CaughtKubfuTwice::
	setflag FLAG_DEFEATED_KUBFU
	releaseall
	end
