#include "global.h"
#include "event_data.h"
#include "pokemon.h"
#include "pokedex.h"
#include "test/test.h"

TEST("Brisk onigin_pixelart all nine species have complete graphics and usable postgame moves")
{
    const enum Species species[] = {SPECIES_BUDCHIRP, SPECIES_VERDWING,
        SPECIES_ROSPLASH, SPECIES_ROSAFIN, SPECIES_FERRUNT, SPECIES_FERRADON,
        SPECIES_FERRAGON, SPECIES_JELLUME, SPECIES_VOLTIJELLY};
    for (u32 i = 0; i < ARRAY_COUNT(species); i++)
    {
        const struct SpeciesInfo *info = &gSpeciesInfo[species[i]];
        EXPECT(IsSpeciesEnabled(species[i]));
        EXPECT(info->frontPic != NULL);
        EXPECT(info->backPic != NULL);
        EXPECT(info->iconSprite != NULL);
        EXPECT(info->palette != NULL);
        EXPECT(info->shinyPalette != NULL);
        EXPECT(memcmp(info->palette, info->shinyPalette, 32) != 0);
        EXPECT_EQ(NationalPokedexNumToSpecies(info->natDexNum), species[i]);
        EXPECT(info->abilities[0] != ABILITY_NONE);
        EXPECT(info->abilities[2] != ABILITY_NONE);
        struct Pokemon mon;
        CreateMonWithIVs(&mon, species[i], 90, 0, OTID_STRUCT_PLAYER_ID, 31);
        GiveMonInitialMoveset(&mon);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_SPECIES), species[i]);
        for (u32 move = 0; move < MAX_MON_MOVES; move++)
            EXPECT(GetMonData(&mon, MON_DATA_MOVE1 + move) != MOVE_NONE);
    }
}

TEST("Brisk onigin_pixelart all five evolution links respect their level thresholds")
{
    const u16 routes[][3] = {
        {SPECIES_BUDCHIRP, 32, SPECIES_VERDWING},
        {SPECIES_ROSPLASH, 30, SPECIES_ROSAFIN},
        {SPECIES_FERRUNT, 28, SPECIES_FERRADON},
        {SPECIES_FERRADON, 48, SPECIES_FERRAGON},
        {SPECIES_JELLUME, 34, SPECIES_VOLTIJELLY},
    };
    for (u32 i = 0; i < ARRAY_COUNT(routes); i++)
    {
        struct Pokemon mon;
        bool32 canStop;
        CreateMonWithIVs(&mon, routes[i][0], routes[i][1] - 1, 0, OTID_STRUCT_PLAYER_ID, 31);
        EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_NORMAL, ITEM_NONE, NULL, &canStop, CHECK_EVO), SPECIES_NONE);
        CreateMonWithIVs(&mon, routes[i][0], routes[i][1], 0, OTID_STRUCT_PLAYER_ID, 31);
        EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_NORMAL, ITEM_NONE, NULL, &canStop, CHECK_EVO), routes[i][2]);
    }
}
