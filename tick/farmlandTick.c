#include <c64/types.h>
#include <string.h>
#include <fixmath.h>

#include <engine/gameState.h>
#include <translation/common.h>
#include "farmlandTick.h"

// dynamic data - in RAM
#pragma data ( data )

// REPLACED by GameState
// how much water is in the ground, aka irrigation (max 100)
// volatile byte farm->waterLevel = 30;
// add 1 as plant '0' is ----- (nothing planted) in farm management
// volatile unsigned int farm->storage[PLANTS_COUNT+1] = {0};

__striped struct FieldStruct fields[FIELDS_COUNT];
__striped const struct PlantStruct plants[PLANTS_COUNT+1] = {
    {TXT_IDX_TASK_EMPTY_DESCRIPTION, 0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0 },
    {TXT_IDX_TASK_DSC_FARMLAND_POTATO, 10,  8,20,20,40, 15,  18,23,10,30,60, 15,22, 5,20,25 }, // Plant 1st May, high yeld, but fragile, 100 days to mature
    {TXT_IDX_TASK_DSC_FARMLAND_LUPINE,  8,  1,15,20,40, 20,  13,18,15,40,70, 15,20,15,40,20 }, // Plant end of April, high yeld, fragile, 110 days to mature
    {TXT_IDX_TASK_DSC_FARMLAND_WHEAT,   7, -5,15, 5,60,180,   8,25,10,40,90, 20,30, 5,20,45 }, // Plant September, medium yeld, long growth, robust, 315 days to mature
    {TXT_IDX_TASK_DSC_FARMLAND_CORN,    4, -5,30, 5,40, 30,   5,30,10,40,40, 20,30,10,50,30 }, // Plant end of winter, low yeld, very robust, 100 days to mature
};

// =============================================================================
// Tick code
#pragma code ( ticksCode )
// =============================================================================

byte _tempCheck(signed char min, signed char max){
    if(GS.calendar.currentTemp < min){
        return (char) babs(GS.calendar.currentTemp - min);
    }
    if(GS.calendar.currentTemp > max){
        return (char) babs(GS.calendar.currentTemp - max);
    }
    return 0;
}

byte _waterCheck(byte min, byte max){
    if(GS.farm.waterLevel < min){
        return min - GS.farm.waterLevel;
    }
    if(GS.farm.waterLevel > max){
        return GS.farm.waterLevel - max;
    }
    return 0;
}

// =============================================================================
// Sprout stage
// In this stage we are only removing some sprouts (they can die if conditions are not optimal)
void _fieldStateSprout(byte fieldId){
    // get our plant data
    byte plantId = fields[fieldId].plantId;

    // temp check
    byte diff = _tempCheck(plants[plantId].stage1minTemp, plants[plantId].stage1maxTemp);
    // gotoxy(0, 16);
    // printf("S 1 plantId [%u]temp diff %-5u ", plantId, diff);
    // rain check
    diff += _waterCheck(plants[plantId].stage1minWater, plants[plantId].stage1maxWater);

    // gotoxy(20, 16);
    // printf("+ rain diff %-5u ", diff);

    if(diff != 0){
        int alive = fields[fieldId].alive;
        if(diff < alive) {
            fields[fieldId].alive = alive - diff;
        } else {
            fields[fieldId].alive = 0;
            // end it if all died
            fields[fieldId].stage = PLANT_STAGE_NONE;
        }
    }

    // // calculate health
    // fields[fieldId].health = lmuldiv16u(fields[fieldId].alive, FIELD_HEALTH_MAX, fields[fieldId].grown);

    fields[fieldId].timer--;
    if(fields[fieldId].timer == 0){
        fields[fieldId].stage = PLANT_STAGE_GROWTH;
        fields[fieldId].grown = fields[fieldId].alive;
        fields[fieldId].timer = plants[plantId].stage2timer;

        // calculate growth factor
        unsigned int gFactor = lmuldiv16u(fields[fieldId].alive, plants[plantId].maxYeldFactor, plants[plantId].stage2timer);

        // make it gro a bit, even if we got just a few
        if(!gFactor){
            gFactor++;
        }
        fields[fieldId].gFactor = gFactor;
    }
}

// =============================================================================
// Growth stage
void _fieldStateGrowth(byte fieldId){
    // get our plant data
    byte plantId = fields[fieldId].plantId;

    // temp check
    byte diff = _tempCheck(plants[plantId].stage2minTemp, plants[plantId].stage2maxTemp);
    // gotoxy(0, 17);
    // printf("S 2 temp diff %-5u ", diff);
    // rain check
    diff += _waterCheck(plants[plantId].stage2minWater, plants[plantId].stage2maxWater);
    // gotoxy(20, 17);
    // printf("+ rain diff %-5u ", diff);

    // adjust the diff for the size of field
    byte adjustedDiff = diff*fields[fieldId].area;

    if(adjustedDiff < fields[fieldId].gFactor){
        fields[fieldId].grown += fields[fieldId].gFactor - adjustedDiff;
    }

    fields[fieldId].timer--;
    if(fields[fieldId].timer == 0){
        fields[fieldId].stage = PLANT_STAGE_RIPEN;
        fields[fieldId].timer = plants[plantId].stage3timer;
    }
}

// =============================================================================
// Rippen stage

void _fieldStateRipen(byte fieldId){
    // get our plant data
    byte plantId = fields[fieldId].plantId;

    // temp check
    byte diff = _tempCheck(plants[plantId].stage3minTemp, plants[plantId].stage3maxTemp);
    // gotoxy(0, 18);
    // printf("s 3 temp %-5d diff %-5u ", GS.calendar.currentTemp, diff);
    // rain check
    diff += _waterCheck(plants[plantId].stage2minWater, plants[plantId].stage2maxWater);
    // gotoxy(0, 18);
    // printf("temp + rain diff %-5u ", diff);

    if(diff > 10)
        diff = 10;

    fields[fieldId].ready += 10 - diff;

    if(fields[fieldId].ready > 100){
        fields[fieldId].ready = 100;
        // all rippened? It's ready
        fields[fieldId].stage = PLANT_STAGE_READY;
    }

    fields[fieldId].timer--;
    if(fields[fieldId].timer == 0){
        fields[fieldId].stage = PLANT_STAGE_READY;
        // apply the percentage to the 'grown' plants
        if(fields[fieldId].ready < 100)
            fields[fieldId].grown = lmuldiv16u(fields[fieldId].grown, fields[fieldId].ready, 100);
        // calculate the amount to reap which decreases 'alive', store in gFactor as its not needed now
        fields[fieldId].gFactor = ldiv16u(fields[fieldId].grown, fields[fieldId].alive);
    }
}

// =============================================================================
// Tick itself

static void _waterFields(){
    // always vaporise/soak some
    byte vaporised = 2;

    // now depending on temperature - vaporise more
    if(GS.calendar.currentTemp > 25){
        vaporised ++;
    }
    if(GS.calendar.currentTemp > 20){
        vaporised ++;
    }
    if(GS.calendar.currentTemp < 6){
        if(GS.farm.waterLevel < 25){
            GS.farm.waterLevel = 25;
        } else if(GS.farm.waterLevel > 50){
            GS.farm.waterLevel = 50;
        }
    }
    // at low water levels slow down vaporisation
    if(GS.farm.waterLevel < 8)
        vaporised --;

    if(vaporised < GS.calendar.currentRain){
        GS.farm.waterLevel += GS.calendar.currentRain - vaporised;
        if(GS.farm.waterLevel > 100){
            GS.farm.waterLevel = 100;
        }
    } else {
        byte diff = vaporised - GS.calendar.currentRain;
        if(GS.farm.waterLevel > diff){
            GS.farm.waterLevel -= diff;
        } else {
            GS.farm.waterLevel = 0;
        }
    }
}

static void _tickField(byte i){
    if(fields[i].stage == PLANT_STAGE_SPROUT){
        _fieldStateSprout(i);
    }
    else if(fields[i].stage == PLANT_STAGE_GROWTH){
        _fieldStateGrowth(i);
    }
    else if(fields[i].stage == PLANT_STAGE_RIPEN){
        _fieldStateRipen(i);
    }
}

void farmlandTick(){
    // each field gets/loses water every day depending on weather
    _waterFields();
    byte fieldId = 0;
    do {
        // only tick populated fields
        byte id = fields[fieldId].plantId;
        if(id != 0){
            _tickField(fieldId);
        }
        fieldId++;
    } while (fieldId < FIELDS_COUNT);
}


//-----------------------------------------------------------------------------------------
// In Init bank
#pragma code ( gameInitRAMCode )
#pragma data ( gameInitData )
//-----------------------------------------------------------------------------------------
void initFarmland(Farm* farm){
    farm->waterLevel = 25;
    farm->storage[PLANT_POTATO] = 150;
    farm->storage[PLANT_LUPINE] = 50;
    farm->storage[PLANT_WHEAT]  = 500;
    farm->storage[PLANT_CORN]   = 50;

    __striped static const struct FieldStruct initialFields[FIELDS_COUNT] = {
        {1, 3, 2, PLANT_WHEAT , PLANT_STAGE_GROWTH, 666, 96, 90, 448, 15, 0, 0, 60},
        {1, 3, 2, PLANT_NONE  , 0, 0, 0, 0, 0, 0, 0, 0},
        {2, 3, 4, PLANT_CORN  , PLANT_STAGE_GROWTH, 666, 96, 80, 240, 8, 0, 0, 10},
        {4, 4, 6, PLANT_NONE  , 0, 0, 0, 0, 0, 0, 0, 0},
        // {1, 3, 2, PLANT_WHEAT , PLANT_STAGE_GROWTH, 666, 96, 90, 448, 15, 0, 0, 60, 0},
        // {1, 3, 2, PLANT_NONE  , 0, 0, 0, 0, 0, 0, 0, 0, 0},
        // {2, 3, 4, PLANT_CORN  , PLANT_STAGE_GROWTH, 666, 96, 80, 240, 8, 0, 0, 10, 0},
        // {4, 4, 6, PLANT_NONE  , 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    memcpy(fields, initialFields, sizeof(initialFields));

    for(char i=0; i<FIELDS_COUNT; i++){
        fields[i].rseed = rand();
    }
}

//-----------------------------------------------------------------------------------------
// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
//-----------------------------------------------------------------------------------------