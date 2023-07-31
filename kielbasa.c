// #include <stdio.h>
#include <c64/vic.h>
#include <c64/types.h>
#include <c64/cia.h>
#include <stdbool.h>
#include <string.h>
#include <c64/rasterirq.h>
#include <fixmath.h>

#include <engine/logger.h>
#include <engine/gameState.h>

#include <common.h>
#include <menu/menuSystem.h>
#include <menu/mainMenu.h>
#include <menu/optionsMenu.h>
#include <menu/villiageMap.h>
#include <menu/hermansHouse.h>
#include <menu/sulejukowasHouse.h>
#include <menu/fishing.h>
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

// loads a TVScreen to display some news, keeps track of the current menu so it can be restored
static char prvMenu = 0;
void showTvScreen() {
    prvMenu = mnu_menuBank;
    switchScreenTo(SCREEN_TRANSITION);
    loadMenu(MENU_BANK_TV_SCREEN);
    showMenu();
}

void revertPreviousMenu() {
    switchScreenTo(SCREEN_TRANSITION);
    gms_disableTimeControls = false; // do it before loading menu, in case it needs to disable it again
    loadMenu(prvMenu);
    showMenu();
}

static void _showNormalMenu(char menuBank){
    switchScreenTo(SCREEN_TRANSITION);
    loadMenu(menuBank);
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

static void _showMiniGame(char menuBank){
    switchScreenTo(SCREEN_TRANSITION);
    gms_disableTimeControls = true;
    gms_gameSpeed = SPEED_PAUSED;
    updateGameSpeed();
    loadMenu(menuBank);
    showMenu();
}

void gotoLocation(){
    switch(GS.vMap.location){
        case LOCATION_FARM_HOUSE:
            _showNormalMenu(MENU_BANK_MAIN_MENU);
            break;
        case LOCATION_FARM_BARN:
            _showNormalMenu(MENU_BANK_FARMLAND);
            break;
        case LOCATION_FARM_PIGPEN:
            _showNormalMenu(MENU_BANK_PIG_PEN);
            break;
        case LOCATION_SHOP:
            _showNormalMenu(MENU_BANK_SHOP);
            break;
        case LOCATION_HERMAN:
            _showNormalMenu(MENU_BANK_HERMANS_HOUSE);
            break;
        case LOCATION_SULEJUKOWA:
            _showNormalMenu(MENU_BANK_SULEJUKOWAS_HOUSE);
            break;
        case LOCATION_FISHING:
            _showMiniGame(MENU_BANK_FISHING_MENU);
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
    gms_inSpecialMenu = false;
    updateGameSpeed();

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

            // out-of-map events
            switch(gms_event){
                case EVENT_END_OF_MONTH:
                    showTvScreen();
                    break;
            }
            gms_event = EVENT_NONE;
            
        }
        // do this once per frame only
        if(gms_framePos == FRAME_BOTTOM && !oncePerFrameFinished){
            // keep that in RAM as it changes banks a lot to display menus and their code
            checkKeys();
            oncePerFrameFinished = true;
            // process special tasks, like minigame loops
            if(!gms_inSpecialMenu){
                runMenuLoop();
            }
            // map events
            switch(gms_event){
                case EVENT_RESPAWN_AT_HOME:
                    splashScreen(false, 2);
                    GS.vMap.x = MAP_HOME_X;
                    GS.vMap.y = MAP_HOME_Y;
                    GS.vMap.location = LOCATION_FARM_HOUSE;
                    allCharacters[0].energy = 50;
                    GS.cash = lmuldiv16u(GS.cash, 90, 100);
                    updateMoney();
                    // TODO: some nice picture would be good here, and time shift
                    gotoLocation();
                    break;
            }
            gms_event = EVENT_NONE;
        }
        
        // reset the flags once the 'middle' part has been drawn
        if(gms_framePos == FRAME_TOP_BORDER){
            oncePerFrameFinished = false;
        }
    }
}
