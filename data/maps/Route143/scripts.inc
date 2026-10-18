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
	.string "These boardwalks keep our beaches\n"
	.string "open to everyone. Enjoy the islands!$"
