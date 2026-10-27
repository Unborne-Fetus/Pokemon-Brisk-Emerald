#ifndef GUARD_BRISK_DIFFICULTY_H
#define GUARD_BRISK_DIFFICULTY_H
struct Pokemon;
struct TrainerGenerator;
u32 BriskDifficultyPartySize(u32 count, u32 highestLevel, bool32 story, bool32 league, bool32 tuned);
enum Species BriskEasyTrainerSpecies(enum Species species, u32 level);
void BriskExpandTrainerParty(struct Pokemon *party, u32 count, u32 target, struct TrainerGenerator *generator);
void BriskDifficultyHeldItem(struct Pokemon *mon);
bool32 BriskTrainerDifficultyExempt(u32 trainerId);
#endif
