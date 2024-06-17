#ifndef PIGSLE_CMD_MAIN_H
#define PIGSLE_CMD_MAIN_H

#include <menu/pigsleCommand.h>
// #pragma section( pigsleCommandLoaderData, 0 )
// #pragma section( pigsleCommandCode, 0 )
// #pragma section( pigsleCommandConsts, 0 )
// #pragma section( pigsleCommandRAMCode, 0 )
// #pragma section( pigsleCommandRAMData, 0 )
// #pragma region( regionPigsleCommandCrt, 0x8000, 0xafff, , MENU_BANK_PIGSLE_COMMAND_1, { pigsleCommandLoaderData, pigsleCommandCode, pigsleCommandConsts } )
// #pragma region( regionPigsleCommandRam, 0xb000, 0xbfff, , MENU_BANK_PIGSLE_COMMAND_1, { pigsleCommandRAMCode, pigsleCommandRAMData }, 0x7000 )

// #pragma section( pigsleCommandGfx1, 0 )
// #pragma section( pigsleCommandGfx1Loaders, 0 )
// #pragma region( regionPigsleCommandG1, 0x8000, 0xbfff, , MENU_BANK_PIGSLE_COMMAND_GFX_1, { pigsleCommandGfx1, pigsleCommandGfx1Loaders } )

#pragma code ( pigsleCommandRAMCode )
void pigsleScreenInit();
void pigsleSpriteLoader();
void pigsleCmdInit();

#pragma code ( pigsleCommandCode )
void gameState(GameState state);
void gameLoop();

#pragma compile("pigsleCmdMain.c")

#pragma code ( code )
#pragma data ( data )

#endif