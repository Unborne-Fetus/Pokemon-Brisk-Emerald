OrangeCity_MapScripts::
	.byte 0

OrangeCity_EventScript_TownSign::
	msgbox OrangeCity_Text_TownSign, MSGBOX_SIGN
	end

OrangeCity_Text_TownSign:
	.string "ORANGE CITY\n"
	.string "A harbor of the ORANGE ISLANDS.$"

OrangeCity_EventScript_Welcome::
	msgbox OrangeCity_Text_Welcome, MSGBOX_NPC
	end

OrangeCity_Text_Welcome:
	.string "Welcome, CHAMPION! Take a rest\n"
	.string "before exploring the eastern islands.$"

OrangeCity_EventScript_Harbor::
	msgbox OrangeCity_Text_Harbor, MSGBOX_NPC
	end

OrangeCity_Text_Harbor:
	.string "West lies the tidal boardwalk.\n"
	.string "East lies the crescent-island lookout.$"

OrangeCity_EventScript_Town::
	msgbox OrangeCity_Text_Town, MSGBOX_NPC
	end

OrangeCity_Text_Town:
	.string "The town square leads to the\n"
	.string "harbor and our island homes.$"

OrangeCity_EventScript_HoodedFigure::
	lock
	goto_if_ne VAR_BRISK_DIFFICULTY, BRISK_MODE_KAIZO, OrangeCity_EventScript_HoodedFigureIgnored
	faceplayer
	msgbox OrangeCity_Text_HoodedFigureChallenge, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, OrangeCity_EventScript_HoodedFigureLeave
	cleartrainerflag TRAINER_HOODED_FIGURE
	trainerbattle_no_intro TRAINER_HOODED_FIGURE, OrangeCity_Text_HoodedFigureDefeat
	msgbox OrangeCity_Text_HoodedFigureAfter, MSGBOX_DEFAULT
OrangeCity_EventScript_HoodedFigureLeave::
	release
	end

OrangeCity_EventScript_HoodedFigureIgnored::
	msgbox OrangeCity_Text_HoodedFigureIgnored, MSGBOX_DEFAULT
	release
	end

OrangeCity_Text_HoodedFigureChallenge:
	.string "Hooded Figure: You seek\n"
	.string "the highest challenge?\p"
	.string "Face my level-100 team?$"

OrangeCity_Text_HoodedFigureIgnored:
	.string "Hooded Figure pays you no mind.$"

OrangeCity_Text_HoodedFigureDefeat:
	.string "You have earned my respect.$"

OrangeCity_Text_HoodedFigureAfter:
	.string "Return whenever you seek\n"
	.string "another challenge.$"

OrangeCity_EventScript_Tankerson::
	lock
	goto_if_ne VAR_BRISK_DIFFICULTY, BRISK_MODE_KAIZO, OrangeCity_EventScript_TankersonIgnored
	faceplayer
	msgbox OrangeCity_Text_TankersonChallenge, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, OrangeCity_EventScript_TankersonLeave
	special HasEnoughMonsForDoubleBattle
	goto_if_ne VAR_RESULT, PLAYER_HAS_TWO_USABLE_MONS, OrangeCity_EventScript_TankersonNeedsPartner
	cleartrainerflag TRAINER_TANKERSON
	trainerbattle_no_intro TRAINER_TANKERSON, OrangeCity_Text_TankersonDefeat
	msgbox OrangeCity_Text_TankersonAfter, MSGBOX_DEFAULT
OrangeCity_EventScript_TankersonLeave::
	release
	end

OrangeCity_EventScript_TankersonIgnored::
	msgbox OrangeCity_Text_TankersonIgnored, MSGBOX_DEFAULT
	release
	end

OrangeCity_Text_TankersonChallenge:
	.string "Tankerson: Let us test\n"
	.string "your team in doubles.\p"
	.string "Face my level-100 team?$"

OrangeCity_Text_TankersonIgnored:
	.string "Tankerson pays you no mind.$"

OrangeCity_Text_TankersonDefeat:
	.string "You have earned my respect.$"

OrangeCity_Text_TankersonAfter:
	.string "Return whenever you seek\n"
	.string "another challenge.$"

OrangeCity_EventScript_TankersonNeedsPartner::
	msgbox OrangeCity_Text_TankersonNeedsPartner, MSGBOX_DEFAULT
	release
	end

OrangeCity_Text_TankersonNeedsPartner:
	.string "Bring at least two POKéMON\n"
	.string "that can battle. This is doubles.$"
