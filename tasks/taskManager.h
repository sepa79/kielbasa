#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H
#include <c64/types.h>
#include <stdbool.h>

// Task arrays

// max bytes in task
#define TASK_PARAMS_SIZE 5
#define TASK_ARRAY_SIZE 10
#define NO_TASK 0xff

// index table - holds indexes of 'real' data tables, used to simplify sorting tasks when elements are added or removed
extern byte taskRef[TASK_ARRAY_SIZE];
// Function to handle task
extern void (*task_codeRef[TASK_ARRAY_SIZE])(byte);
// Short name displayed on screen
extern byte task_nameIdx[TASK_ARRAY_SIZE];
extern byte task_desc[TASK_ARRAY_SIZE][19];
extern byte task_params[TASK_ARRAY_SIZE][TASK_PARAMS_SIZE];
extern byte task_minReqStat[TASK_ARRAY_SIZE][3];
extern byte task_minReqSkill[TASK_ARRAY_SIZE][4];
// which skill is used for this task, 0-3
extern byte task_reqType[TASK_ARRAY_SIZE];

// used when creating new tasks
struct Task {
    // Function to handle it
    void (*codeRef)(byte);

    // Short name displayed on screen
    byte nameIdx;
    byte desc[19];
    byte params[TASK_PARAMS_SIZE];

    byte minReqStat[3];
    
    byte minReqSkill[4];
    byte reqType;
};

void displayTaskList();
void initTaskList();
void tasksTick();
bool addTask(struct Task * task);
void removeTask(byte taskRefId);

#pragma compile("taskManager.c")

#endif