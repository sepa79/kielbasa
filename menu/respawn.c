#include <c64/vic.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>
#include <oscar.h>

#include <menu/menuSystem.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/uiHandler.h>
#include <engine/gameSettings.h>

// Sections and regions
#pragma section(respawnLoaderData, 0)
#pragma section(respawnCode, 0)
#pragma section(respawnGfx, 0)
#pragma section(respawnSprites, 0)
#pragma region(regionRespawnC, 0x8000, 0x9000, , MENU_BANK_RESPAWN, { respawnLoaderData, respawnCode })
#pragma region(regionRespawnG1, DAY_GFX_BMP, DAY_GFX_BMP + 0x4000, , MENU_BANK_RESPAWN, { respawnGfx })
// #pragma region(regionRespawnG2, DAY_GFX_BMP + 0x2711, DAY_GFX_BMP + 0x2711 + 1024, , MENU_BANK_RESPAWN, { respawnFnt })

#pragma data(respawnGfx)
__export const char respawnGfx[] = {
    #embed 8000 2 rle "assets/multicolorGfx/staminacritical120624.kla"
};
__export const char respawnGfxC1[] = {
    #embed 1000 0x1f42 "assets/multicolorGfx/staminacritical120624.kla"
};
__export const char respawnGfxC2[] = {
    #embed 1000 0x232a "assets/multicolorGfx/staminacritical120624.kla"
};
__export const char respawnSprites[] = {
    #embed 0xc0 20 "assets/sprites/back.spd"
};

// menu code is in ROM - data in RAM
#pragma code(respawnCode)
#pragma data(data)

// c800 is free to use by menu
#define SPRITE_BLOCK 0x20
#define SPRITE_BLOCK_POINTER ((char *)0xc800)

__interrupt static bool _showSprites = false;

__interrupt static void _menuShowSprites(){
    // too complex to put into irq if made as separate library, Oscar complains
    vic.spr_expand_x = 0b11111111;
    vic.spr_expand_y = 0b11111111;
    vic.spr_priority = 0b00000000;
    vic.spr_multi    = 0b00111000;
    vic.spr_mcolor0  = VCOL_WHITE;
    vic.spr_mcolor1  = VCOL_LT_GREY;

    if(_showSprites){
        vic.spr_enable = 0b00111111;
    } else {
        vic.spr_enable = 0b00000000;
    }

    #define xPos   190
    #define yPos   174
    #define xSize  3

    vic_sprxy(0, xPos + 0*48, yPos + 42);
    vic_sprxy(1, xPos + 1*48, yPos + 42);
    vic_sprxy(2, xPos + 2*48, yPos + 42);
    vic.spr_color[0] = VCOL_YELLOW;
    vic.spr_color[1] = VCOL_YELLOW;
    vic.spr_color[2] = VCOL_YELLOW;
    GFX_2_SCR[OFFSET_SPRITE_PTRS+0] = SPRITE_BLOCK + 0;
    GFX_2_SCR[OFFSET_SPRITE_PTRS+1] = SPRITE_BLOCK + 1;
    GFX_2_SCR[OFFSET_SPRITE_PTRS+2] = SPRITE_BLOCK + 2;

    vic_sprxy(3, xPos + 0*48 - 1, yPos + 42 - 9);
    vic_sprxy(4, xPos + 1*48 - 1, yPos + 42 - 9);
    vic_sprxy(5, xPos + 2*48 - 1, yPos + 42 - 9);
    vic.spr_color[3] = VCOL_BLACK;
    vic.spr_color[4] = VCOL_BLACK;
    vic.spr_color[5] = VCOL_BLACK;
    GFX_2_SCR[OFFSET_SPRITE_PTRS+3] = SPRITE_BLOCK + 3;
    GFX_2_SCR[OFFSET_SPRITE_PTRS+4] = SPRITE_BLOCK + 4;
    GFX_2_SCR[OFFSET_SPRITE_PTRS+5] = SPRITE_BLOCK + 5;
}

static void _loadBmpOnly() {
    memset(GFX_2_SCR, 0, 1000);
    memset(COLOR_RAM, 0, 1000);
    oscar_expand_rle(GFX_2_BMP, respawnGfx);
}

// const char recodeTable[] = {
//     VCOL_BLACK,      // VCOL_BLACK
//     VCOL_WHITE,      // VCOL_WHITE
//     VCOL_DARK_GREY,  // VCOL_RED
//     VCOL_LT_GREY,    // VCOL_CYAN
//     VCOL_MED_GREY,   // VCOL_PURPLE
//     VCOL_LT_GREY,    // VCOL_GREEN
//     VCOL_DARK_GREY,  // VCOL_BLUE
//     VCOL_WHITE,      // VCOL_YELLOW
//     VCOL_MED_GREY,   // VCOL_ORANGE
//     VCOL_DARK_GREY,  // VCOL_BROWN
//     VCOL_LT_GREY,    // VCOL_LT_RED
//     VCOL_DARK_GREY,  // VCOL_DARK_GREY
//     VCOL_MED_GREY,   // VCOL_MED_GREY
//     VCOL_WHITE,      // VCOL_LT_GREEN
//     VCOL_MED_GREY,   // VCOL_LT_BLUE
//     VCOL_LT_GREY     // VCOL_LT_GREY
// };

char recodeTable[] = {
    0x00, 0x01, 0x0b, 0x0f, 0x0c, 0x0f, 0x0b, 0x01, 0x0c, 0x0b, 0x0f, 0x0b, 0x0c, 0x01, 0x0c, 0x0f,
    0x10, 0x11, 0x1b, 0x1f, 0x1c, 0x1f, 0x1b, 0x11, 0x1c, 0x1b, 0x1f, 0x1b, 0x1c, 0x11, 0x1c, 0x1f,
    0xb0, 0xb1, 0xbb, 0xbf, 0xbc, 0xbf, 0xbb, 0xb1, 0xbc, 0xbb, 0xbf, 0xbb, 0xbc, 0xb1, 0xbc, 0xbf,
    0xf0, 0xf1, 0xfb, 0xff, 0xfc, 0xff, 0xfb, 0xf1, 0xfc, 0xfb, 0xff, 0xfb, 0xfc, 0xf1, 0xfc, 0xff,
    0xc0, 0xc1, 0xcb, 0xcf, 0xcc, 0xcf, 0xcb, 0xc1, 0xcc, 0xcb, 0xcf, 0xcb, 0xcc, 0xc1, 0xcc, 0xcf,
    0xf0, 0xf1, 0xfb, 0xff, 0xfc, 0xff, 0xfb, 0xf1, 0xfc, 0xfb, 0xff, 0xfb, 0xfc, 0xf1, 0xfc, 0xff,
    0xb0, 0xb1, 0xbb, 0xbf, 0xbc, 0xbf, 0xbb, 0xb1, 0xbc, 0xbb, 0xbf, 0xbb, 0xbc, 0xb1, 0xbc, 0xbf,
    0x10, 0x11, 0x1b, 0x1f, 0x1c, 0x1f, 0x1b, 0x11, 0x1c, 0x1b, 0x1f, 0x1b, 0x1c, 0x11, 0x1c, 0x1f,
    0xc0, 0xc1, 0xcb, 0xcf, 0xcc, 0xcf, 0xcb, 0xc1, 0xcc, 0xcb, 0xcf, 0xcb, 0xcc, 0xc1, 0xcc, 0xcf,
    0xb0, 0xb1, 0xbb, 0xbf, 0xbc, 0xbf, 0xbb, 0xb1, 0xbc, 0xbb, 0xbf, 0xbb, 0xbc, 0xb1, 0xbc, 0xbf,
    0xf0, 0xf1, 0xfb, 0xff, 0xfc, 0xff, 0xfb, 0xf1, 0xfc, 0xfb, 0xff, 0xfb, 0xfc, 0xf1, 0xfc, 0xff,
    0xb0, 0xb1, 0xbb, 0xbf, 0xbc, 0xbf, 0xbb, 0xb1, 0xbc, 0xbb, 0xbf, 0xbb, 0xbc, 0xb1, 0xbc, 0xbf,
    0xc0, 0xc1, 0xcb, 0xcf, 0xcc, 0xcf, 0xcb, 0xc1, 0xcc, 0xcb, 0xcf, 0xcb, 0xcc, 0xc1, 0xcc, 0xcf,
    0x10, 0x11, 0x1b, 0x1f, 0x1c, 0x1f, 0x1b, 0x11, 0x1c, 0x1b, 0x1f, 0x1b, 0x1c, 0x11, 0x1c, 0x1f,
    0xc0, 0xc1, 0xcb, 0xcf, 0xcc, 0xcf, 0xcb, 0xc1, 0xcc, 0xcb, 0xcf, 0xcb, 0xcc, 0xc1, 0xcc, 0xcf,
    0xf0, 0xf1, 0xfb, 0xff, 0xfc, 0xff, 0xfb, 0xf1, 0xfc, 0xfb, 0xff, 0xfb, 0xfc, 0xf1, 0xfc, 0xff
};

// static byte _recodeByte(char orgColor){
//     // lower half
//     char h = orgColor & 0b00001111;
//     char ncl = recodeTable[h];
//     // upper half
//     h = orgColor >> 4;
//     char ncu = recodeTable[h] << 4;
//     return ncl | ncu;
// }

//-------------------------------------------------------------- F1
#define DELAY_BW 5
#define DELAY_W 5

#define FRAME_1_SX 0
#define FRAME_1_SY 0
#define FRAME_1_EX 16
#define FRAME_1_EY 8
static void _showFrame1(){
    // load colors - gray
    char x = FRAME_1_SX;
    do {
#assign y FRAME_1_SY
#repeat
        GFX_2_SCR[y + x] = recodeTable[respawnGfxC1[y + x]];
        COLOR_RAM[y + x] = recodeTable[respawnGfxC2[y + x]];
#assign y y + 40
#until y == FRAME_1_EY*40
        x++;
    } while (x != FRAME_1_EX);
#undef y

    vic_waitFrames(DELAY_BW);

    // load colors - white
    x = FRAME_1_SX;
    do {
#assign y FRAME_1_SY
#repeat
        GFX_2_SCR[y + x] = 0x11;
        COLOR_RAM[y + x] = 0x01;
#assign y y + 40
#until y == FRAME_1_EY*40
        x++;
    } while (x != FRAME_1_EX);
#undef y

    vic_waitFrames(DELAY_W);

    // load colors - color
    x = FRAME_1_SX;
    do {
#assign y FRAME_1_SY
#repeat
        GFX_2_SCR[y + x] = respawnGfxC1[y + x];
        COLOR_RAM[y + x] = respawnGfxC2[y + x];
#assign y y + 40
#until y == FRAME_1_EY*40
        x++;
    } while (x != FRAME_1_EX);
#undef y
}

//-------------------------------------------------------------- F2
#define FRAME_2_SX 16
#define FRAME_2_SY 0
#define FRAME_2_EX FRAME_2_SX+15
#define FRAME_2_EY 9
static void _showFrame2(){
    // load colors - gray
    char x = FRAME_2_SX;
    do {
#assign y FRAME_2_SY
#repeat
        GFX_2_SCR[y + x] = recodeTable[respawnGfxC1[y + x]];
        COLOR_RAM[y + x] = recodeTable[respawnGfxC2[y + x]];
#assign y y + 40
#until y == FRAME_2_EY*40
        x++;
    } while (x != FRAME_2_EX);
#undef y

    vic_waitFrames(DELAY_BW*8);

    // load colors - white
    x = FRAME_2_SX;
    do {
#assign y FRAME_2_SY
#repeat
        GFX_2_SCR[y + x] = 0x11;
        COLOR_RAM[y + x] = 0x01;
#assign y y + 40
#until y == FRAME_2_EY*40
        x++;
    } while (x != FRAME_2_EX);
#undef y

    vic_waitFrames(DELAY_W);

    // load colors - color
    x = FRAME_2_SX;
    do {
#assign y FRAME_2_SY
#repeat
        GFX_2_SCR[y + x] = respawnGfxC1[y + x];
        COLOR_RAM[y + x] = respawnGfxC2[y + x];
#assign y y + 40
#until y == FRAME_2_EY*40
        x++;
    } while (x != FRAME_2_EX);
#undef y
}

//-------------------------------------------------------------- F3
#define FRAME_3_SX 31
#define FRAME_3_SY 0
#define FRAME_3_EX FRAME_3_SX+9
#define FRAME_3_EY 9
static void _showFrame3bw(){
    // load colors - gray
    char x = FRAME_3_SX;
    do {
#assign y FRAME_3_SY
#repeat
        GFX_2_SCR[y + x] = recodeTable[respawnGfxC1[y + x]];
        COLOR_RAM[y + x] = recodeTable[respawnGfxC2[y + x]];
#assign y y + 40
#until y == FRAME_3_EY*40
        x++;
    } while (x != FRAME_3_EX);
#undef y
}
static void _showFrame3w(){
    // load colors - white
    char x = FRAME_3_SX;
    do {
#assign y FRAME_3_SY
#repeat
        GFX_2_SCR[y + x] = 0x11;
        COLOR_RAM[y + x] = 0x01;
#assign y y + 40
#until y == FRAME_3_EY*40
        x++;
    } while (x != FRAME_3_EX);
#undef y
}
static void _showFrame3b(){
    // load colors - white
    char x = FRAME_3_SX;
    do {
#assign y FRAME_3_SY
#repeat
        GFX_2_SCR[y + x] = 0x00;
        COLOR_RAM[y + x] = 0x01;
#assign y y + 40
#until y == FRAME_3_EY*40
        x++;
    } while (x != FRAME_3_EX);
#undef y
}
static void _showFrame3(){
    // 3
    _showFrame3bw();
    vic_waitFrames(5);
    _showFrame3w();
    vic_waitFrames(8);
    // 2
    _showFrame3b();
    vic_waitFrames(4);
    _showFrame3w();
    vic_waitFrames(2);
    _showFrame3b();
    vic_waitFrames(2);
    _showFrame3w();
    vic_waitFrames(1);
    // 1
    _showFrame3bw();
    vic_waitFrames(8);
    _showFrame3w();
    vic_waitFrames(1);

    // load colors - color
    char x = FRAME_3_SX;
    do {
#assign y FRAME_3_SY
#repeat
        GFX_2_SCR[y + x] = respawnGfxC1[y + x];
        COLOR_RAM[y + x] = respawnGfxC2[y + x];
#assign y y + 40
#until y == FRAME_3_EY*40
        x++;
    } while (x != FRAME_3_EX);
#undef y
}

//-------------------------------------------------------------- F4
#define FRAME_4_SX 0
#define FRAME_4_SY 8*40
#define FRAME_4_EX 20
#define FRAME_4_EY 25
static void _showFrame4(){
    // load colors - gray
    char x = FRAME_4_SX;
    do {
#assign y FRAME_4_SY
#repeat
        GFX_2_SCR[y + x] = recodeTable[respawnGfxC1[y + x]];
        COLOR_RAM[y + x] = recodeTable[respawnGfxC2[y + x]];
#assign y y + 40
#until y == FRAME_4_EY*40
        x++;
    } while (x != FRAME_4_EX);
#undef y

    vic_waitFrames(DELAY_BW);

    // load colors - white
    x = FRAME_4_SX;
    do {
#assign y FRAME_4_SY
#repeat
        GFX_2_SCR[y + x] = 0x11;
        COLOR_RAM[y + x] = 0x01;
#assign y y + 40
#until y == FRAME_4_EY*40
        x++;
    } while (x != FRAME_4_EX);
#undef y

    vic_waitFrames(DELAY_W);

    // load colors - color
    x = FRAME_4_SX;
    do {
#assign y FRAME_4_SY
#repeat
        GFX_2_SCR[y + x] = respawnGfxC1[y + x];
        COLOR_RAM[y + x] = respawnGfxC2[y + x];
#assign y y + 40
#until y == FRAME_4_EY*40
        x++;
    } while (x != FRAME_4_EX);
#undef y
}

//-------------------------------------------------------------- F5
#define FRAME_5_SX 20
#define FRAME_5_SY 9*40
#define FRAME_5_EX 40
#define FRAME_5_EY 25
static void _showFrame5(){
    // load colors - gray
    char x = FRAME_5_SX;
    do {
#assign y FRAME_5_SY
#repeat
        GFX_2_SCR[y + x] = recodeTable[respawnGfxC1[y + x]];
        COLOR_RAM[y + x] = recodeTable[respawnGfxC2[y + x]];
#assign y y + 40
#until y == FRAME_5_EY*40
        x++;
    } while (x != FRAME_5_EX);
#undef y

    vic_waitFrames(DELAY_BW);

    // load colors - white
    x = FRAME_5_SX;
    do {
#assign y FRAME_5_SY
#repeat
        GFX_2_SCR[y + x] = 0x11;
        COLOR_RAM[y + x] = 0x01;
#assign y y + 40
#until y == FRAME_5_EY*40
        x++;
    } while (x != FRAME_5_EX);
#undef y

    vic_waitFrames(DELAY_W);

    // load colors - color
    x = FRAME_5_SX;
    do {
#assign y FRAME_5_SY
#repeat
        GFX_2_SCR[y + x] = respawnGfxC1[y + x];
        COLOR_RAM[y + x] = respawnGfxC2[y + x];
#assign y y + 40
#until y == FRAME_5_EY*40
        x++;
    } while (x != FRAME_5_EX);
#undef y
}

static void _showFrames(){
    clearMsxSync();
    _loadBmpOnly();
    waitForMsxSync();
    _showFrame1();
    waitForMsxSync();
    _showFrame2();
    waitForMsxSync();
    _showFrame3();
    waitForMsxSync();
    _showFrame4();
    waitForMsxSync();
    _showFrame5();
    waitForMsxSync();

}

const struct MenuOption RESPAWN_MENU[] = {
    // { TXT_IDX_MENU_EXIT, ' ', SCREEN_MC_GFX, UI_LF+UI_HIDE, &_showFrames, 0, 2, 11},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_TRANSITION, UI_LF+UI_HIDE, &revertPreviousMenu, 0, 2, 11},
    END_MENU_CHOICES
};

static void _menuHandler(void) {
    gms_disableTimeControls = true;
    gms_gameSpeed = SPEED_PAUSED;
    updateGameSpeed();
    
    // clean sprites
    clearStatusBar();
    memset(SPRITE_BLOCK_POINTER, 0, 0x40*8);
    memcpy(SPRITE_BLOCK_POINTER+3*64, respawnSprites, 3*64);
    _showSprites = false;
    loadMenuGfx();
    playSong(RESPAWN_SONG);
    switchScreenTo(SCREEN_MC_GFX);

    _showFrames();

    allCharacters[0].energy = 50;
    unsigned long respawnCost = lmuldiv16u(GS.cash, 10, 100);
    GS.cash -= respawnCost;
    updateMoney();

    char str[10];
    char format[5] = "%7d";
    format[3] = 28; // zl
    format[4] = 0;

    sprintf(str, format, -respawnCost);
    textToSpriteAt(str, 3, SPRITE_BLOCK_POINTER, 0, 1);

    vic_waitFrames(50);
    _showSprites = true;
    // vic_waitFrames(200);

    displayMenu(RESPAWN_MENU);
    updateStatusBarError(TXT[SB_IDX_MENU_RESPAWN]);

    // wait for space
    // do { keyb_poll(); rand();} while (!keyb_key);
    // revertPreviousMenu();
}

#pragma data(respawnLoaderData)

// do nothing - for menu handling
void _localNoop() {
    return;
}
__interrupt void _localSpriteNoop() {
    return;
}

// this menu DOES NOT have shared menu code from menuSystem.c
__export static const Loaders menuLoaders = {
    .loadMenuCode    = &_localNoop,
    .loadMenuGfx     = &_loadBmpOnly,
    .loadMenuSprites = &_localNoop,
    .showMenu        = &_menuHandler,
    // .showSprites     = &_localSpriteNoop,
    .showSprites     = &_menuShowSprites,
    .updateMenu      = &_localNoop,
    .runMenuLoop     = &_localNoop,
};

// Switching code generation back to shared section
#pragma code(code)
#pragma data(data)
