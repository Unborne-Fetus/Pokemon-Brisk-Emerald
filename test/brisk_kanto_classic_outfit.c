#include "global.h"
#include "outfit_menu.h"
#include "test/test.h"
void UnlockJessieKantoClassicOutfit(void);
TEST("Jessie Kanto Classic outfit unlock is idempotent")
{
    gSaveBlock2Ptr->outfits[0]=0;
    UnlockOutfit(OUTFIT_USUAL_GREEN);
    EXPECT(!GetOutfitStatus(OUTFIT_KANTO_CLASSIC));
    UnlockJessieKantoClassicOutfit();
    EXPECT(GetOutfitStatus(OUTFIT_KANTO_CLASSIC));
    EXPECT(GetOutfitStatus(OUTFIT_USUAL_GREEN));
    UnlockJessieKantoClassicOutfit();
    EXPECT(GetOutfitStatus(OUTFIT_KANTO_CLASSIC));
}
TEST("Jessie outfit ID guard rejects invalid indices")
{
    gSaveBlock2Ptr->outfits[0]=0;
    UnlockOutfit(OUTFIT_COUNT);
    UnlockOutfit(OUTFIT_NONE);
    EXPECT(!GetOutfitStatus(OUTFIT_KANTO_CLASSIC));
}
