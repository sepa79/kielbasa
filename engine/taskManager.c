#include <c64/types.h>
#include <c64/vic.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "taskManager.h"
#include <translation/common.h>
#include <character/character.h>

void (*task_codeRef[10])(byte);
// Short name displayed on screen
byte task_nameIdx[10];
// word task_codeRef[10];
byte task_desc[10][19];
byte task_params[10][TASK_PARAMS_SIZE];
byte task_minReqInt[10];
byte task_minReqStr[10];
byte task_minReqCun[10];
byte task_minReqAni[10];
byte task_minReqFrm[10];
byte task_minReqBth[10];
byte task_minReqTrd[10];

// column offset for printing data
#define COL_OFFSET_TASKLIST 12

void initTaskList() {
    for(byte i=0;i<10;i++){
        task_nameIdx[i] = TXT_IDX_TASK_EMPTY_NAME;
        // task_descIdx[i] = EMPTY_TASK_DESC;
        // task_params[i]  =
        strcpy(task_desc[i], TXT[TXT_IDX_TASK_EMPTY_DESCRIPTION]);
    }
}

bool addTask(struct Task * task){
    // 0 as a requirement - empty task
    byte i = 0;
    do {
        if(task_minReqInt[i] == 0){
            // empty task - add here
            task_codeRef[i]   = task->codeRef;
            task_nameIdx[i]   = task->nameIdx;
            task_params[i][0] = task->params[0];
            task_params[i][1] = task->params[1];
            task_params[i][2] = task->params[2];
            task_params[i][3] = task->params[3];
            task_params[i][4] = task->params[4];
            task_minReqInt[i] = task->minReqInt;
            task_minReqStr[i] = task->minReqStr;
            task_minReqCun[i] = task->minReqCun;
            task_minReqAni[i] = task->minReqAni;
            task_minReqFrm[i] = task->minReqFrm;
            task_minReqBth[i] = task->minReqBth;
            task_minReqTrd[i] = task->minReqTrd;
            strcpy(task_desc[i], task->desc);
            return true;
        }
        i++;
    } while (i < 10);
    setErrorCursor();
    return false;
}

void displayTaskList(){
    // header
    cwin_putat_string_raw(&cw, COL_OFFSET_TASKLIST, 1, TXT[TXT_IDX_TASK_LIST_HEADER], VCOL_YELLOW);

    // tasks list
    for(byte i=0;i<10;i++){
        cwin_putat_string_raw(&cw, COL_OFFSET_TASKLIST, 2+i, TXT[task_nameIdx[i]], VCOL_GREEN);
        cwin_putat_string_raw(&cw, COL_OFFSET_TASKLIST+8, 2+i, "\x7e", VCOL_YELLOW);
        cwin_putat_string_raw(&cw, COL_OFFSET_TASKLIST+9, 2+i, task_desc[i], VCOL_GREEN);
    }
}

static void findWorker(byte taskIdx) {
    for(byte charSlot=0;charSlot<4;charSlot++){
        // only check active chars
        if(characterSlots[charSlot] != 0xff){
            decEnergyLevel(charSlot, 5);
        }
    }
}

void tasksTick(){
    // 0 as a requirement - empty task
    byte i = 0;
    do {
        if(task_minReqInt[i] == 0){
            return;
        }
        // task found, delegate
        findWorker(i);
        i++;
    } while (i < 10);
}