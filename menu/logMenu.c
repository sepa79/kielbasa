#include <c64/charwin.h>
#include <c64/keyboard.h>
#include <c64/types.h>
#include <c64/vic.h>
#include <c64/easyflash.h>
#include <c64/memmap.h>

#include <engine/logger.h>
#include <engine/easyFlashBanks.h>
#include <engine/gameSettings.h>
#include <assets/assetsSettings.h>
#include <translation/common.h>
#include <menu/menuSystem.h>

// this menu is special, might be called anytime and should not overwrite the menu memory
// Sections and regions
#pragma section( LogMenuCode, 0 )
#pragma section( LogMenuData, 0 )
#pragma region( LogMenuRegion, 0x8000, 0xa000, , MENU_BANK_SPECIAL_MENUS, { LogMenuCode, LogMenuData } )

// menu code is in ROM - data in RAM
#pragma data ( data )

// how much space on screen msg takes, 9 is already reserved for timestamp
// so 2 screen lines after adding timestamp
#define LOG_MSGS_LENGTH 71
// how many pages we need depends on how many messages can be displayed per page
#define LOG_MSGS_PER_PAGE 8
#define LOG_PAGES_COUNT LOG_MAX_MSGS / LOG_MSGS_PER_PAGE
#define COL_OFFSET_LOGS 0
#define ROW_OFFSET_LOGS 4
static byte _currentPage = 0;
static byte _timestamp[9];
static byte _str[LOG_MSGS_LENGTH];
static byte _pBank = 0;

// this one needs to be in RAM
#pragma code ( code )

// copy of current log message, so that ROM code can work on it
static struct LogMsg currentLogMsg;

static void _copyCurrentLogMsg(byte i){
    // set ROM into mode that allows us to read A000-B000
    __asm{ sei };
    mmap_set(MMAP_NO_ROM);
    currentLogMsg = LOG_MSGS[i];
    // revert ROM
    mmap_set(MMAP_ROM);
    __asm{ cli };
}

// menu code is in ROM - data in RAM
#pragma code ( LogMenuCode )
#pragma data ( data )

// formatters run from ROM
static void _prepareReportLogData(){
    switch(currentLogMsg.LOG_DATA_CONTEXT) {
        case LOG_DATA_CONTEXT_WEATHER_REPORT:
            sprintf(_str, p"T [""%d"p"] R [""%u"p"], rT [""%d"p"] rR [""%u"p"] rTW [""%d"p"] rRW [""%u"p"], AT [""%d"p"] AR [""%u"p"]",
                (signed char)(currentLogMsg.data[1]),
                currentLogMsg.data[2],
                (signed char)(currentLogMsg.data[3]),
                currentLogMsg.data[4],
                (signed char)(currentLogMsg.data[5]),
                currentLogMsg.data[6],
                (signed char)(currentLogMsg.data[7]),
                currentLogMsg.data[8]
                );
            break;
    }
}

static void _prepareTaskLogData(){
    switch(currentLogMsg.LOG_DATA_CONTEXT) {
        case LOG_DATA_CONTEXT_TASK_ASSIGNED_TO_WORKER:
            sprintf(_str, p"Task [""%s"p"] assigned to char [""%u""]", TXT[currentLogMsg.LOG_DATA_TASK_NAMEIDX], currentLogMsg.LOG_DATA_TASK_WORKER);
            break;
        case LOG_DATA_CONTEXT_TASK_NEW_TASK:
            sprintf(_str, p"New task created [""%s""]", TXT[currentLogMsg.LOG_DATA_TASK_NAMEIDX]);
            break;
        case LOG_DATA_CONTEXT_TASK_REMOVE_TASK:
            sprintf(_str, p"Task [""%s"p"] removed, status [""%u"p"]", TXT[currentLogMsg.LOG_DATA_TASK_NAMEIDX], currentLogMsg.LOG_DATA_TASK_STATUS);
            break;
        case LOG_DATA_CONTEXT_TASK_STATUS_UNKNOWN:
            sprintf(_str, p"UNHANDLED STATUS: Task [""%s"p"] char[""%u"p"] status [""%u"p"]", TXT[currentLogMsg.LOG_DATA_TASK_NAMEIDX], currentLogMsg.LOG_DATA_TASK_WORKER, currentLogMsg.LOG_DATA_TASK_STATUS);
            break;

        // farm tasks
        case LOG_DATA_CONTEXT_TASK_FARM_SOW_ENTRY:
            sprintf(_str, p"Processing Sow Field Task [""%u"p"] char[""%u"p"] status [""%u"p"] field [""%u"p"] plant [""%u"p"] ", currentLogMsg.LOG_DATA_TASK_ID, currentLogMsg.LOG_DATA_TASK_WORKER, currentLogMsg.LOG_DATA_TASK_STATUS,
                currentLogMsg.LOG_DATA_TASK_PARAMS2, currentLogMsg.LOG_DATA_TASK_PARAMS1);
            break;
        case LOG_DATA_CONTEXT_TASK_FARM_SOW_EXIT:
            sprintf(_str, p"Processed Sow Field Task [""%u"p"] energy used [""%u"p"] seeds used [""%u"p"]", currentLogMsg.LOG_DATA_TASK_ID, currentLogMsg.LOG_DATA_TASK_PARAMS3, currentLogMsg.LOG_DATA_TASK_PARAMS4);
            break;
        case LOG_DATA_CONTEXT_TASK_FARM_SOW_DONE:
            sprintf(_str, p"Finished Sow Field Task [""%u"p"] char[""%u"p"] status [""%u"p"] field [""%u"p"] plant [""%u"p"] ", currentLogMsg.LOG_DATA_TASK_ID, currentLogMsg.LOG_DATA_TASK_WORKER, currentLogMsg.LOG_DATA_TASK_STATUS,
                currentLogMsg.LOG_DATA_TASK_PARAMS2, currentLogMsg.LOG_DATA_TASK_PARAMS1);
            break;
        case LOG_DATA_CONTEXT_TASK_FARM_SOW_NOT_ENOUGH_ENERGY:
            sprintf(_str, p"No energy - dropped Task [""%u"p"] char[""%u"p"] status [""%u"p"] field [""%u"p"] plant [""%u"p"] ", currentLogMsg.LOG_DATA_TASK_ID, currentLogMsg.LOG_DATA_TASK_WORKER, currentLogMsg.LOG_DATA_TASK_STATUS,
                currentLogMsg.LOG_DATA_TASK_PARAMS2, currentLogMsg.LOG_DATA_TASK_PARAMS1);
            break;
        case LOG_DATA_CONTEXT_TASK_FARM_SOW_NOT_ENOUGH_SEEDS:
            sprintf(_str, p"No seeds - finished Task [""%u"p"] char[""%u"p"] status [""%u"p"] field [""%u"p"] plant [""%u"p"] ", currentLogMsg.LOG_DATA_TASK_ID, currentLogMsg.LOG_DATA_TASK_WORKER, currentLogMsg.LOG_DATA_TASK_STATUS,
                currentLogMsg.LOG_DATA_TASK_PARAMS2, currentLogMsg.LOG_DATA_TASK_PARAMS1);
            break;

        // kitchen tasks
        case LOG_DATA_CONTEXT_TASK_KITCHEN_BAKE_BREAD:
            sprintf(_str, p"Processing Bake Bread Task [""%u"p"] char[""%u"p"] status [""%u"p"]", currentLogMsg.LOG_DATA_TASK_ID, currentLogMsg.LOG_DATA_TASK_WORKER, currentLogMsg.LOG_DATA_TASK_STATUS);
            break;


    }
}

static void _showLogs(){
    // init screen
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, BIG_SCREEN_Y_START, SCREEN_WIDTH, BIG_SCREEN_HEIGHT);
    cwin_clear(&cw);

    // static menu texts
    cwin_putat_string_raw(&cw, 0, 0, TXT[TXT_IDX_LOG_MENU_HEADER], VCOL_GREEN);
    sprintf(_str, s"Page"" %02u/%02u", _currentPage+1, LOG_PAGES_COUNT);
    cwin_putat_string_raw(&cw, 30, 0, _str, VCOL_GREEN);

    displayMenu(LOG_MENU);
    // header
    cwin_putat_string_raw(&cw, COL_OFFSET_LOGS, ROW_OFFSET_LOGS-1, TXT[TXT_IDX_LOG_MENU_LOG_TABLE_HEADER], VCOL_YELLOW);

    byte logIndex = _currentPage * LOG_MSGS_PER_PAGE;
    byte row = 0;
    for(byte i=logIndex;i<logIndex+LOG_MSGS_PER_PAGE;i++){
        _copyCurrentLogMsg(i);
        byte color = VCOL_GREEN;
        // print task marker
        if(currentLogMsg.msgId & LOG_DEBUG){
            color = VCOL_MED_GREY;
        } else if(currentLogMsg.msgId & LOG_ERROR){
            color = VCOL_RED;
        }

        // print timestamp
        sprintf(_timestamp, "%02u.%02u %02u", currentLogMsg.day, currentLogMsg.month, currentLogMsg.hour);
        cwin_putat_string_raw(&cw, COL_OFFSET_LOGS, ROW_OFFSET_LOGS+row, _timestamp, color);

        // now decode the msg type and act on it
        byte msgId = currentLogMsg.msgId & 0b11111100;
        switch(msgId) {
            // ist just some text, print it
            case LOG_MSG_TEXT:
                memcpy(_str, currentLogMsg.data, LOG_DATA_SIZE);
                break;

            // special cases - message types - each has to be handled on its own
            // Each context should have its own handler routine, passing the current Log index 'i' into it
            case LOG_MSG_TASK:
                _prepareTaskLogData();
                break;
            case LOG_MSG_REPORT:
                _prepareReportLogData();
                break;

        }
        cwin_putat_string(&cw, COL_OFFSET_LOGS+9, ROW_OFFSET_LOGS+row, _str, color);
        _str[0] = 0;

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

#pragma data ( LogMenuData )

const struct MenuOption LOG_MENU[] = {
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_TRANSITION, UI_LF, &backToPreviousMenu, 0, 1, 21 },
    { TXT_IDX_MENU_TASK_MANAGER_W, 'w', SCREEN_FULL_TXT, UI_U+UI_HIDE, &_upPage, 0, 0, ROW_OFFSET_LOGS-1 },
    { TXT_IDX_MENU_TASK_MANAGER_S, 's', SCREEN_FULL_TXT, UI_D+UI_HIDE, &_downPage, 0, 0, ROW_OFFSET_LOGS+10 },

    END_MENU_CHOICES
};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )

void showLogMenu(){
    // switch to cart and call menu code
    setBank(MENU_BANK_SPECIAL_MENUS);
    
    _showLogs();
    switchScreenTo(SCREEN_FULL_TXT);
}
