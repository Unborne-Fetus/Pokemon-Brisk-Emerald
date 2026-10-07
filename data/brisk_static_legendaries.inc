// Brisk Emerald repeatable static legendary encounters.
// Mirrors the Route142 Blood Moon Ursaluna behavior: the object remains available
// unless the Pokémon is caught, so losses / fleeing do not consume the encounter.

BriskStatic_EventScript_UXIE::
    lockall
    waitse
    playmoncry SPECIES_UXIE, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_UXIE, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_UXIE

    releaseall
    end

BriskStatic_EventScript_Caught_UXIE::
    setflag FLAG_DEFEATED_UXIE
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_MESPRIT::
    lockall
    waitse
    playmoncry SPECIES_MESPRIT, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_MESPRIT, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_MESPRIT

    releaseall
    end

BriskStatic_EventScript_Caught_MESPRIT::
    setflag FLAG_DEFEATED_MESPRIT
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_AZELF::
    lockall
    waitse
    playmoncry SPECIES_AZELF, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_AZELF, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_AZELF

    releaseall
    end

BriskStatic_EventScript_Caught_AZELF::
    setflag FLAG_DEFEATED_AZELF
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_DIALGA::
    lockall
    waitse
    playmoncry SPECIES_DIALGA, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_DIALGA, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_DIALGA

    releaseall
    end

BriskStatic_EventScript_Caught_DIALGA::
    setflag FLAG_DEFEATED_DIALGA
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_PALKIA::
    lockall
    waitse
    playmoncry SPECIES_PALKIA, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_PALKIA, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_PALKIA

    releaseall
    end

BriskStatic_EventScript_Caught_PALKIA::
    setflag FLAG_DEFEATED_PALKIA
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_HEATRAN::
    lockall
    waitse
    playmoncry SPECIES_HEATRAN, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_HEATRAN, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_HEATRAN

    releaseall
    end

BriskStatic_EventScript_Caught_HEATRAN::
    setflag FLAG_DEFEATED_HEATRAN
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_TAPU_LELE::
    lockall
    waitse
    playmoncry SPECIES_TAPU_LELE, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_TAPU_LELE, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_TAPU_LELE

    releaseall
    end

BriskStatic_EventScript_Caught_TAPU_LELE::
    setflag FLAG_DEFEATED_TAPU_LELE
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_CRESSELIA::
    lockall
    waitse
    playmoncry SPECIES_CRESSELIA, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_CRESSELIA, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_CRESSELIA

    releaseall
    end

BriskStatic_EventScript_Caught_CRESSELIA::
    setflag FLAG_DEFEATED_CRESSELIA
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_PECHARUNT::
    lockall
    waitse
    playmoncry SPECIES_PECHARUNT, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_PECHARUNT, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_PECHARUNT

    releaseall
    end

BriskStatic_EventScript_Caught_PECHARUNT::
    setflag FLAG_DEFEATED_PECHARUNT
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_CALYREX::
    lockall
    waitse
    playmoncry SPECIES_CALYREX, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_CALYREX, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_CALYREX

    releaseall
    end

BriskStatic_EventScript_Caught_CALYREX::
    setflag FLAG_DEFEATED_CALYREX
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_MAGEARNA::
    lockall
    waitse
    playmoncry SPECIES_MAGEARNA, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_MAGEARNA, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_MAGEARNA

    releaseall
    end

BriskStatic_EventScript_Caught_MAGEARNA::
    setflag FLAG_DEFEATED_MAGEARNA
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_SPECTRIER::
    lockall
    waitse
    playmoncry SPECIES_SPECTRIER, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_SPECTRIER, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_SPECTRIER

    releaseall
    end

BriskStatic_EventScript_Caught_SPECTRIER::
    setflag FLAG_DEFEATED_SPECTRIER
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_SHAYMIN::
    lockall
    waitse
    playmoncry SPECIES_SHAYMIN, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_SHAYMIN, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_SHAYMIN

    releaseall
    end

BriskStatic_EventScript_Caught_SHAYMIN::
    setflag FLAG_DEFEATED_SHAYMIN
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_ARTICUNO::
    lockall
    waitse
    playmoncry SPECIES_ARTICUNO, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_ARTICUNO, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_ARTICUNO

    releaseall
    end

BriskStatic_EventScript_Caught_ARTICUNO::
    setflag FLAG_DEFEATED_ARTICUNO
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_MOLTRES::
    lockall
    waitse
    playmoncry SPECIES_MOLTRES, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_MOLTRES, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_MOLTRES

    releaseall
    end

BriskStatic_EventScript_Caught_MOLTRES::
    setflag FLAG_DEFEATED_MOLTRES
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_WO_CHIEN::
    lockall
    waitse
    playmoncry SPECIES_WO_CHIEN, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_WO_CHIEN, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_WO_CHIEN

    releaseall
    end

BriskStatic_EventScript_Caught_WO_CHIEN::
    setflag FLAG_DEFEATED_WO_CHIEN
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_ENTEI::
    lockall
    waitse
    playmoncry SPECIES_ENTEI, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_ENTEI, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_ENTEI

    releaseall
    end

BriskStatic_EventScript_Caught_ENTEI::
    setflag FLAG_DEFEATED_ENTEI
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_SUICUNE::
    lockall
    waitse
    playmoncry SPECIES_SUICUNE, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_SUICUNE, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_SUICUNE

    releaseall
    end

BriskStatic_EventScript_Caught_SUICUNE::
    setflag FLAG_DEFEATED_SUICUNE
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_MUNKIDORI::
    lockall
    waitse
    playmoncry SPECIES_MUNKIDORI, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_MUNKIDORI, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_MUNKIDORI

    releaseall
    end

BriskStatic_EventScript_Caught_MUNKIDORI::
    setflag FLAG_DEFEATED_MUNKIDORI
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_FEZANDIPITI::
    lockall
    waitse
    playmoncry SPECIES_FEZANDIPITI, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_FEZANDIPITI, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_FEZANDIPITI

    releaseall
    end

BriskStatic_EventScript_Caught_FEZANDIPITI::
    setflag FLAG_DEFEATED_FEZANDIPITI
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_CELEBI::
    lockall
    waitse
    playmoncry SPECIES_CELEBI, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_CELEBI, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_CELEBI

    releaseall
    end

BriskStatic_EventScript_Caught_CELEBI::
    setflag FLAG_DEFEATED_CELEBI
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_TING_LU::
    lockall
    waitse
    playmoncry SPECIES_TING_LU, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_TING_LU, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_TING_LU

    releaseall
    end

BriskStatic_EventScript_Caught_TING_LU::
    setflag FLAG_DEFEATED_TING_LU
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_LANDORUS::
    lockall
    waitse
    playmoncry SPECIES_LANDORUS, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_LANDORUS, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_LANDORUS

    releaseall
    end

BriskStatic_EventScript_Caught_LANDORUS::
    setflag FLAG_DEFEATED_LANDORUS
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_CHI_YU::
    lockall
    waitse
    playmoncry SPECIES_CHI_YU, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_CHI_YU, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_CHI_YU

    releaseall
    end

BriskStatic_EventScript_Caught_CHI_YU::
    setflag FLAG_DEFEATED_CHI_YU
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_VICTINI::
    lockall
    waitse
    playmoncry SPECIES_VICTINI, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_VICTINI, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_VICTINI

    releaseall
    end

BriskStatic_EventScript_Caught_VICTINI::
    setflag FLAG_DEFEATED_VICTINI
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_COBALION::
    lockall
    waitse
    playmoncry SPECIES_COBALION, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_COBALION, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_COBALION

    releaseall
    end

BriskStatic_EventScript_Caught_COBALION::
    setflag FLAG_DEFEATED_COBALION
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_TAPU_BULU::
    lockall
    waitse
    playmoncry SPECIES_TAPU_BULU, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_TAPU_BULU, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_TAPU_BULU

    releaseall
    end

BriskStatic_EventScript_Caught_TAPU_BULU::
    setflag FLAG_DEFEATED_TAPU_BULU
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_TAPU_FINI::
    lockall
    waitse
    playmoncry SPECIES_TAPU_FINI, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_TAPU_FINI, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_TAPU_FINI

    releaseall
    end

BriskStatic_EventScript_Caught_TAPU_FINI::
    setflag FLAG_DEFEATED_TAPU_FINI
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_THUNDURUS::
    lockall
    waitse
    playmoncry SPECIES_THUNDURUS, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_THUNDURUS, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_THUNDURUS

    releaseall
    end

BriskStatic_EventScript_Caught_THUNDURUS::
    setflag FLAG_DEFEATED_THUNDURUS
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_RESHIRAM::
    lockall
    waitse
    playmoncry SPECIES_RESHIRAM, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_RESHIRAM, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_RESHIRAM

    releaseall
    end

BriskStatic_EventScript_Caught_RESHIRAM::
    setflag FLAG_DEFEATED_RESHIRAM
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_ZEKROM::
    lockall
    waitse
    playmoncry SPECIES_ZEKROM, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_ZEKROM, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_ZEKROM

    releaseall
    end

BriskStatic_EventScript_Caught_ZEKROM::
    setflag FLAG_DEFEATED_ZEKROM
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_MOLTRES_GALAR::
    lockall
    waitse
    playmoncry SPECIES_MOLTRES_GALAR, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_MOLTRES_GALAR, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_MOLTRES_GALAR

    releaseall
    end

BriskStatic_EventScript_Caught_MOLTRES_GALAR::
    setflag FLAG_DEFEATED_MOLTRES_GALAR
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_ZAPDOS_GALAR::
    lockall
    waitse
    playmoncry SPECIES_ZAPDOS_GALAR, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_ZAPDOS_GALAR, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_ZAPDOS_GALAR

    releaseall
    end

BriskStatic_EventScript_Caught_ZAPDOS_GALAR::
    setflag FLAG_DEFEATED_ZAPDOS_GALAR
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_REGIDRAGO::
    lockall
    waitse
    playmoncry SPECIES_REGIDRAGO, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_REGIDRAGO, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_REGIDRAGO

    releaseall
    end

BriskStatic_EventScript_Caught_REGIDRAGO::
    setflag FLAG_DEFEATED_REGIDRAGO
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_OKIDOGI::
    lockall
    waitse
    playmoncry SPECIES_OKIDOGI, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_OKIDOGI, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_OKIDOGI

    releaseall
    end

BriskStatic_EventScript_Caught_OKIDOGI::
    setflag FLAG_DEFEATED_OKIDOGI
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_TAPU_KOKO::
    lockall
    waitse
    playmoncry SPECIES_TAPU_KOKO, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_TAPU_KOKO, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_TAPU_KOKO

    releaseall
    end

BriskStatic_EventScript_Caught_TAPU_KOKO::
    setflag FLAG_DEFEATED_TAPU_KOKO
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_XERNEAS::
    lockall
    waitse
    playmoncry SPECIES_XERNEAS, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_XERNEAS, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_XERNEAS

    releaseall
    end

BriskStatic_EventScript_Caught_XERNEAS::
    setflag FLAG_DEFEATED_XERNEAS
    removeobject VAR_LAST_TALKED
    releaseall
    end

BriskStatic_EventScript_YVELTAL::
    lockall
    waitse
    playmoncry SPECIES_YVELTAL, CRY_MODE_ENCOUNTER
    delay 40
    waitmoncry
    setwildbattle SPECIES_YVELTAL, 70
    special BattleSetup_StartLegendaryBattle

    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskStatic_EventScript_Caught_YVELTAL

    releaseall
    end

BriskStatic_EventScript_Caught_YVELTAL::
    setflag FLAG_DEFEATED_YVELTAL
    removeobject VAR_LAST_TALKED
    releaseall
    end

