EventScript_PictureBookShelf::
	msgbox Text_PictureBookShelf, MSGBOX_SIGN
	end

EventScript_RestBench::
	lockall
	call EventScript_RestBench_HealParty
	callnative UpdateFollowingPokemon
	playse SE_USE_ITEM
	msgbox Text_RestBench, MSGBOX_DEFAULT
	releaseall
	end

@ Heal exactly as a Center does, without changing the last heal location.
EventScript_RestBench_HealParty::
	special HealPlayerParty
	return

EventScript_Mailbox::
	msgbox Text_Mailbox, MSGBOX_SIGN
	end

EventScript_BookShelf::
	msgbox Text_BookShelf, MSGBOX_SIGN
	end

EventScript_PokemonCenterBookShelf::
	msgbox Text_PokemonCenterBookShelf, MSGBOX_SIGN
	end

EventScript_Vase::
	msgbox Text_Vase, MSGBOX_SIGN
	end

EventScript_EmptyTrashCan::
	msgbox Text_EmptyTrashCan, MSGBOX_SIGN
	end

EventScript_ShopShelf::
	msgbox Text_ShopShelf, MSGBOX_SIGN
	end

EventScript_Blueprint::
	msgbox Text_Blueprint, MSGBOX_SIGN
	end
