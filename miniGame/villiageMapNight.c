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

// 0xa0 - start of 'lightmapped' chars
#define LIGHTMAP_DRAW_ROUTINE \
        {\
            char ci = ti[cx];\
            char lmC = lm[cx];\
            if(lmC){\
                char lMapDetail = lmC & 0b00001111;\
                char color = lmC >> 4;\
                if(color){\
                    cp[cx] = colorMap[color][ci];\
                    if(ci >= 0xa0){\
                        ci += lMapDetail -1;\
                    }\
                } else {\
                    char detailLvl = lMapDetail - moonDetailLevel;\
                    if(detailLvl < 4){\
                        if(ci >= 0xa0){\
                            ci += detailLvl;\
                        }\
                        cp[cx] = VCOL_DARK_GREY;\
                    }\
                }\
                dp[cx] = ci;\
            }\
        }

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

void villiageMapDrawNight(const char * mp, char ox, char oy, WalkDir dir){
    char * dp;
    if(map_2ndScreen){
        dp = GFX_1_SCR2;
    } else {
        dp = GFX_1_SCR3;
    }
    
    // char * cp = COLOR_RAM;
    char * cp = GFX_1_SCR;
    char * lmp = lightMap;

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


// ---------------------------------------------------------------------------------------------
// RAM code
// ---------------------------------------------------------------------------------------------
// this code needs to be in main block, as it switches banks
#pragma code ( villiageMapRAMCode )
#pragma data ( villiageMapRAMData )
