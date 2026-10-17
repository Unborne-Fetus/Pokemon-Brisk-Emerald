OrangeCity_Mart_MapScripts::
	.byte 0

OrangeCity_Mart_EventScript_Clerk::
	lock
	faceplayer
	msgbox OrangeCity_Mart_Text_NewStones, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, OrangeCity_Mart_EventScript_RegularSupplies
	pokemart OrangeCity_Mart_Pokemart_CustomMegaStones
	goto OrangeCity_Mart_EventScript_Goodbye

OrangeCity_Mart_EventScript_RegularSupplies::
	message gText_HowMayIServeYou
	waitmessage
	pokemart 0

OrangeCity_Mart_EventScript_Goodbye::
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
OrangeCity_Mart_Pokemart_CustomMegaStones:
	.2byte ITEM_AMOONGUSSITE
	.2byte ITEM_AZUMARILLITE
	.2byte ITEM_BUTTERFREEITE
	.2byte ITEM_CONKELDURRITE
	.2byte ITEM_DARKRANITE_M
	.2byte ITEM_DUGTRIOITE
	.2byte ITEM_ELECTIVIREITE
	.2byte ITEM_EXCADRITE_M
	.2byte ITEM_FERALIGITE_M
	.2byte ITEM_FLYGONITE
	.2byte ITEM_GOLEMITE
	.2byte ITEM_JUMPLUFFITE
	.2byte ITEM_KABUTOPSITE
	.2byte ITEM_MAGMORTARITE
	.2byte ITEM_MEGANIUMITE_M
	.2byte ITEM_MILOTICITE
	.2byte ITEM_OMASTARITE
	.2byte ITEM_POLIWRATHITE
	.2byte ITEM_PYROARITE_M
	.2byte ITEM_SHIFTRYITE
	.2byte ITEM_SNORLAXITE
	.2byte ITEM_TORKOALITE
	.2byte ITEM_TYPHLOSIONITE
	.2byte ITEM_NIDOQUEENITE
	.2byte ITEM_NIDOKINGITE
	.2byte ITEM_TROPIUSITE
	.2byte ITEM_RILLABOOMITE
	.2byte ITEM_SAMUROTTITE_H
	.2byte ITEM_SKELEDIRGITE
	.2byte ITEM_DUSKNOIRITE
	.2byte ITEM_EMPOLEONITE_X
	.2byte ITEM_EMPOLEONITE_Y
	.2byte ITEM_INFERNITE
	.2byte ITEM_LUXRAYITE
	.2byte ITEM_ROSERADITE
	.2byte ITEM_SLAKINGITE
	.2byte ITEM_TORTERRITE
	.2byte ITEM_CLAWITZERITE
	.2byte ITEM_TINKATONITE
	.2byte ITEM_ARBOLIVITE
	.2byte ITEM_CRYOGONALITE
	.2byte ITEM_HAXORITE
	.2byte ITEM_DODRIONITE
	.2byte ITEM_SIGILYPHITE
	.2byte ITEM_REUNICLUSITE
	.2byte ITEM_RAPIDASHITE
	.2byte ITEM_VESPIQUENITE
	.2byte ITEM_EXPLOUDITE
	.2byte ITEM_MAMOSWINITE
	.2byte ITEM_TSAREENITE
	.2byte ITEM_BRELOOMITE
	.2byte ITEM_NINETALITE
	.2byte ITEM_WHIMSICOTTITE
	.2byte ITEM_DRAGONINITE_CDRX
	.2byte ITEM_DRIFBLIMITE
	.2byte ITEM_LAPRASITE
	.2byte ITEM_SEVIPRITE
	.2byte ITEM_HYDREIGONITE
	.2byte ITEM_REGIGIGITE
	.2byte ITEM_NOIVERNITE
	.2byte ITEM_KINGDRITE
	.2byte ITEM_FEAROWITE
	.2byte ITEM_TOGEKISSITE
	.2byte ITEM_VOLCARONITE
	.2byte ITEM_ZANGOOSITE
	.2byte ITEM_QUAGSIRITE
	.2byte ITEM_XATUNITE
	pokemartlistend

OrangeCity_Mart_Text_NewStones:
	.string "Welcome! Have you heard about the\n"
	.string "newly discovered MEGA STONES?\p"
	.string "They let even more kinds of POKéMON\n"
	.string "undergo MEGA EVOLUTION!\p"
	.string "I've stocked every new variety\n"
	.string "right here in ORANGE CITY.\p"
	.string "Would you like to see the stones?\n"
	.string "Choose NO for our regular supplies.$"
