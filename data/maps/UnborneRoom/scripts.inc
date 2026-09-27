UnborneRoom_MapScripts::
	.byte 0


UnborneRoom_EventScript_Unborne::
	trainerbattle_single TRAINER_UNBORNE, UnborneRoom_Text_UnborneIntro, UnborneRoom_Text_UnborneDefeat
	msgbox UnborneRoom_Text_UnbornePostBattle, MSGBOX_AUTOCLOSE
	givemon SPECIES_POIPOLE, 10
	end



UnborneRoom_Text_UnborneIntro:
	.string "Yo, I'm the dev of this romhack\p"
	.string "Let's see that team of yours,\n"
	.string "Then we can talk.$"

UnborneRoom_Text_UnborneDefeat:
	.string "Sweet, let's talk.$"

UnborneRoom_Text_UnbornePostBattle:
	.string "Yo, I'm the dev of this romhack\p"
	.string "Let's see that team of yours,\n"
	.string "Then we can talk.$"
