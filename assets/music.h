#ifndef MUSIC_H
#define MUSIC_H

// Section and region for Title Screen and code to show it
#pragma section( mainGameMsx, 0 )
#pragma section( musicCode, 0 )
#pragma section( musicData, 0 )
#pragma section( radioMsx1, 0 )
#pragma section( radioMsx2, 0 )
#pragma section( retroMsx1, 0 )
#pragma region( musicBank1, 0x8000, 0xa000, , MUSIC_BANK, { mainGameMsx } )
#pragma region( radio01, 0x8000, 0xa000, , MUSIC_BANK_RADIO_1, { radioMsx1 } )
#pragma region( radio02, 0xa000, 0xbd00, , MUSIC_BANK_RADIO_1, { radioMsx2 } )
#pragma region( retro01, 0x8000, 0xa000, , MUSIC_BANK_RETRO_1, { retroMsx1 } )
// shared section - music loader
#pragma region( musicBank2, 0xbd00, 0xc000, , {MUSIC_BANK, MUSIC_BANK_RADIO_1, MUSIC_BANK_RETRO_1}, { musicCode, musicData } )

#pragma code ( musicCode )
#pragma data ( musicData )

void loadMusic(char sidIdx);

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )

#pragma compile("music.c")

#endif