#ifndef GUARD_BRISK_BATTLE_FORMAT_H
#define GUARD_BRISK_BATTLE_FORMAT_H
#include "pokemon.h"
enum BriskBattleFormat { BRISK_BATTLE_REGULAR, BRISK_BATTLE_SINGLES, BRISK_BATTLE_DOUBLES };
u32 GetBriskBattleFormat(void);
bool32 BriskPokemonFollowersEnabled(void);
bool32 BriskBattleHadTwoOpponents(void);
void BriskPrepareBattleFormat(void);
void BriskPrepareBattleParties(void);
bool32 BriskRefillSinglesOpponent(void);
#endif
