#include <c64/charwin.h>
#include <c64/keyboard.h>
#include <c64/types.h>
#include <c64/vic.h>
#include <c64/easyflash.h>

#include <menu/menuSystem.h>
#include <translation/common.h>
#include <assets/assetsSettings.h>
#include <engine/easyFlashBanks.h>
#include <engine/gameSettings.h>

#define MAX_LANG 1
const byte LANGUAGE_BANKS[MAX_LANG+1] = {TRANSLATION_PL_BANK, TRANSLATION_EN_BANK};
static byte _currentLang = 0;

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
    
    changeBank(LANGUAGE_BANKS[_currentLang]);
    loadTranslation();
    restoreBank();
    showOptionsMenu();
}

static void _backToPreviousMenu(){
    gms_textMode = false;
    gms_disableTimeControls = false;
    // prepareScroll();
    // This will simply call currently mounted menu again
    showMenu();
}

const struct MenuOption OPTIONS_MENU[] = {
    { TXT_IDX_MENU_OPTIONS_LANG, '1', UI_UD, &_changeLanguage, 0, 2, 1},
    { TXT_IDX_MENU_OPTIONS_MSX, '2', UI_UD, &_toggleMusic, 0, 2, 2},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, UI_UD, &_backToPreviousMenu, 0, 1, 20},

    END_MENU_CHOICES
};
