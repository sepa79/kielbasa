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
#include <menu/menuSystem.h>

// ---------------------------------------------------------------------------------------------
// Display code
// ---------------------------------------------------------------------------------------------
#pragma code ( villiageMapDisplayCode )
#pragma data ( villiageMapRAMData )
#define DAY_COLOR_MAP 2
#define DAY_CHAR_MAP 3

// can change charset in mem  - move DAY_CHAR_MAP to postion 0 - and skip special chars check altogether
// saves 1 draw frame
#define LIGHTMAP_DRAW_ROUTINE \
        {\
            char ci = ti[cx];\
            if(ci >= 0xb0){\
                ci ^= DAY_CHAR_MAP;\
            }\
            cp[cx] = colorMap[DAY_COLOR_MAP][ci];\
            dp[cx] = ci;\
        }

static void _drawDayRow0(char * dp, char * cp, const char * mp, const char * tp, const char * rtp){
    for(char tx=0; tx<10; tx++){
        const char * tileP = tp;
        if(mp[tx] <= RAM_TILES_COUNT)
            tileP = rtp;
        const char * ti = tileP + mp[tx] * 16;
#assign cx 0
#repeat
LIGHTMAP_DRAW_ROUTINE
#assign cx cx + 1
#until cx == 4

        dp += 4;
        cp += 4;
    }
}

static void _drawDayRow1(char * dp, char * cp, const char * mp, const char * tp, const char * rtp){
    const char * tileP = tp;
    if(mp[0] <= RAM_TILES_COUNT)
        tileP = rtp;
    const char  * ti = tileP + mp[0] * 16 + 1;

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

#assign cx 0
#repeat
LIGHTMAP_DRAW_ROUTINE
#assign cx cx + 1
#until cx == 1
}

static void _drawDayRow2(char * dp, char * cp, const char * mp, const char * tp, const char * rtp){
    const char * tileP = tp;
    if(mp[0] <= RAM_TILES_COUNT)
        tileP = rtp;
    const char  * ti = tileP + mp[0] * 16 + 2;

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

#assign cx 0
#repeat
LIGHTMAP_DRAW_ROUTINE
#assign cx cx + 1
#until cx == 2
}

static void _drawDayRow3(char * dp, char * cp, const char * mp, const char * tp, const char * rtp){
    const char * tileP = tp;
    if(mp[0] <= RAM_TILES_COUNT)
        tileP = rtp;
    const char  * ti = tileP + mp[0] * 16 + 3;

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

#assign cx 0
#repeat
LIGHTMAP_DRAW_ROUTINE
#assign cx cx + 1
#until cx == 3
}

void villiageMapDrawDay(const char * mp, char ox, char oy){
    char * dp;
    if(map_2ndScreen){
        dp = GFX_1_SCR2;
    } else {
        dp = GFX_1_SCR3;
    }
    
    char * cp = GFX_1_SCR;

    mp += V_MAP_SIZE_X * (oy >> 2) + (ox >> 2);
    oy &= 3;
    ox &= 3;

    for(char ty=0; ty<24; ty++){
        switch (ox){
            case 0:
                _drawDayRow0(dp, cp, mp, romTiles + 4 * oy, ramTiles + 4 * oy);
                break;
            case 1:
                _drawDayRow1(dp, cp, mp, romTiles + 4 * oy, ramTiles + 4 * oy);
                break;
            case 2:
                _drawDayRow2(dp, cp, mp, romTiles + 4 * oy, ramTiles + 4 * oy);
                break;
            case 3:
                _drawDayRow3(dp, cp, mp, romTiles + 4 * oy, ramTiles + 4 * oy);
                break;
        }
        dp += 40;
        cp += 40;

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


// ---------------------------------------------------------------------------------------------
// RAM code
// ---------------------------------------------------------------------------------------------
// this code needs to be in main block, as it switches banks
#pragma code ( villiageMapRAMCode )
#pragma data ( villiageMapRAMData )
