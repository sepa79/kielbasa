#ifndef PIG_PEN_H
#define PIG_PEN_H
// Switching code generation to shared section
#pragma code ( mainCode )
#pragma data ( mainData )

extern const struct MenuOption PIG_PEN_MENU[];

#pragma compile("pigPen.c")
#endif