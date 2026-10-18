Route144_MapScripts::
	.byte 0

Route144_EventScript_RouteSign::
	msgbox Route144_Text_RouteSign, MSGBOX_SIGN
	end

Route144_Text_RouteSign:
	.string "ROUTE 144 - TIDAL BOARDWALK\n"
	.string "East: ORANGE CITY$"

Route144_EventScript_Tide::
	msgbox Route144_Text_Tide, MSGBOX_NPC
	end

Route144_Text_Tide:
	.string "Beautiful life thrives here in\n"
	.string "the ORANGE ISLANDS.\p"
	.string "Some of it is found nowhere\n"
	.string "else in the world!\p"
	.string "At night, moonlight and stars\n"
	.string "make the islands breathtaking.$"
