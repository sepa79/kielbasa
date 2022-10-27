#include <c64/types.h>
#include <c64/vic.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <engine/easyFlashBanks.h>
#include <tasks/taskManager.h>
#include <translation/common.h>
#include <character/character.h>

// Sections and regions
#pragma section( tasksCode, 0 )
#pragma section( tasksData, 0 )
#pragma region( tasksCodeRegion, 0x8000, 0xbfff, , TASKS_BANK, { tasksCode, tasksData } )

// index table - holds indexes of 'real' data tables, used to simplify sorting tasks when elements are added or removed
byte taskRef[TASK_ARRAY_SIZE];
void (*task_codeRef[TASK_ARRAY_SIZE])(byte);
// Short name displayed on screen
byte task_nameIdx[TASK_ARRAY_SIZE];
byte task_desc[TASK_ARRAY_SIZE][19];
byte task_params[TASK_ARRAY_SIZE][TASK_PARAMS_SIZE];
byte task_minReqStat[TASK_ARRAY_SIZE][3];
byte task_minReqSkill[TASK_ARRAY_SIZE][4];
byte task_reqType[TASK_ARRAY_SIZE];
byte task_worker[TASK_ARRAY_SIZE];
byte task_done[TASK_ARRAY_SIZE];
const char * task_icon[TASK_ARRAY_SIZE];

// helper variable, stores last free task entry to speed things up
static byte _nextFreeTaskRef = 0;

//-----------------------------------------------------------------------------------------
// In Init bank
//-----------------------------------------------------------------------------------------
void initTaskList() {
    _nextFreeTaskRef = 0;
    for(byte i=0;i<TASK_ARRAY_SIZE;i++){
        taskRef[i] = i;
        task_codeRef[i]   = nullptr;
        task_nameIdx[i]   = TXT_IDX_TASK_EMPTY_NAME;
        task_params[i][0] = 0;
        task_params[i][1] = 0;
        task_params[i][2] = 0;
        task_params[i][3] = 0;
        task_params[i][4] = 0;
        task_reqType[i] = NO_TASK;
        task_worker[i] = NO_CHARACTER;
        task_done[i] = 0;
        strcpy(task_desc[i], TXT[TXT_IDX_TASK_EMPTY_DESCRIPTION]);
        task_icon[i] = SPR_TASK_MIA;
        task_status[i] = TASK_STATUS_NOTASK;
    }
}

//-----------------------------------------------------------------------------------------
// In Tasks bank
//-----------------------------------------------------------------------------------------
#pragma code ( tasksCode )
#pragma data ( data )
static bool _addTask(struct Task * task){
    // find entry in the task arrays
    if(_nextFreeTaskRef == TASK_ARRAY_SIZE){
        // TODO: Add text
        updateStatusBar(s"  No more room in Task Queue  ");
        setErrorCursor();
        return false;
    }

    // find entry in the taskRefArray and set it
    byte nextFreeTask = taskRef[_nextFreeTaskRef];
    _nextFreeTaskRef++;

    // set new task details
    task_codeRef[nextFreeTask]   = task->codeRef;
    task_nameIdx[nextFreeTask]   = task->nameIdx;
    task_params[nextFreeTask][0] = task->params[0];
    task_params[nextFreeTask][1] = task->params[1];
    task_params[nextFreeTask][2] = task->params[2];
    task_params[nextFreeTask][3] = task->params[3];
    task_params[nextFreeTask][4] = task->params[4];
    task_reqType[nextFreeTask] = task->reqType;
    strcpy(task_desc[nextFreeTask], task->desc);
    task_icon[nextFreeTask] = task->icon;
    task_status[nextFreeTask] = task->status;
    // assigned later during ticks
    task_worker[nextFreeTask] = NO_CHARACTER;
    task_done[nextFreeTask] = 0;
    return true;
}

static void _removeTaskByRef(byte taskRefId){
    // remove task from taskRef table
    byte taskId = taskRef[taskRefId];

    // don't try to delete empty tasks
    if(task_reqType[taskId] == NO_TASK){
        setErrorCursor();
        return;
    }

    // remove worker
    unassignTask(taskId);

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

    // byte str[5];
    // sprintf(str, "%3u", taskRefId);
    // cwin_putat_string_raw(&cw, 0, 0, str, VCOL_GREEN);

    // seal the gap in taskRef[]
    byte currentTask = 0;
    if(taskRefId < TASK_ARRAY_SIZE){
        do {
            taskRef[taskRefId] = taskRef[taskRefId+1];
            currentTask = taskRef[taskRefId];
            taskRefId++;
        } while (taskRefId < TASK_ARRAY_SIZE && task_reqType[currentTask] != NO_TASK);
        taskRefId--;
    }
    _nextFreeTaskRef = taskRefId;
    taskRef[_nextFreeTaskRef] = taskId;

    // update current menu
    updateMenu();
}

// Removes taskRef with given ID, shifts remaining ones up. Wipes the target task_reqType, name and description.
static void _removeTask(byte taskId){
    // find taskRefId
    byte taskRefId = 0;
    for(byte i=0;i<TASK_ARRAY_SIZE;i++){
        if(taskRef[i] == taskId){
            taskId = i;
            break;
        }
    }
    _removeTaskByRef(taskRefId);
}

// Returns best charSlot for a given requirement (must not be busy).
// Returns NO_CHARACTER if none found.
// static byte _findWorkerForTask(byte reqType) {
//     byte bestCharSlot = NO_CHARACTER;
//     byte bestSkill = 0;
//     for(byte charSlot=0;charSlot<CHARACTER_SLOTS;charSlot++){
//         // only check active chars
//         if(characterSlots[charSlot] != NO_CHARACTER){
//             byte charIdx = characterSlots[charSlot];
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

//-----------------------------------------------------------------------------------------
// In Ticks bank
//-----------------------------------------------------------------------------------------
#pragma code ( ticksCode )
#pragma data ( data )
//-----------------------------------------------------------------------------------------

// Returns first matching charSlot for a required skill priority value (must not be busy).
// Returns NO_CHARACTER if none found.
static byte _findFreeWorkerWithPrioXForSkillY(byte reqPrio, byte reqSkill) {
    for(byte charSlot=0;charSlot<CHARACTER_SLOTS;charSlot++){
        // only check active chars
        if(characterSlots[charSlot] != NO_CHARACTER){
            byte charIdx = characterSlots[charSlot];
            // check if busy
            if(!allChars_busy[charIdx]){
                // check reqSkill prio value
                // if matching the request - return this charSLot
                if(allChars_prios[charIdx][reqSkill] == reqPrio){
                    return charSlot;
                }
            }
        }
    }
    return NO_CHARACTER;
}

static byte _findUnassignedTaskForSkill(byte skillIt) {
    for(byte i=0;i<TASK_ARRAY_SIZE;i++){
        byte taskId = taskRef[i];
        // NO_TASK found - no more tasks to process
        if(task_reqType[taskId] == NO_TASK){
            return NO_TASK;
        }
        // there is something we can do
        if(task_reqType[taskId] == skillIt){
            return taskId;
        }
    }
    // nothing found in the whole array
    return NO_TASK;
}

static void _assignTaskToWorker(byte taskId, byte charSlot) {
    byte charIdx = characterSlots[charSlot];
    task_worker[taskId] = charIdx;
    allChars_busy[charIdx] = true;

    // byte str[5];
    // sprintf(str, "%3u  %3u", taskId, charSlot);
    // cwin_putat_string_raw(&cw, 25, 0, str, VCOL_WHITE);

    setCharacterSlotIcon(charSlot, task_icon[taskId]);
    updateStatusBar(s"  Task assigned  ");
}



// Called by callendar.c
void tasksTick(){
    // no point going through the loop if there are no tasks
    if(_nextFreeTaskRef == 0) {
        return;
    }

    // assign tasks again, in case new tasks came or new workers are available
    // any free workers?
    byte freeWorkersCount = 0;
    for(byte it = 0; it < CHARACTER_SLOTS; it++){
        if(characterSlots[it] != NO_CHARACTER){
            byte charIdx = characterSlots[it];
            if(!allChars_busy[charIdx]){
                freeWorkersCount++;
            }
        }
    }
    // debug
    // byte str[4];
    // sprintf(str, "FWC: %3u", freeWorkersCount);
    // cwin_putat_string_raw(&cw, 0, 10, str, VCOL_GREEN);

    // find a task for free workers
    if(freeWorkersCount > 0){
        // iterate through prios starting at 1
        byte prioIt = 1;
        do {
            // foreach skill Y
            for(byte skillIt = 0; skillIt < SKILL_COUNT; skillIt++){
                // find worker with prio X for skill Y (starting at 1)
                byte charSlot = _findFreeWorkerWithPrioXForSkillY(prioIt, skillIt);
                    // byte str[4];
                    // sprintf(str, "%3u", charSlot);
                    // cwin_putat_string_raw(&cw, prioIt*6, skillIt, str, VCOL_GREEN);
                if(charSlot != NO_CHARACTER){
                    // see if there is any task for this max prio skill
                    byte taskId = _findUnassignedTaskForSkill(skillIt);

                    // byte str[5];
                    // sprintf(str, "%3u", taskId);
                    // cwin_putat_string_raw(&cw, prioIt*6, skillIt, str, VCOL_GREEN);

                    // asign it if there is
                    if(taskId != NO_TASK){
                        _assignTaskToWorker(taskId, charSlot);
                        freeWorkersCount--;
                        if(freeWorkersCount == 0){
                            break;
                        }
                    }
                }
                // nope? ok, next X (find next highest prio)
            }
            // no more skills, but we still got free workers? increase Prio and repeat
            prioIt++;
        } while (prioIt <= MAX_PRIO && freeWorkersCount > 0);
    }

    // process tasks in progress
    byte i = 0;
    byte taskId = 0;
    do {
        taskId = taskRef[i];
        if(task_worker[taskId] != NO_CHARACTER){
            updateStatusBar(s"  Exec  ");
            // got a worker? tick that task
            (*task_codeRef[taskId])(taskId);
        }
        // neeeeext
        i++;
    // NO_TASK found - no more tasks to process
    } while (i < TASK_ARRAY_SIZE && task_reqType[taskId] != NO_TASK);

}

//-----------------------------------------------------------------------------------------
#pragma code ( code )
#pragma data ( data )
//-----------------------------------------------------------------------------------------
// Wrappers in RAM
void removeTask(byte taskId){
    changeBank(TASKS_BANK);
    _removeTask(taskId);
    restoreBank();
}

void removeTaskByRef(byte taskRefId){
    changeBank(TASKS_BANK);
    _removeTaskByRef(taskRefId);
    restoreBank();
}

bool addTask(struct Task * task){
    changeBank(TASKS_BANK);
    bool result = _addTask(task);
    restoreBank();
    return result;
}

// finds who was working on it, resets his icon, resets task_worker
void unassignTask(byte taskId){
    byte charId = task_worker[taskId];
    if(charId != NO_CHARACTER){
        byte charSlot = allChars_slot[charId];
        setCharacterSlotIcon(charSlot, SPR_TASK_MIA);
        allChars_busy[charId] = false;
        task_worker[taskId] = NO_CHARACTER;
    }
}
