// #include <stdio.h>
#include <c64/vic.h>
#include <c64/types.h>
#include <c64/cia.h>
#include <stdbool.h>
#include <c64/easyflash.h>

#include <menu/menuSystem.h>
#include <menu/mainMenu.h>
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

void prepareScroll(){
    // clear line 25 and its color
    byte i = 0;
    do {
        GFX_1_SCR[40*24+i] = p' ';
        COLOR_RAM[40*24+i] = VCOL_LT_GREY;
        i++;
    } while (i < 40)

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

void mainLoop(){
    // TODO: Switch to CRT here
    initCharacterList();
    initTaskList();
    initFarmland();

    // start on Main Menu
    loadMenu(MENU_BANK_MAIN_MENU);
    gms_textMode = false;
    showMenu();
    joyCursor.enabled = true;
    gms_gameSpeed = SPEED_PAUSED;
    gms_gameSpeedWait = WAIT_TIME_PAUSED;

    drawFullDate();
    prepareScroll();
    updateStatusBar(TXT[SB_IDX_WELCOME]);

    // splash and turn screen on
    splashScreen(false, 2);

    initRasterIRQ();

    bool oncePerFrameFinished = false;
    while(1){
        // vic.color_border = VCOL_YELLOW;
        if(gms_gameSpeedWait == 0){
            // process main game loop

            // reset wait time
            updateGameSpeed();

            changeBank(TICKS_BANK);
            timeTick();
            restoreBank();

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
