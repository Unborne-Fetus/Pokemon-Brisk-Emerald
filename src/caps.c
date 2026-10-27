#include "global.h"
#include "battle.h"
#include "event_data.h"
#include "caps.h"
#include "brisk_modes.h"
#include "pokemon.h"


bool32 IsExtremeExpCap(void)
{
    return GetBriskDifficulty() == BRISK_MODE_EXTREME;
}

bool32 BriskUsesExpCap(void)
{
    u32 mode = GetBriskDifficulty();
    return mode == BRISK_MODE_EXTREME || (mode == BRISK_MODE_HARD && !VarGet(VAR_BRISK_NO_LEVEL_CAP));
}

u32 GetProgressionLevelCap(void)
{
    u32 mode = GetBriskDifficulty();
    if (mode == BRISK_MODE_EASY || (mode != BRISK_MODE_EXTREME && VarGet(VAR_BRISK_NO_LEVEL_CAP)))
        return MAX_LEVEL;
    // Normal keeps its original obedience curve. Hard/Extreme use an explicit
    // curve shared with trainer scaling, rather than subtracting ten levels.
    static const u32 sLevelCapFlagMap[][3] =
    {
        // Milestone          Normal  Hard/Extreme
        {FLAG_BADGE01_GET,      25,     25},
        {FLAG_BADGE02_GET,      35,     35},
        {FLAG_BADGE03_GET,      45,     45},
        {FLAG_BADGE04_GET,      50,     50},
        {FLAG_BADGE05_GET,      55,     55},
        {FLAG_BADGE06_GET,      65,     65},
        {FLAG_BADGE07_GET,      75,     75},
        {FLAG_BADGE08_GET,     100,     90},
        {FLAG_IS_CHAMPION, MAX_LEVEL, MAX_LEVEL},
    };

    u32 cap = MAX_LEVEL;
    for (u32 i = 0; i < ARRAY_COUNT(sLevelCapFlagMap); i++)
    {
        if (!FlagGet(sLevelCapFlagMap[i][0]))
        {
            cap = sLevelCapFlagMap[i][mode == BRISK_MODE_HARD || mode == BRISK_MODE_EXTREME ? 2 : 1];
            break;
        }
    }

    return cap;
}

u32 GetCurrentLevelCap(void)
{
    if (BriskUsesExpCap())
        return GetProgressionLevelCap();
    if (VarGet(VAR_BRISK_NO_LEVEL_CAP))
        return MAX_LEVEL;
    if (B_LEVEL_CAP_TYPE == LEVEL_CAP_FLAG_LIST)
        return GetProgressionLevelCap();
    else if (B_LEVEL_CAP_TYPE == LEVEL_CAP_VARIABLE)
        return VarGet(B_LEVEL_CAP_VARIABLE);

    return MAX_LEVEL;
}

u32 GetSoftLevelCapExpValue(u32 level, u32 expValue)
{
    static const u32 sExpScalingDown[5] = { 4, 8, 16, 32, 64 };
    static const u32 sExpScalingUp[5]   = { 16, 8, 4, 2, 1 };

    u32 levelDifference;
    u32 currentLevelCap = GetCurrentLevelCap();

    if (BriskUsesExpCap() && level >= currentLevelCap)
        return 0;
    if (B_EXP_CAP_TYPE == EXP_CAP_NONE || GetBriskDifficulty() == BRISK_MODE_EASY)
        return expValue;

    if (level < currentLevelCap)
    {
        if (B_LEVEL_CAP_EXP_UP)
        {
            levelDifference = currentLevelCap - level;
            if (levelDifference > ARRAY_COUNT(sExpScalingUp) - 1)
                return expValue + (expValue / sExpScalingUp[ARRAY_COUNT(sExpScalingUp) - 1]);
            else
                return expValue + (expValue / sExpScalingUp[levelDifference]);
        }
        else
        {
            return expValue;
        }
    }
    else if (B_EXP_CAP_TYPE == EXP_CAP_HARD)
    {
        return 0;
    }
    else if (B_EXP_CAP_TYPE == EXP_CAP_SOFT)
    {
        levelDifference = level - currentLevelCap;
        if (levelDifference > ARRAY_COUNT(sExpScalingDown) - 1)
            return expValue / sExpScalingDown[ARRAY_COUNT(sExpScalingDown) - 1];
        else
            return expValue / sExpScalingDown[levelDifference];
    }
    else
    {
       return expValue;
    }
}

u32 GetCurrentEVCap(void)
{
    static const u16 sEvCapFlagMap[][2] = {
        // Define EV caps for each milestone
        {FLAG_BADGE01_GET, MAX_TOTAL_EVS *  1 / 17},
        {FLAG_BADGE02_GET, MAX_TOTAL_EVS *  3 / 17},
        {FLAG_BADGE03_GET, MAX_TOTAL_EVS *  5 / 17},
        {FLAG_BADGE04_GET, MAX_TOTAL_EVS *  7 / 17},
        {FLAG_BADGE05_GET, MAX_TOTAL_EVS *  9 / 17},
        {FLAG_BADGE06_GET, MAX_TOTAL_EVS * 11 / 17},
        {FLAG_BADGE07_GET, MAX_TOTAL_EVS * 13 / 17},
        {FLAG_BADGE08_GET, MAX_TOTAL_EVS * 15 / 17},
        {FLAG_IS_CHAMPION, MAX_TOTAL_EVS},
    };

    if (B_EV_CAP_TYPE == EV_CAP_FLAG_LIST)
    {
        for (u32 evCap = 0; evCap < ARRAY_COUNT(sEvCapFlagMap); evCap++)
        {
            if (!FlagGet(sEvCapFlagMap[evCap][0]))
                return sEvCapFlagMap[evCap][1];
        }
    }
    else if (B_EV_CAP_TYPE == EV_CAP_VARIABLE)
    {
        return VarGet(B_EV_CAP_VARIABLE);
    }
    else if (B_EV_CAP_TYPE == EV_CAP_NO_GAIN)
    {
        return 0;
    }

    return MAX_TOTAL_EVS;
}
