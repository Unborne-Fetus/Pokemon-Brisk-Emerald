#include "global.h"
#include "contest_rhythm.h"
#include "main.h"
#include "m4a.h"
#include "sound.h"
#include "constants/moves.h"
#include "constants/songs.h"
#include "test/test.h"

extern u8 Test_StartContestRhythmAppeal(u32 contestant, bool32 link, const struct ContestantStatus *status);
extern bool32 Test_StepContestRhythmAppeal(u8 task);
extern void Test_EndContestRhythmAppeal(u8 task, struct ContestantStatus *out);

static u32 GradeAt(u32 rank, u32 pressFrame)
{
    struct ContestRhythmState state;
    ContestRhythm_Init(&state, rank, FALSE);
    for (u32 frame = 1; frame <= CONTEST_RHYTHM_TARGET_FRAME + 10; frame++)
        if (ContestRhythm_Update(&state, frame == pressFrame, frame == pressFrame))
            break;
    EXPECT(state.finished);
    return state.grade;
}

TEST("Contest rhythm grades both edges of every rank's timing window")
{
    for (u32 rank = CONTEST_RANK_NORMAL; rank <= CONTEST_RANK_MASTER; rank++)
    {
        u32 target = CONTEST_RHYTHM_TARGET_FRAME;
        u32 perfect = ContestRhythm_PerfectWindow(rank), good = ContestRhythm_GoodWindow(rank);
        EXPECT_EQ(GradeAt(rank, target), CONTEST_RHYTHM_PERFECT);
        EXPECT_EQ(GradeAt(rank, target - perfect), CONTEST_RHYTHM_PERFECT);
        EXPECT_EQ(GradeAt(rank, target + perfect), CONTEST_RHYTHM_PERFECT);
        EXPECT_EQ(GradeAt(rank, target - perfect - 1), CONTEST_RHYTHM_GOOD);
        EXPECT_EQ(GradeAt(rank, target + perfect + 1), CONTEST_RHYTHM_GOOD);
        EXPECT_EQ(GradeAt(rank, target - good), CONTEST_RHYTHM_GOOD);
        EXPECT_EQ(GradeAt(rank, target + good), CONTEST_RHYTHM_GOOD);
        EXPECT_EQ(GradeAt(rank, target - good - 1), CONTEST_RHYTHM_MISS);
        EXPECT_EQ(GradeAt(rank, target + good + 1), CONTEST_RHYTHM_MISS);
        EXPECT_EQ(GradeAt(rank, 0), CONTEST_RHYTHM_MISS);
    }
}

TEST("Contest rhythm ignores a held selection button and locks the first fresh press")
{
    struct ContestRhythmState state;
    ContestRhythm_Init(&state, CONTEST_RANK_NORMAL, TRUE);
    for (u32 frame = 1; !state.finished; frame++)
        ContestRhythm_Update(&state, frame == 120, TRUE);
    EXPECT_EQ(state.grade, CONTEST_RHYTHM_MISS);
    EXPECT(!state.attempted);
    ContestRhythm_Init(&state, CONTEST_RANK_NORMAL, TRUE);
    for (u32 frame = 1; !state.finished; frame++)
        ContestRhythm_Update(&state, frame == 120, frame != 20);
    EXPECT_EQ(state.grade, CONTEST_RHYTHM_PERFECT);
    ContestRhythm_Init(&state, CONTEST_RANK_NORMAL, FALSE);
    for (u32 frame = 1; !state.finished; frame++)
        ContestRhythm_Update(&state, frame == 10 || frame == 120, frame == 10 || frame == 120);
    EXPECT_EQ(state.grade, CONTEST_RHYTHM_MISS);
    u32 finalFrame = state.frame;
    ContestRhythm_Update(&state, TRUE, TRUE);
    EXPECT_EQ(state.frame, finalFrame);
    EXPECT_EQ(state.grade, CONTEST_RHYTHM_MISS);
}

TEST("Rhythm bonuses preserve base appeal, combinations, jam, condition and repetition rules")
{
    struct ContestantStatus original = {
        .currMove = MOVE_TACKLE, .baseAppeal = 40, .appeal = 40,
        .jam = 20, .condition = 10, .usedComboMove = TRUE,
        .completedCombo = 2, .comboAppealBonus = 80,
        .repeatedMove = TRUE, .repeatJam = 20,
    };
    struct ContestantStatus scored = original;
    ContestRhythm_ApplyBonus(&scored, CONTEST_RHYTHM_PERFECT, FALSE);
    EXPECT_EQ(scored.appeal, 60);
    scored.appeal = original.appeal;
    EXPECT_EQ(memcmp(&scored, &original, sizeof(original)), 0);
    ContestRhythm_ApplyBonus(&scored, CONTEST_RHYTHM_GOOD, FALSE);
    EXPECT_EQ(scored.appeal, 50);
    scored = original;
    ContestRhythm_ApplyBonus(&scored, CONTEST_RHYTHM_MISS, FALSE);
    EXPECT_EQ(memcmp(&scored, &original, sizeof(original)), 0);
    scored.appeal = -30;
    ContestRhythm_ApplyBonus(&scored, CONTEST_RHYTHM_GOOD, FALSE);
    EXPECT_EQ(scored.appeal, -20);
    scored.appeal = 32760;
    ContestRhythm_ApplyBonus(&scored, CONTEST_RHYTHM_PERFECT, FALSE);
    EXPECT_EQ(scored.appeal, 32767);
}

TEST("Link contests, skipped turns, nervous Pokemon and empty moves never gain rhythm bonuses")
{
    struct ContestantStatus original = {.currMove = MOVE_TACKLE, .baseAppeal = 40, .appeal = 40};
    for (u32 reason = 0; reason < 5; reason++)
    {
        struct ContestantStatus status = original;
        if (reason == 0) status.nervous = TRUE;
        if (reason == 1) status.noMoreTurns = TRUE;
        if (reason == 2) status.numTurnsSkipped = 1;
        if (reason == 3) status.currMove = MOVE_NONE;
        EXPECT(!ContestRhythm_IsEligible(&status, reason == 4));
        ContestRhythm_ApplyBonus(&status, CONTEST_RHYTHM_PERFECT, reason == 4);
        EXPECT_EQ(status.appeal, 40);
    }
}

TEST("NPC rhythm accuracy improves by rank without exceeding the player's bonus")
{
    u32 previous = 0;
    for (u32 rank = CONTEST_RANK_NORMAL; rank <= CONTEST_RANK_MASTER; rank++)
    {
        u32 total = 0;
        for (u32 roll = 0; roll < 100; roll++)
        {
            u32 grade = ContestRhythm_NpcGrade(rank, roll);
            EXPECT(grade <= CONTEST_RHYTHM_PERFECT);
            EXPECT(ContestRhythm_Bonus(grade) <= 20);
            total += ContestRhythm_Bonus(grade);
        }
        EXPECT_GT(total, previous);
        previous = total;
    }
    EXPECT_EQ(ContestRhythm_PerfectWindow(255), ContestRhythm_PerfectWindow(CONTEST_RANK_MASTER));
}

TEST("Real contest appeal waits for timing, adds two hearts once, resumes music and proceeds to animation")
{
    u8 flags = gLinkContestFlags, player = gContestPlayerMonIndex;
    u16 rank = gSpecialVar_ContestRank;
    struct ContestantStatus status = {.currMove = MOVE_TACKLE, .baseAppeal = 40, .appeal = 40};
    PlayBGM(MUS_CONTEST);
    m4aSoundMain();
    gSpecialVar_ContestRank = CONTEST_RANK_NORMAL;
    gMain.newKeys = gMain.heldKeys = 0;
    u8 task = Test_StartContestRhythmAppeal(3, FALSE, &status);
    gContestPlayerMonIndex = 3;
    EXPECT(!Test_StepContestRhythmAppeal(task));
    EXPECT(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_PAUSE);
    u32 frames;
    for (frames = 1; frames <= 180; frames++)
    {
        gMain.newKeys = gMain.heldKeys = frames == 120 ? A_BUTTON : 0;
        if (Test_StepContestRhythmAppeal(task))
            break;
    }
    EXPECT(frames < 180);
    EXPECT_EQ(eContestantStatus[3].appeal, 60);
    EXPECT_EQ(eContestantStatus[3].baseAppeal, 40);
    EXPECT(!(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_PAUSE));
    Test_EndContestRhythmAppeal(task, &status);
    EXPECT_EQ(status.appeal, 60);
    m4aMPlayStop(&gMPlayInfo_BGM);
    gMain.newKeys = gMain.heldKeys = 0;
    gLinkContestFlags = flags;
    gContestPlayerMonIndex = player;
    gSpecialVar_ContestRank = rank;
}

TEST("Real contest appeal times out without a penalty; NPC and link turns proceed immediately")
{
    u8 flags = gLinkContestFlags, player = gContestPlayerMonIndex;
    gMain.newKeys = gMain.heldKeys = 0;
    struct ContestantStatus original = {.currMove = MOVE_TACKLE, .baseAppeal = 40, .appeal = 40};
    struct ContestantStatus status;
    u8 task = Test_StartContestRhythmAppeal(0, FALSE, &original);
    EXPECT(!Test_StepContestRhythmAppeal(task));
    u32 frames;
    for (frames = 0; frames < 180 && !Test_StepContestRhythmAppeal(task); frames++);
    EXPECT(frames < 180);
    Test_EndContestRhythmAppeal(task, &status);
    EXPECT_EQ(status.appeal, 40);
    task = Test_StartContestRhythmAppeal(1, FALSE, &original);
    EXPECT(Test_StepContestRhythmAppeal(task));
    Test_EndContestRhythmAppeal(task, &status);
    EXPECT(status.appeal >= 40 && status.appeal <= 60);
    task = Test_StartContestRhythmAppeal(0, TRUE, &original);
    EXPECT(Test_StepContestRhythmAppeal(task));
    Test_EndContestRhythmAppeal(task, &status);
    EXPECT_EQ(status.appeal, 40);
    gLinkContestFlags = flags;
    gContestPlayerMonIndex = player;
}
