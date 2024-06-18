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
const char LANGUAGE_BANKS[MAX_LANG+1] = {TRANSLATION_PL_BANK, TRANSLATION_EN_BANK};
static char _currentLang = 1;
static char _currentSong = 2;
static char _currentRadioSong = RADIO_PLAYLIST_SIZE;
static char _page = 0;

#define PLAYLIST_X 2
#define PLAYLIST_Y 0
#define MAX_PAGE 1

static void _displayPlaylistPage(char start, char end){
    char yPos = PLAYLIST_Y;
    for(byte i=start;i<end;i++){
        byte color = VCOL_GREEN;
        // print songs marker
        if(PLAYLIST[i].bank == TITLE_ONLY){
            color = VCOL_BLUE;
        } else if(_currentSong == i){
            color = VCOL_GREEN;
            cwin_putat_string_raw(&cw, PLAYLIST_X-2, yPos, s">", VCOL_WHITE);
        } else {
            cwin_putat_string_raw(&cw, PLAYLIST_X-2, yPos, s" ", VCOL_WHITE);
        }

        cwin_putat_string_raw(&cw, PLAYLIST_X, yPos, PLAYLIST[i].textIdx, color);

        yPos++;
    }
}
static void _displayPlaylist(){
    // songs list
    char pbank = setBank(MUSIC_BANK);
    // now songs are visible from ROM
    _displayPlaylistPage(0, PLAYLIST_SIZE);

    // if(_page == 0){
    //     _displayPlaylistPage(0, 11);
    // } else {
    //     _displayPlaylistPage(11, PLAYLIST_SIZE);
    // }

    setBank(pbank);
}

static void _displayBoombox(){

    char pbank = setBank(MUSIC_BANK_RETRO_1);
    char pport = mmap_set(MMAP_ROM);

    // rom to buffer -> GFX_1_SCR
    memcpy(GFX_1_SCR, boomboxFnt, 0x400);
    // switch IO off
    mmap_set(MMAP_RAM);
    // buffer to RAM under IO
    memcpy(GFX_1_FNT+0x400, GFX_1_SCR, 0x400);
    mmap_set(pport);
    memcpy(GFX_1_SCR+12*40, boomboxScr, 13*40);

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

static void _left(){
    if(_page > 0){
        _page--;
        _currentSong = 1;
    }
    cwin_clear(&cw);
    _displayPlaylist();
}
static void _right(){
    if(_page < MAX_PAGE){
        _page++;
        _currentSong = 12;
    }
    cwin_clear(&cw);
    _displayPlaylist();
}

static void _upRowCheck(){
    if(_currentSong > 1){
        _currentSong--;
    }
    if(_currentSong < 11 && _page == 1){
        _page = 0;
        cwin_clear(&cw);
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
    }
    if(_currentSong > 11 && _page == 0){
        _page = 1;
        cwin_clear(&cw);
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

static void _playMsx(const struct Song * song, bool restart){
    joyCursor.enabled = false;
    if(gms_enableMusic) {
        // stop music
        gms_enableMusic = false;
        ((byte *)0xd418)[0] &= ~0xf;

        // set Radio bank, it contains titles etc
        char pbank = setBank(MUSIC_BANK);
        // now songs are visible from ROM - we can use structs
        // just this needs to be cached as banks might get changed during load below
        char sidIdx = song->sidIdx;
        // check if we need to load anything
        if(curBank != song->bank || curSidIdx != sidIdx){
            gms_musicPlayCount = song->speed;
            gms_musicPlayNow = true;
            // load it
            setBank(song->bank);

            // load different MSX file
            oscar_expand_lzo_buf((char*)MSX_DST_ADR, sidIndex[sidIdx]);

            setBank(MUSIC_BANK);
            // if we have reloaded, reset SongIdx
            curSongIdx = 0xff;
        }
        // all loaded or there was no need to load
        curBank   = song->bank;
        curSidIdx = song->sidIdx;

        char songIdx = song->songIdx;
        if(curSongIdx != songIdx || restart){
            char pport = mmap_set(MMAP_NO_BASIC);
            // init it
            __asm {
                lda songIdx
                jsr MSX_INIT
            };

            // set ROM back
            mmap_set(pport);
        }
        curSongIdx = songIdx;

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

static void closeMsxMenu(){
   // reload fonts
    char pBank = setBank(MAIN_GFX_BANK);
    char pport = mmap_set(MMAP_ROM);

    // rom to buffer -> GFX_1_SCR
    memcpy(GFX_1_SCR, GFX_1_FNT_SRC+0x400, 0x400);
    // switch IO off
    mmap_set(MMAP_RAM);
    // buffer to RAM under IO
    memcpy(GFX_1_FNT+0x400, GFX_1_SCR, 0x400);
    mmap_set(pport);
    setBank(pBank);
    showOptionsMenu();
}

static void _showMusicMenu(){
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, BIG_SCREEN_Y_START, SCREEN_WIDTH, 12);

    _displayBoombox();
    // clean after, as disply uses screen as buffer
    cwin_clear(&cw);
    displayMenu(MUSIC_MENU);
    _displayPlaylist();
    // static menu texts
    updateStatusBar(TXT[SB_IDX_MENU_OPTIONS_MSX_MENU]);
    switchScreenTo(SCREEN_MC_TXT_BOOMBOX);
}

void showOptionsMenu(){
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, BIG_SCREEN_Y_START, SCREEN_WIDTH, BIG_SCREEN_HEIGHT);
    cwin_clear(&cw);

    // copy text options menu cache from cart to memory
    loadCacheTxt(TXT_OPTIONS_MENU_CACHE_INDEX);

    // static menu texts
    #define HELP_Y 6
    cwin_putat_string_raw(&cw, 0, 0, TXT_CACHE[LTXT_IDX_OPTIONS_HEADER], VCOL_GREEN);
    cwin_putat_string_raw(&cw, 0, HELP_Y+0, TXT_CACHE[LTXT_IDX_OPTIONS_HELP1], VCOL_LT_GREY);
    cwin_putat_string_raw(&cw, 0, HELP_Y+2, TXT_CACHE[LTXT_IDX_OPTIONS_HELP2], VCOL_MED_GREY);
    cwin_putat_string_raw(&cw, 0, HELP_Y+3, TXT_CACHE[LTXT_IDX_OPTIONS_HELP3], VCOL_MED_GREY);
    cwin_putat_string_raw(&cw, 0, HELP_Y+4, TXT_CACHE[LTXT_IDX_OPTIONS_HELP4], VCOL_MED_GREY);
    cwin_putat_string_raw(&cw, 0, HELP_Y+5, TXT_CACHE[LTXT_IDX_OPTIONS_HELP5], VCOL_MED_GREY);
    cwin_putat_string_raw(&cw, 0, HELP_Y+6, TXT_CACHE[LTXT_IDX_OPTIONS_HELP6], VCOL_MED_GREY);

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

// Plays given song, does not restart it if already playing
void playSong(char song){
    _playMsx(&PLAYLIST[song], false);
}

// Starts given song from the beginning, regardles if it is playing already or not
void startSong(char song){
    _playMsx(&PLAYLIST[song], true);
}

const struct MenuOption MUSIC_MENU[] = {
    { TXT_IDX_MENU_SELECT, KEY_RETURN, SCREEN_MC_TXT_BOOMBOX, UI_SELECT+UI_HIDE, &_loadMsx, 0, 1, 1},
    { TXT_IDX_MENU_A, 'm', SCREEN_MC_TXT_BOOMBOX, UI_HIDE, &_toggleMusic, 0, 1, 1},
    // { TXT_IDX_MENU_A, 'a', SCREEN_MC_TXT_BOOMBOX, UI_L+UI_HIDE, &_left, 0, 1, 1},
    // { TXT_IDX_MENU_D, 'd', SCREEN_MC_TXT_BOOMBOX, UI_R+UI_HIDE, &_right, 0, 1, 1},
    { TXT_IDX_MENU_W, 'w', SCREEN_MC_TXT_BOOMBOX, UI_U+UI_HIDE, &_upRow, 0, 1, 1 },
    { TXT_IDX_MENU_S, 's', SCREEN_MC_TXT_BOOMBOX, UI_D+UI_HIDE, &_downRow, 0, 1, 1 },
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_TRANSITION, UI_LF+UI_HIDE, &closeMsxMenu, 0, 1, 1},
    END_MENU_CHOICES
};
const struct MenuOption OPTIONS_MENU[] = {
    { TXT_IDX_MENU_OPTIONS_LANG, '1', SCREEN_FULL_TXT, UI_SELECT, &_changeLanguage, 0, 2, 1},
    { TXT_IDX_MENU_OPTIONS_MSX, '2', SCREEN_TRANSITION, UI_SELECT, &_showMusicMenu, 0, 2, 2},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_TRANSITION, UI_LF, &backToPreviousMenu, 0, 2, 3},
    END_MENU_CHOICES
};
