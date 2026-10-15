#include "global.h"
#include "battle.h"
#include "item.h"
#include "constants/hold_effects.h"
#include "pokemon.h"
#include "test/battle.h"

static const struct CustomMegaStoneCase
{
    enum Species base, mega;
    enum Item stone;
} sCustomMegaStones[] =
{
    {SPECIES_AMOONGUSS, SPECIES_AMOONGUSS_MEGA, ITEM_AMOONGUSSITE},
    {SPECIES_AZUMARILL, SPECIES_AZUMARILL_MEGA, ITEM_AZUMARILLITE},
    {SPECIES_BUTTERFREE, SPECIES_BUTTERFREE_MEGA, ITEM_BUTTERFREEITE},
    {SPECIES_CONKELDURR, SPECIES_CONKELDURR_MEGA, ITEM_CONKELDURRITE},
    {SPECIES_DARKRAI, SPECIES_DARKRAI_MEGA_M, ITEM_DARKRANITE_M},
    {SPECIES_DUGTRIO, SPECIES_DUGTRIO_MEGA, ITEM_DUGTRIOITE},
    {SPECIES_ELECTIVIRE, SPECIES_ELECTIVIRE_MEGA, ITEM_ELECTIVIREITE},
    {SPECIES_EXCADRILL, SPECIES_EXCADRILL_MEGA_M, ITEM_EXCADRITE_M},
    {SPECIES_FERALIGATR, SPECIES_FERALIGATR_MEGA_M, ITEM_FERALIGITE_M},
    {SPECIES_FLYGON, SPECIES_FLYGON_MEGA, ITEM_FLYGONITE},
    {SPECIES_GOLEM, SPECIES_GOLEM_MEGA, ITEM_GOLEMITE},
    {SPECIES_JUMPLUFF, SPECIES_JUMPLUFF_MEGA, ITEM_JUMPLUFFITE},
    {SPECIES_KABUTOPS, SPECIES_KABUTOPS_MEGA, ITEM_KABUTOPSITE},
    {SPECIES_MAGMORTAR, SPECIES_MAGMORTAR_MEGA, ITEM_MAGMORTARITE},
    {SPECIES_MEGANIUM, SPECIES_MEGANIUM_MEGA_M, ITEM_MEGANIUMITE_M},
    {SPECIES_MILOTIC, SPECIES_MILOTIC_MEGA, ITEM_MILOTICITE},
    {SPECIES_OMASTAR, SPECIES_OMASTAR_MEGA, ITEM_OMASTARITE},
    {SPECIES_POLIWRATH, SPECIES_POLIWRATH_MEGA, ITEM_POLIWRATHITE},
    {SPECIES_PYROAR, SPECIES_PYROAR_MEGA_M, ITEM_PYROARITE_M},
    {SPECIES_SHIFTRY, SPECIES_SHIFTRY_MEGA, ITEM_SHIFTRYITE},
    {SPECIES_SNORLAX, SPECIES_SNORLAX_MEGA, ITEM_SNORLAXITE},
    {SPECIES_TORKOAL, SPECIES_TORKOAL_MEGA, ITEM_TORKOALITE},
    {SPECIES_TYPHLOSION, SPECIES_TYPHLOSION_MEGA, ITEM_TYPHLOSIONITE},
    {SPECIES_WHIMSICOTT, SPECIES_WHIMSICOTT_MEGA, ITEM_WHIMSICOTTITE},
};

TEST("Custom Mega stones select the correct form and keep it when switching")
{
    for (u32 i = 0; i < ARRAY_COUNT(sCustomMegaStones); i++)
    {
        const struct CustomMegaStoneCase *row = &sCustomMegaStones[i];
        struct FormChangeContext ctx = {.currentSpecies = row->base, .heldItem = row->stone, .method = FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM};
        EXPECT_EQ(GetFormChangeTargetSpecies_Internal(ctx), row->mega);
        ctx.heldItem = ITEM_NONE;
        EXPECT_EQ(GetFormChangeTargetSpecies_Internal(ctx), row->base);
        ctx.currentSpecies = row->mega;
        ctx.heldItem = row->stone;
        ctx.method = FORM_CHANGE_BATTLE_SWITCH_OUT;
        EXPECT_EQ(GetFormChangeTargetSpecies_Internal(ctx), row->mega);
        ctx.method = FORM_CHANGE_END_BATTLE;
        EXPECT_EQ(GetFormChangeTargetSpecies_Internal(ctx), row->base);
        ctx.method = FORM_CHANGE_FAINT;
        EXPECT_EQ(GetFormChangeTargetSpecies_Internal(ctx), row->base);
        EXPECT_EQ(gItemsInfo[row->stone].holdEffect, HOLD_EFFECT_MEGA_STONE);
        EXPECT(gItemsInfo[row->stone].iconPic != NULL);
        EXPECT(gItemsInfo[row->stone].iconPalette != NULL);
    }
}

SINGLE_BATTLE_TEST("Custom Mega stones enable all 24 forms in battle")
{
    enum Species base = SPECIES_NONE, mega = SPECIES_NONE;
    enum Item stone = ITEM_NONE;
    for (u32 i = 0; i < ARRAY_COUNT(sCustomMegaStones); i++)
        PARAMETRIZE { base = sCustomMegaStones[i].base; mega = sCustomMegaStones[i].mega; stone = sCustomMegaStones[i].stone; }
    GIVEN {
        PLAYER(base) { Item(stone); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE, gimmick: GIMMICK_MEGA); }
    } THEN {
        EXPECT_EQ(player->species, mega);
        EXPECT_EQ(player->ability, gSpeciesInfo[mega].abilities[0]);
    }
}

TEST("Custom Mega stones preserve the five original Mega stone targets")
{
    static const struct CustomMegaStoneCase original[] = {
        {SPECIES_MEGANIUM, SPECIES_MEGANIUM_MEGA, ITEM_MEGANIUMITE},
        {SPECIES_FERALIGATR, SPECIES_FERALIGATR_MEGA, ITEM_FERALIGITE},
        {SPECIES_EXCADRILL, SPECIES_EXCADRILL_MEGA, ITEM_EXCADRITE},
        {SPECIES_PYROAR, SPECIES_PYROAR_MEGA, ITEM_PYROARITE},
        {SPECIES_DARKRAI, SPECIES_DARKRAI_MEGA, ITEM_DARKRANITE},
    };
    for (u32 i = 0; i < ARRAY_COUNT(original); i++)
    {
        struct FormChangeContext ctx = {.currentSpecies = original[i].base, .heldItem = original[i].stone, .method = FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM};
        EXPECT_EQ(GetFormChangeTargetSpecies_Internal(ctx), original[i].mega);
    }
}
