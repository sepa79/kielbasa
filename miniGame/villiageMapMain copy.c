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

static const char _charAttribsL1[] = {
    #embed "assets/charGfx/HiresVilliage_L1 - CharAttribs_L1.bin"
};
static const char _charAttribsL2[] = {
    #embed "assets/charGfx/HiresVilliage_L2 - CharAttribs_L1.bin"
};
static const char _charAttribsL3[] = {
    #embed "assets/charGfx/HiresVilliage_L3 - CharAttribs_L1.bin"
};
static const char _charAttribsL4[] = {
    #embed "assets/charGfx/HiresVilliage_L4 - CharAttribs_L1.bin"
};

static const char ro_chars[] = {
    #embed "assets/charGfx/HiresVilliage_L3 - Chars.bin"
};

static const char ro_map[] = {
    #embed "assets/charGfx/HiresVilliage_L3 - (8bpc, 28x18) Map.bin"
};

static const char ro_tiles[] = {
    #embed "assets/charGfx/HiresVilliage_L3 - Tiles.bin"
};

static const char _lightMap[] = {
    #embed "assets/charGfx/lightMap_up.bin"
};

// static const char ro_tileAttribs[] = {
//     #embed "assets/charGfx/kielbasaMapS2 - TileAttribs_L1.bin"
// };

#pragma code ( villiageMapGfx1Loaders )
#pragma data ( villiageMapRAMData )

static void _screenInit(){
    // ROM on, I/O off - as we will copy to RAM under I/O ports
    mmap_set(0b00110011);
    memcpy(GFX_1_FNT2, ro_chars, sizeof(ro_chars));
    // turn ROMS and I/O back on, so that we don't get a problem when bank tries to be switched but I/O is not visible
    mmap_set(MMAP_ROM);
    vic.color_back = VCOL_BROWN;
    vic.color_border = VCOL_BLACK;
    vic.color_back1 = VCOL_DARK_GREY;
    vic.color_back2 = VCOL_LT_GREY;
}

// this code needs to be in main block, as it switches banks
#pragma code ( villiageMapRAMCode )
#pragma data ( villiageMapRAMData )

static void _spriteLoader(){
}

void villiageMapScreenInit(void){
    char pbank = setBank(MENU_BANK_MAP_VILLIAGE_1);
    _screenInit();
    setBank(pbank);
}

void villiageMapSpriteLoader(){
    char pbank = setBank(MENU_BANK_MAP_VILLIAGE_1);
    _spriteLoader();
    setBank(pbank);
}

#pragma code ( villiageMapRAMCode )
#pragma data ( villiageMapRAMData )
#include <c64/rasterirq.h>
RIRQCode topMap, msxMap, openMap;

// ---------------------------------------------------------------------------------------------
// Game code
// ---------------------------------------------------------------------------------------------
#pragma code ( villiageMapCode )
#pragma data ( villiageMapRAMData )
static char ro_xmap[V_MAP_SIZE_X * V_MAP_SIZE_Y];
static char _moonlight = VCOL_DARK_GREY;

// copy base map from ROM, add any specials to it
void tiles_remap(void)
{
    for(char y=0; y<V_MAP_SIZE_Y; y++)
    {
        for(char x=0; x<V_MAP_SIZE_X; x++)
        {
            ro_xmap[V_MAP_SIZE_X * y + x] = ro_map[V_MAP_SIZE_X * y + x];
        }
    }
}

// L0 is moonlight, +3 chars, dark grey/all black
// L1 is darker, +3 chars, grey-ish
// L2 is dark, +2 chars, dimmer colors
// L3 is neutral, +1 chars, normal light
// L4 is bright, normal char, brighter colors
// Chars will be remapped according to entry here, number on given index indicates charcode to use instead
// static const byte _charRemapL3[256] = {
//     // we don't remap currently the first 192 chars
//     0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, // 32 bytes
//     0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, // 32 bytes
//     0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, // 32 bytes
//     0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, // 32 bytes
//     0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, // 32 bytes
//     0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, // 32 bytes
//     // we are at our remappable chars
//     0xc0, 0xc1, 0xc2, 0xc3,
//     0xc4, 0xc5, 0xc6, 0xc7,
//     0xc8, 0xc9, 0xca, 0xcb,
//     0xcc, 0xcd, 0xce, 0xcf,
// };

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
            switch (lightMap){
                case 0:
                    cp[cx] = _moonlight;
                    break;
                case 1:
                    cp[cx] = _charAttribsL1[ci];
                    break;
                case 2:
                    cp[cx] = _charAttribsL2[ci];
                    break;
                case 3:
                    cp[cx] = _charAttribsL3[ci];
                    break;
                case 4:
                    cp[cx] = _charAttribsL4[ci];
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
                tiles_put4x4row0(dp, cp, lmp, mp, ro_tiles + 4 * oy);
                break;
            case 1:
                tiles_put4x4row1(dp, cp, mp, ro_tiles + 4 * oy);
                break;
            case 2:
                tiles_put4x4row2(dp, cp, mp, ro_tiles + 4 * oy);
                break;
            case 3:
                tiles_put4x4row3(dp, cp, mp, ro_tiles + 4 * oy);
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


// Main game loop, entered every VSYNC
void mapGameLoop(){
    tiles_put4x4(ro_xmap, vMapX, vMapY);
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
    tiles_remap();

    splashScreen(false, 2);
    // start raster IRQ processing
    rirq_start();
    mapGameLoop();
}
