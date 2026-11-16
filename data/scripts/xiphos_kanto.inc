@ Team Xiphos Aegis postgame. Rocket graphics are provisional art only.
@ Each defeat is saved through the Kanto trainer flag extension.
XiphosKanto_EventScript_AGENT_A::
	trainerbattle_double TRAINER_XIPHOS_AGENT_A, XiphosKanto_Text_AGENT_A_Intro, XiphosKanto_Text_AGENT_A_Defeat, BriskKanto_Text_TwoPokemon
	msgbox XiphosKanto_Text_AGENT_A_After, MSGBOX_AUTOCLOSE
	end

XiphosKanto_Text_AGENT_A_Intro:
	.string "Reports from Kanto are\n"
	.string "arriving. Xiphos will\p"
	.string "control the chaos.$"

XiphosKanto_Text_AGENT_A_Defeat:
	.string "Our defenses need more\n"
	.string "work.$"

XiphosKanto_Text_AGENT_A_After:
	.string "You are stronger than our\n"
	.string "models.$"

XiphosKanto_EventScript_AGENT_B::
	trainerbattle_double TRAINER_XIPHOS_AGENT_B, XiphosKanto_Text_AGENT_B_Intro, XiphosKanto_Text_AGENT_B_Defeat, BriskKanto_Text_TwoPokemon
	msgbox XiphosKanto_Text_AGENT_B_After, MSGBOX_AUTOCLOSE
	end

XiphosKanto_Text_AGENT_B_Intro:
	.string "The old Rocket roads are\n"
	.string "useful. We have new orders.$"

XiphosKanto_Text_AGENT_B_Defeat:
	.string "Those orders were not\n"
	.string "enough.$"

XiphosKanto_Text_AGENT_B_After:
	.string "We were told you would be\n"
	.string "tough.$"

XiphosKanto_EventScript_AGENT_C::
	trainerbattle_double TRAINER_XIPHOS_AGENT_C, XiphosKanto_Text_AGENT_C_Intro, XiphosKanto_Text_AGENT_C_Defeat, BriskKanto_Text_TwoPokemon
	msgbox XiphosKanto_Text_AGENT_C_After, MSGBOX_AUTOCLOSE
	end

XiphosKanto_Text_AGENT_C_Intro:
	.string "Our teams adapt to every\n"
	.string "threat. Test our research.$"

XiphosKanto_Text_AGENT_C_Defeat:
	.string "The test failed!$"

XiphosKanto_Text_AGENT_C_After:
	.string "I need to revise my report.$"

XiphosKanto_EventScript_AGENT_D::
	trainerbattle_double TRAINER_XIPHOS_AGENT_D, XiphosKanto_Text_AGENT_D_Intro, XiphosKanto_Text_AGENT_D_Defeat, BriskKanto_Text_TwoPokemon
	msgbox XiphosKanto_Text_AGENT_D_After, MSGBOX_AUTOCLOSE
	end

XiphosKanto_Text_AGENT_D_Intro:
	.string "The Director is watching\n"
	.string "our performance.$"

XiphosKanto_Text_AGENT_D_Defeat:
	.string "I let the Director down.$"

XiphosKanto_Text_AGENT_D_After:
	.string "Our organization is\n"
	.string "spreading.$"

XiphosKanto_EventScript_AGENT_E::
	trainerbattle_double TRAINER_XIPHOS_AGENT_E, XiphosKanto_Text_AGENT_E_Intro, XiphosKanto_Text_AGENT_E_Defeat, BriskKanto_Text_TwoPokemon
	msgbox XiphosKanto_Text_AGENT_E_After, MSGBOX_AUTOCLOSE
	end

XiphosKanto_Text_AGENT_E_Intro:
	.string "Every battle feeds the\n"
	.string "Aegis project.$"

XiphosKanto_Text_AGENT_E_Defeat:
	.string "Our readings were wrong.$"

XiphosKanto_Text_AGENT_E_After:
	.string "The results are going to\n"
	.string "Jessie.$"

XiphosKanto_EventScript_AGENT_F::
	trainerbattle_double TRAINER_XIPHOS_AGENT_F, XiphosKanto_Text_AGENT_F_Intro, XiphosKanto_Text_AGENT_F_Defeat, BriskKanto_Text_TwoPokemon
	msgbox XiphosKanto_Text_AGENT_F_After, MSGBOX_AUTOCLOSE
	end

XiphosKanto_Text_AGENT_F_Intro:
	.string "Team Xiphos is not Team\n"
	.string "Rocket. Do not confuse us.$"

XiphosKanto_Text_AGENT_F_Defeat:
	.string "I see the difference now.$"

XiphosKanto_Text_AGENT_F_After:
	.string "These old uniforms are\n"
	.string "temporary.$"

XiphosKanto_EventScript_AGENT_G::
	trainerbattle_double TRAINER_XIPHOS_AGENT_G, XiphosKanto_Text_AGENT_G_Intro, XiphosKanto_Text_AGENT_G_Defeat, BriskKanto_Text_TwoPokemon
	msgbox XiphosKanto_Text_AGENT_G_After, MSGBOX_AUTOCLOSE
	end

XiphosKanto_Text_AGENT_G_Intro:
	.string "Kanto will witness a new\n"
	.string "age of containment.$"

XiphosKanto_Text_AGENT_G_Defeat:
	.string "The plan has flaws.$"

XiphosKanto_Text_AGENT_G_After:
	.string "We still have work to do.$"

XiphosKanto_EventScript_SCIENTIST_A::
	trainerbattle_double TRAINER_XIPHOS_SCIENTIST_A, XiphosKanto_Text_SCIENTIST_A_Intro, XiphosKanto_Text_SCIENTIST_A_Defeat, BriskKanto_Text_TwoPokemon
	msgbox XiphosKanto_Text_SCIENTIST_A_After, MSGBOX_AUTOCLOSE
	end

XiphosKanto_Text_SCIENTIST_A_Intro:
	.string "We engineered the\n"
	.string "Containment Protocol.\p"
	.string "Observe.$"

XiphosKanto_Text_SCIENTIST_A_Defeat:
	.string "My calculations were wrong.$"

XiphosKanto_Text_SCIENTIST_A_After:
	.string "The first hit reveals\n"
	.string "everything.$"

XiphosKanto_EventScript_SCIENTIST_B::
	trainerbattle_double TRAINER_XIPHOS_SCIENTIST_B, XiphosKanto_Text_SCIENTIST_B_Intro, XiphosKanto_Text_SCIENTIST_B_Defeat, BriskKanto_Text_TwoPokemon
	msgbox XiphosKanto_Text_SCIENTIST_B_After, MSGBOX_AUTOCLOSE
	end

XiphosKanto_Text_SCIENTIST_B_Intro:
	.string "The shield activates only\n"
	.string "when our team is hit.$"

XiphosKanto_Text_SCIENTIST_B_Defeat:
	.string "You broke through our\n"
	.string "design.$"

XiphosKanto_Text_SCIENTIST_B_After:
	.string "The Director must see this.$"

XiphosKanto_EventScript_SENIOR_A::
	trainerbattle_double TRAINER_XIPHOS_SENIOR_A, XiphosKanto_Text_SENIOR_A_Intro, XiphosKanto_Text_SENIOR_A_Defeat, BriskKanto_Text_TwoPokemon
	msgbox XiphosKanto_Text_SENIOR_A_After, MSGBOX_AUTOCLOSE
	end

XiphosKanto_Text_SENIOR_A_Intro:
	.string "You have attracted\n"
	.string "attention across Kanto.$"

XiphosKanto_Text_SENIOR_A_Defeat:
	.string "This changes our mission.$"

XiphosKanto_Text_SENIOR_A_After:
	.string "The commanders will hear of\n"
	.string "you.$"

XiphosKanto_EventScript_SENIOR_B::
	trainerbattle_double TRAINER_XIPHOS_SENIOR_B, XiphosKanto_Text_SENIOR_B_Intro, XiphosKanto_Text_SENIOR_B_Defeat, BriskKanto_Text_TwoPokemon
	msgbox XiphosKanto_Text_SENIOR_B_After, MSGBOX_AUTOCLOSE
	end

XiphosKanto_Text_SENIOR_B_Intro:
	.string "Averra taught us to prepare\n"
	.string "for anything.$"

XiphosKanto_Text_SENIOR_B_Defeat:
	.string "We were not prepared for\n"
	.string "you.$"

XiphosKanto_Text_SENIOR_B_After:
	.string "The Director came from\n"
	.string "Averra.$"

XiphosKanto_EventScript_COMMANDER_FAIRY::
	trainerbattle_double TRAINER_XIPHOS_COMMANDER_FAIRY, XiphosKanto_Text_COMMANDER_FAIRY_Intro, XiphosKanto_Text_COMMANDER_FAIRY_Defeat, BriskKanto_Text_TwoPokemon
	msgbox XiphosKanto_Text_COMMANDER_FAIRY_After, MSGBOX_AUTOCLOSE
	end

XiphosKanto_Text_COMMANDER_FAIRY_Intro:
	.string "I am Astra. Aegis relies on\n"
	.string "control, not luck.$"

XiphosKanto_Text_COMMANDER_FAIRY_Defeat:
	.string "Our coordination failed.$"

XiphosKanto_Text_COMMANDER_FAIRY_After:
	.string "The Director awaits a true\n"
	.string "challenger.$"

XiphosKanto_EventScript_COMMANDER_FIGHTING::
	trainerbattle_double TRAINER_XIPHOS_COMMANDER_FIGHTING, XiphosKanto_Text_COMMANDER_FIGHTING_Intro, XiphosKanto_Text_COMMANDER_FIGHTING_Defeat, BriskKanto_Text_TwoPokemon
	msgbox XiphosKanto_Text_COMMANDER_FIGHTING_After, MSGBOX_AUTOCLOSE
	end

XiphosKanto_Text_COMMANDER_FIGHTING_Intro:
	.string "Valor here. Face the might\n"
	.string "of our division.$"

XiphosKanto_Text_COMMANDER_FIGHTING_Defeat:
	.string "You overcame our power.$"

XiphosKanto_Text_COMMANDER_FIGHTING_After:
	.string "Only the Director can\n"
	.string "answer that.$"

XiphosKanto_EventScript_COMMANDER_STEEL::
	trainerbattle_double TRAINER_XIPHOS_COMMANDER_STEEL, XiphosKanto_Text_COMMANDER_STEEL_Intro, XiphosKanto_Text_COMMANDER_STEEL_Defeat, BriskKanto_Text_TwoPokemon
	msgbox XiphosKanto_Text_COMMANDER_STEEL_After, MSGBOX_AUTOCLOSE
	end

XiphosKanto_Text_COMMANDER_STEEL_Intro:
	.string "Ferrum. Our defenses have\n"
	.string "never broken.$"

XiphosKanto_Text_COMMANDER_STEEL_Defeat:
	.string "Even steel can yield.$"

XiphosKanto_Text_COMMANDER_STEEL_After:
	.string "Jessie will not accept this\n"
	.string "result.$"

XiphosKanto_EventScript_COMMANDER_DRAGON::
	trainerbattle_double TRAINER_XIPHOS_COMMANDER_DRAGON, XiphosKanto_Text_COMMANDER_DRAGON_Intro, XiphosKanto_Text_COMMANDER_DRAGON_Defeat, BriskKanto_Text_TwoPokemon
	msgbox XiphosKanto_Text_COMMANDER_DRAGON_After, MSGBOX_AUTOCLOSE
	end

XiphosKanto_Text_COMMANDER_DRAGON_Intro:
	.string "I am Drakon. This is the\n"
	.string "final field trial.$"

XiphosKanto_Text_COMMANDER_DRAGON_Defeat:
	.string "The trial is over.$"

XiphosKanto_Text_COMMANDER_DRAGON_After:
	.string "Tell Jessie what you\n"
	.string "learned.$"

XiphosKanto_EventScript_JESSIE::
	goto_if_unset FLAG_XIPHOS_COMMANDER_FAIRY_DEFEATED, XiphosKanto_EventScript_JessieLocked
	goto_if_unset FLAG_XIPHOS_COMMANDER_FIGHTING_DEFEATED, XiphosKanto_EventScript_JessieLocked
	goto_if_unset FLAG_XIPHOS_COMMANDER_STEEL_DEFEATED, XiphosKanto_EventScript_JessieLocked
	goto_if_unset FLAG_XIPHOS_COMMANDER_DRAGON_DEFEATED, XiphosKanto_EventScript_JessieLocked
	trainerbattle_double TRAINER_XIPHOS_JESSIE, XiphosKanto_Text_JESSIE_Intro, XiphosKanto_Text_JESSIE_Defeat, BriskKanto_Text_TwoPokemon
	call Common_EventScript_JessieKantoClassicReward
	msgbox XiphosKanto_Text_JESSIE_After, MSGBOX_AUTOCLOSE
	end

XiphosKanto_Text_JESSIE_Intro:
	.string "I came from Averra with one\n"
	.string "purpose: protect through\p"
	.string "control.$"

XiphosKanto_Text_JESSIE_Defeat:
	.string "So Aegis cannot control\n"
	.string "everything.$"

XiphosKanto_Text_JESSIE_After:
	.string "You earned this victory.\n"
	.string "Kanto should see it.$"

XiphosKanto_EventScript_JessieLocked::
	msgbox XiphosKanto_Text_JessieLocked, MSGBOX_NPC
	end

XiphosKanto_Text_JessieLocked:
	.string "Defeat my four commanders\n"
	.string "first. Their field trials\p"
	.string "are not complete.$"

