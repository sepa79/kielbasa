#include <c64/vic.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>

#include <menu/menuSystem.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/uiHandler.h>

// Sections and regions
#pragma section(sulejukowasHouseLoaderData, 0)
#pragma section(sulejukowasHouseCode, 0)
#pragma section(sulejukowasHouseGfxDay, 0)
#pragma section(sulejukowasHouseSprites, 0)
#pragma region(bankSulejukowasHouseC, 0x8000, 0x9000, , MENU_BANK_SULEJUKOWAS_HOUSE, { sulejukowasHouseLoaderData, sulejukowasHouseCode })
#pragma region(bankSulejukowasHouseG1, DAY_GFX_BMP, DAY_GFX_BMP + 0x1400, , MENU_BANK_SULEJUKOWAS_HOUSE, { sulejukowasHouseGfxDay })
#pragma region(bankSulejukowasHouseG3, MENU_SPRITE_SRC, MENU_SPRITE_SRC + 0x0400, , MENU_BANK_SULEJUKOWAS_HOUSE, { sulejukowasHouseSprites })

#pragma data(sulejukowasHouseGfxDay)
__export const char sulejukowasHouseGfx1[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/herman3.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/herman3.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/herman3.kla"
};

// menu code is in ROM - data in RAM
#pragma code(sulejukowasHouseCode)
#pragma data(data)

static void _shMenu1() {
    // Define the functionality for sulejukowasHouse menu option 1
    // ...

}

static void _shMenu2() {
    // Define the functionality for sulejukowasHouse menu option 2
    // ...

}

static void _shMenu3() {
    // Define the functionality for sulejukowasHouse menu option 3
    // ...

}

const struct MenuOption SULEJUKOWA_HOUSE_MENU[] = {
    // { TXT_IDX_MENU_SULEJUKOWA_HOUSE1, '1', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_shMenu1, 0, 1, 1 },
    // { TXT_IDX_MENU_SULEJUKOWA_HOUSE2, '2', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_shMenu2, 0, 1, 2 },
    // { TXT_IDX_MENU_SULEJUKOWA_HOUSE3, '3', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_shMenu3, 0, 1, 3 },
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

    displayMenu(SULEJUKOWA_HOUSE_MENU);
    switchScreenTo(SCREEN_SPLIT_MC_TXT);
}

#pragma data(sulejukowasHouseLoaderData)

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
