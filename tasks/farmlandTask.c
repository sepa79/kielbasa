#include <c64/types.h>

#include <tick/farmlandTick.h>
#include <tasks/taskManager.h>
#include <assets/assetsSettings.h>
#include <character/character.h>

// 9 as per skill range, + 3 on each 'side' of the scale for possible bonuses/penalties
const byte fieldPlantedTable[9+5] = {
    2, 5, 8,
    10, 12, 13, 15, 17, 20, 25, 34, 50,
    66, 85
};

// for now anybody can do it.
// later: energy used should depend on skills and stats, and one can't finish it if he runs out of energy
void sowFieldTask(byte taskId){
    // get the fieldId - this is the definition from menu/farmland.c
    // task.params[0] = _currentPlant;
    // task.params[1] = _currentField;
    // task.params[2] = 0;
    // task.params[3] = 0;
    // task.params[4] = 0;
    byte plantId = task_params[taskId][0];
    byte fieldId = task_params[taskId][1];

    if(task_status[taskId] == TASK_STATUS_NEW){
        // how much can be sown depends on:
        // - how many plant seeds are left (end if 0)
        // - field size
        // - skill of the farmer
        // Formula:
        // field_stage_planted += how much was planted (skill based)
        // task_done += how much % was covered (skill based) 
        //              (when reach 100% task is done, unless there will be a shortage of seeds - in that case we end early)
        // seeds left -= seeds used (based on skill + Int)

        field_plantId[fieldId] = plantId;
        field_stage[fieldId]   = PLANT_STAGE_SPROUT;
        field_timer[fieldId]   = plant_stage1timer[plantId];

        // get worker, get his skills and the value he can 'do' in a turn from the table
        byte worker   = task_worker[taskId];
        byte skill    = allChars_skills[ worker ][ task_reqType[taskId] ];
        byte modifier = allChars_stats[ worker ][ STAT_STR ];
        byte partDone = fieldPlantedTable[skill + modifier - 0];

        field_stage_planted[fieldId] += partDone;
        field_stage_grown[fieldId]   = 0;
        field_stage_ready[fieldId]   = 0; // reap takes this / SOME_DIVIDER

        if(field_stage_planted[fieldId] >= 100*field_area[fieldId]) {
            // task done, set status & remove
            task_status[taskId] = TASK_STATUS_DONE;
            removeTask(taskId);
        }
    } else if(task_status[taskId] == TASK_STATUS_REMOVE){
        // clean up, don't leave field in dangling 'sowing' state
        field_stage[fieldId]   = PLANT_STAGE_NONE;
    } else {
        byte str[50];
        sprintf(str, s"  ""[%3u]"s"sowFieldTask - unknown status code   ", task_status[taskId]);
        updateStatusBar(str);
        setErrorCursor();
    }
}