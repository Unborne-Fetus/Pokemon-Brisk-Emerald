#include "global.h"
#include "pokemon.h"
#include "battle.h"
#include "pokedex.h"
#include "constants/abilities.h"
#include "test/test.h"

TEST("KFWeagz imports retain supplied art, healthy boxed data and complete movesets")
{
    for (u32 species = SPECIES_DRIFBLIM_MEGA; species <= SPECIES_STEAM_SHELL; species++)
    {
        struct Pokemon mon, restored;
        CreateMonWithIVs(&mon, species, 70, 0, OTID_STRUCT_PLAYER_ID, 31);
        GiveMonInitialMoveset(&mon);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_SPECIES), species);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_SANITY_IS_BAD_EGG), FALSE);
        EXPECT_GT(GetMonData(&mon, MON_DATA_MAX_HP), 0);
        for (u32 i = 0; i < MAX_MON_MOVES; i++)
            EXPECT_NE(GetMonData(&mon, MON_DATA_MOVE1 + i), MOVE_NONE);
        EXPECT(gSpeciesInfo[species].frontPic80 != NULL);
        EXPECT(gSpeciesInfo[species].backPic80 != NULL);
        EXPECT(gSpeciesInfo[species].palette != gSpeciesInfo[species].shinyPalette);
        EXPECT(gSpeciesInfo[species].iconSprite != NULL);
        EXPECT_NE(gSpeciesInfo[species].abilities[0], ABILITY_NONE);
        BoxMonToMon(&mon.box, &restored);
        EXPECT_EQ(GetMonData(&restored, MON_DATA_SPECIES), species);
        EXPECT_EQ(GetMonData(&restored, MON_DATA_SANITY_IS_BAD_EGG), FALSE);
    }
}

TEST("KFWeagz Megas keep base HP and gain exactly 100 base stats")
{
    static const u16 forms[][2] = {
        {SPECIES_DRIFBLIM, SPECIES_DRIFBLIM_MEGA},
        {SPECIES_LAPRAS, SPECIES_LAPRAS_MEGA},
        {SPECIES_SEVIPER, SPECIES_SEVIPER_MEGA},
        {SPECIES_HYDREIGON, SPECIES_HYDREIGON_MEGA},
        {SPECIES_REGIGIGAS, SPECIES_REGIGIGAS_MEGA},
        {SPECIES_NOIVERN, SPECIES_NOIVERN_MEGA},
        {SPECIES_KINGDRA, SPECIES_KINGDRA_MEGA},
        {SPECIES_FEAROW, SPECIES_FEAROW_MEGA},
        {SPECIES_TOGEKISS, SPECIES_TOGEKISS_MEGA},
        {SPECIES_VOLCARONA, SPECIES_VOLCARONA_MEGA},
        {SPECIES_ZANGOOSE, SPECIES_ZANGOOSE_MEGA},
        {SPECIES_QUAGSIRE, SPECIES_QUAGSIRE_MEGA},
        {SPECIES_XATU, SPECIES_XATU_MEGA},
    };
    for (u32 i = 0; i < ARRAY_COUNT(forms); i++)
    {
        const struct SpeciesInfo *base = &gSpeciesInfo[forms[i][0]];
        const struct SpeciesInfo *mega = &gSpeciesInfo[forms[i][1]];
        u32 baseTotal = base->baseHP + base->baseAttack + base->baseDefense + base->baseSpeed + base->baseSpAttack + base->baseSpDefense;
        u32 megaTotal = mega->baseHP + mega->baseAttack + mega->baseDefense + mega->baseSpeed + mega->baseSpAttack + mega->baseSpDefense;
        EXPECT_EQ(megaTotal, baseTotal + 100);
        EXPECT_EQ(mega->baseHP, base->baseHP);
        EXPECT(mega->isMegaEvolution);
        EXPECT_EQ(GET_BASE_SPECIES_ID(forms[i][1]), forms[i][0]);
    }
}

TEST("Both past paradoxes evolve permanently with a Sun Stone, not another stone or level")
{
    static const u16 evolutions[][2] = {
        {SPECIES_BLASTOISE, SPECIES_STEAM_SHELL},
        {SPECIES_GARGANACL, SPECIES_IRON_FURNACE},
    };
    for (u32 i = 0; i < ARRAY_COUNT(evolutions); i++)
    {
        struct Pokemon mon;
        bool32 canStop;
        CreateMonWithIVs(&mon, evolutions[i][0], 70, 0, OTID_STRUCT_PLAYER_ID, 31);
        EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_SUN_STONE, NULL, &canStop, CHECK_EVO), evolutions[i][1]);
        EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_FIRE_STONE, NULL, &canStop, CHECK_EVO), SPECIES_NONE);
        EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_NORMAL, ITEM_NONE, NULL, &canStop, CHECK_EVO), SPECIES_NONE);
        const struct SpeciesInfo *paradox = &gSpeciesInfo[evolutions[i][1]];
        EXPECT(paradox->isParadox);
        EXPECT(!paradox->isMegaEvolution);
        EXPECT_EQ(paradox->abilities[0], ABILITY_PROTOSYNTHESIS);
        EXPECT_EQ(GET_BASE_SPECIES_ID(evolutions[i][1]), evolutions[i][1]);
        EXPECT_EQ(NationalPokedexNumToSpecies(SpeciesToNationalPokedexNum(evolutions[i][1])), evolutions[i][1]);
        struct FormChangeContext ctx = {.currentSpecies = evolutions[i][1], .method = FORM_CHANGE_END_BATTLE};
        EXPECT_EQ(GetFormChangeTargetSpecies_Internal(ctx), evolutions[i][1]);
    }
}

TEST("Sun Stone additions preserve Blastoise Mega and both existing Gigantamax form indexes")
{
    EXPECT_EQ(GetFormSpeciesId(SPECIES_BLASTOISE, 1), SPECIES_BLASTOISE_MEGA);
    EXPECT_EQ(GetFormSpeciesId(SPECIES_BLASTOISE, 2), SPECIES_BLASTOISE_GMAX);
    EXPECT_EQ(GetFormSpeciesId(SPECIES_LAPRAS, 1), SPECIES_LAPRAS_GMAX);
    EXPECT_EQ(GetFormSpeciesId(SPECIES_LAPRAS, 2), SPECIES_LAPRAS_MEGA);
    struct FormChangeContext ctx = {.currentSpecies = SPECIES_BLASTOISE, .heldItem = ITEM_BLASTOISINITE, .method = FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM};
    EXPECT_EQ(GetFormChangeTargetSpecies_Internal(ctx), SPECIES_BLASTOISE_MEGA);
    ctx.method = FORM_CHANGE_BATTLE_GIGANTAMAX;
    ctx.gmaxFactor = TRUE;
    EXPECT_EQ(GetFormChangeTargetSpecies_Internal(ctx), SPECIES_BLASTOISE_GMAX);
    ctx.currentSpecies = SPECIES_LAPRAS;
    EXPECT_EQ(GetFormChangeTargetSpecies_Internal(ctx), SPECIES_LAPRAS_GMAX);
}
