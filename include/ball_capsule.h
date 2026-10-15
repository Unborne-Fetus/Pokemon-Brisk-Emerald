#ifndef GUARD_BALL_CAPSULE_H
#define GUARD_BALL_CAPSULE_H

#define BALL_CAPSULE_SLOTS 3
#define BALL_CAPSULE_STYLES 4
#define BALL_CAPSULE_TEXT_COUNT 8
const u8 *GetBallCapsuleText(u32 text);
void PlayBallCapsule(enum BattlerId battler, s16 x, s16 y);
u32 GetBallCapsuleSeal(u32 capsule, u32 slot);
u32 SetBallCapsuleSeal(u32 capsule, u32 slot, u32 seal);

#endif
