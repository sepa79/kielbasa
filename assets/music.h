#ifndef MUSIC_H
#define MUSIC_H

// Section and region for Title Screen and code to show it
#pragma section( musicPlayer, 0 )
#pragma section( musicCode, 0 )
#pragma section( musicData, 0 )
#pragma region( musicBank1, 0x8000, 0xafff, , MUSIC_BANK, { musicPlayer } )
#pragma region( musicBank2, 0xb000, 0xbfff, , MUSIC_BANK, { musicCode, musicData } )

#pragma code ( musicData )
#pragma data ( musicData )

void loadMusic();

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )

#pragma compile("music.c")

#endif