#include "global.h"
#include "battle.h"
#include "ball_capsule.h"
#include "pokemon.h"
#include "sprite.h"
#include "trig.h"
#include "string_util.h"
#include "constants/characters.h"

#define TAG_CAPSULE 0x5F20

static const u8 sCapsuleGfx[] = INCGFX_U8("graphics/battle_anims/sprites/ball_capsule_seals.png", ".4bpp");
static const u16 sCapsulePal[] = INCGFX_U16("graphics/battle_anims/sprites/ball_capsule_seals.png", ".gbapal");
static const struct SpriteSheet sSheet = {sCapsuleGfx, sizeof(sCapsuleGfx), TAG_CAPSULE};
static const struct SpritePalette sPalette = {sCapsulePal, TAG_CAPSULE};
static const struct OamData sOam = {.shape = SPRITE_SHAPE(8x8), .size = SPRITE_SIZE(8x8), .priority = 1};
static const union AnimCmd sSeal0[] = {ANIMCMD_FRAME(0, 1), ANIMCMD_END};
static const union AnimCmd sSeal1[] = {ANIMCMD_FRAME(1, 1), ANIMCMD_END};
static const union AnimCmd sSeal2[] = {ANIMCMD_FRAME(2, 1), ANIMCMD_END};
static const union AnimCmd sSeal3[] = {ANIMCMD_FRAME(3, 1), ANIMCMD_END};
static const union AnimCmd sSeal4[] = {ANIMCMD_FRAME(4, 1), ANIMCMD_END};
static const union AnimCmd sSeal5[] = {ANIMCMD_FRAME(5, 1), ANIMCMD_END};
static const union AnimCmd sSeal6[] = {ANIMCMD_FRAME(6, 1), ANIMCMD_END};
static const union AnimCmd sSeal7[] = {ANIMCMD_FRAME(7, 1), ANIMCMD_END};
static const union AnimCmd sSeal8[] = {ANIMCMD_FRAME(8, 1), ANIMCMD_END};
static const union AnimCmd sSeal9[] = {ANIMCMD_FRAME(9, 1), ANIMCMD_END};
static const union AnimCmd sSeal10[] = {ANIMCMD_FRAME(10, 1), ANIMCMD_END};
static const union AnimCmd sSeal11[] = {ANIMCMD_FRAME(11, 1), ANIMCMD_END};
static const union AnimCmd sSeal12[] = {ANIMCMD_FRAME(12, 1), ANIMCMD_END};
static const union AnimCmd sSeal13[] = {ANIMCMD_FRAME(13, 1), ANIMCMD_END};
static const union AnimCmd sSeal14[] = {ANIMCMD_FRAME(14, 1), ANIMCMD_END};
static const union AnimCmd sSeal15[] = {ANIMCMD_FRAME(15, 1), ANIMCMD_END};
static const union AnimCmd sSeal16[] = {ANIMCMD_FRAME(16, 1), ANIMCMD_END};
static const union AnimCmd sSeal17[] = {ANIMCMD_FRAME(17, 1), ANIMCMD_END};
static const union AnimCmd sSeal18[] = {ANIMCMD_FRAME(18, 1), ANIMCMD_END};
static const union AnimCmd sSeal19[] = {ANIMCMD_FRAME(19, 1), ANIMCMD_END};
static const union AnimCmd sSeal20[] = {ANIMCMD_FRAME(20, 1), ANIMCMD_END};
static const union AnimCmd sSeal21[] = {ANIMCMD_FRAME(21, 1), ANIMCMD_END};
static const union AnimCmd sSeal22[] = {ANIMCMD_FRAME(22, 1), ANIMCMD_END};
static const union AnimCmd sSeal23[] = {ANIMCMD_FRAME(23, 1), ANIMCMD_END};
static const union AnimCmd sSeal24[] = {ANIMCMD_FRAME(24, 1), ANIMCMD_END};
static const union AnimCmd sSeal25[] = {ANIMCMD_FRAME(25, 1), ANIMCMD_END};
static const union AnimCmd sSeal26[] = {ANIMCMD_FRAME(26, 1), ANIMCMD_END};
static const union AnimCmd sSeal27[] = {ANIMCMD_FRAME(27, 1), ANIMCMD_END};
static const union AnimCmd sSeal28[] = {ANIMCMD_FRAME(28, 1), ANIMCMD_END};
static const union AnimCmd sSeal29[] = {ANIMCMD_FRAME(29, 1), ANIMCMD_END};
static const union AnimCmd sSeal30[] = {ANIMCMD_FRAME(30, 1), ANIMCMD_END};
static const union AnimCmd sSeal31[] = {ANIMCMD_FRAME(31, 1), ANIMCMD_END};
static const union AnimCmd sSeal32[] = {ANIMCMD_FRAME(32, 1), ANIMCMD_END};
static const union AnimCmd sSeal33[] = {ANIMCMD_FRAME(33, 1), ANIMCMD_END};
static const union AnimCmd sSeal34[] = {ANIMCMD_FRAME(34, 1), ANIMCMD_END};
static const union AnimCmd sSeal35[] = {ANIMCMD_FRAME(35, 1), ANIMCMD_END};
static const union AnimCmd sSeal36[] = {ANIMCMD_FRAME(36, 1), ANIMCMD_END};
static const union AnimCmd sSeal37[] = {ANIMCMD_FRAME(37, 1), ANIMCMD_END};
static const union AnimCmd *const sAnims[] = {sSeal0, sSeal1, sSeal2, sSeal3, sSeal4, sSeal5, sSeal6, sSeal7, sSeal8, sSeal9, sSeal10, sSeal11, sSeal12, sSeal13, sSeal14, sSeal15, sSeal16, sSeal17, sSeal18, sSeal19, sSeal20, sSeal21, sSeal22, sSeal23, sSeal24, sSeal25, sSeal26, sSeal27, sSeal28, sSeal29, sSeal30, sSeal31, sSeal32, sSeal33, sSeal34, sSeal35, sSeal36, sSeal37};
static void SpriteCB_Capsule(struct Sprite *sprite);
static const struct SpriteTemplate sTemplate = {
    .tileTag = TAG_CAPSULE, .paletteTag = TAG_CAPSULE, .oam = &sOam,
    .anims = sAnims, .callback = SpriteCB_Capsule,
};
static EWRAM_DATA u16 sParticleCount = 0;

u8 *GetCustomSendOutText(u32 slot)
{
    if (slot >= SEND_OUT_CUSTOM_COUNT)
        slot = 0;
    if (gSaveBlock1Ptr->sendOutTexts.magic != 0x534F5554)
    {
        memset(gSaveBlock1Ptr->sendOutTexts.phrases, EOS, sizeof(gSaveBlock1Ptr->sendOutTexts.phrases));
        gSaveBlock1Ptr->sendOutTexts.magic = 0x534F5554;
    }
    gSaveBlock1Ptr->sendOutTexts.phrases[slot][SEND_OUT_TEXT_LENGTH] = EOS;
    return gSaveBlock1Ptr->sendOutTexts.phrases[slot];
}

const u8 *GetBallCapsuleText(u32 text)
{
    static const u8 *const texts[] = {
        COMPOUND_STRING("NONE"), COMPOUND_STRING("Let's go!"),
        COMPOUND_STRING("Shine bright!"), COMPOUND_STRING("Showtime!"),
        COMPOUND_STRING("You can do it!"), COMPOUND_STRING("Make a splash!"),
        COMPOUND_STRING("Bring the heat!"), COMPOUND_STRING("Let's win!"),
    };
    if (text >= SEND_OUT_CUSTOM_FIRST && text < BALL_CAPSULE_TEXT_COUNT)
        return GetCustomSendOutText(text - SEND_OUT_CUSTOM_FIRST);
    return texts[text < ARRAY_COUNT(texts) ? text : 0];
}

const u8 *GetBallCapsuleSealName(u32 seal)
{
    static const u8 *const names[] = {
        COMPOUND_STRING("NONE"), COMPOUND_STRING("HEART"), COMPOUND_STRING("STAR"),
        COMPOUND_STRING("BUBBLES"), COMPOUND_STRING("LINE"), COMPOUND_STRING("SMOKE"),
        COMPOUND_STRING("ELECTRICITY"), COMPOUND_STRING("FIRE"), COMPOUND_STRING("CONFETTI"),
        COMPOUND_STRING("FLOWERS"), COMPOUND_STRING("MUSIC NOTE"),
    };
    static u8 letter[2];
    if (seal < ARRAY_COUNT(names))
        return names[seal];
    letter[0] = seal < 37 ? CHAR_A + seal - 11 : seal == 37 ? CHAR_EXCL_MARK : CHAR_QUESTION_MARK;
    letter[1] = EOS;
    return letter;
}

u32 GetBallCapsuleSeal(u32 capsule, u32 slot)
{
    if (slot >= BALL_CAPSULE_SLOTS || capsule >= 39 * 39 * 39)
        return 0;
    while (slot--)
        capsule /= BALL_CAPSULE_STYLES;
    return capsule % BALL_CAPSULE_STYLES;
}

u32 SetBallCapsuleSeal(u32 capsule, u32 slot, u32 seal)
{
    if (capsule >= 39 * 39 * 39)
        capsule = 0;
    if (slot >= BALL_CAPSULE_SLOTS || seal >= BALL_CAPSULE_STYLES)
        return capsule;
    u32 factor = slot == 0 ? 1 : slot == 1 ? 39 : 39 * 39;
    return capsule - GetBallCapsuleSeal(capsule, slot) * factor + seal * factor;
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
    switch (sprite->data[2])
    {
    case 5: // Smoke drifts and rises.
    case 7: // Fire rises quickly.
        sprite->x2 = Sin(sprite->data[1] + age * 4, age);
        sprite->y2 = -age * (sprite->data[2] == 7 ? 2 : 1);
        break;
    case 6: // Electric sparks jitter along their rays.
        sprite->x2 += (age & 2) ? 3 : -3;
        break;
    case 8: // Confetti falls after the burst.
    case 9: // Flowers flutter down.
        sprite->x2 += Sin(age * 12, 4);
        sprite->y2 += age * age / 32;
        break;
    case 10: // Music floats upward with a gentle wave.
        sprite->x2 = Sin(sprite->data[1] + age * 5, age);
        sprite->y2 = -age;
        break;
    default:
        break;
    }
    if (age > 36)
    {
        DestroySprite(sprite);
        if (sParticleCount)
            sParticleCount--;
        FreeCapsuleGfxIfUnused();
    }
}

void PlayBallCapsule(enum BattlerId battler, s16 x, s16 y)
{
    u32 capsule = GetMonData(GetBattlerMon(battler), MON_DATA_BALL_CAPSULE_SEALS);
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
            gSprites[id].data[2] = seal;
            gSprites[id].data[1] = (i * 256 / 6 + slot * 14) & 255;
            StartSpriteAnim(&gSprites[id], seal - 1);
            sParticleCount++;
        }
    }
    FreeCapsuleGfxIfUnused();
}
