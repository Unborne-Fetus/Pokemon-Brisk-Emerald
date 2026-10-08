Route135_Archives_MapScripts::
	.byte 0

Route135_Archives_EventScript_Blue::
	lock
	faceplayer
	msgbox Route135_Archives_Text_BlueIntroSpeech, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_BLUE, Route135_Archives_Text_BlueDefeat
	goto Route135_Archives_EventScript_BlueDefeated
	end

Route135_Archives_EventScript_BlueDefeated::
	msgbox Route135_Archives_Text_BluePostBattleSpeech, MSGBOX_DEFAULT
	bufferspeciesname STR_VAR_1, SPECIES_JIRACHI
	givemon SPECIES_JIRACHI, 50
	call_if_ne VAR_RESULT, MON_CANT_GIVE, Common_EventScript_ReceivedGiftPokemon
	release
	end

Route135_Archives_EventScript_Red::
	lock
	faceplayer
	msgbox Route135_Archives_Text_RedIntroSpeech, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_RED2, Route135_Archives_Text_RedDefeat
	goto Route135_Archives_EventScript_RedDefeated
	end


Route135_Archives_EventScript_RedDefeated::
	msgbox Route135_Archives_Text_RedPostBattleSpeech, MSGBOX_DEFAULT
	bufferspeciesname STR_VAR_1, SPECIES_MEWTWO
	givemon SPECIES_MEWTWO, 50
	call_if_ne VAR_RESULT, MON_CANT_GIVE, Common_EventScript_ReceivedGiftPokemon
	release
	end


Route135_Archives_EventScript_Wallace::
	lock
	faceplayer
	msgbox Route135_Archives_Text_WallaceIntroSpeech, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_WALLACE, Route135_Archives_Text_WallaceDefeat
	goto Route135_Archives_EventScript_WallaceDefeated
	end

Route135_Archives_EventScript_WallaceDefeated::
	msgbox Route135_Archives_Text_WallacePostBattleSpeech, MSGBOX_DEFAULT
	bufferspeciesname STR_VAR_1, SPECIES_TYPE_NULL
	givemon SPECIES_TYPE_NULL, 50
	call_if_ne VAR_RESULT, MON_CANT_GIVE, Common_EventScript_ReceivedGiftPokemon
	release
	end


Route135_Archives_EventScript_Cynthia::
	lock
	faceplayer
	msgbox Route135_Archives_Text_CynthiaIntroSpeech, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_CYNTHIA, Route135_Archives_Text_CynthiaDefeat
	goto Route135_Archives_EventScript_CynthiaDefeated
	end


Route135_Archives_EventScript_CynthiaDefeated::
	msgbox Route135_Archives_Text_CynthiaPostBattleSpeech, MSGBOX_DEFAULT
	bufferspeciesname STR_VAR_1, SPECIES_GENESECT
	givemon SPECIES_GENESECT, 50
	call_if_ne VAR_RESULT, MON_CANT_GIVE, Common_EventScript_ReceivedGiftPokemon
	release
	end


Route135_Archives_EventScript_Iris::
	lock
	faceplayer
	msgbox Route135_Archives_Text_IrisIntroSpeech, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_IRIS, Route135_Archives_Text_IrisDefeat
	goto Route135_Archives_EventScript_IrisDefeated
	end



Route135_Archives_EventScript_IrisDefeated::
	msgbox Route135_Archives_Text_IrisPostBattleSpeech, MSGBOX_DEFAULT
	bufferspeciesname STR_VAR_1, SPECIES_KORAIDON
	givemon SPECIES_KORAIDON, 50
	call_if_ne VAR_RESULT, MON_CANT_GIVE, Common_EventScript_ReceivedGiftPokemon
	release
	end


Route135_Archives_EventScript_Diantha::
	lock
	faceplayer
	msgbox Route135_Archives_Text_DianthaIntroSpeech, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_DIANTHA, Route135_Archives_Text_DianthaDefeat
	goto Route135_Archives_EventScript_DianthaDefeated
	end



Route135_Archives_EventScript_DianthaDefeated::
	msgbox Route135_Archives_Text_DianthaPostBattleSpeech, MSGBOX_DEFAULT
	bufferspeciesname STR_VAR_1, SPECIES_DIANCIE
	givemon SPECIES_DIANCIE, 50
	call_if_ne VAR_RESULT, MON_CANT_GIVE, Common_EventScript_ReceivedGiftPokemon
	release
	end


Route135_Archives_EventScript_Leon::
	lock
	faceplayer
	msgbox Route135_Archives_Text_LeonIntroSpeech, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_LEON, Route135_Archives_Text_LeonDefeat
	goto Route135_Archives_EventScript_LeonDefeated
	end



Route135_Archives_EventScript_LeonDefeated::
	msgbox Route135_Archives_Text_LeonPostBattleSpeech, MSGBOX_DEFAULT
	bufferspeciesname STR_VAR_1, SPECIES_ETERNATUS
	givemon SPECIES_ETERNATUS, 50
	call_if_ne VAR_RESULT, MON_CANT_GIVE, Common_EventScript_ReceivedGiftPokemon
	release
	end


Route135_Archives_EventScript_Nemona::
	lock
	faceplayer
	msgbox Route135_Archives_Text_NemonaIntroSpeech, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_NEMONA, Route135_Archives_Text_NemonaDefeat
	goto Route135_Archives_EventScript_NemonaDefeated
	end


Route135_Archives_EventScript_NemonaDefeated::
	msgbox Route135_Archives_Text_NemonaPostBattleSpeech, MSGBOX_DEFAULT
	bufferspeciesname STR_VAR_1, SPECIES_MIRAIDON
	givemon SPECIES_MIRAIDON, 50
	call_if_ne VAR_RESULT, MON_CANT_GIVE, Common_EventScript_ReceivedGiftPokemon
	release
	end


Route135_Archives_EventScript_Volo::
	lock
	faceplayer
	msgbox Route135_Archives_Text_VoloIntroSpeech, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_VOLO, Route135_Archives_Text_VoloDefeat
	goto Route135_Archives_EventScript_VoloDefeated
	end


Route135_Archives_EventScript_VoloDefeated::
	msgbox Route135_Archives_Text_VoloPostBattleSpeech, MSGBOX_DEFAULT
	bufferspeciesname STR_VAR_1, SPECIES_GIRATINA
	givemon SPECIES_GIRATINA, 50
	call_if_ne VAR_RESULT, MON_CANT_GIVE, Common_EventScript_ReceivedGiftPokemon
	release
	end






Route135_Archives_Text_BlueIntroSpeech:
	.string "I am the most powerful\n"
	.string "trainer in the world!$"


Route135_Archives_Text_BlueDefeat:
	.string "NO! That can't be! You beat\n"
	.string "my best! After all that work,\p"
	.string "to become League champ? My\n"
	.string "reign is over already? It's\p"
	.string "not fair!$"


Route135_Archives_Text_BluePostBattleSpeech:
	.string "Why? Why did I lose? I never\n"
	.string "made any mistakes raising my\p"
	.string "Pokemon Darn it! You're the\n"
	.string "new Pokemon League Champ!\p"
	.string "Although I don't like to\n"
	.string "admit it.$"


Route135_Archives_Text_RedIntroSpeech:
	.string "... ... ... .................$"


Route135_Archives_Text_RedDefeat:
	.string "...$"


Route135_Archives_Text_RedPostBattleSpeech:
	.string "...........$"


Route135_Archives_Text_WallaceIntroSpeech:
	.string "I cannot allow a child to\n"
	.string "beat me!$"


Route135_Archives_Text_WallaceDefeat:
	.string "... ... ... ... ... ... ...\n"
	.string "... You were brilliant,\p"
	.string "reminiscent of my younger\n"
	.string "self Congratulations\p"
	.string "{PLAYER}{KUN}! Your strength\n"
	.string "is brilliant Like a Brisk\p"
	.string "Emerald!$"


Route135_Archives_Text_WallacePostBattleSpeech:
	.string "WALLACE: The POKéMON you sent\n"
	.string "into battle… At times they\p"
	.string "danced like a spring breeze,\n"
	.string "and at times they struck like\p"
	.string "lightning. It was with light,\n"
	.string "yet surefooted, elegance that\p"
	.string "you led your POKéMON. Even I\n"
	.string "felt bedazzled by your\p"
	.string "masterful performance! You\n"
	.string "now stand at the glorious\p"
	.string "peak of the POKéMON LEAGUE. I\n"
	.string "now proclaim you to be the\p"
	.string "new HOENN region…$"


Route135_Archives_Text_CynthiaIntroSpeech:
	.string "You've done well to make it\n"
	.string "this far. I'm Cynthia,\p"
	.string "Champion of Sinnoh. Show me\n"
	.string "the strength that brought you\p"
	.string "here!$"


Route135_Archives_Text_CynthiaDefeat:
	.string "What an incredible battle...\n"
	.string "Your bond with your Pokémon\p"
	.string "carried you all the way\n"
	.string "through.$"


Route135_Archives_Text_CynthiaPostBattleSpeech:
	.string "That was an excellent battle.\n"
	.string "You trusted your Pokémon, and\p"
	.string "they answered that trust.\n"
	.string "Keep moving forward together.\p"
	.string "That's the mark of a true\n"
	.string "Champion.$"


Route135_Archives_Text_IrisIntroSpeech:
	.string "OK! Brace yourself! I'm Iris,\n"
	.string "the Pokémon League Champion,\p"
	.string "and I'm going to defeat you!$"


Route135_Archives_Text_IrisDefeat:
	.string "Aghhhh... I did my best, but\n"
	.string "we lost...$"


Route135_Archives_Text_IrisPostBattleSpeech:
	.string "I'm upset I couldn't win! But\n"
	.string "you know what? More than\p"
	.string "that, I'm happy! I mean, come\n"
	.string "on. By having a serious\p"
	.string "battle, you and your Pokémon,\n"
	.string "and me and my Pokémon, we all\p"
	.string "got to know one another\n"
	.string "better than before! Yep, we\p"
	.string "sure did! OK, let's go! OK!\n"
	.string "Go on ahead! Hurry!$"


Route135_Archives_Text_DianthaIntroSpeech:
	.string "Welcome at last, dear\n"
	.string "Trainer. As you must know, I\p"
	.string "am the Champion, Diantha.$"


Route135_Archives_Text_DianthaDefeat:
	.string "Witnessing the noble spirits\n"
	.string "of you and your Pokémon in\p"
	.string "battle has really touched my\n"
	.string "heart...$"


Route135_Archives_Text_DianthaPostBattleSpeech:
	.string "I-I'm so choked up that it's\n"
	.string "hard to know what I should\p"
	.string "say, but I...I... I'm just so\n"
	.string "glad at this moment that I\p"
	.string "was good enough to be the\n"
	.string "Champion... After all, it\p"
	.string "gave me the chance to meet\n"
	.string "and battle with you and your\p"
	.string "wonderful Pokémon! Now I\n"
	.string "understand just why dear\p"
	.string "Augustine--that's Professor\n"
	.string "Sycamore to you--said he had\p"
	.string "to get everything ready for\n"
	.string "the new Champion! He'll be\p"
	.string "waiting for us in Lumiose\n"
	.string "City, but before I take you\p"
	.string "there... My dear, would you\n"
	.string "mind coming this way with me?$"


Route135_Archives_Text_LeonIntroSpeech:
	.string "I couldn't have dreamed of a\n"
	.string "better challenger to help\p"
	.string "increase my winning streak!$"


Route135_Archives_Text_LeonDefeat:
	.string "Congratulations! You've beat\n"
	.string "even the unbeatable\p"
	.string "Champion-- making you the new\n"
	.string "Champion of the region!$"


Route135_Archives_Text_LeonPostBattleSpeech:
	.string "You've become a first-rate\n"
	.string "Trainer, all right. I offer\p"
	.string "you my heartfelt\n"
	.string "congratulations! And now that\p"
	.string "you've grown so strong,\n"
	.string "perhaps it's time I started\p"
	.string "looking ahead myself. It's\n"
	.string "time we adults started\p"
	.string "working on improving the here\n"
	.string "and now, for the sake of all\p"
	.string "that's to come! And you! What\n"
	.string "you ought to do now is...\p"
	.string "believe in yourself and your\n"
	.string "partner Pokémon, and keep on\p"
	.string "blasting ahead! In order to\n"
	.string "create that bright future we\p"
	.string "all hope to see! People of\n"
	.string "Galar! Here you have it! A\p"
	.string "new legend born right before\n"
	.string "your eyes! Your Champion! Bet\p"
	.string "you can't wait to see what\n"
	.string "kind of future a kid with\p"
	.string "this kind of strength might\n"
	.string "bring about! I sure can't!\p"
	.string "Let's watch and see what they\n"
	.string "can do!$"


Route135_Archives_Text_VoloIntroSpeech:
	.string "Even if you don't wish to\n"
	.string "battle me, I'm not above\p"
	.string "using force to take those\n"
	.string "plates from you.$"


Route135_Archives_Text_VoloDefeat:
	.string "Why? Why you?! Why do you\n"
	.string "have the blessing of Arceus?!$"


Route135_Archives_Text_VoloPostBattleSpeech:
	.string "How? How could this happen?!\n"
	.string "Almighty Arceus, if you have\p"
	.string "any heart within you, then\n"
	.string "tell me... The blood of the\p"
	.string "ancient Sinnoh people flows\n"
	.string "in my veins, does it not?!\p"
	.string "What is it, then, that you\n"
	.string "find so lacking in me?! Do\p"
	.string "you mean to tell me that this\n"
	.string "world doesn't need to be\p"
	.string "remade? I can't live with\n"
	.string "such questions... I can't\p"
	.string "bear not satisfying this ache\n"
	.string "to know! Answer me, then. Do\p"
	.string "you have some dream that\n"
	.string "propels you, as I've had? You\p"
	.string "don't even know... Then\n"
	.string "perhaps the day will come\p"
	.string "when you will suffer and\n"
	.string "agonize as I do now... I am\p"
	.string "the great wielder of Pokémon.\n"
	.string "And you... You battle\p"
	.string "alongside your Pokémon. In\n"
	.string "the end, I was alone... But\p"
	.string "not you. You will fight\n"
	.string "together with your Pokémon to\p"
	.string "seize whatever dreams you\n"
	.string "have. Here. Take it... Take\p"
	.string "the plate that started me on\n"
	.string "this path-- the plate I once\p"
	.string "received from Giratina. My\n"
	.string "journey is over. My story\p"
	.string "ended when I lost to you. I\n"
	.string "suppose now you've gathered\p"
	.string "all of the plates said to\n"
	.string "exist in this land of Hisui.\p"
	.string "So that's...that's it... The\n"
	.string "Azure Flute. Heh. It comes to\p"
	.string "you. So Arceus wishes to meet\n"
	.string "you... Of all people, you had\p"
	.string "to be the one... Is that why\n"
	.string "you were brought to this\p"
	.string "world? Gah! I've no desire to\n"
	.string "watch from the sidelines as\p"
	.string "Arceus comes to you! And I\n"
	.string "absolutely cannot accept a\p"
	.string "world in which you would ever\n"
	.string "manage to defeat Arceus...\p"
	.string "Someday, I'll solve every\n"
	.string "riddle in the legends of\p"
	.string "Hisui's Pokémon. And on that\n"
	.string "day, I'll stand before Arceus\p"
	.string "at last-- No, I will CONQUER\n"
	.string "it! No matter how many years,\p"
	.string "how many decades, how many\n"
	.string "centuries it takes me!$"


Route135_Archives_Text_NemonaIntroSpeech:
	.string "You ready for a battle\n"
	.string "between Champions? Lets see\p"
	.string "who comes out on top!$"


Route135_Archives_Text_NemonaDefeat:
	.string "You... YOU DID IT!$"


Route135_Archives_Text_NemonaPostBattleSpeech:
	.string "This is legit incredible!\n"
	.string "You're the strongest out\p"
	.string "there! I mean, you knocked me\n"
	.string "flat, and I was giving you\p"
	.string "absolutely everything I had!\n"
	.string "Everything I...I...Ha haaa!\p"
	.string "Hahaha! Oh man. Pokémon\n"
	.string "battling, am I right? It's so\p"
	.string "much better--so much more fun\n"
	.string "than I ever even knew! Thanks\p"
	.string "for that incredible match.\n"
	.string "You beat me for real this\p"
	.string "time! Now let's give\n"
	.string "ourselves a minute and have a\p"
	.string "quick break, ...before round\n"
	.string "two! Which Pokémon should I\p"
	.string "bring out this time?\n"
	.string "Decisions, decisions!$"


