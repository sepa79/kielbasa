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

#define MOON_PHASE_FULL 1
#define MOON_PHASE_NONE 3

static void _villiageMapCodeLoader(){
    // source is where the regionVilliageMapRam section starts in real mem
    memcpy(MENU_CODE_DST, (char *)0xb000, 0x1000);
    villiageMapInit();
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
    
    vic.spr_mcolor0  = 9;
    vic.spr_mcolor1  = 7;

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

    // char pport = mmap_set(MMAP_NO_ROM);
    vic.spr_color[0] = 0;
    vic.spr_color[1] = 6;
    // mmap_set(pport);

    vic.spr_enable = 0b00000011;
    // vic.color_border++;
}

#pragma code ( villiageMapRAMCode )
#pragma data ( villiageMapRAMData )

const char MOVE_COST[] = {
    0,
    1,
    2,
    3,
    5,
    10
};

#define WALKABLE 0b11110000

static char _getMaterialCost(char scrChar){
    char mapCharAttr1 = colorMap[CHAR_ATTRIBS][scrChar] >> 4;
    // byteToSprite(scrChar, SPR_TXT_UP_1);
    return MOVE_COST[mapCharAttr1];
}

static void _mapUp(){
    // check if tiles above us are walkable
    char pport = mmap_set(MMAP_RAM);
    char mapChar1 = mapScreen[40*11+19];
    char mapCharAttr1 = colorMap[CHAR_ATTRIBS][mapChar1];
    bool canWalk1 = mapCharAttr1 & WALKABLE;
    char mapChar2 = mapScreen[40*11+20];
    char mapCharAttr2 = colorMap[CHAR_ATTRIBS][mapChar2];
    bool canWalk = canWalk1 && (mapCharAttr2 & WALKABLE);
    mmap_set(pport);
    
    if(canWalk){
        if (GS.vMap.y > 0){
            // GS.vMap.y--;
            GS.vMap.direction = WALK_UP;
            // get material cost of both chars as we are 2x wide on up/down move
            char lmc = _getMaterialCost(mapChar1);
            char rmc = _getMaterialCost(mapChar2);
            // use the higher cost
            // decrease timer - each move takes time
            moveCostBase = lmc > rmc ? lmc : rmc;
        }
    }
}
static void _mapDown(){
    // check if tile below us is walkable
    char pport = mmap_set(MMAP_RAM);
    char mapChar1 = mapScreen[40*13+19];
    char mapCharAttr1 = colorMap[CHAR_ATTRIBS][mapChar1];
    bool canWalk1 = mapCharAttr1 & WALKABLE;
    char mapChar2 = mapScreen[40*13+20];
    char mapCharAttr2 = colorMap[CHAR_ATTRIBS][mapChar2];
    bool canWalk = canWalk1 && (mapCharAttr2 & WALKABLE);
    mmap_set(pport);

    if(canWalk){
        if (GS.vMap.y < V_MAP_SIZE_Y*4-6*4){
            // GS.vMap.y++;
            GS.vMap.direction = WALK_DOWN;
            // byteToSprite(GS.vMap.y, SPR_TXT_BOTTOM_1);
            // get material cost of both chars as we are 2x wide on up/down move
            char lmc = _getMaterialCost(mapChar1);
            char rmc = _getMaterialCost(mapChar2);
            // use the higher cost
            // decrease timer - each move takes time
            moveCostBase = lmc > rmc ? lmc : rmc;
        }
    }
}
static void _mapLeft(){
    // check if tile to the left is walkable
    char pport = mmap_set(MMAP_RAM);
    char mapChar1 = mapScreen[40*12+18];
    char mapCharAttr1 = colorMap[CHAR_ATTRIBS][mapChar1];
    bool canWalk = mapCharAttr1 & WALKABLE;
    mmap_set(pport);

    if(canWalk){
        if (GS.vMap.x > 0){
            // GS.vMap.x--;
            GS.vMap.direction = WALK_LEFT;
            // decrease timer - each move takes time
            moveCostBase = _getMaterialCost(mapChar1);
        }
    }
}
static void _mapRight(){
    // check if tile to the right is walkable
    char pport = mmap_set(MMAP_RAM);
    char mapChar1 = mapScreen[40*12+21];
    char mapCharAttr1 = colorMap[CHAR_ATTRIBS][mapChar1];
    bool canWalk = mapCharAttr1 & WALKABLE;
    mmap_set(pport);

    if(canWalk){
        if (GS.vMap.x < V_MAP_SIZE_X*4-10*4){
            // GS.vMap.x++;
            GS.vMap.direction = WALK_RIGHT;
            // decrease timer - each move takes time
            moveCostBase = _getMaterialCost(mapChar1);
        }
    }
}
// static void _mapFire(){
//     isMapDay = !isMapDay;
//     villiageMapDraw(WALK_RIGHT);
// }

const struct MenuOption VILLIAGE_MAP_MENU[] = {
    { TXT_IDX_MENU_W, 'w', SCREEN_HIRES_TXT, UI_U+UI_HIDE, &_mapUp, 0, 1, 1},
    { TXT_IDX_MENU_S, 's', SCREEN_HIRES_TXT, UI_D+UI_HIDE, &_mapDown, 0, 1, 1},
    { TXT_IDX_MENU_A, 'a', SCREEN_HIRES_TXT, UI_L+UI_HIDE, &_mapLeft, 0, 1, 1},
    { TXT_IDX_MENU_D, 'd', SCREEN_HIRES_TXT, UI_R+UI_HIDE, &_mapRight, 0, 1, 1},
    // { TXT_IDX_MENU_TASK_MANAGER_D, KEY_RETURN, SCREEN_HIRES_TXT, UI_F+UI_HIDE, &_mapFire, 0, 1, 1},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_HIRES_TXT, UI_F+UI_HIDE, &gotoLocation, 0, 2, 5},
    END_MENU_CHOICES
};

static void _villiageMapLoadGfx(){
    if(!GS.calendar.isDay){
        playSong(VILLIAGE_MAP_SONG_NIGHT);
        // fill screen with moonlight
        switch(GS.calendar.moonPhase){
            case MOON_PHASE_FULL:
                moonDetailLevel = 1;
                break;
            case MOON_PHASE_NONE:
                moonDetailLevel = 3;
                break;
            default:
                moonDetailLevel = 2;
        }
        // copy lightmap and init screen colors
        // this is 2nd buffer's Color mem
        memset(GFX_1_SCR, VCOL_BLACK, 960);
        char pbank = setBank(MENU_BANK_MAP_VILLIAGE_3);
        memcpy(lightMap, allLightMaps[GS.vMap.direction], 960);
        setBank(pbank);
    } else {
        playSong(VILLIAGE_MAP_SONG_DAY);
    }

    // clean last row in case we came back from options menu
    memset(COLOR_RAM+960, VCOL_BLACK, 40);
    // force map redraw
    villiageMapDraw();
}

static void _villiageMapShowMenu(void){

    // copy text village map cache from cart to memory
    loadCacheTxt(TXT_VILLAGE_MAP_CACHE_INDEX);

    _villiageMapLoadGfx();
    // make screen visible
    switchScreenTo(SCREEN_HIRES_TXT);
    displayMenu(VILLIAGE_MAP_MENU);
}

// load and init routines, from MENU_BANK_MAP_VILLIAGE_1
// called from _villiageMapCodeLoader on menu load only
void villiageMapInit(void){
    // SCREEN_TRANSITION mode is on, so screen is black
    gms_disableTimeControls = true;
    gms_gameSpeed = SPEED_PAUSED;

    // Load GFX
    villiageMapScreenInit();
    buildRamTiles();
}

#pragma data ( villiageMapLoaderData )

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &_villiageMapCodeLoader,
    .loadMenuGfx     = &_villiageMapLoadGfx,
    .loadMenuSprites = &_villiageMapNoop,
    .showMenu        = &_villiageMapShowMenu,
    .showSprites     = &_showVilliageMapSprites,
    .updateMenu      = &_villiageMapNoop,
    .runMenuLoop     = &villiageMapGameLoop, // in villiageMapMain
};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
