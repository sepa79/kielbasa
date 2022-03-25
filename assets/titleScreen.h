#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H


// Section and region for Title Screen and code to show it
#pragma section( titleScreenGfx, 0 )
#pragma section( titleScreenCode, 0 )
#pragma section( titleScreenData, 0 )
#pragma region( bankTitleGfx, 0x8000, 0xafff, , TITLE_SCREEN_BANK, { titleScreenGfx } )
#pragma region( bankTitleCode, 0xb000, 0xbfff, , TITLE_SCREEN_BANK, { titleScreenCode, titleScreenData } )

#pragma code ( titleScreenCode )
#pragma data ( titleScreenData )

void loadAndShowTitleScreen();

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )

#pragma compile("titleScreen.c")

#endif