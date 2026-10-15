OrangeCity_MapScripts::
	.byte 0

OrangeCity_EventScript_TownSign::
	msgbox OrangeCity_Text_TownSign, MSGBOX_SIGN
	end

OrangeCity_Text_TownSign:
	.string "ORANGE CITY\n"
	.string "A bright harbor on the Orange Coast.$"

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
	.string "The houses here are built with\n"
	.string "local bamboo and warm orange roofs.$"
