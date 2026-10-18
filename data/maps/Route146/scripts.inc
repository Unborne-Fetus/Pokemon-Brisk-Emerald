Route146_MapScripts::
	.byte 0

Route146_EventScript_RouteSign::
	msgbox Route146_Text_RouteSign, MSGBOX_SIGN
	end

Route146_Text_RouteSign:
	.string "ROUTE 146 - CRESCENT LOOKOUT\n"
	.string "A view across the island sea.$"

Route146_EventScript_Lookout::
	msgbox Route146_Text_Lookout, MSGBOX_NPC
	end

Route146_Text_Lookout:
	.string "On clear days, you can see\n"
	.string "Hoenn across the sea.\p"
	.string "Its distant mountains look\n"
	.string "blue against the horizon.$"
