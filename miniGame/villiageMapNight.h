#ifndef VILLIAGE_MAP_NIGHT_H
#define VILLIAGE_MAP_NIGHT_H

// should not be needed, but without it code goes to main bank
#pragma code ( villiageMapDisplayCode )
#pragma data ( villiageMapRAMData )

// display routine, from MENU_BANK_MAP_VILLIAGE_2
void villiageMapDrawNight(const char * mp, char ox, char oy);

#pragma compile("villiageMapNight.c")

#endif