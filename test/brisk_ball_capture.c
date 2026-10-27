#include "global.h"
#include "brisk_modes.h"
#include "event_data.h"
#include "pokeball.h"
#include "test/battle.h"

WILD_BATTLE_TEST("Brisk every ball reaches the throw animation and capture script in Hard and Extreme")
{
    enum Item item = ITEM_POKE_BALL;
    u32 mode = BRISK_MODE_HARD;
    for (u32 ball = BALL_STRANGE; ball < POKEBALL_COUNT; ball++)
        for (u32 m = BRISK_MODE_HARD; m <= BRISK_MODE_EXTREME; m++)
            PARAMETRIZE(item = gPokeBalls[ball].itemId, mode = m);
    GIVEN {
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        VarSet(VAR_BRISK_BATTLE_BAG_DISABLED, 0);
        FlagClear(WE_FLAG_NO_CATCHING);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_CATERPIE);
    } WHEN {
        TURN { USE_ITEM(player, item, WITH_RNG(RNG_BALLTHROW_SHAKE, 0)); }
    } SCENE {
        ONE_OF {
            ANIMATION(ANIM_TYPE_SPECIAL, B_ANIM_CRITICAL_CAPTURE_THROW);
            ANIMATION(ANIM_TYPE_SPECIAL, B_ANIM_BALL_THROW);
        }
    }
}
