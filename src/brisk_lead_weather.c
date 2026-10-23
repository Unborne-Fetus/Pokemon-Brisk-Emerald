#include "global.h"
#include "brisk_lead_weather.h"
#include "field_weather.h"
#include "overworld.h"
#include "palette.h"
#include "pokemon.h"
#include "script.h"
#include "constants/map_types.h"
#include "constants/weather.h"

static bool32 IsProtectedWeather(u8 weather)
{
    switch (weather)
    {
    case WEATHER_SANDSTORM: // Includes the Route 111 desert/access restriction.
    case WEATHER_ABNORMAL: // Groudon/Kyogre crisis.
    case WEATHER_DROUGHT:  // Terra Cave locator and scripted legendary weather.
    case WEATHER_DOWNPOUR: // Marine Cave locator.
    case WEATHER_VOLCANIC_ASH: // Soot collection.
    case WEATHER_FOG:
    case WEATHER_FOG_HORIZONTAL:
    case WEATHER_FOG_DIAGONAL:
    case WEATHER_UNDERWATER:
    case WEATHER_UNDERWATER_BUBBLES:
        return TRUE;
    default:
        return FALSE;
    }
}

u8 BriskLeadWeather_Apply(u8 weather)
{
    if (gSaveBlock1Ptr == NULL || ScriptContext_IsEnabled() || ArePlayerFieldControlsLocked())
        return weather;
    switch (gMapHeader.mapType)
    {
    case MAP_TYPE_TOWN:
    case MAP_TYPE_CITY:
    case MAP_TYPE_ROUTE:
    case MAP_TYPE_OCEAN_ROUTE:
        break;
    default:
        return weather;
    }
    // This reads native weather, never the lead's visual overlay. In
    // particular, ability sandstorms cannot activate desert access checks.
    if (IsProtectedWeather(GetSavedWeather()) || IsProtectedWeather(gMapHeader.weather))
        return weather;
    struct Pokemon *lead = &gParties[B_TRAINER_PLAYER][0];
    enum Species species = GetMonData(lead, MON_DATA_SPECIES_OR_EGG);
    if (species == SPECIES_NONE || species == SPECIES_EGG)
        return weather;
    switch (GetMonAbility(lead))
    {
    case ABILITY_DRIZZLE:
        return WEATHER_RAIN;
    case ABILITY_PRIMORDIAL_SEA:
        return WEATHER_DOWNPOUR;
    case ABILITY_DROUGHT:
    case ABILITY_DESOLATE_LAND:
    case ABILITY_ORICHALCUM_PULSE:
        return WEATHER_DROUGHT;
    case ABILITY_SAND_STREAM:
    case ABILITY_SAND_SPIT:
        return WEATHER_SANDSTORM;
    case ABILITY_SNOW_WARNING:
        return WEATHER_SNOW;
    case ABILITY_DELTA_STREAM:
        return WEATHER_STRONG_WINDS;
    default:
        return weather;
    }
}

void BriskLeadWeather_Update(void)
{
    // Weather scripts own their transitions. Abnormal weather has its own
    // alternating task and is not a directly renderable weather index.
    if (gSaveBlock1Ptr == NULL || gPaletteFade.active || ScriptContext_IsEnabled() || ArePlayerFieldControlsLocked())
        return;
    u8 native = GetSavedWeather();
    if (native >= WEATHER_ABNORMAL && native != WEATHER_STRONG_WINDS)
        return;
    u8 effective = BriskLeadWeather_Apply(native);
    if (gWeatherPtr->nextWeather != effective)
        SetNextWeather(effective);
}
