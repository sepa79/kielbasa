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

static const char _charsRom[] = {
    #embed "assets/charGfx/HiresVilliage_L3 - Chars.bin"
};

static const char _mapRom[] = {
    #embed "assets/charGfx/HiresVilliage_L3 - (8bpc, 112x72) Map.bin"
    // #embed "assets/charGfx/HiresVilliage_L3 - (8bpc, 28x18) Map.bin"
};

static const char _tilesRom[] = {
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
    char pport = setPort(MMAP_ALL_ROM);
    memcpy(GFX_1_FNT2, _charsRom, sizeof(_charsRom));
    // turn ROMS and I/O back on, so that we don't get a problem when bank tries to be switched but I/O is not visible
    setPort(pport);
    vic.color_back = VCOL_BROWN;
    vic.color_border = VCOL_BLACK;
    vic.color_back1 = VCOL_DARK_GREY;
    vic.color_back2 = VCOL_LT_GREY;
}

// Copy base map from ROM, 'unpack' it from tilesets to plain simple bytes, add any 'specials' like
// crops status or 'permanent' map changes to it.
// Also apply light conditions straight away, to speed things up.
void _tilesRemap(void){
    // unpack map to RAM
    // for(char y=0; y<V_MAP_SIZE_Y; y++) {
    //     for(char x=0; x<V_MAP_SIZE_X; x++) {
    //         // get tileId, then pointer to tile data
    //         const char tileId = _mapRom[V_MAP_SIZE_X * y + x];
    //         const char * tileP = _tilesRom + tileId*V_MAP_TILE_SIZE_Y*V_MAP_TILE_SIZE_X;
    //         // pointer to map's RAM position
    //         char * mapPos = MAP_RAM + V_MAP_SIZE_X * V_MAP_TILE_SIZE_Y * y + x * V_MAP_TILE_SIZE_X;

    //         // loop over tile's Y
    //         for(char ty=0; ty<V_MAP_TILE_SIZE_Y; ty++) {
    //             // copy tile characters to RAM
    //             #assign cx 0
    //             #repeat
    //             mapPos[cx] = tileP[cx];
    //             #assign cx cx + 1
    //             #until cx == 4
    //             // move tile pointer one row down
    //             tileP += V_MAP_TILE_SIZE_X;
    //             // adjust map position
    //             mapPos += V_MAP_SIZE_X*V_MAP_TILE_SIZE_X;
    //         }
    //     }
    // }
    memcpy(MAP_RAM, _mapRom, 8192);
}

// this code needs to be in main block, as it switches banks
#pragma code ( villiageMapRAMCode )
#pragma data ( villiageMapRAMData )
static char _moonlight = 0;

static void villiageMapTilesRemap(){
    char pbank = setBank(MENU_BANK_MAP_VILLIAGE_1);
    _tilesRemap();
    setBank(pbank);
}

void villiageMapScreenInit(void){
    char pbank = setBank(MENU_BANK_MAP_VILLIAGE_1);
    _screenInit();
    setBank(pbank);
}

// void villiageMapSpriteLoader(){
//     char pbank = setBank(MENU_BANK_MAP_VILLIAGE_1);
//     _spriteLoader();
//     setBank(pbank);
// }

void drawVilliageMapNight(char ox, char oy){
    rirq_stop();
    vic.color_border++;
    char pport = setPort(MMAP_NO_ROM);
    char * dp = GFX_1_SCR, * cp = COLOR_RAM, * lmp = ((char *)_lightMap);

    char * mapPos = MAP_RAM;// + V_MAP_SIZE_X * V_MAP_TILE_SIZE_Y * oy + ox;

    for(char ty=0; ty<24; ty++) {
        for(char tx=0; tx<40; tx++) {
            // *((char *)0x01) = MMAP_NO_ROM;
            mmap_set(MMAP_NO_ROM);
            char c = mapPos[tx];
            mmap_set(MMAP_ROM);
            // *((char *)0x01) = MMAP_ROM;
            char lightMap = lmp[tx];

            if(lightMap && c >= 0xc0){
                c += lightMap-1;
            }
            dp[tx] = c;
            switch (lightMap){
                case 0:
                    cp[tx] = VCOL_DARK_GREY;
                    break;
                case 1:
                    cp[tx] = _charAttribsL1[c];
                    break;
                case 2:
                    cp[tx] = _charAttribsL2[c];
                    break;
                case 3:
                    cp[tx] = _charAttribsL3[c];
                    break;
                case 4:
                    // cp[tx] = _charAttribsL4[c];
                    cp[tx] = _charAttribsL3[c];
                    break;
            }
        }
        dp += 40;
        cp += 40;
        lmp += 40;
        mapPos += V_MAP_SIZE_X*V_MAP_TILE_SIZE_X;
    }
    setPort(pport);

    vic.color_border--;
    rirq_start();
}

// ---------------------------------------------------------------------------------------------
// Game code
// ---------------------------------------------------------------------------------------------
#pragma code ( villiageMapCode )
#pragma data ( villiageMapRAMData )

// L0 is moonlight, +3 chars, dark grey/all black
// L1 is darker, +3 chars, grey-ish
// L2 is dark, +2 chars, dimmer colors
// L3 is neutral, +1 chars, normal light
// L4 is bright, normal char, brighter colors

// void tiles_put4x4row0(char * dp, char * cp, const char * lmp, const char * mp, const char * tp){
//     for(char tx=0; tx<40; tx++) {
//         const char * lm = lmp + tx*4;
//         byte lightMap = lm[cx];

//         char * mapPos = mp + tx;

//         char ci = mapPos[0];
//         if(lightMap && ci >= 0xc0){
//             ci += lightMap-1;
//         }
//         dp[cx] = ci;
//         switch (lightMap){
//             case 0:
//                 cp[cx] = _moonlight;
//                 break;
//             case 1:
//                 cp[cx] = _charAttribsL1[ci];
//                 break;
//             case 2:
//                 cp[cx] = _charAttribsL2[ci];
//                 break;
//             case 3:
//                 cp[cx] = _charAttribsL3[ci];
//                 break;
//             case 4:
//                 cp[cx] = _charAttribsL4[ci];
//                 break;
//         }
//         dp += 4;
//         cp += 4;
//     }
// }

// Main game loop, entered every VSYNC
void mapGameLoop(){
    drawVilliageMapNight(vMapX, vMapY);
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
    char pport = setPort(MMAP_ROM);
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
    villiageMapTilesRemap();

    splashScreen(false, 2);
    // start raster IRQ processing
    rirq_start();
    mapGameLoop();
}
