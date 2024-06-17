#include <c64/vic.h>
#include <oscar.h>

#include <assets/music.h>
#include <engine/gameSettings.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>

// some msx is here
#include <menu/respawn.h>
#include <menu/pigsleIntro.h>

// Section and region for Title Screen and code to show it
#pragma section( mainGameMsx, 0 )
#pragma section( musicCode, 0 )
#pragma section( musicData, 0 )
#pragma section( radioMsx1, 0 )
#pragma section( radioMsx2, 0 )
#pragma section( retroMsx1, 0 )
#pragma region( musicBank1, 0x8000, 0xbc00, , MUSIC_BANK, { mainGameMsx } )
#pragma region( radio01, 0x8000, 0xc000, , MUSIC_BANK_RADIO_1, { radioMsx1 } )
#pragma region( radio02, 0x8000, 0xc000, , MUSIC_BANK_RADIO_2, { radioMsx2 } )
#pragma region( retro01, 0x8000, 0xa000, , MUSIC_BANK_RETRO_1, { retroMsx1 } )
// shared section - music loader
// #pragma region( musicBank2, 0xbc00, 0xc000, , {MUSIC_BANK, MUSIC_BANK_RADIO_1, MUSIC_BANK_RETRO_1}, { musicCode, musicData } )
#pragma region( musicBank2, 0xbc00, 0xc000, , {MUSIC_BANK}, { musicCode, musicData } )

//++++++++++++++++++++++++++++++++
#pragma data ( mainGameMsx )
//++++++++++++++++++++++++++++++++

__export const char msxTitle[] = {
    #embed 0xffff lzo "assets/music/8k_demoSong.sid"
};
__export const char msxMain[] = {
    #embed 0xffff 0x88 lzo "assets/music/FarmGame-1.sid"
};

// __export const char msxFightIntro[] = {
//     #embed 0xffff 0x88 lzo "assets/music/FarmGame-2.sid"
// };
__export const char msxAirDef[] = {
    #embed 0xffff 0x88 lzo "assets/music/FarmGame-3.sid"
};
__export const char msxVMap[] = {
    #embed 0xffff 0x88 lzo "assets/music/FarmGame-4.sid"
};

//++++++++++++++++++++++++++++++++
#pragma data ( radioMsx1 )
//++++++++++++++++++++++++++++++++

__export const char radioMsx1_1[] = {
    // #embed 0xffff 136 "assets/music/FarmGameRadio1.sid"
    #embed 0xffff 0x7e lzo "assets/music/8k_mjThriller.sid"
};
__export const char radioMsx1_2[] = {
    #embed 0xffff 0x7e lzo "assets/music/8k_tinaTheBest.sid"
};
__export const char radioMsx1_3[] = {
    #embed 0xffff 0x7e lzo "assets/music/8k_highwayToHell.sid"
};
// __export const char radioMsx1_4[] = {
//     #embed 0xffff 0x7e lzo "assets/music/8k_pinbalBDG.sid"
// };
// __export const char radioMsx1_5[] = {
//     #embed 0xffff 0x7e lzo "assets/music/8k_pinbalPartyland.sid"
// };

//++++++++++++++++++++++++++++++++
#pragma data ( radioMsx2 )
//++++++++++++++++++++++++++++++++
// __export const char radioMsx02[] = {
//     // #embed 0xffff 136 "assets/music/FarmGameRadio2.sid"
// };

__export const char radioMsx2_1[] = {
    #embed 0xffff 0x7e lzo "assets/music/8k_crocket.sid"
};
__export const char radioMsx2_2[] = {
    #embed 0xffff 0x88 lzo "assets/music/8k_mjBillieJean.sid"
};
// __export const char radioMsx2_2[] = {
//     #embed 0xffff 0x7e lzo "assets/music/8k_pinbalPartyland.sid"
// };

//++++++++++++++++++++++++++++++++
#pragma data ( retroMsx1 )
//++++++++++++++++++++++++++++++++
// __export const char retroMsx01[] = {
//     // #embed 0xffff 136 "assets/music/FarmGameRadioRetro.sid"
// };

// boombox and its fonts
__export const char boomboxScr[] = {
    #embed "assets/charGfx/boombox - MapArea.bin"
};
__export const char boomboxFnt[] = {
    #embed 0x400 0x400 "assets/charGfx/boombox - Chars.bin"
};
__export const char boomboxAtr[] = {
    #embed "assets/charGfx/boombox - CharAttribs_L1.bin"
};

//++++++++++++++++++++++++++++++++
#pragma data ( respawnMedia )
//++++++++++++++++++++++++++++++++
__export const char msxRespawn[] = {
    #embed 0xffff 0x7e lzo "assets/music/pigs_v01.sid"
};
//++++++++++++++++++++++++++++++++
#pragma data ( pigsleIntroMedia )
//++++++++++++++++++++++++++++++++
__export const char msxpigsleIntro[] = {
    #embed 0xffff 0x7e lzo "assets/music/8k_UFO-Enemy_Unknown.sid"
};

// this needs to be visible to msx loader in optionsMenu.c
#pragma code ( code )
#pragma data ( data )
__export const char* sidIndex[] ={
    msxTitle,
    msxMain,
    // msxFightIntro,
    msxAirDef,
    msxVMap,

    radioMsx1_1,
    radioMsx1_2,
    radioMsx1_3,

    radioMsx2_1,
    radioMsx2_2,

    msxRespawn,
    msxpigsleIntro,
};

// store this song data in main music section
#pragma code ( musicCode )
#pragma data ( musicData )

const char TXT_PLAYLIST_GM_NAME[] =     s" Kielbasa music                      ";
const char TXT_PLAYLIST_GM_S1[] =       s"1 Jeroen Tel - Demo Song             ";
const char TXT_PLAYLIST_GM_S2[] =       s"2 Crisps - For Sausage and Stonka    ";
const char TXT_PLAYLIST_GM_S3[] =       s"3 Mibri - Respawn theme              ";
const char TXT_PLAYLIST_GM_S4[] =       s"4 Nordishsound - UFO intro           ";
const char TXT_PLAYLIST_GM_S5[] =       s"5 Crisps - Flack Attack              ";
const char TXT_PLAYLIST_GM_S6[] =       s"6 Crisps - Forest Froth              ";
const char TXT_PLAYLIST_GM_S7[] =       s"7 C. - War of the Crops and Worlds   ";
// const char TXT_PLAYLIST_RETRO_NAME[] =  s" Retro Tributes                      ";
// const char TXT_PLAYLIST_RETRO_S1[] =    s"1 Robbing Mr Hubbard                 ";
// const char TXT_PLAYLIST_RETRO_S2[] =    s"2 From Bangladesh to Bankok          ";
// const char TXT_PLAYLIST_RETRO_S3[] =    s"3 Gray walls torn down               ";
const char TXT_PLAYLIST_RADIO_NAME[] =  s" 90's Radio - Nordishsound           ";
const char TXT_PLAYLIST_RADIO_S1[]   =  s"1 Michael Jackson - Thriller         ";
const char TXT_PLAYLIST_RADIO_S2[]   =  s"2 Tina Turner - The best             ";
const char TXT_PLAYLIST_RADIO_S3[]   =  s"3 AC/DC - Highway to Hell            ";
const char TXT_PLAYLIST_RADIO_S4[]   =  s"4 Jan Hammer - Crockett's Theme      ";
const char TXT_PLAYLIST_RADIO_S5[]   =  s"5 Michael Jackson - Billy Jean       ";
// const char TXT_PLAYLIST_RADIO_S1[] =    s"1 Kraftwerk - The Model              ";
// const char TXT_PLAYLIST_RADIO_S2[] =    s"2 Yazz - Don't Go                    ";
// const char TXT_PLAYLIST_RADIO_S3[] =    s"3 Human League - Don't You Want Me?  ";
// const char TXT_PLAYLIST_RADIO_S4[] =    s"4 Depeche Mode - Everything Counts   ";
// const char TXT_PLAYLIST_RADIO_S5[] =    s"5 Donna Summer - I Feel Love         ";
// const char TXT_PLAYLIST_RADIO_S6[] =    s"6 OMD - Enola Gay                    ";
// const char TXT_PLAYLIST_RADIO_S7[] =    s"7 Pink Floyd - A.B.I.T.W. Part 2     ";
// const char TXT_PLAYLIST_RADIO_S8[] =    s"8 Tom Robinson - Listen to the radio ";
// const char TXT_PLAYLIST_RADIO_S9[] =    s"9 Eurythmics - Sweet Dreams          ";

const struct Song RADIO_PLAYLIST[RADIO_PLAYLIST_SIZE] = {
    { TXT_PLAYLIST_RADIO_S1, MUSIC_BANK_RADIO_1, 0, 4, SPEED_1X },
    { TXT_PLAYLIST_RADIO_S2, MUSIC_BANK_RADIO_1, 0, 5, SPEED_1X },
    { TXT_PLAYLIST_RADIO_S3, MUSIC_BANK_RADIO_1, 0, 6, SPEED_1X },
    { TXT_PLAYLIST_RADIO_S4, MUSIC_BANK_RADIO_2, 0, 7, SPEED_1X },
    { TXT_PLAYLIST_RADIO_S5, MUSIC_BANK_RADIO_2, 0, 8, SPEED_2X },
    // { TXT_PLAYLIST_RADIO_S1, MUSIC_BANK_RADIO_1, 0, 4, sizeof(radioMsx01) },
    // { TXT_PLAYLIST_RADIO_S2, MUSIC_BANK_RADIO_1, 1, 4, sizeof(radioMsx01) },
    // { TXT_PLAYLIST_RADIO_S3, MUSIC_BANK_RADIO_1, 2, 4, sizeof(radioMsx01) },
    // { TXT_PLAYLIST_RADIO_S4, MUSIC_BANK_RADIO_1, 3, 4, sizeof(radioMsx01) },
    // { TXT_PLAYLIST_RADIO_S5, MUSIC_BANK_RADIO_1, 4, 4, sizeof(radioMsx01) },
    // { TXT_PLAYLIST_RADIO_S6, MUSIC_BANK_RADIO_1, 5, 4, sizeof(radioMsx01) },
    // { TXT_PLAYLIST_RADIO_S7, MUSIC_BANK_RADIO_1, 0, 5, sizeof(radioMsx02) },
    // { TXT_PLAYLIST_RADIO_S8, MUSIC_BANK_RADIO_1, 1, 5, sizeof(radioMsx02) },
    // { TXT_PLAYLIST_RADIO_S9, MUSIC_BANK_RADIO_1, 2, 5, sizeof(radioMsx02) }
};

const struct Song PLAYLIST[PLAYLIST_SIZE] = {
    { TXT_PLAYLIST_GM_NAME, TITLE_ONLY, TITLE_ONLY, TITLE_ONLY, 0 },
    { TXT_PLAYLIST_GM_S1, MUSIC_BANK, 0, 0, SPEED_1X },
    { TXT_PLAYLIST_GM_S2, MUSIC_BANK, 1, 1, SPEED_2X },
    { TXT_PLAYLIST_GM_S3, MENU_BANK_RESPAWN, 0, 9, SPEED_1X },
    { TXT_PLAYLIST_GM_S4, MENU_BANK_PIGSLE_INTRO, 0, 10, SPEED_1X },
    { TXT_PLAYLIST_GM_S5, MUSIC_BANK, 0, 2, SPEED_2X },
    { TXT_PLAYLIST_GM_S6, MUSIC_BANK, 0, 3, SPEED_2X },
    { TXT_PLAYLIST_GM_S7, MUSIC_BANK, 1, 3, SPEED_2X },
    // { TXT_PLAYLIST_RETRO_NAME, TITLE_ONLY, TITLE_ONLY, TITLE_ONLY, 0 },
    // { TXT_PLAYLIST_RETRO_S1, MUSIC_BANK_RETRO_1, 0, 6, sizeof(retroMsx01) },
    // { TXT_PLAYLIST_RETRO_S2, MUSIC_BANK_RETRO_1, 1, 6, sizeof(retroMsx01) },
    // { TXT_PLAYLIST_RETRO_S3, MUSIC_BANK_RETRO_1, 2, 6, sizeof(retroMsx01) },
    // // find a way to avoid duplicating these
    { TXT_PLAYLIST_RADIO_NAME, TITLE_ONLY, TITLE_ONLY, TITLE_ONLY, 0 },
    { TXT_PLAYLIST_RADIO_S1, MUSIC_BANK_RADIO_1, 0, 4, SPEED_1X },
    { TXT_PLAYLIST_RADIO_S2, MUSIC_BANK_RADIO_1, 0, 5, SPEED_1X },
    { TXT_PLAYLIST_RADIO_S3, MUSIC_BANK_RADIO_1, 0, 6, SPEED_1X },
    { TXT_PLAYLIST_RADIO_S4, MUSIC_BANK_RADIO_2, 0, 7, SPEED_1X },
    { TXT_PLAYLIST_RADIO_S5, MUSIC_BANK_RADIO_2, 0, 8, SPEED_2X },
    // { TXT_PLAYLIST_RADIO_S1, MUSIC_BANK_RADIO_1, 0, 4, sizeof(radioMsx01) },
    // { TXT_PLAYLIST_RADIO_S2, MUSIC_BANK_RADIO_1, 1, 4, sizeof(radioMsx01) },
    // { TXT_PLAYLIST_RADIO_S3, MUSIC_BANK_RADIO_1, 2, 4, sizeof(radioMsx01) },
    // { TXT_PLAYLIST_RADIO_S4, MUSIC_BANK_RADIO_1, 3, 4, sizeof(radioMsx01) },
    // { TXT_PLAYLIST_RADIO_S5, MUSIC_BANK_RADIO_1, 4, 4, sizeof(radioMsx01) },
    // { TXT_PLAYLIST_RADIO_S6, MUSIC_BANK_RADIO_1, 5, 4, sizeof(radioMsx01) },
    // { TXT_PLAYLIST_RADIO_S7, MUSIC_BANK_RADIO_1, 0, 5, sizeof(radioMsx02) },
    // { TXT_PLAYLIST_RADIO_S8, MUSIC_BANK_RADIO_1, 1, 5, sizeof(radioMsx02) },
    // { TXT_PLAYLIST_RADIO_S9, MUSIC_BANK_RADIO_1, 2, 5, sizeof(radioMsx02) }
};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
