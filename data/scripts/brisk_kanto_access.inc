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
	.string "Ten years since TEAM ROCKET\n"
	.string "dissolved, wild POKéMON and\p"
	.string "Trainers have grown stronger.$"

BriskKantoAccessOnly_Text_ChallengePending:
	.string "TEAM ROCKET fell ten years ago.\p"
	.string "KANTO's POKéMON grew stronger\n"
	.string "in peace. Enjoy the challenge!$"

@ Kanto's physical objects must not use the generic resident dialogue.
@ Every hidden item/reward gets an independent SaveBlock3 object flag.

BriskKanto_EventScript_Electrode::
    lock
    faceplayer
    msgbox BriskKanto_Text_ElectrodeSurprise, MSGBOX_DEFAULT
    setwildbattle SPECIES_ELECTRODE, 95
    setflag FLAG_SYS_CTRL_OBJ_DELETE
    dowildbattle
    clearflag FLAG_SYS_CTRL_OBJ_DELETE
    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskKanto_EventScript_ElectrodeFinished
    goto_if_eq VAR_RESULT, B_OUTCOME_WON, BriskKanto_EventScript_ElectrodeFinished
    release
    end

BriskKanto_EventScript_ElectrodeFinished::
    removeobject VAR_LAST_TALKED
    release
    end

BriskKanto_Text_ElectrodeSurprise:
    .string "The POKé BALL was an ELECTRODE!$"

BriskKanto_EventScript_GiftHitmonlee::
    setvar VAR_TEMP_1, SPECIES_HITMONLEE
    goto BriskKanto_EventScript_GiveDojoHitmon
    end

BriskKanto_EventScript_GiftHitmonchan::
    setvar VAR_TEMP_1, SPECIES_HITMONCHAN
    goto BriskKanto_EventScript_GiveDojoHitmon
    end

BriskKanto_EventScript_GiveDojoHitmon::
    lock
    faceplayer
    goto_if_set FLAG_BRISK_KANTO_DOJO_REWARD, BriskKanto_EventScript_DojoAlreadyClaimed
    msgbox BriskKanto_Text_ChooseDojoGift, MSGBOX_YESNO
    goto_if_eq VAR_RESULT, NO, BriskKanto_EventScript_DojoCancel
    givemon VAR_TEMP_1, 90
    goto_if_eq VAR_RESULT, 2, BriskKanto_EventScript_DojoNoRoom
    setflag FLAG_BRISK_KANTO_DOJO_REWARD
    setflag FLAG_BRISK_KANTO_DOJO_HITMONLEE
    setflag FLAG_BRISK_KANTO_DOJO_HITMONCHAN
    removeobject 6
    removeobject 7
    msgbox BriskKanto_Text_DojoGiftReceived, MSGBOX_DEFAULT
    release
    end

BriskKanto_EventScript_DojoCancel::
    release
    end

BriskKanto_EventScript_DojoNoRoom::
    msgbox BriskKanto_Text_DojoNoRoom, MSGBOX_DEFAULT
    release
    end

BriskKanto_EventScript_DojoAlreadyClaimed::
    msgbox BriskKanto_Text_DojoAlreadyClaimed, MSGBOX_DEFAULT
    release
    end

BriskKanto_Text_ChooseDojoGift:
    .string "The DOJO offers one partner to a\n"
    .string "worthy CHAMPION. Take this one?$"

BriskKanto_Text_DojoGiftReceived:
    .string "You received your DOJO partner!$"

BriskKanto_Text_DojoNoRoom:
    .string "There's no room for this POKéMON.\n"
    .string "Make room and try again.$"

BriskKanto_Text_DojoAlreadyClaimed:
    .string "You've already claimed your\n"
    .string "FIGHTING DOJO reward.$"

BriskKanto_EventScript_OldAmberExhibit::
    msgbox BriskKanto_Text_OldAmberExhibit, MSGBOX_SIGN
    end

BriskKanto_Text_OldAmberExhibit:
    .string "An OLD AMBER specimen from\n"
    .string "KANTO's prehistoric past.$"

BriskKanto_EventScript_ZooSlowpoke::
    msgbox BriskKanto_Text_ZooSlowpoke, MSGBOX_NPC
    end

BriskKanto_Text_ZooSlowpoke:
    .string "SLOWPOKE is dozing peacefully.$"

BriskKanto_EventScript_ZooVoltorb::
    msgbox BriskKanto_Text_ZooVoltorb, MSGBOX_NPC
    end

BriskKanto_Text_ZooVoltorb:
    .string "VOLTORB rolls around its enclosure.$"

BriskKanto_EventScript_ZooChansey::
    msgbox BriskKanto_Text_ZooChansey, MSGBOX_NPC
    end

BriskKanto_Text_ZooChansey:
    .string "CHANSEY is carefully tending an EGG.$"

BriskKanto_EventScript_ZooLapras::
    msgbox BriskKanto_Text_ZooLapras, MSGBOX_NPC
    end

BriskKanto_Text_ZooLapras:
    .string "LAPRAS watches the water quietly.$"

BriskKanto_EventScript_ZooOmanyte::
    msgbox BriskKanto_Text_ZooOmanyte, MSGBOX_NPC
    end

BriskKanto_Text_ZooOmanyte:
    .string "OMANYTE examines the aquarium.$"

BriskKanto_EventScript_ZooKangaskhan::
    msgbox BriskKanto_Text_ZooKangaskhan, MSGBOX_NPC
    end

BriskKanto_Text_ZooKangaskhan:
    .string "KANGASKHAN protects its youngster.$"

@ Original static Kanto encounters remain wild battles rather than talkative NPCs.
BriskKanto_EventScript_Zapdos::
    lock
    faceplayer
    setwildbattle SPECIES_ZAPDOS, 95
    goto BriskKanto_EventScript_StartWildLegendary
    end

BriskKanto_EventScript_Moltres::
    lock
    faceplayer
    setwildbattle SPECIES_MOLTRES, 95
    goto BriskKanto_EventScript_StartWildLegendary
    end

BriskKanto_EventScript_Mewtwo::
    lock
    faceplayer
    setwildbattle SPECIES_MEWTWO, 100
    goto BriskKanto_EventScript_StartWildLegendary
    end

BriskKanto_EventScript_StartWildLegendary::
    setflag FLAG_SYS_CTRL_OBJ_DELETE
    dowildbattle
    clearflag FLAG_SYS_CTRL_OBJ_DELETE
    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, BriskKanto_EventScript_StaticBattleFinished
    goto_if_eq VAR_RESULT, B_OUTCOME_WON, BriskKanto_EventScript_StaticBattleFinished
    release
    end

BriskKanto_EventScript_StaticBattleFinished::
    removeobject VAR_LAST_TALKED
    release
    end

BriskKanto_EventScript_PokemonDoll::
    msgbox BriskKanto_Text_PokemonDoll, MSGBOX_SIGN
    end

BriskKanto_Text_PokemonDoll:
    .string "A carefully arranged POKéMON doll.$"

BriskKanto_EventScript_GemDisplay::
    msgbox BriskKanto_Text_GemDisplay, MSGBOX_SIGN
    end

BriskKanto_Text_GemDisplay:
    .string "A rare gemstone rests deep in\n"
    .string "the stone, firmly embedded.$"

@ Celadon City's rooftop ball is a one-time Eevee gift.
BriskKanto_EventScript_GiftEevee::
    lock
    faceplayer
    msgbox BriskKanto_Text_ChooseEeveeGift, MSGBOX_YESNO
    goto_if_eq VAR_RESULT, NO, BriskKanto_EventScript_EeveeCancel
    givemon SPECIES_EEVEE, 90
    goto_if_eq VAR_RESULT, 2, BriskKanto_EventScript_DojoNoRoom
    removeobject VAR_LAST_TALKED
    msgbox BriskKanto_Text_EeveeGiftReceived, MSGBOX_DEFAULT
    release
    end

BriskKanto_EventScript_EeveeCancel::
    release
    end

BriskKanto_Text_ChooseEeveeGift:
    .string "An EEVEE is resting in this ball.\n"
    .string "Would you like to take it?$"

BriskKanto_Text_EeveeGiftReceived:
    .string "EEVEE joined your collection!$"
