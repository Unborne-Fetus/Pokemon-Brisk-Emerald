UnborneRoom_MapScripts::
	.byte 0



UnborneRoom_Text_UnborneIntro:
	.string "Yo, I'm the dev of this romhack\p"
	.string "Let's see that team of yours,\n"
	.string "Then we can talk.$"

UnborneRoom_Text_UnborneDefeat:
	.string "Sweet, let's talk.$"

UnborneRoom_Text_UnbornePostBattle:
	.string "I'm Unborne Fetus, the dev.\p"
	.string "If you've made it this far,\n"
	.string "then you probably like this hack\p"
	.string "Allow me to just say, thank you!\n"
	.string "This romhack is my largest\p"
	.string "coding project, to date.\n"
	.string "it's taking up a large portion\p"
	.string "of my life. So I'm going to take\n"
	.string "a break. But when I'm done.\p"
	.string "I'm going to work on:\n"
	.string "POKéMON SEARING SUN\p"
	.string "which is an USUM hack.\n"
	.string "But enough about the future.\p"
	.string "These past 3 routes have been\n"
	.string "Full of my favorite Pokemon.\p"
	.string "This is my absolute favorite.\n"
	.string "YOU RECEIVED MEGA SCIZOR\p"
	.string "After leaving this room, walk\n"
	.string "straight to the east.\p"
	.string "That's where you can continue your\n"
	.string "adventure.\p"
	.string "See you around!$"


UnborneRoom_EventScript_Unborne::
	lock
	faceplayer
	msgbox UnborneRoom_Text_UnborneIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_UNBORNE, UnborneRoom_Text_UnborneDefeat
	goto UnborneRoom_EventScript_UnborneDefeated
	end



UnborneRoom_EventScript_UnborneDefeated::
	msgbox UnborneRoom_Text_UnbornePostBattle, MSGBOX_DEFAULT
	givemon SPECIES_SCIZOR, 100
		item=ITEM_SCIZORITE
		ball=BALL_MASTER
		nature=NATURE_ADAMANT
		abilityNum=2
		gender=MALE
		hpEv=248
		atkEv=252
		defEv=0
		speedEv=8
		spAtkEv=0
		spDefEv=0
		hpIv=31
		atkIv=31
		defIv=31
		speedIv=31
		spAtkIv=31
		spDefIv=31
		move1=MOVE_AERIAL_ACE
		move2=MOVE_BULLET_PUNCH
		move3=MOVE_FURY_CUTTER
		move4=MOVE_SWORDS_DANCE
	release
	end

