#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H
#include <c64/types.h>
#include <stdbool.h>
#include <tick/calendar.h>

// Task arrays

// max bytes in task
#define TASK_PARAMS_SIZE 5
#define TASK_ARRAY_SIZE 10
#define NO_TASK 0xff

#define TASK_STATUS_NOTASK 0
#define TASK_STATUS_NEW 1
#define TASK_STATUS_REMOVE 2
#define TASK_STATUS_DONE 4

// index table - holds indexes of 'real' data tables, used to simplify sorting tasks when elements are added or removed
extern byte taskRef[TASK_ARRAY_SIZE];
// Function to handle task, parameter should by task index
extern void (*task_codeRef[TASK_ARRAY_SIZE])(byte);
// Short name displayed on screen
extern byte task_nameIdx[TASK_ARRAY_SIZE];
extern byte task_desc[TASK_ARRAY_SIZE][19];
extern byte task_params[TASK_ARRAY_SIZE][TASK_PARAMS_SIZE];
// which skill is used for this task, 0-3
extern byte task_reqType[TASK_ARRAY_SIZE];
// who does it (when started, also indicates that task is worked on)
extern byte task_worker[TASK_ARRAY_SIZE];
// percentage done - shown in the table - does not need to go to 100, its just an indicator
// e.g. sowing fields can 'finish' at any percent if there are not enough seeds
// extern byte task_percentDone[TASK_ARRAY_SIZE];
// task icon
extern char * task_icon[TASK_ARRAY_SIZE];
// one of the task statuses, tells the task handler why it is called
extern byte task_status[TASK_ARRAY_SIZE];

// used when creating new tasks
struct Task {
    // Function to handle it
    void (*codeRef)(byte);

    // Short name displayed on screen
    char nameIdx;
    char desc[19];
    char params[TASK_PARAMS_SIZE];

    char reqType;
    char * icon;
    char status;
};

//-----------------------------------------------------------------------------------------
#pragma code ( gameInitRAMCode )
#pragma data ( gameInitData )
//-----------------------------------------------------------------------------------------

void initTaskList();

//-----------------------------------------------------------------------------------------
#pragma code ( ticksCode )
#pragma data ( data )
//-----------------------------------------------------------------------------------------

void tasksTick();

//-----------------------------------------------------------------------------------------
#pragma code ( code )
#pragma data ( data )
//-----------------------------------------------------------------------------------------
// Visible in RAM, but will switch banks and call the 'body' in ROM

bool addTask(struct Task * task);
void removeTaskByRef(byte taskRefId);
void removeTask(byte taskRefId);
void unassignTask(byte taskId);
void finishTask(byte taskId);

// log helper
void setTaskLogMsg(byte taskId);


#pragma compile("taskManager.c")

#endif