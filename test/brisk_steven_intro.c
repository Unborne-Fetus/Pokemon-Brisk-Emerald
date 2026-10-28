#include "global.h"
#include "test/battle.h"
#include "sprite.h"

MULTI_BATTLE_TEST("Brisk Steven multi battle intro has graphics memory for all four trainers and Pokemon")
{
    FORCE_MOVE_ANIM(TRUE);
    GIVEN {
        PLAYER(SPECIES_CHARIZARD) { Moves(MOVE_CELEBRATE); }
        PARTNER(SPECIES_METAGROSS) { Moves(MOVE_CELEBRATE); }
        OPPONENT_A(SPECIES_CAMERUPT) { Moves(MOVE_CELEBRATE); }
        OPPONENT_B(SPECIES_CROBAT) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_CELEBRATE); MOVE(playerRight, MOVE_CELEBRATE); MOVE(opponentLeft, MOVE_CELEBRATE); MOVE(opponentRight, MOVE_CELEBRATE); }
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        EXPECT_EQ(gLoadFail, FALSE);
    }
}

#include "battle_gfx_sfx_util.h"
#include "battle_interface.h"
#include "battle_controllers.h"
#include "battle_util2.h"
#include "pokemon.h"
#include "task.h"
#include "main.h"
#include "constants/trainers.h"

TEST("Brisk Steven four trainer trays share graphics and send-outs wait for trainer tile release")
{
    gMain.inBattle = TRUE;
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI | BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_TWO_OPPONENTS;
    gBattlersCount = 4;
    ResetSpriteData(); FreeAllSpritePalettes(); ResetTasks();
    AllocateBattleResources(); AllocateBattleSpritesData(); AllocateMonSpritesGfx();
    for (u32 i=0;i<4;i++) { gBattlerPositions[i]=i; gBattlerPartyIndexes[i]=0; gPartiesCount[i]=6; CreateMon(&gParties[i][0], SPECIES_METAGROSS, 80, 0, OTID_STRUCT_PRESET(0)); }
    u8 state=0, battler=0;
    while (!BattleInitAllSprites(&state,&battler));
    const u16 pics[] = {TRAINER_PIC_BRENDAN,TRAINER_PIC_MAGMA_LEADER_MAXIE,TRAINER_PIC_STEVEN,TRAINER_PIC_MAGMA_ADMIN};
    for (u32 i=0;i<4;i++) BtlController_HandleDrawTrainerPic(i,pics[i], i&1,60,80,0);
    struct HpAndStatus status[6] = {0};
    gBattleSpritesDataPtr->healthBoxesData[0].partyStatusSummaryShown = TRUE;
    gBattlerStatusSummaryTaskId[0] = CreatePartyStatusSummarySprites(0,status,FALSE,TRUE);
    u32 freeAfterFirst = 0;
    while(freeAfterFirst < 1024 && CanAllocSpriteTiles(freeAfterFirst+1)) freeAfterFirst++;
    for (u32 i=1;i<4;i++)
    {
        gBattleSpritesDataPtr->healthBoxesData[i].partyStatusSummaryShown = TRUE;
        gBattlerStatusSummaryTaskId[i] = CreatePartyStatusSummarySprites(i,status,FALSE,TRUE);
    }
    u32 largest=0, used=0;
    while(largest<1024 && CanAllocSpriteTiles(largest+1)) largest++;
    for(u32 i=0;i<64;i++)used+=gSprites[i].inUse;
    EXPECT_EQ(largest, freeAfterFirst);
    EXPECT(used < MAX_SPRITES);
    EXPECT(CanAllocSpriteTiles(100));
    // Retail's fast intro can request a send-out before the trainer has left.
    // Do not allocate its Pokemon/ball while that picture still owns OBJ tiles.
    for (u32 i=0;i<4;i++)
    {
        gBattlerSpriteIds[i] = MAX_SPRITES;
        gBattleResources->bufferA[i][1] = 0;
        BtlController_HandleIntroTrainerBallThrow(i, 0, NULL, 0, BattleControllerDummy);
    }
    RunTasks();
    for (u32 i=0;i<4;i++) EXPECT_EQ(gBattlerSpriteIds[i], MAX_SPRITES);
    for (u32 frame=0;frame<85;frame++) { AnimateSprites(); RunTasks(); }
    for (u32 i=0;i<4;i++)
    {
        EXPECT_EQ(gBattleStruct->trainerSlideSpriteIds[i], MAX_SPRITES);
        EXPECT(gBattlerSpriteIds[i] < MAX_SPRITES);
    }
    EXPECT_EQ((u32)gBattleSpritesDataPtr->animationData->field_9_x1C, 0);
    EXPECT_EQ(GetSpriteTileStartByTag(TAG_STATUS_SUMMARY_BAR_TILE), TAG_NONE);
    // Mid-battle trays share the same resources; closing one cannot invalidate
    // another, and closing the final tray must release both sheets.
    for (u32 cycle = 0; cycle < 2; cycle++)
    {
        u8 first = CreatePartyStatusSummarySprites(1,status,FALSE,FALSE);
        u8 second = CreatePartyStatusSummarySprites(3,status,FALSE,FALSE);
        Task_HidePartyStatusSummary(first);
        for (u32 frame = 0; frame < 24; frame++) { AnimateSprites(); RunTasks(); }
        EXPECT(GetSpriteTileStartByTag(TAG_STATUS_SUMMARY_BAR_TILE) != TAG_NONE);
        EXPECT_EQ((u32)gBattleSpritesDataPtr->animationData->field_9_x1C, 1);
        Task_HidePartyStatusSummary(second);
        for (u32 frame = 0; frame < 24; frame++) { AnimateSprites(); RunTasks(); }
        EXPECT_EQ(GetSpriteTileStartByTag(TAG_STATUS_SUMMARY_BAR_TILE), TAG_NONE);
        EXPECT_EQ(GetSpriteTileStartByTag(TAG_STATUS_SUMMARY_BALLS_TILE), TAG_NONE);
        EXPECT_EQ((u32)gBattleSpritesDataPtr->animationData->field_9_x1C, 0);
    }
    EXPECT_EQ(gLoadFail, FALSE);
    ResetTasks(); ResetSpriteData(); FreeAllSpritePalettes();
    FreeMonSpritesGfx(); FreeBattleSpritesData(); FreeBattleResources(); gMain.inBattle=FALSE;
}
