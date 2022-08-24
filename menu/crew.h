#ifndef EMPLOYMENT_H
#define EMPLOYMENT_H
// Switching code generation to shared section
#pragma code ( mainCode )
#pragma data ( mainData )

extern const struct MenuOption CREW_MENU[];

#pragma compile("crew.c")
#endif