#include <c64/vic.h>
#include <c64/easyflash.h>
#include <c64/sprites.h>
#include <string.h>
#include <c64/memmap.h>
#include <c64/keyboard.h>

#include <engine/gameState.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/gameSettings.h>
#include <assets/mainGfx.h>
#include <miniGame/villiageMapMain.h>
#include <miniGame/villiageMapDay.h>
#include <miniGame/villiageMapNight.h>
#include <menu/menuSystem.h>
#include <tick/farmlandTick.h>

// ---------------------------------------------------------------------------------------------
// Main screen and sprite bank + loaders code
// ---------------------------------------------------------------------------------------------

#pragma data ( villiageMapGfx1 )
// L0 is moonlight, +0 chars, dark grey/all black
// L1 is darker, +0 chars, grey-ish
// L2 is dark, +1 chars, dimmer colors
// L3 is neutral, +2 chars, normal light
// L4 is bright, +3 normal char, brighter colors

// just colors used
static const char _charAttribsL1[] = {
    #embed ctm_attr1    "assets/charGfx/HiresVilliage_L1.ctm"
};
static const char _charAttribsL2[] = {
    #embed ctm_attr1    "assets/charGfx/HiresVilliage_L2.ctm"
};
static const char _charAttribsL3[] = {
    #embed ctm_attr1    "assets/charGfx/HiresVilliage_L3.ctm"
};
static const char _charAttribsL4[] = {
    #embed ctm_attr1    "assets/charGfx/HiresVilliage_L4.ctm"
};
// main character set
static const char _chars[] = {
    #embed ctm_chars    "assets/charGfx/HiresVilliage_L3.ctm"
};

static const char _mapSprites[] = {
    #embed 0xffff 20 "assets/sprites/Player2.spd"
};


#pragma data ( villiageMapData )

static const char _map[] = {
    // #embed ctm_map8     "assets/charGfx/VilliageMap.ctm"
    #embed "assets/charGfx/VilliageMap_Map.bin"
};

const char romTiles[] = {
    #embed ctm_tiles8   "assets/charGfx/VilliageMap.ctm"
};

#pragma data ( villiageMapData2 )

const char1024 allLightMaps[5] = { { 
    #embed ctm_map8     "assets/charGfx/lightMap_up.ctm"
},{
    #embed ctm_map8     "assets/charGfx/lightMap_down.ctm"
},{
    #embed ctm_map8     "assets/charGfx/lightMap_left.ctm"
},{
    #embed ctm_map8     "assets/charGfx/lightMap_right.ctm"
},{ // TODO: wasted space, remove and change display code
    #embed ctm_map8     "assets/charGfx/lightMap_right.ctm"
}};

static const char _mapLocations[] = {
    #embed "assets/charGfx/VilliageMap.ctm.ids.bin"
};

// ---------------------------------------------------------------------------------------------
// Loaders code, called with IRQ off
// ---------------------------------------------------------------------------------------------
#pragma code ( villiageMapGfx1Loaders )
#pragma data ( villiageMapRAMData )

static char _previousDir = WALK_UP;

// 4 colorsets for 4 light levels
char256 * const colorMap = (char256 *)0xcc00;
// lightmap for night
char * lightMap = (char *)0xc800;

// Copy chars and lightmaps, any sprites etc
static void _mapInit(){
    memcpy(colorMap[0], _charAttribsL1, 0x100);
    memcpy(colorMap[1], _charAttribsL2, 0x100);
    memcpy(colorMap[2], _charAttribsL3, 0x100);
    memcpy(colorMap[3], _charAttribsL4, 0x100);

    memset(COLOR_RAM, VCOL_BLACK, 1000);
    memset(GFX_1_SCR, VCOL_BLACK, 1000);
    memcpy((char *)0xc780, _mapSprites, 0x80);

    //set UI sprite
    setCharacterSlotIcon(0, SPR_MAP_UI_PLR);

}

// ---------------------------------------------------------------------------------------------
// Display code
// ---------------------------------------------------------------------------------------------
#pragma code ( villiageMapDisplayCode )
#pragma data ( villiageMapRAMData )

// ---------------------------------------------------------------------------------------------
// Game code
// ---------------------------------------------------------------------------------------------
#pragma code ( villiageMapCode )
#pragma data ( villiageMapRAMData )

char * ramTiles = (char *)0xc480;

#define GROUND_CHAR 0xbc
#define CROPS_CHAR 0xa0

#define FIELD_START ramTiles

static unsigned int fLayout[96*4] = {0xffff};

// copy base map from ROM, add any specials to it
static void buildRamTiles(void){
    // memset(ramTiles, GROUND_CHAR, 16*(RAM_TILES_COUNT+1));
    
    // for showing crops die in nice ways
    // Field Y
    char * fieldPointer = FIELD_START;
    for(char fi=0; fi<FIELDS_COUNT; fi++){
        // how many crops to draw
        unsigned int plantedCount = fields[fi].planted;
        unsigned int aliveCount = fields[fi].alive;
        // prepare field dithering
        unsigned int fSize = fields[fi].width * fields[fi].height * 16;
        for(unsigned int i=0; i<fSize; i++){
            if(i<plantedCount){
                fLayout[i] = i;
            } else {
                fLayout[i] = 0xffff;
            }
        }
        // seed the generator to repeat the same patterns
        srand(fields[fi].rseed);
        for(unsigned int i=0; i<plantedCount; i++){
            unsigned int rnd = lmul16u(plantedCount, rand()) >> 16;
            unsigned int tmp = fLayout[rnd];
            fLayout[rnd] = fLayout[i];
            fLayout[i] = tmp;
        }
        // plant & stage to draw, plantId 0 is nothing, so -1 there
        char plant = CROPS_CHAR + 4*(fields[fi].plantId-1);
        // stages start at 4 for planting, but above 7 its just tasks, so stop there
        if(fields[fi].stage < PLANT_STAGE_READY){
            plant += PLANT_STAGE_READY-fields[fi].stage;
        } else {
            // stage 5 has special needs - we have leave the used squares in their random positions,
            // but sort them so that reaping process is in sensible order
            unsigned int newFieldOrder = aliveCount;
            for(unsigned int i=0; i<plantedCount; i++){
                if(fLayout[i] < aliveCount){
                    fLayout[i] = newFieldOrder;
                    newFieldOrder--;
                }
            }
            aliveCount = aliveCount - fields[fi].reaped;
        }
        // reset fLayout index
        unsigned int i = 0;
        for(char fy=0; fy<fields[fi].height; fy++){
            // Tile Y
            for(char ty=0; ty<4; ty++){
                // Field X
                for(char fx=0; fx<fields[fi].width; fx++){
                    // set pointer to current X tile, Y row
                    char * field = fieldPointer + fx*16 + fy*fields[fi].width*16;
                    // Tile X
                    for(char tx=0; tx<4; tx++){
                        if(fLayout[i] < aliveCount){
                            field[tx+4*ty] = plant;
                        } else {
                            field[tx+4*ty] = GROUND_CHAR;
                        } 
                        i++;
                    }
                }
            }
        }
        fieldPointer += fields[fi].width*fields[fi].height*16;
    }
}

static void _drawColors(){
    // color top of the screen, hoping to be just behind the raster
    // vic.color_border++;

    #pragma unroll(full)
    for(int i=0; i<480; i++)
        COLOR_RAM[i] = GFX_1_SCR[i];
    
    // now wait until we move to top of the screen again
    // vic.color_border--;
    if(gms_framePos == FRAME_TOP){
        while(gms_framePos == FRAME_TOP){};
    }
    // now wait until we move to selected section of the screen
    while(gms_framePos != FRAME_TOP){};
    // vic.color_border--;
    // color bottom of the screen
    #pragma unroll(full)
    for(int i=480; i<960; i++)
        COLOR_RAM[i] = GFX_1_SCR[i];
    // vic.color_border++;
}

// draw player, signal IRQ to switch screen at next frame
static void _drawPlayerAndColors(){
    // wait for raster
    if(gms_framePos == FRAME_MIDDLE){
        while(gms_framePos == FRAME_MIDDLE){};
    }
    // now wait until we move to selected section of the screen
    while(gms_framePos != FRAME_MIDDLE){};
    // change buffer - it will be visible at next frame
    if(map_2ndScreen){
        map_2ndScreen = false;
    } else {
        map_2ndScreen = true;
    }
    _drawColors();
}

// ---------------------------------------------------------------------------------------------
// RAM code
// ---------------------------------------------------------------------------------------------
// this code needs to be in main block, as it switches banks
#pragma code ( villiageMapRAMCode )
#pragma data ( villiageMapRAMData )

// dark moon
// char moonLightColor = VCOL_BLACK;
char moonDetailLevel = 0;
static volatile char timer = 0;
volatile char moveCostBase = 0;
volatile char moveCostTime = 0;
volatile char moveCostEnergy = 0;

void villiageMapScreenInit(void){

    // copy fonts
    char pbank = setBank(MENU_BANK_MAP_VILLIAGE_1);
    char pport = mmap_set(MMAP_ROM);
    
    // rom to buffer -> GFX_1_SCR
    memcpy(GFX_1_SCR, _chars, 0x400);
    // switch IO off
    mmap_set(MMAP_RAM);
    // buffer to RAM under IO
    memcpy(GFX_1_FNT2, GFX_1_SCR, 0x400);

    // repeat to copy 2nd half
    // rom to buffer -> GFX_1_SCR
    mmap_set(MMAP_ROM);
    memcpy(GFX_1_SCR, _chars+0x400, 0x400);
    // switch IO off
    mmap_set(MMAP_RAM);
    // buffer to RAM under IO
    memcpy(GFX_1_FNT2+0x400, GFX_1_SCR, 0x400);

    mmap_set(pport);
    setBank(pbank);

    // saving mem - call ROM code
    _mapInit();
    
    // reset timer
    timer = MAP_TICK_DELAY;
    moveCostBase = 0;
}

static void _drawPlayer(){
    char pbank = setBank(MENU_BANK_MAP_VILLIAGE_1);
    _drawPlayerAndColors();
    setBank(pbank);
}

void villiageMapDraw(){
    // performance metrics
    char frameStart = gms_frameCount;

    // draw map
    char pbank = setBank(MENU_BANK_MAP_VILLIAGE_2);
    if(GS.calendar.isDay){
        villiageMapDrawDay(_map, GS.vMap.x, GS.vMap.y);
    } else {
        if(_previousDir != GS.vMap.direction){
            memset(GFX_1_SCR, VCOL_BLACK, 960);
            char pbank = setBank(MENU_BANK_MAP_VILLIAGE_3);
            memcpy(lightMap, allLightMaps[GS.vMap.direction], 960);
            setBank(pbank);
        }
        _previousDir = GS.vMap.direction;
        villiageMapDrawNight(_map, GS.vMap.x, GS.vMap.y);
    }
    
    setBank(pbank);

    // draw player
    _drawPlayer();

    // text window
    pbank = setBank(MENU_BANK_MAP_VILLIAGE_3);
    char vx = (GS.vMap.x+19) >> 2;
    char vy = (GS.vMap.y+12) >> 2;
    char locId = _mapLocations[vx + vy*64];
    setBank(pbank);
    if(GS.vMap.location != locId){
        GS.vMap.location = locId;
        // char str[12*3+1];
        // char framesUsed = gms_frameCount - frameStart;
        // sprintf(str, "%03d", framesUsed);
        // textToSprite(str, 4);
        textToSprite((char *)TXT_VILLAGE_MAP[LTXT_IDX_NOTHING + locId], 4, SPR_TXT_BOTTOM_1);

        // sprawdz w tabeli lokacji, czy jest callback. jak jest to go wolaj.
        // callback sam sie wylaczy, jak bedzie 'zaliczony' czyli np. pies dostal mieso, strefa nieaktywna.
        // calendar.c bedzie codziennie resetowal strefy czy co tam bedzie innego miala strefa w CalendarTick.
    }

    joyCursor.moveDelayCurrent = 0;
}

static void _mapTimeTick(){
    // check if move happened and we need to adjust the timer
    // char str[12*3+1];
    // sprintf(str, "%03d %03d", moveCostBase, timer);
    // textToSprite(str, 4, SPR_TXT_BOTTOM_1);
    if(moveCostTime){
        if(timer > moveCostTime){
            timer -= moveCostTime;
            moveCostTime = 0;
        } else {
            moveCostTime -= timer;
            timer = 1;
        }
    }
    if(!--timer){
        timer = MAP_TICK_DELAY;
        if(GS.calendar.dateMinute < 59){
            GS.calendar.dateMinute++;
        } else {
            GS.calendar.dateMinute = 0;
            char pbank = setBank(TICKS_BANK);
            hourTick();
            regenTickMapHour();
            setBank(pbank);
        }
        char pbank = setBank(TICKS_BANK);
        regenTickMinute();
        setBank(pbank);
        updateMinute(GS.calendar.dateMinute);
    }
    
}

static void _movePlayer(){
    switch(GS.vMap.direction){
        case WALK_UP:
            GS.vMap.y--;
            break;
        case WALK_DOWN:
            GS.vMap.y++;
            break;
        case WALK_LEFT:
            GS.vMap.x--;
            break;
        case WALK_RIGHT:
            GS.vMap.x++;
            break;
    }
}

void villiageMapGameLoop(){
    // vic.color_border--;
    _mapTimeTick();
    // decide if we can move
    if(GS.vMap.direction != WALK_NONE){
        moveCostTime = moveCostBase * 10;
        moveCostEnergy = moveCostBase;
        if(checkEnergyLevel(0, moveCostBase)){
            decEnergyLevel(0, moveCostBase);
            drawBattery(0);

            // char str[12*3+1];
            // sprintf(str, "%03d %03d", moveCostTime, moveCostEnergy);
            // textToSprite(str, 4, SPR_TXT_BOTTOM_1);

            _movePlayer();
            villiageMapDraw();
        } else if(!GS.regenMin){
            // out of energy, out of regen - respawn
            gms_event = EVENT_RESPAWN_AT_HOME;
        }
        GS.vMap.direction = WALK_NONE;
    }
    // vic.color_border++;
}
