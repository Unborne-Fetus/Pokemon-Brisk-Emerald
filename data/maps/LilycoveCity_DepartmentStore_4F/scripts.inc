LilycoveCity_DepartmentStore_4F_MapScripts::
	.byte 0

LilycoveCity_DepartmentStore_4F_EventScript_Gentleman::
	msgbox LilycoveCity_DepartmentStore_4F_Text_AttackOrDefenseTM, MSGBOX_NPC
	end

LilycoveCity_DepartmentStore_4F_EventScript_Woman::
	msgbox LilycoveCity_DepartmentStore_4F_Text_FiftyDifferentTMs, MSGBOX_NPC
	end

LilycoveCity_DepartmentStore_4F_EventScript_Youngster::
	msgbox LilycoveCity_DepartmentStore_4F_Text_PokemonOnlyHaveFourMoves, MSGBOX_NPC
	end

LilycoveCity_DepartmentStore_4F_EventScript_ClerkLeft::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart LilycoveCity_DepartmentStore_4F_Pokemart_AttackTMs
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
LilycoveCity_DepartmentStore_4F_Pokemart_AttackTMs:
	.2byte ITEM_TM_FIRE_BLAST
	.2byte ITEM_TM_THUNDER
	.2byte ITEM_TM_BLIZZARD
	.2byte ITEM_TM_HYPER_BEAM
	.2byte ITEM_TM_PROTECT
	.2byte ITEM_TM_SAFEGUARD
	.2byte ITEM_TM_REFLECT
	.2byte ITEM_TM_LIGHT_SCREEN
	pokemartlistend

LilycoveCity_DepartmentStore_4F_EventScript_ClerkRight::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart LilycoveCity_DepartmentStore_4F_Pokemart_DefenseTMs
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
LilycoveCity_DepartmentStore_4F_Pokemart_DefenseTMs:
	.2byte ITEM_LUCKY_EGG
	.2byte ITEM_FIRE_GEM
	.2byte ITEM_WATER_GEM
	.2byte ITEM_ELECTRIC_GEM
	.2byte ITEM_GRASS_GEM
	.2byte ITEM_ICE_GEM
	.2byte ITEM_FIGHTING_GEM
	.2byte ITEM_POISON_GEM
	.2byte ITEM_GROUND_GEM
	.2byte ITEM_FLYING_GEM
	.2byte ITEM_PSYCHIC_GEM
	.2byte ITEM_BUG_GEM
	.2byte ITEM_ROCK_GEM
	.2byte ITEM_GHOST_GEM
	.2byte ITEM_DRAGON_GEM
	.2byte ITEM_DARK_GEM
	.2byte ITEM_STEEL_GEM
	.2byte ITEM_NORMAL_GEM
	.2byte ITEM_FAIRY_GEM
	.2byte ITEM_ADAMANT_CRYSTAL
	.2byte ITEM_LUSTROUS_GLOBE
	.2byte ITEM_RUSTED_SHIELD
	.2byte ITEM_RUSTED_SWORD
	.2byte ITEM_GRISEOUS_CORE
	.2byte ITEM_WELLSPRING_MASK
	.2byte ITEM_HEARTHFLAME_MASK
	.2byte ITEM_CORNERSTONE_MASK
	.2byte ITEM_BUGINIUM_Z
	.2byte ITEM_DARKINIUM_Z
	.2byte ITEM_DRAGONIUM_Z
	.2byte ITEM_ELECTRIUM_Z
	.2byte ITEM_FAIRIUM_Z
	.2byte ITEM_FIGHTINIUM_Z
	.2byte ITEM_FIRIUM_Z
	.2byte ITEM_FLYINIUM_Z
	.2byte ITEM_GHOSTIUM_Z
	.2byte ITEM_GRASSIUM_Z
	.2byte ITEM_GROUNDIUM_Z
	.2byte ITEM_ICIUM_Z
	.2byte ITEM_NORMALIUM_Z
	.2byte ITEM_POISONIUM_Z
	.2byte ITEM_PSYCHIUM_Z
	.2byte ITEM_ROCKIUM_Z
	.2byte ITEM_STEELIUM_Z
	.2byte ITEM_WATERIUM_Z
	.2byte ITEM_ALORAICHIUM_Z
	.2byte ITEM_DECIDIUM_Z
	.2byte ITEM_EEVIUM_Z
	.2byte ITEM_INCINIUM_Z
	.2byte ITEM_KOMMONIUM_Z
	.2byte ITEM_LUNALIUM_Z
	.2byte ITEM_LYCANIUM_Z
	.2byte ITEM_MARSHADIUM_Z
	.2byte ITEM_MEWNIUM_Z
	.2byte ITEM_MIMIKIUM_Z
	.2byte ITEM_PIKANIUM_Z
	.2byte ITEM_PIKASHUNIUM_Z
	.2byte ITEM_PRIMARIUM_Z
	.2byte ITEM_SNORLIUM_Z
	.2byte ITEM_SOLGANIUM_Z
	.2byte ITEM_TAPUNIUM_Z
	.2byte ITEM_ULTRANECROZIUM_Z
	pokemartlistend

LilycoveCity_DepartmentStore_4F_Text_AttackOrDefenseTM:
	.string "Hmm…\p"
	.string "An attacking move…\n"
	.string "Or a defensive move…\p"
	.string "It's no easy matter to decide which TM\n"
	.string "moves should be taught to POKéMON…$"

LilycoveCity_DepartmentStore_4F_Text_FiftyDifferentTMs:
	.string "There are so many different kinds of\n"
	.string "TM moves.\p"
	.string "A catalog I read said there are fifty\n"
	.string "different kinds.$"

LilycoveCity_DepartmentStore_4F_Text_PokemonOnlyHaveFourMoves:
	.string "I'd like to get all the different TMs,\n"
	.string "but a POKéMON learns only four moves.$"


LilycoveCity_DepartmentStore_4F_EventScript_ClerkTOPRight::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart LilycoveCity_DepartmentStore_4F_Pokemart_TOPRIGHT
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
LilycoveCity_DepartmentStore_4F_Pokemart_TOPRIGHT:
	.2byte ITEM_GENGARITE
	.2byte ITEM_GARDEVOIRITE
	.2byte ITEM_AMPHAROSITE
	.2byte ITEM_VENUSAURITE
	.2byte ITEM_CHARIZARDITE_X
	.2byte ITEM_CHARIZARDITE_Y
	.2byte ITEM_BLASTOISINITE
	.2byte ITEM_MEWTWONITE_X
	.2byte ITEM_MEWTWONITE_Y
	.2byte ITEM_BLAZIKENITE
	.2byte ITEM_MEDICHAMITE
	.2byte ITEM_HOUNDOOMINITE
	.2byte ITEM_AGGRONITE
	.2byte ITEM_BANETTITE
	.2byte ITEM_TYRANITARITE
	.2byte ITEM_PINSIRITE
	.2byte ITEM_AERODACTYLITE
	.2byte ITEM_SCIZORITE
	.2byte ITEM_LUCARIONITE
	.2byte ITEM_LUCARIONITE_Z
	.2byte ITEM_ABOMASITE
	.2byte ITEM_KANGASKHANITE
	.2byte ITEM_GYARADOSITE
	.2byte ITEM_ABSOLITE
	.2byte ITEM_ABSOLITE_Z
	.2byte ITEM_ALAKAZITE
	.2byte ITEM_HERACRONITE
	.2byte ITEM_MAWILITE
	.2byte ITEM_MANECTITE
	.2byte ITEM_GARCHOMPITE
	.2byte ITEM_GARCHOMPITE_Z
	.2byte ITEM_LATIASITE
	.2byte ITEM_LATIOSITE
	.2byte ITEM_SWAMPERTITE
	.2byte ITEM_SCEPTILITE
	.2byte ITEM_SABLENITE
	.2byte ITEM_ALTARIANITE
	.2byte ITEM_GALLADITE
	.2byte ITEM_AUDINITE
	.2byte ITEM_METAGROSSITE
	.2byte ITEM_SHARPEDONITE
	.2byte ITEM_SLOWBRONITE
	.2byte ITEM_STEELIXITE
	.2byte ITEM_PIDGEOTITE
	.2byte ITEM_GLALITITE
	.2byte ITEM_DIANCITE
	.2byte ITEM_CAMERUPTITE
	.2byte ITEM_LOPUNNITE
	.2byte ITEM_SALAMENCITE
	.2byte ITEM_BEEDRILLITE
	.2byte ITEM_CLEFABLITE
	.2byte ITEM_VICTREEBELITE
	.2byte ITEM_STARMINITE
	.2byte ITEM_DRAGONINITE
	.2byte ITEM_MEGANIUMITE
	.2byte ITEM_FERALIGITE
	.2byte ITEM_SKARMORITE
	.2byte ITEM_FROSLASSITE
	.2byte ITEM_HEATRANITE
	.2byte ITEM_DARKRANITE
	.2byte ITEM_EMBOARITE
	.2byte ITEM_EXCADRITE
	.2byte ITEM_SCOLIPITE
	.2byte ITEM_SCRAFTINITE
	.2byte ITEM_EELEKTROSSITE
	.2byte ITEM_CHANDELURITE
	.2byte ITEM_CHESNAUGHTITE
	.2byte ITEM_DELPHOXITE
	.2byte ITEM_GRENINJITE
	.2byte ITEM_PYROARITE
	.2byte ITEM_FLOETTITE
	.2byte ITEM_MALAMARITE
	.2byte ITEM_BARBARACITE
	.2byte ITEM_DRAGALGITE
	.2byte ITEM_HAWLUCHANITE
	.2byte ITEM_ZYGARDITE
	.2byte ITEM_DRAMPANITE
	.2byte ITEM_ZERAORITE
	.2byte ITEM_FALINKSITE
	.2byte ITEM_RAICHUNITE_X
	.2byte ITEM_RAICHUNITE_Y
	.2byte ITEM_CHIMECHITE
	.2byte ITEM_STARAPTITE
	.2byte ITEM_GOLURKITE
	.2byte ITEM_MEOWSTICITE
	.2byte ITEM_CRABOMINITE
	.2byte ITEM_GOLISOPITE
	.2byte ITEM_MAGEARNITE
	.2byte ITEM_BAXCALIBRITE
	.2byte ITEM_TATSUGIRINITE
	.2byte ITEM_GLIMMORANITE
	pokemartlistend


LilycoveCity_DepartmentStore_4F_EventScript_ClerkTOP::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart LilycoveCity_DepartmentStore_4F_Pokemart_TOP
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
LilycoveCity_DepartmentStore_4F_Pokemart_TOP:
	.2byte ITEM_ABILITY_SHIELD
	.2byte ITEM_AIR_BALLOON
	.2byte ITEM_AMULET_COIN
	.2byte ITEM_ASSAULT_VEST
	.2byte ITEM_BERSERK_GENE
	.2byte ITEM_BIG_ROOT
	.2byte ITEM_BINDING_BAND
	.2byte ITEM_BLUNDER_POLICY
	.2byte ITEM_BRIGHT_POWDER
	.2byte ITEM_CHOICE_BAND
	.2byte ITEM_CHOICE_SCARF
	.2byte ITEM_CHOICE_SPECS
	.2byte ITEM_CLEAR_AMULET
	.2byte ITEM_COVERT_CLOAK
	.2byte ITEM_DAMP_ROCK
	.2byte ITEM_DESTINY_KNOT
	.2byte ITEM_EJECT_BUTTON
	.2byte ITEM_EJECT_PACK
	.2byte ITEM_EXPERT_BELT
	.2byte ITEM_FLAME_ORB
	.2byte ITEM_FLOAT_STONE
	.2byte ITEM_FOCUS_BAND
	.2byte ITEM_FOCUS_SASH
	.2byte ITEM_GRIP_CLAW
	.2byte ITEM_HEAT_ROCK
	.2byte ITEM_HEAVY_DUTY_BOOTS
	.2byte ITEM_ICY_ROCK
	.2byte ITEM_IRON_BALL
	.2byte ITEM_KINGS_ROCK
	.2byte ITEM_LAGGING_TAIL
	.2byte ITEM_LIFE_ORB
	.2byte ITEM_LIGHT_CLAY
	.2byte ITEM_LOADED_DICE
	.2byte ITEM_MACHO_BRACE
	.2byte ITEM_METRONOME
	.2byte ITEM_MENTAL_HERB
	.2byte ITEM_MIRROR_HERB
	.2byte ITEM_MUSCLE_BAND
	.2byte ITEM_POWER_HERB
	.2byte ITEM_PROTECTIVE_PADS
	.2byte ITEM_PUNCHING_GLOVE
	.2byte ITEM_QUICK_CLAW
	.2byte ITEM_RAZOR_CLAW
	.2byte ITEM_RAZOR_FANG
	.2byte ITEM_RED_CARD
	.2byte ITEM_RING_TARGET
	.2byte ITEM_ROCKY_HELMET
	.2byte ITEM_ROOM_SERVICE
	.2byte ITEM_SAFETY_GOGGLES
	.2byte ITEM_SCOPE_LENS
	.2byte ITEM_SHED_SHELL
	.2byte ITEM_SMOKE_BALL
	.2byte ITEM_TERRAIN_EXTENDER
	.2byte ITEM_THROAT_SPRAY
	.2byte ITEM_TOXIC_ORB
	.2byte ITEM_UTILITY_UMBRELLA
	.2byte ITEM_WHITE_HERB
	.2byte ITEM_WIDE_LENS
	.2byte ITEM_WISE_GLASSES
	.2byte ITEM_ZOOM_LENS
	.2byte ITEM_LEFTOVERS
	.2byte ITEM_SHELL_BELL
	pokemartlistend


LilycoveCity_DepartmentStore_4F_EventScript_ClerkTOPLeft::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart LilycoveCity_DepartmentStore_4F_Pokemart_TOPLEFT
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
LilycoveCity_DepartmentStore_4F_Pokemart_TOPLEFT:
	.2byte ITEM_BLACK_BELT
	.2byte ITEM_BLACK_GLASSES
	.2byte ITEM_CHARCOAL
	.2byte ITEM_DRAGON_FANG
	.2byte ITEM_FAIRY_FEATHER
	.2byte ITEM_HARD_STONE
	.2byte ITEM_MAGNET
	.2byte ITEM_METAL_COAT
	.2byte ITEM_MIRACLE_SEED
	.2byte ITEM_MYSTIC_WATER
	.2byte ITEM_NEVER_MELT_ICE
	.2byte ITEM_POISON_BARB
	.2byte ITEM_SHARP_BEAK
	.2byte ITEM_SILK_SCARF
	.2byte ITEM_SILVER_POWDER
	.2byte ITEM_SOFT_SAND
	.2byte ITEM_SPELL_TAG
	.2byte ITEM_TWISTED_SPOON
	.2byte ITEM_DOUSE_DRIVE
	.2byte ITEM_SHOCK_DRIVE
	.2byte ITEM_BURN_DRIVE
	.2byte ITEM_CHILL_DRIVE
	.2byte ITEM_BUG_MEMORY
	.2byte ITEM_ICE_MEMORY
	.2byte ITEM_DARK_MEMORY
	.2byte ITEM_FIRE_MEMORY
	.2byte ITEM_ROCK_MEMORY
	.2byte ITEM_FAIRY_MEMORY
	.2byte ITEM_GHOST_MEMORY
	.2byte ITEM_GRASS_MEMORY
	.2byte ITEM_STEEL_MEMORY
	.2byte ITEM_WATER_MEMORY
	.2byte ITEM_DRAGON_MEMORY
	.2byte ITEM_FLYING_MEMORY
	.2byte ITEM_GROUND_MEMORY
	.2byte ITEM_POISON_MEMORY
	.2byte ITEM_PSYCHIC_MEMORY
	.2byte ITEM_ELECTRIC_MEMORY
	.2byte ITEM_FIGHTING_MEMORY
	.2byte ITEM_POWER_ANKLET
	.2byte ITEM_POWER_BAND
	.2byte ITEM_POWER_BELT
	.2byte ITEM_POWER_BRACER
	.2byte ITEM_POWER_LENS
	.2byte ITEM_POWER_WEIGHT
	.2byte ITEM_FULL_INCENSE
	.2byte ITEM_LAX_INCENSE
	.2byte ITEM_LUCK_INCENSE
	.2byte ITEM_ODD_INCENSE
	.2byte ITEM_PURE_INCENSE
	.2byte ITEM_ROCK_INCENSE
	.2byte ITEM_ROSE_INCENSE
	.2byte ITEM_SEA_INCENSE
	.2byte ITEM_WAVE_INCENSE
	.2byte ITEM_EVIOLITE
	.2byte ITEM_LEEK
	.2byte ITEM_LIGHT_BALL
	.2byte ITEM_LUCKY_PUNCH
	.2byte ITEM_METAL_POWDER
	.2byte ITEM_QUICK_POWDER
	.2byte ITEM_SOUL_DEW
	.2byte ITEM_THICK_CLUB
	pokemartlistend
