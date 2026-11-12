# Add Ball Capsules and custom send-out text

**Difficulty:** Advanced, **save-format sensitive**. This feature adds player-selected particles and phrases to Pokémon send-out animations without taking a held-item slot.

**Implementation:** [`include/ball_capsule.h`](../../../include/ball_capsule.h), [`src/ball_capsule.c`](../../../src/ball_capsule.c), [`include/pokemon.h`](../../../include/pokemon.h), [`src/pokemon.c`](../../../src/pokemon.c), [`src/party_menu.c`](../../../src/party_menu.c).  
**Behavior reference:** [`docs/ball_capsules.md`](../../ball_capsules.md).

## 1. Implement data handling before graphics

Brisk supports **three seal slots** with **39 values per slot** (including NONE, particles, letters and punctuation). Rather than three arrays, the logical configuration is a base-39 number:

~~~c
u32 GetBallCapsuleSeal(u32 capsule, u32 slot)
{
    if (slot >= BALL_CAPSULE_SLOTS || capsule >= 39 * 39 * 39)
        return 0;
    while (slot--)
        capsule /= BALL_CAPSULE_STYLES;
    return capsule % BALL_CAPSULE_STYLES;
}
~~~

`SetBallCapsuleSeal()` replaces one digit using weights 1, 39 and 39². Keep the getter and setter paired so changing slot two cannot overwrite slot one.

## 2. Protect Pokémon save structures

Brisk packs expanded values into carefully selected bits of its existing `BoxPokemon` substructures. The expanded values are exposed as `MON_DATA_BALL_CAPSULE_SEALS`; the old `MON_DATA_BALL_CAPSULE` remains understood for legacy saves.

**Never copy Brisk's bitfields onto another fork blindly.** They may be occupied, and changing a struct's byte size, checksum or encrypted substruct order can make old Pokémon unreadable. Verify a round-trip of every slot on both older and newer saves **before** adding user interface code. If your fork cannot reserve those bits safely, store capsule data in a separately versioned save extension.

Custom send-out text likewise uses an existing Brisk save area with a version/magic check in `GetCustomSendOutText()`. That "spare" area may not be free in a different engine.

## 3. Add the party editor

[`src/party_menu.c`](../../../src/party_menu.c) adds Party → Pokémon → ITEM → BALL CAPSULE and SEND-OUT TEXT. A user edits slots with directional input; **A commits** and **B discards**. Use a temporary copy while the menu is open so canceled changes never corrupt the Pokémon data.

## 4. Connect the battle send-out

[`PlayBallCapsule()`](../../../src/ball_capsule.c) reads `MON_DATA_BALL_CAPSULE_SEALS` for the Pokémon being sent out, loads particle graphics/palettes only when needed, and creates the desired sprite effects. Check the caller in your fork's battle controller and place the effect after the Pokémon sprite is ready, not during the menu or save write.

Budget sprite slots and palettes: a large number of particles must not crash doubles, partner battles, follower displays or battles with extra visual effects. Clean up graphics resources when sprites stop using them.

## 5. Test aggressively

- Old-format Pokémon with no capsule must still load correctly.
- All three slots round-trip through saving, boxing, trade/storage and reload.
- Canceling edits makes no change; changing one slot does not alter the others.
- Sending out from either side of singles/doubles works without flicker or crashes.
- Custom text is length-bounded, saves safely, and survives reboot.
- Run any encrypted-box-Pokémon checksum and layout tests your project provides.

For user-facing features and compatibility notes, see [Ball Capsules](../../ball_capsules.md).
