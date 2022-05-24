#include <c64/types.h>

#include <tick/farmlandTick.h>
#include <tasks/taskManager.h>
#include <assets/assetsSettings.h>


// for now anybody can do it.
// later: energy used should depend on skills and stats, and one can't finish it if he runs out of energy
void sowFieldTask(byte taskId){
    // get the fieldId - this is the definition from menu/farmland.c
    // task.params[0] = _currentPlant;
    // task.params[1] = _currentField;
    // task.params[2] = 0;
    // task.params[3] = 0;
    // task.params[4] = 0;
    byte fieldId = task_params[taskId][1];
    byte plantId = task_params[taskId][0];

    field_plantId[fieldId] = plantId;
    field_stage[fieldId]   = PLANT_STAGE_SPROUT;
    field_timer[fieldId]   = plant_stage1timer[plantId];
    field_planted[fieldId] = 100; // in percent
    field_grown[fieldId]   = 0;
    field_ready[fieldId]   = 0;

    // task done, remove
    removeTask(taskId);
}