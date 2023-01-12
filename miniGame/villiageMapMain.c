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
#include <menu/menuSystem.h>

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
    #embed ctm_chars    "assets/charGfx/HiresVilliage_L1.ctm"
};



#pragma data ( villiageMapData )

static const char _map[] = {
    #embed ctm_map8     "assets/charGfx/VilliageMapHiresMainBig.ctm"
};

const char romTiles[] = {
    #embed ctm_tiles8   "assets/charGfx/VilliageMapHiresMainBig.ctm"
};

typedef char char1024[1024];
// static char1024 * const _lightMap = &_lightMapU;

const char1024 _lightMap[4] = { { 
    #embed ctm_map8     "assets/charGfx/lightMap_up.ctm"
},{
    #embed ctm_map8     "assets/charGfx/lightMap_down.ctm"
},{
    #embed ctm_map8     "assets/charGfx/lightMap_left.ctm"
},{
    #embed ctm_map8     "assets/charGfx/lightMap_right.ctm"
}};
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
    // ROM on, I/O off - as we will copy to RAM under I/O ports
    mmap_set(0b00110011);
    memcpy(GFX_1_FNT2, _chars, sizeof(_chars));
    memcpy(colorMap[0], _charAttribsL1, 0x100);
    memcpy(colorMap[1], _charAttribsL2, 0x100);
    memcpy(colorMap[2], _charAttribsL3, 0x100);
    memcpy(colorMap[3], _charAttribsL4, 0x100);
    // turn ROMS and I/O back on, so that we don't get a problem when bank tries to be switched but I/O is not visible
    mmap_set(MMAP_ROM);
    vic.color_back = VCOL_BROWN;
    vic.color_border = VCOL_BLACK;
    vic.color_back1 = VCOL_DARK_GREY;
    vic.color_back2 = VCOL_LT_GREY;
}

// ---------------------------------------------------------------------------------------------
// Display code
// ---------------------------------------------------------------------------------------------
#pragma code ( villiageMapDisplayCode )
#pragma data ( villiageMapRAMData )
// no moon
// static char _moonlight = VCOL_BLACK;
// dark moon
static char _moonlight = VCOL_DARK_GREY;
bool isMapDay = true;

#define LIGHTMAP_DRAW_ROUTINE \
        {\
            byte lightMap = lm[cx];\
            char ci = ti[cx];\
            if(lightMap){\
                --lightMap;\
                cp[cx] = colorMap[lightMap][ci];\
                if(ci >= 0x80){\
                    ci += lightMap;\
                } \
            }\
            dp[cx] = ci;\
        }

// #define LIGHTMAP_DRAW_ROUTINE \
//          {\
//             byte lightMap = lm[cx];\
//             char ci = ti[cx];\
//             if(lightMap){\
//                 --lightMap;\
//                 cp[cx] = colorMap[lightMap][ci];\
//                 if(ci >= 0x80){\
//                     ci += lightMap;\
//                 } \
//             // no need to color bits that are moonlight color - done at the beginning\
//             // } else {\
//             //     cp[cx] = _moonlight;\
//             }\
//             dp[cx] = ci;\
//         }

char * ramTiles = (char *)0xc480;

void tiles_put4x4row0(char * dp, char * cp, const char * lmp, const char * mp, const char * tp, const char * rtp){
    for(char tx=0; tx<10; tx++){
        const char * tileP = tp;
        if(mp[tx] <= RAM_TILES_COUNT)
            tileP = rtp;
        const char * ti = tileP + mp[tx] * 16;
        const char * lm = lmp + tx*4;
#assign cx 0
#repeat
LIGHTMAP_DRAW_ROUTINE
#assign cx cx + 1
#until cx == 4

        dp += 4;
        cp += 4;
    }
}

void tiles_put4x4row1(char * dp, char * cp, const char * lmp, const char * mp, const char * tp, const char * rtp){
    const char * tileP = tp;
    if(mp[0] <= RAM_TILES_COUNT)
        tileP = rtp;
    const char  * ti = tileP + mp[0] * 16 + 1;
    const char * lm = lmp;

#assign cx 0
#repeat
LIGHTMAP_DRAW_ROUTINE
#assign cx cx + 1
#until cx == 3

    dp += 3;
    cp += 3;

    for(char tx=1; tx<10; tx++){
        const char * tileP = tp;
        if(mp[tx] <= RAM_TILES_COUNT)
            tileP = rtp;
        ti = tileP + mp[tx] * 16;
        lm = lmp + tx*4 -1;

#assign cx 0
#repeat
LIGHTMAP_DRAW_ROUTINE
#assign cx cx + 1
#until cx == 4

        dp += 4;
        cp += 4;
    }
    tileP = tp;
    if(mp[10] <= RAM_TILES_COUNT)
        tileP = rtp;
    ti = tileP + mp[10] * 16;
    lm = lmp + 40-1;

#assign cx 0
#repeat
LIGHTMAP_DRAW_ROUTINE
#assign cx cx + 1
#until cx == 1
}

void tiles_put4x4row2(char * dp, char * cp, const char * lmp, const char * mp, const char * tp, const char * rtp){
    const char * tileP = tp;
    if(mp[0] <= RAM_TILES_COUNT)
        tileP = rtp;
    const char  * ti = tileP + mp[0] * 16 + 2;
    const char * lm = lmp;

#assign cx 0
#repeat
LIGHTMAP_DRAW_ROUTINE
#assign cx cx + 1
#until cx == 2

    dp += 2;
    cp += 2;

    for(char tx=1; tx<10; tx++){
        const char * tileP = tp;
        if(mp[tx] <= RAM_TILES_COUNT)
            tileP = rtp;
        ti = tileP + mp[tx] * 16;
        lm = lmp + tx*4-2;

#assign cx 0
#repeat
LIGHTMAP_DRAW_ROUTINE
#assign cx cx + 1
#until cx == 4

        dp += 4;
        cp += 4;
    }

    tileP = tp;
    if(mp[10] <= RAM_TILES_COUNT)
        tileP = rtp;
    ti = tileP + mp[10] * 16;
    lm = lmp + 40-2;

#assign cx 0
#repeat
LIGHTMAP_DRAW_ROUTINE
#assign cx cx + 1
#until cx == 2
}

void tiles_put4x4row3(char * dp, char * cp, const char * lmp, const char * mp, const char * tp, const char * rtp){
    const char * tileP = tp;
    if(mp[0] <= RAM_TILES_COUNT)
        tileP = rtp;
    const char  * ti = tileP + mp[0] * 16 + 3;
    const char * lm = lmp;

#assign cx 0
#repeat
LIGHTMAP_DRAW_ROUTINE
#assign cx cx + 1
#until cx == 1

    dp += 1;
    cp += 1;

    for(char tx=1; tx<10; tx++){
        const char * tileP = tp;
        if(mp[tx] <= RAM_TILES_COUNT)
            tileP = rtp;
        ti = tileP + mp[tx] * 16;
        lm = lmp + tx*4 -3;

#assign cx 0
#repeat
LIGHTMAP_DRAW_ROUTINE
#assign cx cx + 1
#until cx == 4

        dp += 4;
        cp += 4;
    }

    tileP = tp;
    if(mp[10] <= RAM_TILES_COUNT)
        tileP = rtp;
    ti = tileP + mp[10] * 16;
    lm = lmp + 40-3;

#assign cx 0
#repeat
LIGHTMAP_DRAW_ROUTINE
#assign cx cx + 1
#until cx == 3
}

void villiageMapDrawNight(const char * mp, char ox, char oy, char dir){
    char * dp;
    if(map_2ndScreen){
        dp = GFX_1_SCR2;
    } else {
        dp = GFX_1_SCR3;
    }
    
    // char * cp = COLOR_RAM;
    char * cp = GFX_1_SCR;
    char * lmp = _lightMap[dir];

    if(_previousDir != dir){
        // fill screen with moonlight, TODO: Unpack it.
        memset(GFX_1_SCR, _moonlight, 960);
    }
    _previousDir = dir;

    mp += V_MAP_SIZE_X * (oy >> 2) + (ox >> 2);
    oy &= 3;
    ox &= 3;

    for(char ty=0; ty<24; ty++){
        switch (ox){
            case 0:
                tiles_put4x4row0(dp, cp, lmp, mp, romTiles + 4 * oy, ramTiles + 4 * oy);
                break;
            case 1:
                tiles_put4x4row1(dp, cp, lmp, mp, romTiles + 4 * oy, ramTiles + 4 * oy);
                break;
            case 2:
                tiles_put4x4row2(dp, cp, lmp, mp, romTiles + 4 * oy, ramTiles + 4 * oy);
                break;
            case 3:
                tiles_put4x4row3(dp, cp, lmp, mp, romTiles + 4 * oy, ramTiles + 4 * oy);
                break;
        }
        dp += 40;
        cp += 40;
        lmp += 40;

        oy ++;
        if (oy == 4){
            mp += V_MAP_SIZE_X;
            oy = 0;
        }
    }
}


// ---------------------------------------------------------------------------------------------
// Game code
// ---------------------------------------------------------------------------------------------
#pragma code ( villiageMapCode )
#pragma data ( villiageMapRAMData )

#define FIELD_1_WIDTH 3
// *4 to go through 4 tile characters
#define FIELD_1_HEIGHT 2
#define FIELD_2_WIDTH 2
#define FIELD_2_HEIGHT 3
#define FIELD_3_WIDTH 3
#define FIELD_3_HEIGHT 4
#define FIELD_4_WIDTH 3
#define FIELD_4_HEIGHT 6

#define GROUND_TILE 0xbc
// 1st tile is empty
#define FIELD_START ramTiles+0x10

// copy base map from ROM, add any specials to it
static void buildRamTiles(void){
    // memset(ramTiles, GROUND_TILE, 16*(RAM_TILES_COUNT+1));
    
    // how many crops to draw
    char cropCount = 44;
    // plant & stage to draw
    // char plant = 0x10 + 4*plant + 3-stage;
    char plant = 0x10 + 4*0 + 3-0;
    // field
    char * field = FIELD_START;
    // Field Y
    for(char fy=0; fy<FIELD_1_HEIGHT; fy++){
        // Tile Y
        for(char ty=0; ty<4; ty++){
            // Field X
            for(char fx=0; fx<FIELD_1_WIDTH; fx++){
                // set pointer to current X tile, Y row
                field = FIELD_START + fx*16 + fy*FIELD_1_WIDTH*16;
                // Tile X
                for(char tx=0; tx<4; tx++){
                    if(cropCount){
                        field[tx+4*ty] = plant;
                        cropCount--;
                    } else {
                        field[tx+4*ty] = GROUND_TILE;
                    }
                }
            }
        }
    }
}

// Main game loop, entered every VSYNC
void villiageMapGameLoop(){
    // tiles_put4x4(_map, vMapX, vMapY);
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
        GFX_1_SCR2[40*11+19] = 0;
        map_2ndScreen = false;
    } else {
        GFX_1_SCR3[40*11+19] = 0;
        map_2ndScreen = true;
    }
    _drawColors();
    // player color
    COLOR_RAM[40*11+19] = VCOL_MED_GREY;
}

// ---------------------------------------------------------------------------------------------
// RAM code
// ---------------------------------------------------------------------------------------------
// this code needs to be in main block, as it switches banks
#pragma code ( villiageMapRAMCode )
#pragma data ( villiageMapRAMData )

void villiageMapScreenInit(void){
    char pbank = setBank(MENU_BANK_MAP_VILLIAGE_1);
    _mapInit();
    isMapDay = cal_isDay;
    setBank(pbank);
    // fill screen with moonlight
    memset(COLOR_RAM, _moonlight, 960);
    memset(GFX_1_SCR, _moonlight, 960);
}

void drawPlayer(){
    char pbank = setBank(MENU_BANK_MAP_VILLIAGE_1);
    _drawPlayerAndColors();
    setBank(pbank);
}

void villiageMapDraw(char dir){
    byte frameStart = gms_frameCount;


    // draw map
    char pbank = setBank(MENU_BANK_MAP_VILLIAGE_2);
    if(isMapDay){
        villiageMapDrawDay(_map, vMapX, vMapY, dir);
    } else {
        villiageMapDrawNight(_map, vMapX, vMapY, dir);
    }
    
    setBank(pbank);

    // draw player
    drawPlayer();



    // frame counter
    sprBankPointer = SPR_CHARACTER_BAR2;
    byte framesUsed = gms_frameCount - frameStart;
    byte str[4];
    sprintf(str, "%03u", framesUsed);
    copyCharToSprite(str[0], 0, 0);
    copyCharToSprite(str[1], 1, 0);
    copyCharToSprite(str[2], 2, 0);

    // ready to move again
    joyCursor.moveDelayCurrent = 0;
}

void villiageMapInit(){
    // could replace with simply changing screens
    splashScreen(false, 1);
    // stop IRQs and change to ours
    rirq_stop();

    // msx off
    ((byte *)0xd418)[0] &= ~0xf;
    // screen off, sprites off
    // vic.ctrl1 = VIC_CTRL1_BMM | VIC_CTRL1_RSEL | 3;
    vic.spr_enable = 0b00000000;

    __asm {
        // init music
        lda #MSX_ROM
        sta $01
        lda #$04
        jsr MSX_INIT
    }
    // if you use the mmap_trampoline() you have to call the mmap_set() at least once to init the shadow variable
    mmap_set(MMAP_ROM);
    // Activate trampoline
    mmap_trampoline();
    // Disable CIA interrupts, we do not want interference
    // with our joystick interrupt
    cia_init();
    // clean 0xffff - so we don't have artefacts when we open borders
    ((char *)0xffff)[0] = 0;

    // Load GFX
    villiageMapScreenInit();
    // villiageMapSpriteLoader();
    buildRamTiles();

    splashScreen(false, 2);
    // start raster IRQ processing
    rirq_start();

    // draw map
    villiageMapDraw(WALK_UP);

}
