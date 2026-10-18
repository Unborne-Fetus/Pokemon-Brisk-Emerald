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

Route145_EventScript_HexManiac::
	msgbox Route145_Text_HexManiac, MSGBOX_NPC
	end

Route145_Text_HexManiac:
	.string "About twenty years ago, LUGIA\n"
	.string "came to these islands.\p"
	.string "ARTICUNO, ZAPDOS, and MOLTRES\n"
	.string "flew at its side.\p"
	.string "They carried endangered\n"
	.string "species here from distant lands.\p"
	.string "Safe on these sheltered shores,\n"
	.string "those POKéMON began to thrive.\p"
	.string "It sparked a remarkable boom\n"
	.string "in the islands' biodiversity!\p"
	.string "Unusual colors are common here.\n"
	.string "A blessing from the birds?\p"
	.string "ISLAND SHINY in Options 5\n"
	.string "toggles the 1/50 shiny rate.$"
