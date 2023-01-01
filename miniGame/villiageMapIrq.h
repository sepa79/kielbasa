#ifndef VILLIAGE_MAP_IRQ_H
#define VILLIAGE_MAP_IRQ_H
#include <miniGame/villiageMapMain.h>

#define IRQ_MAP_TOP 1
#define IRQ_MAP_MSX2 200
#define IRQ_MAP_FRAME_OPEN 50 + 200 - 3

#pragma code ( villiageMapRAMCode )
#pragma data ( villiageMapRAMData )

__interrupt void villiageMapIrq_topMap();
__interrupt void villiageMapIrq_openBorder();
__interrupt void villiageMapIrq_msx2();

__interrupt void _villiageMapSpriteInit();

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )

#pragma compile("villiageMapIrq.c")

#endif