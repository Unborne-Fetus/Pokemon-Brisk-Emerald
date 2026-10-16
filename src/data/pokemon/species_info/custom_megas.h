// Stats, types and abilities match the supplied custom Mega reference cards.
// Each form is activated by its distinct held Mega Stone.
    [SPECIES_AMOONGUSS_MEGA] =
    {
        .baseHP        = 114,
        .baseAttack    = 90,
        .baseDefense   = 120,
        .baseSpeed     = 30,
        .baseSpAttack  = 100,
        .baseSpDefense = 110,
        .types = MON_TYPES(TYPE_GRASS, TYPE_POISON),
        .catchRate = 75,
        .expYield = 162,
        .evYield_HP = 2,
        .itemCommon = ITEM_TINY_MUSHROOM,
        .itemRare = ITEM_BIG_MUSHROOM,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_GRASS),
        .abilities = { ABILITY_EFFECT_SPORE, ABILITY_EFFECT_SPORE, ABILITY_EFFECT_SPORE },
        .bodyColor = BODY_COLOR_WHITE,
        .noFlip = TRUE,
        .speciesName = _("Amoonguss"),
        .cryId = CRY_AMOONGUSS,
        .natDexNum = NATIONAL_DEX_AMOONGUSS,
        .categoryName = _("Mushroom"),
        .height = 6,
        .weight = 105,
        .description = COMPOUND_STRING(
            "It lures prey close by dancing and waving\n"
            "its arm caps, which resemble Poké Balls,\n"
            "in a swaying motion, but very few Pokémon\n"
            "are fooled by this."),
        .pokemonScale = 422,
        .pokemonOffset = 13,
        .trainerScale = 256,
        .trainerOffset = 0,
        .iconSprite = gMonIcon_Amoonguss,
        .iconPalIndex = 1,
        .pokemonJumpType = PKMN_JUMP_TYPE_SLOW,
        SHADOW(1, 5, SHADOW_SIZE_M)
        FOOTPRINT(Amoonguss)
        OVERWORLD(
            sPicTable_Amoonguss,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following_Asym,
            gOverworldPalette_Amoonguss,
            gShinyOverworldPalette_Amoonguss
        )
        .levelUpLearnset = sAmoongussLevelUpLearnset,
        .teachableLearnset = sAmoongussTeachableLearnset,
        .frontPic = gMonFrontPic_AmoongussMega,
        .backPic = gMonBackPic_AmoongussMega,
        .frontPic80 = gMonFrontPic80_AmoongussMega,
        .backPic80 = gMonBackPic80_AmoongussMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_AmoongussMega,
        .shinyPalette = gMonShinyPalette_AmoongussMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sAmoongussCustomMegaFormSpeciesIdTable,
        .formChangeTable = sAmoongussCustomMegaFormChangeTable,
    },

    [SPECIES_AZUMARILL_MEGA] =
    {
        .baseHP        = 100,
        .baseAttack    = 80,
        .baseDefense   = 118,
        .baseSpeed     = 42,
        .baseSpAttack  = 60,
        .baseSpDefense = 120,
    #if P_UPDATED_TYPES >= GEN_6
        .types = MON_TYPES(TYPE_WATER, TYPE_FAIRY),
    #else
        .types = MON_TYPES(TYPE_WATER, TYPE_FAIRY),
    #endif
        .catchRate = 75,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 210,
    #elif P_UPDATED_EXP_YIELDS >= GEN_6
        .expYield = 189,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 185,
    #else
        .expYield = 153,
    #endif
        .evYield_HP = 3,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 10,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_1, EGG_GROUP_FAIRY),
        .abilities = { ABILITY_HUGE_POWER, ABILITY_HUGE_POWER, ABILITY_HUGE_POWER },
        .bodyColor = BODY_COLOR_BLUE,
        .noFlip = TRUE,
        .speciesName = _("Azumarill"),
        .cryId = CRY_AZUMARILL,
        .natDexNum = NATIONAL_DEX_AZUMARILL,
        .categoryName = _("Aqua Rabbit"),
        .height = 8,
        .weight = 285,
        .description = COMPOUND_STRING(
            "It lives in water virtually all day long.\n"
            "Its body color and pattern act as\n"
            "camouflage that makes it tough for\n"
            "enemies to spot in water."),
        .pokemonScale = 448,
        .pokemonOffset = 16,
        .trainerScale = 256,
        .trainerOffset = 0,
        .iconSprite = gMonIcon_Azumarill,
        .iconPalIndex = P_GBA_STYLE_SPECIES_ICONS ? 2 : 0,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(-4, 8, SHADOW_SIZE_S)
        FOOTPRINT(Azumarill)
        OVERWORLD(
            sPicTable_Azumarill,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Azumarill,
            gShinyOverworldPalette_Azumarill
        )
        .levelUpLearnset = sAzumarillLevelUpLearnset,
        .teachableLearnset = sAzumarillTeachableLearnset,
        .frontPic = gMonFrontPic_AzumarillMega,
        .backPic = gMonBackPic_AzumarillMega,
        .frontPic80 = gMonFrontPic80_AzumarillMega,
        .backPic80 = gMonBackPic80_AzumarillMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_AzumarillMega,
        .shinyPalette = gMonShinyPalette_AzumarillMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sAzumarillCustomMegaFormSpeciesIdTable,
        .formChangeTable = sAzumarillCustomMegaFormChangeTable,
    },

    [SPECIES_BUTTERFREE_MEGA] =
    {
        .baseHP        = 60,
        .baseAttack    = 25,
        .baseDefense   = 90,
        .baseSpeed     = 100,
        .baseSpAttack  = 110,
        .baseSpDefense = 110,
        .types = MON_TYPES(TYPE_BUG, TYPE_FLYING),
        .catchRate = 45,
        .expYield = BUTTERFREE_EXP_YIELD,
        .evYield_SpAttack = 2,
        .evYield_SpDefense = 1,
        .itemRare = ITEM_SILVER_POWDER,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 15,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_BUG),
        .abilities = { ABILITY_SERENE_GRACE, ABILITY_SERENE_GRACE, ABILITY_SERENE_GRACE },
        .bodyColor = BODY_COLOR_WHITE,
        .speciesName = _("Butterfree"),
        .cryId = CRY_BUTTERFREE,
        .natDexNum = NATIONAL_DEX_BUTTERFREE,
        .categoryName = _("Butterfly"),
        .height = 11,
        .weight = 320,
        .description = COMPOUND_STRING(
            "It has a superior ability to search for\n"
            "delicious honey from flowers. It can seek,\n"
            "extract, and carry honey from flowers\n"
            "blooming over six miles away."),
        .pokemonScale = 312,
        .pokemonOffset = 2,
        .trainerScale = 256,
        .trainerOffset = 0,
        .enemyMonElevation = P_GBA_STYLE_SPECIES_GFX ? 8 : 12,
        .iconSprite = gMonIcon_Butterfree,
        .iconPalIndex = 0,
#if P_GENDER_DIFFERENCES && !P_GBA_STYLE_SPECIES_GFX
#endif //P_GENDER_DIFFERENCES && !P_GBA_STYLE_SPECIES_GFX
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(-3, 13, SHADOW_SIZE_S)
        FOOTPRINT(Butterfree)
        OVERWORLD(
            sPicTable_Butterfree,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Butterfree,
            gShinyOverworldPalette_Butterfree
        )
        OVERWORLD_FEMALE(
            sPicTable_ButterfreeF,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following
        )
        .levelUpLearnset = sButterfreeLevelUpLearnset,
        .teachableLearnset = sButterfreeTeachableLearnset,
        .formSpeciesIdTable = sButterfreeFormSpeciesIdTable,
        .formChangeTable = sButterfreeCustomMegaFormChangeTable,
        .frontPic = gMonFrontPic_ButterfreeMega,
        .backPic = gMonBackPic_ButterfreeMega,
        .frontPic80 = gMonFrontPic80_ButterfreeMega,
        .backPic80 = gMonBackPic80_ButterfreeMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_ButterfreeMega,
        .shinyPalette = gMonShinyPalette_ButterfreeMega,
        .isMegaEvolution = TRUE,
    },

    [SPECIES_CONKELDURR_MEGA] =
    {
        .baseHP        = 105,
        .baseAttack    = 170,
        .baseDefense   = 125,
        .baseSpeed     = 45,
        .baseSpAttack  = 55,
        .baseSpDefense = 105,
        .types = MON_TYPES(TYPE_FIGHTING),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_8) ? 253 : 227,
        .evYield_Attack = 3,
        .genderRatio = PERCENT_FEMALE(25),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
        .abilities = { ABILITY_STAMINA, ABILITY_STAMINA, ABILITY_STAMINA },
        .bodyColor = BODY_COLOR_BROWN,
        .speciesName = _("Conkeldurr"),
        .cryId = CRY_CONKELDURR,
        .natDexNum = NATIONAL_DEX_CONKELDURR,
        .categoryName = _("Muscular"),
        .height = 14,
        .weight = 870,
        .description = COMPOUND_STRING(
            "They use concrete pillars as walking\n"
            "canes. Rather than rely solely on force,\n"
            "they master moves that utilize the\n"
            "centrifugal force of spinning concrete."),
        .pokemonScale = 265,
        .pokemonOffset = 3,
        .trainerScale = 262,
        .trainerOffset = 0,
        .iconSprite = gMonIcon_Conkeldurr,
        .iconPalIndex = 1,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(0, 3, SHADOW_SIZE_XL_BATTLE_ONLY)
        FOOTPRINT(Conkeldurr)
        OVERWORLD(
            sPicTable_Conkeldurr,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Conkeldurr,
            gShinyOverworldPalette_Conkeldurr
        )
        .levelUpLearnset = sConkeldurrLevelUpLearnset,
        .teachableLearnset = sConkeldurrTeachableLearnset,
        .frontPic = gMonFrontPic_ConkeldurrMega,
        .backPic = gMonBackPic_ConkeldurrMega,
        .frontPic80 = gMonFrontPic80_ConkeldurrMega,
        .backPic80 = gMonBackPic80_ConkeldurrMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_ConkeldurrMega,
        .shinyPalette = gMonShinyPalette_ConkeldurrMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sConkeldurrCustomMegaFormSpeciesIdTable,
        .formChangeTable = sConkeldurrCustomMegaFormChangeTable,
    },

    [SPECIES_DARKRAI_MEGA_M] =
    {
        .baseHP        = 70,
        .baseAttack    = 110,
        .baseDefense   = 105,
        .baseSpeed     = 145,
        .baseSpAttack  = 165,
        .baseSpDefense = 105,
        .types = MON_TYPES(TYPE_DARK),
        .catchRate = 3,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 300,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 270,
    #else
        .expYield = 210,
    #endif
        .evYield_Speed = 1,
        .evYield_SpAttack = 2,
        .genderRatio = MON_GENDERLESS,
        .eggCycles = 120,
        .friendship = 0,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_NO_EGGS_DISCOVERED),
        .abilities = { ABILITY_DARK_AURA, ABILITY_DARK_AURA, ABILITY_DARK_AURA },
        .bodyColor = BODY_COLOR_BLACK,
        .noFlip = TRUE,
        .speciesName = _("Darkrai"),
        .cryId = CRY_DARKRAI,
        .natDexNum = NATIONAL_DEX_DARKRAI,
        .categoryName = _("Pitch-Black"),
        .height = 15,
        .weight = 505,
        .description = COMPOUND_STRING(
            "It chases people and Pokémon from its\n"
            "territory by causing them to experience\n"
            "deep, nightmarish slumbers. However,\n"
            "it means no harm."),
        .pokemonScale = 268,
        .pokemonOffset = 2,
        .trainerScale = 271,
        .trainerOffset = 0,
        .enemyMonElevation = 4,
        .iconSprite = gMonIcon_Darkrai,
        .iconPalIndex = 0,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(4, 12, SHADOW_SIZE_M)
        FOOTPRINT(Darkrai)
        OVERWORLD(
            sPicTable_Darkrai,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Darkrai,
            gShinyOverworldPalette_Darkrai
        )
        .isMythical = TRUE,
        .isFrontierBanned = TRUE,
        .perfectIVCount = LEGENDARY_PERFECT_IV_COUNT,
        .levelUpLearnset = sDarkraiLevelUpLearnset,
        .teachableLearnset = sDarkraiTeachableLearnset,
        .formSpeciesIdTable = sDarkraiFormSpeciesIdTable,
        .formChangeTable = sDarkraiCustomMegaFormChangeTable,
        .frontPic = gMonFrontPic_DarkraiMegaM,
        .backPic = gMonBackPic_DarkraiMegaM,
        .frontPic80 = gMonFrontPic80_DarkraiMegaM,
        .backPic80 = gMonBackPic80_DarkraiMegaM,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_DarkraiMegaM,
        .shinyPalette = gMonShinyPalette_DarkraiMegaM,
        .isMegaEvolution = TRUE,
    },

    [SPECIES_DUGTRIO_MEGA] =
    {
        .baseHP        = 35,
        .baseAttack    = 150,
        .baseDefense   = 90,
        .baseSpeed     = 130,
        .baseSpAttack  = 50,
        .baseSpDefense = 70,
        .types = MON_TYPES(TYPE_GROUND, TYPE_FIGHTING),
        .catchRate = 50,
    #if P_UPDATED_EXP_YIELDS >= GEN_7
        .expYield = 149,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 142,
    #else
        .expYield = 153,
    #endif
        .evYield_Speed = 2,
        .itemRare = ITEM_SOFT_SAND,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_UNSEEN_FIST, ABILITY_UNSEEN_FIST, ABILITY_UNSEEN_FIST },
        .bodyColor = BODY_COLOR_BROWN,
        .speciesName = _("Dugtrio"),
        .cryId = CRY_DUGTRIO,
        .natDexNum = NATIONAL_DEX_DUGTRIO,
        .categoryName = _("Mole"),
        .height = 7,
        .weight = 333,
        .description = COMPOUND_STRING(
            "Because the triplets originally split from\n"
            "one body, they think exactly alike.\n"
            "They work cooperatively to burrow\n"
            "endlessly through the ground."),
        .pokemonScale = 406,
        .pokemonOffset = 18,
        .trainerScale = 256,
        .trainerOffset = 0,
        .iconSprite = gMonIcon_Dugtrio,
        .iconPalIndex = 2,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        NO_SHADOW
        FOOTPRINT(Dugtrio)
        OVERWORLD(
            sPicTable_Dugtrio,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_NONE,
            sAnimTable_Following,
            gOverworldPalette_Dugtrio,
            gShinyOverworldPalette_Dugtrio
        )
        .isTelekinesisBanned = TRUE,
        .levelUpLearnset = sDugtrioLevelUpLearnset,
        .teachableLearnset = sDugtrioTeachableLearnset,
        .formSpeciesIdTable = sDugtrioFormSpeciesIdTable,
        .frontPic = gMonFrontPic_DugtrioMega,
        .backPic = gMonBackPic_DugtrioMega,
        .frontPic80 = gMonFrontPic80_DugtrioMega,
        .backPic80 = gMonBackPic80_DugtrioMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_DugtrioMega,
        .shinyPalette = gMonShinyPalette_DugtrioMega,
        .isMegaEvolution = TRUE,
        .formChangeTable = sDugtrioCustomMegaFormChangeTable,
    },

    [SPECIES_ELECTIVIRE_MEGA] =
    {
        .baseHP        = 75,
        .baseAttack    = 168,
        .baseDefense   = 95,
        .baseSpeed     = 110,
        .baseSpAttack  = 70,
        .baseSpDefense = 122,
        .types = MON_TYPES(TYPE_ELECTRIC, TYPE_FIGHTING),
        .catchRate = 30,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 270,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 243,
    #else
        .expYield = 199,
    #endif
        .evYield_Attack = 3,
        .itemRare = ITEM_ELECTIRIZER,
        .genderRatio = PERCENT_FEMALE(25),
        .eggCycles = 25,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
        .abilities = { ABILITY_MOTOR_DRIVE, ABILITY_MOTOR_DRIVE, ABILITY_MOTOR_DRIVE },
        .bodyColor = BODY_COLOR_YELLOW,
        .speciesName = _("Electivire"),
        .cryId = CRY_ELECTIVIRE,
        .natDexNum = NATIONAL_DEX_ELECTIVIRE,
        .categoryName = _("Thunderbolt"),
        .height = 18,
        .weight = 1386,
        .description = COMPOUND_STRING(
            "When it gets excited, it thumps its chest.\n"
            "With every thud, thunder roars, electric\n"
            "sparks shower all around and blue sparks\n"
            "begin to crackle between its horns."),
        .pokemonScale = 267,
        .pokemonOffset = 2,
        .trainerScale = 286,
        .trainerOffset = 1,
        .iconSprite = gMonIcon_Electivire,
        .iconPalIndex = 1,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(-3, 11, SHADOW_SIZE_XL_BATTLE_ONLY)
        FOOTPRINT(Electivire)
        OVERWORLD(
            sPicTable_Electivire,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Electivire,
            gShinyOverworldPalette_Electivire
        )
        .levelUpLearnset = sElectivireLevelUpLearnset,
        .teachableLearnset = sElectivireTeachableLearnset,
        .frontPic = gMonFrontPic_ElectivireMega,
        .backPic = gMonBackPic_ElectivireMega,
        .frontPic80 = gMonFrontPic80_ElectivireMega,
        .backPic80 = gMonBackPic80_ElectivireMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_ElectivireMega,
        .shinyPalette = gMonShinyPalette_ElectivireMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sElectivireCustomMegaFormSpeciesIdTable,
        .formChangeTable = sElectivireCustomMegaFormChangeTable,
    },

    [SPECIES_EXCADRILL_MEGA_M] =
    {
        .baseHP        = 110,
        .baseAttack    = 155,
        .baseDefense   = 110,
        .baseSpeed     = 103,
        .baseSpAttack  = 65,
        .baseSpDefense = 65,
        .types = MON_TYPES(TYPE_GROUND, TYPE_STEEL),
        .catchRate = 60,
        .expYield = 178,
        .evYield_Attack = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_PIERCING_DRILL, ABILITY_PIERCING_DRILL, ABILITY_PIERCING_DRILL },
        .bodyColor = BODY_COLOR_GRAY,
        .noFlip = TRUE,
        .speciesName = _("Excadrill"),
        .cryId = CRY_EXCADRILL,
        .natDexNum = NATIONAL_DEX_EXCADRILL,
        .categoryName = _("Subterrene"),
        .height = 7,
        .weight = 404,
        .description = COMPOUND_STRING(
            "More than 300 feet below the surface,\n"
            "they build mazelike nests. Their activity\n"
            "can be destructive to subway tunnels,\n"
            "but it can also help with construction."),
        .pokemonScale = 365,
        .pokemonOffset = 12,
        .trainerScale = 256,
        .trainerOffset = 0,
        .iconSprite = gMonIcon_Excadrill,
        .iconPalIndex = 0,
        .pokemonJumpType = PKMN_JUMP_TYPE_NORMAL,
        SHADOW(3, 8, SHADOW_SIZE_L)
        FOOTPRINT(Excadrill)
        OVERWORLD(
            sPicTable_Excadrill,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Excadrill,
            gShinyOverworldPalette_Excadrill
        )
        .levelUpLearnset = sExcadrillLevelUpLearnset,
        .teachableLearnset = sExcadrillTeachableLearnset,
        .formSpeciesIdTable = sExcadrillFormSpeciesIdTable,
        .formChangeTable = sExcadrillCustomMegaFormChangeTable,
        .frontPic = gMonFrontPic_ExcadrillMegaM,
        .backPic = gMonBackPic_ExcadrillMegaM,
        .frontPic80 = gMonFrontPic80_ExcadrillMegaM,
        .backPic80 = gMonBackPic80_ExcadrillMegaM,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_ExcadrillMegaM,
        .shinyPalette = gMonShinyPalette_ExcadrillMegaM,
        .isMegaEvolution = TRUE,
    },

    [SPECIES_FERALIGATR_MEGA_M] =
    {
        .baseHP        = 85,
        .baseAttack    = 150,
        .baseDefense   = 130,
        .baseSpeed     = 97,
        .baseSpAttack  = 85,
        .baseSpDefense = 83,
        .types = MON_TYPES(TYPE_WATER, TYPE_DRAGON),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 265,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 239,
    #else
        .expYield = 210,
    #endif
        .evYield_Attack = 2,
        .evYield_Defense = 1,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MONSTER, EGG_GROUP_WATER_1),
        .abilities = { ABILITY_DRAGONIZE, ABILITY_DRAGONIZE, ABILITY_DRAGONIZE },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Feraligatr"),
        .cryId = CRY_FERALIGATR,
        .natDexNum = NATIONAL_DEX_FERALIGATR,
        .categoryName = _("Big Jaw"),
        .height = 23,
        .weight = 888,
        .description = COMPOUND_STRING(
            "It opens its huge mouth to intimidate\n"
            "enemies. In battle, it runs using its thick\n"
            "and powerful hind legs to charge the\n"
            "foe with incredible speed."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 342,
        .trainerOffset = 7,
        .iconSprite = gMonIcon_Feraligatr,
        .iconPalIndex = P_GBA_STYLE_SPECIES_ICONS ? 2 : 0,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(3, 11, SHADOW_SIZE_XL_BATTLE_ONLY)
        FOOTPRINT(Feraligatr)
        OVERWORLD(
            sPicTable_Feraligatr,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Feraligatr,
            gShinyOverworldPalette_Feraligatr
        )
        .levelUpLearnset = sFeraligatrLevelUpLearnset,
        .teachableLearnset = sFeraligatrTeachableLearnset,
        .formSpeciesIdTable = sFeraligatrFormSpeciesIdTable,
        .formChangeTable = sFeraligatrCustomMegaFormChangeTable,
        .frontPic = gMonFrontPic_FeraligatrMegaM,
        .backPic = gMonBackPic_FeraligatrMegaM,
        .frontPic80 = gMonFrontPic80_FeraligatrMegaM,
        .backPic80 = gMonBackPic80_FeraligatrMegaM,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_FeraligatrMegaM,
        .shinyPalette = gMonShinyPalette_FeraligatrMegaM,
        .isMegaEvolution = TRUE,
    },

    [SPECIES_FLYGON_MEGA] =
    {
        .baseHP        = 80,
        .baseAttack    = 140,
        .baseDefense   = 110,
        .baseSpeed     = 100,
        .baseSpAttack  = 80,
        .baseSpDefense = 100,
        .types = MON_TYPES(TYPE_GROUND, TYPE_DRAGON),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 260,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 234,
    #else
        .expYield = 197,
    #endif
        .evYield_Attack = 1,
        .evYield_Speed = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = TRAPINCH_FAMILY_EGG_GROUPS,
        .abilities = { ABILITY_SAND_RUSH, ABILITY_SAND_RUSH, ABILITY_SAND_RUSH },
        .bodyColor = BODY_COLOR_GREEN,
        .speciesName = _("Flygon"),
        .cryId = CRY_FLYGON,
        .natDexNum = NATIONAL_DEX_FLYGON,
        .categoryName = _("Mystic"),
        .height = 20,
        .weight = 820,
        .description = COMPOUND_STRING(
            "The flapping of its wings sounds like\n"
            "singing. To prevent detection by enemies,\n"
            "it hides itself by flapping up a cloud of\n"
            "desert sand."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 268,
        .trainerOffset = 1,
        .enemyMonElevation = 7,
        .iconSprite = gMonIcon_Flygon,
        .iconPalIndex = 1,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(0, 17, SHADOW_SIZE_M)
        FOOTPRINT(Flygon)
        OVERWORLD(
            sPicTable_Flygon,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_NONE,
            sAnimTable_Following,
            gOverworldPalette_Flygon,
            gShinyOverworldPalette_Flygon
        )
        .levelUpLearnset = sFlygonLevelUpLearnset,
        .teachableLearnset = sFlygonTeachableLearnset,
        .frontPic = gMonFrontPic_FlygonMega,
        .backPic = gMonBackPic_FlygonMega,
        .frontPic80 = gMonFrontPic80_FlygonMega,
        .backPic80 = gMonBackPic80_FlygonMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_FlygonMega,
        .shinyPalette = gMonShinyPalette_FlygonMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sFlygonCustomMegaFormSpeciesIdTable,
        .formChangeTable = sFlygonCustomMegaFormChangeTable,
    },

    [SPECIES_GOLEM_MEGA] =
    {
        .baseHP        = 80,
        .baseAttack    = 150,
        .baseDefense   = 160,
        .baseSpeed     = 35,
        .baseSpAttack  = 70,
        .baseSpDefense = 100,
        .types = MON_TYPES(TYPE_ROCK),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 248,
    #elif P_UPDATED_EXP_YIELDS >= GEN_6
        .expYield = 223,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 218,
    #else
        .expYield = 177,
    #endif
        .evYield_Defense = 3,
        .itemRare = ITEM_EVERSTONE,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 15,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MINERAL),
        .abilities = { ABILITY_ROUGH_SKIN, ABILITY_ROUGH_SKIN, ABILITY_ROUGH_SKIN },
        .bodyColor = BODY_COLOR_BROWN,
        .speciesName = _("Golem"),
        .cryId = CRY_GOLEM,
        .natDexNum = NATIONAL_DEX_GOLEM,
        .categoryName = _("Megaton"),
        .height = 14,
        .weight = 3000,
        .description = COMPOUND_STRING(
            "It is said to live in volcanic craters\n"
            "on mountain peaks. Once a year, it sheds\n"
            "its hide and grows larger. The shed hide\n"
            "crumbles and returns to the soil."),
        .pokemonScale = 256,
        .pokemonOffset = 3,
        .trainerScale = 296,
        .trainerOffset = 2,
        .iconSprite = gMonIcon_Golem,
        .iconPalIndex = P_GBA_STYLE_SPECIES_ICONS ? 1 : 2,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(3, 5, SHADOW_SIZE_XL_BATTLE_ONLY)
        FOOTPRINT(Golem)
        OVERWORLD(
            sPicTable_Golem,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Golem,
            gShinyOverworldPalette_Golem
        )
        .levelUpLearnset = sGolemLevelUpLearnset,
        .teachableLearnset = sGolemTeachableLearnset,
        .formSpeciesIdTable = sGolemFormSpeciesIdTable,
        .frontPic = gMonFrontPic_GolemMega,
        .backPic = gMonBackPic_GolemMega,
        .frontPic80 = gMonFrontPic80_GolemMega,
        .backPic80 = gMonBackPic80_GolemMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_GolemMega,
        .shinyPalette = gMonShinyPalette_GolemMega,
        .isMegaEvolution = TRUE,
        .formChangeTable = sGolemCustomMegaFormChangeTable,
    },

    [SPECIES_JUMPLUFF_MEGA] =
    {
        .baseHP        = 75,
        .baseAttack    = 65,
        .baseDefense   = 120,
        .baseSpeed     = 125,
        .baseSpAttack  = 65,
        .baseSpDefense = 110,
        .types = MON_TYPES(TYPE_GRASS, TYPE_FLYING),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 230,
    #elif P_UPDATED_EXP_YIELDS >= GEN_6
        .expYield = 207,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 203,
    #else
        .expYield = 176,
    #endif
        .evYield_Speed = 3,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FAIRY, EGG_GROUP_GRASS),
    #if P_UPDATED_ABILITIES >= GEN_4
        .abilities = { ABILITY_COTTON_DOWN, ABILITY_COTTON_DOWN, ABILITY_COTTON_DOWN },
    #else
        .abilities = { ABILITY_COTTON_DOWN, ABILITY_COTTON_DOWN, ABILITY_COTTON_DOWN },
    #endif
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Jumpluff"),
        .cryId = CRY_JUMPLUFF,
        .natDexNum = NATIONAL_DEX_JUMPLUFF,
        .categoryName = _("Cottonweed"),
        .height = 8,
        .weight = 30,
        .description = COMPOUND_STRING(
            "Jumpluff ride warm southern winds to\n"
            "cross the sea and fly to foreign lands.\n"
            "This Pokémon lands when it encounters\n"
            "cold air while it is floating."),
        .pokemonScale = 418,
        .pokemonOffset = -4,
        .trainerScale = 256,
        .trainerOffset = 0,
        .enemyMonElevation = 9,
        .iconSprite = gMonIcon_Jumpluff,
        .iconPalIndex = 2,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(-2, 11, SHADOW_SIZE_S)
        FOOTPRINT(Jumpluff)
        OVERWORLD(
            sPicTable_Jumpluff,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_NONE,
            sAnimTable_Following,
            gOverworldPalette_Jumpluff,
            gShinyOverworldPalette_Jumpluff
        )
        .levelUpLearnset = sJumpluffLevelUpLearnset,
        .teachableLearnset = sJumpluffTeachableLearnset,
        .frontPic = gMonFrontPic_JumpluffMega,
        .backPic = gMonBackPic_JumpluffMega,
        .frontPic80 = gMonFrontPic80_JumpluffMega,
        .backPic80 = gMonBackPic80_JumpluffMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_JumpluffMega,
        .shinyPalette = gMonShinyPalette_JumpluffMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sJumpluffCustomMegaFormSpeciesIdTable,
        .formChangeTable = sJumpluffCustomMegaFormChangeTable,
    },

    [SPECIES_KABUTOPS_MEGA] =
    {
        .baseHP        = 60,
        .baseAttack    = 145,
        .baseDefense   = 130,
        .baseSpeed     = 105,
        .baseSpAttack  = 75,
        .baseSpDefense = 80,
        .types = MON_TYPES(TYPE_ROCK, TYPE_WATER),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 173,
    #elif P_UPDATED_EXP_YIELDS >= GEN_4
        .expYield = 199,
    #else
        .expYield = 201,
    #endif
        .evYield_Attack = 2,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 30,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_1, EGG_GROUP_WATER_3),
        .abilities = { ABILITY_INTREPID_SWORD, ABILITY_INTREPID_SWORD, ABILITY_INTREPID_SWORD },
        .bodyColor = BODY_COLOR_BROWN,
        .speciesName = _("Kabutops"),
        .cryId = CRY_KABUTOPS,
        .natDexNum = NATIONAL_DEX_KABUTOPS,
        .categoryName = _("Shellfish"),
        .height = 13,
        .weight = 405,
        .description = COMPOUND_STRING(
            "Kabutops once swam underwater to hunt \n"
            "for prey. It was apparently evolving from\n"
            "being a water dweller to living on land as\n"
            "evident from changes in its gills and legs."),
        .pokemonScale = 271,
        .pokemonOffset = 3,
        .trainerScale = 256,
        .trainerOffset = 0,
        .iconSprite = gMonIcon_Kabutops,
        .iconPalIndex = 2,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(1, 10, SHADOW_SIZE_XL_BATTLE_ONLY)
        FOOTPRINT(Kabutops)
        OVERWORLD(
            sPicTable_Kabutops,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Kabutops,
            gShinyOverworldPalette_Kabutops
        )
        .levelUpLearnset = sKabutopsLevelUpLearnset,
        .teachableLearnset = sKabutopsTeachableLearnset,
        .frontPic = gMonFrontPic_KabutopsMega,
        .backPic = gMonBackPic_KabutopsMega,
        .frontPic80 = gMonFrontPic80_KabutopsMega,
        .backPic80 = gMonBackPic80_KabutopsMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_KabutopsMega,
        .shinyPalette = gMonShinyPalette_KabutopsMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sKabutopsCustomMegaFormSpeciesIdTable,
        .formChangeTable = sKabutopsCustomMegaFormChangeTable,
    },

    [SPECIES_MAGMORTAR_MEGA] =
    {
        .baseHP        = 75,
        .baseAttack    = 105,
        .baseDefense   = 122,
        .baseSpeed     = 63,
        .baseSpAttack  = 165,
        .baseSpDefense = 110,
        .types = MON_TYPES(TYPE_FIRE),
        .catchRate = 30,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 270,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 243,
    #else
        .expYield = 199,
    #endif
        .evYield_SpAttack = 3,
        .itemRare = ITEM_MAGMARIZER,
        .genderRatio = PERCENT_FEMALE(25),
        .eggCycles = 25,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
        .abilities = { ABILITY_STEAM_ENGINE, ABILITY_STEAM_ENGINE, ABILITY_STEAM_ENGINE },
        .bodyColor = BODY_COLOR_RED,
        .noFlip = TRUE,
        .speciesName = _("Magmortar"),
        .cryId = CRY_MAGMORTAR,
        .natDexNum = NATIONAL_DEX_MAGMORTAR,
        .categoryName = _("Blast"),
        .height = 16,
        .weight = 680,
        .description = COMPOUND_STRING(
            "According to what is known, a single pair\n"
            "of male and female Magmortar lives in\n"
            "one volcano. From its arm, it launches\n"
            "fireballs hotter than 3,600ºF."),
        .pokemonScale = 259,
        .pokemonOffset = 1,
        .trainerScale = 296,
        .trainerOffset = 1,
        .iconSprite = gMonIcon_Magmortar,
        .iconPalIndex = 0,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(4, 11, SHADOW_SIZE_XL_BATTLE_ONLY)
        FOOTPRINT(Magmortar)
        OVERWORLD(
            sPicTable_Magmortar,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Magmortar,
            gShinyOverworldPalette_Magmortar
        )
        .levelUpLearnset = sMagmortarLevelUpLearnset,
        .teachableLearnset = sMagmortarTeachableLearnset,
        .frontPic = gMonFrontPic_MagmortarMega,
        .backPic = gMonBackPic_MagmortarMega,
        .frontPic80 = gMonFrontPic80_MagmortarMega,
        .backPic80 = gMonBackPic80_MagmortarMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_MagmortarMega,
        .shinyPalette = gMonShinyPalette_MagmortarMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sMagmortarCustomMegaFormSpeciesIdTable,
        .formChangeTable = sMagmortarCustomMegaFormChangeTable,
    },

    [SPECIES_MEGANIUM_MEGA_M] =
    {
        .baseHP        = 80,
        .baseAttack    = 102,
        .baseDefense   = 140,
        .baseSpeed     = 80,
        .baseSpAttack  = 103,
        .baseSpDefense = 120,
        .types = MON_TYPES(TYPE_GRASS, TYPE_FAIRY),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 263,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 236,
    #else
        .expYield = 208,
    #endif
        .evYield_Defense = 1,
        .evYield_SpDefense = 2,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MONSTER, EGG_GROUP_GRASS),
        .abilities = { ABILITY_TRIAGE, ABILITY_TRIAGE, ABILITY_TRIAGE },
        .bodyColor = BODY_COLOR_GREEN,
        .speciesName = _("Meganium"),
        .cryId = CRY_MEGANIUM,
        .natDexNum = NATIONAL_DEX_MEGANIUM,
        .categoryName = _("Herb"),
        .height = 18,
        .weight = 1005,
        .description = COMPOUND_STRING(
            "The fragrance of a Meganium's flower\n"
            "soothes and calms emotions. In battle,\n"
            "it gives off more of its becalming scent\n"
            "to blunt the foe's fighting spirit."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 277,
        .trainerOffset = 1,
        .iconSprite = gMonIcon_Meganium,
        .iconPalIndex = 1,
#if P_GENDER_DIFFERENCES && !P_GBA_STYLE_SPECIES_GFX
#endif //P_GENDER_DIFFERENCES && !P_GBA_STYLE_SPECIES_GFX
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(0, 13, SHADOW_SIZE_M)
        FOOTPRINT(Meganium)
        OVERWORLD(
            sPicTable_Meganium,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Meganium,
            gShinyOverworldPalette_Meganium
        )
        OVERWORLD_FEMALE(
            sPicTable_MeganiumF,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following
        )
        .levelUpLearnset = sMeganiumLevelUpLearnset,
        .teachableLearnset = sMeganiumTeachableLearnset,
        .formSpeciesIdTable = sMeganiumFormSpeciesIdTable,
        .formChangeTable = sMeganiumCustomMegaFormChangeTable,
        .frontPic = gMonFrontPic_MeganiumMegaM,
        .backPic = gMonBackPic_MeganiumMegaM,
        .frontPic80 = gMonFrontPic80_MeganiumMegaM,
        .backPic80 = gMonBackPic80_MeganiumMegaM,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_MeganiumMegaM,
        .shinyPalette = gMonShinyPalette_MeganiumMegaM,
        .isMegaEvolution = TRUE,
    },

    [SPECIES_MILOTIC_MEGA] =
    {
        .baseHP        = 95,
        .baseAttack    = 60,
        .baseDefense   = 92,
        .baseSpeed     = 113,
        .baseSpAttack  = 150,
        .baseSpDefense = 130,
        .types = MON_TYPES(TYPE_WATER),
        .catchRate = 60,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 189 : 213,
        .evYield_SpDefense = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_ERRATIC,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_1, EGG_GROUP_DRAGON),
    #if P_UPDATED_ABILITIES >= GEN_4
        .abilities = { ABILITY_DAZZLING, ABILITY_DAZZLING, ABILITY_DAZZLING },
    #else
        .abilities = { ABILITY_DAZZLING, ABILITY_DAZZLING, ABILITY_DAZZLING },
    #endif
        .bodyColor = BODY_COLOR_PINK,
        .speciesName = _("Milotic"),
        .cryId = CRY_MILOTIC,
        .natDexNum = NATIONAL_DEX_MILOTIC,
        .categoryName = _("Tender"),
        .height = 62,
        .weight = 1620,
        .description = COMPOUND_STRING(
            "It is said to live at the bottom of\n"
            "large lakes. Considered to be the most\n"
            "beautiful of all Pokémon, it has been\n"
            "depicted in paintings and statues."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 360,
        .trainerOffset = 7,
        .iconSprite = gMonIcon_Milotic,
        .iconPalIndex = P_GBA_STYLE_SPECIES_ICONS ? 0 : 2,
#if P_GENDER_DIFFERENCES && !P_GBA_STYLE_SPECIES_GFX
#endif //P_GENDER_DIFFERENCES && !P_GBA_STYLE_SPECIES_GFX
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(0, 12, SHADOW_SIZE_M)
        FOOTPRINT(Milotic)
        OVERWORLD(
            sPicTable_Milotic,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_SLITHER,
            sAnimTable_Following,
            gOverworldPalette_Milotic,
            gShinyOverworldPalette_Milotic
        )
        OVERWORLD_FEMALE(
            sPicTable_MiloticF,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_SLITHER,
            sAnimTable_Following
        )
        .levelUpLearnset = sMiloticLevelUpLearnset,
        .teachableLearnset = sMiloticTeachableLearnset,
        .frontPic = gMonFrontPic_MiloticMega,
        .backPic = gMonBackPic_MiloticMega,
        .frontPic80 = gMonFrontPic80_MiloticMega,
        .backPic80 = gMonBackPic80_MiloticMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_MiloticMega,
        .shinyPalette = gMonShinyPalette_MiloticMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sMiloticCustomMegaFormSpeciesIdTable,
        .formChangeTable = sMiloticCustomMegaFormChangeTable,
    },

    [SPECIES_OMASTAR_MEGA] =
    {
        .baseHP        = 70,
        .baseAttack    = 80,
        .baseDefense   = 130,
        .baseSpeed     = 45,
        .baseSpAttack  = 145,
        .baseSpDefense = 125,
        .types = MON_TYPES(TYPE_ROCK, TYPE_WATER),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 173 : 199,
        .evYield_Defense = 2,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 30,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_1, EGG_GROUP_WATER_3),
        .abilities = { ABILITY_STORM_DRAIN, ABILITY_STORM_DRAIN, ABILITY_STORM_DRAIN },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Omastar"),
        .cryId = CRY_OMASTAR,
        .natDexNum = NATIONAL_DEX_OMASTAR,
        .categoryName = _("Spiral"),
        .height = 10,
        .weight = 350,
        .description = COMPOUND_STRING(
            "An Omastar uses its tentacles to capture\n"
            "its prey. It is believed to have become\n"
            "extinct because its shell grew too large,\n"
            "making its movements slow and ponderous."),
        .pokemonScale = 307,
        .pokemonOffset = 7,
        .trainerScale = 256,
        .trainerOffset = 0,
        .iconSprite = gMonIcon_Omastar,
        .iconPalIndex = 0,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(0, 4, SHADOW_SIZE_XL_BATTLE_ONLY)
        FOOTPRINT(Omastar)
        OVERWORLD(
            sPicTable_Omastar,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Omastar,
            gShinyOverworldPalette_Omastar
        )
        .levelUpLearnset = sOmastarLevelUpLearnset,
        .teachableLearnset = sOmastarTeachableLearnset,
        .frontPic = gMonFrontPic_OmastarMega,
        .backPic = gMonBackPic_OmastarMega,
        .frontPic80 = gMonFrontPic80_OmastarMega,
        .backPic80 = gMonBackPic80_OmastarMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_OmastarMega,
        .shinyPalette = gMonShinyPalette_OmastarMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sOmastarCustomMegaFormSpeciesIdTable,
        .formChangeTable = sOmastarCustomMegaFormChangeTable,
    },

    [SPECIES_POLIWRATH_MEGA] =
    {
        .baseHP        = 90,
        .baseAttack    = 155,
        .baseDefense   = 125,
        .baseSpeed     = 70,
        .baseSpAttack  = 60,
        .baseSpDefense = 110,
        .types = MON_TYPES(TYPE_WATER, TYPE_FIGHTING),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 255,
    #elif P_UPDATED_EXP_YIELDS >= GEN_6
        .expYield = 230,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 225,
    #else
        .expYield = 185,
    #endif
        .evYield_Defense = 3,
        .itemRare = ITEM_KINGS_ROCK,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_1),
        .abilities = { ABILITY_STAMINA, ABILITY_STAMINA, ABILITY_STAMINA },
        .bodyColor = BODY_COLOR_BLUE,
        .noFlip = TRUE,
        .speciesName = _("Poliwrath"),
        .cryId = CRY_POLIWRATH,
        .natDexNum = NATIONAL_DEX_POLIWRATH,
        .categoryName = _("Tadpole"),
        .height = 13,
        .weight = 540,
        .description = COMPOUND_STRING(
            "Its highly developed muscles never grow\n"
            "fatigued, however much it exercises.\n"
            "This Pokémon can swim back and forth\n"
            "across the Pacific Ocean without effort."),
        .pokemonScale = 256,
        .pokemonOffset = 6,
        .trainerScale = 256,
        .trainerOffset = 0,
        .iconSprite = gMonIcon_Poliwrath,
        .iconPalIndex = 0,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(0, 7, SHADOW_SIZE_L)
        FOOTPRINT(Poliwrath)
        OVERWORLD(
            sPicTable_Poliwrath,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Poliwrath,
            gShinyOverworldPalette_Poliwrath
        )
        .levelUpLearnset = sPoliwrathLevelUpLearnset,
        .teachableLearnset = sPoliwrathTeachableLearnset,
        .frontPic = gMonFrontPic_PoliwrathMega,
        .backPic = gMonBackPic_PoliwrathMega,
        .frontPic80 = gMonFrontPic80_PoliwrathMega,
        .backPic80 = gMonBackPic80_PoliwrathMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_PoliwrathMega,
        .shinyPalette = gMonShinyPalette_PoliwrathMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sPoliwrathCustomMegaFormSpeciesIdTable,
        .formChangeTable = sPoliwrathCustomMegaFormChangeTable,
    },

    [SPECIES_PYROAR_MEGA_M] =
    {
        .baseHP        = 86,
        .baseAttack    = 68,
        .baseDefense   = 87,
        .baseSpeed     = 126,
        .baseSpAttack  = 134,
        .baseSpDefense = 106,
        .types = MON_TYPES(TYPE_FIRE, TYPE_NORMAL),
        .catchRate = 65,
        .expYield = 177,
        .evYield_SpAttack = 2,
        .genderRatio = PERCENT_FEMALE(87.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_FIRE_MANE, ABILITY_FIRE_MANE, ABILITY_FIRE_MANE },
        .bodyColor = BODY_COLOR_BROWN,
        .speciesName = _("Pyroar"),
        .cryId = CRY_PYROAR,
        .natDexNum = NATIONAL_DEX_PYROAR,
        .categoryName = _("Royal"),
        .height = 15,
        .weight = 815,
        .description = COMPOUND_STRING(
            "The male with the largest mane of fire\n"
            "is the leader of the pride. The females\n"
            "protect the pride's cubs. They viciously\n"
            "threaten any challenger."),
        .pokemonScale = 268,
        .pokemonOffset = 2,
        .trainerScale = 271,
        .trainerOffset = 0,
        .iconSprite = gMonIcon_Pyroar,
        .iconPalIndex = 2,
#if P_GENDER_DIFFERENCES
#endif //P_GENDER_DIFFERENCES
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(-2, 11, SHADOW_SIZE_XL_BATTLE_ONLY)
        FOOTPRINT(Pyroar)
        OVERWORLD(
            sPicTable_Pyroar,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Pyroar,
            gShinyOverworldPalette_Pyroar
        )
        OVERWORLD_FEMALE(
            sPicTable_PyroarF,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following
        )
        .levelUpLearnset = sPyroarLevelUpLearnset,
        .teachableLearnset = sPyroarTeachableLearnset,
        .formSpeciesIdTable = sPyroarFormSpeciesIdTable,
        .formChangeTable = sPyroarCustomMegaFormChangeTable,
        .frontPic = gMonFrontPic_PyroarMegaM,
        .backPic = gMonBackPic_PyroarMegaM,
        .frontPic80 = gMonFrontPic80_PyroarMegaM,
        .backPic80 = gMonBackPic80_PyroarMegaM,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_PyroarMegaM,
        .shinyPalette = gMonShinyPalette_PyroarMegaM,
        .isMegaEvolution = TRUE,
    },

    [SPECIES_SHIFTRY_MEGA] =
    {
        .baseHP        = 90,
        .baseAttack    = 140,
        .baseDefense   = 60,
        .baseSpeed     = 130,
        .baseSpAttack  = 100,
        .baseSpDefense = 60,
        .types = MON_TYPES(TYPE_GRASS, TYPE_DARK),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 240,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 216,
    #else
        .expYield = 181,
    #endif
        .evYield_Attack = 3,
        .itemRare = ITEM_POWER_HERB,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 15,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD, EGG_GROUP_GRASS),
    #if P_UPDATED_ABILITIES >= GEN_9
        .abilities = { ABILITY_WIND_RIDER, ABILITY_WIND_RIDER, ABILITY_WIND_RIDER },
    #else
        .abilities = { ABILITY_WIND_RIDER, ABILITY_WIND_RIDER, ABILITY_WIND_RIDER },
    #endif
        .bodyColor = BODY_COLOR_BROWN,
        .speciesName = _("Shiftry"),
        .cryId = CRY_SHIFTRY,
        .natDexNum = NATIONAL_DEX_SHIFTRY,
        .categoryName = _("Wicked"),
        .height = 13,
        .weight = 596,
        .description = COMPOUND_STRING(
            "It is said to arrive on chilly, wintry winds.\n"
            "Feared from long ago as the guardian of\n"
            "forests, this Pokémon lives in a deep\n"
            "forest where people do not venture."),
        .pokemonScale = 290,
        .pokemonOffset = 4,
        .trainerScale = 256,
        .trainerOffset = 0,
        .iconSprite = gMonIcon_Shiftry,
        .iconPalIndex = P_GBA_STYLE_SPECIES_ICONS ? 0 : 5,
#if P_GENDER_DIFFERENCES && !P_GBA_STYLE_SPECIES_GFX
#endif //P_GENDER_DIFFERENCES && !P_GBA_STYLE_SPECIES_GFX
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(-5, 5, SHADOW_SIZE_M)
        FOOTPRINT(Shiftry)
        OVERWORLD(
            sPicTable_Shiftry,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Shiftry,
            gShinyOverworldPalette_Shiftry
        )
        OVERWORLD_FEMALE(
            sPicTable_ShiftryF,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following
        )
        .levelUpLearnset = sShiftryLevelUpLearnset,
        .teachableLearnset = sShiftryTeachableLearnset,
        .frontPic = gMonFrontPic_ShiftryMega,
        .backPic = gMonBackPic_ShiftryMega,
        .frontPic80 = gMonFrontPic80_ShiftryMega,
        .backPic80 = gMonBackPic80_ShiftryMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_ShiftryMega,
        .shinyPalette = gMonShinyPalette_ShiftryMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sShiftryCustomMegaFormSpeciesIdTable,
        .formChangeTable = sShiftryCustomMegaFormChangeTable,
    },

    [SPECIES_SNORLAX_MEGA] =
    {
        .baseHP        = 160,
        .baseAttack    = 145,
        .baseDefense   = 85,
        .baseSpeed     = 30,
        .baseSpAttack  = 85,
        .baseSpDefense = 135,
        .types = MON_TYPES(TYPE_NORMAL),
        .catchRate = 25,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 189 : 154,
        .evYield_HP = 2,
        .itemCommon = ITEM_LEFTOVERS,
        .itemRare = ITEM_LEFTOVERS,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 40,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MONSTER),
        .abilities = { ABILITY_FLUFFY, ABILITY_FLUFFY, ABILITY_FLUFFY },
        .bodyColor = BODY_COLOR_BLACK,
        .speciesName = _("Snorlax"),
        .cryId = CRY_SNORLAX,
        .natDexNum = NATIONAL_DEX_SNORLAX,
        .categoryName = _("Sleeping"),
        .height = 21,
        .weight = 4600,
        .description = COMPOUND_STRING(
            "Snorlax's typical day consists of nothing\n"
            "more than eating and sleeping. It is such\n"
            "a docile Pokémon that there are children\n"
            "who use its big belly as a place to play."),
        .pokemonScale = 256,
        .pokemonOffset = 4,
        .trainerScale = 423,
        .trainerOffset = 11,
        .iconSprite = gMonIcon_Snorlax,
        .iconPalIndex = P_GBA_STYLE_SPECIES_ICONS ? 1 : 3,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(0, 9, SHADOW_SIZE_XL_BATTLE_ONLY)
        FOOTPRINT(Snorlax)
        OVERWORLD(
            sPicTable_Snorlax,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Snorlax,
            gShinyOverworldPalette_Snorlax
        )
        .levelUpLearnset = sSnorlaxLevelUpLearnset,
        .teachableLearnset = sSnorlaxTeachableLearnset,
        .formSpeciesIdTable = sSnorlaxFormSpeciesIdTable,
        .formChangeTable = sSnorlaxCustomMegaFormChangeTable,
        .frontPic = gMonFrontPic_SnorlaxMega,
        .backPic = gMonBackPic_SnorlaxMega,
        .frontPic80 = gMonFrontPic80_SnorlaxMega,
        .backPic80 = gMonBackPic80_SnorlaxMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_SnorlaxMega,
        .shinyPalette = gMonShinyPalette_SnorlaxMega,
        .isMegaEvolution = TRUE,
    },

    [SPECIES_TORKOAL_MEGA] =
    {
        .baseHP        = 70,
        .baseAttack    = 110,
        .baseDefense   = 160,
        .baseSpeed     = 20,
        .baseSpAttack  = 120,
        .baseSpDefense = 90,
        .types = MON_TYPES(TYPE_FIRE, TYPE_STEEL),
        .catchRate = 90,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 165 : 161,
        .evYield_Defense = 2,
        .itemRare = ITEM_CHARCOAL,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
    #if P_UPDATED_ABILITIES >= GEN_7
        .abilities = { ABILITY_DROUGHT, ABILITY_DROUGHT, ABILITY_DROUGHT },
    #else
        .abilities = { ABILITY_DROUGHT, ABILITY_DROUGHT, ABILITY_DROUGHT },
    #endif
        .bodyColor = BODY_COLOR_BROWN,
        .speciesName = _("Torkoal"),
        .cryId = CRY_TORKOAL,
        .natDexNum = NATIONAL_DEX_TORKOAL,
        .categoryName = _("Coal"),
        .height = 5,
        .weight = 804,
        .description = COMPOUND_STRING(
            "It battles using energy it gets from\n"
            "burning coal. When loosing smoke from its\n"
            "nostrils, it lets off a sound that is\n"
            "similar to a locomotive's horn."),
        .pokemonScale = 390,
        .pokemonOffset = 9,
        .trainerScale = 256,
        .trainerOffset = 0,
        .iconSprite = gMonIcon_Torkoal,
        .iconPalIndex = P_GBA_STYLE_SPECIES_ICONS ? 1 : 2,
        .pokemonJumpType = PKMN_JUMP_TYPE_SLOW,
        SHADOW(2, 7, SHADOW_SIZE_L)
        FOOTPRINT(Torkoal)
        OVERWORLD(
            sPicTable_Torkoal,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Torkoal,
            gShinyOverworldPalette_Torkoal
        )
        .levelUpLearnset = sTorkoalLevelUpLearnset,
        .teachableLearnset = sTorkoalTeachableLearnset,
        .frontPic = gMonFrontPic_TorkoalMega,
        .backPic = gMonBackPic_TorkoalMega,
        .frontPic80 = gMonFrontPic80_TorkoalMega,
        .backPic80 = gMonBackPic80_TorkoalMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_TorkoalMega,
        .shinyPalette = gMonShinyPalette_TorkoalMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sTorkoalCustomMegaFormSpeciesIdTable,
        .formChangeTable = sTorkoalCustomMegaFormChangeTable,
    },

    [SPECIES_TYPHLOSION_MEGA] =
    {
        .baseHP        = 78,
        .baseAttack    = 89,
        .baseDefense   = 108,
        .baseSpeed     = 105,
        .baseSpAttack  = 144,
        .baseSpDefense = 110,
        .types = MON_TYPES(TYPE_FIRE),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 267,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 240,
    #else
        .expYield = 209,
    #endif
        .evYield_SpAttack = 3,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_SOLAR_POWER, ABILITY_SOLAR_POWER, ABILITY_SOLAR_POWER },
        .bodyColor = BODY_COLOR_YELLOW,
        .speciesName = _("Typhlosion"),
        .cryId = CRY_TYPHLOSION,
        .natDexNum = NATIONAL_DEX_TYPHLOSION,
        .categoryName = _("Volcano"),
        .height = 17,
        .weight = 795,
        .description = COMPOUND_STRING(
            "It can hide behind a shimmering heat haze\n"
            "that it creates using its intense flames.\n"
            "Typhlosion create blazing explosive\n"
            "blasts that burn everything to cinders."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 268,
        .trainerOffset = 1,
        .iconSprite = gMonIcon_Typhlosion,
        .iconPalIndex = P_GBA_STYLE_SPECIES_ICONS ? 1 : 3,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(4, 14, SHADOW_SIZE_L)
        FOOTPRINT(Typhlosion)
        OVERWORLD(
            sPicTable_Typhlosion,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Typhlosion,
            gShinyOverworldPalette_Typhlosion
        )
        .levelUpLearnset = sTyphlosionLevelUpLearnset,
        .teachableLearnset = sTyphlosionTeachableLearnset,
        .formSpeciesIdTable = sTyphlosionFormSpeciesIdTable,
        .frontPic = gMonFrontPic_TyphlosionMega,
        .backPic = gMonBackPic_TyphlosionMega,
        .frontPic80 = gMonFrontPic80_TyphlosionMega,
        .backPic80 = gMonBackPic80_TyphlosionMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_TyphlosionMega,
        .shinyPalette = gMonShinyPalette_TyphlosionMega,
        .isMegaEvolution = TRUE,
        .formChangeTable = sTyphlosionCustomMegaFormChangeTable,
    },

    [SPECIES_WHIMSICOTT_MEGA] =
    {
        .baseHP        = 60,
        .baseAttack    = 67,
        .baseDefense   = 110,
        .baseSpeed     = 138,
        .baseSpAttack  = 105,
        .baseSpDefense = 100,
        .types = MON_TYPES(TYPE_GRASS, TYPE_FAIRY),
        .catchRate = 75,
        .expYield = 168,
        .evYield_Speed = 2,
        .itemRare = ITEM_ABSORB_BULB,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_GRASS, EGG_GROUP_FAIRY),
        .abilities = { ABILITY_HOSPITALITY, ABILITY_HOSPITALITY, ABILITY_HOSPITALITY },
        .bodyColor = BODY_COLOR_GREEN,
        .speciesName = _("Whimsicott"),
        .cryId = CRY_WHIMSICOTT,
        .natDexNum = NATIONAL_DEX_WHIMSICOTT,
        .categoryName = _("Windveiled"),
        .height = 7,
        .weight = 66,
        .description = COMPOUND_STRING(
            "It rides on the wind and slips into\n"
            "people's homes. After it has turned a\n"
            "room into a cotton-filled mess, it giggles\n"
            "to itself and takes off."),
        .pokemonScale = 365,
        .pokemonOffset = 12,
        .trainerScale = 256,
        .trainerOffset = 0,
        .iconSprite = gMonIcon_Whimsicott,
        .iconPalIndex = 1,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(-1, 8, SHADOW_SIZE_M)
        FOOTPRINT(Whimsicott)
        OVERWORLD(
            sPicTable_Whimsicott,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Whimsicott,
            gShinyOverworldPalette_Whimsicott
        )
        .levelUpLearnset = sWhimsicottLevelUpLearnset,
        .teachableLearnset = sWhimsicottTeachableLearnset,
        .frontPic = gMonFrontPic_WhimsicottMega,
        .backPic = gMonBackPic_WhimsicottMega,
        .frontPic80 = gMonFrontPic80_WhimsicottMega,
        .backPic80 = gMonBackPic80_WhimsicottMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_WhimsicottMega,
        .shinyPalette = gMonShinyPalette_WhimsicottMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sWhimsicottCustomMegaFormSpeciesIdTable,
        .formChangeTable = sWhimsicottCustomMegaFormChangeTable,
    },

    [SPECIES_NIDOQUEEN_MEGA] =
    {
        .baseHP = 90,
        .baseAttack = 112,
        .baseDefense = 127,
        .baseSpeed = 76,
        .baseSpAttack = 95,
        .baseSpDefense = 105,
        .types = MON_TYPES(TYPE_POISON, TYPE_GROUND),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 253,
    #elif P_UPDATED_EXP_YIELDS >= GEN_6
        .expYield = 227,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 223,
    #else
        .expYield = 194,
    #endif
        .evYield_HP = 3,
        .genderRatio = MON_FEMALE,
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_NO_EGGS_DISCOVERED),
    #if P_UPDATED_ABILITIES >= GEN_4
        .abilities = { ABILITY_SHEER_FORCE, ABILITY_SHEER_FORCE, ABILITY_SHEER_FORCE },
    #else
        .abilities = { ABILITY_SHEER_FORCE, ABILITY_SHEER_FORCE, ABILITY_SHEER_FORCE },
    #endif
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Nidoqueen"),
        .cryId = CRY_NIDOQUEEN,
        .natDexNum = NATIONAL_DEX_NIDOQUEEN,
        .categoryName = _("Drill"),
        .height = 13,
        .weight = 600,
        .description = COMPOUND_STRING(
            "It is adept at sending foes flying with\n"
            "harsh tackles using its tough, scaly body.\n"
            "This Pokémon is at its strongest when\n"
            "it is defending its young."),
        .pokemonScale = 293,
        .pokemonOffset = 4,
        .trainerScale = 256,
        .trainerOffset = 0,

        .iconSprite = gMonIcon_Nidoqueen,
        .iconPalIndex = 2,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(1, 9, SHADOW_SIZE_XL_BATTLE_ONLY)
        FOOTPRINT(Nidoqueen)
        OVERWORLD(
            sPicTable_Nidoqueen,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Nidoqueen,
            gShinyOverworldPalette_Nidoqueen
        )
        .levelUpLearnset = sNidoqueenLevelUpLearnset,
        .teachableLearnset = sNidoqueenTeachableLearnset,
        .frontPic = gMonFrontPic_NidoqueenMega,
        .backPic = gMonBackPic_NidoqueenMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 4,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_NidoqueenMega,
        .shinyPalette = gMonShinyPalette_NidoqueenMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sNidoqueenAddedMegaFormSpeciesIdTable,
        .formChangeTable = sNidoqueenAddedMegaFormChangeTable,
    },

    [SPECIES_NIDOKING_MEGA] =
    {
        .baseHP = 81,
        .baseAttack = 132,
        .baseDefense = 97,
        .baseSpeed = 105,
        .baseSpAttack = 115,
        .baseSpDefense = 75,
        .types = MON_TYPES(TYPE_POISON, TYPE_GROUND),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 253,
    #elif P_UPDATED_EXP_YIELDS >= GEN_6
        .expYield = 227,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 223,
    #else
        .expYield = 195,
    #endif
        .evYield_Attack = 3,
        .genderRatio = MON_MALE,
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MONSTER, EGG_GROUP_FIELD),
    #if P_UPDATED_ABILITIES >= GEN_4
        .abilities = { ABILITY_SHEER_FORCE, ABILITY_SHEER_FORCE, ABILITY_SHEER_FORCE },
    #else
        .abilities = { ABILITY_SHEER_FORCE, ABILITY_SHEER_FORCE, ABILITY_SHEER_FORCE },
    #endif
        .bodyColor = BODY_COLOR_PURPLE,
        .speciesName = _("Nidoking"),
        .cryId = CRY_NIDOKING,
        .natDexNum = NATIONAL_DEX_NIDOKING,
        .categoryName = _("Drill"),
        .height = 14,
        .weight = 620,
        .description = COMPOUND_STRING(
            "A Nidoking's thick tail packs enormously\n"
            "destructive power capable of toppling\n"
            "a metal transmission tower. Once it goes\n"
            "on a rampage, there is no stopping it."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = 0,

        .frontAnimDelay = 25,

        .iconSprite = gMonIcon_Nidoking,
        .iconPalIndex = 2,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(0, 8, SHADOW_SIZE_XL_BATTLE_ONLY)
        FOOTPRINT(Nidoking)
        OVERWORLD(
            sPicTable_Nidoking,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Nidoking,
            gShinyOverworldPalette_Nidoking
        )
        .levelUpLearnset = sNidokingLevelUpLearnset,
        .teachableLearnset = sNidokingTeachableLearnset,
        .frontPic = gMonFrontPic_NidokingMega,
        .backPic = gMonBackPic_NidokingMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 4,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_NidokingMega,
        .shinyPalette = gMonShinyPalette_NidokingMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sNidokingAddedMegaFormSpeciesIdTable,
        .formChangeTable = sNidokingAddedMegaFormChangeTable,
    },

    [SPECIES_TROPIUS_MEGA] =
    {
        .baseHP = 99,
        .baseAttack = 88,
        .baseDefense = 113,
        .baseSpeed = 61,
        .baseSpAttack = 122,
        .baseSpDefense = 77,
        .types = MON_TYPES(TYPE_GRASS, TYPE_FLYING),
        .catchRate = 200,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 161 : 169,
        .evYield_HP = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 25,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MONSTER, EGG_GROUP_GRASS),
    #if P_UPDATED_ABILITIES >= GEN_4
        .abilities = { ABILITY_THICK_FAT, ABILITY_THICK_FAT, ABILITY_THICK_FAT },
    #else
        .abilities = { ABILITY_THICK_FAT, ABILITY_THICK_FAT, ABILITY_THICK_FAT },
    #endif
        .bodyColor = BODY_COLOR_GREEN,
        .speciesName = _("Tropius"),
        .cryId = CRY_TROPIUS,
        .natDexNum = NATIONAL_DEX_TROPIUS,
        .categoryName = _("Fruit"),
        .height = 20,
        .weight = 1000,
        .description = COMPOUND_STRING(
            "It flies by flapping its broad leaves.\n"
            "The bunch of fruit that grows around its\n"
            "neck is deliciously sweet. In the spring,\n"
            "it scatters pollen from its neck."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 344,
        .trainerOffset = 7,

        .iconSprite = gMonIcon_Tropius,
        .iconPalIndex = 1,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(-6, 13, SHADOW_SIZE_L)
        FOOTPRINT(Tropius)
        OVERWORLD(
            sPicTable_Tropius,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Tropius,
            gShinyOverworldPalette_Tropius
        )
        .levelUpLearnset = sTropiusLevelUpLearnset,
        .teachableLearnset = sTropiusTeachableLearnset,
        .eggMoveLearnset = sTropiusEggMoveLearnset,
        .frontPic = gMonFrontPic_TropiusMega,
        .backPic = gMonBackPic_TropiusMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 4,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_TropiusMega,
        .shinyPalette = gMonShinyPalette_TropiusMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sTropiusAddedMegaFormSpeciesIdTable,
        .formChangeTable = sTropiusAddedMegaFormChangeTable,
    },

    [SPECIES_RILLABOOM_MEGA] =
    {
        .baseHP = 100,
        .baseAttack = 155,
        .baseDefense = 120,
        .baseSpeed = 95,
        .baseSpAttack = 60,
        .baseSpDefense = 100,
        .types = MON_TYPES(TYPE_GRASS, TYPE_ROCK),
        .catchRate = 45,
        .expYield = 265,
        .evYield_Attack = 3,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD, EGG_GROUP_GRASS),
        .abilities = { ABILITY_GRASSY_SURGE, ABILITY_GRASSY_SURGE, ABILITY_GRASSY_SURGE },
        .bodyColor = BODY_COLOR_GREEN,
        .speciesName = _("Rillaboom"),
        .cryId = CRY_RILLABOOM,
        .natDexNum = NATIONAL_DEX_RILLABOOM,
        .categoryName = _("Drummer"),
        .height = 21,
        .weight = 900,
        .description = COMPOUND_STRING(
            "By drumming, it taps into the power of\n"
            "its special tree stump. The roots of the\n"
            "stump follow its direction in battle."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 365,
        .trainerOffset = 7,

        .iconSprite = gMonIcon_Rillaboom,
        .iconPalIndex = 1,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(2, 8, SHADOW_SIZE_L)
        FOOTPRINT(Rillaboom)
        OVERWORLD(
            sPicTable_Rillaboom,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Rillaboom,
            gShinyOverworldPalette_Rillaboom
        )
        .levelUpLearnset = sRillaboomLevelUpLearnset,
        .teachableLearnset = sRillaboomTeachableLearnset,

        .frontPic = gMonFrontPic_RillaboomMega,
        .backPic = gMonBackPic_RillaboomMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 4,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_RillaboomMega,
        .shinyPalette = gMonShinyPalette_RillaboomMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sRillaboomFormSpeciesIdTable,
        .formChangeTable = sRillaboomFormChangeTable,
    },

    [SPECIES_SAMUROTT_HISUI_MEGA] =
    {
        .baseHP = 90,
        .baseAttack = 148,
        .baseDefense = 100,
        .baseSpeed = 115,
        .baseSpAttack = 100,
        .baseSpDefense = 75,
        .types = MON_TYPES(TYPE_WATER, TYPE_DARK),
        .catchRate = 45,
        .expYield = 264,
        .evYield_Attack = 3,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_SHARPNESS, ABILITY_SHARPNESS, ABILITY_SHARPNESS },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Samurott"),
        .cryId = CRY_SAMUROTT,
        .natDexNum = NATIONAL_DEX_SAMUROTT,
        .categoryName = _("Formidable"),
        .height = 15,
        .weight = 582,
        .description = COMPOUND_STRING(
            "Hard of heart and deft of blade,\n"
            "this rare form of Samurott is a\n"
            "product of the Pokémon's evolution in the\n"
            "region of Hisui."),
        .pokemonScale = 268,
        .pokemonOffset = 2,
        .trainerScale = 271,
        .trainerOffset = 0,

        .iconSprite = gMonIcon_SamurottHisui,
        .iconPalIndex = 0,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(-2, 13, SHADOW_SIZE_L)
        FOOTPRINT(Samurott)
        OVERWORLD(
            sPicTable_SamurottHisui,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_SamurottHisui,
            gShinyOverworldPalette_SamurottHisui
        )
        .isHisuianForm = TRUE,
        .levelUpLearnset = sSamurottHisuiLevelUpLearnset,
        .teachableLearnset = sSamurottHisuiTeachableLearnset,

        .frontPic = gMonFrontPic_SamurottHisuiMega,
        .backPic = gMonBackPic_SamurottHisuiMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 4,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_SamurottHisuiMega,
        .shinyPalette = gMonShinyPalette_SamurottHisuiMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sSamurottFormSpeciesIdTable,
        .formChangeTable = sSamurottHisuiAddedMegaFormChangeTable,
    },

    [SPECIES_SKELEDIRGE_MEGA] =
    {
        .baseHP = 104,
        .baseAttack = 75,
        .baseDefense = 130,
        .baseSpeed = 66,
        .baseSpAttack = 150,
        .baseSpDefense = 105,
        .types = MON_TYPES(TYPE_FIRE, TYPE_GHOST),
        .catchRate = 45,
        .expYield = 265,
        .evYield_SpAttack = 3,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_PUNK_ROCK, ABILITY_PUNK_ROCK, ABILITY_PUNK_ROCK },
        .bodyColor = BODY_COLOR_RED,
        .speciesName = _("Skeledirge"),
        .cryId = CRY_SKELEDIRGE,
        .natDexNum = NATIONAL_DEX_SKELEDIRGE,
        .categoryName = _("Singer"),
        .height = 16,
        .weight = 3265,
        .description = COMPOUND_STRING(
            "Skeledirge's gentle singing\n"
            "soothes the souls of all that hear it. It\n"
            "burns its enemies to a crisp with\n"
            "flames of over 5,400 degrees Fahrenheit."),
        .pokemonScale = 356,
        .pokemonOffset = 17,
        .trainerScale = 256,
        .trainerOffset = 0,

        .iconSprite = gMonIcon_Skeledirge,
        .iconPalIndex = 0,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(6, 7, SHADOW_SIZE_XL_BATTLE_ONLY)
        FOOTPRINT(Skeledirge)
        OVERWORLD(
            sPicTable_Skeledirge,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Skeledirge,
            gShinyOverworldPalette_Skeledirge
        )
        .levelUpLearnset = sSkeledirgeLevelUpLearnset,
        .teachableLearnset = sSkeledirgeTeachableLearnset,
        .frontPic = gMonFrontPic_SkeledirgeMega,
        .backPic = gMonBackPic_SkeledirgeMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 4,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_SkeledirgeMega,
        .shinyPalette = gMonShinyPalette_SkeledirgeMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sSkeledirgeAddedMegaFormSpeciesIdTable,
        .formChangeTable = sSkeledirgeAddedMegaFormChangeTable,
    },

    [SPECIES_DUSKNOIR_MEGA] =
    {
        .baseHP = 45,
        .baseAttack = 140,
        .baseDefense = 165,
        .baseSpeed = 45,
        .baseSpAttack = 65,
        .baseSpDefense = 165,
        .types = MON_TYPES(TYPE_GHOST),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 263,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 236,
    #else
        .expYield = 210,
    #endif
        .evYield_Defense = 1,
        .evYield_SpDefense = 2,
        .itemRare = ITEM_SPELL_TAG,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 25,
        .friendship = 35,
        .growthRate = GROWTH_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_AMORPHOUS),
    #if P_UPDATED_ABILITIES >= GEN_6
        .abilities = { ABILITY_LEVITATE, ABILITY_LEVITATE, ABILITY_LEVITATE },
    #else
        .abilities = { ABILITY_LEVITATE, ABILITY_LEVITATE, ABILITY_LEVITATE },
    #endif
        .bodyColor = BODY_COLOR_BLACK,
        .speciesName = _("Dusknoir"),
        .cryId = CRY_DUSKNOIR,
        .natDexNum = NATIONAL_DEX_DUSKNOIR,
        .categoryName = _("Gripper"),
        .height = 22,
        .weight = 1066,
        .description = COMPOUND_STRING(
            "This feared Pokémon is said to travel to\n"
            "worlds unknown. Some even believe that\n"
            "it takes lost spirits into its pliant body\n"
            "and guides them home."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 348,
        .trainerOffset = 6,

        .enemyMonElevation = 6,

        .iconSprite = gMonIcon_Dusknoir,
        .iconPalIndex = 2,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(6, 13, SHADOW_SIZE_M)
        FOOTPRINT(Dusknoir)
        OVERWORLD(
            sPicTable_Dusknoir,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Dusknoir,
            gShinyOverworldPalette_Dusknoir
        )
        .levelUpLearnset = sDusknoirLevelUpLearnset,
        .teachableLearnset = sDusknoirTeachableLearnset,
        .frontPic = gMonFrontPic_DusknoirMega,
        .backPic = gMonBackPic_DusknoirMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 4,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_DusknoirMega,
        .shinyPalette = gMonShinyPalette_DusknoirMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sDusknoirAddedMegaFormSpeciesIdTable,
        .formChangeTable = sDusknoirAddedMegaFormChangeTable,
    },

    [SPECIES_EMPOLEON_MEGA_X] =
    {
        .baseHP = 84,
        .baseAttack = 86,
        .baseDefense = 108,
        .baseSpeed = 80,
        .baseSpAttack = 151,
        .baseSpDefense = 121,
        .types = MON_TYPES(TYPE_WATER, TYPE_STEEL),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 265,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 239,
    #else
        .expYield = 210,
    #endif
        .evYield_SpAttack = 3,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_1, EGG_GROUP_FIELD),
    #if P_UPDATED_ABILITIES >= GEN_9
        .abilities = { ABILITY_COMPETITIVE, ABILITY_COMPETITIVE, ABILITY_COMPETITIVE },
    #else
        .abilities = { ABILITY_COMPETITIVE, ABILITY_COMPETITIVE, ABILITY_COMPETITIVE },
    #endif
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Empoleon"),
        .cryId = CRY_EMPOLEON,
        .natDexNum = NATIONAL_DEX_EMPOLEON,
        .categoryName = _("Emperor"),
        .height = 17,
        .weight = 845,
        .description = COMPOUND_STRING(
            "The three horns that extend from its\n"
            "beak attest to its power. It avoids\n"
            "unnecessary disputes, but it will decimate\n"
            "anything that threatens its pride."),
        .pokemonScale = 259,
        .pokemonOffset = 0,
        .trainerScale = 290,
        .trainerOffset = 1,

        .iconSprite = gMonIcon_Empoleon,
        .iconPalIndex = 0,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(2, 12, SHADOW_SIZE_M)
        FOOTPRINT(Empoleon)
        OVERWORLD(
            sPicTable_Empoleon,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Empoleon,
            gShinyOverworldPalette_Empoleon
        )
        .levelUpLearnset = sEmpoleonLevelUpLearnset,
        .teachableLearnset = sEmpoleonTeachableLearnset,
        .frontPic = gMonFrontPic_EmpoleonMegaX,
        .backPic = gMonBackPic_EmpoleonMegaX,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 4,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_EmpoleonMegaX,
        .shinyPalette = gMonShinyPalette_EmpoleonMegaX,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sEmpoleonAddedMegaFormSpeciesIdTable,
        .formChangeTable = sEmpoleonAddedMegaFormChangeTable,
    },

    [SPECIES_EMPOLEON_MEGA_Y] =
    {
        .baseHP = 84,
        .baseAttack = 126,
        .baseDefense = 128,
        .baseSpeed = 60,
        .baseSpAttack = 111,
        .baseSpDefense = 121,
        .types = MON_TYPES(TYPE_WATER, TYPE_STEEL),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 265,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 239,
    #else
        .expYield = 210,
    #endif
        .evYield_SpAttack = 3,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_1, EGG_GROUP_FIELD),
    #if P_UPDATED_ABILITIES >= GEN_9
        .abilities = { ABILITY_DEFIANT, ABILITY_DEFIANT, ABILITY_DEFIANT },
    #else
        .abilities = { ABILITY_DEFIANT, ABILITY_DEFIANT, ABILITY_DEFIANT },
    #endif
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Empoleon"),
        .cryId = CRY_EMPOLEON,
        .natDexNum = NATIONAL_DEX_EMPOLEON,
        .categoryName = _("Emperor"),
        .height = 17,
        .weight = 845,
        .description = COMPOUND_STRING(
            "The three horns that extend from its\n"
            "beak attest to its power. It avoids\n"
            "unnecessary disputes, but it will decimate\n"
            "anything that threatens its pride."),
        .pokemonScale = 259,
        .pokemonOffset = 0,
        .trainerScale = 290,
        .trainerOffset = 1,

        .iconSprite = gMonIcon_Empoleon,
        .iconPalIndex = 0,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(2, 12, SHADOW_SIZE_M)
        FOOTPRINT(Empoleon)
        OVERWORLD(
            sPicTable_Empoleon,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Empoleon,
            gShinyOverworldPalette_Empoleon
        )
        .levelUpLearnset = sEmpoleonLevelUpLearnset,
        .teachableLearnset = sEmpoleonTeachableLearnset,

        .frontPic = gMonFrontPic_EmpoleonMegaY,
        .backPic = gMonBackPic_EmpoleonMegaY,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 4,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_EmpoleonMegaY,
        .shinyPalette = gMonShinyPalette_EmpoleonMegaY,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sEmpoleonAddedMegaFormSpeciesIdTable,
        .formChangeTable = sEmpoleonAddedMegaFormChangeTable,
    },

    [SPECIES_INFERNAPE_MEGA] =
    {
        .baseHP = 76,
        .baseAttack = 134,
        .baseDefense = 81,
        .baseSpeed = 138,
        .baseSpAttack = 124,
        .baseSpDefense = 81,
        .types = MON_TYPES(TYPE_FIRE, TYPE_FIGHTING),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 267,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 240,
    #else
        .expYield = 209,
    #endif
        .evYield_Attack = 1,
        .evYield_Speed = 1,
        .evYield_SpAttack = 1,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD, EGG_GROUP_HUMAN_LIKE),
        .abilities = { ABILITY_IRON_FIST, ABILITY_IRON_FIST, ABILITY_IRON_FIST },
        .bodyColor = BODY_COLOR_BROWN,
        .speciesName = _("Infernape"),
        .cryId = CRY_INFERNAPE,
        .natDexNum = NATIONAL_DEX_INFERNAPE,
        .categoryName = _("Flame"),
        .height = 12,
        .weight = 550,
        .description = COMPOUND_STRING(
            "Its crown of fire is indicative of its\n"
            "fiery nature. It uses a special kind of\n"
            "martial arts involving all of its limbs to\n"
            "take on any opponent."),
        .pokemonScale = 282,
        .pokemonOffset = 3,
        .trainerScale = 256,
        .trainerOffset = 0,

        .iconSprite = gMonIcon_Infernape,
        .iconPalIndex = 0,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(0, 9, SHADOW_SIZE_L)
        FOOTPRINT(Infernape)
        OVERWORLD(
            sPicTable_Infernape,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Infernape,
            gShinyOverworldPalette_Infernape
        )
        .levelUpLearnset = sInfernapeLevelUpLearnset,
        .teachableLearnset = sInfernapeTeachableLearnset,
        .frontPic = gMonFrontPic_InfernapeMega,
        .backPic = gMonBackPic_InfernapeMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 4,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_InfernapeMega,
        .shinyPalette = gMonShinyPalette_InfernapeMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sInfernapeAddedMegaFormSpeciesIdTable,
        .formChangeTable = sInfernapeAddedMegaFormChangeTable,
    },

    [SPECIES_LUXRAY_MEGA] =
    {
        .baseHP = 90,
        .baseAttack = 175,
        .baseDefense = 110,
        .baseSpeed = 140,
        .baseSpAttack = 75,
        .baseSpDefense = 110,
        .types = MON_TYPES(TYPE_ELECTRIC, TYPE_DARK),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 262,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 235,
    #else
        .expYield = 194,
    #endif
        .evYield_Attack = 3,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_STRONG_JAW, ABILITY_STRONG_JAW, ABILITY_STRONG_JAW },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Luxray"),
        .cryId = CRY_LUXRAY,
        .natDexNum = NATIONAL_DEX_LUXRAY,
        .categoryName = _("Gleam Eyes"),
        .height = 14,
        .weight = 420,
        .description = COMPOUND_STRING(
            "It has eyes which can see through\n"
            "anything. Luxray's ability to see\n"
            "through objects comes in handy when\n"
            "it's scouting for danger."),
        .pokemonScale = 265,
        .pokemonOffset = 2,
        .trainerScale = 262,
        .trainerOffset = 0,

        .iconSprite = gMonIcon_Luxray,
        .iconPalIndex = 0,
#if P_GENDER_DIFFERENCES
        .frontPicFemale = gMonFrontPic_LuxrayF,
        .frontPicSizeFemale = MON_COORDS_SIZE(64, 64),
        .backPicFemale = gMonBackPic_LuxrayF,
        .backPicSizeFemale = MON_COORDS_SIZE(64, 64),
#endif //P_GENDER_DIFFERENCES
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(-1, 10, SHADOW_SIZE_L)
        FOOTPRINT(Luxray)
        OVERWORLD(
            sPicTable_Luxray,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Luxray,
            gShinyOverworldPalette_Luxray
        )
        OVERWORLD_FEMALE(
            sPicTable_LuxrayF,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following
        )
        .levelUpLearnset = sLuxrayLevelUpLearnset,
        .teachableLearnset = sLuxrayTeachableLearnset,
        .frontPic = gMonFrontPic_LuxrayMega,
        .backPic = gMonBackPic_LuxrayMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 4,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_LuxrayMega,
        .shinyPalette = gMonShinyPalette_LuxrayMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sLuxrayAddedMegaFormSpeciesIdTable,
        .formChangeTable = sLuxrayAddedMegaFormChangeTable,
    },

    [SPECIES_ROSERADE_MEGA] =
    {
        .baseHP = 60,
        .baseAttack = 70,
        .baseDefense = 85,
        .baseSpeed = 120,
        .baseSpAttack = 155,
        .baseSpDefense = 125,
        .types = MON_TYPES(TYPE_GRASS, TYPE_POISON),
        .catchRate = 75,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 258,
    #elif P_UPDATED_EXP_YIELDS >= GEN_6
        .expYield = 232,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 227,
    #else
        .expYield = 204,
    #endif
        .evYield_SpAttack = 3,
        .itemRare = ITEM_POISON_BARB,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FAIRY, EGG_GROUP_GRASS),
        .abilities = { ABILITY_TECHNICIAN, ABILITY_TECHNICIAN, ABILITY_TECHNICIAN },
        .bodyColor = BODY_COLOR_GREEN,
        .noFlip = TRUE,
        .speciesName = _("Roserade"),
        .cryId = CRY_ROSERADE,
        .natDexNum = NATIONAL_DEX_ROSERADE,
        .categoryName = _("Bouquet"),
        .height = 9,
        .weight = 145,
        .description = COMPOUND_STRING(
            "Its sweet aroma attracts prey.\n"
            "With the movements of a dancer,\n"
            "it strikes with whips that are densely\n"
            "lined with poison thorns."),
        .pokemonScale = 338,
        .pokemonOffset = 10,
        .trainerScale = 256,
        .trainerOffset = 0,

        .iconSprite = gMonIcon_Roserade,
        .iconPalIndex = 0,
#if P_GENDER_DIFFERENCES
        .frontPicFemale = gMonFrontPic_RoseradeF,
        .frontPicSizeFemale = MON_COORDS_SIZE(48, 56),
        .backPicFemale = gMonBackPic_RoseradeF,
        .backPicSizeFemale = MON_COORDS_SIZE(64, 56),
#endif //P_GENDER_DIFFERENCES
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(0, 6, SHADOW_SIZE_S)
        FOOTPRINT(Roserade)
        OVERWORLD(
            sPicTable_Roserade,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following_Asym,
            gOverworldPalette_Roserade,
            gShinyOverworldPalette_Roserade
        )
        OVERWORLD_FEMALE(
            sPicTable_RoseradeF,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following_Asym
        )
        .levelUpLearnset = sRoseradeLevelUpLearnset,
        .teachableLearnset = sRoseradeTeachableLearnset,
        .frontPic = gMonFrontPic_RoseradeMega,
        .backPic = gMonBackPic_RoseradeMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 4,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_RoseradeMega,
        .shinyPalette = gMonShinyPalette_RoseradeMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sRoseradeAddedMegaFormSpeciesIdTable,
        .formChangeTable = sRoseradeAddedMegaFormChangeTable,
    },

    [SPECIES_SLAKING_MEGA] =
    {
        .baseHP = 150,
        .baseAttack = 180,
        .baseDefense = 140,
        .baseSpeed = 100,
        .baseSpAttack = 105,
        .baseSpDefense = 95,
        .types = MON_TYPES(TYPE_NORMAL),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 285,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 252,
    #else
        .expYield = 210,
    #endif
        .evYield_HP = 3,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 15,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_SLOW_START, ABILITY_SLOW_START, ABILITY_SLOW_START },
        .bodyColor = BODY_COLOR_BROWN,
        .speciesName = _("Slaking"),
        .cryId = CRY_SLAKING,
        .natDexNum = NATIONAL_DEX_SLAKING,
        .categoryName = _("Lazy"),
        .height = 20,
        .weight = 1305,
        .description = COMPOUND_STRING(
            "Hordes of Slaking gather around trees\n"
            "when fruits come into season. They wait\n"
            "around patiently for ripened fruits to fall\n"
            "out of the trees."),
        .pokemonScale = 256,
        .pokemonOffset = 2,
        .trainerScale = 300,
        .trainerOffset = 1,

        .iconSprite = gMonIcon_Slaking,
        .iconPalIndex = P_GBA_STYLE_SPECIES_ICONS ? 1 : 2,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(0, 6, SHADOW_SIZE_XL_BATTLE_ONLY)
        FOOTPRINT(Slaking)
        OVERWORLD(
            sPicTable_Slaking,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Slaking,
            gShinyOverworldPalette_Slaking
        )
        .levelUpLearnset = sSlakingLevelUpLearnset,
        .teachableLearnset = sSlakingTeachableLearnset,
        .frontPic = gMonFrontPic_SlakingMega,
        .backPic = gMonBackPic_SlakingMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 4,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_SlakingMega,
        .shinyPalette = gMonShinyPalette_SlakingMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sSlakingAddedMegaFormSpeciesIdTable,
        .formChangeTable = sSlakingAddedMegaFormChangeTable,
    },

    [SPECIES_TORTERRA_MEGA] =
    {
        .baseHP = 95,
        .baseAttack = 139,
        .baseDefense = 145,
        .baseSpeed = 46,
        .baseSpAttack = 85,
        .baseSpDefense = 115,
        .types = MON_TYPES(TYPE_GRASS, TYPE_GROUND),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 263,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 236,
    #else
        .expYield = 208,
    #endif
        .evYield_Attack = 2,
        .evYield_Defense = 1,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MONSTER, EGG_GROUP_GRASS),
        .abilities = { ABILITY_SOLID_ROCK, ABILITY_SOLID_ROCK, ABILITY_SOLID_ROCK },
        .bodyColor = BODY_COLOR_GREEN,
        .noFlip = TRUE,
        .speciesName = _("Torterra"),
        .cryId = CRY_TORTERRA,
        .natDexNum = NATIONAL_DEX_TORTERRA,
        .categoryName = _("Continent"),
        .height = 22,
        .weight = 3100,
        .description = COMPOUND_STRING(
            "Some Pokémon are born on a Torterra's\n"
            "back and spend their entire life there.\n"
            "Ancient people imagined that beneath \n"
            "the ground, a gigantic Torterra dwelled."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 348,
        .trainerOffset = 6,

        .iconSprite = gMonIcon_Torterra,
        .iconPalIndex = 1,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(1, 10, SHADOW_SIZE_L)
        FOOTPRINT(Torterra)
        OVERWORLD(
            sPicTable_Torterra,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following_Asym,
            gOverworldPalette_Torterra,
            gShinyOverworldPalette_Torterra
        )
        .levelUpLearnset = sTorterraLevelUpLearnset,
        .teachableLearnset = sTorterraTeachableLearnset,
        .frontPic = gMonFrontPic_TorterraMega,
        .backPic = gMonBackPic_TorterraMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 4,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_TorterraMega,
        .shinyPalette = gMonShinyPalette_TorterraMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sTorterraAddedMegaFormSpeciesIdTable,
        .formChangeTable = sTorterraAddedMegaFormChangeTable,
    },

    [SPECIES_CLAWITZER_MEGA] =
    {
        .baseHP = 71,
        .baseAttack = 83,
        .baseDefense = 108,
        .baseSpeed = 79,
        .baseSpAttack = 170,
        .baseSpDefense = 89,
        .types = MON_TYPES(TYPE_WATER, TYPE_DRAGON),
        .catchRate = 55,
        .expYield = 100,
        .evYield_SpAttack = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 15,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_1, EGG_GROUP_WATER_3),
        .abilities = { ABILITY_MEGA_LAUNCHER, ABILITY_MEGA_LAUNCHER, ABILITY_MEGA_LAUNCHER },
        .bodyColor = BODY_COLOR_BLUE,
        .noFlip = TRUE,
        .speciesName = _("Clawitzer"),
        .cryId = CRY_CLAWITZER,
        .natDexNum = NATIONAL_DEX_CLAWITZER,
        .categoryName = _("Howitzer"),
        .height = 13,
        .weight = 353,
        .description = COMPOUND_STRING(
            "By expelling water from the nozzle in the\n"
            "back of its enormous claw, it can move\n"
            "at a speed of 60 knots. They launch\n"
            "cannonballs made of water."),
        .pokemonScale = 272,
        .pokemonOffset = 3,
        .trainerScale = 256,
        .trainerOffset = 0,

        .iconSprite = gMonIcon_Clawitzer,
        .iconPalIndex = 0,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(6, 1, SHADOW_SIZE_L)
        FOOTPRINT(Clawitzer)
        OVERWORLD(
            sPicTable_Clawitzer,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following_Asym,
            gOverworldPalette_Clawitzer,
            gShinyOverworldPalette_Clawitzer
        )
        .levelUpLearnset = sClawitzerLevelUpLearnset,
        .teachableLearnset = sClawitzerTeachableLearnset,
        .frontPic = gMonFrontPic_ClawitzerMega,
        .backPic = gMonBackPic_ClawitzerMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_ClawitzerMega,
        .shinyPalette = gMonShinyPalette_ClawitzerMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sClawitzerAddedMegaFormSpeciesIdTable,
        .formChangeTable = sClawitzerAddedMegaFormChangeTable,
        .frontPic80 = gMonFrontPic80_ClawitzerMega,
        .backPic80 = gMonBackPic80_ClawitzerMega,
    },

    [SPECIES_TINKATON_MEGA] =
    {
        .baseHP = 85,
        .baseAttack = 115,
        .baseDefense = 107,
        .baseSpeed = 114,
        .baseSpAttack = 70,
        .baseSpDefense = 115,
        .types = MON_TYPES(TYPE_FAIRY, TYPE_STEEL),
        .catchRate = 45,
        .expYield = 253,
        .evYield_SpDefense = 3,
        .genderRatio = MON_FEMALE,
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FAIRY),
        .abilities = { ABILITY_TOUGH_CLAWS, ABILITY_TOUGH_CLAWS, ABILITY_TOUGH_CLAWS },
        .bodyColor = BODY_COLOR_PINK,
        .speciesName = _("Tinkaton"),
        .cryId = CRY_TINKATON,
        .natDexNum = NATIONAL_DEX_TINKATON,
        .categoryName = _("Hammer"),
        .height = 7,
        .weight = 1128,
        .description = COMPOUND_STRING(
            "The hammer tops 220 pounds, yet it\n"
            "gets swung around easily by\n"
            "Tinkaton as it steals whatever it pleases\n"
            "and carries its plunder back home."),
        .pokemonScale = 356,
        .pokemonOffset = 17,
        .trainerScale = 256,
        .trainerOffset = 0,

        .iconSprite = gMonIcon_Tinkaton,
        .iconPalIndex = 1,
        .pokemonJumpType = PKMN_JUMP_TYPE_SLOW,
        SHADOW(-5, 15, SHADOW_SIZE_XL_BATTLE_ONLY)
        FOOTPRINT(Tinkaton)
        OVERWORLD(
            sPicTable_Tinkaton,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following_Asym,
            gOverworldPalette_Tinkaton,
            gShinyOverworldPalette_Tinkaton
        )
        .levelUpLearnset = sTinkatonLevelUpLearnset,
        .teachableLearnset = sTinkatonTeachableLearnset,
        .frontPic = gMonFrontPic_TinkatonMega,
        .backPic = gMonBackPic_TinkatonMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_TinkatonMega,
        .shinyPalette = gMonShinyPalette_TinkatonMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sTinkatonAddedMegaFormSpeciesIdTable,
        .formChangeTable = sTinkatonAddedMegaFormChangeTable,
        .frontPic80 = gMonFrontPic80_TinkatonMega,
        .backPic80 = gMonBackPic80_TinkatonMega,
    },

    [SPECIES_ARBOLIVA_MEGA] =
    {
        .baseHP = 78,
        .baseAttack = 69,
        .baseDefense = 120,
        .baseSpeed = 49,
        .baseSpAttack = 165,
        .baseSpDefense = 129,
        .types = MON_TYPES(TYPE_GRASS, TYPE_FAIRY),
        .catchRate = 45,
        .expYield = 255,
        .evYield_SpAttack = 3,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_GRASS),
        .abilities = { ABILITY_GRASSY_SURGE, ABILITY_GRASSY_SURGE, ABILITY_GRASSY_SURGE },
        .bodyColor = BODY_COLOR_GREEN,
        .speciesName = _("Arboliva"),
        .cryId = CRY_ARBOLIVA,
        .natDexNum = NATIONAL_DEX_ARBOLIVA,
        .categoryName = _("Olive"),
        .height = 14,
        .weight = 482,
        .description = COMPOUND_STRING(
            "This Pokémon drives back enemies\n"
            "by launching its rich, aromatic oil at\n"
            "them with enough force to smash a\n"
            "boulder."),
        .pokemonScale = 356,
        .pokemonOffset = 17,
        .trainerScale = 256,
        .trainerOffset = 0,

        .iconSprite = gMonIcon_Arboliva,
        .iconPalIndex = 1,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(1, 13, SHADOW_SIZE_L)
        FOOTPRINT(Arboliva)
        OVERWORLD(
            sPicTable_Arboliva,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Arboliva,
            gShinyOverworldPalette_Arboliva
        )
        .levelUpLearnset = sArbolivaLevelUpLearnset,
        .teachableLearnset = sArbolivaTeachableLearnset,
        .frontPic = gMonFrontPic_ArbolivaMega,
        .backPic = gMonBackPic_ArbolivaMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_ArbolivaMega,
        .shinyPalette = gMonShinyPalette_ArbolivaMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sArbolivaAddedMegaFormSpeciesIdTable,
        .formChangeTable = sArbolivaAddedMegaFormChangeTable,
        .frontPic80 = gMonFrontPic80_ArbolivaMega,
        .backPic80 = gMonBackPic80_ArbolivaMega,
    },

    [SPECIES_CRYOGONAL_MEGA] =
    {
        .baseHP = 80,
        .baseAttack = 50,
        .baseDefense = 90,
        .baseSpeed = 125,
        .baseSpAttack = 125,
        .baseSpDefense = 145,
        .types = MON_TYPES(TYPE_ICE),
        .catchRate = 25,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_7) ? 180 : 170,
        .evYield_SpDefense = 2,
        .itemRare = ITEM_NEVER_MELT_ICE,
        .genderRatio = MON_GENDERLESS,
        .eggCycles = 25,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MINERAL),
        .abilities = { ABILITY_LEVITATE, ABILITY_LEVITATE, ABILITY_LEVITATE },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Cryogonal"),
        .cryId = CRY_CRYOGONAL,
        .natDexNum = NATIONAL_DEX_CRYOGONAL,
        .categoryName = _("Crystallize"),
        .height = 11,
        .weight = 1480,
        .description = COMPOUND_STRING(
            "They are born in snow clouds. When\n"
            "its body temperature goes up, it turns\n"
            "into steam and vanishes. When its\n"
            "temperature lowers, it returns to ice."),
        .pokemonScale = 320,
        .pokemonOffset = 7,
        .trainerScale = 256,
        .trainerOffset = 0,

        .enemyMonElevation = 3,

        .iconSprite = gMonIcon_Cryogonal,
        .iconPalIndex = 0,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(0, 14, SHADOW_SIZE_M)
        FOOTPRINT(Cryogonal)
        OVERWORLD(
            sPicTable_Cryogonal,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Cryogonal,
            gShinyOverworldPalette_Cryogonal
        )
        .levelUpLearnset = sCryogonalLevelUpLearnset,
        .teachableLearnset = sCryogonalTeachableLearnset,
        .frontPic = gMonFrontPic_CryogonalMega,
        .backPic = gMonBackPic_CryogonalMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_CryogonalMega,
        .shinyPalette = gMonShinyPalette_CryogonalMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sCryogonalAddedMegaFormSpeciesIdTable,
        .formChangeTable = sCryogonalAddedMegaFormChangeTable,
        .frontPic80 = gMonFrontPic80_CryogonalMega,
        .backPic80 = gMonBackPic80_CryogonalMega,
    },

    [SPECIES_HAXORUS_MEGA] =
    {
        .baseHP = 76,
        .baseAttack = 177,
        .baseDefense = 120,
        .baseSpeed = 117,
        .baseSpAttack = 60,
        .baseSpDefense = 90,
        .types = MON_TYPES(TYPE_DRAGON, TYPE_STEEL),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_8) ? 270 : 243,
        .evYield_Attack = 3,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 40,
        .friendship = 35,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MONSTER, EGG_GROUP_DRAGON),
        .abilities = { ABILITY_SHARPNESS, ABILITY_SHARPNESS, ABILITY_SHARPNESS },
        .bodyColor = BODY_COLOR_YELLOW,
        .speciesName = _("Haxorus"),
        .cryId = CRY_HAXORUS,
        .natDexNum = NATIONAL_DEX_HAXORUS,
        .categoryName = _("Axe Jaw"),
        .height = 18,
        .weight = 1055,
        .description = COMPOUND_STRING(
            "They are kind but can be relentless\n"
            "when defending territory. Their sturdy\n"
            "tusks will stay sharp even if used to cut\n"
            "steel beams."),
        .pokemonScale = 267,
        .pokemonOffset = 2,
        .trainerScale = 286,
        .trainerOffset = 1,

        .iconSprite = gMonIcon_Haxorus,
        .iconPalIndex = 2,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        SHADOW(2, 9, SHADOW_SIZE_L)
        FOOTPRINT(Haxorus)
        OVERWORLD(
            sPicTable_Haxorus,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Haxorus,
            gShinyOverworldPalette_Haxorus
        )
        .levelUpLearnset = sHaxorusLevelUpLearnset,
        .teachableLearnset = sHaxorusTeachableLearnset,
        .frontPic = gMonFrontPic_HaxorusMega,
        .backPic = gMonBackPic_HaxorusMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .backPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .palette = gMonPalette_HaxorusMega,
        .shinyPalette = gMonShinyPalette_HaxorusMega,
        .isMegaEvolution = TRUE,
        .formSpeciesIdTable = sHaxorusAddedMegaFormSpeciesIdTable,
        .formChangeTable = sHaxorusAddedMegaFormChangeTable,
        .frontPic80 = gMonFrontPic80_HaxorusMega,
        .backPic80 = gMonBackPic80_HaxorusMega,
    },
