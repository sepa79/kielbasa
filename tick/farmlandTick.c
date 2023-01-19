#include <c64/types.h>
#include <fixmath.h>

#include <translation/common.h>
#include "farmlandTick.h"

// dynamic data - in RAM
#pragma data ( data )

// how much water is in the ground, aka irrigation (max 100)
volatile byte flt_waterLevel = 30;
// add 1 as plant '0' is ----- (nothing planted) in farm management
volatile unsigned int flt_storage[PLANTS_COUNT+1] = 0;

__striped struct FieldStruct fields[FIELDS_COUNT];
__striped const struct PlantStruct plants[PLANTS_COUNT+1] = {
    {TXT_IDX_TASK_EMPTY_DESCRIPTION, 0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0 },
    {TXT_IDX_TASK_DSC_FARMLAND_POTATO, 10,  8,20,20,40, 15,  18,23,10,30,60, 15,22, 5,20,25 },
    {TXT_IDX_TASK_DSC_FARMLAND_LUPINE,  5,  1,15,20,40, 20,  13,18,15,40,70, 15,20,15,40,20 },
    {TXT_IDX_TASK_DSC_FARMLAND_WHEAT,   5, -5,15, 5,60,180,   8,25,10,40,90, 20,30, 5,20,45 },
    {TXT_IDX_TASK_DSC_FARMLAND_CORN,    5, -5,30, 5,40, 30,   5,30,10,40,40, 10,30,10,50,20 }
};

void initFarmland(){
    flt_waterLevel = 25;
    flt_storage[PLANT_POTATO] = 900;
    flt_storage[PLANT_LUPINE] = 50;
    flt_storage[PLANT_WHEAT]  = 50;
    flt_storage[PLANT_CORN]   = 50;

    static const struct FieldStruct f0 = {1, 0, 0, 0, 0, 0, 0, 0};
    static const struct FieldStruct f1 = {1, 0, 0, 0, 0, 0, 0, 0};
    static const struct FieldStruct f2 = {2, 0, 0, 0, 0, 0, 0, 0};
    static const struct FieldStruct f3 = {4, 0, 0, 0, 0, 0, 0, 0};

    fields[0] = f0;
    fields[1] = f1;
    fields[2] = f2;
    fields[3] = f3;
}

// =============================================================================
// Tick code
// =============================================================================
// ticks code
#pragma code ( ticksCode )

byte _tempCheck(signed char min, signed char max){
    if(cal_currentTemp < min){
        return (char) babs(cal_currentTemp - min);
    }
    if(cal_currentTemp > max){
        return (char) babs(cal_currentTemp - max);
    }
    return 0;
}

byte _waterCheck(byte min, byte max){
    if(flt_waterLevel < min){
        return min - flt_waterLevel;
    }
    if(flt_waterLevel > max){
        return flt_waterLevel - max;
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
        int planted = fields[fieldId].planted;
        if(diff < planted) {
            fields[fieldId].planted = planted - diff;
        } else {
            fields[fieldId].planted = 0;
        }
    }

    fields[fieldId].timer--;
    if(fields[fieldId].timer == 0){
        if(fields[fieldId].planted == 0){
            // everything died, end growing cycle
            fields[fieldId].stage = PLANT_STAGE_NONE;
        } else {
            fields[fieldId].stage = PLANT_STAGE_GROWTH;
            fields[fieldId].grown = fields[fieldId].planted;
            fields[fieldId].timer = plants[plantId].stage2timer;

            // calculate growth factor
            fields[fieldId].gFactor = lmuldiv16u(fields[fieldId].planted, plants[plantId].maxYeldFactor, plants[plantId].stage2timer);
        }
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
    // printf("s 3 temp %-5d diff %-5u ", cal_currentTemp, diff);
    // rain check
    diff += _waterCheck(plants[plantId].stage2minWater, plants[plantId].stage2maxWater);
    // gotoxy(0, 18);
    // printf("temp + rain diff %-5u ", diff);

    if(diff > 10)
        diff = 10;

    fields[fieldId].ready += 10 - diff;

    if(fields[fieldId].ready > 100)
        fields[fieldId].ready = 100;

    fields[fieldId].timer--;
    if(fields[fieldId].timer == 0){
        fields[fieldId].stage = PLANT_STAGE_READY;
        // apply the percentage to the 'grown' plants
        if(fields[fieldId].ready < 100)
            fields[fieldId].grown = lmuldiv16u(fields[fieldId].grown, 100, fields[fieldId].ready);
    }
}

// =============================================================================
// Tick itself

void _waterFields(){
    // always vaporise/soak some
    byte vaporised = 2;

    // now depending on temperature - vaporise more
    if(cal_currentTemp > 25){
        vaporised ++;
    }
    if(cal_currentTemp > 20){
        vaporised ++;
    }
    if(cal_currentTemp < 6){
        if(flt_waterLevel < 25){
            flt_waterLevel = 25;
        } else if(flt_waterLevel > 50){
            flt_waterLevel = 50;
        }
    }
    // at low water levels slow down vaporisation
    if(flt_waterLevel < 8)
        vaporised --;

    if(vaporised < cal_currentRain){
        flt_waterLevel += cal_currentRain - vaporised;
        if(flt_waterLevel > 100){
            flt_waterLevel = 100;
        }
    } else {
        byte diff = vaporised - cal_currentRain;
        if(flt_waterLevel > diff){
            flt_waterLevel -= diff;
        } else {
            flt_waterLevel = 0;
        }
    }
}

void _tickField(byte i){
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

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
