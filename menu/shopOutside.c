#include <c64/vic.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>

#include <menu/menuSystem.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/uiHandler.h>
#include <engine/gameSettings.h>

#define SHOP_OPEN_WEEK_O1 7
#define SHOP_OPEN_WEEK_C1 11
#define SHOP_OPEN_WEEK_O2 15
#define SHOP_OPEN_WEEK_C2 19

// Sections and regions
#pragma section( shopOutLoaderData, 0 )
#pragma section( shopOutCode, 0 )
#pragma section( shopOutGfxDay, 0 )
#pragma section( shopOutGfxNight, 0 )
#pragma region( bankShopOutC, 0x8000, 0x9000, , MENU_BANK_SHOP, { shopOutLoaderData, shopOutCode } )
#pragma region( bankShopOutG1, DAY_GFX_BMP, DAY_GFX_BMP+0x1400, , MENU_BANK_SHOP, { shopOutGfxDay } )
#pragma region( bankShopOutG2, NIGHT_GFX_BMP, NIGHT_GFX_BMP+0x1400, , MENU_BANK_SHOP, { shopOutGfxNight } )

#pragma data ( shopOutGfxDay )
__export const char shopOutGfx1[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/shopOutsideCropped.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/shopOutsideCropped.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/shopOutsideCropped.kla"
};
#pragma data ( shopOutGfxNight )
__export const char shopOutGfx2[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/shopOutsideCropped.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/shopOutsideCropped.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/shopOutsideCropped.kla"
};

// menu code is in ROM - data in RAM
#pragma code ( shopOutCode )
#pragma data ( mainData )

static bool _checkIfOpen(){
    // weekdays
    if(cal_dateHour >= SHOP_OPEN_WEEK_O1)
        if(cal_dateHour < SHOP_OPEN_WEEK_C1)
            return true;

    if(cal_dateHour >= SHOP_OPEN_WEEK_O2)
        if(cal_dateHour < SHOP_OPEN_WEEK_C2)
            return true;

    return false;
}

// Switching code generation back to shared section
// this method must operate from RAM, as it switches banks
#pragma code ( mainCode )
#pragma data ( mainData )
static void _getInside(){
    if(!_checkIfOpen()){
        updateStatusBar(TXT[SB_IDX_SHOP_CLOSED]);
        setErrorCursor();
        return;
    }
    joyCursor.enabled = false;
    gms_disableTimeControls = true;
    gms_gameSpeed = SPEED_PAUSED;
    updateGameSpeed();
    loadMenu(MENU_BANK_SHOP_IN);
    showMenu();
    joyCursor.enabled = true;
}

// menu code is in ROM - data in RAM
#pragma code ( shopOutCode )
#pragma data ( mainData )

static void _soMenu2(){
    updateStatusBar("   Sklep menu, opcja 2");
}

const struct MenuOption SHOP_OUTSIDE_MENU[] = {
    { TXT_IDX_MENU_SHOPOUT1, '1', UI_LR, &_getInside, 0, 1, 4},
    { TXT_IDX_MENU_SHOPOUT2, '2', UI_LR, &_soMenu2, 0, 1, 5},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, UI_LR, &showMenu, MENU_BANK_MAIN_MENU, 2, 7},
    END_MENU_CHOICES
};

static void _menuHandler(void){
    loadMenuGfx(cal_isDay);

    // Prepare output window
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, SCREEN_Y_START, SCREEN_WIDTH, SCREEN_HEIGHT);
    cwin_clear(&cw);
    cwin_putat_string_raw(&cw, 0, 0, TXT[TXT_IDX_SHOP_DESCRIPTION], VCOL_GREEN);
    
    gms_disableTimeControls = false;
    displayMenu(SHOP_OUTSIDE_MENU);
}

#pragma data ( shopOutLoaderData )

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &menuNoop,
    .loadMenuGfx     = &menuGfxLoader,
    .loadMenuSprites = &menuSpriteLoader,
    .showMenu        = &_menuHandler,
    .showSprites     = &menuNoop,
    .updateMenu      = &menuNoop,
}

// Switching code generation back to shared section
#pragma code ( mainCode )
#pragma data ( mainData )
