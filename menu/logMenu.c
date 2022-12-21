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

// how much space on screen msg takes, 9 is already reserved for timestamp
// so 2 screen lines after adding timestamp
#define LOG_MSGS_LENGTH 71
// how many pages we need depends on how many messages can be displayed per page
#define LOG_MSGS_PER_PAGE 8
#define LOG_PAGES_COUNT LOG_MAX_MSGS / LOG_MSGS_PER_PAGE
#define COL_OFFSET_LOGS 0
#define ROW_OFFSET_LOGS 4
#define LOG_LINE LOG_MSGS[i]
#define LOG_LINE_DATA_CONTEXT LOG_MSGS[i].data[0]
#define LOG_LINE_TASK_WORKER LOG_MSGS[i].data[2]
static byte _currentPage = 0;
static byte _timestamp[9];
static byte _str[LOG_MSGS_LENGTH];

static void _prepareTaskLogData(byte i){
    switch(LOG_LINE_DATA_CONTEXT) {
        case LOG_DATA_CONTEXT_TASK_ASSIGNED_TO_WORKER:
            sprintf(_str, p"Task [""%s"p"] assigned to char [""%u""]", TXT[LOG_LINE_TASK_NAMEIDX], LOG_LINE_TASK_WORKER);
            break;
        case LOG_DATA_CONTEXT_TASK_NEW_TASK:
            sprintf(_str, p"New task created [""%s""]", TXT[LOG_LINE_TASK_NAMEIDX]);
            break;
        case LOG_DATA_CONTEXT_TASK_REMOVE_TASK:
            sprintf(_str, p"Task [""%s"p"] removed, status [""%u"p"]", TXT[LOG_LINE_TASK_NAMEIDX], LOG_LINE_TASK_STATUS);
            break;
        case LOG_DATA_CONTEXT_TASK_STATUS_UNKNOWN:
            sprintf(_str, p"UNHANDLED STATUS: Task [""%s"p"] char[""%u"p"] status [""%u"p"]", TXT[LOG_LINE_TASK_NAMEIDX], LOG_LINE_TASK_WORKER, LOG_LINE_TASK_STATUS);
            break;
    }
}

static void _showLogs(){
    cwin_clear(&cw);

    // static menu texts
    cwin_putat_string_raw(&cw, 0, 0, TXT[TXT_IDX_LOG_MENU_HEADER], VCOL_GREEN);

    displayMenu(LOG_MENU);
    // header
    cwin_putat_string_raw(&cw, COL_OFFSET_LOGS, ROW_OFFSET_LOGS-1, TXT[TXT_IDX_LOG_MENU_LOG_TABLE_HEADER], VCOL_YELLOW);

    byte logIndex = _currentPage * LOG_MSGS_PER_PAGE;
    byte row = 0;
    for(byte i=logIndex;i<logIndex+LOG_MSGS_PER_PAGE;i++){
        byte color = VCOL_GREEN;
        // print task marker
        if(LOG_LINE.msgId & LOG_DEBUG){
            color = VCOL_MED_GREY;
        } else if(LOG_LINE.msgId & LOG_ERROR){
            color = VCOL_RED;
        }

        // print timestamp
        sprintf(_timestamp, "%02u.%02u %02u", LOG_LINE.day, LOG_LINE.month, LOG_LINE.hour);
        cwin_putat_string_raw(&cw, COL_OFFSET_LOGS, ROW_OFFSET_LOGS+row, _timestamp, color);

        // now decode the msg type and act on it
        byte msgId = LOG_LINE.msgId & 0b11111100;
        switch(msgId) {
            // ist just some text, print it
            case LOG_MSG_TEXT:
                cwin_putat_chars(&cw, COL_OFFSET_LOGS+9, ROW_OFFSET_LOGS+row, LOG_LINE.data, LOG_DATA_SIZE, color);
                break;

            // special cases - message types - each has to be handled on its own
            //Each context should have its own handler routine, passing the current Log index 'i' into it
            case LOG_MSG_TASK:
                _prepareTaskLogData(i);
                cwin_putat_string(&cw, COL_OFFSET_LOGS+9, ROW_OFFSET_LOGS+row, _str, color);
                break;
        }

        row+=2;
   }
}

static void _upPage(){
    if(_currentPage > 0){
        _currentPage--;
    } else {
        _currentPage = LOG_PAGES_COUNT-1;
    }
    _showLogs();
}
static void _downPage(){
    if(_currentPage < LOG_PAGES_COUNT-1){
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

    _showLogs();
}

const struct MenuOption LOG_MENU[] = {
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, UI_LR, &_backToPreviousMenu, 0, 1, 21 },
    { TXT_IDX_MENU_TASK_MANAGER_W, 'w', UI_U+UI_HIDE, &_upPage, 0, 0, ROW_OFFSET_LOGS-1 },
    { TXT_IDX_MENU_TASK_MANAGER_S, 's', UI_D+UI_HIDE, &_downPage, 0, 0, ROW_OFFSET_LOGS+10 },

    END_MENU_CHOICES
};
