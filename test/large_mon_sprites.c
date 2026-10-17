#include "global.h"
#include "battle.h"
#include "battle_gfx_sfx_util.h"
#include "main.h"
#include "pokemon.h"
#include "sprite.h"
#include "test/test.h"

static void InitLargeSpriteTest(void)
{
    gMain.inBattle = TRUE;
    gBattleTypeFlags = BATTLE_TYPE_DOUBLE;
    gBattlersCount = MAX_BATTLERS_COUNT;
    for (u32 i = 0; i < MAX_BATTLERS_COUNT; i++)
        gBattlerPositions[i] = i;
    ResetSpriteData();
    AllocateMonSpritesGfx();
}

static void CreateTestMonSprite(enum BattlerId battler, enum Species species)
{
    SetMultiuseSpriteTemplateToPokemon(species, GetBattlerPosition(battler));
    gBattlerSpriteIds[battler] = CreateSprite(&gMultiuseSpriteTemplate, 100, 80, 0);
    UpdateBattleMonSubsprites(battler, species);
    gSprites[gBattlerSpriteIds[battler]].data[0] = battler;
}

TEST("Native Mega sprites support all native forms and both battle views")
{
    InitLargeSpriteTest();
    for (u32 species = SPECIES_AMOONGUSS_MEGA; species <= SPECIES_NINETALES_MEGA; species++)
    {
        if (species > SPECIES_WHIMSICOTT_MEGA && species < SPECIES_CLAWITZER_MEGA)
            continue;
        EXPECT(gSpeciesInfo[species].isMegaEvolution);
        EXPECT(gSpeciesInfo[species].frontPic80 != NULL);
        EXPECT(gSpeciesInfo[species].backPic80 != NULL);
        for (u32 position = 0; position < MAX_BATTLERS_COUNT; position++)
        {
            PrepareBattleMonSpriteImages(species, position);
            EXPECT_EQ(gMonSpritesGfxPtr->frameImages[position][0].size, LARGE_MON_PIC_SIZE);
            EXPECT_EQ(gMonSpritesGfxPtr->frameImages[position][1].data, gMonSpritesGfxPtr->frameImages[position][0].data);
            EXPECT_EQ(gMonSpritesGfxPtr->frameImages[position][0].data,
                      position & BIT_SIDE ? gSpeciesInfo[species].frontPic80 : gSpeciesInfo[species].backPic80);
        }
    }
    FreeMonSpritesGfx();
    gMain.inBattle = FALSE;
}

TEST("Native Mega sprites reserve tiles for all four battlers and return to normal graphics")
{
    InitLargeSpriteTest();
    for (u32 battler = 0; battler < MAX_BATTLERS_COUNT; battler++)
    {
        CreateTestMonSprite(battler, SPECIES_BUTTERFREE);
        struct Sprite *sprite = &gSprites[gBattlerSpriteIds[battler]];
        EXPECT_EQ((u32)sprite->subspriteMode, SUBSPRITES_OFF);
        UpdateBattleMonSubsprites(battler, SPECIES_BUTTERFREE_MEGA);
        EXPECT_EQ((u32)sprite->subspriteMode, SUBSPRITES_MON_80x80);
        EXPECT_EQ(sprite->images[0].size, LARGE_MON_PIC_SIZE);
        UpdateBattleMonSubsprites(battler, SPECIES_NONE); // Substitute.
        EXPECT_EQ((u32)sprite->subspriteMode, SUBSPRITES_OFF);
        EXPECT_EQ(sprite->images[0].data, gMonSpritesGfxPtr->spritesGfx[GetBattlerPosition(battler)]);
        for (u32 other = 0; other < battler; other++)
        {
            u32 a = sprite->oam.tileNum;
            u32 b = gSprites[gBattlerSpriteIds[other]].oam.tileNum;
            EXPECT(a + 100 <= b || b + 100 <= a);
        }
        UpdateBattleMonSubsprites(battler, SPECIES_BUTTERFREE_MEGA);
    }
    for (u32 battler = 0; battler < MAX_BATTLERS_COUNT; battler++)
    {
        FreeSpriteOamMatrix(&gSprites[gBattlerSpriteIds[battler]]);
        DestroySprite(&gSprites[gBattlerSpriteIds[battler]]);
    }
    FreeMonSpritesGfx();
    gMain.inBattle = FALSE;
}

TEST("Native Mega sprites keep affine pieces centered without matrix-number flips")
{
    InitLargeSpriteTest();
    CreateTestMonSprite(B_BATTLER_0, SPECIES_BUTTERFREE_MEGA);
    struct Sprite *sprite = &gSprites[gBattlerSpriteIds[B_BATTLER_0]];
    sprite->oam.affineMode = ST_OAM_AFFINE_DOUBLE;
    CalcCenterToCornerVec(sprite, sprite->oam.shape, sprite->oam.size, sprite->oam.affineMode);
    sprite->oam.matrixNum = 24; // Both bits would mean flips for a non-affine sprite.
    SetOamMatrix(24, 256, 0, 0, 256);
    u8 count = 0;
    sprite->oam.x = 100 + sprite->centerToCornerVecX;
    sprite->oam.y = 80 + sprite->centerToCornerVecY;
    AddSubspritesToOamBuffer(sprite, &count);
    EXPECT_EQ(count, 6);
    EXPECT_EQ((u32)gMain.oamBuffer[0].x, 28); // Center 100, piece center 92, double-width bound 64.
    EXPECT_EQ((u32)gMain.oamBuffer[0].y, 0);  // Center 80, piece center 64, double-height bound 64.
    EXPECT_EQ((u32)gMain.oamBuffer[5].x, 116);
    EXPECT_EQ((u32)gMain.oamBuffer[5].y, 88);
    EXPECT_EQ((u32)gMain.oamBuffer[5].tileNum, sprite->oam.tileNum + 96);
    count = 60;
    AddSubspritesToOamBuffer(sprite, &count);
    EXPECT_EQ(count, 66); // Composite pieces can cross the old 64-object cutoff.
    sprite->oam.matrixNum = 0;
    FreeSpriteOamMatrix(sprite);
    DestroySprite(sprite);
    FreeMonSpritesGfx();
    gMain.inBattle = FALSE;
}

TEST("Native Mega sprites erase every packed row on faint without modifying ROM")
{
    InitLargeSpriteTest();
    CreateTestMonSprite(B_BATTLER_0, SPECIES_BUTTERFREE_MEGA);
    struct Sprite *sprite = &gSprites[gBattlerSpriteIds[B_BATTLER_0]];
    const u8 *native = sprite->images[0].data;
    for (u32 row = 0; row < 10; row++)
        ClearBattleMonSpriteRow80(sprite, row);
    EXPECT(sprite->images[0].data != native);
    const u8 *erased = sprite->images[0].data;
    for (u32 i = 0; i < LARGE_MON_PIC_SIZE; i++)
        EXPECT_EQ(erased[i], 0);
    FreeSpriteOamMatrix(sprite);
    DestroySprite(sprite);
    FreeMonSpritesGfx();
    gMain.inBattle = FALSE;
}

TEST("Native Mega sprites rotate, reflect and shrink around the shared origin")
{
    // Hardware inverse matrices and expected centers for the 16x16 corner.
    static const s16 cases[][6] =
    {
        {256, 0, 0, 256, 32, 24},
        {-256, 0, 0, 256, -32, 24},
        {256, 0, 0, -256, 32, -24},
        {0, -256, 256, 0, 24, -32},
        {512, 0, 0, 512, 16, 12},
    };
    InitLargeSpriteTest();
    CreateTestMonSprite(B_BATTLER_0, SPECIES_BUTTERFREE_MEGA);
    struct Sprite *sprite = &gSprites[gBattlerSpriteIds[B_BATTLER_0]];
    sprite->oam.x = 100 + sprite->centerToCornerVecX;
    sprite->oam.y = 80 + sprite->centerToCornerVecY;
    for (u32 i = 0; i < ARRAY_COUNT(cases); i++)
    {
        u8 count = 0;
        SetOamMatrix(sprite->oam.matrixNum, cases[i][0], cases[i][1], cases[i][2], cases[i][3]);
        AddSubspritesToOamBuffer(sprite, &count);
        EXPECT_EQ(count, 6);
        EXPECT_EQ((u32)gMain.oamBuffer[5].x, 100 + cases[i][4] - 8);
        EXPECT_EQ((u32)gMain.oamBuffer[5].y, 80 + cases[i][5] - 8);
    }
    FreeSpriteOamMatrix(sprite);
    DestroySprite(sprite);
    FreeMonSpritesGfx();
    gMain.inBattle = FALSE;
}

TEST("Native Mega sprites use compatibility graphics in the summary template")
{
    InitLargeSpriteTest();
    CreateTestMonSprite(B_BATTLER_1, SPECIES_BUTTERFREE_MEGA);
    EXPECT_EQ(gMonSpritesGfxPtr->frameImages[B_POSITION_OPPONENT_LEFT][0].data, gSpeciesInfo[SPECIES_BUTTERFREE_MEGA].frontPic80);
    SetMultiuseSpriteTemplateToPokemon(SPECIES_BUTTERFREE_MEGA, B_POSITION_OPPONENT_LEFT);
    EXPECT_EQ(gMultiuseSpriteTemplate.images[0].data, gMonSpritesGfxPtr->spritesGfx[B_POSITION_OPPONENT_LEFT]);
    UpdateBattleMonSubsprites(B_BATTLER_1, SPECIES_BUTTERFREE_MEGA);
    EXPECT_EQ(gMonSpritesGfxPtr->frameImages[B_POSITION_OPPONENT_LEFT][0].data, gSpeciesInfo[SPECIES_BUTTERFREE_MEGA].frontPic80);
    FreeSpriteOamMatrix(&gSprites[gBattlerSpriteIds[B_BATTLER_1]]);
    DestroySprite(&gSprites[gBattlerSpriteIds[B_BATTLER_1]]);
    FreeMonSpritesGfx();
    gMain.inBattle = FALSE;
}
