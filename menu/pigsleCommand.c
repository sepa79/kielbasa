#include <c64/vic.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>

#include <menu/menuSystem.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/uiHandler.h>

// Sections and regions
#pragma section( pigsleCommandLoaderData, 0 )
#pragma section( pigsleCommandCode, 0 )
#pragma section( pigsleCommandGfxDay, 0 )
#pragma section( pigsleCommandGfxNight, 0 )
#pragma region( bankpigsleCommandC, 0x8000, 0x9000, , MENU_BANK_PIGSLE_COMMAND, { pigsleCommandLoaderData, pigsleCommandCode } )
#pragma region( bankpigsleCommandG1, DAY_GFX_BMP, DAY_GFX_BMP+0x1400, , MENU_BANK_PIGSLE_COMMAND, { pigsleCommandGfxDay } )
#pragma region( bankpigsleCommandG2, NIGHT_GFX_BMP, NIGHT_GFX_BMP+0x1400, , MENU_BANK_PIGSLE_COMMAND, { pigsleCommandGfxNight } )

#pragma data ( pigsleCommandGfxDay )
__export const char pigsleCommandGfx1[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/flakJurasNauczSieKurwaGita.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/flakJurasNauczSieKurwaGita.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/flakJurasNauczSieKurwaGita.kla"
};
#pragma data ( pigsleCommandGfxNight )
__export const char pigsleCommandGfx2[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/flak_88_10.08.22.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/flak_88_10.08.22.kla"
    #embed 0x01df 0x232a "assets/multicolorGfx/flak_88_10.08.22.kla"
};

// menu code is in ROM - data in RAM
#pragma code ( pigsleCommandCode )
#pragma data ( data )

static void _shMenu1(){
    updateStatusBar("   pigsleCommand menu, opcja 1");
}
static void _shMenu2(){
    updateStatusBar("   pigsleCommand menu, opcja 2");
}
static void _shMenu3(){
    updateStatusBar("   pigsleCommand menu, opcja 3");
}

const struct MenuOption PIGSLE_COMMAND_MENU[] = {
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, UI_LR, &showMenu, MENU_BANK_MAIN_MENU, 2, 5},
    END_MENU_CHOICES
};

static void _menuHandler(void){
    loadMenuGfx(cal_isDay);

    // Prepare output window
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, SCREEN_Y_START, SCREEN_WIDTH, SCREEN_HEIGHT);
    cwin_clear(&cw);
    
    displayMenu(PIGSLE_COMMAND_MENU);
}

#pragma data ( pigsleCommandLoaderData )

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
