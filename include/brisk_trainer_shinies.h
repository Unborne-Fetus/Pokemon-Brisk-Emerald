#ifndef GUARD_BRISK_TRAINER_SHINIES_H
#define GUARD_BRISK_TRAINER_SHINIES_H

// Returns PARTY_SIZE if this trainer has no incidental shiny.
// Stable per encounter ID across difficulty modes, rematches and reloads.
u8 BriskTrainerShinyPartySlot(u32 trainerId, u8 count);

#endif // GUARD_BRISK_TRAINER_SHINIES_H
