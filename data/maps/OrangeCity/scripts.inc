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
