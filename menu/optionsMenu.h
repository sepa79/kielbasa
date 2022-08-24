#ifndef OPTIONS_MENU_H
#define OPTIONS_MENU_H
// Switching code generation to shared section
#pragma code ( mainCode )
#pragma data ( mainData )

extern const struct MenuOption OPTIONS_MENU[];
void showOptionsMenu();

#pragma compile("optionsMenu.c")
#endif
