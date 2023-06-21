// #include <stdio.h>
#include <c64/vic.h>
#include <c64/types.h>
#include <c64/cia.h>
#include <stdbool.h>
#include <string.h>
#include <c64/rasterirq.h>

#include <engine/logger.h>
#include <engine/gameState.h>

#include <menu/menuSystem.h>
#include <menu/mainMenu.h>
#include <menu/optionsMenu.h>
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

static void _showNormalMenu(){
    switchScreenTo(SCREEN_TRANSITION);
    // clean sprites
    memset(SPR_CHARACTER_PORTRAIT2, 0, 64*4);
    // show menu is sensitive to timing, as it load GFXs and needs to wait for middle screen
    // trying to play music before it shows can lead to issues, something to solve a bit better one day
    showMenu();
    playSong(MAIN_MENU_SONG);
    switchScreenTo(SCREEN_SPLIT_MC_TXT);
    gms_disableTimeControls = false;
    joyCursor.enabled = true;
}
void gotoLocation(){
    switch(GS.vMap.location){
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
    GS.vMap.location = LOCATION_RESET;
}

void mainLoop(){
    memcpy(LOG_DATA, p"Game Start", 10);
    logger(LOG_INFO | LOG_MSG_TEXT);

    // active RAM code at 0x7000 is gameInitRAMCode - set by titleScreen.h
    // all the below modules must be in that section
    initGame();

    // now we are back to main code block

    // splash and turn screen off
    splashScreen(false, 1);

    // stop IRQs and change to ours
    rirq_stop();
    // msx off
    ((byte *)0xd418)[0] &= ~0xf;
    vic.spr_enable = 0b00000000;
    playSong(MAIN_MENU_SONG);

    // get the main sprites, fonts etc
    loadMainFont();
    initUI();

    // this wipes out 0x7000 and puts new code in there
    // TODO: change menu bank here to whatever comes back from save game
    loadMenu(MENU_BANK_MAIN_MENU);
    
    // indicate frame position, as IRQs are stopped
    gms_framePos = FRAME_UNKNOWN;
    showMenu();
    joyCursor.enabled = true;
    gms_gameSpeed = SPEED_PAUSED;
    gms_gameSpeedWait = WAIT_TIME_PAUSED;
    gms_disableTimeControls = false;
    updateGameSpeed();

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
            // process special tasks
            runMenuLoop();
        }
        // reset the flags once the 'middle' part has been drawn
        if(gms_framePos == FRAME_TOP_BORDER){
            oncePerFrameFinished = false;
        }
    }
}
