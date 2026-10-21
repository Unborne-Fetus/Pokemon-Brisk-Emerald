#ifndef GUARD_BRISK_LIGHTING_H
#define GUARD_BRISK_LIGHTING_H

#include "palette.h"

// Environment selection is independent of UI light/dark mode and save data.
enum BriskLightingProfile
{
    BRISK_LIGHT_PLAINS,
    BRISK_LIGHT_COAST,
    BRISK_LIGHT_ISLANDS,
    BRISK_LIGHT_FOREST,
    BRISK_LIGHT_MOUNTAIN,
    BRISK_LIGHT_VOLCANIC,
    BRISK_LIGHT_CAVE,
    BRISK_LIGHT_ICE,
    BRISK_LIGHT_UNDERWATER,
    BRISK_LIGHT_RUINS,
    BRISK_LIGHT_HAUNTED,
    BRISK_LIGHT_INDUSTRIAL,
    BRISK_LIGHT_INTERIOR,
    BRISK_LIGHT_PROFILE_COUNT,
};

bool32 BriskLighting_IsEnabled(u32 mapType);
enum BriskLightingProfile BriskLighting_GetProfile(u32 mapType, u32 section);
struct BlendSettings BriskLighting_GetTimeBlend(u32 time);

#endif
