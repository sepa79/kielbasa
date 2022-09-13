#ifndef PIGSLE_CMD_IRQ_H
#define PIGSLE_CMD_IRQ_H
#include <miniGame/pigsleCmdMain.h>

#define IRQ1RAS 50
#define IRQ2RAS IRQ1RAS + 152

#pragma code ( pigsleCommandRAMCode )
#pragma data ( pigsleCommandRAMData )

// void pigsleCmdIrq1();
// void pigsleCmdIrq2();
__interrupt void pigsleCmdIrq();
__interrupt void pigsleCmdIrq1_C();
__interrupt void pigsleCmdIrq2_C();

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )

#pragma compile("pigsleCmdIrq.c")

#endif