#include "global.h"
#include "brisk_options.h"
#include "event_data.h"
#include "m4a.h"
#include "main.h"
#include "sound.h"
#include "task.h"
#include "test/test.h"
#include "constants/songs.h"

u8 Test_OpenBriskWorldOptions(void);
void Test_CloseBriskThemeOption(u8 taskId);

static const u16 sMuteFlags[] = {FLAG_BRISK_MUTE_SOUND, FLAG_BRISK_MUTE_MUSIC, FLAG_BRISK_MUTE_CRIES};

static void SetMuteMask(u32 mask)
{
    for (u32 i = 0; i < ARRAY_COUNT(sMuteFlags); i++)
        if (mask & (1u << i)) FlagSet(sMuteFlags[i]); else FlagClear(sMuteFlags[i]);
    RefreshBriskAudioMute();
}

static u32 GetMuteMask(void)
{
    return (BriskSoundMuted() ? 1 : 0) | (BriskMusicMuted() ? 2 : 0) | (BriskCriesMuted() ? 4 : 0);
}

static u32 MixedVolume(struct MusicPlayerInfo *player)
{
    struct MusicPlayerTrack track = {.flags = MPT_FLG_VOLCHG, .vol = 100, .volX = 64};
    TrkVolPitSet(player, &track);
    EXPECT_EQ(track.vol, 100);
    EXPECT_EQ(track.volX, 64); // Preserve the real gain so unmuting is lossless.
    return track.volMR + track.volML;
}

TEST("Brisk three audio mutes toggle independently and persist when reopening Options 7")
{
    u32 saved = GetMuteMask();
    SetMuteMask(0);
    u8 task = Test_OpenBriskWorldOptions();
    for (u32 i = 0; i < 3; i++)
    {
        gMain.newKeys = 0;
        gMain.newAndRepeatedKeys = DPAD_DOWN;
        RunTasks();
    }
    for (u32 i = 0; i < 3; i++)
    {
        gMain.newAndRepeatedKeys = 0;
        gMain.newKeys = A_BUTTON;
        RunTasks();
        EXPECT_EQ(GetMuteMask(), (1u << (i + 1)) - 1);
        gMain.newKeys = 0;
        gMain.newAndRepeatedKeys = DPAD_DOWN;
        RunTasks();
    }
    EXPECT_EQ(gTasks[task].data[0], 6); // CANCEL is still reachable on the last row.
    gMain.newAndRepeatedKeys = 0;
    Test_CloseBriskThemeOption(task);
    task = Test_OpenBriskWorldOptions();
    EXPECT_EQ(GetMuteMask(), 7);
    gMain.newAndRepeatedKeys = DPAD_UP;
    RunTasks();
    EXPECT_EQ(gTasks[task].data[0], 6);
    gMain.newAndRepeatedKeys = DPAD_DOWN;
    RunTasks();
    EXPECT_EQ(gTasks[task].data[0], 0);
    gMain.newAndRepeatedKeys = 0;
    Test_CloseBriskThemeOption(task);
    SetMuteMask(saved);
}

TEST("Brisk mixer separates music and fanfares from menu weather effects and both cry channels")
{
    u32 saved = GetMuteMask();
    m4aSongNumStart(MUS_LITTLEROOT);
    m4aSongNumStart(SE_RAIN);
    m4aSongNumStart(MUS_LEVEL_UP); // A music fanfare sharing the SE2 player.
    for (u32 mask = 0; mask < 8; mask++)
    {
        SetMuteMask(mask);
        EXPECT_EQ(MixedVolume(&gMPlayInfo_BGM) == 0, (mask & 2) != 0);
        EXPECT_EQ(MixedVolume(&gMPlayInfo_SE2) == 0, (mask & 2) != 0);
        EXPECT_EQ(MixedVolume(&gMPlayInfo_SE1) == 0, (mask & 1) != 0);
        EXPECT_EQ(MixedVolume(&gMPlayInfo_SE3) == 0, (mask & 1) != 0);
        for (u32 i = 0; i < MAX_POKEMON_CRIES; i++)
            EXPECT_EQ(MixedVolume(&gPokemonCryMusicPlayers[i]) == 0, (mask & 4) != 0);
    }
    m4aMPlayImmInit(&gMPlayInfo_SE2);
    m4aMPlayStop(&gMPlayInfo_SE2);
    PlaySE(SE_SELECT);
    SetMuteMask(1);
    EXPECT_EQ(MixedVolume(&gMPlayInfo_SE2), 0); // Same player, now a menu effect.
    SetMuteMask(2);
    EXPECT(MixedVolume(&gMPlayInfo_SE2) > 0);
    m4aMPlayImmInit(&gMPlayInfo_SE2);
    m4aMPlayStop(&gMPlayInfo_SE2);
    m4aSongNumStart(PH_TRAP_BLEND); // The Bard's voiced music is music too.
    SetMuteMask(2);
    EXPECT_EQ(MixedVolume(&gMPlayInfo_SE2), 0);
    SetMuteMask(1);
    EXPECT(MixedVolume(&gMPlayInfo_SE2) > 0);
    m4aMPlayStop(&gMPlayInfo_BGM);
    m4aMPlayStop(&gMPlayInfo_SE1);
    m4aMPlayStop(&gMPlayInfo_SE2);
    m4aMPlayStop(&gMPlayInfo_SE3);
    SetMuteMask(saved);
}

TEST("Brisk live mute and unmute recalculate active tracks without changing song position fades or wait status")
{
    u32 saved = GetMuteMask();
    SetMuteMask(0);
    m4aSongNumStart(MUS_LITTLEROOT);
    m4aMPlayImmInit(&gMPlayInfo_BGM);
    gMPlayInfo_BGM.tracks[0].flags = MPT_FLG_EXIST;
    u32 status = gMPlayInfo_BGM.status, clock = gMPlayInfo_BGM.clock;
    u16 fade = gMPlayInfo_BGM.fadeOV;
    SetMuteMask(2);
    EXPECT(gMPlayInfo_BGM.tracks[0].flags & MPT_FLG_VOLSET);
    EXPECT_EQ(MixedVolume(&gMPlayInfo_BGM), 0);
    EXPECT_EQ(gMPlayInfo_BGM.status, status);
    EXPECT_EQ(gMPlayInfo_BGM.clock, clock);
    EXPECT_EQ(gMPlayInfo_BGM.fadeOV, fade);
    SetMuteMask(0);
    EXPECT(MixedVolume(&gMPlayInfo_BGM) > 0);
    EXPECT_EQ(gMPlayInfo_BGM.status, status);
    EXPECT_EQ(gMPlayInfo_BGM.clock, clock);
    EXPECT_EQ(gMPlayInfo_BGM.fadeOV, fade);
    m4aMPlayStop(&gMPlayInfo_BGM);
    SetMuteMask(saved);
}
