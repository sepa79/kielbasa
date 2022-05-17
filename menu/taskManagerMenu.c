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

static byte _currentCharacter = 0;
static byte _currentSkill = 0;
// where to start displaying stats, also used for cursor
static byte TASK_SHOW_LINE = 14;
static byte TASK_SHOW_COLUMN = 32;

static void _showTaskPriorities(){

    // TASK_MANAGER_MENU[0].y = TASK_SHOW_LINE + _currentCharacter;
    // TASK_MANAGER_MENU[0].x = TASK_SHOW_COLUMN + _currentSkill * 2;
    // TASK_MANAGER_MENU[1].y = TASK_SHOW_LINE + _currentCharacter;
    // TASK_MANAGER_MENU[1].x = TASK_SHOW_COLUMN + _currentSkill * 2;
    // TASK_MANAGER_MENU[2].y = TASK_SHOW_LINE + _currentCharacter;
    // TASK_MANAGER_MENU[2].x = TASK_SHOW_COLUMN + _currentSkill * 2;
    // TASK_MANAGER_MENU[3].y = TASK_SHOW_LINE + _currentCharacter;
    // TASK_MANAGER_MENU[3].x = TASK_SHOW_COLUMN + _currentSkill * 2;

    cwin_putat_string_raw(&cw, 0, 4, TXT[TXT_IDX_TASK_MANAGER_TABLE_HEADER_1], VCOL_YELLOW);
    cwin_putat_string_raw(&cw, 0, 5, TXT[TXT_IDX_TASK_MANAGER_TABLE_HEADER_2], VCOL_YELLOW);
    cwin_putat_string_raw(&cw, 0, 6, TXT[TXT_IDX_TASK_MANAGER_TABLE_HEADER_3], VCOL_YELLOW);
    cwin_putat_string_raw(&cw, 0, 7, TXT[TXT_IDX_TASK_MANAGER_TABLE_HEADER_4], VCOL_YELLOW);
    cwin_putat_string_raw(&cw, 0, 8, TXT[TXT_IDX_TASK_MANAGER_TABLE_HEADER_5], VCOL_YELLOW);
    cwin_putat_string_raw(&cw, 0, 9, TXT[TXT_IDX_TASK_MANAGER_TABLE_HEADER_6], VCOL_YELLOW);
    cwin_putat_string_raw(&cw, 0, 10, TXT[TXT_IDX_TASK_MANAGER_TABLE_HEADER_7], VCOL_YELLOW);
    cwin_putat_string_raw(&cw, 0, 11, TXT[TXT_IDX_TASK_MANAGER_TABLE_HEADER_8], VCOL_YELLOW);
    cwin_putat_string_raw(&cw, 0, 12, TXT[TXT_IDX_TASK_MANAGER_TABLE_HEADER_9], VCOL_YELLOW);
    cwin_putat_string_raw(&cw, 0, 13, TXT[TXT_IDX_TASK_MANAGER_TABLE_HEADER_10], VCOL_YELLOW);

    byte col1;
    byte col2[4];
    byte str[2];

    for(byte charSlot=0;charSlot<CHARACTER_SLOTS;charSlot++){
        // only check active chars
        if(characterSlots[charSlot] != NO_CHARACTER){
            byte character = characterSlots[charSlot];

            byte line = TASK_SHOW_LINE+character;
            if(character == _currentCharacter){
                col1 = VCOL_LT_GREEN;
                col2[0] = VCOL_LT_GREY;
                col2[1] = VCOL_LT_GREY;
                col2[2] = VCOL_LT_GREY;
                col2[3] = VCOL_LT_GREY;
                // mark current column with white
                col2[_currentSkill] = VCOL_WHITE;
            } else {
                col1 = VCOL_GREEN;
                col2[0] = VCOL_MED_GREY;
                col2[1] = VCOL_MED_GREY;
                col2[2] = VCOL_MED_GREY;
                col2[3] = VCOL_MED_GREY;
            }

            cwin_putat_string_raw(&cw, 0, line, TXT[allChars_nameIdx[character]], col1);

            cwin_putat_string_raw(&cw, 21, line, "\x7e", VCOL_YELLOW);

            sprintf(str, "%u", allChars_skills[character][SKILL_BREEDING]);
            cwin_putat_string_raw(&cw, 22,  line, str, col1);
            sprintf(str, "%u", allChars_skills[character][SKILL_FARMING]);
            cwin_putat_string_raw(&cw, 24,  line, str, col1);
            sprintf(str, "%u", allChars_skills[character][SKILL_BUTCHERY]);
            cwin_putat_string_raw(&cw, 26,  line, str, col1);
            sprintf(str, "%u", allChars_skills[character][SKILL_TRADE]);
            cwin_putat_string_raw(&cw, 28,  line, str, col1);
            cwin_putat_string_raw(&cw, 23, line, "\x7e", VCOL_YELLOW);
            cwin_putat_string_raw(&cw, 25, line, "\x7e", VCOL_YELLOW);
            cwin_putat_string_raw(&cw, 27, line, "\x7e", VCOL_YELLOW);
            cwin_putat_string_raw(&cw, 30, line, "\x7e", VCOL_YELLOW);


            sprintf(str, "%u", allChars_prios[character][SKILL_BREEDING]);
            cwin_putat_string_raw(&cw, 32,  line, str, col2[0]);
            sprintf(str, "%u", allChars_prios[character][SKILL_FARMING]);
            cwin_putat_string_raw(&cw, 34,  line, str, col2[1]);
            sprintf(str, "%u", allChars_prios[character][SKILL_BUTCHERY]);
            cwin_putat_string_raw(&cw, 36,  line, str, col2[2]);
            sprintf(str, "%u", allChars_prios[character][SKILL_TRADE]);
            cwin_putat_string_raw(&cw, 38,  line, str, col2[3]);

            cwin_putat_string_raw(&cw, 33, line, "\x7e", VCOL_YELLOW);
            cwin_putat_string_raw(&cw, 35, line, "\x7e", VCOL_YELLOW);
            cwin_putat_string_raw(&cw, 37, line, "\x7e", VCOL_YELLOW);
        }
    }
}

// CHANGE TO ACTIVE CHARACTERS
static void _upChar(){
    if(_currentCharacter > 0){
        _currentCharacter--;
    } else {
        _currentCharacter = CHARACTER_COUNT-1;
    }
    _showTaskPriorities();
}
static void _downChar(){
    if(_currentCharacter < CHARACTER_COUNT-1){
        _currentCharacter++;
    } else {
        _currentCharacter = 0;
    }
    _showTaskPriorities();
}

static void _skillLeft(){
    if(_currentSkill > 0){
        _currentSkill--;
    } else {
        _currentSkill = SKILL_COUNT-1;
    }
    _showTaskPriorities();
}
static void _skillRight(){
    if(_currentSkill < SKILL_COUNT-1){
        _currentSkill++;
    } else {
        _currentSkill = 0;
    }
    _showTaskPriorities();
}

static void _prioDown(){
    byte prio = allChars_prios[_currentCharacter][_currentSkill];
    if(prio > 0){
        prio--;
    } else {
        prio = MAX_PRIO;
    }
    allChars_prios[_currentCharacter][_currentSkill] = prio;
    _showTaskPriorities();
}

static void _prioUp(){
    byte prio = allChars_prios[_currentCharacter][_currentSkill];
    if(prio < MAX_PRIO){
        prio++;
    } else {
        prio = 0;
    }
    allChars_prios[_currentCharacter][_currentSkill] = prio;
    _showTaskPriorities();
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
    { TXT_IDX_MENU_TASK_MANAGER_W, 'w', &_upChar, 0, 2, 1},
    { TXT_IDX_MENU_TASK_MANAGER_S, 's', &_downChar, 0, 2, 3},
    { TXT_IDX_MENU_TASK_MANAGER_A, 'a', &_skillLeft, 0, 1, 2},
    { TXT_IDX_MENU_TASK_MANAGER_D, 'd', &_skillRight, 0, 3, 2},
    { TXT_IDX_MENU_TASK_MANAGER_PLUS, '+', &_prioDown, 0, 10, 1},
    { TXT_IDX_MENU_TASK_MANAGER_MINUS, '-', &_prioUp, 0, 10, 3},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, &_backToPreviousMenu, 0, 1, 20},

    END_MENU_CHOICES
};
