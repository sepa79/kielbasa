#include <c64/vic.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>

#include <menu/menuSystem.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/uiHandler.h>

// Sections and regions
#pragma section( smokeHouseLoaderData, 0 )
#pragma section( smokeHouseCode, 0 )
#pragma section( smokeHouseGfxDay, 0 )
#pragma region( regionSmokeHouseC, 0x8000, 0x9000, , MENU_BANK_SMOKE_HOUSE, { smokeHouseLoaderData, smokeHouseCode } )
#pragma region( regionSmokeHouseG1, DAY_GFX_BMP, DAY_GFX_BMP+0x1400, , MENU_BANK_SMOKE_HOUSE, { smokeHouseGfxDay } )

#pragma data ( smokeHouseGfxDay )
__export const char smokeHouseGfx1[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/wedzarnia_27.10.22.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/wedzarnia_27.10.22.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/wedzarnia_27.10.22.kla"
};

// menu code is in ROM - data in RAM
#pragma code ( smokeHouseCode )
#pragma data ( data )

static void _shMenu1(){
    updateStatusBar("   smokeHouse menu, opcja 1");
}
static void _shMenu2(){
    updateStatusBar("   smokeHouse menu, opcja 2");
}
static void _shMenu3(){
    updateStatusBar("   smokeHouse menu, opcja 3");
}

const struct MenuOption SMOKE_HOUSE_MENU[] = {
    { TXT_IDX_MENU_SMOKEHOUSE1, '1', UI_SELECT, &_shMenu1, 0, 1, 1},
    { TXT_IDX_MENU_SMOKEHOUSE2, '2', UI_SELECT, &_shMenu2, 0, 1, 2},
    { TXT_IDX_MENU_SMOKEHOUSE3, '3', UI_SELECT, &_shMenu3, 0, 1, 3},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, UI_SELECT, &showMenu, MENU_BANK_MAIN_MENU, 2, 5},
    END_MENU_CHOICES
};

static void _menuHandler(void){
    mnu_isGfxLoaded = false;
    loadMenuGfx(true);

    // Prepare output window
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, SCREEN_Y_START, SCREEN_WIDTH, SCREEN_HEIGHT);
    cwin_clear(&cw);
    
    displayMenu(SMOKE_HOUSE_MENU);
}

#pragma data ( smokeHouseLoaderData )

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &menuNoop,
    .loadMenuGfx     = &menuGfxLoaderSingleBitmap,
    .loadMenuSprites = &menuSpriteLoader,
    .showMenu        = &_menuHandler,
    .showSprites     = &spriteNoop,
    .updateMenu      = &menuNoop,
};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
