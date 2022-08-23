#include <c64/types.h>

#include <translation/common.h>
#include "farmlandTick.h"

// how much water is in the ground, aka irrigation (max 100)
volatile byte flt_waterLevel = 50;
volatile word flt_storagePotato = 0;
volatile word flt_storageLupine = 0;
volatile word flt_storageWheat = 0;
volatile word flt_storageCorn = 0;

// used instead of struct
__export byte field_area[4]      = {0,0,0,0};
__export byte field_fertility[4] = {127,127,127,127};
__export byte field_plantId[4]   = {0,0,0,0};
__export byte field_stage[4]     = {0,0,0,0};
__export int field_stage_planted[4]   = {0,0,0,0};
__export int field_stage_grown[4]     = {0,0,0,0};
__export int field_stage_ready[4]     = {0,0,0,0};
__export byte field_timer[4]     = {0,0,0,0};

void initFarmland(){
    flt_waterLevel = 50;
    flt_storagePotato = 219;
    flt_storageLupine = 250;
    flt_storageWheat = 100;
    flt_storageCorn = 125;

    field_area[0] = 1;
    field_area[1] = 1;
    field_area[2] = 2;
    field_area[3] = 4;
}

// =============================================================================
// Tick code
// =============================================================================
// ticks code
// #pragma code ( ticksCode )

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

void _fieldStateSprout(byte fieldId){
    // get our plant data
    byte plantId = field_plantId[fieldId];

    // temp check
    byte diff = _tempCheck(plant_stage1minTemp[plantId], plant_stage1maxTemp[plantId]);
    // gotoxy(0, 16);
    // printf("S 1 plantId [%u]temp diff %-5u ", plantId, diff);
    // rain check
    diff += _waterCheck(plant_stage1minRain[plantId], plant_stage1maxRain[plantId]);

    // gotoxy(20, 16);
    // printf("+ rain diff %-5u ", diff);

    if(diff != 0){
        byte planted = field_stage_planted[fieldId];
        if(diff < planted) {
            field_stage_planted[fieldId] = planted - diff;
        } else {
            field_stage_planted[fieldId] = 0;
        }
    }

    field_timer[fieldId]--;
    if(field_timer[fieldId] == 0){
        if(field_stage_planted[fieldId] == 0){
            // everything died, end growing cycle
            field_stage[fieldId] = PLANT_STAGE_NONE;
        } else {
            field_stage[fieldId] = PLANT_STAGE_GROWTH;
            field_stage_grown[fieldId] = field_stage_planted[fieldId];
            field_timer[fieldId] = plant_stage2timer[plantId];
        }
    }
}

// =============================================================================
// Growth stage

void _fieldStateGrowth(byte fieldId){
    // get our plant data
    byte plantId = field_plantId[fieldId];

    // temp check
    byte diff = _tempCheck(plant_stage2minTemp[plantId], plant_stage2maxTemp[plantId]);
    // gotoxy(0, 17);
    // printf("S 2 temp diff %-5u ", diff);
    // rain check
    diff += _waterCheck(plant_stage2minRain[plantId], plant_stage2maxRain[plantId]);
    // gotoxy(20, 17);
    // printf("+ rain diff %-5u ", diff);

    if(diff == 0){
        field_stage_grown[fieldId]++;
    }

    field_timer[fieldId]--;
    if(field_timer[fieldId] == 0){
        field_stage[fieldId] = PLANT_STAGE_RIPEN;
        field_timer[fieldId] = plant_stage3timer[plantId];
    }
}

// =============================================================================
// Rippen stage

void _fieldStateRipen(byte fieldId){
    // get our plant data
    byte plantId = field_plantId[fieldId];

    // temp check
    byte diff = _tempCheck(plant_stage3minTemp[plantId], plant_stage3maxTemp[plantId]);
    // gotoxy(0, 18);
    // printf("s 3 temp %-5d diff %-5u ", cal_currentTemp, diff);
    // rain check
    diff += _waterCheck(plant_stage2minRain[plantId], plant_stage2maxRain[plantId]);
    // gotoxy(0, 18);
    // printf("temp + rain diff %-5u ", diff);

    if(diff == 0){
        field_stage_ready[fieldId] += 10;
        if(field_stage_ready[fieldId] > 100)
            field_stage_ready[fieldId] = 100;
    }

    field_timer[fieldId]--;
    if(field_timer[fieldId] == 0){
        field_stage[fieldId] = PLANT_STAGE_READY;
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
    if(field_stage[i] == PLANT_STAGE_SPROUT){
        _fieldStateSprout(i);
    }
    else if(field_stage[i] == PLANT_STAGE_GROWTH){
        _fieldStateGrowth(i);
    }
    else if(field_stage[i] == PLANT_STAGE_RIPEN){
        _fieldStateRipen(i);
    }
}

void farmlandTick(){
    // each field gets/loses water every day depending on weather
    _waterFields();
    byte fieldId = 0;
    do {
        // only tick populated fields
        byte id = field_plantId[fieldId];
        if(id != 0){
            _tickField(fieldId);
        }
        fieldId++;
    } while (fieldId <= FIELDS_COUNT);
}

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
