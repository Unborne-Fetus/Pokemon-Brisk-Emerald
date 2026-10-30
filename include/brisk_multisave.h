#ifndef GUARD_BRISK_MULTISAVE_H
#define GUARD_BRISK_MULTISAVE_H

// Two independent in-ROM profiles. All 20 PC boxes belong to each profile.
// Old single-profile saves are imported into profile 0 on the first save.
void BriskMultiSave_Scan(void);
void BriskMultiSave_NoticeLegacy(u8 status);
bool8 BriskMultiSave_HasCompactCurrent(void);
u8 BriskMultiSave_CurrentSlot(void);
bool8 BriskMultiSave_IsActive(void);
bool8 BriskMultiSave_SlotOccupied(u8 slot);
bool8 BriskMultiSave_OtherSlotOccupied(void);
u8 BriskMultiSave_Select(u8 slot);
u8 BriskMultiSave_LoadCurrent(void);
u8 BriskMultiSave_Save(void);

#endif
