#include <c64/charwin.h>
#include <c64/keyboard.h>
#include <c64/vic.h>
#include <c64/types.h>
#include <stdio.h>

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
#define COL_OFFSET_FIELDLIST 0

static byte _currentField = 0;
static byte _currentPlant = 0;

enum FARMLAND_SPRITE_VIC_BANKS {
    SPR_BANK_THERMO_BAR_1=0xbc,
    SPR_BANK_THERMO_BAR_2,
    SPR_BANK_THERMO_BAR_3,
    SPR_BANK_POTATO_UI,
    SPR_BANK_LUPINE_UI,
    SPR_BANK_WHEAT_UI,
    SPR_BANK_CORN_UI
};

#define SPR_THERMO_BAR_1 ((char *)GFX_1_BASE + 64*SPR_BANK_THERMO_BAR_1)
#define SPR_THERMO_BAR_2 ((char *)GFX_1_BASE + 64*SPR_BANK_THERMO_BAR_2)
#define SPR_THERMO_BAR_3 ((char *)GFX_1_BASE + 64*SPR_BANK_THERMO_BAR_3)
#define SPR_POTATO_UI    ((char *)GFX_1_BASE + 64*SPR_BANK_POTATO_UI)
#define SPR_LUPINE_UI    ((char *)GFX_1_BASE + 64*SPR_BANK_LUPINE_UI)
#define SPR_WHEAT_UI     ((char *)GFX_1_BASE + 64*SPR_BANK_WHEAT_UI)
#define SPR_CORN_UI      ((char *)GFX_1_BASE + 64*SPR_BANK_CORN_UI)

// Sections and regions
#pragma section( farmlandLoaderData, 0 )
#pragma section( farmlandCode, 0 )
#pragma section( farmlandGfxDay, 0 )
#pragma section( farmlandGfxNight, 0 )
#pragma section( farmlandSprites, 0 )
#pragma region( bankFarmlandC, 0x8000, 0x9000, , MENU_BANK_FARMLAND, { farmlandLoaderData, farmlandCode } )
#pragma region( bankFarmlandG1, DAY_GFX_BMP, DAY_GFX_BMP+0x1400, , MENU_BANK_FARMLAND, { farmlandGfxDay } )
#pragma region( bankFarmlandG2, NIGHT_GFX_BMP, NIGHT_GFX_BMP+0x1400, , MENU_BANK_FARMLAND, { farmlandGfxNight } )
#pragma region( bankFarmlandG3, MENU_SPRITE_SRC, MENU_SPRITE_SRC+0x0400, , MENU_BANK_FARMLAND, { farmlandSprites } )

#pragma data ( farmlandGfxDay )
__export const byte farmlandGfx1[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/pole_18.02.22.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/pole_18.02.22.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/pole_18.02.22.kla"
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

// menu code is in ROM - data in RAM
#pragma code ( farmlandCode )
#pragma data ( data )

static void _updateSprite(unsigned int num) {
    byte num2str[4];
    if(num > 999) {
        num = 999;
    }
    utoa(num, num2str, 10);
    copyCharToSprite(num2str[0], 0, 0);
    copyCharToSprite(num2str[1], 1, 0);
    copyCharToSprite(num2str[2], 2, 0);
}

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

__interrupt static void _menuShowSprites(){
    vic.spr_enable   = 0b01111111;
    vic.spr_expand_x = 0b00000000;
    vic.spr_expand_y = 0b00000000;
    vic.spr_priority = 0b00000000;
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
    vic.spr_pos[3].y = 50+67;
    vic.spr_pos[4].y = 50+67;
    vic.spr_pos[5].y = 50+67;
    vic.spr_pos[6].y = 50+67;

    vic.spr_color[0] = VCOL_LT_BLUE;
    vic.spr_color[1] = VCOL_LT_BLUE;
    vic.spr_color[2] = VCOL_LT_BLUE;
    vic.spr_color[3] = VCOL_MED_GREY;
    vic.spr_color[4] = VCOL_MED_GREY;
    vic.spr_color[5] = VCOL_MED_GREY;
    vic.spr_color[6] = VCOL_MED_GREY;

    if(field_plantId[_currentField]){
        vic.spr_color[2+field_plantId[_currentField]] = VCOL_WHITE;
    }
    if(_currentPlant){
        vic.spr_color[2+_currentPlant] = VCOL_LT_GREY;
    }

    GFX_1_SCR[OFFSET_SPRITE_PTRS+0] = SPR_BANK_THERMO_BAR_1;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+1] = SPR_BANK_THERMO_BAR_2;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+2] = SPR_BANK_THERMO_BAR_3;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+3] = SPR_BANK_POTATO_UI;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+4] = SPR_BANK_LUPINE_UI;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+5] = SPR_BANK_WHEAT_UI;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+6] = SPR_BANK_CORN_UI;
}

static void _displayFieldList(){

    // clearBox(COL_OFFSET_FIELDLIST, 20, 40-COL_OFFSET_FIELDLIST, 5);

    // gotoxy(COL_OFFSET_FIELDLIST, 19);
    // printf("Water 0b-2u Temp 0b-2d Week 0b-2u Rain 0b-1u", flt_waterLevel, cal_currentTemp, cal_dateWeek, WEEKLY_AVG_RAIN[cal_dateWeek]);

    cwin_putat_string_raw(&cw, COL_OFFSET_FIELDLIST, 7, TXT[TXT_IDX_FIELD_LIST_HEADER], VCOL_YELLOW);

    byte str[4];
    byte col;
    for(byte i=0;i<=FIELDS_COUNT;i++){
        sprintf(str, "%u", field_area[i]);
        if(i==_currentField){
            col = VCOL_LT_GREEN;
        } else {
            col = VCOL_GREEN;
        }
        cwin_putat_string(&cw, COL_OFFSET_FIELDLIST, 8+i, str, col);

        cwin_putat_string_raw(&cw, COL_OFFSET_FIELDLIST+2, 8+i, TBL_V, VCOL_YELLOW);

        byte plantId = field_plantId[i];
        cwin_putat_string_raw(&cw, COL_OFFSET_FIELDLIST+3, 8+i, PLANT_TYPES_TXT[plantId], col);

        cwin_putat_string_raw(&cw, COL_OFFSET_FIELDLIST+12, 8+i, TBL_V, VCOL_YELLOW);
        byte stage = field_stage[i];
        cwin_putat_string_raw(&cw, COL_OFFSET_FIELDLIST+13, 8+i, PLANT_STAGE_NAMES[stage], col);

        cwin_putat_string_raw(&cw, COL_OFFSET_FIELDLIST+21, 8+i, TBL_V, VCOL_YELLOW);
        sprintf(str, "%3u", field_timer[i]);
        cwin_putat_string(&cw, COL_OFFSET_FIELDLIST+22, 8+i, str, col);

        cwin_putat_string_raw(&cw, COL_OFFSET_FIELDLIST+25, 8+i, TBL_V, VCOL_YELLOW);
        sprintf(str, "%3u", field_stage_planted[i]);
        cwin_putat_string(&cw, COL_OFFSET_FIELDLIST+26, 8+i, str, col);

        cwin_putat_string_raw(&cw, COL_OFFSET_FIELDLIST+29, 8+i, TBL_V, VCOL_YELLOW);
        sprintf(str, "%3u", field_stage_grown[i]);
        cwin_putat_string(&cw, COL_OFFSET_FIELDLIST+30, 8+i, str, col);

        cwin_putat_string_raw(&cw, COL_OFFSET_FIELDLIST+33, 8+i, TBL_V, VCOL_YELLOW);
        sprintf(str, "%3u", field_stage_ready[i]);
        cwin_putat_string(&cw, COL_OFFSET_FIELDLIST+34, 8+i, str, col);
    }
}

static void _updateFieldView(){
    byte temp = (char) (cal_currentTemp + 23);
    _drawThermometer(temp);
    sprBankPointer = SPR_POTATO_UI;
    _updateSprite(flt_storagePotato);
    sprBankPointer = SPR_LUPINE_UI;
    _updateSprite(flt_storageLupine);
    sprBankPointer = SPR_WHEAT_UI;
    _updateSprite(flt_storageWheat);
    sprBankPointer = SPR_CORN_UI;
    _updateSprite(flt_storageCorn);

    _displayFieldList();
}

static void _showMenuDetails(){
    cwin_putat_string_raw(&cw, 0, 2, TXT[TXT_IDX_FARM_FIELD_TXT], VCOL_GREEN);
    byte str[2];
    sprintf(str, "%u", _currentField+1);
    cwin_putat_string(&cw, 8, 2, str, VCOL_GREEN);
    cwin_putat_string_raw(&cw, 12, 2, TXT[TXT_IDX_FARM_PLANT_TXT], VCOL_GREEN);
    cwin_putat_string_raw(&cw, 12+11, 2, PLANT_TYPES_TXT[_currentPlant], VCOL_GREEN);
}

static void _nextField(){
    if(_currentField < FIELDS_COUNT){
        _currentField++;
    } else {
        _currentField = 0;
    }
    _showMenuDetails();
    _displayFieldList();
}
static void _previousField(){
    if(_currentField > 0){
        _currentField--;
    } else {
        _currentField = FIELDS_COUNT;
    }
    _showMenuDetails();
    _displayFieldList();
}

static void _reorganise(){
    updateStatusBar(p"   reorganise");
}
// Set the plant task
static void _sowPlant(){
    if(_currentPlant == 0){
        setErrorCursor();
        return;
    }
    // don't allow to ruin growth in progress
    if(field_stage[_currentField] != PLANT_STAGE_NONE){
        setErrorCursor();
        return;
    }
    // indicate task is assigned
    field_plantId[_currentField] = _currentPlant;
    field_stage[_currentField]   = PLANT_STAGE_SOW_TASK_ASSIGNED;
    field_timer[_currentField]   = 0;
    field_stage_planted[_currentField] = 0; // in percent
    field_stage_grown[_currentField]   = 0;
    field_stage_ready[_currentField]   = 0;
    _displayFieldList();

    // create Task
    // TODO: Make it global
    struct Task task;
    byte idx = plant_taskDscIdx[_currentPlant];
    // "Field 2, Potatoes"
    sprintf(task.desc, "%s %u, %s", TXT[TXT_IDX_TASK_DSC_FARMLAND_FIELD], _currentField+1, TXT[idx]);
    task.codeRef = &sowFieldTask;
    task.nameIdx = TXT_IDX_TASK_FARMLAND_FARM;
    task.params[0] = _currentPlant;
    task.params[1] = _currentField;
    task.params[2] = 0;
    task.params[3] = 0;
    task.params[4] = 0;
    task.reqType = SKILL_FARMING;
    task.icon = SPR_TASK_FARM1;
    task.status = TASK_STATUS_NEW;
    addTask(&task);
}

static void _maintainPlant(){
    updateStatusBar(s"   maintainPlant");
}

static void _reapPlant(){
    field_plantId[_currentField] = 0;
    field_stage[_currentField] = PLANT_STAGE_NONE;
    _displayFieldList();
}

static void _nextPlant(){
    if(_currentPlant < PLANTS_COUNT){
        _currentPlant++;
    } else {
        _currentPlant = 1;
    }
    _showMenuDetails();
}

static void _previousPlant(){
    if(_currentPlant > 1){
        _currentPlant--;
    } else {
        _currentPlant = PLANTS_COUNT;
    }
    _showMenuDetails();
}

const struct MenuOption FARMLAND_MENU[] = {
    { TXT_IDX_MENU_FARMLAND1, ',', UI_U, &_previousField, 0, 7, 2},
    { TXT_IDX_MENU_FARMLAND2, '.', UI_D, &_nextField, 0, 9, 2},
    { TXT_IDX_MENU_FARMLAND3, ':', UI_LR, &_previousPlant, 0, 12+10, 2},
    { TXT_IDX_MENU_FARMLAND4, ';', UI_LR, &_nextPlant, 0, 12+10+11, 2},
    { TXT_IDX_MENU_FARMLAND5, '1', UI_LR, &_sowPlant, 0, 1, 3},
    { TXT_IDX_MENU_FARMLAND6, '2', UI_LR, &_maintainPlant, 0, 11, 3},
    { TXT_IDX_MENU_FARMLAND7, '3', UI_LR, &_reapPlant, 0, 21, 3},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, UI_LR, &showMenu, MENU_BANK_MAIN_MENU, 31, 3},
    END_MENU_CHOICES
};

static void _menuHandler(void){
    _currentPlant = field_plantId[_currentField];
    loadMenuGfx(cal_isDay);
    loadMenuSprites();

    // Prepare output window
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, SCREEN_Y_START, SCREEN_WIDTH, SCREEN_HEIGHT);
    cwin_clear(&cw);
    
    displayMenu(FARMLAND_MENU);
    _showMenuDetails();
    _updateFieldView();
}

#pragma data ( farmlandLoaderData )

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &menuNoop,
    .loadMenuGfx     = &menuGfxLoader,
    .loadMenuSprites = &menuSpriteLoader,
    .showMenu        = &_menuHandler,
    .showSprites     = &_menuShowSprites,
    .updateMenu      = &_updateFieldView,
};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
