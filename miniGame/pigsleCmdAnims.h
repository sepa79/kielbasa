#ifndef PIGSLE_CMD_ANIMS_H
#define PIGSLE_CMD_ANIMS_H

// Sections and regions
#pragma section( pigsleCommandAnimCode, 0 )
#pragma region( regionPigsleCommandAnim, 0x8000, 0xbfff, , MENU_BANK_PIGSLE_COMMAND_2, { pigsleCommandAnimCode } )

#pragma code ( pigsleCommandAnimCode )

void copyCannonUp();
void copyCannonL60();
void copyCannonR60();
void copyCannonL75();
void copyCannonR75();

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )

#pragma compile("pigsleCmdAnims.c")

#endif