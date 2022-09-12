// title screen doubles as main Init - loader uses this crt bank to store all temporary init procedures.

#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H

// Section and region for Title Screen and code to show it
#pragma section( titleScreenGfx, 0 )
#pragma section( titleScreenCRTCode, 0 )
#pragma section( gameInitRAMCode, 0 )
#pragma section( gameInitData, 0 )
#pragma region( bankTitleGfx, 0x8000, 0xafff, , TITLE_SCREEN_BANK, { titleScreenGfx } )
#pragma region( bankTitleCode1, 0xb000, 0xb1ff, , TITLE_SCREEN_BANK, { titleScreenCRTCode } )
#pragma region( bankTitleCode2, 0xb200, 0xbfff, , TITLE_SCREEN_BANK, { gameInitRAMCode, gameInitData }, 0x7000 )

#pragma code ( titleScreenCRTCode )
#pragma data ( gameInitData )

void loadTitleScreen();
void loadAndRunInit();

#pragma code ( gameInitRAMCode )
void initGame();

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )

#pragma compile("titleScreen.c")

#endif