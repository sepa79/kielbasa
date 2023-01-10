#include <c64/charwin.h>
#include <c64/keyboard.h>
#include <c64/types.h>
#include <c64/vic.h>
#include <c64/easyflash.h>
#include <c64/memmap.h>

#include <menu/menuSystem.h>
#include <translation/common.h>
#include <assets/assetsSettings.h>
#include <engine/easyFlashBanks.h>
#include <engine/gameSettings.h>

#define MAX_LANG 1
const byte LANGUAGE_BANKS[MAX_LANG+1] = {TRANSLATION_PL_BANK, TRANSLATION_EN_BANK};
static byte _currentLang = 1;
static byte _currentSong = 2;

struct Song {
    /* Text to display */
    const char* textIdx;
    /* SID bank */
    byte bank;
    /* Song index in SID */
    byte songIdx;
    /* SID index in the bank */
    byte sidIdx;
};

// TODO: toss somewhere into ROM - the main menu is in RAM so music menu might one day need its own bank
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

#define TITLE_ONLY 0xff
#define RADIO_PLAYLIST_SIZE 9
static byte _currentRadioSong = RADIO_PLAYLIST_SIZE;

#define PLAYLIST_SIZE 12 + RADIO_PLAYLIST_SIZE
const struct Song RADIO_PLAYLIST[RADIO_PLAYLIST_SIZE] = {
    { TXT_PLAYLIST_RADIO_S1, MUSIC_BANK_RADIO_1, 0, 0 },
    { TXT_PLAYLIST_RADIO_S2, MUSIC_BANK_RADIO_1, 1, 0 },
    { TXT_PLAYLIST_RADIO_S3, MUSIC_BANK_RADIO_1, 2, 0 },
    { TXT_PLAYLIST_RADIO_S4, MUSIC_BANK_RADIO_1, 3, 0 },
    { TXT_PLAYLIST_RADIO_S5, MUSIC_BANK_RADIO_1, 4, 0 },
    { TXT_PLAYLIST_RADIO_S6, MUSIC_BANK_RADIO_1, 5, 0 },
    { TXT_PLAYLIST_RADIO_S7, MUSIC_BANK_RADIO_1, 0, 1 },
    { TXT_PLAYLIST_RADIO_S8, MUSIC_BANK_RADIO_1, 1, 1 },
    { TXT_PLAYLIST_RADIO_S9, MUSIC_BANK_RADIO_1, 2, 1 }
};

const struct Song PLAYLIST[PLAYLIST_SIZE] = {
    { TXT_PLAYLIST_GM_NAME, TITLE_ONLY, TITLE_ONLY, TITLE_ONLY },
    { TXT_PLAYLIST_GM_S1, MUSIC_BANK, 0, 0 },
    { TXT_PLAYLIST_GM_S2, MUSIC_BANK, 1, 0 },
    { TXT_PLAYLIST_GM_S3, MUSIC_BANK, 2, 0 },
    { TXT_PLAYLIST_GM_S4, MUSIC_BANK, 3, 0 },
    { TXT_PLAYLIST_GM_S5, MUSIC_BANK, 4, 0 },
    { TXT_PLAYLIST_GM_S6, MUSIC_BANK, 5, 0 },
    { TXT_PLAYLIST_RETRO_NAME, TITLE_ONLY, TITLE_ONLY, TITLE_ONLY },
    { TXT_PLAYLIST_RETRO_S1, MUSIC_BANK_RETRO_1, 0, 0 },
    { TXT_PLAYLIST_RETRO_S2, MUSIC_BANK_RETRO_1, 1, 0 },
    { TXT_PLAYLIST_RETRO_S3, MUSIC_BANK_RETRO_1, 2, 0 },
    // find a way to avoid duplicating these
    { TXT_PLAYLIST_RADIO_NAME, TITLE_ONLY, TITLE_ONLY, TITLE_ONLY },
    { TXT_PLAYLIST_RADIO_S1, MUSIC_BANK_RADIO_1, 0, 0 },
    { TXT_PLAYLIST_RADIO_S2, MUSIC_BANK_RADIO_1, 1, 0 },
    { TXT_PLAYLIST_RADIO_S3, MUSIC_BANK_RADIO_1, 2, 0 },
    { TXT_PLAYLIST_RADIO_S4, MUSIC_BANK_RADIO_1, 3, 0 },
    { TXT_PLAYLIST_RADIO_S5, MUSIC_BANK_RADIO_1, 4, 0 },
    { TXT_PLAYLIST_RADIO_S6, MUSIC_BANK_RADIO_1, 5, 0 },
    { TXT_PLAYLIST_RADIO_S7, MUSIC_BANK_RADIO_1, 0, 1 },
    { TXT_PLAYLIST_RADIO_S8, MUSIC_BANK_RADIO_1, 1, 1 },
    { TXT_PLAYLIST_RADIO_S9, MUSIC_BANK_RADIO_1, 2, 1 }
};

#define PLAYLIST_X 2
#define PLAYLIST_Y 3
static void _displayPlaylist(){
    // songs list
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
}

static void _upRowCheck(){
    if(_currentSong > 0){
        _currentSong--;
    } else {
        _currentSong = PLAYLIST_SIZE-1;
    }
}
static void _upRow(){
    _upRowCheck();
    if(PLAYLIST[_currentSong].bank == TITLE_ONLY){
        _upRowCheck();
    }
    _displayPlaylist();
}
static void _downRowCheck(){
    if(_currentSong < PLAYLIST_SIZE-1){
        _currentSong++;
    } else {
        _currentSong = 0;
    }
}
static void _downRow(){
    _downRowCheck();
    if(PLAYLIST[_currentSong].bank == TITLE_ONLY){
        _downRowCheck();
    }
    _displayPlaylist();
}

static void _playMsx(struct Song * song){
    joyCursor.enabled = false;
    if(gms_enableMusic) {
        // set Radio bank
        char pbank = setBank(song->bank);

        // stop music
        gms_enableMusic = false;
        ((byte *)0xd418)[0] &= ~0xf;

        // load different MSX file
        loadMusic(song->sidIdx);
        byte songIdx = song->songIdx;

        // init it
        __asm {
            lda #MSX_ROM
            sta $01
            lda songIdx
            jsr MSX_INIT
        };

        // set ROM back
        mmap_set(MMAP_ROM);

        // reenable music
        gms_enableMusic = true;

        // revert menu bank
        setBank(pbank);
    }
    joyCursor.enabled = true;
    // vic.color_back--;
}

// menu wrapper
static void _loadMsx() {
    _playMsx(&PLAYLIST[_currentSong]);
}

void _showMusicMenu(){
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, BIG_SCREEN_Y_START, SCREEN_WIDTH, BIG_SCREEN_HEIGHT);
    cwin_clear(&cw);

    // static menu texts
    cwin_putat_string_raw(&cw, 0, 0, TXT[TXT_IDX_MUSIC_OPTIONS_HEADER], VCOL_GREEN);

    displayMenu(MUSIC_MENU);
    _displayPlaylist();
}

void showOptionsMenu(){
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, BIG_SCREEN_Y_START, SCREEN_WIDTH, BIG_SCREEN_HEIGHT);
    cwin_clear(&cw);

    // static menu texts
    cwin_putat_string_raw(&cw, 0, 0, TXT[TXT_IDX_OPTIONS_HEADER], VCOL_GREEN);

    displayMenu(OPTIONS_MENU);
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
    _playMsx(&RADIO_PLAYLIST[_currentRadioSong]);
}

const struct MenuOption MUSIC_MENU[] = {
    { TXT_IDX_MENU_OPTIONS_MSX_PLAY, KEY_RETURN, SCREEN_FULL_TXT, UI_SELECT, &_loadMsx, 0, 1, 1},
    { TXT_IDX_MENU_OPTIONS_MSX_ON_OFF, '1', SCREEN_FULL_TXT, UI_SELECT, &_toggleMusic, 0, 10, 1},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_FULL_TXT, UI_LF, &showOptionsMenu, 0, 30, 0},
    { TXT_IDX_MENU_TASK_MANAGER_W, 'w', SCREEN_FULL_TXT, UI_U+UI_HIDE, &_upRow, 0, 0, 4 },
    { TXT_IDX_MENU_TASK_MANAGER_S, 's', SCREEN_FULL_TXT, UI_D+UI_HIDE, &_downRow, 0, 0, 22 },
    END_MENU_CHOICES
};
const struct MenuOption OPTIONS_MENU[] = {
    { TXT_IDX_MENU_OPTIONS_LANG, '1', SCREEN_FULL_TXT, UI_SELECT, &_changeLanguage, 0, 2, 1},
    { TXT_IDX_MENU_OPTIONS_MSX, '2', SCREEN_FULL_TXT, UI_SELECT, &_showMusicMenu, 0, 2, 2},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_RESTORE, UI_LF, &backToPreviousMenu, 0, 2, 3},
    END_MENU_CHOICES
};
