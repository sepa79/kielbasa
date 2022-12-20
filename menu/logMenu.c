#include <c64/charwin.h>
#include <c64/keyboard.h>
#include <c64/types.h>
#include <c64/vic.h>
#include <c64/easyflash.h>

#include <engine/logger.h>
#include <menu/menuSystem.h>
#include <translation/common.h>
#include <assets/assetsSettings.h>
#include <engine/gameSettings.h>

// how many pages we need depends on how many messages can be displayed per page
#define MSG_PER_PAGE 8
#define PAGES_COUNT LOG_MAX_MSGS / MSG_PER_PAGE
#define COL_OFFSET_LOGS 0
#define ROW_OFFSET_LOGS 4
#define LOG_LINE LOG_MSGS[i]
static byte _currentPage = 0;

static void _showLogs(){
    // header
    cwin_putat_string_raw(&cw, COL_OFFSET_LOGS, ROW_OFFSET_LOGS-1, TXT[TXT_IDX_TASK_MANAGER_TABLE_HEADER], VCOL_YELLOW);

    byte logIndex = _currentPage * MSG_PER_PAGE;
    byte timestamp[9];
    byte row = 0;
    for(byte i=logIndex;i<logIndex+MSG_PER_PAGE;i++){
        byte color = VCOL_GREEN;
        // print task marker
        if(LOG_LINE.msgId & LOG_DEBUG){
            color = VCOL_MED_GREY;
        } else if(LOG_LINE.msgId & LOG_ERROR){
            color = VCOL_RED;
        }

        sprintf(timestamp, "%02u.%02u %02u", LOG_LINE.day, LOG_LINE.month, LOG_LINE.hour);
        cwin_putat_string_raw(&cw, COL_OFFSET_LOGS, ROW_OFFSET_LOGS+row, timestamp, color);

        // now decode the msg type and act on it
        byte msgId = LOG_LINE.msgId & 0b11111100;
        switch(msgId) {
            case LOG_MSG_TEXT:
                cwin_putat_chars(&cw, COL_OFFSET_LOGS+9, ROW_OFFSET_LOGS+row, LOG_LINE.data, 10, color);
                break;

            case LOG_MSG_TASK:
                break;
        }

        row++;
   }
}

static void _upPage(){
    if(_currentPage > 0){
        _currentPage--;
    } else {
        _currentPage = PAGES_COUNT-1;
    }
    _showLogs();
}
static void _downPage(){
    if(_currentPage < PAGES_COUNT-1){
        _currentPage++;
    } else {
        _currentPage = 0;
    }
    _showLogs();
}

static void _backToPreviousMenu(){
    gms_textMode = false;
    gms_disableTimeControls = false;
    // This will simply call currently mounted menu again
    showMenu();
}

void showLogMenu(){
    gms_textMode = true;
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, BIG_SCREEN_Y_START, SCREEN_WIDTH, BIG_SCREEN_HEIGHT);
    cwin_clear(&cw);

    // static menu texts
    cwin_putat_string_raw(&cw, 0, 0, TXT[TXT_IDX_TASK_MANAGER_HEADER], VCOL_GREEN);

    displayMenu(LOG_MENU);
    _showLogs();
}

const struct MenuOption LOG_MENU[] = {
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, UI_LR, &_backToPreviousMenu, 0, 20, 20 },
    { TXT_IDX_MENU_TASK_MANAGER_W, 'w', UI_U+UI_HIDE, &_upPage, 0, 0, ROW_OFFSET_LOGS-1 },
    { TXT_IDX_MENU_TASK_MANAGER_S, 's', UI_D+UI_HIDE, &_downPage, 0, 0, ROW_OFFSET_LOGS+10 },

    END_MENU_CHOICES
};
