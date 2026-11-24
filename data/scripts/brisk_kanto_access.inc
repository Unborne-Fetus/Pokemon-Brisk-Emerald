@ Kanto's story is omitted in Brisk Emerald. Do not re-enable original FRLG
@ rivals, Rocket scenes or badges: their flags and trainer IDs overlap Hoenn.
@ Authored postgame challenges use separate rosters and saved defeat records.
BriskKantoAccessOnly_MapScripts::
	.byte 0

BriskKantoAccessOnly_EventScript_NotReady::
	msgbox BriskKantoAccessOnly_Text_NotReady, MSGBOX_DEFAULT
	releaseall
	end

BriskKantoAccessOnly_EventScript_ChallengePending::
	msgbox BriskKantoAccessOnly_Text_ChallengePending, MSGBOX_DEFAULT
	releaseall
	end

BriskKantoAccessOnly_Text_NotReady:
	.string "Welcome to KANTO, HOENN CHAMPION!\p"
	.string "Ten years since TEAM ROCKET\n"
	.string "dissolved, wild POKéMON and\p"
	.string "Trainers have grown stronger.$"

BriskKantoAccessOnly_Text_ChallengePending:
	.string "TEAM ROCKET fell ten years ago.\p"
	.string "KANTO's POKéMON grew stronger\n"
	.string "in peace. Enjoy the challenge!$"

@ Kanto elevators use their native menus but no imported quest state.
BriskKantoAccessOnly_EventScript_RocketLift::
	lockall
	message gText_WantWhichFloor
	waitmessage
	multichoice 0, 0, MULTI_ROCKET_HIDEOUT_ELEVATOR, FALSE
	switch VAR_RESULT
	case 0, BriskKantoAccessOnly_EventScript_RocketLift_Floor1
	case 1, BriskKantoAccessOnly_EventScript_RocketLift_Floor2
	case 2, BriskKantoAccessOnly_EventScript_RocketLift_Floor4
	goto BriskKantoAccessOnly_EventScript_LiftDone

BriskKantoAccessOnly_EventScript_RocketLift_Floor1::
	setdynamicwarp MAP_ROCKET_HIDEOUT_B1F, 255, 24, 25
	goto BriskKantoAccessOnly_EventScript_LiftDone

BriskKantoAccessOnly_EventScript_RocketLift_Floor2::
	setdynamicwarp MAP_ROCKET_HIDEOUT_B2F, 255, 28, 16
	goto BriskKantoAccessOnly_EventScript_LiftDone

BriskKantoAccessOnly_EventScript_RocketLift_Floor4::
	setdynamicwarp MAP_ROCKET_HIDEOUT_B4F, 255, 20, 23
	goto BriskKantoAccessOnly_EventScript_LiftDone

BriskKantoAccessOnly_EventScript_CeladonLift::
	lockall
	message gText_WantWhichFloor
	waitmessage
	multichoice 0, 0, MULTI_DEPT_STORE_ELEVATOR, FALSE
	switch VAR_RESULT
	case 0, BriskKantoAccessOnly_EventScript_CeladonLift_Floor5
	case 1, BriskKantoAccessOnly_EventScript_CeladonLift_Floor4
	case 2, BriskKantoAccessOnly_EventScript_CeladonLift_Floor3
	case 3, BriskKantoAccessOnly_EventScript_CeladonLift_Floor2
	case 4, BriskKantoAccessOnly_EventScript_CeladonLift_Floor1
	goto BriskKantoAccessOnly_EventScript_LiftDone

BriskKantoAccessOnly_EventScript_CeladonLift_Floor5::
	setdynamicwarp MAP_CELADON_CITY_DEPARTMENT_STORE_5F, 255, 6, 1
	goto BriskKantoAccessOnly_EventScript_LiftDone

BriskKantoAccessOnly_EventScript_CeladonLift_Floor4::
	setdynamicwarp MAP_CELADON_CITY_DEPARTMENT_STORE_4F, 255, 6, 1
	goto BriskKantoAccessOnly_EventScript_LiftDone

BriskKantoAccessOnly_EventScript_CeladonLift_Floor3::
	setdynamicwarp MAP_CELADON_CITY_DEPARTMENT_STORE_3F, 255, 6, 1
	goto BriskKantoAccessOnly_EventScript_LiftDone

BriskKantoAccessOnly_EventScript_CeladonLift_Floor2::
	setdynamicwarp MAP_CELADON_CITY_DEPARTMENT_STORE_2F, 255, 6, 1
	goto BriskKantoAccessOnly_EventScript_LiftDone

BriskKantoAccessOnly_EventScript_CeladonLift_Floor1::
	setdynamicwarp MAP_CELADON_CITY_DEPARTMENT_STORE_1F, 255, 6, 1
	goto BriskKantoAccessOnly_EventScript_LiftDone

BriskKantoAccessOnly_EventScript_SilphLift::
	lockall
	message gText_WantWhichFloor
	waitmessage
	setvar VAR_0x8004, SCROLL_MULTI_SILPHCO_FLOORS
	special ShowScrollableMultichoice
	waitstate
	switch VAR_RESULT
	case 0, BriskKantoAccessOnly_EventScript_SilphLift_Floor11
	case 1, BriskKantoAccessOnly_EventScript_SilphLift_Floor10
	case 2, BriskKantoAccessOnly_EventScript_SilphLift_Floor9
	case 3, BriskKantoAccessOnly_EventScript_SilphLift_Floor8
	case 4, BriskKantoAccessOnly_EventScript_SilphLift_Floor7
	case 5, BriskKantoAccessOnly_EventScript_SilphLift_Floor6
	case 6, BriskKantoAccessOnly_EventScript_SilphLift_Floor5
	case 7, BriskKantoAccessOnly_EventScript_SilphLift_Floor4
	case 8, BriskKantoAccessOnly_EventScript_SilphLift_Floor3
	case 9, BriskKantoAccessOnly_EventScript_SilphLift_Floor2
	case 10, BriskKantoAccessOnly_EventScript_SilphLift_Floor1
	goto BriskKantoAccessOnly_EventScript_LiftDone

BriskKantoAccessOnly_EventScript_SilphLift_Floor11::
	setdynamicwarp MAP_SILPH_CO_11F, 255, 13, 3
	goto BriskKantoAccessOnly_EventScript_LiftDone

BriskKantoAccessOnly_EventScript_SilphLift_Floor10::
	setdynamicwarp MAP_SILPH_CO_10F, 255, 13, 3
	goto BriskKantoAccessOnly_EventScript_LiftDone

BriskKantoAccessOnly_EventScript_SilphLift_Floor9::
	setdynamicwarp MAP_SILPH_CO_9F, 255, 24, 3
	goto BriskKantoAccessOnly_EventScript_LiftDone

BriskKantoAccessOnly_EventScript_SilphLift_Floor8::
	setdynamicwarp MAP_SILPH_CO_8F, 255, 22, 3
	goto BriskKantoAccessOnly_EventScript_LiftDone

BriskKantoAccessOnly_EventScript_SilphLift_Floor7::
	setdynamicwarp MAP_SILPH_CO_7F, 255, 23, 3
	goto BriskKantoAccessOnly_EventScript_LiftDone

BriskKantoAccessOnly_EventScript_SilphLift_Floor6::
	setdynamicwarp MAP_SILPH_CO_6F, 255, 20, 3
	goto BriskKantoAccessOnly_EventScript_LiftDone

BriskKantoAccessOnly_EventScript_SilphLift_Floor5::
	setdynamicwarp MAP_SILPH_CO_5F, 255, 22, 3
	goto BriskKantoAccessOnly_EventScript_LiftDone

BriskKantoAccessOnly_EventScript_SilphLift_Floor4::
	setdynamicwarp MAP_SILPH_CO_4F, 255, 22, 3
	goto BriskKantoAccessOnly_EventScript_LiftDone

BriskKantoAccessOnly_EventScript_SilphLift_Floor3::
	setdynamicwarp MAP_SILPH_CO_3F, 255, 22, 3
	goto BriskKantoAccessOnly_EventScript_LiftDone

BriskKantoAccessOnly_EventScript_SilphLift_Floor2::
	setdynamicwarp MAP_SILPH_CO_2F, 255, 22, 3
	goto BriskKantoAccessOnly_EventScript_LiftDone

BriskKantoAccessOnly_EventScript_SilphLift_Floor1::
	setdynamicwarp MAP_SILPH_CO_1F, 255, 22, 3
	goto BriskKantoAccessOnly_EventScript_LiftDone

BriskKantoAccessOnly_EventScript_LiftDone::
	closemessage
	releaseall
	end
