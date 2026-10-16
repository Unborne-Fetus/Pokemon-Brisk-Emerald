#ifndef GUARD_BALL_CAPSULE_H
#define GUARD_BALL_CAPSULE_H

#define BALL_CAPSULE_SLOTS 3
#define BALL_CAPSULE_STYLES 39
#define SEND_OUT_CUSTOM_FIRST 8
#define SEND_OUT_CUSTOM_COUNT 3
#define SEND_OUT_TEXT_LENGTH 15
#define BALL_CAPSULE_TEXT_COUNT 11
const u8 *GetBallCapsuleText(u32 text);
const u8 *GetBallCapsuleSealName(u32 seal);
u8 *GetCustomSendOutText(u32 slot);
void PlayBallCapsule(enum BattlerId battler, s16 x, s16 y);
u32 GetBallCapsuleSeal(u32 capsule, u32 slot);
u32 SetBallCapsuleSeal(u32 capsule, u32 slot, u32 seal);

#endif
