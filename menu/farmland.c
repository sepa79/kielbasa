#include <c64/charwin.h>
#include <c64/keyboard.h>
#include <c64/vic.h>
#include <c64/types.h>
#include <stdio.h>
#include <fixmath.h>

#include <menu/farmland.h>
#include <engine/gameState.h>
#include <menu/menuSystem.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/uiHandler.h>
#include <engine/gameSettings.h>
#include <tasks/taskManager.h>
#include <character/character.h>
#include <tick/calendar.h>
#include <tick/farmlandTick.h>
#include <tasks/farmlandTask.h>

// column offset for printing data
#define COL_OFFSET_MENU 22
#define COL_OFFSET_FIELDLIST 0
#define ROW_OFFSET_FIELDLIST 1

static bool _confirm = false;
static byte _currentField = 0;
static byte _currentPlant = 0;
static byte _waterSpriteBank = 0;

enum FARMLAND_SPRITE_VIC_BANKS {
    SPR_BANK_THERMO_BAR_1=0xbc,
    SPR_BANK_THERMO_BAR_2,
    SPR_BANK_THERMO_BAR_3,
    SPR_BANK_POTATO_UI,
    SPR_BANK_LUPINE_UI,
    SPR_BANK_WHEAT_UI,
    SPR_BANK_CORN_UI,
    SPR_BANK_WATER_UI,
};

#define SPR_THERMO_BAR_1 ((char *)GFX_1_BASE + 64*SPR_BANK_THERMO_BAR_1)
#define SPR_THERMO_BAR_2 ((char *)GFX_1_BASE + 64*SPR_BANK_THERMO_BAR_2)
#define SPR_THERMO_BAR_3 ((char *)GFX_1_BASE + 64*SPR_BANK_THERMO_BAR_3)
#define SPR_POTATO_UI    ((char *)GFX_1_BASE + 64*SPR_BANK_POTATO_UI)
#define SPR_LUPINE_UI    ((char *)GFX_1_BASE + 64*SPR_BANK_LUPINE_UI)
#define SPR_WHEAT_UI     ((char *)GFX_1_BASE + 64*SPR_BANK_WHEAT_UI)
#define SPR_CORN_UI      ((char *)GFX_1_BASE + 64*SPR_BANK_CORN_UI)
#define SPR_WATER_UI     ((char *)GFX_1_BASE + 64*SPR_BANK_WATER_UI)

// Sections and regions
#pragma section( farmlandLoaderData, 0 )
#pragma section( farmlandCode, 0 )
#pragma section( farmlandGfxDay, 0 )
#pragma section( farmlandGfxNight, 0 )
#pragma section( farmlandSprites, 0 )
#pragma region( bankFarmlandC, 0x8000, 0x9000, , MENU_BANK_FARMLAND, { farmlandLoaderData, farmlandCode } )
#pragma region( bankFarmlandG1, DAY_GFX_BMP, DAY_GFX_BMP+0x12c0, , MENU_BANK_FARMLAND, { farmlandGfxDay } )
#pragma region( bankFarmlandG2, NIGHT_GFX_BMP, NIGHT_GFX_BMP+0x12c0, , MENU_BANK_FARMLAND, { farmlandGfxNight } )
#pragma region( bankFarmlandG3, MENU_SPRITE_SRC, MENU_SPRITE_SRC+0x0400, , MENU_BANK_FARMLAND, { farmlandSprites } )

#pragma data ( farmlandGfxDay )
__export const byte farmlandGfx1[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/pole200623fix.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/pole200623fix.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/pole200623fix.kla"
};
#pragma data ( farmlandGfxNight )
__export const byte farmlandGfx2[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/farmland_bottomBar_night.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/farmland_bottomBar_night.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/farmland_bottomBar_night.kla"
};

#pragma data ( farmlandSprites )

__export char SPR_DATA_THERMO_BAR_12[64*2] = {0};
__export char SPR_DATA_THERMO_BAR_3[64*1] = {
// Sprite Thermometer bar, lowest
// Single color mode, BG color: 0, Sprite color: 14
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0xF0, 0x00, 0x00,
    0xF0, 0x00, 0x00,0,
};

__export char SPR_DATA_PLANTS_[64*4] = {0};

__export const char farmlandWaterSprites[] = {
    #embed 0x0400 20 "assets/sprites/waterMarker.spd"
};

// menu code is in ROM - data in RAM
#pragma code ( farmlandCode )
#pragma data ( data )

// min 'normal' temp is -23, so temp shall start at '0' which is -23
static void _drawThermometer(byte temp){
    // vic.color_border++;
    // sprite 1
    // TODO: can be done on 2 sprites - bottom one as is now, top one just moved on Y, constant max height. Sprite 2 routine moves sprite, Sprite 1 - doubles height and moves.
    if(temp > 40){
        byte max = 61;
        byte i = 0;
        do {
            if(temp >= max){
                SPR_THERMO_BAR_1[i] = 0b00110000;
            } else {
                SPR_THERMO_BAR_1[i] = 0b00000000;
            }
            i += 3;
            max --;
        } while (i < 64);
    } else {
        // zero it
        byte i = 0;
        do {
            SPR_THERMO_BAR_1[i] = 0b00000000;
            i += 3;
        } while (i < 64);
    }

    // sprite 2
    if(temp > 19){
        byte max = 40;
        byte i = 0;
        do {
            if(temp >= max){
                SPR_THERMO_BAR_2[i] = 0b00110000;
            } else {
                SPR_THERMO_BAR_2[i] = 0b00000000;
            }
            i += 3;
            max --;
        } while (i < 64);
    } else {
        // zero it
        byte i = 0;
        do {
            SPR_THERMO_BAR_2[i] = 0b00000000;
            i += 3;
        } while (i < 64);
    }
    // sprite 3 covers -22 to -3 (2px used for thermometer mercury container at the bottom)
    byte max = 19;
    byte i = 0;
    do {
        if(temp >= max){
            SPR_THERMO_BAR_3[i] = 0b00110000;
        } else {
            SPR_THERMO_BAR_3[i] = 0b00000000;
        }
        i += 3;
        max --;
    } while (i < 55);
    // vic.color_border--;
}

static void _showWaterLevel(){
    char wLevel = GS.farm.waterLevel > 42 ? 42 : GS.farm.waterLevel;
    wLevel = wLevel / 2;
    // byteToSprite(wLevel, SPR_TXT_UP_1);
    char i = 0;
    char c = 21 - wLevel;
    while (c){
        SPR_WATER_UI[i] = 0b00000000;
        i++;
        i++;
        i++;
        c--;
    }
    while (i<64){
        SPR_WATER_UI[i] = 0b11111111;
        i++;
        i++;
        i++;
    } ;
}

__interrupt static void _menuShowSprites(){
    vic.spr_enable   = 0b11111111;
    vic.spr_expand_x = 0b00000000;
    vic.spr_expand_y = 0b00000000;
    vic.spr_priority = 0b10000000;
    vic.spr_multi    = 0b00000000;
    vic.spr_msbx     = 0b01100000;

    // thermometer sprites
    vic.spr_pos[0].x = 34;
    vic.spr_pos[1].x = 34;
    vic.spr_pos[2].x = 34;
    vic.spr_pos[0].y = 87+50-63;
    vic.spr_pos[1].y = 87+50-42;
    vic.spr_pos[2].y = 87+50-21;

    // plant ui sprites
    vic.spr_pos[3].x = 24+148;
    vic.spr_pos[4].x = 24+196;
    vic.spr_pos[5].x = 12;
    vic.spr_pos[6].x = 60;
    vic.spr_pos[3].y = 50+67+2;
    vic.spr_pos[4].y = 50+67+2;
    vic.spr_pos[5].y = 50+67+2;
    vic.spr_pos[6].y = 50+67+2;

    vic.spr_color[0] = VCOL_LT_BLUE;
    vic.spr_color[1] = VCOL_LT_BLUE;
    vic.spr_color[2] = VCOL_LT_BLUE;
    vic.spr_color[3] = VCOL_MED_GREY;
    vic.spr_color[4] = VCOL_MED_GREY;
    vic.spr_color[5] = VCOL_MED_GREY;
    vic.spr_color[6] = VCOL_MED_GREY;

    if(_currentPlant){
        vic.spr_color[2+_currentPlant] = VCOL_LT_GREY;
    }
    if(fields[_currentField].plantId){
        vic.spr_color[2+fields[_currentField].plantId] = VCOL_WHITE;
    }

    GFX_1_SCR[OFFSET_SPRITE_PTRS+0] = SPR_BANK_THERMO_BAR_1;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+1] = SPR_BANK_THERMO_BAR_2;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+2] = SPR_BANK_THERMO_BAR_3;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+3] = SPR_BANK_POTATO_UI;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+4] = SPR_BANK_LUPINE_UI;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+5] = SPR_BANK_WHEAT_UI;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+6] = SPR_BANK_CORN_UI;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+7] = SPR_BANK_WATER_UI;
    
    // water sprite
    vic.spr_pos[7].x = 116+24;
    vic.spr_pos[7].y = 65+50;
    if(GS.calendar.currentTemp < 1){
        vic.spr_color[7] = VCOL_LT_BLUE;
    } else {
        vic.spr_color[7] = VCOL_BLUE;
    }
}


// Set the plant task
static void _sowPlant(){
    if(_currentPlant == 0){
        updateStatusBarError(TXT_CACHE[LTXT_IDX_CONFIRM_ERROR]);
        return;
    }
    // // must be plowed
    // if(fields[_currentField].stage != PLANT_STAGE_PLOWED){
    //     setErrorCursor();
    //     return;
    // }
    // indicate task is assigned
    fields[_currentField].plantId = _currentPlant;
    fields[_currentField].stage   = PLANT_STAGE_SOW_TASK_ASSIGNED;
    fields[_currentField].rseed   = rand();
    fields[_currentField].timer   = 0;
    fields[_currentField].planted = 0;
    fields[_currentField].alive   = 0;
    fields[_currentField].grown   = 0;
    fields[_currentField].ready   = 0;
    // _displayFieldList();

    byte idx = plants[_currentPlant].taskDscIdx;
    // "Field 2, Potatoes"
    sprintf(newTask.desc, "%s %u, %s", TXT_CACHE[LTXT_IDX_TASK_DSC_FARMLAND_FIELD], _currentField+1, TXT[idx]);
    newTask.codeRef   = &sowFieldTask;
    newTask.nameIdx   = TXT_IDX_TASK_FARMLAND_FARM;
    newTask.params[0] = _currentPlant;
    newTask.params[1] = _currentField;
    newTask.params[2] = 0;
    newTask.params[3] = 0;
    newTask.params[4] = 0;
    newTask.reqType   = SKILL_FARMING;
    newTask.icon      = SPR_TASK_FARM2;
    newTask.status    = TASK_STATUS_NEW;
    addTask();
}

static void _reapPlant(){
    // don't allow to ruin growth in progress
    if(fields[_currentField].stage != PLANT_STAGE_READY || !_confirm){
        updateStatusBarError(TXT_CACHE[LTXT_IDX_CONFIRM_ERROR]);
        return;
    }
    // indicate task is assigned
    fields[_currentField].stage = PLANT_STAGE_REAP_TASK_ASSIGNED;
    // _displayFieldList();

    byte idx = plants[fields[_currentField].plantId].taskDscIdx;
    // "Field 2, Potatoes"
    sprintf(newTask.desc, "%s %u, %s", TXT_CACHE[LTXT_IDX_TASK_DSC_FARMLAND_FIELD], _currentField+1, TXT[idx]);
    newTask.codeRef   = &reapFieldTask;
    newTask.nameIdx   = TXT_IDX_TASK_FARMLAND_REAP;
    newTask.params[0] = _currentField;
    newTask.params[1] = 0;
    newTask.params[2] = 0;
    newTask.params[3] = 0;
    newTask.params[4] = 0;
    newTask.reqType   = SKILL_FARMING;
    newTask.icon      = SPR_TASK_FARM2;
    newTask.status    = TASK_STATUS_NEW;
    addTask();
}

// Plow can be called anytime to destroy what's on the field
static void _plowField() {
    // don't allow to ruin growth in progress
    if(!_confirm){
        updateStatusBarError(TXT_CACHE[LTXT_IDX_CONFIRM_ERROR]);
        return;
    }
    // Indicate task is assigned
    fields[_currentField].plantId = PLANT_NONE;
    fields[_currentField].stage   = PLANT_STAGE_PLOW_TASK_ASSIGNED;
    fields[_currentField].rseed   = rand();
    fields[_currentField].timer   = 0;
    fields[_currentField].planted = 0;
    fields[_currentField].alive   = 0;
    fields[_currentField].grown   = 0;
    fields[_currentField].ready   = 0;

    // "Field 2"
    sprintf(newTask.desc, "%s %u", TXT_CACHE[LTXT_IDX_TASK_DSC_FARMLAND_FIELD], _currentField + 1);
    newTask.codeRef   = &plowFieldTask;
    newTask.nameIdx   = TXT_IDX_TASK_FARMLAND_PLOW;
    newTask.params[0] = _currentField;
    newTask.params[1] = 0;
    newTask.params[2] = 0;
    newTask.params[3] = 0;
    newTask.params[4] = 0;
    newTask.reqType   = SKILL_FARMING;
    newTask.icon      = SPR_TASK_FARM2;
    newTask.status    = TASK_STATUS_NEW;
    addTask();
}

#define FARM_CTX_MENU_X 23
#define FARM_CTX_MENU_Y 5

// Prepare output window
static CharWin cd;
static CharWin ht;

static void _showField(){
    cwin_putat_string_raw(&cw, FARM_CTX_MENU_X+4, 1, TXT_CACHE[LTXT_IDX_FARM_FIELD_TXT], VCOL_MED_GREY);
    byte str[2];
    sprintf(str, "%u", _currentField+1);
    cwin_putat_string(&cw, FARM_CTX_MENU_X+4+7, 1, str, VCOL_LT_GREY);
}

static void _showConfirmBox(){
    // Confirm:
    cwin_putat_string_raw(&cw, FARM_CTX_MENU_X+4, 2, TXT_CACHE[LTXT_IDX_CONFIRM_TXT], VCOL_MED_GREY);
    // yes/no
    if(_confirm){
        cwin_putat_string_raw(&cw, FARM_CTX_MENU_X+4, 3, TXT_CACHE[LTXT_IDX_CONFIRM_YES], VCOL_LT_GREY);
    } else {
        cwin_putat_string_raw(&cw, FARM_CTX_MENU_X+4, 3, TXT_CACHE[LTXT_IDX_CONFIRM_NO], VCOL_LT_GREY);
    }
    // arrows
    cwin_putat_string_raw(&cw, FARM_CTX_MENU_X+2, 3, "\x1f", VCOL_MED_GREY);
    cwin_putat_string_raw(&cw, FARM_CTX_MENU_X+14, 3, "$", VCOL_MED_GREY);
}

static void _showOptionPlow(){
    // cwin_clear(&cd);
    cwin_putat_string_raw(&cw, FARM_CTX_MENU_X, FARM_CTX_MENU_Y, TXT_CACHE[LTXT_IDX_MENU_FARMLAND_PLOW], VCOL_DARK_GREY);
    cwin_write_string_raw(&ht, TXT_CACHE[LTXT_IDX_DESC_FARMLAND_PLOW]);

    _showField();
    _showConfirmBox();
}
static void _showOptionSow(){
    cwin_putat_string_raw(&cw, FARM_CTX_MENU_X, FARM_CTX_MENU_Y, TXT_CACHE[LTXT_IDX_MENU_FARMLAND_SOW], VCOL_DARK_GREY);
    cwin_write_string_raw(&ht, TXT_CACHE[LTXT_IDX_DESC_FARMLAND_SOW]);

    _showField();

    // Growing:
    cwin_putat_string_raw(&cw, FARM_CTX_MENU_X+4, 2, TXT_CACHE[LTXT_IDX_FARM_PLANT_TXT], VCOL_MED_GREY);
    // plantname
    cwin_putat_string_raw(&cw, FARM_CTX_MENU_X+4, 3, TXT_CACHE[LTXT_IDX_PLANT_NAME_NONE + _currentPlant], VCOL_LT_GREY);
    // arrows
    cwin_putat_string_raw(&cw, FARM_CTX_MENU_X+2, 3, "\x1f", VCOL_MED_GREY);
    cwin_putat_string_raw(&cw, FARM_CTX_MENU_X+14, 3, "$", VCOL_MED_GREY);
}
static void _showOptionReap(){
    // cwin_clear(&cd);

    _showField();
    _showConfirmBox();

    cwin_putat_string_raw(&cw, FARM_CTX_MENU_X, FARM_CTX_MENU_Y, TXT_CACHE[LTXT_IDX_MENU_FARMLAND_REAP], VCOL_DARK_GREY);
    cwin_write_string_raw(&ht, TXT_CACHE[LTXT_IDX_DESC_FARMLAND_REAP]);
}

static void _showContextMenu(){
    switch(fields[_currentField].stage){
        case PLANT_STAGE_PLOWED:
            _showOptionSow();
            break;
        case PLANT_STAGE_READY:
            _showOptionReap();
            break;
        case PLANT_STAGE_PLOW_TASK_ASSIGNED:
            break;
        default:
            _showOptionPlow();
            break;
    }
}

static const char separator[23] = { 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7f, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x0 };
static void _displayFieldList(){

    cwin_putat_string_raw(&cw, COL_OFFSET_FIELDLIST, ROW_OFFSET_FIELDLIST, TXT_CACHE[LTXT_IDX_FIELD_LIST_HEADER], VCOL_YELLOW);

    char str[10];
    char col;
    for(byte i=0;i<FIELDS_COUNT;i++){
        if(i==_currentField){
            col = VCOL_GREEN;
        } else {
            col = VCOL_DARK_GREY;
        }
        // separators - skip last line
        if(i<FIELDS_COUNT-1){
            cwin_putat_string_raw(&cw, COL_OFFSET_FIELDLIST, ROW_OFFSET_FIELDLIST+3+i*3, separator, VCOL_YELLOW);
        }
        cwin_putat_string_raw(&cw, COL_OFFSET_FIELDLIST+11, ROW_OFFSET_FIELDLIST+1+i*3, TBL_V, VCOL_YELLOW);
        cwin_putat_string_raw(&cw, COL_OFFSET_FIELDLIST+11, ROW_OFFSET_FIELDLIST+2+i*3, TBL_V, VCOL_YELLOW);
        // field size
        cwin_putat_string_raw(&cw, COL_OFFSET_FIELDLIST, ROW_OFFSET_FIELDLIST+1+i*3, TXT_CACHE[LTXT_IDX_FIELD_SMALL + fields[i].area-1], col);
        // plant name
        char plantId = fields[i].plantId;
        cwin_putat_string_raw(&cw, COL_OFFSET_FIELDLIST+1, ROW_OFFSET_FIELDLIST+2+i*3, TXT_CACHE[LTXT_IDX_PLANT_NAME_NONE + plantId], col);

        // stage
        char stage = fields[i].stage;
        cwin_putat_string_raw(&cw, COL_OFFSET_FIELDLIST+13, ROW_OFFSET_FIELDLIST+1+i*3, TXT_CACHE[LTXT_IDX_PLANT_STAGE_NONE + stage], col);

        // state
        switch (fields[i].stage) {
            case PLANT_STAGE_SPROUT:
                sprintf(str, "%4u$%4u", fields[i].planted, fields[i].alive);
                break;
            case PLANT_STAGE_GROWTH:
                sprintf(str, "%4u$%4u", fields[i].alive, fields[i].grown);
                break;
            case PLANT_STAGE_RIPEN:
                unsigned int ready = lmuldiv16u(fields[i].grown, fields[i].ready, 100);
                sprintf(str, "%4u$%4u", fields[i].grown, ready);
                break;
            case PLANT_STAGE_READY:
            case PLANT_STAGE_REAP_TASK_ASSIGNED:
                sprintf(str, "%4u     ", fields[i].grown);
                break;
            case PLANT_STAGE_PLOW_TASK_ASSIGNED:
                sprintf(str, "%4u %%   ", fields[i].ready);
                break;
            default:
                sprintf(str, "         ");
                break;
        }

        cwin_putat_string_raw(&cw, COL_OFFSET_FIELDLIST+13, ROW_OFFSET_FIELDLIST+2+i*3, str, col);
    }
    _showContextMenu();
}

static void _updateFieldView(){
    byte temp = (char) (GS.calendar.currentTemp + 23);
    _drawThermometer(temp);

    // // water debug
    // byteToSprite(GS.farm.waterLevel, SPR_THERMO_BAR_3);
    _showWaterLevel();

    intToWeightToSprite(GS.farm.storage[PLANT_POTATO], SPR_POTATO_UI);
    intToWeightToSprite(GS.farm.storage[PLANT_LUPINE], SPR_LUPINE_UI);
    intToWeightToSprite(GS.farm.storage[PLANT_WHEAT], SPR_WHEAT_UI);
    intToWeightToSprite(GS.farm.storage[PLANT_CORN], SPR_CORN_UI);

    _displayFieldList();
}

// select field - display sub-menu
static void _selectField(){
    switch(fields[_currentField].stage){
        case PLANT_STAGE_PLOWED:
            _sowPlant();
            break;
        case PLANT_STAGE_READY:
            _reapPlant();
            break;
        case PLANT_STAGE_PLOW_TASK_ASSIGNED:
            break;
        default:
            _plowField();
            break;
    }
    _displayFieldList();
}

static void _showFarmMenu(){
    // Prepare output window
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, SCREEN_Y_START, SCREEN_WIDTH, SCREEN_HEIGHT);
    cwin_clear(&cw);
    // secondary window for context menu
    cwin_init(&cd, GFX_1_SCR, COL_OFFSET_MENU, SCREEN_Y_START, SCREEN_WIDTH-COL_OFFSET_MENU, 7);
    // help text window
    cwin_init(&ht, GFX_1_SCR, COL_OFFSET_MENU+1, SCREEN_Y_START+7, SCREEN_WIDTH-COL_OFFSET_MENU-1, SCREEN_HEIGHT-8);
    cwin_fill(&ht, 32, VCOL_DARK_GREY);

    displayMenu(FARMLAND_MENU);
    _updateFieldView();
    // in case we seen the TV screen - turn things back on
    switchScreenTo(SCREEN_SPLIT_MC_TXT);
}

static void _nextField(){
    _confirm = false;
    _currentPlant = PLANT_NONE;
    clearStatusBar();

    if(_currentField < FIELDS_COUNT-1){
        _currentField++;
    } else {
        _currentField = 0;
    }
    _displayFieldList();
}
static void _previousField(){
    _confirm = false;
    _currentPlant = PLANT_NONE;
    clearStatusBar();

    if(_currentField > 0){
        _currentField--;
    } else {
        _currentField = FIELDS_COUNT-1;
    }
    _displayFieldList();
}

static void _nextPlant(){
    // blocked if not in the right context
    if(fields[_currentField].stage == PLANT_STAGE_PLOWED){
        if(_currentPlant < PLANTS_COUNT){
            _currentPlant++;
        } else {
            _currentPlant = 1;
        }
    } else {
        _confirm = true;
    }
    _showContextMenu();
}

static void _previousPlant(){
    // blocked if not in the right context
    if(fields[_currentField].stage == PLANT_STAGE_PLOWED){
        if(_currentPlant > 1){
            _currentPlant--;
        } else {
            _currentPlant = PLANTS_COUNT;
        }
    } else {
        _confirm = false;
    }
    _showContextMenu();
}

const struct MenuOption FARMLAND_MENU[] = {
    // options to select
    { TXT_IDX_MENU_SELECT, KEY_RETURN, SCREEN_SPLIT_MC_TXT, UI_F+UI_HIDE, &_selectField, 0, 23, 4},
    // standard navigation
    { TXT_IDX_MENU_A, 'a', SCREEN_SPLIT_MC_TXT, UI_L+UI_HIDE, &_previousPlant, 0, 1, 2},
    { TXT_IDX_MENU_D, 'd', SCREEN_SPLIT_MC_TXT, UI_R+UI_HIDE, &_nextPlant, 0, 1, 2},
    { TXT_IDX_MENU_W, 'w', SCREEN_SPLIT_MC_TXT, UI_U+UI_HIDE, &_previousField, 0, 7, 2},
    { TXT_IDX_MENU_S, 's', SCREEN_SPLIT_MC_TXT, UI_D+UI_HIDE, &_nextField, 0, 9, 2},
    { TXT_IDX_EXIT_TO_MAP, KEY_ARROW_LEFT, SCREEN_TRANSITION, UI_LF, &showMenu, MENU_BANK_MAP_VILLIAGE_1, 32, 12},
    END_MENU_CHOICES
};

static void _menuHandler(void){
    _currentPlant = PLANT_POTATO;
    mnu_isGfxLoaded = false;
    loadMenuGfx();
    loadMenuSprites();

    // copy text farmland cache from cart to memory
    loadCacheTxt(TXT_FARMLAND_CACHE_INDEX);

    _showFarmMenu();
}

#pragma data ( farmlandLoaderData )

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &menuNoop,
    .loadMenuGfx     = &menuGfxLoaderSingleBitmap,
    // .loadMenuGfx     = &menuGfxLoader,
    .loadMenuSprites = &menuSpriteLoader,
    .showMenu        = &_menuHandler,
    .showSprites     = &_menuShowSprites,
    .updateMenu      = &_updateFieldView,
    .runMenuLoop     = &menuNoop,
};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
