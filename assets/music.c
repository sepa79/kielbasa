#include <c64/vic.h>

#include <assets/music.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>

// Section and region for Title Screen and code to show it
#pragma section( mainGameMsx, 0 )
#pragma section( musicCode, 0 )
#pragma section( musicData, 0 )
#pragma section( radioMsx1, 0 )
// #pragma section( radioMsx2, 0 )
#pragma section( retroMsx1, 0 )
#pragma region( musicBank1, 0x8000, 0xbd00, , MUSIC_BANK, { mainGameMsx } )
#pragma region( radio01, 0x8000, 0xbd00, , MUSIC_BANK_RADIO_1, { radioMsx1 } )
// #pragma region( radio02, 0xa000, 0xbd00, , MUSIC_BANK_RADIO_1, { radioMsx2 } )
#pragma region( retro01, 0x8000, 0xa000, , MUSIC_BANK_RETRO_1, { retroMsx1 } )
// shared section - music loader
#pragma region( musicBank2, 0xbd00, 0xc000, , {MUSIC_BANK, MUSIC_BANK_RADIO_1, MUSIC_BANK_RETRO_1}, { musicCode, musicData } )

//++++++++++++++++++++++++++++++++
#pragma data ( mainGameMsx )
//++++++++++++++++++++++++++++++++

__export const char msxMain[] = {
    #embed 0xffff 136 "assets/music/FarmGame-1.sid"
};
__export const char msxFightIntro[] = {
    #embed 0xffff 136 "assets/music/FarmGame-2.sid"
};
__export const char msxAirDef[] = {
    #embed 0xffff 136 "assets/music/FarmGame-3.sid"
};
__export const char msxVMap[] = {
    #embed 0xffff 136 "assets/music/FarmGame-4.sid"
};

//++++++++++++++++++++++++++++++++
#pragma data ( radioMsx1 )
//++++++++++++++++++++++++++++++++

__export const char radioMsx01[] = {
    #embed 0xffff 136 "assets/music/FarmGameRadio1.sid"
};

// #pragma data ( radioMsx2 )
__export const char radioMsx02[] = {
    #embed 0xffff 136 "assets/music/FarmGameRadio2.sid"
};

//++++++++++++++++++++++++++++++++
#pragma data ( retroMsx1 )
//++++++++++++++++++++++++++++++++
__export const char retroMsx01[] = {
    #embed 0xffff 136 "assets/music/FarmGameRadioRetro.sid"
};

// store this song data in shared section, so we can safely switch banks to load SIDs and this data is always available
#pragma code ( musicCode )
#pragma data ( musicData )
// #pragma code ( code )
// #pragma data ( data )
__export const char* sidIndex[] ={
    msxMain,
    msxFightIntro,
    msxAirDef,
    msxVMap,

    radioMsx01,
    radioMsx02,

    retroMsx01
};

const char TXT_PLAYLIST_GM_NAME[] = s" Kielbasa music";
const char TXT_PLAYLIST_GM_S1[] = s"1 Take me for a dance";
const char TXT_PLAYLIST_GM_S2[] = s"2 For Sausage and Stonka";
const char TXT_PLAYLIST_GM_S3[] = s"3 Flack Attack";
const char TXT_PLAYLIST_GM_S4[] = s"4 Dare to Lair";
const char TXT_PLAYLIST_GM_S5[] = s"5 Forest Froth";
const char TXT_PLAYLIST_GM_S6[] = s"6 War of the Crops and Worlds";
const char TXT_PLAYLIST_RETRO_NAME[] = s" Retro Tributes";
const char TXT_PLAYLIST_RETRO_S1[] = s"1 Robbing Mr Hubbard";
const char TXT_PLAYLIST_RETRO_S2[] = s"2 From Bangladesh to Bankok";
const char TXT_PLAYLIST_RETRO_S3[] = s"3 Gray walls torn down";
const char TXT_PLAYLIST_RADIO_NAME[] = s" 90's Radio";
const char TXT_PLAYLIST_RADIO_S1[] = s"1 Kraftwerk - The Model";
const char TXT_PLAYLIST_RADIO_S2[] = s"2 Yazz - Don't Go";
const char TXT_PLAYLIST_RADIO_S3[] = s"3 Human League - Don't You Want Me?";
const char TXT_PLAYLIST_RADIO_S4[] = s"4 Depeche Mode - Everything Counts";
const char TXT_PLAYLIST_RADIO_S5[] = s"5 Donna Summer - I Feel Love";
const char TXT_PLAYLIST_RADIO_S6[] = s"6 OMD - Enola Gay";
const char TXT_PLAYLIST_RADIO_S7[] = s"7 Pink Floyd - A.B.I.T.W. Part 2";
const char TXT_PLAYLIST_RADIO_S8[] = s"8 Tom Robinson - Listen to the radio";
const char TXT_PLAYLIST_RADIO_S9[] = s"9 Eurythmics - Sweet Dreams";

const struct Song RADIO_PLAYLIST[RADIO_PLAYLIST_SIZE] = {
    { TXT_PLAYLIST_RADIO_S1, MUSIC_BANK_RADIO_1, 0, 4, sizeof(radioMsx01) },
    { TXT_PLAYLIST_RADIO_S2, MUSIC_BANK_RADIO_1, 1, 4, sizeof(radioMsx01) },
    { TXT_PLAYLIST_RADIO_S3, MUSIC_BANK_RADIO_1, 2, 4, sizeof(radioMsx01) },
    { TXT_PLAYLIST_RADIO_S4, MUSIC_BANK_RADIO_1, 3, 4, sizeof(radioMsx01) },
    { TXT_PLAYLIST_RADIO_S5, MUSIC_BANK_RADIO_1, 4, 4, sizeof(radioMsx01) },
    { TXT_PLAYLIST_RADIO_S6, MUSIC_BANK_RADIO_1, 5, 4, sizeof(radioMsx01) },
    { TXT_PLAYLIST_RADIO_S7, MUSIC_BANK_RADIO_1, 0, 5, sizeof(radioMsx02) },
    { TXT_PLAYLIST_RADIO_S8, MUSIC_BANK_RADIO_1, 1, 5, sizeof(radioMsx02) },
    { TXT_PLAYLIST_RADIO_S9, MUSIC_BANK_RADIO_1, 2, 5, sizeof(radioMsx02) }
};

const struct Song PLAYLIST[PLAYLIST_SIZE] = {
    { TXT_PLAYLIST_GM_NAME, TITLE_ONLY, TITLE_ONLY, TITLE_ONLY, 0 },
    { TXT_PLAYLIST_GM_S1, MUSIC_BANK, 0, 0, sizeof(msxMain) },
    { TXT_PLAYLIST_GM_S2, MUSIC_BANK, 1, 0, sizeof(msxMain) },
    { TXT_PLAYLIST_GM_S3, MUSIC_BANK, 0, 2, sizeof(msxAirDef) },
    { TXT_PLAYLIST_GM_S4, MUSIC_BANK, 0, 1, sizeof(msxFightIntro) },
    { TXT_PLAYLIST_GM_S5, MUSIC_BANK, 0, 3, sizeof(msxVMap) },
    { TXT_PLAYLIST_GM_S6, MUSIC_BANK, 1, 3, sizeof(msxVMap) },
    { TXT_PLAYLIST_RETRO_NAME, TITLE_ONLY, TITLE_ONLY, TITLE_ONLY, 0 },
    { TXT_PLAYLIST_RETRO_S1, MUSIC_BANK_RETRO_1, 0, 6, sizeof(retroMsx01) },
    { TXT_PLAYLIST_RETRO_S2, MUSIC_BANK_RETRO_1, 1, 6, sizeof(retroMsx01) },
    { TXT_PLAYLIST_RETRO_S3, MUSIC_BANK_RETRO_1, 2, 6, sizeof(retroMsx01) },
    // find a way to avoid duplicating these
    { TXT_PLAYLIST_RADIO_NAME, TITLE_ONLY, TITLE_ONLY, TITLE_ONLY, 0 },
    { TXT_PLAYLIST_RADIO_S1, MUSIC_BANK_RADIO_1, 0, 4, sizeof(radioMsx01) },
    { TXT_PLAYLIST_RADIO_S2, MUSIC_BANK_RADIO_1, 1, 4, sizeof(radioMsx01) },
    { TXT_PLAYLIST_RADIO_S3, MUSIC_BANK_RADIO_1, 2, 4, sizeof(radioMsx01) },
    { TXT_PLAYLIST_RADIO_S4, MUSIC_BANK_RADIO_1, 3, 4, sizeof(radioMsx01) },
    { TXT_PLAYLIST_RADIO_S5, MUSIC_BANK_RADIO_1, 4, 4, sizeof(radioMsx01) },
    { TXT_PLAYLIST_RADIO_S6, MUSIC_BANK_RADIO_1, 5, 4, sizeof(radioMsx01) },
    { TXT_PLAYLIST_RADIO_S7, MUSIC_BANK_RADIO_1, 0, 5, sizeof(radioMsx02) },
    { TXT_PLAYLIST_RADIO_S8, MUSIC_BANK_RADIO_1, 1, 5, sizeof(radioMsx02) },
    { TXT_PLAYLIST_RADIO_S9, MUSIC_BANK_RADIO_1, 2, 5, sizeof(radioMsx02) }
};

// this better be called when music IRQs are not trying to use the player...
void loadMusic(struct Song * song){
    // vic.color_back++;
    memcpy((volatile char*)MSX_DST_ADR, sidIndex[song->sidIdx], song->size);
    // int i = 0;
    // do {
    //     ((volatile char*) MSX_DST_ADR)[i] = (sidIndex[song->sidIdx])[i];
    //     i++;
    // } while (i < song->size);
}

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )