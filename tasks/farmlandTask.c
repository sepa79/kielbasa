#include <c64/types.h>

#include <engine/logger.h>

#include <tick/farmlandTick.h>
#include <tasks/taskManager.h>
#include <assets/assetsSettings.h>
#include <character/character.h>

// modifier values, make sure stat is -1'ed so that '3' means middle value is chosen
// substract 10 from it.
const byte priModifierTable[5] = {0, 5, 10, 15, 20};
const byte secModifierTable[5] = {2, 6, 10, 14, 18};
const byte terModifierTable[5] = {4, 8, 10, 12, 16};

void sowFieldTask(byte taskId){
    LOG_MSG.LOG_DATA_CONTEXT = LOG_DATA_CONTEXT_TASK_FARM_SOW_ENTRY;
    setTaskLogMsg(taskId);
    logger(LOG_DEBUG | LOG_MSG_TASK);

    // get the fieldId - this is the definition from menu/farmland.c
    // task.params[0] = _currentPlant;
    // task.params[1] = _currentField;
    // task.params[2] = 0;
    // task.params[3] = 0;
    // task.params[4] = 0;
    byte plantId = task_params[taskId][0];
    byte fieldId = task_params[taskId][1];

    byte energyNeeded = 0;
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
        byte charSlot      = task_worker[taskId];
        struct CharacterStruct * worker = characterSlots[charSlot];
        byte skill       = worker->skill[ task_reqType[taskId] ];
        byte priModifier = worker->stat[ STAT_STR ] -1;
        byte secModifier = worker->stat[ STAT_INT ] -1;
        byte terModifier = worker->stat[ STAT_CUN ] -1;
        partDone = skill * 10 + priModifierTable[priModifier]-10 + secModifierTable[secModifier]-10 + terModifierTable[terModifier]-10;
        if(partDone <= 0){
            partDone = 1;
        }
        // make sure we don't plant more than field 'capacity'
        if(fields[fieldId].planted+partDone > FIELD_CAPACITY*fields[fieldId].area){
            partDone = FIELD_CAPACITY*fields[fieldId].area - fields[fieldId].planted;
        }
        
        // check if we got enough seeds
        if(flt_storage[plantId] > 0 && partDone > flt_storage[plantId]){
            // reduce 'partDone' to match available seeds
            partDone = flt_storage[plantId];
        }
        if(flt_storage[plantId] >= partDone) {

            // check if we got enough energy
            energyNeeded = partDone > 10 ? 10 : partDone;
            if(checkEnergyLevel(worker, energyNeeded)){
                flt_storage[plantId] -= partDone;

                // decrease energy
                decEnergyLevel(worker, energyNeeded);
                // process task
                fields[fieldId].planted += partDone;
                fields[fieldId].grown   = 0;
                fields[fieldId].ready   = 0; // reap takes this / SOME_DIVIDER

                // is the whole field done now?
                if(fields[fieldId].planted >= FIELD_CAPACITY*fields[fieldId].area) {
                    // task done, set status & remove
                    task_status[taskId] = TASK_STATUS_DONE;

                    LOG_MSG.LOG_DATA_CONTEXT = LOG_DATA_CONTEXT_TASK_FARM_SOW_DONE;
                    setTaskLogMsg(taskId);
                    logger(LOG_INFO | LOG_MSG_TASK);

                    removeTask(taskId);
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

        byte str[50];
        sprintf(str, s"  ""[%3u]"s"sowFieldTask - unknown status code   ", task_status[taskId]);
        updateStatusBar(str);
        setErrorCursor();
    }

    LOG_MSG.LOG_DATA_CONTEXT = LOG_DATA_CONTEXT_TASK_FARM_SOW_EXIT;
    setTaskLogMsg(taskId);
    LOG_MSG.LOG_DATA_TASK_PARAMS3 = energyNeeded;
    LOG_MSG.LOG_DATA_TASK_PARAMS4 = partDone;
    logger(LOG_DEBUG | LOG_MSG_TASK);

    updateMenuIfIn(MENU_BANK_FARMLAND);
}

void reapFieldTask(byte taskId){
    // get the fieldId - this is the definition from menu/farmland.c
    // task.params[0] = _currentField;
    // task.params[1] = 0;
    // task.params[2] = 0;
    // task.params[3] = 0;
    // task.params[4] = 0;
    byte fieldId = task_params[taskId][0];
    byte plantId = fields[fieldId].plantId;

    if(task_status[taskId] == TASK_STATUS_NEW){
        // reap is simple, same logic as with planting really

        // get worker, get his skills and the value he can 'do' in a turn from the table
        byte charSlot      = task_worker[taskId];
        struct CharacterStruct * worker = characterSlots[charSlot];
        byte skill       = worker->skill[ task_reqType[taskId] ];
        byte priModifier = worker->stat[ STAT_STR ] -1;
        byte secModifier = worker->stat[ STAT_INT ] -1;
        byte terModifier = worker->stat[ STAT_CUN ] -1;
        signed int partDone = skill * 10 + priModifierTable[priModifier]-10 + secModifierTable[secModifier]-10 + terModifierTable[terModifier]-10;
        if(partDone <= 0){
            partDone = 1;
        }
        // make sure we don't reap more than field 'capacity'
        if(fields[fieldId].grown < partDone){
            partDone = fields[fieldId].grown;
        }
        
        // check if we got enough energy
        byte energyNeeded = partDone > 10 ? 10 : partDone;
        if(checkEnergyLevel(worker, energyNeeded)){
            flt_storage[plantId] += partDone;

            // decrease energy
            decEnergyLevel(worker, energyNeeded);
            // process task
            fields[fieldId].grown -= partDone;

            // is the whole field done now?
            if(fields[fieldId].grown == 0) {
                // task done, set status & remove
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

        byte str[50];
        sprintf(str, s"  ""[%3u]"s"reapFieldTask - unknown status code   ", task_status[taskId]);
        updateStatusBar(str);
        setErrorCursor();
    }
    updateMenuIfIn(MENU_BANK_FARMLAND);
}