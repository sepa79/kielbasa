#ifndef MUSIC_H
#define MUSIC_H

// Section and region for Title Screen and code to show it
#pragma section( musicPlayer, 0 )
#pragma section( musicCode, 0 )
#pragma section( musicData, 0 )
#pragma section( radioMsx1, 0 )
#pragma region( musicBank1, 0x8000, 0xafff, , MUSIC_BANK, { musicPlayer } )
#pragma region( musicBank2, 0xb000, 0xbfff, , {MUSIC_BANK, MUSIC_BANK_RADIO_1}, { musicCode, musicData } )
#pragma region( radio01, 0x8000, 0xafff, , MUSIC_BANK_RADIO_1, { radioMsx1 } )

#pragma code ( musicData )
#pragma data ( musicData )

void loadMusic();

// how many songs in Radio 1 file
#define RADIO_1_SONGS 5

// Switching code generation back to shared section
#pragma code ( mainCode )
#pragma data ( mainData )

#pragma compile("music.c")

#endif
