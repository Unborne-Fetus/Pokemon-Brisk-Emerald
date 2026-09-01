Route135_Archives_MapScripts::
	.byte 0

Route135_Archives_EventScript_Blue::
	lock
	faceplayer
	msgbox Route135_Archives_Text_BlueIntroSpeech, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_BLUE, Route135_Archives_Text_BlueDefeat
	goto Route135_Archives_EventScript_BlueDefeated
	end

Route135_Archives_EventScript_BluePostBattleSpeech::
	msgbox Route135_Archives_Text_BluePostBattleSpeech, MSGBOX_DEFAULT
	release
	end

Route135_Archives_EventScript_BlueDefeated::
	msgbox Route135_Archives_Text_BluePostBattleSpeech, MSGBOX_DEFAULT
	release
	end

Route135_Archives_EventScript_Red::
	lock
	faceplayer
	msgbox Route135_Archives_Text_RedIntroSpeech, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_RED2, Route135_Archives_Text_RedDefeat
	goto Route135_Archives_EventScript_RedDefeated
	end

Route135_Archives_EventScript_RedPostBattleSpeech::
	msgbox Route135_Archives_Text_RedPostBattleSpeech, MSGBOX_DEFAULT
	release
	end

Route135_Archives_EventScript_RedDefeated::
	msgbox Route135_Archives_Text_RedPostBattleSpeech, MSGBOX_DEFAULT
	release
	end


Route135_Archives_EventScript_Wallace::
	lock
	faceplayer
	msgbox Route135_Archives_Text_WallaceIntroSpeech, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_WALLACE, Route135_Archives_Text_WallaceDefeat
	goto Route135_Archives_EventScript_WallaceDefeated
	end

Route135_Archives_EventScript_WallacePostBattleSpeech::
	msgbox Route135_Archives_Text_WallacePostBattleSpeech, MSGBOX_DEFAULT
	release
	end

Route135_Archives_EventScript_WallaceDefeated::
	msgbox Route135_Archives_Text_WallacePostBattleSpeech, MSGBOX_DEFAULT
	release
	end


Route135_Archives_EventScript_Cynthia::
	lock
	faceplayer
	msgbox Route135_Archives_Text_CynthiaIntroSpeech, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_CYNTHIA, Route135_Archives_Text_CynthiaDefeat
	goto Route135_Archives_EventScript_CynthiaDefeated
	end

Route135_Archives_EventScript_CynthiaPostBattleSpeech::
	msgbox Route135_Archives_Text_CynthiaPostBattleSpeech, MSGBOX_DEFAULT
	release
	end

Route135_Archives_EventScript_CynthiaDefeated::
	msgbox Route135_Archives_Text_CynthiaPostBattleSpeech, MSGBOX_DEFAULT
	release
	end


Route135_Archives_EventScript_Iris::
	lock
	faceplayer
	msgbox Route135_Archives_Text_IrisIntroSpeech, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_IRIS, Route135_Archives_Text_IrisDefeat
	goto Route135_Archives_EventScript_IrisDefeated
	end

Route135_Archives_EventScript_IrisPostBattleSpeech::
	msgbox Route135_Archives_Text_IrisPostBattleSpeech, MSGBOX_DEFAULT
	release
	end

Route135_Archives_EventScript_IrisDefeated::
	msgbox Route135_Archives_Text_IrisPostBattleSpeech, MSGBOX_DEFAULT
	release
	end


Route135_Archives_EventScript_Diantha::
	lock
	faceplayer
	msgbox Route135_Archives_Text_DianthaIntroSpeech, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_DIANTHA, Route135_Archives_Text_DianthaDefeat
	goto Route135_Archives_EventScript_DianthaDefeated
	end

Route135_Archives_EventScript_DianthaPostBattleSpeech::
	msgbox Route135_Archives_Text_DianthaPostBattleSpeech, MSGBOX_DEFAULT
	release
	end

Route135_Archives_EventScript_DianthaDefeated::
	msgbox Route135_Archives_Text_DianthaPostBattleSpeech, MSGBOX_DEFAULT
	release
	end


Route135_Archives_EventScript_Leon::
	lock
	faceplayer
	msgbox Route135_Archives_Text_LeonIntroSpeech, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_LEON, Route135_Archives_Text_LeonDefeat
	goto Route135_Archives_EventScript_LeonDefeated
	end

Route135_Archives_EventScript_LeonPostBattleSpeech::
	msgbox Route135_Archives_Text_LeonPostBattleSpeech, MSGBOX_DEFAULT
	release
	end

Route135_Archives_EventScript_LeonDefeated::
	msgbox Route135_Archives_Text_LeonPostBattleSpeech, MSGBOX_DEFAULT
	release
	end


Route135_Archives_EventScript_Nemona::
	lock
	faceplayer
	msgbox Route135_Archives_Text_NemonaIntroSpeech, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_NEMONA, Route135_Archives_Text_NemonaDefeat
	goto Route135_Archives_EventScript_NemonaDefeated
	end

Route135_Archives_EventScript_NemonaPostBattleSpeech::
	msgbox Route135_Archives_Text_NemonaPostBattleSpeech, MSGBOX_DEFAULT
	release
	end

Route135_Archives_EventScript_NemonaDefeated::
	msgbox Route135_Archives_Text_NemonaPostBattleSpeech, MSGBOX_DEFAULT
	release
	end


Route135_Archives_EventScript_Volo::
	lock
	faceplayer
	msgbox Route135_Archives_Text_VoloIntroSpeech, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_VOLO, Route135_Archives_Text_VoloDefeat
	goto Route135_Archives_EventScript_VoloDefeated
	end

Route135_Archives_EventScript_VoloPostBattleSpeech::
	msgbox Route135_Archives_Text_VoloPostBattleSpeech, MSGBOX_DEFAULT
	release
	end

Route135_Archives_EventScript_VoloDefeated::
	msgbox Route135_Archives_Text_VoloPostBattleSpeech, MSGBOX_DEFAULT
	release
	end






Route135_Archives_Text_BlueIntroSpeech:
	.string "I am the most powerful trainer in the world!$"


Route135_Archives_Text_BlueDefeat:
	.string "NO! That can't be! You beat\n"
	.string "my best! After all that work,\n"
	.string "to become League champ?\n"
	.string "My reign is over already? It's not fair!$"

Route135_Archives_Text_BluePostBattleSpeech:
	.string "Why? Why did I lose?\n"
	.string "I never made any mistakes raising my Pokemon\n"
	.string "Darn it!\n"
	.string "You're the new Pokemon League Champ!\n"
	.string "Although I don't like to admit it.$"



Route135_Archives_Text_RedIntroSpeech:
	.string "...\n"
	.string "...\n"
	.string "... \n"
	.string ".................$"


Route135_Archives_Text_RedDefeat:
	.string "...$"

Route135_Archives_Text_RedPostBattleSpeech:
	.string "...........$"


Route135_Archives_Text_WallaceIntroSpeech:
	.string "I cannot allow\p"
	.string "a child to beat me!$"

Route135_Archives_Text_WallaceDefeat:
	.string "...\p"
	.string "...\n"
	.string "...\p"
	.string "...\n"
	.string "...\p"
	.string "...\n"
	.string "...\p"
	.string "...\n"
	.string "You were brilliant, reminiscent\l"
	.string "of my younger self\p"
	.string "Congratulations {PLAYER}{KUN}!\p"
	.string "Your strength is brilliant\n"
	.string "Like a Brisk Emerald!$"


Route135_Archives_Text_WallacePostBattleSpeech:
	.string "WALLACE: The POKéMON you sent into\n"
	.string "battle…\p"
	.string "At times they danced like a spring\n"
	.string "breeze, and at times they struck\l"
	.string "like lightning.\p"
	.string "It was with light, yet surefooted,\n"
	.string "elegance that you led your POKéMON.\p"
	.string "Even I felt bedazzled by your\n"
	.string "masterful performance!\p"
	.string "You now stand at the glorious peak\n"
	.string "of the POKéMON LEAGUE.\p"
	.string "I now proclaim you to be the new\n"
	.string "HOENN region…$"



Route135_Archives_Text_CynthiaIntroSpeech:
	.string "But that's enough talk. Let's get on with why you're here.\n"
	.string "As the Pokémon League Champion, I accept your challenge!$"


Route135_Archives_Text_CynthiaDefeat:
	.string "...Just a few moments ago, you were the most\n"
	.string "powerful challenger. And just now, you became\n"
	.string "the most powerful of all the Trainers.\n"
	.string "You are now our newest Champion!$"


Route135_Archives_Text_CynthiaPostBattleSpeech:
	.string "That was excellent. Truly, an outstanding battle.\n"
	.string "You gave the support your Pokémon needed to\n"
	.string "maximize their power. And you guided them with\n"
	.string "certainty to secure victory.\n"
	.string "You have both passion and calculating coolness.\n"
	.string "Together, you and your Pokémon can overcome\n"
	.string "any challenge that may come your way.\n"
	.string "Those are the impressions I got from our battle.\n"
	.string "I'm glad I got to take part in the crowning of\n"
	.string "Sinnoh's new Champion! Come with me.\n"
	.string "We'll take the lift.$"



Route135_Archives_Text_IrisIntroSpeech:
	.string "OK! Brace yourself! I'm Iris, the Pokémon League\n"
	.string "Champion, and I'm going to defeat you!$"


Route135_Archives_Text_IrisDefeat:
	.string "Aghhhh... I did my best, but we lost...$"


Route135_Archives_Text_IrisPostBattleSpeech:
	.string "I'm upset I couldn't win! But you know what?\n"
	.string "More than that, I'm happy!\n"
	.string "I mean, come on. By having a serious battle,\n"
	.string "you and your Pokémon, and me and my Pokémon,\n"
	.string "we all got to know one another better than before!\n"
	.string "Yep, we sure did! OK, let's go!\n"
	.string "OK! Go on ahead! Hurry!$"


Route135_Archives_Text_DianthaIntroSpeech:
	.string "Welcome at last, dear Trainer. As you must know,\n"
	.string "I am the Champion, Diantha.\n"


Route135_Archives_Text_DianthaDefeat:
	.string "Witnessing the noble spirits of you and your\n"
	.string "Pokémon in battle has really touched my heart...$"


Route135_Archives_Text_DianthaPostBattleSpeech:
	.string "I-I'm so choked up that it's hard to know what\n"
	.string "I should say, but I...I... I'm just so glad at this\n"
	.string "moment that I was good enough to be the Champion...\n"
	.string "After all, it gave me the chance to meet and battle\n"
	.string "with you and your wonderful Pokémon!\n"
	.string "Now I understand just why dear Augustine--that's\n"
	.string "Professor Sycamore to you--said he had to get\n"
	.string "everything ready for the new Champion!\n"
	.string "He'll be waiting for us in Lumiose City, but before\n"
	.string "I take you there...\n"
	.string "My dear, would you mind coming this way with me?$"



Route135_Archives_Text_LeonIntroSpeech:
	.string "I couldn't have dreamed of a better challenger\n"
	.string "to help increase my winning streak!$"


Route135_Archives_Text_LeonDefeat:
	.string "Congratulations!\n"
	.string "You've beat even the unbeatable Champion--\n"
	.string "making you the new Champion of the region!$"


Route135_Archives_Text_LeonPostBattleSpeech:
	.string "You've become a first-rate Trainer, all right.\n"
	.string "I offer you my heartfelt congratulations!\n"
	.string "And now that you've grown so strong, perhaps it's\n"
	.string "time I started looking ahead myself.\n"
	.string "It's time we adults started working on improving\n"
	.string "the here and now, for the sake of all that's to come!\n"
	.string "And you! What you ought to do now is...\n"
	.string "believe in yourself and your partner Pokémon,\n"
	.string "and keep on blasting ahead!\n"
	.string "In order to create that bright future we all hope to see!\n"
	.string "People of Galar!\n"
	.string "Here you have it! A new legend born right before your eyes!\n"
	.string "Your Champion!\n"
	.string "Bet you can't wait to see what kind of future a kid\n"
	.string "with this kind of strength might bring about!\n"
	.string "I sure can't! Let's watch and see what they can do!$"



Route135_Archives_Text_VoloIntroSpeech:
	.string "Even if you don't wish to battle me, I'm not above\n"
	.string "using force to take those plates from you.$"


Route135_Archives_Text_VoloDefeat:
	.string "Why? Why you?! Why do you have the blessing\n"
	.string "of Arceus?!$"


Route135_Archives_Text_VoloPostBattleSpeech:
	.string "How? How could this happen?! Almighty Arceus,\n"
	.string "if you have any heart within you, then tell me...\n"
	.string "The blood of the ancient Sinnoh people flows in\n"
	.string "my veins, does it not?! What is it, then, that you\n"
	.string "find so lacking in me?!\n"
	.string "Do you mean to tell me that this world doesn't\n"
	.string "need to be remade?\n"
	.string "I can't live with such questions...\n"
	.string "I can't bear not satisfying this ache to know!\n"
	.string "Answer me, then. Do you have some dream that\n"
	.string "propels you, as I've had?\n"
	.string "You don't even know...\n"
	.string "Then perhaps the day will come when you will suffer\n"
	.string "and agonize as I do now...\n"
	.string "I am the great wielder of Pokémon.\n"
	.string "And you... You battle alongside your Pokémon.\n"
	.string "In the end, I was alone... But not you.\n"
	.string "You will fight together with your Pokémon to seize\n"
	.string "whatever dreams you have.\n"
	.string "Here. Take it...\n"
	.string "Take the plate that started me on this path--\n"
	.string "the plate I once received from Giratina.\n"
	.string "My journey is over.\n"
	.string "My story ended when I lost to you.\n"
	.string "I suppose now you've gathered all of the plates\n"
	.string "said to exist in this land of Hisui.\n"
	.string "So that's...that's it... The Azure Flute.\n"
	.string "Heh. It comes to you.\n"
	.string "So Arceus wishes to meet you...\n"
	.string "Of all people, you had to be the one...\n"
	.string "Is that why you were brought to this world?\n"
	.string "Gah! I've no desire to watch from the sidelines\n"
	.string "as Arceus comes to you!\n"
	.string "And I absolutely cannot accept a world in which\n"
	.string "you would ever manage to defeat Arceus...\n"
	.string "Someday, I'll solve every riddle in the legends\n"
	.string "of Hisui's Pokémon.\n"
	.string "And on that day, I'll stand before Arceus at last--\n"
	.string "No, I will CONQUER it!\n"
	.string "No matter how many years, how many decades,\n"
	.string "how many centuries it takes me!$"




Route135_Archives_Text_NemonaIntroSpeech:
	.string "You ready for a battle between Champions?\n"
	.string "Lets see who comes out on top!$"





Route135_Archives_Text_NemonaDefeat:
	.string "You...\n"
	.string "YOU DID IT!$"



Route135_Archives_Text_NemonaPostBattleSpeech: 
	.string "This is legit incredible! You're the strongest\n" 
	.string "out there!\n" 
	.string "I mean, you knocked me flat, and I was giving you\n" 
	.string "absolutely everything I had!\n" 
	.string "Everything I...I...Ha haaa! Hahaha!\n"
	.string "Oh man. Pokémon battling, am I right?\n"
	.string "It's so much better--so much more fun than I ever\n"
	.string "even knew!\n"
	.string "Thanks for that incredible match.\n"
	.string "You beat me for real this time!\n"
	.string "Now let's give ourselves a minute and have a quick\n"
	.string "break, ...before round two!\n"
	.string "Which Pokémon should I bring out this time?\n"
	.string "Decisions, decisions!$"



