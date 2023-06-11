#include <engine/logger.h>

#include <tick/farmlandTick.h>
#include <tick/kitchenTick.h>
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
// static const char priModifierTable[5] = {0, 5, 10, 15, 20};
// static const char secModifierTable[5] = {2, 6, 10, 14, 18};
// static const char terModifierTable[5] = {4, 8, 10, 12, 16};

#define WHEAT_NEEDED_FOR_BREAD 1

// simplistic atm - takes jus an hour, no bonuses yet
static void _bakeBreadTask(char taskId){
    LOG_MSG.LOG_DATA_CONTEXT = LOG_DATA_CONTEXT_TASK_KITCHEN_BAKE_BREAD;
    setTaskLogMsg(taskId);
    logger(LOG_DEBUG | LOG_MSG_TASK);

    char energyNeeded = 5;

    if(task_status[taskId] == TASK_STATUS_NEW){

        // get worker, get his skills and the value he can 'do' in a turn from the table
        char charIdx     = task_worker[taskId];
        // char skill       = allCharacters[charIdx].skill[ task_reqType[taskId] ];
        // char priModifier = allCharacters[charIdx].stat[ STAT_STR ] -1;
        // char secModifier = allCharacters[charIdx].stat[ STAT_INT ] -1;
        // char terModifier = allCharacters[charIdx].stat[ STAT_CUN ] -1;
        // partDone = skill * 10 + priModifierTable[priModifier]-10 + secModifierTable[secModifier]-10 + terModifierTable[terModifier]-10;
        // if(partDone <= 0){
        //     partDone = 1;
        // }
        
        // check if we got enough wheat
        if(flt_storage[PLANT_WHEAT] > WHEAT_NEEDED_FOR_BREAD && GS.kitchen.storage[FOOD_HOME_BREAD] < GS.kitchen.maxStorage){
            // check if we got enough energy
            if(checkEnergyLevel(charIdx, energyNeeded)){

                // decrease energy
                decEnergyLevel(charIdx, energyNeeded);
                // process task
                flt_storage[PLANT_WHEAT] -= WHEAT_NEEDED_FOR_BREAD;
                GS.kitchen.storage[FOOD_HOME_BREAD]++;
                // LOG_MSG.LOG_DATA_CONTEXT = LOG_DATA_CONTEXT_TASK_FARM_SOW_DONE;
                // setTaskLogMsg(taskId);
                // logger(LOG_INFO | LOG_MSG_TASK);

                removeTask(taskId);
            } else {
                // not enough energy? set character to MIA by unassigning this task
                // LOG_MSG.LOG_DATA_CONTEXT = LOG_DATA_CONTEXT_TASK_FARM_SOW_NOT_ENOUGH_ENERGY;
                // setTaskLogMsg(taskId);
                // logger(LOG_INFO | LOG_MSG_TASK);

                unassignTask(taskId);
            }
        } else {
            // task done - not enough wheat, set status & remove
            task_status[taskId] = TASK_STATUS_DONE;
            if(GS.kitchen.storage[FOOD_HOME_BREAD] >= GS.kitchen.maxStorage){
                updateStatusBarError(TXT[SB_IDX_TASK_KITCHEN_BAKE_BREAD_STORAGE_FULL]);
            } else {
                updateStatusBarError(TXT[SB_IDX_TASK_KITCHEN_BAKE_BREAD_NO_WHEAT]);
            }
            // LOG_MSG.LOG_DATA_CONTEXT = LOG_DATA_CONTEXT_TASK_FARM_SOW_NOT_ENOUGH_SEEDS;
            // setTaskLogMsg(taskId);
            // logger(LOG_INFO | LOG_MSG_TASK);
            
            removeTask(taskId);
        }
    // handle task removal
    } else if(task_status[taskId] == TASK_STATUS_REMOVE){
        // nothing to do atm
    // hanlde errors - should never happen!
    } else {
        // Sum Ting Wong, We Tu Lo

        LOG_MSG.LOG_DATA_CONTEXT = LOG_DATA_CONTEXT_TASK_STATUS_UNKNOWN;
        setTaskLogMsg(taskId);
        logger(LOG_ERROR | LOG_MSG_TASK);

        char str[36];
        sprintf(str, "[%3u]"s"bakeBread status unknown", task_status[taskId]);
        updateStatusBar(str);
        setErrorCursor();
    }

    // LOG_MSG.LOG_DATA_CONTEXT = LOG_DATA_CONTEXT_TASK_FARM_SOW_EXIT;
    // setTaskLogMsg(taskId);
    // LOG_MSG.LOG_DATA_TASK_PARAMS3 = energyNeeded;
    // LOG_MSG.LOG_DATA_TASK_PARAMS4 = partDone;
    // logger(LOG_DEBUG | LOG_MSG_TASK);

    updateMenuIfIn(MENU_BANK_KITCHEN);
}

#pragma code ( code )
#pragma data ( data )
//-----------------------------------------------------------------------------------------
// Wrappers in RAM

void bakeBreadTask(char taskId){
    char pbank = setBank(TASKS_BANK);
    _bakeBreadTask(taskId);
    setBank(pbank);
}
