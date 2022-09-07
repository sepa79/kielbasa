#ifndef PIGSLE_CMD_IRQ_H
#define PIGSLE_CMD_IRQ_H

#pragma section( pigsleCommandRAMCode, 0 )
#pragma section( pigsleCommandRAMData, 0 )
#pragma region( regionPigsleCommandMemC, 0xb000, 0xbfff, , MENU_BANK_PIGSLE_COMMAND_1, { pigsleCommandRAMCode, pigsleCommandRAMData }, 0x7000 )

#pragma code ( pigsleCommandRAMCode )
#pragma data ( pigsleCommandRAMData )

void pigsleCmdIrq1();
void pigsleCmdIrq2();

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )

#pragma compile("pigsleCmdIrq.c")

#endif