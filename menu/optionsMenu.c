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

#define TITLE_ONLY 0xff
#define PLAYLIST_SIZE 18
const struct Song PLAYLIST[PLAYLIST_SIZE] = {
    { TXT_PLAYLIST_GM_NAME, TITLE_ONLY, TITLE_ONLY },
    { TXT_PLAYLIST_GM_S1, MUSIC_BANK, 0 },
    { TXT_PLAYLIST_GM_S2, MUSIC_BANK, 1 },
    { TXT_PLAYLIST_GM_S3, MUSIC_BANK, 2 },
    { TXT_PLAYLIST_GM_S4, MUSIC_BANK, 3 },
    { TXT_PLAYLIST_GM_S5, MUSIC_BANK, 4 },
    { TXT_PLAYLIST_GM_S6, MUSIC_BANK, 5 },
    { TXT_PLAYLIST_RETRO_NAME, TITLE_ONLY, TITLE_ONLY },
    { TXT_PLAYLIST_RETRO_S1, MUSIC_BANK_RETRO_1, 0 },
    { TXT_PLAYLIST_RETRO_S2, MUSIC_BANK_RETRO_1, 1 },
    { TXT_PLAYLIST_RETRO_S3, MUSIC_BANK_RETRO_1, 2 },
    { TXT_PLAYLIST_RADIO_NAME, TITLE_ONLY, TITLE_ONLY },
    { TXT_PLAYLIST_RADIO_S1, MUSIC_BANK_RADIO_1, 0 },
    { TXT_PLAYLIST_RADIO_S2, MUSIC_BANK_RADIO_1, 1 },
    { TXT_PLAYLIST_RADIO_S3, MUSIC_BANK_RADIO_1, 2 },
    { TXT_PLAYLIST_RADIO_S4, MUSIC_BANK_RADIO_1, 3 },
    { TXT_PLAYLIST_RADIO_S5, MUSIC_BANK_RADIO_1, 4 },
    { TXT_PLAYLIST_RADIO_S6, MUSIC_BANK_RADIO_1, 5 }
};

#define PLAYLIST_X 2
#define PLAYLIST_Y 5
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

static void _loadMsx(){
    // vic.color_back++;
    joyCursor.enabled = false;
    if(gms_enableMusic) {
        // set Radio bank
        char pbank = setBank(PLAYLIST[_currentSong].bank);

        // stop music
        gms_enableMusic = false;
        ((byte *)0xd418)[0] &= ~0xf;

        // load different MSX file
        loadMusic();
        byte songIdx = PLAYLIST[_currentSong].songIdx;

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
        // gms_musicSpeed2x = true;
        gms_enableMusic = true;

        // revert menu bank
        setBank(pbank);
    }
    joyCursor.enabled = true;
    // vic.color_back--;
}

void _showMusicMenu(){
    gms_textMode = true;
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, BIG_SCREEN_Y_START, SCREEN_WIDTH, BIG_SCREEN_HEIGHT);
    cwin_clear(&cw);

    // static menu texts
    cwin_putat_string_raw(&cw, 0, 0, TXT[TXT_IDX_MUSIC_OPTIONS_HEADER], VCOL_GREEN);

    displayMenu(MUSIC_MENU);
    _displayPlaylist();
}

void showOptionsMenu(){
    gms_textMode = true;
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

static void _backToPreviousMenu(){
    gms_textMode = false;
    gms_disableTimeControls = false;
    // This will simply call currently mounted menu again
    switchScreenToSplitMcTxt();
    showMenu();
}

const struct MenuOption MUSIC_MENU[] = {
    { TXT_IDX_MENU_OPTIONS_MSX_PLAY, KEY_RETURN, UI_SELECT, &_loadMsx, 0, 1, 1},
    { TXT_IDX_MENU_OPTIONS_MSX_ON_OFF, '1', UI_SELECT, &_toggleMusic, 0, 16, 1},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, UI_SELECT, &showOptionsMenu, 0, 33, 1},
    { TXT_IDX_MENU_TASK_MANAGER_W, 'w', UI_U+UI_HIDE, &_upRow, 0, 0, 4 },
    { TXT_IDX_MENU_TASK_MANAGER_S, 's', UI_D+UI_HIDE, &_downRow, 0, 0, 22 },
    END_MENU_CHOICES
};
const struct MenuOption OPTIONS_MENU[] = {
    { TXT_IDX_MENU_OPTIONS_LANG, '1', UI_SELECT, &_changeLanguage, 0, 2, 1},
    { TXT_IDX_MENU_OPTIONS_MSX, '2', UI_SELECT, &_showMusicMenu, 0, 2, 2},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, UI_SELECT, &_backToPreviousMenu, 0, 2, 3},
    END_MENU_CHOICES
};
