#include <assets/assetsSettings.h>
#include <engine/easyFlashBanks.h>

// this should never be in mem, just used by loaders code
const char pigsleCommandGfxCannonAnim[] = {
    #embed 0xffff 0x0002 "assets/multicolorGfx/flak_88_10.08.22.kla"
};
// ---------------------------------------------------------------------------------------------
// Cannon Animation handling
// ---------------------------------------------------------------------------------------------
#pragma code ( pigsleCommandAnimCode )
#pragma data ( pigsleCommandRAMData )

#define STONKA_KOALA_BMP pigsleCommandGfxCannonAnim
#define STONKA_KOALA_SCR ((char *)pigsleCommandGfxCannonAnim + 0x1f40)
#define STONKA_KOALA_COL ((char *)pigsleCommandGfxCannonAnim + 0x2328)

#define CANNON_X_POS 16
#define CANNON_Y_POS 19

// load bitmap - 8x5 chars square, starting 0,0
void copyCannonUp(){
#define CANNON_FRAME 0
#assign _y 0
#repeat
#assign _x 0
#repeat
    GFX_1_BMP[40 * 8 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS*8] = STONKA_KOALA_BMP[40 * 8 * _y + _x + CANNON_FRAME*8*8];
#assign _x _x + 1
#until _x == 8*8
#assign _y _y + 1
#until _y == 5

#assign _y 0
#repeat
#assign _x 0
#repeat
    GFX_1_SCR[40 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS] = STONKA_KOALA_SCR[40*_y + _x + CANNON_FRAME*8];
    COLOR_RAM[40 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS] = STONKA_KOALA_COL[40*_y + _x + CANNON_FRAME*8];
#assign _x _x + 1
#until _x == 8
#assign _y _y + 1
#until _y == 5
}

// load bitmap - 8x5 chars square
void copyCannonL60(){
#define CANNON_FRAME 1
#assign _y 0
#repeat
#assign _x 0
#repeat
    GFX_1_BMP[40 * 8 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS*8] = STONKA_KOALA_BMP[40 * 8 * _y + _x + CANNON_FRAME*8*8];
#assign _x _x + 1
#until _x == 8*8
#assign _y _y + 1
#until _y == 5

#assign _y 0
#repeat
#assign _x 0
#repeat
    GFX_1_SCR[40 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS] = STONKA_KOALA_SCR[40*_y + _x + CANNON_FRAME*8];
    COLOR_RAM[40 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS] = STONKA_KOALA_COL[40*_y + _x + CANNON_FRAME*8];
#assign _x _x + 1
#until _x == 8
#assign _y _y + 1
#until _y == 5
}

// load bitmap - 8x5 chars square
void copyCannonR60(){
#define CANNON_FRAME 2
#assign _y 0
#repeat
#assign _x 0
#repeat
    GFX_1_BMP[40 * 8 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS*8] = STONKA_KOALA_BMP[40 * 8 * _y + _x + CANNON_FRAME*8*8];
#assign _x _x + 1
#until _x == 8*8
#assign _y _y + 1
#until _y == 5

#assign _y 0
#repeat
#assign _x 0
#repeat
    GFX_1_SCR[40 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS] = STONKA_KOALA_SCR[40*_y + _x + CANNON_FRAME*8];
    COLOR_RAM[40 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS] = STONKA_KOALA_COL[40*_y + _x + CANNON_FRAME*8];
#assign _x _x + 1
#until _x == 8
#assign _y _y + 1
#until _y == 5
}

// load bitmap - 8x5 chars square
void copyCannonL75(){
#define CANNON_FRAME 3
#assign _y 0
#repeat
#assign _x 0
#repeat
    GFX_1_BMP[40 * 8 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS*8] = STONKA_KOALA_BMP[40 * 8 * _y + _x + CANNON_FRAME*8*8];
#assign _x _x + 1
#until _x == 8*8
#assign _y _y + 1
#until _y == 5

#assign _y 0
#repeat
#assign _x 0
#repeat
    GFX_1_SCR[40 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS] = STONKA_KOALA_SCR[40*_y + _x + CANNON_FRAME*8];
    COLOR_RAM[40 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS] = STONKA_KOALA_COL[40*_y + _x + CANNON_FRAME*8];
#assign _x _x + 1
#until _x == 8
#assign _y _y + 1
#until _y == 5
}

// load bitmap - 8x5 chars square, starting 0,0
void copyCannonR75(){
#define CANNON_FRAME 4
#assign _y 0
#repeat
#assign _x 0
#repeat
    GFX_1_BMP[40 * 8 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS*8] = STONKA_KOALA_BMP[40 * 8 * _y + _x + CANNON_FRAME*8*8];
#assign _x _x + 1
#until _x == 8*8
#assign _y _y + 1
#until _y == 5

#assign _y 0
#repeat
#assign _x 0
#repeat
    GFX_1_SCR[40 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS] = STONKA_KOALA_SCR[40*_y + _x + CANNON_FRAME*8];
    COLOR_RAM[40 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS] = STONKA_KOALA_COL[40*_y + _x + CANNON_FRAME*8];
#assign _x _x + 1
#until _x == 8
#assign _y _y + 1
#until _y == 5
}
