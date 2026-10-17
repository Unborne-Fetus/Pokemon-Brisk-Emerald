#include "global.h"
#include "bike.h"
#include "field_player_avatar.h"
#include "constants/metatile_behaviors.h"
#include "test/test.h"

static void SetBikeMode(u8 flags)
{
    gPlayerAvatar.flags = flags;
    gPlayerAvatar.runningState = NOT_MOVING;
    gPlayerAvatar.tileTransitionState = T_NOT_MOVING;
    gPlayerAvatar.acroBikeState = ACRO_STATE_NORMAL;
}

TEST("Brisk 2-in-1 bike switches either existing bike with L without using the DexNav button")
{
    struct PlayerAvatar original = gPlayerAvatar;
    SetBikeMode(PLAYER_AVATAR_FLAG_MACH_BIKE);
    EXPECT_EQ(GetBikeSwitchTarget(L_BUTTON, MB_NORMAL), PLAYER_AVATAR_FLAG_ACRO_BIKE);
    EXPECT_EQ(GetBikeSwitchTarget(L_BUTTON | A_BUTTON, MB_NORMAL), PLAYER_AVATAR_FLAG_ACRO_BIKE); // L=A still works.
    EXPECT_EQ(GetBikeSwitchTarget(R_BUTTON, MB_NORMAL), 0);
    EXPECT_EQ(GetBikeSwitchTarget(0, MB_NORMAL), 0);
    SetBikeMode(PLAYER_AVATAR_FLAG_ACRO_BIKE);
    EXPECT_EQ(GetBikeSwitchTarget(L_BUTTON, MB_NORMAL), PLAYER_AVATAR_FLAG_MACH_BIKE);
    SetBikeMode(PLAYER_AVATAR_FLAG_ON_FOOT);
    EXPECT_EQ(GetBikeSwitchTarget(L_BUTTON, MB_NORMAL), 0);
    SetBikeMode(PLAYER_AVATAR_FLAG_SURFING);
    EXPECT_EQ(GetBikeSwitchTarget(L_BUTTON, MB_NORMAL), 0);
    gPlayerAvatar = original;
}

TEST("Brisk 2-in-1 bike refuses mode changes while moving or performing Acro tricks")
{
    struct PlayerAvatar original = gPlayerAvatar;
    SetBikeMode(PLAYER_AVATAR_FLAG_MACH_BIKE);
    gPlayerAvatar.runningState = MOVING;
    EXPECT_EQ(GetBikeSwitchTarget(L_BUTTON, MB_NORMAL), 0);
    SetBikeMode(PLAYER_AVATAR_FLAG_ACRO_BIKE);
    gPlayerAvatar.acroBikeState = ACRO_STATE_BUNNY_HOP;
    EXPECT_EQ(GetBikeSwitchTarget(L_BUTTON, MB_NORMAL), 0);
    gPlayerAvatar = original;
}

TEST("Brisk 2-in-1 bike keeps Acro mode on every rail and bumpy slopes")
{
    struct PlayerAvatar original = gPlayerAvatar;
    SetBikeMode(PLAYER_AVATAR_FLAG_ACRO_BIKE);
    EXPECT_EQ(GetBikeSwitchTarget(L_BUTTON, MB_VERTICAL_RAIL), 0);
    EXPECT_EQ(GetBikeSwitchTarget(L_BUTTON, MB_HORIZONTAL_RAIL), 0);
    EXPECT_EQ(GetBikeSwitchTarget(L_BUTTON, MB_ISOLATED_VERTICAL_RAIL), 0);
    EXPECT_EQ(GetBikeSwitchTarget(L_BUTTON, MB_ISOLATED_HORIZONTAL_RAIL), 0);
    EXPECT_EQ(GetBikeSwitchTarget(L_BUTTON, MB_BUMPY_SLOPE), 0);
    gPlayerAvatar = original;
}
