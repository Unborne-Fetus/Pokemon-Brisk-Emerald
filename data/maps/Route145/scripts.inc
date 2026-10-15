Route145_MapScripts::
	.byte 0

Route145_EventScript_RouteSign::
	msgbox Route145_Text_RouteSign, MSGBOX_SIGN
	end

Route145_Text_RouteSign:
	.string "ROUTE 145 - PALM CAPE\n"
	.string "West: ORANGE CITY  East: Route 146$"

Route145_EventScript_Cape::
	msgbox Route145_Text_Cape, MSGBOX_NPC
	end

Route145_Text_Cape:
	.string "Cast a line from the shore, or\n"
	.string "follow the trail to the lookout.$"
