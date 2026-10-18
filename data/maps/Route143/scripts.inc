Route143_MapScripts::
	.byte 0

Route143_EventScript_RouteSign::
	msgbox Route143_Text_RouteSign, MSGBOX_SIGN
	end

Route143_Text_RouteSign:
	.string "ROUTE 143 - ORANGE ISLANDS\n"
	.string "East: Route 144 and ORANGE CITY$"

Route143_EventScript_Landing::
	msgbox Route143_Text_Landing, MSGBOX_NPC
	end

Route143_Text_Landing:
	.string "The sea is closed for now.\p"
	.string "There was a collision with a\n"
	.string "boat called the KURAMA.\p"
	.string "Heavy whirlpools have made\n"
	.string "the surrounding waters unsafe.\p"
	.string "Please stay on the island\n"
	.string "until the sea is safe again.$"
