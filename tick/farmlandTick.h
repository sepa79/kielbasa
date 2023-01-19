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
    // how much has rippened in stage 3
    unsigned int ready;
    // timer till the end of current stage (days)
    char timer;
};

extern FieldStruct fields[FIELDS_COUNT];

//******************************
// Plant
//******************************

// Plant name is in PLANT_TYPE_TXT
// TXT_PLANT_NAME_POTATO,
// TXT_PLANT_NAME_LUPINE,
// TXT_PLANT_NAME_WHEAT,
// TXT_PLANT_NAME_CORN,
static const byte plant_taskDscIdx[5]             = { TXT_IDX_TASK_EMPTY_DESCRIPTION, TXT_IDX_TASK_DSC_FARMLAND_POTATO, TXT_IDX_TASK_DSC_FARMLAND_LUPINE, TXT_IDX_TASK_DSC_FARMLAND_WHEAT, TXT_IDX_TASK_DSC_FARMLAND_CORN};
static const signed char plant_stage1minTemp[5]   = { 0,  8,  1, -5, -5};
static const signed char plant_stage1maxTemp[5]   = { 0, 20, 15, 15, 30};
static const byte plant_stage1minWater[5]         = { 0, 20, 20,  5,  5};
static const byte plant_stage1maxWater[5]         = { 0, 40, 40, 60, 40};
static const byte plant_stage1timer[5]            = { 0, 15, 20,180, 30};
static const byte plant_stage1upkeep[5]           = { 0,  0,  0,  0,  0};
static const signed char plant_stage2minTemp[5]   = { 0, 18, 13,  8,  5};
static const signed char plant_stage2maxTemp[5]   = { 0, 23, 18, 25, 30};
// static const signed char plant_stage2minTemp[5]   = { 0, 15, 13,  8,  5};
// static const signed char plant_stage2maxTemp[5]   = { 0, 26, 18, 25, 30};
static const byte plant_stage2minWater[5]         = { 0, 10, 15, 10, 10};
static const byte plant_stage2maxWater[5]         = { 0, 30, 40, 40, 40};
static const byte plant_stage2timer[5]            = { 0, 60, 70, 90, 40};
static const byte plant_stage2upkeep[5]           = { 0,  0,  0,  0,  0};
static const signed char plant_stage3minTemp[5]   = { 0, 15, 15, 20, 10};
static const signed char plant_stage3maxTemp[5]   = { 0, 22, 20, 30, 30};
static const byte plant_stage3minWater[5]         = { 0,  5, 15,  5, 10};
static const byte plant_stage3maxWater[5]         = { 0, 20, 40, 20, 50};
static const byte plant_stage3timer[5]            = { 0, 25, 20, 45, 20};
static const byte plant_stage3upkeep[5]           = { 0,  0,  0,  0,  0};
static const byte plant_maxYeldFactor[5]          = { 0, 10,  5,  5,  5};

#define PLANT_STAGE_NONE 0x00
#define PLANT_STAGE_SOW_TASK_ASSIGNED 0x01
#define PLANT_STAGE_SPROUT 0x02
#define PLANT_STAGE_GROWTH 0x03
#define PLANT_STAGE_RIPEN 0x04
#define PLANT_STAGE_READY 0x05
#define PLANT_STAGE_REAP_TASK_ASSIGNED 0x06


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