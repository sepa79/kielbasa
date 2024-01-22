#include <c64/vic.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <engine/logger.h>

#include <engine/easyFlashBanks.h>
#include <engine/logger.h>
#include <tasks/taskManager.h>
#include <translation/common.h>
#include <character/character.h>

// Sections and regions
#pragma section( tasksCode, 0 )
#pragma section( tasksData, 0 )
#pragma region( tasksCodeRegion, 0x8000, 0xbfff, , TASKS_BANK, { tasksCode, tasksData } )

// index table - holds indexes of 'real' data tables, used to simplify sorting tasks when elements are added or removed
char taskRef[TASK_ARRAY_SIZE];
void (*task_codeRef[TASK_ARRAY_SIZE])(byte);
// Short name displayed on screen
char task_nameIdx[TASK_ARRAY_SIZE];
char task_desc[TASK_ARRAY_SIZE][19];
char task_params[TASK_ARRAY_SIZE][TASK_PARAMS_SIZE];
char task_minReqStat[TASK_ARRAY_SIZE][3];
char task_minReqSkill[TASK_ARRAY_SIZE][4];
char task_reqType[TASK_ARRAY_SIZE];
char task_worker[TASK_ARRAY_SIZE];
char * task_icon[TASK_ARRAY_SIZE];

// helper variable, stores last free task entry to speed things up
static char _nextFreeTaskRef = 0;
// helper, everytime we unassignTask but do not reset icon it gets increased
static char _pendingIconResets = 0;

//-----------------------------------------------------------------------------------------
// In Init bank
#pragma code ( gameInitRAMCode )
#pragma data ( gameInitData )
//-----------------------------------------------------------------------------------------
void initTaskList() {
    _nextFreeTaskRef = 0;
    for(char i=0;i<TASK_ARRAY_SIZE;i++){
        taskRef[i] = i;
        task_codeRef[i]   = nullptr;
        task_nameIdx[i]   = TXT_IDX_TASK_EMPTY_NAME;
        task_params[i][0] = 0;
        task_params[i][1] = 0;
        task_params[i][2] = 0;
        task_params[i][3] = 0;
        task_params[i][4] = 0;
        task_reqType[i]   = NO_TASK;
        task_worker[i]    = NO_SLOT;
        strcpy(task_desc[i], TXT[TXT_IDX_TASK_EMPTY_DESCRIPTION]);
        task_icon[i]      = SPR_TASK_MIA;
        task_status[i]    = TASK_STATUS_NOTASK;
    }
}

//-----------------------------------------------------------------------------------------
// In Tasks bank
#pragma code ( tasksCode )
#pragma data ( data )
//-----------------------------------------------------------------------------------------
Task newTask = {0};
#pragma code ( code )

// definitons in logger.h
void setTaskLogMsg(char taskId){
    LOG_MSG.LOG_DATA_TASK_NAMEIDX = task_nameIdx[taskId];
    LOG_MSG.LOG_DATA_TASK_WORKER  = task_worker[taskId];
    LOG_MSG.LOG_DATA_TASK_STATUS  = task_status[taskId];
    LOG_MSG.LOG_DATA_TASK_ID      = taskId;
    LOG_MSG.LOG_DATA_TASK_PARAMS1 = task_params[taskId][0];
    LOG_MSG.LOG_DATA_TASK_PARAMS2 = task_params[taskId][1];
    LOG_MSG.LOG_DATA_TASK_PARAMS3 = task_params[taskId][2];
    LOG_MSG.LOG_DATA_TASK_PARAMS4 = task_params[taskId][3];
    LOG_MSG.LOG_DATA_TASK_PARAMS5 = task_params[taskId][4];
}
#pragma code ( tasksCode )

static bool _addTask(){
    // find entry in the task arrays
    if(_nextFreeTaskRef == TASK_ARRAY_SIZE){
        memcpy(LOG_DATA, p"Tasks full", 10);
        logger(LOG_INFO | LOG_MSG_TEXT);

        updateStatusBarError(TXT[SB_IDX_TASKS_FULL]);
        return false;
    }

    // find entry in the taskRefArray and set it
    char nextFreeTask = taskRef[_nextFreeTaskRef];
    _nextFreeTaskRef++;

    // set new task details
    task_codeRef[nextFreeTask]   = newTask.codeRef;
    task_nameIdx[nextFreeTask]   = newTask.nameIdx;
    task_params[nextFreeTask][0] = newTask.params[0];
    task_params[nextFreeTask][1] = newTask.params[1];
    task_params[nextFreeTask][2] = newTask.params[2];
    task_params[nextFreeTask][3] = newTask.params[3];
    task_params[nextFreeTask][4] = newTask.params[4];
    task_reqType[nextFreeTask]   = newTask.reqType;
    strcpy(task_desc[nextFreeTask], newTask.desc);
    task_icon[nextFreeTask]      = newTask.icon;
    task_status[nextFreeTask]    = newTask.status;
    // assigned later during ticks
    task_worker[nextFreeTask]    = NO_SLOT;

    LOG_MSG.LOG_DATA_CONTEXT = LOG_DATA_CONTEXT_TASK_NEW_TASK;
    setTaskLogMsg(nextFreeTask);
    logger(LOG_INFO | LOG_MSG_TASK);

    updateStatusBar(TXT[SB_IDX_TASK_ADDED]);

    return true;
}

static void _removeTaskByRef(char taskRefId){
    // remove task from taskRef table
    char taskId = taskRef[taskRefId];

    // don't try to delete empty tasks
    if(task_reqType[taskId] == NO_TASK){
        setErrorCursor();
        return;
    }

    LOG_MSG.LOG_DATA_CONTEXT = LOG_DATA_CONTEXT_TASK_REMOVE_TASK;
    setTaskLogMsg(taskId);
    logger(LOG_INFO | LOG_MSG_TASK);

    // remove worker, leave the icon, it will be reset on next tick
    finishTask(taskId);

    // wipe the task in task_ array (just the task_reqType + text fields is enough)
    task_reqType[taskId] = NO_TASK;
    task_nameIdx[taskId] = TXT_IDX_TASK_EMPTY_NAME;
    strcpy(task_desc[taskId], TXT[TXT_IDX_TASK_EMPTY_DESCRIPTION]);

    // check if the task is done, if so - no need to call task handler.
    if(task_status[taskId] != TASK_STATUS_DONE){
        // not done - it's being cancelled somehow - set removal status
        task_status[taskId] = TASK_STATUS_REMOVE;
        // call the codeRef, it might need to cleanup
        (*task_codeRef[taskId])(taskId);
    }

    // char str[12*3+1];
    // sprintf(str, "%03d %03d ", taskRefId, taskId);
    // cwin_putat_string_raw(&cw, 0, 1, str, VCOL_DARK_GREY);

    // seal the gap in taskRef[]
    if(taskRefId < TASK_ARRAY_SIZE-1){
        do {
            taskRefId++;
            taskRef[taskRefId-1] = taskRef[taskRefId];
        } while (task_reqType[taskRef[taskRefId]] != NO_TASK && taskRefId < TASK_ARRAY_SIZE-1);
        // sprintf(str, "%03d %03d %03d", taskRefId, taskId, task_reqType[taskRef[taskRefId]]);
        // cwin_putat_string_raw(&cw, 0, 2, str, VCOL_GREEN);
        if(task_reqType[taskRef[taskRefId]] == NO_TASK){
            taskRefId--;
        }
    }
    _nextFreeTaskRef = taskRefId;
    taskRef[_nextFreeTaskRef] = taskId;
    // sprintf(str, "%d", _nextFreeTaskRef);
    // cwin_putat_string_raw(&cw, 0, 3, str, VCOL_DARK_GREY);

    // update current menu
    updateMenu();
}

// Removes taskRef with given ID, shifts remaining ones up. Wipes the target task_reqType, name and description.
static void _removeTask(char taskId){
    // find taskRefId
    char taskRefId = 0;
    for(char i=0;i<TASK_ARRAY_SIZE;i++){
        if(taskRef[i] == taskId){
            taskRefId = i;
            break;
        }
    }
    _removeTaskByRef(taskRefId);
}

// Returns best charSlot for a given requirement (must not be busy).
// Returns NO_CHARACTER if none found.
// static char _findWorkerForTask(char reqType) {
//     char bestCharSlot = NO_CHARACTER;
//     char bestSkill = 0;
//     for(char charSlot=0;charSlot<CHARACTER_SLOTS;charSlot++){
//         // only check active chars
//         if(characterSlots[charSlot] != NO_CHARACTER){
//             char charIdx = characterSlots[charSlot];
//             // check if busy
//             if(!allChars_busy[charIdx]){
//                 // check reqSkill value
//                 // if higher than previous - swap with previous
//                 if(allChars_skills[charIdx][reqType] > bestSkill){
//                     bestSkill = allChars_skills[charIdx][reqType];
//                     bestCharSlot = charSlot;
//                 }
//             }
//         }
//     }
//     return bestCharSlot;
// }

// Finds who was working on it, resets his icon, resets task_worker.
// Unassign means task remains on queue to be picked up again.
// As removal/unassign might happen at the end of the tick within the same hour it was picked up, the reset icon might not be desirable.
// - We do not want to reset icon if task was done within an hour (as it would not be visible at all).
// - We do want to reset icon if task was aborted (to indicate that it is not being worked on).
//-----------------------------------------------------------------------------------------
static void _unassignTask(char taskId, bool resetIcon){
    char charSlot = task_worker[taskId];
    if(charSlot != NO_SLOT){
        char charIdx = characterSlots[charSlot];
        if(resetIcon){
            setCharacterSlotIcon(charIdx, SPR_TASK_MIA);
        }
        allCharacters[charIdx].busy = false;
        task_worker[taskId] = NO_SLOT;
    }
}

// Unasign all tasks, leave icons - next process will reset them
static void _unassignAllTasks(){
    char i = 0;
    char taskId = 0;
    do {
        taskId = taskRef[i];
        if (task_worker[taskId] != NO_SLOT) {
            _unassignTask(taskId, false);
        }
        // next task
        i++;
    } while (i < TASK_ARRAY_SIZE && task_reqType[taskId] != NO_TASK);
}

//-----------------------------------------------------------------------------------------
// In Ticks bank
//-----------------------------------------------------------------------------------------
#pragma code ( ticksCode )
#pragma data ( data )
//-----------------------------------------------------------------------------------------

// Returns first matching charSlot for a required skill priority value (must not be busy).
// Returns NO_CHARACTER if none found.
static char _findFreeWorkerWithPrioXForSkillY(char reqPrio, char reqSkill) {
    for(char charSlot=0;charSlot<CHARACTER_SLOTS;charSlot++){
        // only check active chars
        char charIdx = characterSlots[charSlot];
        if(charIdx != NO_CHARACTER){

            // check if busy TODO: change it so we got task reference, not 'busy' bool
            if(!allCharacters[charIdx].busy){
                // check reqSkill prio value
                // if matching the request - return this charSLot
                if(allCharacters[charIdx].prio[reqSkill] == reqPrio){
                    return charIdx;
                }
            }
        }
    }
    return NO_CHARACTER;
}

static char _findUnassignedTaskForSkill(char skillIt) {
    for(char i=0;i<TASK_ARRAY_SIZE;i++){
        char taskId = taskRef[i];
        // NO_TASK found - no more tasks to process
        if(task_reqType[taskId] == NO_TASK){
            return NO_TASK;
        }
        // there is something we can do
        if(task_worker[taskId] == NO_SLOT && task_reqType[taskId] == skillIt){
            return taskId;
        }
    }
    // nothing found in the whole array
    return NO_TASK;
}

static void _assignTaskToWorker(char taskId, char charIdx) {
    task_worker[taskId] = allCharacters[charIdx].slot;
    allCharacters[charIdx].busy = true;

    // char str[5];
    // sprintf(str, "%3u  %3u", taskId, charSlot);
    // cwin_putat_string_raw(&cw, 25, 0, str, VCOL_WHITE);

    setCharacterSlotIcon(charIdx, task_icon[taskId]);
    // updateStatusBar(s"  Task assigned  ");

    LOG_MSG.LOG_DATA_CONTEXT = LOG_DATA_CONTEXT_TASK_ASSIGNED_TO_WORKER;
    setTaskLogMsg(taskId);
    logger(LOG_DEBUG | LOG_MSG_TASK);
}

// Minimum energy needed by character to undertake any tasks
#define MIN_ENERGY_TO_CONTINUE 100


// Reset character icons if they are not busy
void _resetCharacterIcons() {
    for (char it = 0; it < CHARACTER_SLOTS; it++) {
        char charIdx = characterSlots[it];
        if (charIdx != NO_CHARACTER && !allCharacters[charIdx].busy) {
            // reset icon
            setCharacterSlotIcon(charIdx, SPR_TASK_MIA);
            if (_pendingIconResets) {
                _pendingIconResets--;
            }
        }
    }
}

// Count the number of available free workers
char _countFreeWorkers() {
    char freeWorkersCount = 0;
    for (char it = 0; it < CHARACTER_SLOTS; it++) {
        char charIdx = characterSlots[it];
        if (charIdx != NO_CHARACTER && !allCharacters[charIdx].busy && allCharacters[charIdx].energy >= MIN_ENERGY_TO_CONTINUE) {
            freeWorkersCount++;
        }
    }
    return freeWorkersCount;
}

// Assign tasks to available free workers
void _assignTasksToWorkers(char freeWorkersCount) {
    if (freeWorkersCount > 0 && _nextFreeTaskRef) {
        char prioIt = 1;
        do {
            for (char skillIt = 0; skillIt < SKILL_COUNT; skillIt++) {
                // find worker with priority X for skill Y (starting at 1)
                char charIdx = _findFreeWorkerWithPrioXForSkillY(prioIt, skillIt);

                if (charIdx != NO_CHARACTER) {
                    // see if there is any task for this max priority skill
                    char taskId = _findUnassignedTaskForSkill(skillIt);

                    // assign it if there is
                    if (taskId != NO_TASK) {
                        _assignTaskToWorker(taskId, charIdx);
                        freeWorkersCount--;
                        if (freeWorkersCount == 0) {
                            break;
                        }
                    }
                }
                // get these tasks out to everybody

                // nope? ok, next X (find next highest priority)
            }
            // no more skills, but we still have free workers? Increase priority and repeat
            prioIt++;
        } while (prioIt <= MAX_PRIO && freeWorkersCount > 0);
    }
}

// Process tasks that are in progress
void _processTasksInProgress() {
    // iterate through real tasks, not the ref array as it moves around on task completion
    char taskId = 0;
    do {
        if (task_worker[taskId] != NO_SLOT) {
            // got a worker? tick that task
            (*task_codeRef[taskId])(taskId);
        }
        // next task
        taskId++;
    } while (taskId < TASK_ARRAY_SIZE);
}

// Called by callendar.c
void tasksTick() {
    // No point going through the loop if there are no tasks
    if (_nextFreeTaskRef == 0 && !_pendingIconResets) {
        return;
    }

    _resetCharacterIcons();
    char freeWorkersCount = _countFreeWorkers();
    _assignTasksToWorkers(freeWorkersCount);

    _processTasksInProgress();
}

#pragma code ( code )
#pragma data ( data )
//-----------------------------------------------------------------------------------------
// Wrappers in RAM

// Remove task means its gone (done most likely), won't be picked up again
void removeTask(char taskId){
    char pbank = setBank(TASKS_BANK);
    _removeTask(taskId);
    setBank(pbank);
}

void removeTaskByRef(char taskRefId){
    char pbank = setBank(TASKS_BANK);
    _removeTaskByRef(taskRefId);
    setBank(pbank);
}

bool addTask(){
    char pbank = setBank(TASKS_BANK);
    bool result = _addTask();
    setBank(pbank);
    return result;
}

void finishTask(char taskId){
    _pendingIconResets++;
    char pbank = setBank(TASKS_BANK);
    _unassignTask(taskId, false);
    setBank(pbank);
}
void unassignTask(char taskId){
    char pbank = setBank(TASKS_BANK);
    _unassignTask(taskId, true);
    setBank(pbank);
}
// Used at end of day
void unassignAllTasks(){
    char pbank = setBank(TASKS_BANK);
    _unassignAllTasks();
    setBank(pbank);
}
