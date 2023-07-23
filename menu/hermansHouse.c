#include <c64/vic.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>

#include <menu/menuSystem.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/uiHandler.h>

// Sections and regions
#pragma section(hermansHouseLoaderData, 0)
#pragma section(hermansHouseCode, 0)
#pragma section(hermansHouseGfxDay, 0)
#pragma section(hermansHouseSprites, 0)
#pragma region(bankHermansHouseC, 0x8000, 0x9000, , MENU_BANK_HERMANS_HOUSE, { hermansHouseLoaderData, hermansHouseCode })
#pragma region(bankHermansHouseG1, DAY_GFX_BMP, DAY_GFX_BMP + 0x1400, , MENU_BANK_HERMANS_HOUSE, { hermansHouseGfxDay })
#pragma region(bankHermansHouseG3, MENU_SPRITE_SRC, MENU_SPRITE_SRC + 0x0400, , MENU_BANK_HERMANS_HOUSE, { hermansHouseSprites })

#pragma data(hermansHouseGfxDay)
__export const char hermansHouseGfx1[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/herman140723.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/herman140723.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/herman140723.kla"
};

// menu code is in ROM - data in RAM
#pragma code(hermansHouseCode)
#pragma data(data)

static void _hhMenu1() {
    // Define the functionality for hermansHouse menu option 1
    // ...

}

static void _hhMenu2() {
    // Define the functionality for hermansHouse menu option 2
    // ...

}

static void _hhMenu3() {
    // Define the functionality for hermansHouse menu option 3
    // ...

}

const struct MenuOption HERMANS_HOUSE_MENU[] = {
    // { TXT_IDX_MENU_HERMANS_HOUSE1, '1', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_hhMenu1, 0, 1, 1 },
    // { TXT_IDX_MENU_HERMANS_HOUSE2, '2', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_hhMenu2, 0, 1, 2 },
    // { TXT_IDX_MENU_HERMANS_HOUSE3, '3', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_hhMenu3, 0, 1, 3 },
    { TXT_IDX_MENU_AIR_DEF, '1', SCREEN_TRANSITION, UI_SELECT, &showMenu, MENU_BANK_PIGSLE_COMMAND_1, 1, 2 },
    { TXT_IDX_EXIT_TO_MAP, KEY_ARROW_LEFT, SCREEN_TRANSITION, UI_LF + UI_SELECT, &showMenu, MENU_BANK_MAP_VILLIAGE_1, 2, 5 },
    END_MENU_CHOICES
};

static void _menuHandler() {
    mnu_isGfxLoaded = false;
    loadMenuGfx();
    // loadMenuSprites();

    // Prepare output window
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, SCREEN_Y_START, SCREEN_WIDTH, SCREEN_HEIGHT);
    cwin_clear(&cw);

    displayMenu(HERMANS_HOUSE_MENU);
    switchScreenTo(SCREEN_SPLIT_MC_TXT);
}

#pragma data(hermansHouseLoaderData)

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &menuNoop,
    .loadMenuGfx     = &menuGfxLoaderSingleBitmap,
    .loadMenuSprites = &menuSpriteLoader,
    .showMenu        = &_menuHandler,
    .showSprites     = &spriteNoop,
    .updateMenu      = &menuNoop,
    .runMenuLoop     = &menuNoop,
};

// Switching code generation back to shared section
#pragma code(code)
#pragma data(data)
