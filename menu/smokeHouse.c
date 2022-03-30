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
#pragma section( smokeHouseGfxNight, 0 )
#pragma region( bankSmokeHouseC, 0x8000, 0x9000, , MENU_BANK_SMOKE_HOUSE, { smokeHouseLoaderData, smokeHouseCode } )
#pragma region( bankSmokeHouseG1, DAY_GFX_BMP, DAY_GFX_BMP+0x1400, , MENU_BANK_SMOKE_HOUSE, { smokeHouseGfxDay } )
#pragma region( bankSmokeHouseG2, NIGHT_GFX_BMP, NIGHT_GFX_BMP+0x1400, , MENU_BANK_SMOKE_HOUSE, { smokeHouseGfxNight } )

#pragma data ( smokeHouseGfxDay )
__export const char smokeHouseGfx1[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/smokehouse.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/smokehouse.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/smokehouse.kla"
};
#pragma data ( smokeHouseGfxNight )
__export const char smokeHouseGfx2[] = {
    #embed 0x0100 0x0002 "assets/multicolorGfx/smokehouse.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/smokehouse.kla"
    #embed 0x01df 0x232a "assets/multicolorGfx/smokehouse.kla"
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
    { TXT_IDX_MENU_SMOKEHOUSE1, '1', &_shMenu1, 0, 1, 1},
    { TXT_IDX_MENU_SMOKEHOUSE2, '2', &_shMenu2, 0, 1, 2},
    { TXT_IDX_MENU_SMOKEHOUSE3, '3', &_shMenu3, 0, 1, 3},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, &showMenu, MENU_BANK_MAIN_MENU, 2, 5},
    END_MENU_CHOICES
};

static void _menuHandler(void){
    loadMenuGfx(cal_isDay);

    // Prepare output window
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, SCREEN_Y_START, SCREEN_WIDTH, SCREEN_HEIGHT);
    cwin_clear(&cw);
    
    displayMenu(SMOKE_HOUSE_MENU);
}

#pragma data ( smokeHouseLoaderData )

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