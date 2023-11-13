// UNFUNUSHED - NEED TO ITERATE ONLY OVER ACTIVE CHARS


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

static void _showTaskPriorities(){

    cwin_putat_string_raw(&cw, 0,  4, TXT_TASK_MANAGER_PRIO_TABLE_HEADER_1, VCOL_YELLOW);
    cwin_putat_string_raw(&cw, 0,  5, TXT_TASK_MANAGER_PRIO_TABLE_HEADER_2, VCOL_YELLOW);
    cwin_putat_string_raw(&cw, 0,  6, TXT_TASK_MANAGER_PRIO_TABLE_HEADER_3, VCOL_YELLOW);
    cwin_putat_string_raw(&cw, 0,  7, TXT_TASK_MANAGER_PRIO_TABLE_HEADER_4, VCOL_YELLOW);
    cwin_putat_string_raw(&cw, 0,  8, TXT_TASK_MANAGER_PRIO_TABLE_HEADER_5, VCOL_YELLOW);
    cwin_putat_string_raw(&cw, 0,  9, TXT_TASK_MANAGER_PRIO_TABLE_HEADER_6, VCOL_YELLOW);
    cwin_putat_string_raw(&cw, 0, 10, TXT_TASK_MANAGER_PRIO_TABLE_HEADER_7, VCOL_YELLOW);
    cwin_putat_string_raw(&cw, 0, 11, TXT_TASK_MANAGER_PRIO_TABLE_HEADER_8, VCOL_YELLOW);
    cwin_putat_string_raw(&cw, 0, 12, TXT_TASK_MANAGER_PRIO_TABLE_HEADER_9, VCOL_YELLOW);
    cwin_putat_string_raw(&cw, 0, 13, TXT_TASK_MANAGER_PRIO_TABLE_HEADER_10, VCOL_YELLOW);

    char col1;
    char col2[4];
    char str[3];

    for(char charSlot=0;charSlot<CHARACTER_SLOTS;charSlot++){
        // only check active chars
        char charIdx = characterSlots[charSlot];
        if(charIdx != NO_CHARACTER){

            char line = TASK_SHOW_LINE+charSlot;
            if(charSlot == _currentCharacter){
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

            // cwin_putat_string_raw(&cw, 0, line, TXT[allCharacters[charIdx].nameIdx], col1);  // TXT_PL_ARRAY
            cwin_putat_string_raw(&cw, 0, line, allCharacters[charIdx].nameIdx, col1);  // TXT_PL_ARRAY

            cwin_putat_string_raw(&cw, 21, line, TBL_V, VCOL_YELLOW);

            // sprintf(str, "%2u", allCharacters[charIdx].skill[SKILL_ANIMALS]);
            // cwin_putat_string_raw(&cw, 22,  line, str, col1);
            // sprintf(str, "%2u", allCharacters[charIdx].skill[SKILL_FARMING]);
            // cwin_putat_string_raw(&cw, 24,  line, str, col1);
            // sprintf(str, "%2u", allCharacters[charIdx].skill[SKILL_COOKING]);
            // cwin_putat_string_raw(&cw, 26,  line, str, col1);
            // sprintf(str, "%2u", allCharacters[charIdx].skill[SKILL_TRADE]);
            // cwin_putat_string_raw(&cw, 28, line, str, col1);
            // cwin_putat_string_raw(&cw, 23, line, TBL_V, VCOL_YELLOW);
            // cwin_putat_string_raw(&cw, 25, line, TBL_V, VCOL_YELLOW);
            // cwin_putat_string_raw(&cw, 27, line, TBL_V, VCOL_YELLOW);
            // cwin_putat_string_raw(&cw, 30, line, TBL_V, VCOL_YELLOW);


            sprintf(str, "%u", allCharacters[charIdx].prio[SKILL_ANIMALS]);
            cwin_putat_string_raw(&cw, 32,  line, str, col2[0]);
            sprintf(str, "%u", allCharacters[charIdx].prio[SKILL_FARMING]);
            cwin_putat_string_raw(&cw, 34,  line, str, col2[1]);
            sprintf(str, "%u", allCharacters[charIdx].prio[SKILL_COOKING]);
            cwin_putat_string_raw(&cw, 36,  line, str, col2[2]);
            sprintf(str, "%u", allCharacters[charIdx].prio[SKILL_TRADE]);
            cwin_putat_string_raw(&cw, 38,  line, str, col2[3]);

            cwin_putat_string_raw(&cw, 33, line, TBL_V, VCOL_YELLOW);
            cwin_putat_string_raw(&cw, 35, line, TBL_V, VCOL_YELLOW);
            cwin_putat_string_raw(&cw, 37, line, TBL_V, VCOL_YELLOW);
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
    byte prio = allCharacters[_currentCharacter].prio[_currentSkill];
    if(prio > 0){
        prio--;
    } else {
        prio = MAX_PRIO;
    }
    allCharacters[_currentCharacter].prio[_currentSkill] = prio;
    _showTaskPriorities();
}

static void _prioUp(){
    byte prio = allCharacters[_currentCharacter].prio[_currentSkill];
    if(prio < MAX_PRIO){
        prio++;
    } else {
        prio = 0;
    }
    allCharacters[_currentCharacter].prio[_currentSkill] = prio;
    _showTaskPriorities();
}

void showTaskManagerPrioMenu(){
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, BIG_SCREEN_Y_START, SCREEN_WIDTH, BIG_SCREEN_HEIGHT);
    cwin_clear(&cw);

    // copy text task manager cache from cart to memory
    loadCacheTxt(TXT_TASK_MANAGER_CACHE_INDEX);

    // static menu texts
    cwin_putat_string_raw(&cw, 0, 0, TXT_TASK_MANAGER_PRIO_HEADER, VCOL_GREEN);

    displayMenu(TASK_MANAGER_PRIO_MENU);
    _showTaskPriorities();
    switchScreenTo(SCREEN_FULL_TXT);
}

const struct MenuOption TASK_MANAGER_PRIO_MENU[] = {
    { TXT_MENU_TASK_MANAGER_PLUS, '+', SCREEN_FULL_TXT, UI_SELECT, &_prioDown, 0, 10, 1},
    { TXT_MENU_TASK_MANAGER_MINUS, '-', SCREEN_FULL_TXT, UI_SELECT, &_prioUp, 0, 10, 3},
    { TXT_MENU_W, 'w', SCREEN_FULL_TXT, UI_SELECT, &_upChar, 0, 2, 1},
    { TXT_MENU_A, 'a', SCREEN_FULL_TXT, UI_L+UI_HIDE, &_skillLeft, 0, 1, 2},
    { TXT_MENU_S, 's', SCREEN_FULL_TXT, UI_SELECT, &_downChar, 0, 2, 3},
    { TXT_MENU_D, 'd', SCREEN_FULL_TXT, UI_R+UI_HIDE, &_skillRight, 0, 3, 2},
    { TXT_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_TRANSITION, UI_LF, &backToPreviousMenu, 0, 1, 20},

    END_MENU_CHOICES
};
