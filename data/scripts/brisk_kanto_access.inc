@ Kanto's story is omitted in Brisk Emerald. Do not re-enable original FRLG
@ rivals, Rocket scenes or badges: their flags and trainer IDs overlap Hoenn.
@ Challenge NPCs remain visible until independent battle rosters are ported.
BriskKantoAccessOnly_MapScripts::
	.byte 0

BriskKantoAccessOnly_EventScript_NotReady::
	msgbox BriskKantoAccessOnly_Text_NotReady, MSGBOX_DEFAULT
	releaseall
	end

BriskKantoAccessOnly_EventScript_ChallengePending::
	msgbox BriskKantoAccessOnly_Text_ChallengePending, MSGBOX_DEFAULT
	releaseall
	end

BriskKantoAccessOnly_Text_NotReady:
	.string "KANTO is a region for TRAINERS.\\p"
	.string "The old story has ended here.$"

BriskKantoAccessOnly_Text_ChallengePending:
	.string "This KANTO battle challenge is\\n"
	.string "still being prepared.$"
