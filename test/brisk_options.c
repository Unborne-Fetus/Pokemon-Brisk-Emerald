#include "global.h"
#include "battle.h"
#include "main.h"
#include "sprite.h"
#include "battle_setup.h"
#include "event_scripts.h"
#include "script.h"
#include "constants/opponents.h"
#include "constants/event_objects.h"
#include "ball_capsule.h"
#include "brisk_options.h"
#include "caps.h"
#include "event_data.h"
#include "pokemon.h"
#include "test/test.h"

TEST("Brisk options preserve defaults and toggle level caps without changing badges")
{
    VarSet(VAR_BRISK_NO_LEVEL_CAP, 0);
    u32 cap = GetProgressionLevelCap();
    bool32 badge = FlagGet(FLAG_BADGE01_GET);
    VarSet(VAR_BRISK_NO_LEVEL_CAP, 1);
    EXPECT_EQ(GetCurrentLevelCap(), MAX_LEVEL);
    EXPECT_EQ(GetProgressionLevelCap(), MAX_LEVEL);
    EXPECT_EQ(FlagGet(FLAG_BADGE01_GET), badge);
    VarSet(VAR_BRISK_NO_LEVEL_CAP, 0);
    EXPECT_EQ(GetProgressionLevelCap(), cap);
    VarSet(VAR_BRISK_SHINY_ODDS, 0);
    EXPECT_EQ(GetBriskShinyDenominator(), 512);
    EXPECT_EQ(GetBriskShinyThreshold(), 128);
}

TEST("Brisk shiny settings cover the requested range and preserve existing Pokemon")
{
    static const u16 odds[] = {8192, 4096, 2048, 1024, 512, 256, 128, 100, 50};
    struct Pokemon mon;
    CreateMon(&mon, SPECIES_PIKACHU, 20, 0, OTID_STRUCT_PRESET(0));
    for (u32 shiny = 0; shiny < 2; shiny++)
    {
        SetMonData(&mon, MON_DATA_IS_SHINY, &shiny);
        for (u32 i = 0; i < ARRAY_COUNT(odds); i++)
        {
            VarSet(VAR_BRISK_SHINY_ODDS, i + 1);
            EXPECT_EQ(GetBriskShinyDenominator(), odds[i]);
            EXPECT_EQ(GetBriskShinyThreshold(), (65536 + odds[i] / 2) / odds[i]);
            EXPECT_EQ(GetMonData(&mon, MON_DATA_IS_SHINY), shiny);
        }
    }
    VarSet(VAR_BRISK_SHINY_ODDS, 0);
}

TEST("Brisk candy learning retains final levels when every replacement is declined")
{
    struct Pokemon mon;
    VarSet(VAR_BRISK_NO_LEVEL_CAP, 1);
    CreateMon(&mon, SPECIES_BULBASAUR, 5, 0, OTID_STRUCT_PRESET(0));
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
        SetMonMoveSlot(&mon, MOVE_CELEBRATE, i);
    EXPECT(!ExecuteTableBasedItemEffect(&mon, ITEM_EXP_CANDY_M, 0, 0, TRUE, 10));
    u32 level = GetMonData(&mon, MON_DATA_LEVEL);
    u32 exp = GetMonData(&mon, MON_DATA_EXP);
    EXPECT(level > 5);
    u32 replacements = 0;
    for (u32 learnLevel = 6; learnLevel <= level; learnLevel++)
    {
        bool32 first = TRUE;
        enum Move result;
        do {
            result = MonTryLearningNewMoveAtLevel(&mon, first, learnLevel);
            first = FALSE;
            if (result == MON_HAS_MAX_MOVES)
                replacements++;
            EXPECT_EQ(GetMonData(&mon, MON_DATA_LEVEL), level);
        } while (result != MOVE_NONE);
    }
    EXPECT(replacements > 0);
    CalculateMonStats(&mon);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_LEVEL), level);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_EXP), exp);
    VarSet(VAR_BRISK_NO_LEVEL_CAP, 0);
}

TEST("Brisk Ball Capsules survive boxing and preserve held items and checksums")
{
    struct Pokemon mon, restored;
    CreateMon(&mon, SPECIES_PIKACHU, 20, 0, OTID_STRUCT_PRESET(0));
    u16 item = ITEM_LEFTOVERS;
    SetMonData(&mon, MON_DATA_HELD_ITEM, &item);
    for (u32 layout = 0; layout < 64; layout++)
    {
        u32 encoded = 0;
        for (u32 slot = 0; slot < BALL_CAPSULE_SLOTS; slot++)
            encoded = SetBallCapsuleSeal(encoded, slot, (layout >> (slot * 2)) & 3);
        EXPECT_EQ(encoded, layout);
        u8 capsule = encoded;
        SetMonData(&mon, MON_DATA_BALL_CAPSULE, &capsule);
        struct BoxPokemon boxed = mon.box;
        BoxMonToMon(&boxed, &restored);
        EXPECT_EQ(GetMonData(&restored, MON_DATA_BALL_CAPSULE), layout);
        EXPECT_EQ(GetMonData(&restored, MON_DATA_HELD_ITEM), item);
        EXPECT_EQ(GetMonData(&restored, MON_DATA_SANITY_IS_BAD_EGG), FALSE);
        EXPECT_EQ(GetMonData(&restored, MON_DATA_SPECIES), SPECIES_PIKACHU);
    }
}

static bool32 ContextContains(const struct ScriptContext *ctx, const u8 *script)
{
    if (ctx->scriptPtr == script)
        return TRUE;
    for (u32 i = 0; i < ctx->stackDepth; i++)
        if (ctx->stack[i] == script)
            return TRUE;
    return FALSE;
}

TEST("Brisk repeat trainers skip story continuations and never clear defeated flags")
{
    TrainerBattleParameter params = {0};
    params.params.opponentA = TRAINER_CALVIN_1;
    params.params.objEventLocalIdA = LOCALID_NONE;
    params.params.continueScript = TRUE;
    SetTrainerFlag(TRAINER_CALVIN_1);
    bool32 badge = FlagGet(FLAG_BADGE01_GET);
    for (u32 repeat = 0; repeat < 3; repeat++)
    {
        struct ScriptContext ctx = {0};
        ctx.scriptPtr = params.data;
        VarSet(VAR_BRISK_INFINITE_REMATCHES, 1);
        ConfigureTrainerBattle(&ctx);
        EXPECT(ContextContains(&ctx, EventSnippet_DoTrainerBattle));
        EXPECT(ContextContains(&ctx, EventSnippet_EndRepeatTrainerBattle));
        EXPECT(!ContextContains(&ctx, EventSnippet_GotoPostBattleScript));
        EXPECT(!ContextContains(&ctx, EventSnippet_EndTrainerBattle));
        EXPECT(HasTrainerBeenFought(TRAINER_CALVIN_1));
        EXPECT_EQ(FlagGet(FLAG_BADGE01_GET), badge);
    }
    VarSet(VAR_BRISK_INFINITE_REMATCHES, 0);
    struct ScriptContext ctx = {0};
    ctx.scriptPtr = params.data;
    ConfigureTrainerBattle(&ctx);
    EXPECT(ContextContains(&ctx, EventSnippet_GotoPostBattleScript));
    EXPECT(!ContextContains(&ctx, EventSnippet_DoTrainerBattle));
    ClearTrainerFlag(TRAINER_CALVIN_1);
}

TEST("Brisk Ball Capsule particles finish and handle a full sprite pool")
{
    gMain.inBattle = TRUE;
    gBattleTypeFlags = 0;
    gBattlersCount = 2;
    gBattlerPositions[0] = B_POSITION_PLAYER_LEFT;
    gBattlerPartyIndexes[0] = 0;
    CreateMon(&gParties[B_TRAINER_PLAYER][0], SPECIES_PIKACHU, 20, 0, OTID_STRUCT_PRESET(0));
    u8 capsule = 1 | (2 << 2) | (3 << 4);
    SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_BALL_CAPSULE, &capsule);
    ResetSpriteData();
    FreeAllSpritePalettes();
    PlayBallCapsule(0, 100, 80);
    u32 active = 0;
    for (u32 i = 0; i < MAX_SPRITES; i++)
        active += gSprites[i].inUse;
    EXPECT_EQ(active, 18);
    for (u32 frame = 0; frame < 50; frame++)
        AnimateSprites();
    for (u32 i = 0; i < MAX_SPRITES; i++)
        EXPECT(!gSprites[i].inUse);
    EXPECT_EQ(GetSpriteTileStartByTag(0x5F20), 0xFFFF);
    EXPECT_EQ(IndexOfSpritePaletteTag(0x5F20), 0xFF);
    for (u32 i = 0; i < MAX_SPRITES; i++)
        EXPECT(CreateSpriteUnchecked(&gDummySpriteTemplate, 0, 0, 0) < MAX_SPRITES);
    PlayBallCapsule(0, 100, 80);
    EXPECT_EQ(GetSpriteTileStartByTag(0x5F20), 0xFFFF);
    EXPECT_EQ(IndexOfSpritePaletteTag(0x5F20), 0xFF);
    ResetSpriteData();
    FreeAllSpritePalettes();
}
