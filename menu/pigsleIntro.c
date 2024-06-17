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

#include <menu/pigsleIntro.h>

#pragma data(pigsleIntroMedia)
__export const char pigsleIntroGfx[] = {
    #embed 8000 2 rle "assets/multicolorGfx/pigsleIntro.kla"
};
__export const char pigsleIntroGfxC1[] = {
    #embed 1000 0x1f42 "assets/multicolorGfx/pigsleIntro.kla"
};
__export const char pigsleIntroGfxC2[] = {
    #embed 1000 0x232a "assets/multicolorGfx/pigsleIntro.kla"
};

// menu code is in ROM - data in RAM
#pragma code(pigsleIntroCode)
#pragma data(pigsleIntroData)

static void _loadBmpOnly() {
    memset(GFX_2_SCR, 0, 1000);
    memset(COLOR_RAM, 0, 1000);
    oscar_expand_rle(GFX_2_BMP, pigsleIntroGfx);
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

static const char recodeTable[] = {
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

//-------------------------------------------------------------- F1
#define DELAY_BW 5
#define DELAY_W 5

#define FRAME_1_SX 0
#define FRAME_1_SY 0
#define FRAME_1_EX 18
#define FRAME_1_EY 10
static void _showFrame1(){
    // load colors - gray
    char x = FRAME_1_SX;
    do {
#assign y FRAME_1_SY
#repeat
        GFX_2_SCR[y + x] = recodeTable[pigsleIntroGfxC1[y + x]];
        COLOR_RAM[y + x] = recodeTable[pigsleIntroGfxC2[y + x]];
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
        GFX_2_SCR[y + x] = pigsleIntroGfxC1[y + x];
        COLOR_RAM[y + x] = pigsleIntroGfxC2[y + x];
#assign y y + 40
#until y == FRAME_1_EY*40
        x++;
    } while (x != FRAME_1_EX);
#undef y
}

//-------------------------------------------------------------- F2
#define FRAME_2_SX 20
#define FRAME_2_SY 0
#define FRAME_2_EX 40
#define FRAME_2_EY 12
static void _showFrame2(){
    // load colors - gray
    char x = FRAME_2_SX;
    do {
#assign y FRAME_2_SY
#repeat
        GFX_2_SCR[y + x] = recodeTable[pigsleIntroGfxC1[y + x]];
        COLOR_RAM[y + x] = recodeTable[pigsleIntroGfxC2[y + x]];
#assign y y + 40
#until y == FRAME_2_EY*40
        x++;
    } while (x != FRAME_2_EX);
#undef y

    vic_waitFrames(DELAY_BW);

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
        GFX_2_SCR[y + x] = pigsleIntroGfxC1[y + x];
        COLOR_RAM[y + x] = pigsleIntroGfxC2[y + x];
#assign y y + 40
#until y == FRAME_2_EY*40
        x++;
    } while (x != FRAME_2_EX);
#undef y
}

//-------------------------------------------------------------- F3
#define FRAME_3_SX 0
#define FRAME_3_SY 10*40
#define FRAME_3_EX 20
#define FRAME_3_EY 24
static void _showFrame3(){
    // load colors - gray
    char x = FRAME_3_SX;
    do {
#assign y FRAME_3_SY
#repeat
        GFX_2_SCR[y + x] = recodeTable[pigsleIntroGfxC1[y + x]];
        COLOR_RAM[y + x] = recodeTable[pigsleIntroGfxC2[y + x]];
#assign y y + 40
#until y == FRAME_3_EY*40
        x++;
    } while (x != FRAME_3_EX);
#undef y

    vic_waitFrames(DELAY_BW);

    // load colors - white
    x = FRAME_3_SX;
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

    vic_waitFrames(DELAY_W);

    // load colors - color
    x = FRAME_3_SX;
    do {
#assign y FRAME_3_SY
#repeat
        GFX_2_SCR[y + x] = pigsleIntroGfxC1[y + x];
        COLOR_RAM[y + x] = pigsleIntroGfxC2[y + x];
#assign y y + 40
#until y == FRAME_3_EY*40
        x++;
    } while (x != FRAME_3_EX);
#undef y
}

//-------------------------------------------------------------- F4
#define FRAME_4_SX 20
#define FRAME_4_SY 12*40
#define FRAME_4_EX 40
#define FRAME_4_EY 24
static void _showFrame4(){
    // load colors - gray
    char x = FRAME_4_SX;
    do {
#assign y FRAME_4_SY
#repeat
        GFX_2_SCR[y + x] = recodeTable[pigsleIntroGfxC1[y + x]];
        COLOR_RAM[y + x] = recodeTable[pigsleIntroGfxC2[y + x]];
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
        GFX_2_SCR[y + x] = pigsleIntroGfxC1[y + x];
        COLOR_RAM[y + x] = pigsleIntroGfxC2[y + x];
#assign y y + 40
#until y == FRAME_4_EY*40
        x++;
    } while (x != FRAME_4_EX);
#undef y
}

static void _showFrames(){
    // clearMsxSync();
    _loadBmpOnly();

    vic_waitFrames(2*50+30);

    _showFrame1();
    vic_waitFrames(3*50);

    _showFrame4();
    vic_waitFrames(4*50+15);

    _showFrame2();
    vic_waitFrames(1*50);

    _showFrame3();

}

const struct MenuOption PIGSLE_INTRO_MENU[] = {
    // { TXT_IDX_MENU_EXIT, ' ', SCREEN_MC_GFX, UI_LF+UI_HIDE, &_showFrames, 0, 2, 11},
    // { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_TRANSITION, UI_LF+UI_HIDE, &revertPreviousMenu, 0, 2, 11},
    END_MENU_CHOICES
};

static void _menuHandler(void) {
    gms_disableTimeControls = true;
    gms_gameSpeed = SPEED_PAUSED;
    updateGameSpeed();

    // clean sprites
    clearStatusBar();

    loadMenuGfx();
    startSong(PIGSLE_INTRO_SONG);
    switchScreenTo(SCREEN_MC_GFX);

    _showFrames();

    displayMenu(PIGSLE_INTRO_MENU);
    updateStatusBarError(TXT[SB_IDX_MENU_RESPAWN]);
    // updateStatusBarError(TXT[SB_IDX_MENU_PIGSLE_INTRO]);

    // wait for any key, once we exit here the event code is going to reload main menu
    do { keyb_poll(); rand();} while (!keyb_key);
}

#pragma data(pigsleIntroLoaderData)

// do nothing - for menu handling
static void _localNoop() {
    return;
}

__interrupt static void _localSpriteNoop() {
    return;
}

// this menu DOES NOT have shared menu code from menuSystem.c
__export static const Loaders menuLoaders = {
    .loadMenuCode    = &_localNoop,
    .loadMenuGfx     = &_localNoop,
    .loadMenuSprites = &_localNoop,
    .showMenu        = &_menuHandler,
    .showSprites     = &_localSpriteNoop,
    .updateMenu      = &_localNoop,
    .runMenuLoop     = &_localNoop,
};

// Switching code generation back to shared section
#pragma code(code)
#pragma data(data)
