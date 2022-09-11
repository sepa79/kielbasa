#ifndef PIGSLE_CMD_IRQ_H
#define PIGSLE_CMD_IRQ_H
#include <miniGame/pigsleCmdMain.h>

#pragma code ( pigsleCommandRAMCode )
#pragma data ( pigsleCommandRAMData )

void pigsleCmdIrq1();
void pigsleCmdIrq2();

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )

#pragma compile("pigsleCmdIrq.c")

#endif