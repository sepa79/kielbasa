#include <c64/charwin.h>
#include <c64/keyboard.h>
#include <c64/types.h>
#include <c64/vic.h>
#include <c64/easyflash.h>

#include <character/character.h>
#include <menu/menuSystem.h>
#include <translation/common.h>
#include <assets/assetsSettings.h>
#include <engine/easyFlashBanks.h>
#include <engine/gameSettings.h>

static void _showTaskPriorities(){

    cwin_putat_string_raw(&cw, 0, 4, TXT[TXT_IDX_TASK_MANAGER_TABLE_HEADER_1], VCOL_YELLOW);
    cwin_putat_string_raw(&cw, 0, 5, TXT[TXT_IDX_TASK_MANAGER_TABLE_HEADER_2], VCOL_YELLOW);
    cwin_putat_string_raw(&cw, 0, 6, TXT[TXT_IDX_TASK_MANAGER_TABLE_HEADER_3], VCOL_YELLOW);
    for(byte character = 0; character < CHARACTER_COUNT; character++){
        byte line = 7+character;
        cwin_putat_string_raw(&cw, 0, line, TXT[allChars_nameIdx[character]], VCOL_GREEN);

        cwin_putat_string_raw(&cw, 21, line, "\x7e", VCOL_YELLOW);

        byte str[2];
        sprintf(str, "%u", allChars_skillAni[character]);
        cwin_putat_string_raw(&cw, 22,  line, str, VCOL_GREEN);
        sprintf(str, "%u", allChars_skillFrm[character]);
        cwin_putat_string_raw(&cw, 24,  line, str, VCOL_GREEN);
        sprintf(str, "%u", allChars_skillBth[character]);
        cwin_putat_string_raw(&cw, 26,  line, str, VCOL_GREEN);
        sprintf(str, "%u", allChars_skillTrd[character]);
        cwin_putat_string_raw(&cw, 28,  line, str, VCOL_GREEN);
        cwin_putat_string_raw(&cw, 23, line, "\x7e", VCOL_YELLOW);
        cwin_putat_string_raw(&cw, 25, line, "\x7e", VCOL_YELLOW);
        cwin_putat_string_raw(&cw, 27, line, "\x7e", VCOL_YELLOW);
        cwin_putat_string_raw(&cw, 30, line, "\x7e", VCOL_YELLOW);

        sprintf(str, "%u", allChars_skillAni[character]);
        cwin_putat_string_raw(&cw, 32,  line, str, VCOL_MED_GREY);
        sprintf(str, "%u", allChars_skillFrm[character]);
        cwin_putat_string_raw(&cw, 34,  line, str, VCOL_MED_GREY);
        sprintf(str, "%u", allChars_skillBth[character]);
        cwin_putat_string_raw(&cw, 36,  line, str, VCOL_MED_GREY);
        sprintf(str, "%u", allChars_skillTrd[character]);
        cwin_putat_string_raw(&cw, 38,  line, str, VCOL_MED_GREY);
        cwin_putat_string_raw(&cw, 33, line, "\x7e", VCOL_YELLOW);
        cwin_putat_string_raw(&cw, 35, line, "\x7e", VCOL_YELLOW);
        cwin_putat_string_raw(&cw, 37, line, "\x7e", VCOL_YELLOW);
    }
}

static void _option1(){

}
static void _backToPreviousMenu(){
    gms_textMode = false;
    gms_disableTimeControls = false;
    // This will simply call currently mounted menu again
    showMenu();
}

void showTaskManagerMenu(){
    gms_textMode = true;
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, BIG_SCREEN_Y_START, SCREEN_WIDTH, BIG_SCREEN_HEIGHT);
    cwin_clear(&cw);

    // static menu texts
    cwin_putat_string_raw(&cw, 0, 0, TXT[TXT_IDX_TASK_MANAGER_HEADER], VCOL_GREEN);

    displayMenu(TASK_MANAGER_MENU);
    _showTaskPriorities();
}

const struct MenuOption TASK_MANAGER_MENU[] = {
    { TXT_IDX_MENU_TASK_MANAGER_W, 'w', &_option1, 0, 1, 1},
    { TXT_IDX_MENU_TASK_MANAGER_S, 's', &_option1, 0, 1, 3},
    { TXT_IDX_MENU_TASK_MANAGER_A, 'a', &_option1, 0, 0, 2},
    { TXT_IDX_MENU_TASK_MANAGER_D, 'd', &_option1, 0, 2, 2},
    { TXT_IDX_MENU_TASK_MANAGER_PLUS, '-', &_option1, 0, 10, 1},
    { TXT_IDX_MENU_TASK_MANAGER_MINUS, '+', &_option1, 0, 10, 3},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, &_backToPreviousMenu, 0, 1, 20},

    END_MENU_CHOICES
};
