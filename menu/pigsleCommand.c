#include <c64/vic.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>
#include <c64/easyflash.h>
#include <c64/sprites.h>

#include <menu/menuSystem.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/uiHandler.h>
#include <engine/irqHandler.h>
#include <engine/gameSettings.h>

#include <menu/pigsleCommand.h>
// #include <miniGame/pigsleCmdMain.h>

// ---------------------------------------------------------------------------------------------
// Menu code
// ---------------------------------------------------------------------------------------------
#pragma code ( pigsleCommandCode )
#pragma data ( data )

const struct MenuOption PIGSLE_COMMAND_MENU[] = {
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_SPLIT_MC_TXT, UI_LF+UI_HIDE, &showMenu, MENU_BANK_HERMANS_HOUSE, 2, 5},
    END_MENU_CHOICES
};

static void _pigsleCmdCodeLoader(){
    // source is where the regionPigsleCommandRam section starts in real mem
    memcpy(MENU_CODE_DST, (char *)0xb000, 0x1000);
}

// Noop - just return - to satisfy menu handlers
static void _pigsleCmdNoop(){
    return;
}
__interrupt static void _pigsleSpriteCmdNoop(){
    return;
}

#pragma code ( pigsleCommandRAMCode )
#pragma data ( pigsleCommandRAMData )

static void _pigsleCmdInit(void){
    vic.color_border++;
    displayMenu(PIGSLE_COMMAND_MENU);

    // switchScreenTo(SCREEN_PIGSLE);

    // pigsleCmdInit();
    // TODO: return to main menu
}

static void _pigsleCmdLoop(void){
    // main loop
    vic.color_border++;

    // gameLoop();
    // vic_waitFrame();
    // rirq_wait();
    vic.color_border--;

}

#pragma data ( pigsleCommandLoaderData )

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &_pigsleCmdCodeLoader,
    .loadMenuGfx     = nullptr,
    .loadMenuSprites = &menuNoop,
    .showMenu        = &_pigsleCmdInit,
    .showSprites     = &spriteNoop,
    .updateMenu      = &menuNoop,
    .runMenuLoop     = &_pigsleCmdLoop,
};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
