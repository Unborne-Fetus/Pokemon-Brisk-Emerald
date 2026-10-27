#include "global.h"
#include "battle.h"
#include "brisk_modes.h"
#include "brisk_trainer_moves.h"
#include "data.h"
#include "event_data.h"
#include "item.h"
#include "item_use.h"
#include "move.h"
#include "party_menu.h"
#include "pokeball.h"
#include "pokemon.h"
#include "trainer_util.h"
#include "test/test.h"
#include "constants/item_effects.h"
#include "constants/opponents.h"
#include "constants/trainers.h"

bool32 Test_BriskStoryTrainerId(u32 id);

TEST("Brisk all 28 ball items dispatch to the battle Bag handler in every mode while keeping field use")
{
    u16 mode = VarGet(VAR_BRISK_DIFFICULTY);
    u16 bag = VarGet(VAR_BRISK_BATTLE_BAG_DISABLED);
    VarSet(VAR_BRISK_BATTLE_BAG_DISABLED, 0);
    for (u32 m = 0; m < 4; m++)
    {
        VarSet(VAR_BRISK_DIFFICULTY, m);
        for (u32 ball = BALL_STRANGE; ball < POKEBALL_COUNT; ball++)
        {
            enum Item item = gPokeBalls[ball].itemId;
            EXPECT_EQ(GetItemBattleUsage(item), EFFECT_ITEM_THROW_BALL);
            EXPECT_EQ(GetItemBattleMenuType(item), ITEM_USE_BAG_MENU);
            EXPECT(BriskCanUseBattleItem(item));
        }
    }
    const enum Item fieldBalls[] = {ITEM_POKE_BALL, ITEM_GREAT_BALL, ITEM_ULTRA_BALL};
    for (u32 i = 0; i < ARRAY_COUNT(fieldBalls); i++)
    {
        EXPECT_EQ(GetItemType(fieldBalls[i]), ITEM_USE_FIELD);
        EXPECT(GetItemFieldFunc(fieldBalls[i]) == ItemUseOutOfBattle_OverworldBall);
    }
    EXPECT_EQ(GetItemBattleMenuType(ITEM_POTION), GetItemType(ITEM_POTION));
    VarSet(VAR_BRISK_DIFFICULTY, mode);
    VarSet(VAR_BRISK_BATTLE_BAG_DISABLED, bag);
}

TEST("Brisk Great and Ultra balls ignore Embargo like other balls and respect catching restrictions")
{
    struct BattlePokemon mons[MAX_BATTLERS_COUNT];
    memcpy(mons, gBattleMons, sizeof(mons));
    u32 flags = gBattleTypeFlags;
    u8 count = gPartiesCount[B_TRAINER_PLAYER];
    u8 absent = gAbsentBattlerFlags;
    u8 battlerCount = gBattlersCount;
    u8 positions[MAX_BATTLERS_COUNT];
    memcpy(positions, gBattlerPositions, sizeof(positions));
    u8 slot = gPartyMenu.slotId;
    u8 menuBattler = gBattlerInMenuId;
    u16 bag = VarGet(VAR_BRISK_BATTLE_BAG_DISABLED);
    u16 restriction = VarGet(B_VAR_NO_BAG_USE);
    bool32 noCatch = FlagGet(WE_FLAG_NO_CATCHING);
    memset(gBattleMons, 0, sizeof(mons));
    gBattleMons[0].hp = 100;
    gBattleMons[0].volatiles.embargoTimer = 3;
    gBattleMons[1].hp = 100;
    gAbsentBattlerFlags = 0;
    gBattlersCount = 2;
    for (u32 i = 0; i < MAX_BATTLERS_COUNT; i++) gBattlerPositions[i] = i;
    gPartiesCount[B_TRAINER_PLAYER] = 1;
    gBattleTypeFlags = 0;
    gPartyMenu.slotId = 0;
    gBattlerInMenuId = 0;
    VarSet(VAR_BRISK_BATTLE_BAG_DISABLED, 0);
    VarSet(B_VAR_NO_BAG_USE, NO_BAG_RESTRICTION);
    FlagClear(WE_FLAG_NO_CATCHING);
    const enum Item balls[] = {ITEM_POKE_BALL, ITEM_GREAT_BALL, ITEM_ULTRA_BALL, ITEM_MASTER_BALL};
    for (u32 i = 0; i < ARRAY_COUNT(balls); i++)
        EXPECT(!CannotUseItemsInBattle(balls[i], &gParties[B_TRAINER_PLAYER][0]));
    VarSet(VAR_BRISK_BATTLE_BAG_DISABLED, 1);
    EXPECT(CannotUseItemsInBattle(ITEM_ULTRA_BALL, &gParties[B_TRAINER_PLAYER][0]));
    VarSet(VAR_BRISK_BATTLE_BAG_DISABLED, 0);
    if (WE_FLAG_NO_CATCHING != 0)
    {
        FlagSet(WE_FLAG_NO_CATCHING);
        EXPECT(CannotUseItemsInBattle(ITEM_ULTRA_BALL, &gParties[B_TRAINER_PLAYER][0]));
    }
    memcpy(gBattleMons, mons, sizeof(mons));
    gBattleTypeFlags = flags;
    gPartiesCount[B_TRAINER_PLAYER] = count;
    gAbsentBattlerFlags = absent;
    gBattlersCount = battlerCount;
    memcpy(gBattlerPositions, positions, sizeof(positions));
    gPartyMenu.slotId = slot;
    gBattlerInMenuId = menuBattler;
    VarSet(VAR_BRISK_BATTLE_BAG_DISABLED, bag);
    VarSet(B_VAR_NO_BAG_USE, restriction);
    if (noCatch) FlagSet(WE_FLAG_NO_CATCHING); else FlagClear(WE_FLAG_NO_CATCHING);
}

TEST("Brisk story policy covers every rival variant and both Magma double opponents but excludes dedication and Easter eggs")
{
    const u16 ids[] = {TRAINER_MAY_ROUTE_110_MUDKIP, TRAINER_MAY_ROUTE_110_TREECKO,
        TRAINER_MAY_ROUTE_110_TORCHIC, TRAINER_MAY_ROUTE_119_MUDKIP,
        TRAINER_MAY_ROUTE_119_TREECKO, TRAINER_MAY_ROUTE_119_TORCHIC,
        TRAINER_BRENDAN_ROUTE_119_MUDKIP, TRAINER_BRENDAN_ROUTE_119_TREECKO,
        TRAINER_BRENDAN_ROUTE_119_TORCHIC, TRAINER_TABITHA_MT_CHIMNEY,
        TRAINER_MAXIE_MT_CHIMNEY, TRAINER_TABITHA_MOSSDEEP, TRAINER_MAXIE_MOSSDEEP,
        TRAINER_ROXANNE_1, TRAINER_WALLACE, TRAINER_SIDNEY};
    for (u32 i = 0; i < ARRAY_COUNT(ids); i++)
    {
        EXPECT(Test_BriskStoryTrainerId(ids[i]));
        struct TrainerGenerator generator = {0};
        MakeTrainerGenerator(&generator, &gTrainers[DIFFICULTY_NORMAL][ids[i]]);
        EXPECT(generator.storyLoadout);
        EXPECT(!generator.fairLoadout);
    }
    const u16 exempt[] = {TRAINER_DALTON_1, TRAINER_UNBORNE, TRAINER_JOEY, TRAINER_CYNTHIA, TRAINER_IRIS, TRAINER_RED2};
    for (u32 i = 0; i < ARRAY_COUNT(exempt); i++)
        EXPECT(!Test_BriskStoryTrainerId(exempt[i]));
}

TEST("Brisk story generation strengthens both opponents and preserves support gaps under Hard Extreme caps")
{
    u16 oldMode = VarGet(VAR_BRISK_DIFFICULTY);
    u16 oldRandom = VarGet(VAR_BRISK_RANDOMIZER);
    VarSet(VAR_BRISK_RANDOMIZER, 0);
    static const struct Trainer fixture = {.trainerClass = TRAINER_CLASS_MAGMA_LEADER, .trainerName = _("MAXIE")};
    const struct TrainerMon entries[] = {
        {.species = SPECIES_CAMERUPT, .lvl = 50, .gender = TRAINER_MON_RANDOM_GENDER, .moves = {MOVE_EMBER}, .nature = NATURE_MODEST, .ball = BALL_POKE},
        {.species = SPECIES_CROBAT, .lvl = 49, .gender = TRAINER_MON_RANDOM_GENDER, .moves = {MOVE_TACKLE}, .nature = NATURE_ADAMANT, .ball = BALL_POKE},
    };
    for (u32 mode = BRISK_MODE_HARD; mode <= BRISK_MODE_EXTREME; mode++)
    {
        VarSet(VAR_BRISK_DIFFICULTY, mode);
        for (u32 i = 0; i < ARRAY_COUNT(entries); i++)
        {
            struct TrainerGenerator generator = {0};
            MakeTrainerGenerator(&generator, &fixture);
            generator.storyLoadout = TRUE;
            generator.highestLevel = 50;
            struct Pokemon mon;
            GenerateMonFromTrainerMon(&mon, &entries[i], &generator);
            EXPECT_EQ(GetMonData(&mon, MON_DATA_LEVEL), ScaleBriskStoryTrainerLevel(entries[i].lvl, 50));
            EXPECT_EQ(GetMonData(&mon, MON_DATA_IVS), 0x3FFFFFFF);
            bool32 attack = FALSE;
            for (u32 slot = 0; slot < MAX_MON_MOVES; slot++)
            {
                enum Move move = GetMonData(&mon, MON_DATA_MOVE1 + slot);
                if (move == MOVE_NONE) continue;
                EXPECT(Test_BriskTrainerMoveLegal(entries[i].species, GetMonData(&mon, MON_DATA_LEVEL), move));
                if (GetMovePower(move) > 1) attack = TRUE;
            }
            EXPECT(attack);
        }
        EXPECT_LT(ScaleBriskStoryTrainerLevel(49, 50), ScaleBriskStoryTrainerLevel(50, 50));
    }
    VarSet(VAR_BRISK_DIFFICULTY, oldMode);
    VarSet(VAR_BRISK_RANDOMIZER, oldRandom);
}
