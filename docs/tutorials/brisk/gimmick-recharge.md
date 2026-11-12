# Add a reusable Tera Orb option (GIMMICK GEN)

**Difficulty:** Beginner-to-intermediate if your fork already has Terastallization. **Goal:** recharge an **owned** Tera Orb after completed battles, without granting a free item or allowing repeated activation *within* one battle.

**Implementation:** [`src/brisk_options.c`](../../../src/brisk_options.c), [`src/battle_util2.c`](../../../src/battle_util2.c), [`include/constants/flags.h`](../../../include/constants/flags.h), [`src/option_menu.c`](../../../src/option_menu.c).  
**Tests:** [`test/brisk_gimmick_gen.c`](../../../test/brisk_gimmick_gen.c).

## 1. Store the preference

Allocate a save-safe flag in **your** project, then add an Options toggle. Brisk's flag is `FLAG_BRISK_GIMMICK_GEN`. The saved default is OFF.

The accessor also checks an optional boss-rush override. If you don't have boss rush, use a simple `FlagGet(myFlag)` instead; don't paste a call to `BriskBossRushIsActive()` into a fork without that module.

## 2. Recharge only a real owned item

Brisk's function looks like this:

~~~c
void BriskRechargeGimmicks(void)
{
    if (BriskGimmickGenEnabled() && CheckBagHasItem(ITEM_TERA_ORB, 1))
        FlagSet(B_FLAG_TERA_ORB_CHARGED);
}
~~~

The key invariants are that the player **owns** the Orb, and the method only restores its existing charged state. It does **not** add an Orb or override the game mode's item-use rules.

## 3. Use end-of-battle cleanup, not every frame

In [`src/battle_util2.c`](../../../src/battle_util2.c), Brisk calls the recharge function when `gBattleOutcome != 0`, **before** freeing the battle resources. Recorded and recorded-link battles are excluded:

~~~c
if (gBattleOutcome != 0 &&
    !(gBattleTypeFlags & (BATTLE_TYPE_RECORDED | BATTLE_TYPE_RECORDED_LINK)))
    BriskRechargeGimmicks();
~~~

Adapt this to your battle engine's **completed encounter** lifecycle. Calling it too early can allow two Terastallizations in one battle; calling it in a replay can modify the player's actual save.

## 4. Recharge when enabling the option

Brisk's Options handler calls `BriskRechargeGimmicks()` immediately after the player turns GIMMICK GEN ON. This lets an already spent, already owned Orb become usable without requiring one extra battle.

## 5. Verify

Check winning, losing, fleeing, catching, draws, replay battles, and modes that disallow the Bag. Also confirm a missing Orb is never granted, turning OFF does not remove an existing charge, and Mega/Z usage does not suddenly ignore its ordinary per-battle limits.

Read the [GIMMICK GEN behavior reference](../../brisk_gimmick_gen.md) for the exact Brisk policy.
