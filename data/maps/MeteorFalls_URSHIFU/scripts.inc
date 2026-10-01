MeteorFalls_URSHIFU_MapScripts::
	.byte 0

MeteorFalls_URSHIFU_EventScript_KUBFU::
    lockall
    waitse
    playmoncry SPECIES_KUBFU, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_KUBFU, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, MeteorFalls_URSHIFU_EventScript_CaughtKUBFU

    releaseall
    end

MeteorFalls_URSHIFU_EventScript_CaughtKUBFU::
    setflag FLAG_DEFEATED_KUBFU 
    removeobject VAR_LAST_TALKED
    releaseall
    end


MeteorFalls_URSHIFU_EventScript_KUBFU2::
    lockall
    waitse
    playmoncry SPECIES_KUBFU, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_KUBFU, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, MeteorFalls_URSHIFU_EventScript_CaughtKUBFU2

    releaseall
    end

MeteorFalls_URSHIFU_EventScript_CaughtKUBFU2::
    setflag FLAG_DEFEATED_KUBFUT 
    removeobject VAR_LAST_TALKED
    releaseall
    end
