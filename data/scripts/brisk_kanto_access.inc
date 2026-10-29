@ Kanto's original map sources remain unchanged. Its campaign will be ported
@ separately; do not run the template's colliding Hoenn variables/zero flags.
BriskKantoAccessOnly_MapScripts::
	.byte 0

BriskKantoAccessOnly_EventScript_NotReady::
	msgbox BriskKantoAccessOnly_Text_NotReady, MSGBOX_DEFAULT
	releaseall
	end

BriskKantoAccessOnly_Text_NotReady:
	.string "KANTO is open for exploration.\p"
	.string "Its story, battles, and services\n"
	.string "are not available yet.$"
