Route146_MapScripts::
	.byte 0

Route146_EventScript_RouteSign::
	msgbox Route146_Text_RouteSign, MSGBOX_SIGN
	end

Route146_Text_RouteSign:
	.string "ROUTE 146 - CRESCENT LOOKOUT\n"
	.string "A lookout over the\nORANGE ISLANDS.$"

Route146_EventScript_Lookout::
	msgbox Route146_Text_Lookout, MSGBOX_NPC
	end

Route146_Text_Lookout:
	.string "From here, the islands\n"
	.string "looks like a string of orange pearls.$"
