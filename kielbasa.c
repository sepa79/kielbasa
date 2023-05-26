// #include <stdio.h>
#include <c64/vic.h>
#include <c64/types.h>
#include <c64/cia.h>
#include <stdbool.h>
#include <c64/easyflash.h>
#include <string.h>
#include <engine/logger.h>

#include <menu/menuSystem.h>
#include <menu/mainMenu.h>
#include <menu/villiageMap.h>
#include <engine/easyFlashBanks.h>
#include <engine/gameSettings.h>
#include <engine/irqSpriteController.h>
#include <engine/irqHandler.h>
#include <engine/uiHandler.h>
#include <assets/assetsSettings.h>
#include <translation/common.h>
#include <translation/textsPL.h>
#include <tasks/taskManager.h>
#include <character/character.h>
#include <tick/calendar.h>
#include <tick/farmlandTick.h>
#include <tick/kitchenTick.h>

void prepareScroll(){
    // clear line 25 and its color
    byte i = 0;
    do {
        GFX_1_SCR[40*24+i] = p' ';
        COLOR_RAM[40*24+i] = VCOL_LT_GREY;
        i++;
    } while (i < 40);

    COLOR_RAM[40*24+0] = VCOL_DARK_GREY;
    COLOR_RAM[40*24+1] = VCOL_DARK_GREY;
    COLOR_RAM[40*24+2] = VCOL_DARK_GREY;
    COLOR_RAM[40*24+3] = VCOL_MED_GREY;
    COLOR_RAM[40*24+4] = VCOL_MED_GREY;

    COLOR_RAM[40*24+35] = VCOL_MED_GREY;
    COLOR_RAM[40*24+36] = VCOL_MED_GREY;
    COLOR_RAM[40*24+37] = VCOL_DARK_GREY;
    COLOR_RAM[40*24+38] = VCOL_DARK_GREY;
    COLOR_RAM[40*24+39] = VCOL_DARK_GREY;
}

static void _showNormalMenu(){
    // clean sprites
    memset(SPR_CHARACTER_PORTRAIT2, 0, 64*4);
    switchScreenTo(SCREEN_SPLIT_MC_TXT);
    showMenu();
    gms_disableTimeControls = false;
    joyCursor.enabled = true;
}
void gotoLocation(){
    switch(vMapLocation){
        case LOCATION_FARM_HOUSE:
            loadMenu(MENU_BANK_MAIN_MENU);
            _showNormalMenu();
            break;
        case LOCATION_FARM_BARN:
            loadMenu(MENU_BANK_FARMLAND);
            _showNormalMenu();
            break;
        case LOCATION_FARM_PIGPEN:
            loadMenu(MENU_BANK_PIG_PEN);
            _showNormalMenu();
            break;
        case LOCATION_SHOP:
            loadMenu(MENU_BANK_SHOP);
            _showNormalMenu();
            break;
    }
    // reset location so when we are back to map it gets re-set and sprites are drawn
    vMapLocation = LOCATION_RESET;
}

void mainLoop(){
    memcpy(LOG_DATA, p"Game Start", 10);
    logger(LOG_INFO | LOG_MSG_TEXT);

    initCharacterList();
    initCalendar();
    initTaskList();
    initFarmland();
    initKitchen();

    // splash and turn screen off
    splashScreen(false, 1);

    // TODO: CLEANUP, initRasterIRQ does all of it already, called at the end. Most likely this should be simplified
    // stop IRQs and change to ours
    __asm {
        sei
    }
    // msx off
    ((byte *)0xd418)[0] &= ~0xf;
    // screen off, sprites off
    // vic.ctrl1 = VIC_CTRL1_BMM | VIC_CTRL1_RSEL | 3;
    vic.spr_enable   = 0b00000000;

    // get the main sprites, fonts etc
    loadMainFont();
    drawFullDate();

    loadMenu(MENU_BANK_MAIN_MENU);
    // indicate frame position, as IRQs are stopped
    gms_framePos = FRAME_UNKNOWN;
    showMenu();
    joyCursor.enabled = true;
    gms_gameSpeed = SPEED_PAUSED;
    gms_gameSpeedWait = WAIT_TIME_PAUSED;
    gms_disableTimeControls = false;

    prepareScroll();
    updateStatusBar(TXT[SB_IDX_WELCOME]);

    // splash and turn screen on
    splashScreen(false, 2);

    initRasterIRQ();

    memcpy(LOG_DATA, p"Main Loop ", 10);
    logger(LOG_DEBUG | LOG_MSG_TEXT);

    bool oncePerFrameFinished = false;
    while(1){
        // vic.color_border = VCOL_YELLOW;
        if(gms_gameSpeedWait == 0){
            // process main game loop

            // reset wait time
            updateGameSpeed();

            char pbank = setBank(TICKS_BANK);
            timeTick();
            setBank(pbank);

        }
        // do this once per frame only - carefull with FRAME_MIDDLE as its not fired in gms_textMode
        if(gms_framePos == FRAME_BOTTOM && !oncePerFrameFinished){
            // keep that in RAM as it changes banks a lot to display menus and their code
            checkKeys();
            oncePerFrameFinished = true;
        }
        // reset the tasks once the 'middle' part has been drawn
        if(gms_framePos == FRAME_TOP_BORDER){
            oncePerFrameFinished = false;
        }
        // process special tasks
        
    }
}
