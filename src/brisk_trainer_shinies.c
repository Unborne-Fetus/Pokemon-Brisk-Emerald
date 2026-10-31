#include "global.h"
#include "brisk_trainer_shinies.h"

// Incidental opponent shinies: around one ordinary trainer in twenty.
// Hashing the trainer ID avoids correlated stretches of adjacent trainers,
// without consuming battle RNG or re-rolling on resets/rematches.
u8 BriskTrainerShinyPartySlot(u32 trainerId, u8 count)
{
    u32 hash;

    if (trainerId == TRAINER_NONE || count == 0 || count > PARTY_SIZE)
        return PARTY_SIZE;

    hash = trainerId ^ 0xA5A57D31u;
    hash ^= hash >> 16;
    hash *= 0x7FEB352Du;
    hash ^= hash >> 15;
    hash *= 0x846CA68Bu;
    hash ^= hash >> 16;

    if (hash % 20 != 0)
        return PARTY_SIZE;

    return (hash / 20) % count;
}
