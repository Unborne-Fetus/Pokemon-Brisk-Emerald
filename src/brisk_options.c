#include "global.h"
#include "brisk_options.h"
#include "event_data.h"

u32 GetBriskShinyDenominatorForSetting(u32 setting)
{
    static const u16 odds[BRISK_SHINY_CHOICES] = {8192, 4096, 2048, 1024, 512, 256, 128, 100, 50};
    // Zero keeps existing saves at Brisk's previous 1/512 baseline.
    return setting >= 1 && setting <= ARRAY_COUNT(odds) ? odds[setting - 1] : 512;
}

u32 GetBriskShinyDenominator(void)
{
    return GetBriskShinyDenominatorForSetting(VarGet(VAR_BRISK_SHINY_ODDS));
}

u32 GetBriskShinyThreshold(void)
{
    u32 denominator = GetBriskShinyDenominator();
    return (65536 + denominator / 2) / denominator;
}

bool32 BriskSkipMovePrompts(void) { return VarGet(VAR_BRISK_SKIP_MOVE_PROMPTS) != 0; }
bool32 BriskInfiniteRematches(void) { return VarGet(VAR_BRISK_INFINITE_REMATCHES) != 0; }
bool32 BriskShinyHunting(void) { return VarGet(VAR_BRISK_SHINY_HUNT) != 0; }
