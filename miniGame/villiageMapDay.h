#ifndef VILLIAGE_MAP_DAY_H
#define VILLIAGE_MAP_DAY_H

// should not be needed, but without it code goes to main bank
#pragma code ( villiageMapDisplayCode )
#pragma data ( villiageMapRAMData )

// display routine, from MENU_BANK_MAP_VILLIAGE_2
void villiageMapDrawDay(const char * mp, char ox, char oy);

#pragma compile("villiageMapDay.c")

#endif