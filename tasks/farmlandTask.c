#include <engine/logger.h>

#include <tick/farmlandTick.h>
#include <tasks/taskManager.h>
#include <assets/assetsSettings.h>
#include <character/character.h>

//-----------------------------------------------------------------------------------------
// In Tasks bank
//-----------------------------------------------------------------------------------------
#pragma code ( tasksCode )
#pragma data ( data )

// modifier values, make sure stat is -1'ed so that '3' means middle value is chosen
// substract 10 from it.
static const signed char priModifierTable[5] = {-10, -5, 0, 5, 10};
static const signed char secModifierTable[5] = { -8, -4, 0, 4,  8};
static const signed char terModifierTable[5] = { -6, -2, 0, 2,  6};

void _sowFieldTask(char taskId){
    LOG_MSG.LOG_DATA_CONTEXT = LOG_DATA_CONTEXT_TASK_FARM_SOW_ENTRY;
    setTaskLogMsg(taskId);
    logger(LOG_DEBUG | LOG_MSG_TASK);

    // get the fieldId - this is the definition from menu/farmland.c
    // task.params[0] = _currentPlant;
    // task.params[1] = _currentField;
    // task.params[2] = 0;
    // task.params[3] = 0;
    // task.params[4] = 0;
    char plantId = task_params[taskId][0];
    char fieldId = task_params[taskId][1];

    unsigned int energyNeeded = 0;
    signed int partDone = 0;

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

        fields[fieldId].plantId = plantId;
        fields[fieldId].stage   = PLANT_STAGE_SPROUT;
        fields[fieldId].timer   = plants[plantId].stage1timer;

        // get worker, get his skills and the value he can 'do' in a turn from the table
        char charIdx     = task_worker[taskId];
        char skill       = allCharacters[charIdx].skill[ task_reqType[taskId] ];
        char priModifier = allCharacters[charIdx].stat[ STAT_STR ] -1;
        char secModifier = allCharacters[charIdx].stat[ STAT_INT ] -1;
        char terModifier = allCharacters[charIdx].stat[ STAT_AGI ] -1;
        partDone = skill + priModifierTable[priModifier] + secModifierTable[secModifier] + terModifierTable[terModifier];
        if(partDone <= 0){
            partDone = 1;
        }
        // make sure we don't plant more than field 'capacity'
        if(fields[fieldId].planted+partDone > FIELD_CAPACITY*fields[fieldId].area){
            partDone = FIELD_CAPACITY*fields[fieldId].area - fields[fieldId].planted;
        }
        
        // check if we got enough seeds
        if(GS.farm.storage[plantId] > 0 && partDone > GS.farm.storage[plantId]){
            // reduce 'partDone' to match available seeds
            partDone = GS.farm.storage[plantId];
        }
        if(GS.farm.storage[plantId] >= partDone) {

            // check if we got enough energy
            energyNeeded = partDone*ENERGY_COST_MULTIPLIER_FIELD_TASK;
            if(checkEnergyLevel(charIdx, energyNeeded)){
                // decrease available seeds
                GS.farm.storage[plantId] -= partDone;

                // decrease energy
                decEnergyLevel(charIdx, energyNeeded);
                // process task
                fields[fieldId].planted += partDone;
                fields[fieldId].alive   += partDone;
                fields[fieldId].grown   = 0;
                fields[fieldId].ready   = 0;

                // is the whole field done now?
                if(fields[fieldId].planted >= FIELD_CAPACITY*fields[fieldId].area) {
                    // task done, set status & remove
                    task_status[taskId] = TASK_STATUS_DONE;

                    // first log how much was processed
                    LOG_MSG.LOG_DATA_CONTEXT = LOG_DATA_CONTEXT_TASK_FARM_SOW_EXIT;
                    setTaskLogMsg(taskId);
                    LOG_MSG.LOG_DATA_TASK_PARAMS3 = energyNeeded;
                    LOG_MSG.LOG_DATA_TASK_PARAMS4 = partDone;
                    logger(LOG_DEBUG | LOG_MSG_TASK);
                    // now log that its finished
                    LOG_MSG.LOG_DATA_CONTEXT = LOG_DATA_CONTEXT_TASK_FARM_SOW_DONE;
                    setTaskLogMsg(taskId);
                    logger(LOG_INFO | LOG_MSG_TASK);

                    removeTask(taskId);
                } else {
                    // not done yet, so just leave trace in logs
                    LOG_MSG.LOG_DATA_CONTEXT = LOG_DATA_CONTEXT_TASK_FARM_SOW_EXIT;
                    setTaskLogMsg(taskId);
                    LOG_MSG.LOG_DATA_TASK_PARAMS3 = energyNeeded;
                    LOG_MSG.LOG_DATA_TASK_PARAMS4 = partDone;
                    logger(LOG_DEBUG | LOG_MSG_TASK);
                }
            } else {
                // not enough energy? set character to MIA by unassigning this task
                LOG_MSG.LOG_DATA_CONTEXT = LOG_DATA_CONTEXT_TASK_FARM_SOW_NOT_ENOUGH_ENERGY;
                setTaskLogMsg(taskId);
                logger(LOG_INFO | LOG_MSG_TASK);

                unassignTask(taskId);
            }
        } else {
            // task done - not enough seeds, set status & remove
            task_status[taskId] = TASK_STATUS_DONE;

            LOG_MSG.LOG_DATA_CONTEXT = LOG_DATA_CONTEXT_TASK_FARM_SOW_NOT_ENOUGH_SEEDS;
            setTaskLogMsg(taskId);
            logger(LOG_INFO | LOG_MSG_TASK);
            
            removeTask(taskId);
        }
    // handle task removal
    } else if(task_status[taskId] == TASK_STATUS_REMOVE){
        // clean up, don't leave field in dangling 'sowing' state
        fields[fieldId].stage = PLANT_STAGE_NONE;
    // hanlde errors - should never happen!
    } else {
        // Sum Ting Wong, We Tu Lo

        LOG_MSG.LOG_DATA_CONTEXT = LOG_DATA_CONTEXT_TASK_STATUS_UNKNOWN;
        setTaskLogMsg(taskId);
        logger(LOG_ERROR | LOG_MSG_TASK);

        char str[50];
        sprintf(str, s"  ""[%3u]"s"sowFieldTask - unknown status code   ", task_status[taskId]);
        updateStatusBar(str);
        setErrorCursor();
    }

    updateMenuIfIn(MENU_BANK_FARMLAND);
}

void _reapFieldTask(char taskId){
    // get the fieldId - this is the definition from menu/farmland.c
    // task.params[0] = _currentField;
    // task.params[1] = 0;
    // task.params[2] = 0;
    // task.params[3] = 0;
    // task.params[4] = 0;
    char fieldId = task_params[taskId][0];
    char plantId = fields[fieldId].plantId;

    if(task_status[taskId] == TASK_STATUS_NEW){
        // reap is simple, same logic as with planting really

        // get worker, get his skills and the value he can 'do' in a turn from the table
        char charIdx     = task_worker[taskId];
        char skill       = allCharacters[charIdx].skill[ task_reqType[taskId] ];
        char priModifier = allCharacters[charIdx].stat[ STAT_STR ] -1;
        char secModifier = allCharacters[charIdx].stat[ STAT_INT ] -1;
        char terModifier = allCharacters[charIdx].stat[ STAT_AGI ] -1;
        signed int partDone = skill + priModifierTable[priModifier] + secModifierTable[secModifier] + terModifierTable[terModifier];
        if(partDone <= 0){
            partDone = 1;
        }
        // make sure we don't reap more than field 'capacity'
        if(fields[fieldId].grown < partDone){
            partDone = fields[fieldId].grown;
        }
        
        // check if we got enough energy
        unsigned int energyNeeded = partDone*ENERGY_COST_MULTIPLIER_FIELD_TASK;
        if(checkEnergyLevel(charIdx, energyNeeded)){
            GS.farm.storage[plantId] += partDone;
            // decrease energy
            decEnergyLevel(charIdx, energyNeeded);
            // process task
            fields[fieldId].grown -= partDone;
            // increase Reaped count
            fields[fieldId].reaped = fields[fieldId].alive - ldiv16u(fields[fieldId].grown, fields[fieldId].gFactor);

            // is the whole field done now?
            if(fields[fieldId].grown == 0) {
                // task done, set status & remove
                fields[fieldId].alive = 0;
                task_status[taskId] = TASK_STATUS_DONE;
                fields[fieldId].stage = PLANT_STAGE_NONE;
                removeTask(taskId);
            }
        } else {
            // not enough energy? set character to MIA by unassigning this task
            unassignTask(taskId);
        }

    // handle task removal - currently unused
    } else if(task_status[taskId] == TASK_STATUS_REMOVE){
        // clean up, don't leave field in dangling 'reaping' state
        fields[fieldId].stage = PLANT_STAGE_READY;
    // hanlde errors - should never happen!
    } else {
        // Sum Ting Wong, We Tu Lo
        LOG_DATA[0] = LOG_DATA_CONTEXT_TASK_STATUS_UNKNOWN;
        setTaskLogMsg(taskId);
        logger(LOG_ERROR | LOG_MSG_TASK);

        char str[50];
        sprintf(str, s"  ""[%3u]"s"reapFieldTask - unknown status code   ", task_status[taskId]);
        updateStatusBar(str);
        setErrorCursor();
    }
    updateMenuIfIn(MENU_BANK_FARMLAND);
}

void _plowFieldTask(char taskId) {
    // Retrieve the fieldId from the task parameters
    char fieldId = task_params[taskId][0];

    if(task_status[taskId] == TASK_STATUS_NEW){
        // get worker, get his skills and the value he can 'do' in a turn from the table
        char charIdx     = task_worker[taskId];
        char skill       = allCharacters[charIdx].skill[ task_reqType[taskId] ];
        char priModifier = allCharacters[charIdx].stat[ STAT_STR ] -1;
        char secModifier = allCharacters[charIdx].stat[ STAT_INT ] -1;
        char terModifier = allCharacters[charIdx].stat[ STAT_AGI ] -1;
        signed int partDone = skill + priModifierTable[priModifier] + secModifierTable[secModifier] + terModifierTable[terModifier];
        if(partDone <= 0){
            partDone = 1;
        }
        // make sure we don't plow more than field 'capacity'
        unsigned int fSize = FIELD_CAPACITY*fields[fieldId].area;
        if(fields[fieldId].planted+partDone > fSize){
            partDone = fSize - fields[fieldId].planted;
        }
        
        // check if we got enough energy
        unsigned int energyNeeded = partDone*ENERGY_COST_MULTIPLIER_FIELD_TASK;
        if(checkEnergyLevel(charIdx, energyNeeded)){
            // decrease energy
            decEnergyLevel(charIdx, energyNeeded);
            // process task
            fields[fieldId].planted += partDone;

            // calculate % done
            fields[fieldId].ready = lmuldiv16u(100, fields[fieldId].planted, fSize);

            // is the whole field done now?
            if(fields[fieldId].planted >= fSize) {
                // task done, set status & remove
                fields[fieldId].planted = 0;
                task_status[taskId] = TASK_STATUS_DONE;
                fields[fieldId].stage = PLANT_STAGE_PLOWED;
                removeTask(taskId);
            }
        } else {
            // not enough energy? set character to MIA by unassigning this task
            unassignTask(taskId);
        }

    } else if (task_status[taskId] == TASK_STATUS_REMOVE) {
        // Task removal logic goes here

        // Clean up and set the field stage to none
        fields[fieldId].stage = PLANT_STAGE_NONE;
    } else {
        // Handle unknown status codes

        // Log the error and update the status bar
        LOG_DATA[0] = LOG_DATA_CONTEXT_TASK_STATUS_UNKNOWN;
        setTaskLogMsg(taskId);
        logger(LOG_ERROR | LOG_MSG_TASK);

        char str[50];
        sprintf(str, "  [%3u] _plowFieldsTask - unknown status code   ", task_status[taskId]);
        updateStatusBar(str);
        setErrorCursor();
    }

    updateMenuIfIn(MENU_BANK_FARMLAND);
}

#pragma code ( code )
#pragma data ( data )
//-----------------------------------------------------------------------------------------
// Wrappers in RAM

void sowFieldTask(char taskId){
    char pbank = setBank(TASKS_BANK);
    _sowFieldTask(taskId);
    setBank(pbank);
}

void reapFieldTask(char taskId){
    char pbank = setBank(TASKS_BANK);
    _reapFieldTask(taskId);
    setBank(pbank);
}

void plowFieldTask(char taskId) {
    char pbank = setBank(TASKS_BANK);
    _plowFieldTask(taskId);
    setBank(pbank);
}
