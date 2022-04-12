#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H
#include <c64/types.h>
#include <stdbool.h>

// Task arrays
// Function to handle it
// extern word task_codeRef[10];

// max bytes in task
#define TASK_PARAMS_SIZE 5
extern void (*task_codeRef[10])(byte);
// Short name displayed on screen
extern byte task_nameIdx[10];
extern byte task_desc[10][19];
extern byte task_params[10][TASK_PARAMS_SIZE];
extern byte task_minReqInt[10];
extern byte task_minReqStr[10];
extern byte task_minReqCun[10];
extern byte task_minReqAni[10];
extern byte task_minReqFrm[10];
extern byte task_minReqBth[10];
extern byte task_minReqTrd[10];
// which skill is used for this task, 0-3
extern byte task_reqType[10];

// used when creating new tasks
struct Task {
    // Function to handle it
    void (*codeRef)(byte);

    // Short name displayed on screen
    byte nameIdx;
    byte desc[19];
    byte params[TASK_PARAMS_SIZE];

    byte minReqInt;
    byte minReqStr;
    byte minReqCun;
    
    byte minReqAni;
    byte minReqFrm;
    byte minReqBth;
    byte minReqTrd;
    byte reqType;
};

#define SKILL_HERDING 0
#define SKILL_FARMING 1
#define SKILL_BUTCHERY 2
#define SKILL_TRADE 3

void displayTaskList();
void initTaskList();
void tasksTick();
bool addTask(struct Task * task);
byte findWorkerForTask(byte reqType);

#pragma compile("taskManager.c")

#endif