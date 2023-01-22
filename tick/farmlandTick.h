#ifndef FARMLAND_TICK_H
#define FARMLAND_TICK_H
#include <c64/types.h>
#include <translation/common.h>

#define FIELDS_COUNT 4
#define PLANTS_COUNT 4
// capacity is determined by Map field size. As tiles are 4x4, and field size 1 is 6 tiles - 96 is the number
#define FIELD_CAPACITY 96

enum PantName {
    PLANT_NONE,
    PLANT_POTATO,
    PLANT_LUPINE,
    PLANT_WHEAT,
    PLANT_CORN,
};
// Plant name is in PLANT_TYPE_TXT
// TXT_PLANT_NAME_POTATO,
// TXT_PLANT_NAME_LUPINE,
// TXT_PLANT_NAME_WHEAT,
// TXT_PLANT_NAME_CORN,

extern volatile byte flt_waterLevel = 50;

// storage for farming
// add 1 as plant '0' is ----- (nothing planted) in farm management
// makes it easier in tasks etc to manage IDs
extern volatile unsigned int flt_storage[PLANTS_COUNT+1];

//******************************
// Field
//******************************

struct FieldStruct {
    // area of the field
    char area;
    // on map width, in tiles
    char width;
    // on map height, in tiles
    char height;
    // what plant is on it
    char plantId;
    // which growth stage is the plant in
    char stage;
    // random seed used to visualise this field
    unsigned int rseed;
    // how much was planted in stage 1
    unsigned int planted;
    // how much have survived stage 1
    unsigned int alive;
    // how much was grown in stage 2
    unsigned int grown;
    // growth factor calculated at the end of the 1st stage
    unsigned int gFactor;
    // how much has rippened in stage 3, percent
    unsigned int ready;
    // used to show nicely how much is left during reaping
    unsigned int reaped;
    // timer till the end of current stage (days)
    char timer;
};

extern __striped struct FieldStruct fields[FIELDS_COUNT];

//******************************
// Plant
//******************************

struct PlantStruct {
    // Text used in task description, usualy lower case plant name
    const char        taskDscIdx;
    const char        maxYeldFactor;

    const signed char stage1minTemp;
    const signed char stage1maxTemp;
    const char        stage1minWater;
    const char        stage1maxWater;
    const char        stage1timer;

    const signed char stage2minTemp;
    const signed char stage2maxTemp;
    const char        stage2minWater;
    const char        stage2maxWater;
    const char        stage2timer;

    const signed char stage3minTemp;
    const signed char stage3maxTemp;
    const char        stage3minWater;
    const char        stage3maxWater;
    const char        stage3timer;
};

// add 1 as plant '0' is ----- (nothing planted) in farm management
// makes it easier in tasks etc to manage IDs
extern __striped const struct PlantStruct plants[PLANTS_COUNT+1];

enum PLANT_STAGE {
    PLANT_STAGE_NONE,
    PLANT_STAGE_SOW_TASK_ASSIGNED,
    PLANT_STAGE_SPROUT,
    PLANT_STAGE_GROWTH,
    PLANT_STAGE_RIPEN,
    PLANT_STAGE_READY,
    PLANT_STAGE_REAP_TASK_ASSIGNED,
};


//-----------------------------------------------------------------------------------------
#pragma code ( gameInitRAMCode )
#pragma data ( gameInitData )
//-----------------------------------------------------------------------------------------
void initFarmland();

//-----------------------------------------------------------------------------------------
#pragma code ( ticksCode )
#pragma data ( data )
//-----------------------------------------------------------------------------------------

void farmlandTick();

//-----------------------------------------------------------------------------------------
#pragma code ( code )
#pragma data ( data )
//-----------------------------------------------------------------------------------------
#pragma compile("farmlandTick.c")

#endif