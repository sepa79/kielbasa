#include <c64/vic.h>
#include <c64/easyflash.h>
#include <c64/sprites.h>
#include <string.h>
#include <c64/memmap.h>
#include <c64/keyboard.h>

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
static const char _chars[] = {
    #embed ctm_chars    "assets/charGfx/HiresVilliage_L3.ctm"
};

#pragma data ( villiageMapData )

static const char _map[] = {
    // #embed ctm_map8     "assets/charGfx/VilliageMapHiresMain16xWood.ctm"
    #embed "assets/charGfx/VilliageMapHiresMain16xWood_-_8bpc_64x64_Map_-_grass_16_pion_poziom_slash_backslash.bin"
};

const char romTiles[] = {
    #embed ctm_tiles8   "assets/charGfx/VilliageMapHiresMain16xWood.ctm"
};

typedef char char1024[1024];

const char1024 _lightMap[4] = { { 
    #embed ctm_map8     "assets/charGfx/lightMap_up.ctm"
},{
    #embed ctm_map8     "assets/charGfx/lightMap_down.ctm"
},{
    #embed ctm_map8     "assets/charGfx/lightMap_left.ctm"
},{
    #embed ctm_map8     "assets/charGfx/lightMap_right.ctm"
}};

#pragma data ( villiageMapData2 )

static const char _mapLocations[] = {
    #embed "assets/charGfx/VilliageMapHiresMain16xWood.ctm.ids.bin"
};
static const char _halfMoonColorMap[] = {
    #embed ctm_map8 "assets/charGfx/HalfMoonColorMap.ctm"
};

// ---------------------------------------------------------------------------------------------
// Loaders code, called with IRQ off
// ---------------------------------------------------------------------------------------------
#pragma code ( villiageMapGfx1Loaders )
#pragma data ( villiageMapRAMData )

static char _previousDir = WALK_UP;

// 4 colorsets for 4 light levels
char256 * const colorMap = (char256 *)0xcc00;

// Copy chars and lightmaps, any sprites etc
static void _mapInit(){
    // ROM on, I/O off - as we will copy to RAM under I/O ports - IRQs must be off
    // mmap_set(0b00110011);
    // memcpy(GFX_1_FNT2, _chars, sizeof(_chars));
    // // turn ROMS and I/O back on, so that we don't get a problem when bank tries to be switched but I/O is not visible
    // mmap_set(MMAP_ROM);
    memcpy(colorMap[0], _charAttribsL1, 0x100);
    memcpy(colorMap[1], _charAttribsL2, 0x100);
    memcpy(colorMap[2], _charAttribsL3, 0x100);
    memcpy(colorMap[3], _charAttribsL4, 0x100);
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

#define GROUND_CHAR 0xbc
#define CROPS_CHAR 0x20
// 1st tile is empty
#define FIELD_START ramTiles+0x10

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
        // stages start at 2 for planting, but above 5 its just tasks, so stop there
        if(fields[fi].stage < 5){
            plant += 5-fields[fi].stage;
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
        int i = 0;
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
        GFX_1_SCR2[40*11+19] = PLAYER_CHAR_0;
        GFX_1_SCR2[40*11+20] = PLAYER_CHAR_1;
        GFX_1_SCR2[40*12+19] = PLAYER_CHAR_2;
        GFX_1_SCR2[40*12+20] = PLAYER_CHAR_3;
        map_2ndScreen = false;
    } else {
        GFX_1_SCR3[40*11+19] = PLAYER_CHAR_0;
        GFX_1_SCR3[40*11+20] = PLAYER_CHAR_1;
        GFX_1_SCR3[40*12+19] = PLAYER_CHAR_2;
        GFX_1_SCR3[40*12+20] = PLAYER_CHAR_3;
        map_2ndScreen = true;
    }
    _drawColors();
    // player color
    COLOR_RAM[40*11+19] = colorMap[2][PLAYER_CHAR_0];
    COLOR_RAM[40*11+20] = colorMap[2][PLAYER_CHAR_1];
    COLOR_RAM[40*12+19] = colorMap[2][PLAYER_CHAR_2];
    COLOR_RAM[40*12+20] = colorMap[2][PLAYER_CHAR_3];
}

// ---------------------------------------------------------------------------------------------
// RAM code
// ---------------------------------------------------------------------------------------------
// this code needs to be in main block, as it switches banks
#pragma code ( villiageMapRAMCode )
#pragma data ( villiageMapRAMData )

// dark moon
char moonLight = VCOL_BLACK;
char moonLightLevel = 0;
bool isMapDay = true;
#define MOON_PHASE_FULL 1
#define MOON_PHASE_NONE 3

void villiageMapScreenInit(void){
    char pbank = setBank(MENU_BANK_MAP_VILLIAGE_1);
    // set the pointer for chars to copy and indicate it needs to be done, SCREEN_TRANSITION IRQ handler will pick it up
    fontCopySrc = _chars;
    fontCopyDone = false;

    _mapInit();
    isMapDay = cal_isDay;
    setBank(pbank);
    if(!isMapDay){
        // fill screen with moonlight
        switch(cal_moonPhase){
            case MOON_PHASE_FULL:
                moonLight = VCOL_DARK_GREY;
                moonLightLevel = 1;
                memset(COLOR_RAM, moonLight, 960);
                memset(GFX_1_SCR, moonLight, 960);
                break;
            case MOON_PHASE_NONE:
                moonLight = VCOL_BLACK;
                memset(COLOR_RAM, moonLight, 960);
                memset(GFX_1_SCR, moonLight, 960);
                break;
            default:
                moonLight = VCOL_DARK_GREY;
                while(!fontCopyDone){};
                setBank(MENU_BANK_MAP_VILLIAGE_3);
                memcpy(COLOR_RAM, _halfMoonColorMap, 960);
                memcpy(GFX_1_SCR, _halfMoonColorMap, 960);
                moonLightLevel = 0;
                setBank(MENU_BANK_MAP_VILLIAGE_1);
        }
    }
}

void drawPlayer(){
    char pbank = setBank(MENU_BANK_MAP_VILLIAGE_1);
    _drawPlayerAndColors();
    setBank(pbank);
}

void villiageMapDraw(char dir){
    char frameStart = gms_frameCount;


    // draw map
    char pbank = setBank(MENU_BANK_MAP_VILLIAGE_2);
    if(isMapDay){
        villiageMapDrawDay(_map, vMapX, vMapY, dir);
    } else {
        if(_previousDir != dir){
            // fill screen with moonlight, TODO: Unpack it. <- not enough mem, would have to be in ROM... not sure if the speed increase is worth it
            // memset(GFX_1_SCR, moonLight, 960);
            switch(cal_moonPhase){
                case MOON_PHASE_FULL:
                    // TODO: add a _fullMoonColorMap and copy it
                    memset(GFX_1_SCR, moonLight, 960);
                    break;
                case MOON_PHASE_NONE:
                    memset(GFX_1_SCR, moonLight, 960);
                    break;
                default:
                    char tbank = setBank(MENU_BANK_MAP_VILLIAGE_3);
                    memcpy(GFX_1_SCR, _halfMoonColorMap, 960);
                    setBank(tbank);
            }
        }
        _previousDir = dir;
        villiageMapDrawNight(_map, vMapX, vMapY, dir);
    }
    
    setBank(pbank);

    // draw player
    drawPlayer();


    // text window
    pbank = setBank(MENU_BANK_MAP_VILLIAGE_3);
    char vx = (vMapX+19) >> 2;
    char vy = (vMapY+12) >> 2;
    char locId = _mapLocations[vx + vy*64];
    setBank(pbank);
    if(vMapLocation != locId){
        vMapLocation = locId;
        textToSpriteBankPt = SPR_CHARACTER_PORTRAIT2;
        char str[12*3+1];
        // char framesUsed = gms_frameCount - frameStart;
        textToSprite((char *)LOCATION_NAMES[locId], 4);
    }


    joyCursor.moveDelayCurrent = 0;
}

void villiageMapInit(){
    // SCREEN_TRANSITION mode is on, so screen is black

    // // stop IRQs
    // rirq_stop();

    playSong(VILLIAGE_MAP_SONG);

    // clean 0xffff - so we don't have artefacts when we open borders
    ((char *)0xffff)[0] = 0;
    // Load GFX
    villiageMapScreenInit();
    
    // // start raster IRQ processing
    // rirq_start();
    // villiageMapSpriteLoader();
    buildRamTiles();

    // draw map
    villiageMapDraw(WALK_UP);

    // make screen visible
    switchScreenTo(SCREEN_HIRES_TXT);
}
