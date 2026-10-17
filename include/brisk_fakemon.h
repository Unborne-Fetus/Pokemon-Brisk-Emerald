#ifndef GUARD_BRISK_FAKEMON_H
#define GUARD_BRISK_FAKEMON_H
#include "pokemon.h"
#define IS_BRISK_FAKEMON(species) ((species) >= SPECIES_HISSIORITE && (species) <= SPECIES_NUENFLU)
u32 BriskFakemon_CopyDexPage(enum Species species, u32 page, u8 *out);
bool32 BriskFakemon_PrintDescription(enum Species species);
bool32 BriskFakemon_NextDexPage(void);
#endif
