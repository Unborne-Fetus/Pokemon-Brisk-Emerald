#ifndef GUARD_BRISK_TRAINER_MOVES_H
#define GUARD_BRISK_TRAINER_MOVES_H

struct Pokemon;

void ApplyBriskTrainerLoadout(struct Pokemon *mon);
#if TESTING
s32 Test_BriskTrainerMoveScore(struct Pokemon *mon, enum Move move);
bool32 Test_BriskTrainerMoveLegal(enum Species species, u32 level, enum Move move);
#endif

#endif
