#include <c64/vic.h>
#include <c64/cia.h>
#include <c64/charwin.h>
#include <string.h>
#include <c64/keyboard.h>

#include <menu/menuSystem.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/uiHandler.h>
#include <engine/irqHandler.h>
#include <engine/gameSettings.h>

#pragma data(fishingMenuGfxDay)
__export const char fishingMenuGfx1[] = {
    #embed 0xffff 2 "assets/multicolorGfx/fishing.kla"
};
#pragma data(fishingMenuSprites)
__export const char fishingMenuSprites[] = {
    #embed 0xffff 20 "assets/sprites/rybki.spd"
};

#pragma code(fishingMenuRAMCode)
#pragma data(fishingMenuRAMData)

__interrupt static void IRQ_topFishing() {
    // vic.color_border++;
    // Select GFX screen
    vic.ctrl1 = VIC_CTRL1_BMM | VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3;
    vic.ctrl2 = VIC_CTRL2_MCM | VIC_CTRL2_CSEL | 0;
    vic.memptr = d018_gfx1;
    cia2.pra = dd00_gfx1;

    // show any sprites the menu might have
    setSpritesTopScr();

    // indicate frame position
    gms_framePos = FRAME_TOP;

    playMsx();

    // vic.color_border--;
}

__interrupt static void IRQ_bottomUI() {
    // Set screen height to 24 lines - this is done after the border should have started drawing - so it wont start
    vic.ctrl1 &= (0xff^VIC_CTRL1_RSEL);
    while (vic.raster != 0xfc){}
    // reset to normal screen height
    vic.ctrl1 = VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3;
    showUiSpritesBottom();
    // UI sprite bank
    vic.memptr = d018_UI;
    cia2.pra = dd00_UI;

    // vic.color_border++;
    playMsx();
    if(!gms_enableMusic){
        // wait a few lines as msx is off, we don't want to desync the screen.
        while (vic.raster != 0x10){}
    }

    joyUpdate();

}

void initRasterIRQ_Fishing(){
    // vic.color_border--;

    // Top - switch to txt
    rirq_build(&rirqc_topScreen, 1);
    rirq_call(&rirqc_topScreen, 0, IRQ_topFishing);
    rirq_set(1, IRQ_RASTER_TOP_MC_SCREEN, &rirqc_topScreen);

    // Middle - play msx
    // rirq_build(&rirqc_middleScreen, 1);
    // rirq_call(&rirqc_middleScreen, 0, IRQ_middleScreenMsx);
    // rirq_set(2, IRQ_RASTER_MIDDLE_TXT_SCREEN, &rirqc_middleScreen);
    rirq_clear(2);
    // Bottom Sprites

    rirq_build(&rirqc_bottomUI, 1);
    rirq_call(&rirqc_bottomUI, 0, IRQ_bottomUI);
    // Place it into the last line of the screen
    rirq_set(3, IRQ_RASTER_BOTTOM_UI, &rirqc_bottomUI);

    // sort the raster IRQs
    rirq_sort();
}

#pragma code(fishingMenuCode)
#pragma data(data)

// copy fishingMenuRAMCode
static void _fishingMenuCodeLoader(){
    memcpy(MENU_CODE_DST, (char *)0x8800, 0x0800);
}

void fishingMenuSpriteLoader(){
    // save us some trouble, don't overwrite main cursor (+64 below)
    // memcpy((char *)GFX_1_SPR_DST_ADR+64, fishingMenuSprites, 0x0a80);
}

const struct MenuOption FISHING_MENU[] = {
    // Add the "Exit to Map" option as shown in the example
    { TXT_IDX_EXIT_TO_MAP, KEY_ARROW_LEFT, SCREEN_TRANSITION, UI_LF + UI_HIDE, &showMenu, MENU_BANK_MAP_VILLIAGE_1, 2, 5 },
    END_MENU_CHOICES
};

static void _menuHandler() {
    loadMenuGfx();
    loadMenuSprites();

    displayMenu(FISHING_MENU);
    switchScreenTo(SCREEN_FISHING);
}

#pragma data(fishingMenuLoaderData)

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &_fishingMenuCodeLoader,
    .loadMenuGfx     = &loadFullKoalaToBMP2,
    .loadMenuSprites = &fishingMenuSpriteLoader,
    .showMenu        = &_menuHandler,
    .showSprites     = &spriteNoop,
    .updateMenu      = &menuNoop,
    .runMenuLoop     = &menuNoop,
};

// Switching code generation back to shared section
#pragma code(code)
#pragma data(data)
