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
    #embed 8000 2 rle "assets/multicolorGfx/staminacritical.kla"
};
__export const char respawnGfxC1[] = {
    #embed 1000 0x1f42 "assets/multicolorGfx/staminacritical.kla"
};
__export const char respawnGfxC2[] = {
    #embed 1000 0x232a "assets/multicolorGfx/staminacritical.kla"
};
// // just the colors
// __export const char respawnGfxBW1[] = {
//     #embed 1000 0x1f42 "assets/multicolorGfx/staminacritical_bw.kla"
// };
// __export const char respawnGfxBW2[] = {
//     #embed 1000 0x232a "assets/multicolorGfx/staminacritical_bw.kla"
// };

// menu code is in ROM - data in RAM
#pragma code(respawnCode)
#pragma data(data)

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
#define DELAY_F 20
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
    _loadBmpOnly();
    vic_waitFrames(DELAY_F);
    _showFrame1();
    vic_waitFrames(DELAY_F);
    _showFrame2();
    vic_waitFrames(DELAY_F);
    _showFrame3();
    vic_waitFrames(DELAY_F);
    _showFrame4();
    vic_waitFrames(DELAY_F);
    _showFrame5();

//     // load colors
//     char i = 0;
//     do {
// #assign y 0
// #repeat
//         GFX_2_SCR[y + i] = _recodeByte(respawnGfxC1[y + i]);
//         COLOR_RAM[y + i] = recodeTable[respawnGfxC2[y + i]];
// #assign y y + 256
// #until y == 1024
//         i++;
//     } while (i != 0);
// #undef y

    // memcpy(GFX_2_SCR, respawnGfxBW1, 1000);
    // memcpy(COLOR_RAM, respawnGfxBW2, 1000);
}

const struct MenuOption RESPAWN_MENU[] = {
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_MC_GFX, UI_LF+UI_HIDE, &_showFrames, 0, 2, 11},
    END_MENU_CHOICES
};

static void _menuHandler(void) {
    gms_disableTimeControls = true;
    gms_gameSpeed = SPEED_PAUSED;
    updateGameSpeed();
    
    loadMenuGfx();
    switchScreenTo(SCREEN_MC_GFX);

    _showFrames();

    displayMenu(RESPAWN_MENU);
    // updateStatusBar(TXT[SB_IDX_MENU_RESPAWN]);
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
    .showSprites     = &_localSpriteNoop,
    .updateMenu      = &_localNoop,
    .runMenuLoop     = &_localNoop,
};

// Switching code generation back to shared section
#pragma code(code)
#pragma data(data)
