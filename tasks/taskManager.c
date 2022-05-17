#include <c64/types.h>
#include <c64/vic.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <tasks/taskManager.h>
#include <translation/common.h>
#include <character/character.h>

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
const char * task_icon[TASK_ARRAY_SIZE];

// helper variable, stores last free task entry to speed things up
static byte _nextFreeTaskRef = 0;

// column offset for printing data
#define COL_OFFSET_TASKLIST 12

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
        task_minReqStat[i][0] = 0;
        task_minReqStat[i][1] = 0;
        task_minReqStat[i][2] = 0;
        task_minReqSkill[i][0] = 0;
        task_minReqSkill[i][1] = 0;
        task_minReqSkill[i][2] = 0;
        task_minReqSkill[i][3] = 0;
        task_reqType[i] = NO_TASK;
        task_worker[i] = NO_CHARACTER;
        strcpy(task_desc[i], TXT[TXT_IDX_TASK_EMPTY_DESCRIPTION]);
    }
}

bool addTask(struct Task * task){
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
    task_minReqStat[nextFreeTask][STAT_INT] = task->minReqStat[STAT_INT];
    task_minReqStat[nextFreeTask][STAT_STR] = task->minReqStat[STAT_STR];
    task_minReqStat[nextFreeTask][STAT_CUN] = task->minReqStat[STAT_CUN];
    task_minReqSkill[nextFreeTask][SKILL_BREEDING] = task->minReqSkill[SKILL_BREEDING];
    task_minReqSkill[nextFreeTask][SKILL_FARMING]  = task->minReqSkill[SKILL_FARMING];
    task_minReqSkill[nextFreeTask][SKILL_BUTCHERY] = task->minReqSkill[SKILL_BUTCHERY];
    task_minReqSkill[nextFreeTask][SKILL_TRADE]    = task->minReqSkill[SKILL_TRADE];
    task_reqType[nextFreeTask] = task->reqType;
    strcpy(task_desc[nextFreeTask], task->desc);
    task_icon[nextFreeTask] = SPR_TASK_MIA;
    // assigned later during ticks
    task_worker[nextFreeTask] = NO_CHARACTER;

    return true;
}

// Removes taskRef with given ID, shifts remaining ones up. Wipes the target task_reqType.
void removeTask(byte taskRefId){
    // remove task from taskRef table
    byte nextFreeTask = taskRef[taskRefId];
    // wipe the task in task_ array (just the task_reqType + text fields is enough)
    task_reqType[nextFreeTask] = NO_TASK;
    task_nameIdx[nextFreeTask] = TXT_IDX_TASK_EMPTY_NAME;
    strcpy(task_desc[nextFreeTask], TXT[TXT_IDX_TASK_EMPTY_DESCRIPTION]);

    // seal the gap in taskRef[]
    if(taskRefId < TASK_ARRAY_SIZE-1 && taskRef[taskRefId+1] != NO_TASK){
        do {
            taskRef[taskRefId] = taskRef[++taskRefId];
        } while (taskRef[taskRefId] != NO_TASK && taskRefId < TASK_ARRAY_SIZE-1);
    }
    _nextFreeTaskRef = taskRefId;
    taskRef[_nextFreeTaskRef] = nextFreeTask;
}

void displayTaskList(){
    // header
    cwin_putat_string_raw(&cw, COL_OFFSET_TASKLIST, 1, TXT[TXT_IDX_TASK_LIST_HEADER], VCOL_YELLOW);

    // tasks list
    for(byte i=0;i<TASK_ARRAY_SIZE;i++){
        byte tRef = taskRef[i];
        cwin_putat_string_raw(&cw, COL_OFFSET_TASKLIST, 2+i, TXT[task_nameIdx[tRef]], VCOL_GREEN);
        cwin_putat_string_raw(&cw, COL_OFFSET_TASKLIST+8, 2+i, "\x7e", VCOL_YELLOW);
        cwin_putat_string_raw(&cw, COL_OFFSET_TASKLIST+9, 2+i, task_desc[tRef], VCOL_GREEN);
    }
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

// Returns first matching charSlot for a required skill priority value (must not be busy).
// Returns NO_CHARACTER if none found.
static byte _findFreeWorkerWithSkillXPrioY(byte reqSkill, byte reqPrio) {
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
        byte tRef = taskRef[i];
        // NO_TASK found - no more tasks to process
        if(task_reqType[tRef] == NO_TASK){
            return NO_TASK;
        }
        // there is something we can do
        if(task_reqType[tRef] == skillIt){
            return i;
        }
    }
    // nothing found in the whole array
    return NO_TASK;
}

static byte _assignTaskToWorker(byte taskId, byte charSlot) {
    byte charIdx = characterSlots[charSlot];
    task_worker[taskId] = charIdx;
    setCharacterSlotIcon(charSlot, task_icon[taskId]);
}

// Called by callendar.c
void tasksTick(){
    // process tasks in progress
    // TODO - redo it
    // byte i = 0;
    // do {
    //     if(task_reqType[i] != NO_TASK){
    //         // list should have no gaps in it
    //         return;
    //     }
    //     // task found, find a worker for it
    //     byte charSlot = _findWorkerForTask(task_reqType[i]);
    //     if(charSlot != NO_CHARACTER){
    //         // got a worker? tick that task
    //         (*task_codeRef[i])(charSlot);
    //     }
    //     // neeeeext
    //     i++;
    // } while (i < 10);

    // no point going through the loop if there are no tasks
    if(_nextFreeTaskRef == 0) {
        return;
    }

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
    // sprintf(str, "%3u", freeWorkersCount);
    // cwin_putat_string_raw(&cw, 0, 0, str, VCOL_GREEN);

    // find a task for free workers
    if(freeWorkersCount > 0){
        // iterate through prios starting at 1
        byte prioIt = 1;
        do {
            // foreach skill X
            for(byte skillIt = 0; skillIt < SKILL_COUNT; skillIt++){
                // find worker with skill X prio Y (starting at 1)
                byte charSlot = _findFreeWorkerWithSkillXPrioY(skillIt, prioIt);
                    byte str[4];
                    sprintf(str, "%3u", charSlot);
                    cwin_putat_string_raw(&cw, prioIt*6, skillIt, str, VCOL_GREEN);
                // if(charSlot != NO_CHARACTER){
                //     // see if there is any task for this max prio skill
                //     byte taskId = _findUnassignedTaskForSkill(skillIt);

                //     // byte str[4];
                //     // sprintf(str, "%3u", taskId);
                //     // cwin_putat_string_raw(&cw, 0, 1, str, VCOL_GREEN);

                //     // asign it if there is
                //     if(taskId != NO_TASK){
                //         _assignTaskToWorker(taskId, charSlot);
                //         freeWorkersCount--;
                //         if(freeWorkersCount == 0){
                //             break;
                //         }
                //     }
                // }
                // nope? ok, next X (find next highest prio)
            }
            // no more skills, but we still got free workers? increase Prio and repeat
            prioIt++;
        } while (prioIt <= MAX_PRIO && freeWorkersCount > 0);
    }
}