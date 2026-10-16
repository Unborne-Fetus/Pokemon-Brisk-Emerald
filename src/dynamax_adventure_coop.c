#include "global.h"
#include "dynamax_adventure_coop.h"

bool32 AdventureCoop_Init(struct AdventureCoopSession *session, const struct Pokemon *rentals, const struct Pokemon *boss)
{
    if (session == NULL || rentals == NULL || boss == NULL)
        return FALSE;
    if (GetMonData((struct Pokemon *)boss, MON_DATA_SPECIES) == SPECIES_NONE)
        return FALSE;
    for (u32 i = 0; i < DA_COOP_PLAYERS; i++)
        if (GetMonData((struct Pokemon *)&rentals[i], MON_DATA_SPECIES) == SPECIES_NONE)
            return FALSE;
    memset(session, 0, sizeof(*session));
    memcpy(session->rentals, rentals, sizeof(session->rentals));
    session->boss = *boss;
    session->round = 1;
    return TRUE;
}

bool32 AdventureCoop_Submit(struct AdventureCoopSession *session, u32 round, u32 player, u32 moveSlot, u32 target)
{
    if (session == NULL || session->round != round || round == 0 || player >= DA_COOP_PLAYERS
     || moveSlot >= MAX_MON_MOVES || target > DA_COOP_TARGET_BOSS || (session->submitted & (1u << player)))
        return FALSE;
    if (GetMonData(&session->boss, MON_DATA_HP) == 0 || GetMonData(&session->rentals[player], MON_DATA_HP) == 0
     || GetMonData(&session->rentals[player], MON_DATA_MOVE1 + moveSlot) == MOVE_NONE)
        return FALSE;
    session->actions[player] = (struct AdventureCoopAction){player, moveSlot, target};
    session->submitted |= 1u << player;
    return TRUE;
}

bool32 AdventureCoop_CollectRound(struct AdventureCoopSession *session, struct AdventureCoopAction *actions, u32 *count)
{
    if (session == NULL || actions == NULL || count == NULL || session->round == 0
     || GetMonData(&session->boss, MON_DATA_HP) == 0)
        return FALSE;
    u32 alive = 0;
    for (u32 i = 0; i < DA_COOP_PLAYERS; i++)
        if (GetMonData(&session->rentals[i], MON_DATA_HP) != 0)
            alive |= 1u << i;
    if (alive == 0 || (session->submitted & alive) != alive)
        return FALSE;
    *count = 0;
    for (u32 i = 0; i < DA_COOP_PLAYERS; i++)
        if (alive & (1u << i))
            actions[(*count)++] = session->actions[i];
    session->submitted = 0;
    session->round++;
    return TRUE;
}
