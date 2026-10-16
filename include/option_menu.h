#ifndef GUARD_OPTION_MENU_H
#define GUARD_OPTION_MENU_H

void CB2_InitOptionMenu(void);

#if TESTING
u8 Test_OpenBriskThemeOption(void);
void Test_CloseBriskThemeOption(u8 taskId);
#endif

#endif // GUARD_OPTION_MENU_H
