#ifndef PIGSLE_CMD_IRQ_H
#define PIGSLE_CMD_IRQ_H
#include <miniGame/pigsleCmdMain.h>

#define IRQ_TOP_PLANE 1
#define IRQ_TOP_PESTS 40
#define IRQ_MIDDLE_PESTS 150
#define IRQ_CANNON 200
#define IRQ_FRAME_OPEN 50 + 200 - 3

#pragma code ( pigsleCommandRAMCode )
#pragma data ( pigsleCommandRAMData )

__interrupt void pigsleCmdIrq_topPlane();
__interrupt void pigsleCmdIrq_topPests();
__interrupt void pigsleCmdIrq_middlePests();
__interrupt void pigsleCmdIrq_cannonAnims();
__interrupt void pigsleCmdIrq_openBorder();

__interrupt void _pigsleCmdSpriteInit();

extern volatile byte crosshairBank;
// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )

#pragma compile("pigsleCmdIrq.c")

#endif