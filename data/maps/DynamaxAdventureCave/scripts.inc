DynamaxAdventureCave_MapScripts::
    map_script MAP_SCRIPT_ON_TRANSITION, DynamaxAdventureRestCave_SetRival
	.byte 0

DynamaxAdventureCave_EventScript_Scientist::
	lock
	faceplayer
	setvar VAR_0x8004, DA_OP_STATUS
	special RunDynamaxAdventure
	goto_if_eq VAR_0x8000, DA_PHASE_INACTIVE, DynamaxAdventureCave_Welcome
	msgbox DynamaxAdventureCave_Text_Resume, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, DynamaxAdventureCave_Leave
	goto DynamaxAdventureCave_Dispatch
DynamaxAdventureCave_Welcome::
    msgbox DynamaxAdventureCave_Text_ScientistResearch, MSGBOX_DEFAULT
	msgbox DynamaxAdventureCave_Text_Welcome, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, DynamaxAdventureCave_Leave
	setvar VAR_0x8004, DA_OP_BEGIN
	special RunDynamaxAdventure
	goto_if_eq VAR_RESULT, FALSE, DynamaxAdventureCave_Error
    setvar VAR_0x8004, DA_OP_BOSS_HINT
    special RunDynamaxAdventure
    msgbox DynamaxAdventureCave_Text_BossHint, MSGBOX_DEFAULT
    goto DynamaxAdventureCave_Dispatch
DynamaxAdventureCave_Dispatch::
	setvar VAR_0x8004, DA_OP_STATUS
	special RunDynamaxAdventure
	goto_if_eq VAR_0x8000, DA_PHASE_RENTAL, DynamaxAdventureCave_Rental
	goto_if_eq VAR_0x8000, DA_PHASE_PATH, DynamaxAdventureCave_Path
	goto_if_eq VAR_0x8000, DA_PHASE_READY, DynamaxAdventureCave_Ready
	goto_if_eq VAR_0x8000, DA_PHASE_CATCH, DynamaxAdventureCave_Catch
	goto_if_eq VAR_0x8000, DA_PHASE_SWAP, DynamaxAdventureCave_Swap
	goto_if_eq VAR_0x8000, DA_PHASE_FINISHED, DynamaxAdventureCave_Finished
	goto DynamaxAdventureCave_Error
DynamaxAdventureCave_Rental::
    msgbox DynamaxAdventureCave_Text_PlayerRental, MSGBOX_DEFAULT
DynamaxAdventureCave_RentalChoices::
	setvar VAR_0x8004, DA_OP_RENTAL_INFO
	setvar VAR_0x8005, 0
	special RunDynamaxAdventure
	goto_if_eq VAR_RESULT, FALSE, DynamaxAdventureCave_RentalNext0
	dynmultipush DynamaxAdventureCave_Text_Mon, 0
DynamaxAdventureCave_RentalNext0::
	setvar VAR_0x8004, DA_OP_RENTAL_INFO
	setvar VAR_0x8005, 1
	special RunDynamaxAdventure
	goto_if_eq VAR_RESULT, FALSE, DynamaxAdventureCave_RentalNext1
	dynmultipush DynamaxAdventureCave_Text_Mon, 1
DynamaxAdventureCave_RentalNext1::
	setvar VAR_0x8004, DA_OP_RENTAL_INFO
	setvar VAR_0x8005, 2
	special RunDynamaxAdventure
	goto_if_eq VAR_RESULT, FALSE, DynamaxAdventureCave_RentalNext2
	dynmultipush DynamaxAdventureCave_Text_Mon, 2
DynamaxAdventureCave_RentalNext2::
	setvar VAR_0x8004, DA_OP_RENTAL_INFO
	setvar VAR_0x8005, 3
	special RunDynamaxAdventure
	goto_if_eq VAR_RESULT, FALSE, DynamaxAdventureCave_RentalNext3
	dynmultipush DynamaxAdventureCave_Text_Mon, 3
DynamaxAdventureCave_RentalNext3::
	setvar VAR_0x8004, DA_OP_RENTAL_INFO
	setvar VAR_0x8005, 4
	special RunDynamaxAdventure
	goto_if_eq VAR_RESULT, FALSE, DynamaxAdventureCave_RentalNext4
	dynmultipush DynamaxAdventureCave_Text_Mon, 4
DynamaxAdventureCave_RentalNext4::
	setvar VAR_0x8004, DA_OP_RENTAL_INFO
	setvar VAR_0x8005, 5
	special RunDynamaxAdventure
	goto_if_eq VAR_RESULT, FALSE, DynamaxAdventureCave_RentalNext5
	dynmultipush DynamaxAdventureCave_Text_Mon, 5
DynamaxAdventureCave_RentalNext5::
	setvar VAR_0x8004, DA_OP_RENTAL_INFO
	setvar VAR_0x8005, 6
	special RunDynamaxAdventure
	goto_if_eq VAR_RESULT, FALSE, DynamaxAdventureCave_RentalNext6
	dynmultipush DynamaxAdventureCave_Text_Mon, 6
DynamaxAdventureCave_RentalNext6::
	setvar VAR_0x8004, DA_OP_RENTAL_INFO
	setvar VAR_0x8005, 7
	special RunDynamaxAdventure
	goto_if_eq VAR_RESULT, FALSE, DynamaxAdventureCave_RentalNext7
	dynmultipush DynamaxAdventureCave_Text_Mon, 7
DynamaxAdventureCave_RentalNext7::
	setvar VAR_0x8004, DA_OP_RENTAL_INFO
	setvar VAR_0x8005, 8
	special RunDynamaxAdventure
	goto_if_eq VAR_RESULT, FALSE, DynamaxAdventureCave_RentalNext8
	dynmultipush DynamaxAdventureCave_Text_Mon, 8
DynamaxAdventureCave_RentalNext8::
	setvar VAR_0x8004, DA_OP_RENTAL_INFO
	setvar VAR_0x8005, 9
	special RunDynamaxAdventure
	goto_if_eq VAR_RESULT, FALSE, DynamaxAdventureCave_RentalNext9
	dynmultipush DynamaxAdventureCave_Text_Mon, 9
DynamaxAdventureCave_RentalNext9::
	setvar VAR_0x8004, DA_OP_RENTAL_INFO
	setvar VAR_0x8005, 10
	special RunDynamaxAdventure
	goto_if_eq VAR_RESULT, FALSE, DynamaxAdventureCave_RentalNext10
	dynmultipush DynamaxAdventureCave_Text_Mon, 10
DynamaxAdventureCave_RentalNext10::
	setvar VAR_0x8004, DA_OP_RENTAL_INFO
	setvar VAR_0x8005, 11
	special RunDynamaxAdventure
	goto_if_eq VAR_RESULT, FALSE, DynamaxAdventureCave_RentalNext11
	dynmultipush DynamaxAdventureCave_Text_Mon, 11
DynamaxAdventureCave_RentalNext11::
	dynmultistack 1, 1, FALSE, 3, FALSE, 0, DYN_MULTICHOICE_CB_ADVENTURE_PKMN
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, DynamaxAdventureCave_Pause
	setvar VAR_0x8004, DA_OP_CHOOSE_RENTAL
	copyvar VAR_0x8005, VAR_RESULT
	special RunDynamaxAdventure
	goto_if_eq VAR_RESULT, FALSE, DynamaxAdventureCave_Error
	goto DynamaxAdventureCave_Dispatch
DynamaxAdventureCave_Path::
    goto_if_eq VAR_0x8001, 3, DynamaxAdventureCave_FinalPath
	msgbox DynamaxAdventureCave_Text_Path, MSGBOX_DEFAULT
	setvar VAR_0x8004, DA_OP_PATH_INFO
	setvar VAR_0x8005, 0
	special RunDynamaxAdventure
	dynmultipush DynamaxAdventureCave_Text_Mon, 0
	setvar VAR_0x8004, DA_OP_PATH_INFO
	setvar VAR_0x8005, 1
	special RunDynamaxAdventure
	dynmultipush DynamaxAdventureCave_Text_Mon, 1
	dynmultistack 1, 1, FALSE, 3, FALSE, 0, DYN_MULTICHOICE_CB_NONE
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, DynamaxAdventureCave_Pause
	setvar VAR_0x8004, DA_OP_CHOOSE_PATH
	copyvar VAR_0x8005, VAR_RESULT
	special RunDynamaxAdventure
	goto_if_eq VAR_RESULT, FALSE, DynamaxAdventureCave_Error
	goto DynamaxAdventureCave_Dispatch
DynamaxAdventureCave_FinalPath::
    msgbox DynamaxAdventureCave_Text_FinalPath, MSGBOX_DEFAULT
    setvar VAR_0x8004, DA_OP_CHOOSE_PATH
    setvar VAR_0x8005, 0
    special RunDynamaxAdventure
    goto DynamaxAdventureCave_Dispatch
DynamaxAdventureCave_Ready::
	buffernumberstring STR_VAR_1, VAR_0x8002
	msgbox DynamaxAdventureCave_Text_Ready, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, DynamaxAdventureCave_Pause
	closemessage
	setvar VAR_0x8004, DA_OP_START_BATTLE
	special RunDynamaxAdventure
	goto_if_eq VAR_RESULT, FALSE, DynamaxAdventureCave_Error
    waitstate
    warp MAP_DYNAMAX_ADVENTURE_REST_CAVE, 7, 7
    waitstate
    lockall
    goto DynamaxAdventureCave_Dispatch
DynamaxAdventureCave_Catch::
	msgbox DynamaxAdventureCave_Text_Catch, MSGBOX_YESNO
	setvar VAR_0x8004, DA_OP_CATCH
	copyvar VAR_0x8005, VAR_RESULT
	special RunDynamaxAdventure
	goto_if_eq VAR_RESULT, FALSE, DynamaxAdventureCave_Error
	goto DynamaxAdventureCave_AfterCatch
DynamaxAdventureCave_Swap::
	msgbox DynamaxAdventureCave_Text_Swap, MSGBOX_DEFAULT
	dynmultipush DynamaxAdventureCave_Text_Keep, 0
	dynmultipush DynamaxAdventureCave_Text_ReplacePlayer, 1
	dynmultistack 1, 1, FALSE, 3, FALSE, 0, DYN_MULTICHOICE_CB_NONE
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, DynamaxAdventureCave_Keep
	setvar VAR_0x8004, DA_OP_SWAP
	copyvar VAR_0x8005, VAR_RESULT
	special RunDynamaxAdventure
	goto DynamaxAdventureCave_AfterCatch
DynamaxAdventureCave_Keep::
	setvar VAR_0x8004, DA_OP_SWAP
	setvar VAR_0x8005, 0
	special RunDynamaxAdventure
	goto DynamaxAdventureCave_AfterCatch
DynamaxAdventureCave_AfterCatch::
    setvar VAR_0x8004, DA_OP_STATUS
    special RunDynamaxAdventure
    goto_if_ne VAR_0x8000, DA_PHASE_PATH, DynamaxAdventureCave_Dispatch
    msgbox DynamaxAdventureCave_Text_RestBreak, MSGBOX_DEFAULT
    releaseall
    end
DynamaxAdventureCave_Text_RestBreak:
    .string "Take a moment before the next battle.\p"
    .string "BIRCH can switch your rental for\n"
    .string "a POKéMON caught on this adventure.\p"
    .string "Talk to me when you're ready\n"
    .string "to choose the next route.$"
DynamaxAdventureCave_Finished::
	goto_if_eq VAR_0x8003, 0, DynamaxAdventureCave_NoCatches
	msgbox DynamaxAdventureCave_Text_Rewards, MSGBOX_DEFAULT
	copyvar VAR_0x8008, VAR_0x8003
	goto_if_eq VAR_0x8008, 0, DynamaxAdventureCave_RewardMenu
	setvar VAR_0x8004, DA_OP_REWARD_INFO
	setvar VAR_0x8005, 0
	special RunDynamaxAdventure
	dynmultipush DynamaxAdventureCave_Text_RewardMon, 0
	goto_if_eq VAR_0x8008, 1, DynamaxAdventureCave_RewardMenu
	setvar VAR_0x8004, DA_OP_REWARD_INFO
	setvar VAR_0x8005, 1
	special RunDynamaxAdventure
	dynmultipush DynamaxAdventureCave_Text_RewardMon, 1
	goto_if_eq VAR_0x8008, 2, DynamaxAdventureCave_RewardMenu
	setvar VAR_0x8004, DA_OP_REWARD_INFO
	setvar VAR_0x8005, 2
	special RunDynamaxAdventure
	dynmultipush DynamaxAdventureCave_Text_RewardMon, 2
	goto_if_eq VAR_0x8008, 3, DynamaxAdventureCave_RewardMenu
	setvar VAR_0x8004, DA_OP_REWARD_INFO
	setvar VAR_0x8005, 3
	special RunDynamaxAdventure
	dynmultipush DynamaxAdventureCave_Text_RewardMon, 3
DynamaxAdventureCave_RewardMenu::
	dynmultistack 1, 1, FALSE, 3, FALSE, 0, DYN_MULTICHOICE_CB_ADVENTURE_PKMN
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, DynamaxAdventureCave_RewardLater
	copyvar VAR_0x8009, VAR_RESULT
	setvar VAR_0x8004, DA_OP_REWARD_INFO
	copyvar VAR_0x8005, VAR_0x8009
	special RunDynamaxAdventure
	msgbox DynamaxAdventureCave_Text_ConfirmReward, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, DynamaxAdventureCave_Dispatch
	setvar VAR_0x8004, DA_OP_CLAIM
	copyvar VAR_0x8005, VAR_0x8009
	special RunDynamaxAdventure
	goto_if_eq VAR_RESULT, FALSE, DynamaxAdventureCave_StorageFull
	playfanfare MUS_OBTAIN_ITEM
	msgbox DynamaxAdventureCave_Text_Received, MSGBOX_DEFAULT
	waitfanfare
	goto DynamaxAdventureCave_Leave
DynamaxAdventureCave_NoCatches::
	setvar VAR_0x8004, DA_OP_QUIT
	special RunDynamaxAdventure
	msgbox DynamaxAdventureCave_Text_NoCatches, MSGBOX_DEFAULT
	goto DynamaxAdventureCave_Leave
DynamaxAdventureCave_StorageFull::
	msgbox DynamaxAdventureCave_Text_StorageFull, MSGBOX_DEFAULT
	goto DynamaxAdventureCave_Leave
DynamaxAdventureCave_RewardLater::
	msgbox DynamaxAdventureCave_Text_RewardLater, MSGBOX_DEFAULT
	goto DynamaxAdventureCave_Leave
DynamaxAdventureCave_Pause::
	msgbox DynamaxAdventureCave_Text_Pause, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, DynamaxAdventureCave_Dispatch
	goto DynamaxAdventureCave_Leave
DynamaxAdventureCave_Error::
	msgbox DynamaxAdventureCave_Text_Error, MSGBOX_DEFAULT
DynamaxAdventureCave_Leave::
    closemessage
    setvar VAR_0x8004, DA_OP_IN_REST_CAVE
    special RunDynamaxAdventure
    goto_if_eq VAR_RESULT, FALSE, DynamaxAdventureCave_Release
    warp MAP_DYNAMAX_ADVENTURE_CAVE, 7, 7
    waitstate
DynamaxAdventureCave_Release::
    releaseall
	end

DynamaxAdventureCave_Text_Welcome:
	.string "Welcome to our DYNAMAX research cave!\n"
	.string "Care to join an adventure?\p"
	.string "{RIVAL} will battle beside you.\n"
	.string "BIRCH will help us between battles.\p"
	.string "Rentals and route POKéMON are Lv. 70!\n"
	.string "The final legendary is Lv. 100.\p"
	.string "Your rentals can use MEGA or G-MAX.\n"
	.string "You share four chances to win.\p"
	.string "Catches are free. At the end, you may\n"
	.string "keep one of the POKéMON you caught.\p"
	.string "Your own party stays safe with us.\n"
	.string "Would you like to start?$"

DynamaxAdventureCave_Text_Resume:
	.string "Your adventure is still waiting.\n"
	.string "Shall we continue?$"

DynamaxAdventureCave_Text_PlayerRental:
	.string "Choose your rental POKéMON.\n"
	.string "Your own party will be kept safe.$"

DynamaxAdventureCave_Text_RivalRental:
	.string "{RIVAL} chooses a separate rental.\n"
	.string "It must be a different POKéMON.$"

DynamaxAdventureCave_Text_Path:
	.string "Choose the next POKéMON to face.\n"
	.string "Choose a MEGA or G-MAX route!$"

DynamaxAdventureCave_Text_Ready:
	.string "You have {STR_VAR_1} chances left.\n"
	.string "Ready for the next battle?$"

DynamaxAdventureCave_Text_Catch:
	.string "Excellent! Would you like to catch\n"
	.string "that POKéMON? No ball is needed.$"

DynamaxAdventureCave_Text_Swap:
	.string "Replace a rental with this catch?\n"
	.string "Or keep your current team.$"

DynamaxAdventureCave_Text_Rewards:
	.string "Your adventure is over! Choose one\n"
	.string "caught POKéMON to take home.$"

DynamaxAdventureCave_Text_ConfirmReward:
	.string "Take {STR_VAR_1} ({STR_VAR_2})?\n"
	.string "The other catches will be released.$"

DynamaxAdventureCave_Text_Received:
	.string "You received {STR_VAR_1}!\n"
	.string "Come back for another adventure!$"

DynamaxAdventureCave_Text_NoCatches:
	.string "No catches this time. Your party\n"
	.string "is safe. You can try again anytime!$"

DynamaxAdventureCave_Text_StorageFull:
	.string "Your party and PC are full. Make\n"
	.string "room, then return for your reward.\p"
	.string "I will keep your catches here until\n"
	.string "you choose one or reset the game.$"

DynamaxAdventureCave_Text_RewardLater:
	.string "Come back when you are ready to\n"
	.string "choose. Resetting loses the catches!$"

DynamaxAdventureCave_Text_Pause:
	.string "Take a break? Speak to me to resume.\n"
	.string "Resetting the game loses this run.$"

DynamaxAdventureCave_Text_Error:
	.string "We could not continue the adventure.\n"
	.string "Your own party is safe. Try again.$"

DynamaxAdventureCave_Text_Mon:
	.string "{STR_VAR_1}$"

DynamaxAdventureCave_Text_RewardMon:
	.string "{STR_VAR_1} ({STR_VAR_2})$"

DynamaxAdventureCave_Text_Keep:
	.string "Keep my rental$"

DynamaxAdventureCave_Text_ReplacePlayer:
	.string "Replace my rental$"

DynamaxAdventureCave_Text_ReplaceRival:
	.string "Replace {RIVAL}'s rental$"

DynamaxAdventureCave_Text_BossHint:
    .string "My readings indicate a strong\n"
    .string "{STR_VAR_1}-type POKéMON deep inside!$"
DynamaxAdventureCave_Text_FinalPath:
    .string "We have reached the deepest chamber.\n"
    .string "The powerful POKéMON is just ahead!$"

DynamaxAdventureCave_RivalResearch::
    msgbox DynamaxAdventureCave_Text_RivalResearch, MSGBOX_NPC
    end
DynamaxAdventureCave_Text_RivalResearch:
    .string "{RIVAL}: These adventures are part of\n"
    .string "my research as a future professor!\p"
    .string "I want to study how MEGA EVOLUTION\n"
    .string "and GIGANTAMAX affect POKéMON.\p"
    .string "Your help means a lot. Let's make\n"
    .string "some discoveries together!$"

DynamaxAdventureCave_Text_ScientistResearch:
    .string "I monitor the energy in these caves.\n"
    .string "My equipment detects powerful POKéMON!\p"
    .string "You and {RIVAL} will explore together.\n"
    .string "I'll analyze your findings between fights.$"
