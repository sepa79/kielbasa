#include <c64/vic.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>

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
#pragma region(regionRespawnG1, DAY_GFX_BMP, DAY_GFX_BMP + 0x2711, , MENU_BANK_RESPAWN, { respawnGfx })
// #pragma region(regionRespawnG2, DAY_GFX_BMP + 0x2711, DAY_GFX_BMP + 0x2711 + 1024, , MENU_BANK_RESPAWN, { respawnFnt })

#pragma data(respawnGfx)
__export const char respawnGfx1[] = {
    #embed 0xffff 2 "assets/multicolorGfx/respawn.kla"
};

#pragma code(respawnCode)
#pragma data(data)

const struct MenuOption RESPAWN_MENU[] = {
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_TRANSITION, UI_LF+UI_HIDE, &revertPreviousMenu, 0, 2, 11},
    END_MENU_CHOICES
};

static void _menuHandler(void) {
    gms_disableTimeControls = true;
    gms_gameSpeed = SPEED_PAUSED;
    updateGameSpeed();
    
    loadMenuGfx();
    switchScreenTo(SCREEN_MC_GFX);

    displayMenu(RESPAWN_MENU);
    // updateStatusBar(TXT[SB_IDX_MENU_RESPAWN]);
}

#pragma data(respawnLoaderData)

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &menuNoop,
    .loadMenuGfx     = &loadFullKoalaToBMP2,
    .loadMenuSprites = &menuSpriteLoader,
    .showMenu        = &_menuHandler,
    .showSprites     = &spriteNoop,
    .updateMenu      = &menuNoop,
    .runMenuLoop     = &menuNoop,
};

// Switching code generation back to shared section
#pragma code(code)
#pragma data(data)
