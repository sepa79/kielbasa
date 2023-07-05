#include <c64/charwin.h>
#include <c64/vic.h>
#include <c64/types.h>
#include <c64/memmap.h>

#include <menu/menuSystem.h>
#include <tasks/taskManager.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/uiHandler.h>
#include "farmland.h"
#include "pigPen.h"
#include "kitchen.h"
#include "shopOutside.h"
#include "shopInside.h"
#include "crew.h"
#include "pigsleCommand.h"
#include "villiageMap.h"
#include "tvScreen.h"

// column offset for printing data
#define COL_OFFSET_TASKLIST 12

#define SPR_BUS_BANK 0xbc
#define SPR_BUS ((char *)GFX_1_BASE + 64*SPR_BUS_BANK)

#define BUS_ANIM_FRAMES 4
#define BUS_ANIM_DELAY 2
#define BUS_WAITING_DELAY 80
#define BUS_ANIM_WHEEL_1 7
#define BUS_ANIM_WHEEL_2 40
#define BUS_ANIM_Y 88

enum ANIM_BUS_STAGES {
    ANIM_BUS_ARRIVING1,
    ANIM_BUS_ARRIVING2,
    ANIM_BUS_ARRIVING3,
    ANIM_BUS_DRAWN,
    ANIM_BUS_WAITING,
    ANIM_BUS_LEAVING,
    ANIM_BUS_ERASER1,
    ANIM_BUS_ERASER2,
    ANIM_BUS_ERASER3,
    ANIM_BUS_FINISHED,
    ANIM_BUS_WAIT_FOR_TRIGGER,
};

static volatile byte animBusStage = ANIM_BUS_WAIT_FOR_TRIGGER;
static volatile byte animBusFrame = 0;
static volatile byte animBusDelay = BUS_ANIM_DELAY;
static volatile byte animBusX = 0;

// Sections and regions
#pragma section( mainMenuLoaderData, 0 )
#pragma section( mainMenuLoaderData2, 0 )
#pragma section( mainMenuCode, 0 )
#pragma section( mainMenuGfxDay, 0 )
#pragma section( mainMenuGfxNight, 0 )
#pragma section( mainMenuSprites, 0 )
#pragma region( bankMainMenuC, 0x8000, 0x9000, , MENU_BANK_MAIN_MENU, { mainMenuLoaderData, mainMenuCode, mainMenuLoaderData2 } )
#pragma region( bankMainMenuG1, DAY_GFX_BMP, DAY_GFX_BMP+0x1400, , MENU_BANK_MAIN_MENU, { mainMenuGfxDay } )
#pragma region( bankMainMenuG2, NIGHT_GFX_BMP, NIGHT_GFX_BMP+0x1400, , MENU_BANK_MAIN_MENU, { mainMenuGfxNight } )
#pragma region( bankMainMenuG3, MENU_SPRITE_SRC, MENU_SPRITE_SRC+0x0400, , MENU_BANK_MAIN_MENU, { mainMenuSprites } )

#pragma data ( mainMenuGfxDay )
__export const byte mainMenuGfx1[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/main_14.03.23.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/main_14.03.23.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/main_14.03.23.kla"
};
#pragma data ( mainMenuGfxNight )
__export const byte mainMenuGfx2[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/main_16.10.22_Night.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/main_16.10.22_Night.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/main_16.10.22_Night.kla"
};

#pragma data ( mainMenuSprites )
__export const char mainMenuGfx3[] = {
    #embed 0x0400 20 "assets/sprites/bus.spd"
};

// animation data table
#pragma data ( mainMenuLoaderData2 )
__export const int BUS_ANIM_X_0[] = {
    // stage 1 - arriving
    294,292,290,288,286,284,282,280,278,276,274,272,0,
    270,268,266,264,262,260,258,256,254,252,250,248,0,
    246,244,242,240,238,236,234,232,230,228,226,224,0,
    // stage 2 - arrived
    222,220,219,218,217,216,215,214,213,212,211,210,209,208,207,206,205,204,203,202,201,200,199,0,
    // stage 3 - leaving
    199,198,197,196,195,194,193,192,191,190,189,188,187,186,185,184,183,182,181,180,179,178,177,176,175,174,173,172,171,170,169,168,167,166,165,164,163,162,161,160,159,158,157,156,155,154,153,152,151,150,149,148,147,146,145,144,142,140,138,0,
    // stage 4 - eraser
    136,134,132,130,128,126,124,122,120,118,116,114,0,
    112,110,108,106,104,102,100,98,96,94,92,90,0,
    88,86,84,82,80,78,76,74,72,70,68,66,0
};
__export const byte BUS_ANIM_MIN_X_PAINT[3] = {270+24, 270, 270-24};
__export const byte BUS_ANIM_MIN_X_ERASE[3] = {136, 112, 88};
__export const byte BUS_ANIM_OFFSET[4] = {0, 64, 128, 192};

__export const byte BIT_MASK_PAINT[9] = {
    0b11000000,
    0b11110000,
    0b11111100,
    0b11111111
};
__export const byte BIT_MASK_ERASE[9] = {
    0b00111111,
    0b00001111,
    0b00000011,
    0b00000000
};

// menu code is in ROM - data in RAM
#pragma code ( mainMenuCode )
#pragma data ( data )

__interrupt static void _menuShowSprites(){
    if(animBusStage == ANIM_BUS_FINISHED){
        return;
    }
    // vic.color_border++;
    vic.spr_enable   = 0b11111111;
    vic.spr_expand_x = 0b00000000;
    vic.spr_expand_y = 0b00000000;
    vic.spr_priority = 0b00000000;
    vic.spr_mcolor0  = VCOL_LT_BLUE;
    vic.spr_mcolor1  = VCOL_MED_GREY;
    vic.spr_multi    = 0b11100000;
    vic.spr_msbx     = 0b00000000;

    // wheels
    vic_sprxy(0, BUS_ANIM_X_0[animBusX]+BUS_ANIM_WHEEL_1, BUS_ANIM_Y);
    vic_sprxy(1, BUS_ANIM_X_0[animBusX]+BUS_ANIM_WHEEL_2, BUS_ANIM_Y);
    // overlay and multi
    vic_sprxy(2, BUS_ANIM_X_0[animBusX], BUS_ANIM_Y);
    vic_sprxy(5, BUS_ANIM_X_0[animBusX], BUS_ANIM_Y);
    vic_sprxy(3, BUS_ANIM_X_0[animBusX]+24, BUS_ANIM_Y);
    vic_sprxy(6, BUS_ANIM_X_0[animBusX]+24, BUS_ANIM_Y);
    vic_sprxy(4, BUS_ANIM_X_0[animBusX]+48, BUS_ANIM_Y);
    vic_sprxy(7, BUS_ANIM_X_0[animBusX]+48, BUS_ANIM_Y);

    vic.spr_color[0] = VCOL_DARK_GREY;
    vic.spr_color[1] = VCOL_DARK_GREY;
    vic.spr_color[2] = VCOL_BLACK;
    vic.spr_color[3] = VCOL_BLACK;
    vic.spr_color[4] = VCOL_BLACK;
    vic.spr_color[5] = VCOL_WHITE;
    vic.spr_color[6] = VCOL_WHITE;
    vic.spr_color[7] = VCOL_WHITE;

    GFX_1_SCR[OFFSET_SPRITE_PTRS+0] = SPR_BUS_BANK+6+animBusFrame;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+1] = SPR_BUS_BANK+10+animBusFrame;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+2] = SPR_BUS_BANK+3;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+3] = SPR_BUS_BANK+4;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+4] = SPR_BUS_BANK+5;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+5] = SPR_BUS_BANK+0;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+6] = SPR_BUS_BANK+1;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+7] = SPR_BUS_BANK+2;
    // vic.color_border--;

    animBusDelay--;
    if(animBusDelay == 0){
        animBusDelay = BUS_ANIM_DELAY;
        
        animBusFrame++;
        if(animBusFrame == BUS_ANIM_FRAMES)
            animBusFrame = 0;

        animBusX++;
        if(BUS_ANIM_X_0[animBusX] == 0){
            // trigger next position
            animBusX++;
            animBusStage++;
            if(animBusStage == ANIM_BUS_WAITING){
                animBusDelay = BUS_WAITING_DELAY;
                animBusStage++;
            }
            if(animBusStage == ANIM_BUS_FINISHED)
                return;
        }

        if(animBusStage < ANIM_BUS_DRAWN){
            // wait until raster is below bus
            vic_waitLine(BUS_ANIM_Y);
            byte animBusPainterStage = animBusStage - ANIM_BUS_ARRIVING1;

            byte maskPos = BUS_ANIM_MIN_X_PAINT[animBusPainterStage] - BUS_ANIM_X_0[animBusX];
            byte mask = BIT_MASK_PAINT[(maskPos & 0b00000111)/2];
            char i = BUS_ANIM_OFFSET[animBusPainterStage] + (maskPos >> 3);

            bool drawWheel1 = false;
            byte wheelOffset = 0;
            if((animBusPainterStage == 0) && i > 0){
                drawWheel1 = true;
                wheelOffset = 1;
            }
            if((animBusPainterStage == 1) && i == 64){
                drawWheel1 = true;
                wheelOffset = 62;
            }
            bool drawWheel2 = false;
            if((animBusPainterStage == 1) && i == 66){
                drawWheel2 = true;
                wheelOffset = 66;
            }
            if((animBusPainterStage == 2) && i < 130){
                drawWheel2 = true;
                wheelOffset = 128+2;
            }
            byte busWithoutWheels = BUS_ANIM_OFFSET[animBusPainterStage+1];
            // bus - all the same for all frames
            do{
                ((char *)MENU_SPRITE_DST)[i] = ((char *)MENU_SPRITE_SRC)[i] & mask;
                ((char *)MENU_SPRITE_DST)[64*3+i] = ((char *)MENU_SPRITE_SRC)[64*3+i] & mask;
                if(drawWheel1){
                    byte j = i - wheelOffset;
                    byte wheel = ((char *)MENU_SPRITE_SRC)[64*6+j] & mask;
                    ((char *)MENU_SPRITE_DST)[64*6+j] = wheel;
                    ((char *)MENU_SPRITE_DST)[64*7+j] = wheel;
                    ((char *)MENU_SPRITE_DST)[64*8+j] = wheel;
                    ((char *)MENU_SPRITE_DST)[64*9+j] = wheel;
                }
                if(drawWheel2){
                    byte j = i - wheelOffset;
                    byte wheel = ((char *)MENU_SPRITE_SRC)[64*10+j] & mask;
                    ((char *)MENU_SPRITE_DST)[64*10+j] = wheel;
                    ((char *)MENU_SPRITE_DST)[64*11+j] = wheel;
                    ((char *)MENU_SPRITE_DST)[64*12+j] = wheel;
                    ((char *)MENU_SPRITE_DST)[64*13+j] = wheel;
                }
                i++;
                i++;
                i++;
            } while (i<busWithoutWheels-4*3);
            // wheels - different in each frame
            do{
                ((char *)MENU_SPRITE_DST)[i] = ((char *)MENU_SPRITE_SRC)[i] & mask;
                ((char *)MENU_SPRITE_DST)[64*3+i] = ((char *)MENU_SPRITE_SRC)[64*3+i] & mask;
                if(drawWheel1){
                    byte j = i - wheelOffset;
                    byte wheel = ((char *)MENU_SPRITE_SRC)[64*6+j] & mask;
                    ((char *)MENU_SPRITE_DST)[64*6+j] = wheel;

                    wheel = ((char *)MENU_SPRITE_SRC)[64*7+j] & mask;
                    ((char *)MENU_SPRITE_DST)[64*7+j] = wheel;

                    wheel = ((char *)MENU_SPRITE_SRC)[64*8+j] & mask;
                    ((char *)MENU_SPRITE_DST)[64*8+j] = wheel;

                    wheel = ((char *)MENU_SPRITE_SRC)[64*9+j] & mask;
                    ((char *)MENU_SPRITE_DST)[64*9+j] = wheel;
                }
                if(drawWheel2){
                    byte j = i - wheelOffset;
                    byte wheel = ((char *)MENU_SPRITE_SRC)[64*10+j] & mask;
                    ((char *)MENU_SPRITE_DST)[64*10+j] = wheel;

                    wheel = ((char *)MENU_SPRITE_SRC)[64*11+j] & mask;
                    ((char *)MENU_SPRITE_DST)[64*11+j] = wheel;

                    wheel = ((char *)MENU_SPRITE_SRC)[64*12+j] & mask;
                    ((char *)MENU_SPRITE_DST)[64*12+j] = wheel;

                    wheel = ((char *)MENU_SPRITE_SRC)[64*13+j] & mask;
                    ((char *)MENU_SPRITE_DST)[64*13+j] = wheel;
                }
                i++;
                i++;
                i++;
            } while (i<busWithoutWheels);
        }

        if(animBusStage > ANIM_BUS_LEAVING){
            // wait until raster is below bus
            vic_waitLine(BUS_ANIM_Y+0);
            byte animBusEraserStage = animBusStage - ANIM_BUS_ERASER1;

            byte maskPos = BUS_ANIM_MIN_X_ERASE[animBusEraserStage] - BUS_ANIM_X_0[animBusX];
            byte mask = BIT_MASK_ERASE[(maskPos & 0b00000111)/2];
            char i = BUS_ANIM_OFFSET[animBusEraserStage] + (maskPos >> 3);

            bool eraseWheel1 = false;
            byte wheelOffset = 0;
            if((animBusEraserStage == 0) && i > 0){
                eraseWheel1 = true;
                wheelOffset = 1;
            }
            if((animBusEraserStage == 1) && i == 64){
                eraseWheel1 = true;
                wheelOffset = 62;
            }
            bool eraseWheel2 = false;
            if((animBusEraserStage == 1) && i == 66){
                eraseWheel2 = true;
                wheelOffset = 66;
            }
            if((animBusEraserStage == 2) && i < 130){
                eraseWheel2 = true;
                wheelOffset = 128+2;
            }

            do{
                ((char *)MENU_SPRITE_DST)[i] = ((char *)MENU_SPRITE_SRC)[i] & mask;
                ((char *)MENU_SPRITE_DST)[64*3+i] = ((char *)MENU_SPRITE_SRC)[64*3+i] & mask;
                if(eraseWheel1){
                    byte j = i - wheelOffset;
                    byte wheel = ((char *)MENU_SPRITE_SRC)[64*6+j] & mask;
                    ((char *)MENU_SPRITE_DST)[64*6+j] = wheel;
                    ((char *)MENU_SPRITE_DST)[64*7+j] = wheel;
                    ((char *)MENU_SPRITE_DST)[64*8+j] = wheel;
                    ((char *)MENU_SPRITE_DST)[64*9+j] = wheel;
                }
                if(eraseWheel2){
                    byte j = i - wheelOffset;
                    byte wheel = ((char *)MENU_SPRITE_SRC)[64*10+j] & mask;
                    ((char *)MENU_SPRITE_DST)[64*10+j] = wheel;
                    ((char *)MENU_SPRITE_DST)[64*11+j] = wheel;
                    ((char *)MENU_SPRITE_DST)[64*12+j] = wheel;
                    ((char *)MENU_SPRITE_DST)[64*13+j] = wheel;
                }
                i++;
                i++;
                i++;
            } while (i<BUS_ANIM_OFFSET[animBusEraserStage+1]);
        }
    }
    // vic.color_border++;
}

const struct MenuOption MAIN_MENU[] = {
    {TXT_IDX_MENU_MAIN1, '1', SCREEN_TRANSITION, UI_SELECT, &showMenu, MENU_BANK_KITCHEN, 1, 1},
    {TXT_IDX_MENU_MAIN2, '2', SCREEN_TRANSITION, UI_SELECT, &showMenu, MENU_BANK_CREW, 1, 2},
    {TXT_IDX_MENU_MAIN3, '3', SCREEN_TRANSITION, UI_SELECT, &showTvScreen, 0, 1, 3},
    {TXT_IDX_MENU_MAIN4, '4', SCREEN_TRANSITION, UI_SELECT, &showMenu, MENU_BANK_PIGSLE_COMMAND_1, 1, 4},
    {TXT_IDX_EXIT_TO_MAP, KEY_ARROW_LEFT, SCREEN_TRANSITION, UI_LF+UI_SELECT, &showMenu, MENU_BANK_MAP_VILLIAGE_1, 1, 6},
    END_MENU_CHOICES
};

static void _displayTaskList(){
    // header
    cwin_putat_string_raw(&cw, COL_OFFSET_TASKLIST, 1, TXT[TXT_IDX_TASK_LIST_HEADER], VCOL_YELLOW);

    // tasks list
    for(byte i=0;i<TASK_ARRAY_SIZE;i++){
        byte taskId = taskRef[i];
        cwin_putat_string_raw(&cw, COL_OFFSET_TASKLIST, 2+i, TXT[task_nameIdx[taskId]], VCOL_GREEN);
        cwin_putat_string_raw(&cw, COL_OFFSET_TASKLIST+8, 2+i, TBL_V, VCOL_YELLOW);
        cwin_putat_string_raw(&cw, COL_OFFSET_TASKLIST+9, 2+i, task_desc[taskId], VCOL_GREEN);
    }
}

static void _menuHandler(){
    animBusFrame = 0;
    animBusDelay = BUS_ANIM_DELAY;

    loadMenuGfx();
    loadMenuSprites();

    // zero fill sprites
    char i = 0;
    do {
#assign y 0
#repeat
       ((volatile char*) MENU_SPRITE_DST)[y + i] = 0;
#assign y y + 0x100
#until y == 0x0400
        i++;
    } while (i != 0);

    // Prepare output window
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, SCREEN_Y_START, SCREEN_WIDTH, SCREEN_HEIGHT);
    cwin_clear(&cw);
    
    animBusStage = ANIM_BUS_ARRIVING1;
    animBusX = 0;
    displayMenu(MAIN_MENU);
    switchScreenTo(SCREEN_SPLIT_MC_TXT);
    _displayTaskList();
}

#pragma data ( mainMenuLoaderData )

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &menuNoop,
    .loadMenuGfx     = &menuGfxLoader,
    .loadMenuSprites = &menuSpriteLoader,
    .showMenu        = &_menuHandler,
    .showSprites     = &_menuShowSprites,
    .updateMenu      = &_displayTaskList,
    .runMenuLoop     = &menuNoop,
};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
