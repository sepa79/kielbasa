#include <c64/charwin.h>
#include <c64/keyboard.h>
#include <string.h>
#include <c64/types.h>
#include <c64/vic.h>
#include <c64/easyflash.h>
#include <c64/memmap.h>

#include <assets/music.h>
#include <menu/menuSystem.h>
#include <menu/optionsMenu.h>
#include <translation/common.h>
#include <assets/assetsSettings.h>
#include <engine/easyFlashBanks.h>
#include <engine/gameSettings.h>

#define MAX_LANG 1
const byte LANGUAGE_BANKS[MAX_LANG+1] = {TRANSLATION_PL_BANK, TRANSLATION_EN_BANK};
static byte _currentLang = 1;
static byte _currentSong = 2;
static byte _currentRadioSong = RADIO_PLAYLIST_SIZE;

#define PLAYLIST_X 2
#define PLAYLIST_Y 3
static void _displayPlaylist(){
    // songs list
    char pbank = setBank(MUSIC_BANK);
    // now songs are visible from ROM

    for(byte i=0;i<PLAYLIST_SIZE;i++){
        byte color = VCOL_GREEN;
        // print songs marker
        if(PLAYLIST[i].bank == TITLE_ONLY){
            color = VCOL_LT_BLUE;
        } else if(_currentSong == i){
            color = VCOL_LT_GREEN;
            cwin_putat_string_raw(&cw, PLAYLIST_X-2, PLAYLIST_Y+i, s">", VCOL_WHITE);
        } else {
            cwin_putat_string_raw(&cw, PLAYLIST_X-2, PLAYLIST_Y+i, s" ", VCOL_WHITE);
        }

        cwin_putat_string_raw(&cw, PLAYLIST_X, PLAYLIST_Y+i, PLAYLIST[i].textIdx, color);
    }

    setBank(pbank);
}

static void _displayBoombox(){
    rirq_stop();

    char pbank = setBank(MUSIC_BANK_RETRO_1);
    char pport = setPort(MMAP_ALL_ROM);

    memcpy(GFX_1_FNT+0x400, boomboxFnt, 0x400);
    memcpy(GFX_1_SCR+12*40, boomboxScr, 13*40);
    setPort(pport);
    rirq_start();
    // colors
    for(char i=0; i<130; i++){
        COLOR_RAM[12*40 + i + 000] = boomboxAtr[GFX_1_SCR[12*40 + i + 000]];
        COLOR_RAM[12*40 + i + 130] = boomboxAtr[GFX_1_SCR[12*40 + i + 130]];
        COLOR_RAM[12*40 + i + 260] = boomboxAtr[GFX_1_SCR[12*40 + i + 260]];
        COLOR_RAM[12*40 + i + 390] = boomboxAtr[GFX_1_SCR[12*40 + i + 390]];
    }
    setBank(pbank);
    vic.color_back1 = VCOL_DARK_GREY;
    vic.color_back2 = VCOL_LT_GREY;
}

static void _upRowCheck(){
    if(_currentSong > 0){
        _currentSong--;
    } else {
        _currentSong = PLAYLIST_SIZE-1;
    }
}
static void _upRow(){
    char pbank = setBank(MUSIC_BANK);
    _upRowCheck();
    if(PLAYLIST[_currentSong].bank == TITLE_ONLY){
        _upRowCheck();
    }
    _displayPlaylist();
    setBank(pbank);
}
static void _downRowCheck(){
    if(_currentSong < PLAYLIST_SIZE-1){
        _currentSong++;
    } else {
        _currentSong = 0;
    }
}
static void _downRow(){
    char pbank = setBank(MUSIC_BANK);
    _downRowCheck();
    if(PLAYLIST[_currentSong].bank == TITLE_ONLY){
        _downRowCheck();
    }
    _displayPlaylist();
    setBank(pbank);
}

char curBank    = 0;
char curSidIdx  = 0;
char curSongIdx = 0;

static void _playMsx(struct Song * song, bool restart){
    joyCursor.enabled = false;
    if(gms_enableMusic) {
        // stop music
        gms_enableMusic = false;
        ((byte *)0xd418)[0] &= ~0xf;

        // set Radio bank
        char pbank = setBank(MUSIC_BANK);
        // now songs are visible from ROM - we can use structs

        // check if we need to load anything
        if(curBank != song->bank || curSidIdx != song->sidIdx){
            // load it
            setBank(song->bank);
            // load different MSX file
            loadMusic(song);
        }
        // all loaded or there was no need to load
        curBank   = song->bank;
        curSidIdx = song->sidIdx;

        char songIdx = song->songIdx;
        if(curSongIdx != songIdx || restart){
            char pport = setPort(MMAP_NO_BASIC);
            // init it
            __asm {
                lda songIdx
                jsr MSX_INIT
            };

            // set ROM back
            setPort(pport);
        }

        // revert menu bank
        setBank(pbank);

        // reenable music
        gms_enableMusic = true;
    }
    joyCursor.enabled = true;
    // vic.color_back--;
}

// menu wrapper
static void _loadMsx() {
    _playMsx(&PLAYLIST[_currentSong], true);
}

void _showMusicMenu(){
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, BIG_SCREEN_Y_START, SCREEN_WIDTH, BIG_SCREEN_HEIGHT);
    cwin_clear(&cw);

    // static menu texts
    cwin_putat_string_raw(&cw, 0, 0, TXT[TXT_IDX_MUSIC_OPTIONS_HEADER], VCOL_GREEN);

    switchScreenTo(SCREEN_MC_TXT);
    displayMenu(MUSIC_MENU);
    _displayPlaylist();
    _displayBoombox();
}

void showOptionsMenu(){
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, BIG_SCREEN_Y_START, SCREEN_WIDTH, BIG_SCREEN_HEIGHT);
    cwin_clear(&cw);

    // static menu texts
    cwin_putat_string_raw(&cw, 0, 0, TXT[TXT_IDX_OPTIONS_HEADER], VCOL_GREEN);

    displayMenu(OPTIONS_MENU);
    switchScreenTo(SCREEN_FULL_TXT);
}

static void _toggleMusic(){
    if(gms_enableMusic){
        // stop it
        gms_enableMusic = false;
        ((byte *)0xd418)[0] &= ~0xf;
    } else {
        // play it
        gms_enableMusic = true;
    }
}

static void _changeLanguage(){
    _currentLang++;
    if(_currentLang > MAX_LANG){
        _currentLang = 0;
    }
    
    char pbank = setBank(LANGUAGE_BANKS[_currentLang]);
    loadTranslation();
    setBank(pbank);
    showOptionsMenu();
}

void playNextRadioSong(){
    _currentRadioSong++;
    if(_currentRadioSong >= RADIO_PLAYLIST_SIZE)
        _currentRadioSong = 0;
    _playMsx(&RADIO_PLAYLIST[_currentRadioSong], true);
}

void playSong(char song){
    _playMsx(&PLAYLIST[song], false);
}

const struct MenuOption MUSIC_MENU[] = {
    { TXT_IDX_MENU_OPTIONS_MSX_PLAY, KEY_RETURN, SCREEN_MC_TXT, UI_SELECT, &_loadMsx, 0, 1, 1},
    { TXT_IDX_MENU_OPTIONS_MSX_ON_OFF, '1', SCREEN_MC_TXT, UI_SELECT, &_toggleMusic, 0, 10, 1},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_FULL_TXT, UI_LF, &showOptionsMenu, 0, 30, 0},
    { TXT_IDX_MENU_TASK_MANAGER_W, 'w', SCREEN_MC_TXT, UI_U+UI_HIDE, &_upRow, 0, 0, 4 },
    { TXT_IDX_MENU_TASK_MANAGER_S, 's', SCREEN_MC_TXT, UI_D+UI_HIDE, &_downRow, 0, 0, 22 },
    END_MENU_CHOICES
};
const struct MenuOption OPTIONS_MENU[] = {
    { TXT_IDX_MENU_OPTIONS_LANG, '1', SCREEN_FULL_TXT, UI_SELECT, &_changeLanguage, 0, 2, 1},
    { TXT_IDX_MENU_OPTIONS_MSX, '2', SCREEN_MC_TXT, UI_SELECT, &_showMusicMenu, 0, 2, 2},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_TRANSITION, UI_LF, &backToPreviousMenu, 0, 2, 3},
    END_MENU_CHOICES
};
