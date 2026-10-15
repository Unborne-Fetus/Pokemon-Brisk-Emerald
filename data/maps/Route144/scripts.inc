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
	.string "The tide pools are full of life.\n"
	.string "Please leave the shoreline as you found it.$"
