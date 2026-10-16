#ifndef GUARD_DYNAMAX_ADVENTURE_COOP_H
#define GUARD_DYNAMAX_ADVENTURE_COOP_H

#include "pokemon.h"

// Dormant four-player command layer. No map, battle setup or network transport
// calls this yet; the current engine supports four battlers, not four + a boss.
#define DA_COOP_PLAYERS 4
#define DA_COOP_TARGET_BOSS DA_COOP_PLAYERS
#define DA_COOP_AVAILABLE FALSE

struct AdventureCoopAction
{
    u8 player;
    u8 moveSlot;
    u8 target; // 0..3 ally, 4 shared boss.
};

struct AdventureCoopSession
{
    struct Pokemon rentals[DA_COOP_PLAYERS];
    struct Pokemon boss;
    struct AdventureCoopAction actions[DA_COOP_PLAYERS];
    u32 round;
    u8 submitted;
};

bool32 AdventureCoop_Init(struct AdventureCoopSession *session, const struct Pokemon *rentals, const struct Pokemon *boss);
bool32 AdventureCoop_Submit(struct AdventureCoopSession *session, u32 round, u32 player, u32 moveSlot, u32 target);
bool32 AdventureCoop_CollectRound(struct AdventureCoopSession *session, struct AdventureCoopAction *actions, u32 *count);

#endif
