#include "global.h"
#include "contest_rhythm.h"
#include "main.h"
#include "menu.h"
#include "m4a.h"
#include "sound.h"
#include "text.h"
#include "window.h"
#include "constants/songs.h"

static EWRAM_DATA struct ContestRhythmState sRhythm = {0};
static EWRAM_DATA u8 sWindow = 0;
static EWRAM_DATA u8 sResultFrames = 0;
static EWRAM_DATA bool8 sActive = FALSE;
static EWRAM_DATA bool8 sResumeMusic = FALSE;

u32 ContestRhythm_PerfectWindow(u32 rank)
{
    static const u8 windows[] = {4, 3, 2, 2};
    return windows[min(rank, CONTEST_RANK_MASTER)];
}

u32 ContestRhythm_GoodWindow(u32 rank)
{
    static const u8 windows[] = {8, 7, 6, 5};
    return windows[min(rank, CONTEST_RANK_MASTER)];
}

void ContestRhythm_Init(struct ContestRhythmState *state, u32 rank, bool32 aHeld)
{
    *state = (struct ContestRhythmState){
        .rank = min(rank, CONTEST_RANK_MASTER),
        .grade = CONTEST_RHYTHM_PENDING,
        .released = !aHeld,
    };
}

bool32 ContestRhythm_Update(struct ContestRhythmState *state, bool32 aPressed, bool32 aHeld)
{
    if (state->finished)
        return TRUE;
    state->frame++;
    if (!aHeld)
        state->released = TRUE;
    // Only the first fresh press counts. Holding the move-selection button
    // into the performance cannot hit the note, and mashing cannot retry it.
    if (aPressed && state->released && !state->attempted)
    {
        s32 error = (s32)state->frame - CONTEST_RHYTHM_TARGET_FRAME;
        if (error < 0)
            error = -error;
        state->attempted = TRUE;
        state->grade = error <= ContestRhythm_PerfectWindow(state->rank)
            ? CONTEST_RHYTHM_PERFECT : error <= ContestRhythm_GoodWindow(state->rank)
            ? CONTEST_RHYTHM_GOOD : CONTEST_RHYTHM_MISS;
    }
    if (state->frame >= CONTEST_RHYTHM_TARGET_FRAME + ContestRhythm_GoodWindow(state->rank))
    {
        if (!state->attempted)
            state->grade = CONTEST_RHYTHM_MISS;
        state->finished = TRUE;
    }
    return state->finished;
}

u32 ContestRhythm_Bonus(u32 grade)
{
    if (grade == CONTEST_RHYTHM_PERFECT)
        return 20;
    if (grade == CONTEST_RHYTHM_GOOD)
        return 10;
    return 0;
}

u32 ContestRhythm_NpcGrade(u32 rank, u32 roll)
{
    static const u8 perfect[] = {15, 25, 35, 50};
    static const u8 good[] = {55, 65, 80, 90};
    rank = min(rank, CONTEST_RANK_MASTER);
    roll %= 100;
    return roll < perfect[rank] ? CONTEST_RHYTHM_PERFECT
        : roll < good[rank] ? CONTEST_RHYTHM_GOOD : CONTEST_RHYTHM_MISS;
}

bool32 ContestRhythm_IsEligible(const struct ContestantStatus *status, bool32 link)
{
    return !link && status->currMove != MOVE_NONE && !status->nervous
        && !status->noMoreTurns && status->numTurnsSkipped == 0;
}

void ContestRhythm_ApplyBonus(struct ContestantStatus *status, u32 grade, bool32 link)
{
    if (ContestRhythm_IsEligible(status, link))
        status->appeal = min(32767, (s32)status->appeal + (s32)ContestRhythm_Bonus(grade));
}

static void PrintRhythmText(const u8 *text)
{
    const u8 colors[] = {0, 1, 8}; // The existing contest dialogue palette.
    AddTextPrinterParameterized4(sWindow, FONT_SMALL, 0, 0, 0, 0, colors, TEXT_SKIP_DRAW, text);
}

static void DrawRhythm(void)
{
    const s32 left = 8, target = 120;
    s32 x = left + min(sRhythm.frame, CONTEST_RHYTHM_TARGET_FRAME + ContestRhythm_GoodWindow(sRhythm.rank)) * (target - left) / CONTEST_RHYTHM_TARGET_FRAME;
    u32 good = ContestRhythm_GoodWindow(sRhythm.rank) * (target - left) / CONTEST_RHYTHM_TARGET_FRAME;
    u32 perfect = max(1, ContestRhythm_PerfectWindow(sRhythm.rank) * (target - left) / CONTEST_RHYTHM_TARGET_FRAME);
    FillWindowPixelBuffer(sWindow, PIXEL_FILL(0));
    PrintRhythmText(COMPOUND_STRING("Press A on beat 4!"));
    FillWindowPixelRect(sWindow, PIXEL_FILL(8), left, 23, target - left + 1, 2);
    // Bounded target zone and a contrasting center line work without relying
    // on color alone. Three tick marks match the audible count-in.
    FillWindowPixelRect(sWindow, PIXEL_FILL(8), target - good, 18, good * 2 + 1, 11);
    FillWindowPixelRect(sWindow, PIXEL_FILL(1), target - perfect, 19, perfect * 2 + 1, 9);
    for (u32 beat = 1; beat <= 3; beat++)
    {
        s32 tick = left + beat * (target - left) / 4;
        FillWindowPixelRect(sWindow, PIXEL_FILL(1), tick, 20, 1, 7);
    }
    FillWindowPixelRect(sWindow, PIXEL_FILL(8), x - 2, 15, 5, 4);
    FillWindowPixelRect(sWindow, PIXEL_FILL(1), x - 1, 15, 3, 3);
    PutWindowTilemap(sWindow);
    CopyWindowToVram(sWindow, COPYWIN_FULL);
}

void ContestRhythm_Stop(void)
{
    if (!sActive)
        return;
    if (sResumeMusic)
        m4aMPlayContinue(&gMPlayInfo_BGM);
    sResumeMusic = FALSE;
    sActive = FALSE;
    FillWindowPixelBuffer(sWindow, PIXEL_FILL(0));
    CopyWindowToVram(sWindow, COPYWIN_GFX);
}

void ContestRhythm_Start(u32 windowId, u32 rank)
{
    ContestRhythm_Stop();
    sWindow = windowId;
    sResultFrames = 0;
    ContestRhythm_Init(&sRhythm, rank, JOY_HELD(A_BUTTON));
    sResumeMusic = (gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_TRACK)
        && !(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_PAUSE);
    if (sResumeMusic)
        m4aMPlayStop(&gMPlayInfo_BGM);
    sActive = TRUE;
    DrawRhythm();
}

bool32 ContestRhythm_Poll(void)
{
    if (!sActive)
        return TRUE;
    if (!sRhythm.finished)
    {
        ContestRhythm_Update(&sRhythm, JOY_NEW(A_BUTTON), JOY_HELD(A_BUTTON));
        if (sRhythm.frame % CONTEST_RHYTHM_BEAT_FRAMES == 0
         && sRhythm.frame <= CONTEST_RHYTHM_TARGET_FRAME)
            PlaySE(sRhythm.frame == CONTEST_RHYTHM_TARGET_FRAME ? SE_PIN : SE_SELECT);
        DrawRhythm();
        if (!sRhythm.finished)
            return FALSE;
    }
    if (sResultFrames == 0)
    {
        const u8 *text = sRhythm.grade == CONTEST_RHYTHM_PERFECT
            ? COMPOUND_STRING("PERFECT! +2 hearts") : sRhythm.grade == CONTEST_RHYTHM_GOOD
            ? COMPOUND_STRING("GOOD! +1 heart") : COMPOUND_STRING("MISS - normal appeal");
        FillWindowPixelBuffer(sWindow, PIXEL_FILL(0));
        PrintRhythmText(text);
        CopyWindowToVram(sWindow, COPYWIN_GFX);
        PlaySE(sRhythm.grade == CONTEST_RHYTHM_MISS ? SE_BOO : SE_SUCCESS);
    }
    if (++sResultFrames < 30)
        return FALSE;
    ContestRhythm_Stop();
    return TRUE;
}

u32 ContestRhythm_Result(void)
{
    return sRhythm.grade;
}
