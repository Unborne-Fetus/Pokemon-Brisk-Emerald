@ Kanto's story is omitted in Brisk Emerald. Do not re-enable original FRLG
@ rivals, Rocket scenes or badges: their flags and trainer IDs overlap Hoenn.
@ Authored postgame challenges use separate rosters and saved defeat records.
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
	.string "Welcome to KANTO, HOENN CHAMPION!\p"
	.string "Our Trainers await your challenge.$"

BriskKantoAccessOnly_Text_ChallengePending:
	.string "KANTO welcomes HOENN's savior.\p"
	.string "Enjoy your next challenge!$"
