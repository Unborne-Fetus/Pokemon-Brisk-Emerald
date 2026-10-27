#include "global.h"
#include "battle.h"
#include "battle_util.h"
#include "brisk_modes.h"
#include "brisk_options.h"
#include "data.h"
#include "event_data.h"
#include "item.h"
#include "pokemon.h"
#include "test/test.h"
#include "constants/battle_ai.h"

TEST("Brisk difficulty purchase prices scale while resale values stay fixed")
{
    static const u8 multipliers[] = {2, 1, 4, 10};
    for (u32 mode = 0; mode < 4; mode++)
    {
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        EXPECT_EQ(GetItemPrice(ITEM_POTION), gItemsInfo[ITEM_POTION].price * multipliers[mode]);
        EXPECT_EQ(GetItemPrice(ITEM_RARE_CANDY), gItemsInfo[ITEM_RARE_CANDY].price * multipliers[mode]);
        EXPECT_EQ(GetItemSellPrice(ITEM_RARE_CANDY), gItemsInfo[ITEM_RARE_CANDY].price / ITEM_SELL_FACTOR);
        EXPECT_EQ(GetItemPrice(ITEM_MODERN_EXP_SHARE), 0);
    }
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
}

TEST("Brisk difficulty trainer AI progresses from conventional Easy to difficult Normal expert Hard and predictive Extreme")
{
    u64 original = AI_FLAG_BASIC_TRAINER | AI_FLAG_PREDICT_MOVE | AI_FLAG_RANDOMIZE_SWITCHIN;
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_EASY);
    EXPECT_EQ(GetBriskTrainerAiFlags(original), AI_FLAG_CHECK_BAD_MOVE);
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
    EXPECT(GetBriskTrainerAiFlags(original) & AI_FLAG_BASIC_TRAINER);
    EXPECT(!(GetBriskTrainerAiFlags(original) & (AI_FLAG_OMNISCIENT | AI_FLAG_PREDICTION)));
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_HARD);
    EXPECT(GetBriskTrainerAiFlags(original) & AI_FLAG_SMART_SWITCHING);
    EXPECT(GetBriskTrainerAiFlags(original) & AI_FLAG_OMNISCIENT);
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_EXTREME);
    EXPECT_EQ(GetBriskTrainerAiFlags(original) & AI_FLAG_PREDICTION, AI_FLAG_PREDICTION);
    EXPECT(GetBriskTrainerAiFlags(original) & AI_FLAG_KNOW_OPPONENT_PARTY);
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
}

TEST("Brisk Hard and Extreme enable move scoring for trainers with sparse legacy AI")
{
    const u64 originals[] = {0, AI_FLAG_CHECK_VIABILITY, AI_FLAG_FORCE_SETUP_FIRST_TURN};
    for (u32 i = 0; i < ARRAY_COUNT(originals); i++)
    {
        for (u32 mode = BRISK_MODE_NORMAL; mode <= BRISK_MODE_EXTREME; mode++)
        {
            VarSet(VAR_BRISK_DIFFICULTY, mode);
            u64 flags = GetBriskTrainerAiFlags(originals[i]);
            if (mode == BRISK_MODE_NORMAL)
                EXPECT_EQ(flags & AI_FLAG_BASIC_TRAINER, AI_FLAG_BASIC_TRAINER);
            else if (mode == BRISK_MODE_EASY)
                EXPECT_EQ(flags, AI_FLAG_CHECK_BAD_MOVE);
            else
            {
                EXPECT_EQ(flags & AI_FLAG_BASIC_TRAINER, AI_FLAG_BASIC_TRAINER);
                EXPECT(flags & AI_FLAG_SMART_MON_CHOICES);
                if (mode == BRISK_MODE_EXTREME)
                    EXPECT(flags & AI_FLAG_OMNISCIENT);
            }
        }
    }
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
}

TEST("Brisk difficulty enforces battle style without overwriting the player's preference")
{
    for (u32 mode = 0; mode < 4; mode++)
    {
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        for (u32 preference = 0; preference < 2; preference++)
        {
            u32 expected = mode == BRISK_MODE_EASY ? OPTIONS_BATTLE_STYLE_SHIFT : OPTIONS_BATTLE_STYLE_SET;
            EXPECT_EQ(GetBriskBattleStyle(preference), expected);
        }
    }
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
}

TEST("Brisk difficulty blocks battle consumables on Hard and Extreme but keeps catching")
{
    for (u32 mode = 0; mode < 4; mode++)
    {
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        bool32 allowed = mode == BRISK_MODE_NORMAL || mode == BRISK_MODE_EASY;
        EXPECT_EQ(BriskBattleItemsAllowed(), allowed);
        EXPECT_EQ(BriskCanUseBattleItem(ITEM_POTION), allowed);
        EXPECT_EQ(BriskCanUseBattleItem(ITEM_REVIVE), allowed);
        EXPECT_EQ(BriskCanUseBattleItem(ITEM_X_ATTACK), allowed);
        EXPECT(BriskCanUseBattleItem(ITEM_POKE_BALL));
        EXPECT(BriskCanUseBattleItem(ITEM_MASTER_BALL));
    }
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
}

TEST("Brisk EXP Share modes preserve legacy items and modern on off state")
{
    ClearBag();
    FlagSet(FLAG_RECEIVED_EXP_SHARE);
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
    VarSet(VAR_BRISK_EXP_SHARE, 0);
    AddBagItem(ITEM_EXP_SHARE, 1);
    SetBriskExpShareMode(TRUE);
    EXPECT(BriskModernExpShare());
    EXPECT(IsGen6ExpShareEnabled());
    EXPECT(CheckBagHasItem(ITEM_MODERN_EXP_SHARE, 1));
    EXPECT(CheckBagHasItem(ITEM_EXP_SHARE, 1));
    EXPECT_EQ(GetItemPocket(ITEM_MODERN_EXP_SHARE), POCKET_KEY_ITEMS);
    EXPECT(GetItemImportance(ITEM_MODERN_EXP_SHARE));
    EXPECT_EQ(GetItemPocket(ITEM_EXP_SHARE), POCKET_ITEMS);
    VarSet(VAR_BRISK_EXP_SHARE, 2);
    SetBriskExpShareMode(TRUE);
    EXPECT(!IsGen6ExpShareEnabled());
    EXPECT_EQ(VarGet(VAR_BRISK_EXP_SHARE), 2);
    SetBriskExpShareMode(FALSE);
    EXPECT(!BriskModernExpShare());
    EXPECT(CheckBagHasItem(ITEM_MODERN_EXP_SHARE, 1));
    FlagClear(FLAG_RECEIVED_EXP_SHARE);
    ClearBag();
}

TEST("Brisk EXP Share recipient rules disable both versions on Extreme")
{
    struct Pokemon holder, other;
    CreateMonWithIVs(&holder, SPECIES_PIKACHU, 20, 0, OTID_STRUCT_PRESET(0), 0);
    CreateMonWithIVs(&other, SPECIES_PIKACHU, 20, 0, OTID_STRUCT_PRESET(0), 0);
    u16 held = ITEM_EXP_SHARE;
    SetMonData(&holder, MON_DATA_HELD_ITEM, &held);
    ClearBag();
    AddBagItem(ITEM_MODERN_EXP_SHARE, 1);
    for (u32 mode = 0; mode < 4; mode++)
    {
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        VarSet(VAR_BRISK_EXP_SHARE, 0);
        EXPECT_EQ(BriskMonGetsExpShare(&holder), mode != BRISK_MODE_EXTREME);
        EXPECT(!BriskMonGetsExpShare(&other));
        VarSet(VAR_BRISK_EXP_SHARE, 1);
        EXPECT_EQ(BriskMonGetsExpShare(&holder), mode != BRISK_MODE_EXTREME);
        EXPECT_EQ(BriskMonGetsExpShare(&other), mode != BRISK_MODE_EXTREME);
        VarSet(VAR_BRISK_EXP_SHARE, 2);
        EXPECT(!BriskMonGetsExpShare(&holder));
        EXPECT(!BriskMonGetsExpShare(&other));
    }
    VarSet(VAR_BRISK_DIFFICULTY, BRISK_MODE_NORMAL);
    VarSet(VAR_BRISK_EXP_SHARE, 0);
    ClearBag();
}

TEST("Battle Bag OFF blocks consumables and ball shortcuts in wild and trainer battles")
{
    u16 saved = VarGet(VAR_BRISK_BATTLE_BAG_DISABLED);
    u32 flags = gBattleTypeFlags;
    VarSet(VAR_BRISK_BATTLE_BAG_DISABLED, 1);
    for (u32 trainer = 0; trainer < 2; trainer++)
    {
        gBattleTypeFlags = trainer ? BATTLE_TYPE_TRAINER : 0;
        EXPECT(!IsAllowedToUseBag());
        EXPECT(!BriskCanUseBattleItem(ITEM_POTION));
        EXPECT(!BriskCanUseBattleItem(ITEM_POKE_BALL));
    }
    VarSet(VAR_BRISK_BATTLE_BAG_DISABLED, saved);
    gBattleTypeFlags = flags;
}
