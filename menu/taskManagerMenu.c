#include <c64/charwin.h>
#include <c64/keyboard.h>
#include <c64/types.h>
#include <c64/vic.h>
#include <c64/easyflash.h>

#include <tasks/taskManager.h>
#include <menu/menuSystem.h>
#include <translation/common.h>
#include <assets/assetsSettings.h>
#include <engine/gameSettings.h>

#define COL_OFFSET_TASKLIST_TM 2
#define ROW_OFFSET_TASKLIST_TM 4
static byte _currentTask = 0;

static void _showTasks(){
    // header
    cwin_putat_string_raw(&cw, COL_OFFSET_TASKLIST_TM, ROW_OFFSET_TASKLIST_TM-1, TXT[TXT_IDX_TASK_MANAGER_TABLE_HEADER], VCOL_YELLOW);

    // tasks list
    for(byte i=0;i<TASK_ARRAY_SIZE;i++){
        byte color = VCOL_GREEN;
        // print task marker
        if(_currentTask == i){
            color = VCOL_LT_GREEN;
            cwin_putat_string_raw(&cw, COL_OFFSET_TASKLIST_TM-2, ROW_OFFSET_TASKLIST_TM+i, s">", VCOL_WHITE);
        } else {
            cwin_putat_string_raw(&cw, COL_OFFSET_TASKLIST_TM-2, ROW_OFFSET_TASKLIST_TM+i, s" ", VCOL_WHITE);
        }

        byte taskId = taskRef[i];
        cwin_putat_string_raw(&cw, COL_OFFSET_TASKLIST_TM, ROW_OFFSET_TASKLIST_TM+i, TXT[task_nameIdx[taskId]], color);
        cwin_putat_string_raw(&cw, COL_OFFSET_TASKLIST_TM+8, ROW_OFFSET_TASKLIST_TM+i, "\x7e", VCOL_YELLOW);
        cwin_putat_string_raw(&cw, COL_OFFSET_TASKLIST_TM+9, ROW_OFFSET_TASKLIST_TM+i, task_desc[taskId], color);

    byte str[10];
    sprintf(str, "%2u %2u", i, taskId);

        cwin_putat_string_raw(&cw, COL_OFFSET_TASKLIST_TM+25, ROW_OFFSET_TASKLIST_TM+i, str, color);
    }
}

static void _upRow(){
    if(_currentTask > 0){
        _currentTask--;
    } else {
        _currentTask = TASK_ARRAY_SIZE-1;
    }
    _showTasks();
}
static void _downRow(){
    if(_currentTask < TASK_ARRAY_SIZE-1){
        _currentTask++;
    } else {
        _currentTask = 0;
    }
    _showTasks();
}

static void _deleteTask(){
    removeTaskByRef(_currentTask);
    _showTasks();
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
    _showTasks();
}

const struct MenuOption TASK_MANAGER_MENU[] = {
    { TXT_IDX_MENU_TASK_MANAGER_W, 'w', &_upRow, 0, 0, ROW_OFFSET_TASKLIST_TM-1 },
    { TXT_IDX_MENU_TASK_MANAGER_S, 's', &_downRow, 0, 0, ROW_OFFSET_TASKLIST_TM+10 },
    { TXT_IDX_MENU_TASK_MANAGER_MINUS, '-', &_deleteTask, 0, 0, 20 },
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, &_backToPreviousMenu, 0, 20, 20 },

    END_MENU_CHOICES
};