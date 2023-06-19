#include <c64/vic.h>
#include <c64/charwin.h>
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

#include <assets/mainGfx.h>
#include <c64/memmap.h>

// Sections and regions in villiageMapMain.h

// ---------------------------------------------------------------------------------------------
// Menu code
// ---------------------------------------------------------------------------------------------
#pragma code ( villiageMapCode )
// initial map position, need to be reset on new game. This is top left corner, not player pos.
char vMapX = 114, vMapY = 129;
char vMapLocation = 0;

static void _villiageMapCodeLoader(){
    // source is where the regionVilliageMapRam section starts in real mem
    memcpy(MENU_CODE_DST, (char *)0xb000, 0x1000);
}

// Noop - just return - to satisfy menu handlers
static void _villiageMapNoop(){
    return;
}

__interrupt void _showVilliageMapSprites(){
    // vic.color_border--;
    vic.spr_expand_x = 0b00000000;
    vic.spr_expand_y = 0b00000000;
    vic.spr_priority = 0b00000000;
    vic.spr_multi    = 0b00000010;
    
    vic.spr_mcolor0  = 12;
    vic.spr_mcolor1  = 6;

    vic.spr_msbx = 0b00000000;

    // set on both screens
    GFX_1_SCR2[OFFSET_SPRITE_PTRS+0] = SPR_BANK_PLAYER_S2;
    GFX_1_SCR2[OFFSET_SPRITE_PTRS+1] = SPR_BANK_PLAYER_S1;
    GFX_1_SCR3[OFFSET_SPRITE_PTRS+0] = SPR_BANK_PLAYER_S2;
    GFX_1_SCR3[OFFSET_SPRITE_PTRS+1] = SPR_BANK_PLAYER_S1;

    // char
    vic.spr_pos[0].x = 12 + 160;
    // char bar
    vic.spr_pos[1].x = 12 + 160;

    vic.spr_pos[0].y = 34 + 100;
    vic.spr_pos[1].y = 34 + 100;

    // char pport = setPort(MMAP_NO_ROM);
    vic.spr_color[0] = 0;
    vic.spr_color[1] = 7;
    // setPort(pport);

    vic.spr_enable = 0b00000011;
    // vic.color_border++;
}

#pragma code ( villiageMapRAMCode )
#pragma data ( villiageMapRAMData )

#define WALKABLE 0b00010000
static void _mapUp(){
    // check if tiles above us are walkable
    char pport = setPort(MMAP_RAM);
    char mapCharAttr1 = colorMap[CHAR_ATTRIBS][mapScreen[40*10+19]];
    char mapCharAttr2 = colorMap[CHAR_ATTRIBS][mapScreen[40*10+20]];
    bool canWalk = mapCharAttr1 & mapCharAttr2 & WALKABLE;
    setPort(pport);
    
    if(canWalk){
        if (vMapY > 0)
            vMapY--;
        villiageMapDraw(WALK_UP);
    }
}
static void _mapDown(){
    // check if tile below us is walkable
    char pport = setPort(MMAP_RAM);
    char mapCharAttr1 = colorMap[CHAR_ATTRIBS][mapScreen[40*13+19]];
    char mapCharAttr2 = colorMap[CHAR_ATTRIBS][mapScreen[40*13+20]];
    bool canWalk = mapCharAttr1 & mapCharAttr2 & WALKABLE;
    setPort(pport);

    if(canWalk){
        if (vMapY < V_MAP_SIZE_Y*3-1)
            vMapY++;
        villiageMapDraw(WALK_DOWN);
    }
}
static void _mapLeft(){
    // check if tile to the left is walkable
    char pport = setPort(MMAP_RAM);
    char mapCharAttr1 = colorMap[CHAR_ATTRIBS][mapScreen[40*11+18]];
    char mapCharAttr2 = colorMap[CHAR_ATTRIBS][mapScreen[40*12+18]];
    bool canWalk = mapCharAttr1 & mapCharAttr2 & WALKABLE;
    setPort(pport);

    if(canWalk){
        if (vMapX > 0)
            vMapX--;
        villiageMapDraw(WALK_LEFT);
    }
}
static void _mapRight(){
    // check if tile to the right is walkable
    char pport = setPort(MMAP_RAM);
    char mapCharAttr1 = colorMap[CHAR_ATTRIBS][mapScreen[40*11+21]];
    char mapCharAttr2 = colorMap[CHAR_ATTRIBS][mapScreen[40*12+21]];
    bool canWalk = mapCharAttr1 & mapCharAttr2 & WALKABLE;
    setPort(pport);

    if(canWalk){
        if (vMapX < V_MAP_SIZE_X*3-2*4)
            vMapX++;
        villiageMapDraw(WALK_RIGHT);
    }
}
// static void _mapFire(){
//     isMapDay = !isMapDay;
//     villiageMapDraw(WALK_RIGHT);
// }

const struct MenuOption VILLIAGE_MAP_MENU[] = {
    { TXT_IDX_MENU_TASK_MANAGER_W, 'w', SCREEN_HIRES_TXT, UI_U+UI_HIDE, &_mapUp, 0, 1, 1},
    { TXT_IDX_MENU_TASK_MANAGER_S, 's', SCREEN_HIRES_TXT, UI_D+UI_HIDE, &_mapDown, 0, 1, 1},
    { TXT_IDX_MENU_TASK_MANAGER_A, 'a', SCREEN_HIRES_TXT, UI_L+UI_HIDE, &_mapLeft, 0, 1, 1},
    { TXT_IDX_MENU_TASK_MANAGER_D, 'd', SCREEN_HIRES_TXT, UI_R+UI_HIDE, &_mapRight, 0, 1, 1},
    // { TXT_IDX_MENU_TASK_MANAGER_D, KEY_RETURN, SCREEN_HIRES_TXT, UI_F+UI_HIDE, &_mapFire, 0, 1, 1},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_HIRES_TXT, UI_F+UI_HIDE, &gotoLocation, 0, 2, 5},
    END_MENU_CHOICES
};

static void _villiageMapInit(void){
    gms_disableTimeControls = true;
    gms_gameSpeed = SPEED_PAUSED;
    villiageMapInit();
    displayMenu(VILLIAGE_MAP_MENU);
}

#pragma data ( villiageMapLoaderData )

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &_villiageMapCodeLoader,
    .loadMenuGfx     = nullptr,
    .loadMenuSprites = &_villiageMapNoop,
    .showMenu        = &_villiageMapInit,
    .showSprites     = &_showVilliageMapSprites,
    .updateMenu      = &_villiageMapNoop,
    .runMenuLoop     = &_villiageMapNoop,
};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
