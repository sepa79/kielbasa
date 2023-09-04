#include <c64/vic.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>

#include <menu/menuSystem.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/uiHandler.h>

// Sections and regions
#pragma section(churchMenuLoaderData, 0)
#pragma section(churchMenuCode, 0)
#pragma section(churchMenuGfxDay, 0)
#pragma section(churchMenuSprites, 0)
#pragma region(bankChurchMenuC, 0x8000, 0x9000, , MENU_BANK_CHURCH_MENU, { churchMenuLoaderData, churchMenuCode })
#pragma region(bankChurchMenuG1, DAY_GFX_BMP, DAY_GFX_BMP + 0x1400, , MENU_BANK_CHURCH_MENU, { churchMenuGfxDay })
#pragma region(bankChurchMenuG3, MENU_SPRITE_SRC, MENU_SPRITE_SRC + 0x0400, , MENU_BANK_CHURCH_MENU, { churchMenuSprites })

#pragma data(churchMenuGfxDay)
__export const char churchMenuGfx1[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/church_300823.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/church_300823.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/church_300823.kla"
};

// menu code is in ROM - data in RAM
#pragma code(churchMenuCode)
#pragma data(data)

// Function declarations for churchMenu options
static void _cmMenu1();
static void _cmMenu2();
static void _cmMenu3();

const struct MenuOption CHURCH_MENU[] = {
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

    displayMenu(CHURCH_MENU);
    switchScreenTo(SCREEN_SPLIT_MC_TXT);
}

#pragma data(churchMenuLoaderData)

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
