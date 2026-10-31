#include "global.h"
#include "brisk_trainer_shinies.h"
#include "constants/opponents.h"
#include "test/test.h"

TEST("Brisk incidental trainer shinies are stable, sparse and never more than one per team")
{
    u32 count = 0;
    for (u32 trainerId = 1; trainerId <= 1200; trainerId++)
    {
        u8 chosen = BriskTrainerShinyPartySlot(trainerId, PARTY_SIZE);
        EXPECT_EQ(chosen, BriskTrainerShinyPartySlot(trainerId, PARTY_SIZE));
        if (chosen < PARTY_SIZE)
            count++;
        else
            EXPECT_EQ(chosen, PARTY_SIZE);
        for (u8 size = 1; size <= PARTY_SIZE; size++)
        {
            u8 index = BriskTrainerShinyPartySlot(trainerId, size);
            EXPECT(index == PARTY_SIZE || index < size);
        }
    }
    // Deliberately sparse, approximately 5% of identities.
    EXPECT(count >= 35 && count <= 90);
    EXPECT_EQ(BriskTrainerShinyPartySlot(TRAINER_NONE, PARTY_SIZE), PARTY_SIZE);
    EXPECT_EQ(BriskTrainerShinyPartySlot(3, 0), PARTY_SIZE);
}
