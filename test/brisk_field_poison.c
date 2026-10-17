#include "global.h"
#include "battle.h"
#include "field_poison.h"
#include "fldeff_misc.h"
#include "pokemon.h"
#include "task.h"
#include "constants/field_poison.h"
#include "test/test.h"

static void MakePoisonedMon(u32 slot, enum Species species, u8 abilitySlot, u16 hp, u32 status)
{
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][slot];
    CreateMonWithIVs(mon, species, 50, 0, OTID_STRUCT_PLAYER_ID, 31);
    SetMonData(mon, MON_DATA_ABILITY_NUM, &abilitySlot);
    SetMonData(mon, MON_DATA_HP, &hp);
    SetMonData(mon, MON_DATA_STATUS, &status);
}

TEST("Brisk walking Poison Heal restores one HP for regular and toxic poison without a damage flash")
{
    struct Pokemon saved[PARTY_SIZE];
    memcpy(saved, gParties[B_TRAINER_PLAYER], sizeof(saved));
    for (u32 toxic = 0; toxic < 2; toxic++)
    {
        ZeroPlayerPartyMons();
        ResetTasks();
        u32 status = toxic ? STATUS1_TOXIC_POISON | STATUS1_TOXIC_TURN(4) : STATUS1_POISON;
        MakePoisonedMon(0, SPECIES_BRELOOM, 1, 10, status);
        EXPECT_EQ(GetMonAbility(&gParties[B_TRAINER_PLAYER][0]), ABILITY_POISON_HEAL);
        EXPECT_EQ(DoPoisonFieldEffect(), FLDPSN_NONE);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP), 11);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_STATUS), status);
        EXPECT(!FldEffPoison_IsActive());
    }
    memcpy(gParties[B_TRAINER_PLAYER], saved, sizeof(saved));
}

TEST("Brisk walking Poison Heal cannot overheal or revive and ignores eggs")
{
    struct Pokemon saved[PARTY_SIZE];
    memcpy(saved, gParties[B_TRAINER_PLAYER], sizeof(saved));
    ZeroPlayerPartyMons();
    ResetTasks();
    MakePoisonedMon(0, SPECIES_BRELOOM, 1, 1, STATUS1_POISON);
    u16 maxHp = GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_MAX_HP);
    SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP, &maxHp);
    MakePoisonedMon(1, SPECIES_BRELOOM, 1, 0, STATUS1_POISON);
    MakePoisonedMon(2, SPECIES_BRELOOM, 1, 10, STATUS1_POISON);
    u8 egg = TRUE;
    SetMonData(&gParties[B_TRAINER_PLAYER][2], MON_DATA_IS_EGG, &egg);
    EXPECT_EQ(DoPoisonFieldEffect(), FLDPSN_NONE);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP), maxHp);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_HP), 0);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][2], MON_DATA_HP), 10);
    EXPECT(!FldEffPoison_IsActive());
    memcpy(gParties[B_TRAINER_PLAYER], saved, sizeof(saved));
}

TEST("Brisk walking Magic Guard blocks poison damage without clearing poison")
{
    struct Pokemon saved[PARTY_SIZE];
    memcpy(saved, gParties[B_TRAINER_PLAYER], sizeof(saved));
    ZeroPlayerPartyMons();
    ResetTasks();
    u32 status = STATUS1_TOXIC_POISON | STATUS1_TOXIC_TURN(3);
    MakePoisonedMon(0, SPECIES_CLEFABLE, 1, 1, status);
    EXPECT_EQ(GetMonAbility(&gParties[B_TRAINER_PLAYER][0]), ABILITY_MAGIC_GUARD);
    EXPECT_EQ(DoPoisonFieldEffect(), FLDPSN_NONE);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP), 1);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_STATUS), status);
    EXPECT(!FldEffPoison_IsActive());
    memcpy(gParties[B_TRAINER_PLAYER], saved, sizeof(saved));
}

TEST("Brisk walking poison immunity abilities clear poison and toxic counters")
{
    static const u16 cases[][3] = {
        {SPECIES_SNORLAX, 0, ABILITY_IMMUNITY},
        {SPECIES_PONYTA_GALAR, 1, ABILITY_PASTEL_VEIL},
        {SPECIES_KOMALA, 0, ABILITY_COMATOSE},
    };
    struct Pokemon saved[PARTY_SIZE];
    memcpy(saved, gParties[B_TRAINER_PLAYER], sizeof(saved));
    ZeroPlayerPartyMons();
    ResetTasks();
    for (u32 i = 0; i < ARRAY_COUNT(cases); i++)
    {
        MakePoisonedMon(i, cases[i][0], cases[i][1], 10, STATUS1_TOXIC_POISON | STATUS1_TOXIC_TURN(7));
        EXPECT_EQ(GetMonAbility(&gParties[B_TRAINER_PLAYER][i]), cases[i][2]);
    }
    EXPECT_EQ(DoPoisonFieldEffect(), FLDPSN_NONE);
    for (u32 i = 0; i < ARRAY_COUNT(cases); i++)
    {
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_HP), 10);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_STATUS), STATUS1_NONE);
    }
    EXPECT(!FldEffPoison_IsActive());
    memcpy(gParties[B_TRAINER_PLAYER], saved, sizeof(saved));
}

TEST("Brisk walking poison processes a mixed party and preserves normal damage and fainting")
{
    struct Pokemon saved[PARTY_SIZE];
    memcpy(saved, gParties[B_TRAINER_PLAYER], sizeof(saved));
    ZeroPlayerPartyMons();
    ResetTasks();
    MakePoisonedMon(0, SPECIES_BRELOOM, 1, 10, STATUS1_POISON);
    MakePoisonedMon(1, SPECIES_CLEFABLE, 1, 10, STATUS1_POISON);
    MakePoisonedMon(2, SPECIES_SNORLAX, 0, 10, STATUS1_POISON);
    MakePoisonedMon(3, SPECIES_KOMALA, 0, 10, STATUS1_POISON);
    MakePoisonedMon(4, SPECIES_WOBBUFFET, 0, 10, STATUS1_POISON);
    MakePoisonedMon(5, SPECIES_BRELOOM, 1, 10, STATUS1_POISON);
    EXPECT_EQ(DoPoisonFieldEffect(), FLDPSN_PSN);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP), 11);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_HP), 10);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][2], MON_DATA_STATUS), STATUS1_NONE);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][3], MON_DATA_STATUS), STATUS1_NONE);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][4], MON_DATA_HP), 9);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][5], MON_DATA_HP), 11);
    EXPECT(FldEffPoison_IsActive());
    ResetTasks();
    u16 hp = 1;
    SetMonData(&gParties[B_TRAINER_PLAYER][4], MON_DATA_HP, &hp);
    EXPECT_EQ(DoPoisonFieldEffect(), FLDPSN_FNT);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][4], MON_DATA_HP), 0);
    ResetTasks();
    EXPECT_EQ(DoPoisonFieldEffect(), FLDPSN_NONE);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][4], MON_DATA_HP), 0);
    EXPECT(!FldEffPoison_IsActive());
    memcpy(gParties[B_TRAINER_PLAYER], saved, sizeof(saved));
}
