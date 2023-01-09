#include <c64/vic.h>
#include <c64/easyflash.h>
#include <c64/sprites.h>
#include <string.h>
#include <c64/memmap.h>
#include <c64/keyboard.h>

#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <miniGame/villiageMapMain.h>

// ---------------------------------------------------------------------------------------------
// Main screen and sprite bank + loaders code
// ---------------------------------------------------------------------------------------------

#pragma data ( villiageMapGfx1 )

// L0 is moonlight, +3 chars, dark grey/all black
// L1 is darker, +3 chars, grey-ish
// L2 is dark, +2 chars, dimmer colors
// L3 is neutral, +1 chars, normal light
// L4 is bright, normal char, brighter colors

static const char _charAttribsL1[] = {
    #embed ctm_attr1    "assets/charGfx/HiresVilliage_L1.ctm"
};
static const char _charAttribsL2[] = {
    #embed ctm_attr1    "assets/charGfx/HiresVilliage_L2.ctm"
};
static const char _charAttribsL3[] = {
    #embed ctm_attr1    "assets/charGfx/VilliageMapHiresMainBig.ctm"
};
static const char _charAttribsL4[] = {
    #embed ctm_attr1    "assets/charGfx/HiresVilliage_L4.ctm"
};
static const char _chars[] = {
    #embed ctm_chars    "assets/charGfx/VilliageMapHiresMainBig.ctm"
};



#pragma data ( villiageMapData )

static const char _map[] = {
    #embed ctm_map8     "assets/charGfx/VilliageMapHiresMainBig.ctm"
};

static const char _tiles[] = {
    #embed ctm_tiles8   "assets/charGfx/VilliageMapHiresMainBig.ctm"
};

static const char _lightMap[] = {
    #embed ctm_map8     "assets/charGfx/lightMap_up_andy.ctm"
};


// ---------------------------------------------------------------------------------------------
// Loaders code, called with IRQ off
// ---------------------------------------------------------------------------------------------
#pragma code ( villiageMapGfx1Loaders )
#pragma data ( villiageMapRAMData )
#define COLOR_MAP_1 (char *)0xcc00
#define COLOR_MAP_2 (char *)0xcd00
#define COLOR_MAP_3 (char *)0xce00
#define COLOR_MAP_4 (char *)0xcf00

// Copy chars and lightmaps, any sprites etc
static void _mapInit(){
    // ROM on, I/O off - as we will copy to RAM under I/O ports
    mmap_set(0b00110011);
    memcpy(GFX_1_FNT2, _chars, sizeof(_chars));
    memcpy(COLOR_MAP_1, _charAttribsL1, 0xff);
    memcpy(COLOR_MAP_2, _charAttribsL2, 0xff);
    memcpy(COLOR_MAP_3, _charAttribsL3, 0xff);
    memcpy(COLOR_MAP_4, _charAttribsL4, 0xff);
    // turn ROMS and I/O back on, so that we don't get a problem when bank tries to be switched but I/O is not visible
    mmap_set(MMAP_ROM);
    vic.color_back = VCOL_BROWN;
    vic.color_border = VCOL_BLACK;
    vic.color_back1 = VCOL_DARK_GREY;
    vic.color_back2 = VCOL_LT_GREY;
}


// copy base map from ROM, add any specials to it
static void _tilesRemap(void)
{
    // for(char y=0; y<V_MAP_SIZE_Y; y++)
    // {
    //     for(char x=0; x<V_MAP_SIZE_X; x++)
    //     {
    //         _map[V_MAP_SIZE_X * y + x] = ro_map[V_MAP_SIZE_X * y + x];
    //     }
    // }
}

// ---------------------------------------------------------------------------------------------
// Display code
// ---------------------------------------------------------------------------------------------
#pragma code ( villiageMapDisplayCode )
#pragma data ( villiageMapRAMData )
static char _moonlight = VCOL_DARK_GREY;

void tiles_put4x4row0(char * dp, char * cp, const char * lmp, const char * mp, const char * tp)
{
    for(char tx=0; tx<10; tx++)
    {
        const char  * ti = tp + mp[tx] * 16;
        const char * lm = lmp + tx*4;
#assign cx 0
#repeat
        {
            byte lightMap = lm[cx];
            char ci = ti[cx];
            if(lightMap && ci >= 0xc0){
                ci += lightMap-1;
            }
            dp[cx] = ci;
            cp[cx] = _moonlight;
            switch (lightMap){
                case 0:
                    cp[cx] = _moonlight;
                    break;
                case 1:
                    cp[cx] = (COLOR_MAP_1)[ci];
                    break;
                case 2:
                    cp[cx] = (COLOR_MAP_2)[ci];
                    break;
                case 3:
                    cp[cx] = (COLOR_MAP_3)[ci];
                    break;
                case 4:
                    cp[cx] = (COLOR_MAP_4)[ci];
                    break;
            }
        }
#assign cx cx + 1
#until cx == 4

        dp += 4;
        cp += 4;
    }
}

void tiles_put4x4row1(char * dp, char * cp, const char * mp, const char * tp)
{
    const char  * ti = tp + mp[0] * 16 + 1;

#assign cx 0
#repeat
    {
        char ci = ti[cx];
        dp[cx] = ci;
        cp[cx] = _charAttribsL3[ci];
        // cp[cx] = _moonlight;
    }
#assign cx cx + 1
#until cx == 3

    dp += 3;
    cp += 3;

    for(char tx=1; tx<10; tx++)
    {
        ti = tp + mp[tx] * 16;

#assign cx 0
#repeat
        {
            char ci = ti[cx];
            dp[cx] = ci;
            cp[cx] = _charAttribsL3[ci];
            // cp[cx] = _moonlight;
        }
#assign cx cx + 1
#until cx == 4

        dp += 4;
        cp += 4;
    }

    ti = tp + mp[10] * 16;

#assign cx 0
#repeat
    {
        char ci = ti[cx];
        dp[cx] = ci;
        cp[cx] = _charAttribsL3[ci];
        // cp[cx] = _moonlight;
    }
#assign cx cx + 1
#until cx == 1
}

void tiles_put4x4row2(char * dp, char * cp, const char * mp, const char * tp)
{
    const char  * ti = tp + mp[0] * 16 + 2;

#assign cx 0
#repeat
    {
        char ci = ti[cx];
        dp[cx] = ci;
        cp[cx] = _charAttribsL3[ci];
        // cp[cx] = _moonlight;
    }
#assign cx cx + 1
#until cx == 2

    dp += 2;
    cp += 2;

    for(char tx=1; tx<10; tx++)
    {
        ti = tp + mp[tx] * 16;

#assign cx 0
#repeat
        {
            char ci = ti[cx];
            dp[cx] = ci;
            cp[cx] = _charAttribsL3[ci];
            // cp[cx] = _moonlight;
        }
#assign cx cx + 1
#until cx == 4

        dp += 4;
        cp += 4;
    }

    ti = tp + mp[10] * 16;

#assign cx 0
#repeat
    {
        char ci = ti[cx];
        dp[cx] = ci;
        cp[cx] = _charAttribsL3[ci];
        // cp[cx] = _moonlight;
    }
#assign cx cx + 1
#until cx == 2
}

void tiles_put4x4row3(char * dp, char * cp, const char * mp, const char * tp)
{
    const char  * ti = tp + mp[0] * 16 + 3;

#assign cx 0
#repeat
    {
        char ci = ti[cx];
        dp[cx] = ci;
        cp[cx] = _charAttribsL3[ci];
        // cp[cx] = _moonlight;
    }
#assign cx cx + 1
#until cx == 1

    dp += 1;
    cp += 1;

    for(char tx=1; tx<10; tx++)
    {
        ti = tp + mp[tx] * 16;

#assign cx 0
#repeat
        {
            char ci = ti[cx];
            dp[cx] = ci;
            cp[cx] = _charAttribsL3[ci];
            // cp[cx] = _moonlight;
        }
#assign cx cx + 1
#until cx == 4

        dp += 4;
        cp += 4;
    }

    ti = tp + mp[10] * 16;

#assign cx 0
#repeat
    {
        char ci = ti[cx];
        dp[cx] = ci;
        cp[cx] = _charAttribsL3[ci];
        // cp[cx] = _moonlight;
    }
#assign cx cx + 1
#until cx == 3
}

void tiles_put4x4(const char * mp, char ox, char oy)
{
    char * dp = GFX_1_SCR, * cp = COLOR_RAM, * lmp = ((char *)_lightMap);

    mp += V_MAP_SIZE_X * (oy >> 2) + (ox >> 2);
    oy &= 3;
    ox &= 3;

    for(char ty=0; ty<24; ty++)
    {
        switch (ox)
        {
            case 0:
                tiles_put4x4row0(dp, cp, lmp, mp, _tiles + 4 * oy);
                break;
            case 1:
                tiles_put4x4row1(dp, cp, mp, _tiles + 4 * oy);
                break;
            case 2:
                tiles_put4x4row2(dp, cp, mp, _tiles + 4 * oy);
                break;
            case 3:
                tiles_put4x4row3(dp, cp, mp, _tiles + 4 * oy);
                break;
        }
        dp += 40;
        cp += 40;
        lmp += 40;

        oy ++;
        if (oy == 4)
        {
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

// Main game loop, entered every VSYNC
void villiageMapGameLoop(){
    // tiles_put4x4(_map, vMapX, vMapY);
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
    setBank(pbank);
}

void villiageMapDraw(){
    char pbank = setBank(MENU_BANK_MAP_VILLIAGE_2);
    tiles_put4x4(_map, vMapX, vMapY);
    setBank(pbank);
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
    // tiles_remap();

    splashScreen(false, 2);
    // start raster IRQ processing
    rirq_start();

    // draw map
    villiageMapDraw();
}
