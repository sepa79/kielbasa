#ifndef FARMLAND_TICK_H
#define FARMLAND_TICK_H
#include <c64/types.h>
#include <translation/common.h>

// starts from 0, so +1
#define FIELDS_COUNT 3
#define PLANTS_COUNT 4

extern volatile byte flt_waterLevel = 50;
extern volatile word flt_storagePotato = 0;
extern volatile word flt_storageLupine = 0;
extern volatile word flt_storageWheat = 0;
extern volatile word flt_storageCorn = 0;


//******************************
// Field
//******************************
// area of the field
extern byte field_area[4]      ;
// fertility left
extern byte field_fertility[4] ;
// what plant is on it
extern byte field_plantId[4]   ;
// which growth stage is the plant in
extern byte field_stage[4]     ;
// how much was planted in stage 1
extern byte field_planted[4]   ;
// how much was grown in stage 2
extern byte field_grown[4]     ;
// how much has rippened in stage 3
extern byte field_ready[4]     ;
// timer till the end of current stage (days)
extern byte field_timer[4]     ;

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
static const byte plant_stage1minRain[5]          = { 0, 20, 20,  5,  5};
static const byte plant_stage1maxRain[5]          = { 0, 40, 40, 60, 40};
static const byte plant_stage1timer[5]            = { 0, 15, 20,180, 30};
static const byte plant_stage1upkeep[5]           = { 0,  0,  0,  0,  0};
static const signed char plant_stage2minTemp[5]   = { 0, 15, 13,  8,  5};
static const signed char plant_stage2maxTemp[5]   = { 0, 26, 18, 25, 30};
static const byte plant_stage2minRain[5]          = { 0, 10, 15, 10, 10};
static const byte plant_stage2maxRain[5]          = { 0, 30, 40, 40, 40};
static const byte plant_stage2timer[5]            = { 0, 60, 70, 90, 40};
static const byte plant_stage2upkeep[5]           = { 0,  0,  0,  0,  0};
static const signed char plant_stage3minTemp[5]   = { 0, 15, 15, 20, 10};
static const signed char plant_stage3maxTemp[5]   = { 0, 22, 20, 30, 30};
static const byte plant_stage3minRain[5]          = { 0,  5, 15,  5, 10};
static const byte plant_stage3maxRain[5]          = { 0, 20, 40, 20, 50};
static const byte plant_stage3timer[5]            = { 0, 25, 20, 45, 20};
static const byte plant_stage3upkeep[5]           = { 0,  0,  0,  0,  0};

#define PLANT_STAGE_NONE 0x00
#define PLANT_STAGE_SOW_TASK_ASSIGNED 0x01
#define PLANT_STAGE_SPROUT 0x02
#define PLANT_STAGE_GROWTH 0x03
#define PLANT_STAGE_RIPEN 0x04
#define PLANT_STAGE_READY 0x05
#define PLANT_STAGE_REAP_TASK_ASSIGNED 0x06

void initFarmland();
void farmlandTick();

#pragma compile("farmlandTick.c")

#endif