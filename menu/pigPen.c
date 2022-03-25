#include <c64/vic.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>

#include <menu/menuSystem.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/uiHandler.h>

// Sections and regions
#pragma section( pigPenLoaderData, 0 )
#pragma section( pigPenCode, 0 )
#pragma section( pigPenGfxDay, 0 )
#pragma section( pigPenGfxNight, 0 )
#pragma region( bankPigPenC, 0x8000, 0x9000, , MENU_BANK_PIG_PEN, { pigPenLoaderData, pigPenCode } )
#pragma region( bankPigPenG1, DAY_GFX_BMP, DAY_GFX_BMP+0x1400, , MENU_BANK_PIG_PEN, { pigPenGfxDay } )
#pragma region( bankPigPenG2, NIGHT_GFX_BMP, NIGHT_GFX_BMP+0x1400, , MENU_BANK_PIG_PEN, { pigPenGfxNight } )

#pragma data ( pigPenGfxDay )
__export const char pigPenGfx1[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/pigPen.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/pigPen.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/pigPen.kla"
};
#pragma data ( pigPenGfxNight )
__export const char pigPenGfx2[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/pigPen.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/pigPen.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/pigPen.kla"
};

// menu code is in ROM - data in RAM
#pragma code ( pigPenCode )
#pragma data ( data )

static void _ppMenu1(){
    updateStatusBar("   \xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0Chlewik menu, opcja 1");
}
static void _ppMenu2(){
    updateStatusBar("   Chlewik menu, opcja 2");
}
static void _ppMenu3(){
    updateStatusBar("   Chlewik menu, opcja 3");
}

const struct MenuOption PIG_PEN_MENU[] = {
    { TXT_IDX_MENU_PIGPEN1, '1', &_ppMenu1, 0, 1, 1},
    { TXT_IDX_MENU_PIGPEN2, '2', &_ppMenu2, 0, 1, 2},
    { TXT_IDX_MENU_PIGPEN3, '3', &_ppMenu3, 0, 1, 3},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, &showMenu, MENU_BANK_MAIN_MENU, 2, 5},
    END_MENU_CHOICES
};

static void _menuHandler(void){
    loadMenuGfx(true);

    // Prepare output window
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, SCREEN_Y_START, SCREEN_WIDTH, SCREEN_HEIGHT);
    cwin_clear(&cw);
    
    displayMenu(PIG_PEN_MENU);
}

#pragma data ( pigPenLoaderData )

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &menuNoop,
    .loadMenuGfx     = &menuGfxLoader,
    .loadMenuSprites = &menuSpriteLoader,
    .showMenu        = &_menuHandler,
    .showSprites     = &menuNoop,
    .updateMenu      = &menuNoop,
}

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )