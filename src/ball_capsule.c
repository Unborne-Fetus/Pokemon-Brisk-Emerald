#include "global.h"
#include "battle.h"
#include "ball_capsule.h"
#include "pokemon.h"
#include "sprite.h"
#include "trig.h"

#define TAG_CAPSULE 0x5F20

static const u8 sCapsuleGfx[] = INCGFX_U8("graphics/battle_anims/sprites/ball_capsule_seals.png", ".4bpp");
static const u16 sCapsulePal[] = INCGFX_U16("graphics/battle_anims/sprites/ball_capsule_seals.png", ".gbapal");
static const struct SpriteSheet sSheet = {sCapsuleGfx, sizeof(sCapsuleGfx), TAG_CAPSULE};
static const struct SpritePalette sPalette = {sCapsulePal, TAG_CAPSULE};
static const struct OamData sOam = {.shape = SPRITE_SHAPE(8x8), .size = SPRITE_SIZE(8x8), .priority = 1};
static const union AnimCmd sHeart[] = {ANIMCMD_FRAME(0, 1), ANIMCMD_END};
static const union AnimCmd sStar[] = {ANIMCMD_FRAME(1, 1), ANIMCMD_END};
static const union AnimCmd sBubble[] = {ANIMCMD_FRAME(2, 1), ANIMCMD_END};
static const union AnimCmd *const sAnims[] = {sHeart, sStar, sBubble};
static void SpriteCB_Capsule(struct Sprite *sprite);
static const struct SpriteTemplate sTemplate = {
    .tileTag = TAG_CAPSULE, .paletteTag = TAG_CAPSULE, .oam = &sOam,
    .anims = sAnims, .callback = SpriteCB_Capsule,
};
static EWRAM_DATA u16 sParticleCount = 0;

const u8 *GetBallCapsuleText(u32 text)
{
    static const u8 *const texts[] = {
        COMPOUND_STRING("NONE"), COMPOUND_STRING("Let's go!"),
        COMPOUND_STRING("Shine bright!"), COMPOUND_STRING("Showtime!"),
        COMPOUND_STRING("You can do it!"), COMPOUND_STRING("Make a splash!"),
        COMPOUND_STRING("Bring the heat!"), COMPOUND_STRING("Let's win!"),
    };
    return texts[text < BALL_CAPSULE_TEXT_COUNT ? text : 0];
}

u32 GetBallCapsuleSeal(u32 capsule, u32 slot)
{
    return slot < BALL_CAPSULE_SLOTS ? (capsule >> (slot * 2)) & 3 : 0;
}

u32 SetBallCapsuleSeal(u32 capsule, u32 slot, u32 seal)
{
    if (slot >= BALL_CAPSULE_SLOTS || seal >= BALL_CAPSULE_STYLES)
        return capsule & 63;
    return ((capsule & ~(3 << (slot * 2))) | (seal << (slot * 2))) & 63;
}

static void FreeCapsuleGfxIfUnused(void)
{
    if (sParticleCount == 0)
    {
        FreeSpriteTilesByTag(TAG_CAPSULE);
        FreeSpritePaletteByTag(TAG_CAPSULE);
    }
}

static void SpriteCB_Capsule(struct Sprite *sprite)
{
    s16 age = ++sprite->data[0];
    sprite->invisible = age < 0;
    if (age < 0)
        return;
    sprite->x2 = Sin(sprite->data[1], age * 2);
    sprite->y2 = Cos(sprite->data[1], age * 2) - age / 2;
    if (age > 30)
    {
        DestroySprite(sprite);
        if (sParticleCount)
            sParticleCount--;
        FreeCapsuleGfxIfUnused();
    }
}

void PlayBallCapsule(enum BattlerId battler, s16 x, s16 y)
{
    u32 capsule = GetMonData(GetBattlerMon(battler), MON_DATA_BALL_CAPSULE);
    if (capsule == 0)
        return;
    // Reset accounting after a battle's sprites have been cleared.
    bool32 active = FALSE;
    for (u32 i = 0; i < MAX_SPRITES; i++)
        if (gSprites[i].inUse && gSprites[i].callback == SpriteCB_Capsule)
            active = TRUE;
    if (!active)
        sParticleCount = 0;
    if (GetSpriteTileStartByTag(TAG_CAPSULE) == 0xFFFF && LoadSpriteSheet(&sSheet) == 0xFFFF)
        return;
    if (IndexOfSpritePaletteTag(TAG_CAPSULE) == 0xFF && LoadSpritePalette(&sPalette) == 0xFF)
    {
        FreeCapsuleGfxIfUnused();
        return;
    }
    for (u32 slot = 0; slot < BALL_CAPSULE_SLOTS; slot++)
    {
        u32 seal = GetBallCapsuleSeal(capsule, slot);
        if (seal == 0)
            continue;
        for (u32 i = 0; i < 6; i++)
        {
            u32 id = CreateSpriteUnchecked(&sTemplate, x, y, 28);
            if (id == MAX_SPRITES)
                continue;
            gSprites[id].data[0] = -(s16)(slot * 6);
            gSprites[id].data[1] = (i * 256 / 6 + slot * 14) & 255;
            StartSpriteAnim(&gSprites[id], seal - 1);
            sParticleCount++;
        }
    }
    FreeCapsuleGfxIfUnused();
}
