#ifndef GUARD_CONTEST_RHYTHM_H
#define GUARD_CONTEST_RHYTHM_H

#include "contest.h"

// 120 BPM at the game's native frame rate: three count-ins, then the action.
#define CONTEST_RHYTHM_BEAT_FRAMES 30
#define CONTEST_RHYTHM_TARGET_FRAME (4 * CONTEST_RHYTHM_BEAT_FRAMES)

enum ContestRhythmGrade
{
    CONTEST_RHYTHM_MISS,
    CONTEST_RHYTHM_GOOD,
    CONTEST_RHYTHM_PERFECT,
    CONTEST_RHYTHM_PENDING,
};

struct ContestRhythmState
{
    u16 frame;
    u8 rank;
    u8 grade;
    bool8 attempted;
    bool8 released;
    bool8 finished;
};

u32 ContestRhythm_PerfectWindow(u32 rank);
u32 ContestRhythm_GoodWindow(u32 rank);
void ContestRhythm_Init(struct ContestRhythmState *state, u32 rank, bool32 aHeld);
bool32 ContestRhythm_Update(struct ContestRhythmState *state, bool32 aPressed, bool32 aHeld);
u32 ContestRhythm_Bonus(u32 grade);
u32 ContestRhythm_NpcGrade(u32 rank, u32 roll);
bool32 ContestRhythm_IsEligible(const struct ContestantStatus *status, bool32 link);
void ContestRhythm_ApplyBonus(struct ContestantStatus *status, u32 grade, bool32 link);
void ContestRhythm_Start(u32 windowId, u32 rank);
bool32 ContestRhythm_Poll(void);
u32 ContestRhythm_Result(void);
void ContestRhythm_Stop(void);

#endif
