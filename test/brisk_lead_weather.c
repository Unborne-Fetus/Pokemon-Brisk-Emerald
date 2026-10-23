#include "global.h"
#include "brisk_lead_weather.h"
#include "brisk_modes.h"
#include "event_data.h"
#include "field_weather.h"
#include "overworld.h"
#include "palette.h"
#include "pokemon.h"
#include "script.h"
#include "test/test.h"
#include "constants/map_types.h"
#include "constants/weather.h"

static void SetupLeadWeather(enum Species species)
{
    ScriptContext_Init();
    UnlockPlayerFieldControls();
    VarSet(VAR_BRISK_RANDOMIZER, 0);
    gMapHeader.mapType = MAP_TYPE_ROUTE;
    gMapHeader.weather = WEATHER_SUNNY;
    gSaveBlock1Ptr->weather = WEATHER_SUNNY;
    CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][0], species, 50, 0, OTID_STRUCT_PRESET(0), 31);
}

TEST("Brisk lead weather maps every weather ability without changing native weather")
{
    static const u16 rows[][3] = {
        {SPECIES_KYOGRE, ABILITY_DRIZZLE, WEATHER_RAIN},
        {SPECIES_GROUDON, ABILITY_DROUGHT, WEATHER_DROUGHT},
        {SPECIES_TYRANITAR, ABILITY_SAND_STREAM, WEATHER_SANDSTORM},
        {SPECIES_SANDACONDA, ABILITY_SAND_SPIT, WEATHER_SANDSTORM},
        {SPECIES_ABOMASNOW, ABILITY_SNOW_WARNING, WEATHER_SNOW},
        {SPECIES_KYOGRE_PRIMAL, ABILITY_PRIMORDIAL_SEA, WEATHER_DOWNPOUR},
        {SPECIES_GROUDON_PRIMAL, ABILITY_DESOLATE_LAND, WEATHER_DROUGHT},
        {SPECIES_KORAIDON, ABILITY_ORICHALCUM_PULSE, WEATHER_DROUGHT},
        {SPECIES_RAYQUAZA_MEGA, ABILITY_DELTA_STREAM, WEATHER_STRONG_WINDS},
    };
    struct MapHeader oldHeader = gMapHeader;
    u8 oldWeather = GetSavedWeather();
    for (u32 i = 0; i < ARRAY_COUNT(rows); i++)
    {
        SetupLeadWeather(rows[i][0]);
        EXPECT_EQ(GetMonAbility(&gParties[B_TRAINER_PLAYER][0]), rows[i][1]);
        EXPECT_EQ(BriskLeadWeather_Apply(WEATHER_SUNNY), rows[i][2]);
        EXPECT_EQ(GetSavedWeather(), WEATHER_SUNNY);
    }
    gMapHeader = oldHeader;
    gSaveBlock1Ptr->weather = oldWeather;
}

TEST("Brisk lead weather protects desert sandstorms and all special native weather")
{
    static const u8 special[] = {WEATHER_SANDSTORM, WEATHER_ABNORMAL, WEATHER_DROUGHT,
        WEATHER_DOWNPOUR, WEATHER_VOLCANIC_ASH, WEATHER_FOG_HORIZONTAL,
        WEATHER_FOG_DIAGONAL, WEATHER_UNDERWATER, WEATHER_UNDERWATER_BUBBLES};
    struct MapHeader oldHeader = gMapHeader;
    u8 oldWeather = GetSavedWeather();
    SetupLeadWeather(SPECIES_KYOGRE);
    for (u32 i = 0; i < ARRAY_COUNT(special); i++)
    {
        gSaveBlock1Ptr->weather = special[i];
        EXPECT_EQ(BriskLeadWeather_Apply(special[i]), special[i]);
        gSaveBlock1Ptr->weather = WEATHER_SUNNY;
        gMapHeader.weather = special[i];
        EXPECT_EQ(BriskLeadWeather_Apply(WEATHER_SUNNY), WEATHER_SUNNY);
        gMapHeader.weather = WEATHER_SUNNY;
    }
    gMapHeader = oldHeader;
    gSaveBlock1Ptr->weather = oldWeather;
}

TEST("Brisk lead weather stops indoors and ignores eggs and non-leading weather Pokemon")
{
    static const u8 indoor[] = {MAP_TYPE_INDOOR, MAP_TYPE_UNDERGROUND, MAP_TYPE_UNDERWATER, MAP_TYPE_SECRET_BASE};
    struct MapHeader oldHeader = gMapHeader;
    u8 oldWeather = GetSavedWeather();
    SetupLeadWeather(SPECIES_KYOGRE);
    for (u32 i = 0; i < ARRAY_COUNT(indoor); i++)
    {
        gMapHeader.mapType = indoor[i];
        EXPECT_EQ(BriskLeadWeather_Apply(WEATHER_NONE), WEATHER_NONE);
    }
    gMapHeader.mapType = MAP_TYPE_CITY;
    u32 egg = TRUE;
    SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_IS_EGG, &egg);
    EXPECT_EQ(BriskLeadWeather_Apply(WEATHER_SUNNY), WEATHER_SUNNY);
    SetupLeadWeather(SPECIES_PIKACHU);
    CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][1], SPECIES_KYOGRE, 50, 0, OTID_STRUCT_PRESET(0), 31);
    EXPECT_EQ(BriskLeadWeather_Apply(WEATHER_SUNNY), WEATHER_SUNNY);
    gMapHeader = oldHeader;
    gSaveBlock1Ptr->weather = oldWeather;
}

TEST("Brisk lead weather refreshes after party changes and restores original rain")
{
    struct MapHeader oldHeader = gMapHeader;
    u8 oldWeather = GetSavedWeather();
    u8 oldCurr = gWeatherPtr->currWeather, oldNext = gWeatherPtr->nextWeather;
    bool32 oldFade = gPaletteFade.active;
    SetupLeadWeather(SPECIES_TYRANITAR);
    gMapHeader.weather = gSaveBlock1Ptr->weather = WEATHER_RAIN;
    gWeatherPtr->currWeather = gWeatherPtr->nextWeather = WEATHER_RAIN;
    gPaletteFade.active = FALSE;
    BriskLeadWeather_Update();
    EXPECT_EQ(gWeatherPtr->nextWeather, WEATHER_SANDSTORM);
    EXPECT_EQ(GetSavedWeather(), WEATHER_RAIN); // Cannot become a desert/goggle requirement.
    gWeatherPtr->finishStep = 7;
    BriskLeadWeather_Update();
    EXPECT_EQ(gWeatherPtr->finishStep, 7); // Do not restart transitions every frame.
    CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][0], SPECIES_ABOMASNOW, 50, 0, OTID_STRUCT_PRESET(0), 31);
    BriskLeadWeather_Update();
    EXPECT_EQ(gWeatherPtr->nextWeather, WEATHER_SNOW);
    CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][0], SPECIES_PIKACHU, 50, 0, OTID_STRUCT_PRESET(0), 31);
    BriskLeadWeather_Update();
    EXPECT_EQ(gWeatherPtr->nextWeather, WEATHER_RAIN);
    gMapHeader = oldHeader;
    gSaveBlock1Ptr->weather = oldWeather;
    gWeatherPtr->currWeather = oldCurr;
    gWeatherPtr->nextWeather = oldNext;
    gPaletteFade.active = oldFade;
}

TEST("Brisk lead weather yields while scripts control the field")
{
    struct MapHeader oldHeader = gMapHeader;
    u8 oldWeather = GetSavedWeather();
    SetupLeadWeather(SPECIES_KYOGRE);
    LockPlayerFieldControls();
    EXPECT_EQ(BriskLeadWeather_Apply(WEATHER_SUNNY), WEATHER_SUNNY);
    UnlockPlayerFieldControls();
    EXPECT_EQ(BriskLeadWeather_Apply(WEATHER_SUNNY), WEATHER_RAIN);
    gMapHeader = oldHeader;
    gSaveBlock1Ptr->weather = oldWeather;
}
