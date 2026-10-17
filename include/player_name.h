#ifndef GUARD_PLAYER_NAME_H
#define GUARD_PLAYER_NAME_H

#include "pokemon.h"
#include "main.h"

void OpenPlayerNameEditor(MainCallback returnCallback);
bool32 ChangePlayerName(const u8 *name);
bool32 RenamePlayerOwnedBoxMon(struct BoxPokemon *mon, const u8 *oldName, const u8 *newName);

#endif
