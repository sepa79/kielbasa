#include <c64/vic.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>
#include <c64/easyflash.h>
#include <c64/sprites.h>

#include <menu/menuSystem.h>
#include <menu/villiageMap.h>
#include <translation/common.h>
#include <engine/gameSettings.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/uiHandler.h>
#include <miniGame/villiageMapMain.h>

// Sections and regions in villiageMapMain.h

// ---------------------------------------------------------------------------------------------
// Menu code
// ---------------------------------------------------------------------------------------------
#pragma code ( villiageMapCode )
char vMapX = 10, vMapY = 10;

static void _villiageMapCodeLoader(){
    // source is where the regionVilliageMapRam section starts in real mem
    memcpy(MENU_CODE_DST, (char *)0xb000, 0x1000);
}

// Noop - just return - to satisfy menu handlers
static void _villiageMapNoop(){
    return;
}
__interrupt static void _villiageMapSpriteNoop(){
    return;
}

#pragma code ( villiageMapRAMCode )
#pragma data ( villiageMapRAMData )

static void _mapUp(){
    if (vMapY > 0)
        vMapY--;
    mapGameLoop();
}
static void _mapDown(){
    if (vMapY < V_MAP_SIZE_Y*3-1)
        vMapY++;
    mapGameLoop();
}
static void _mapLeft(){
    if (vMapX > 0)
        vMapX--;
    mapGameLoop();
}
static void _mapRight(){
    if (vMapX < V_MAP_SIZE_X*3-2*4)
        vMapX++;
    mapGameLoop();
}

const struct MenuOption VILLIAGE_MAP_MENU[] = {
    { TXT_IDX_MENU_TASK_MANAGER_W, 'w', SCREEN_HIRES_TXT, UI_U+UI_HIDE, &_mapUp, 0, 1, 1},
    { TXT_IDX_MENU_TASK_MANAGER_S, 's', SCREEN_HIRES_TXT, UI_D+UI_HIDE, &_mapDown, 0, 1, 1},
    { TXT_IDX_MENU_TASK_MANAGER_A, 'a', SCREEN_HIRES_TXT, UI_L+UI_HIDE, &_mapLeft, 0, 1, 1},
    { TXT_IDX_MENU_TASK_MANAGER_D, 'd', SCREEN_HIRES_TXT, UI_R+UI_HIDE, &_mapRight, 0, 1, 1},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_SPLIT_MC_TXT, UI_LF+UI_HIDE, &gotoMainMenu, 0, 2, 5},
    END_MENU_CHOICES
};

static void _villiageMapInit(void){
    villiageMapInit();
    gms_disableTimeControls = true;
    gms_gameSpeed = SPEED_PAUSED;
    displayMenu(VILLIAGE_MAP_MENU);
}

#pragma data ( villiageMapLoaderData )

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &_villiageMapCodeLoader,
    .loadMenuGfx     = nullptr,
    .loadMenuSprites = &_villiageMapNoop,
    .showMenu        = &_villiageMapInit,
    .showSprites     = &_villiageMapSpriteNoop,
    .updateMenu      = &_villiageMapNoop,
};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
