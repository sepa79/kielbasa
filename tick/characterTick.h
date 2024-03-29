#ifndef CHARACTER_TICK_H
#define CHARACTER_TICK_H

//-----------------------------------------------------------------------------------------
#pragma code ( ticksCode )
#pragma data ( data )
//-----------------------------------------------------------------------------------------

void sleepTick();
void miaTick();
void regenTick();
void regenTickMinute();
void regenTickMapHour();

//-----------------------------------------------------------------------------------------
#pragma code ( code )
#pragma data ( data )
//-----------------------------------------------------------------------------------------
#pragma compile("characterTick.c")

#endif