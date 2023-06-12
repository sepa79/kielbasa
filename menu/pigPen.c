#include <c64/vic.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>

#include <menu/menuSystem.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/uiHandler.h>

#define PIGPEN_SC   2
#define PIGPEN_SMC1 10
#define PIGPEN_SMC2 1
#define PIGPEN_SX   130
#define PIGPEN_SY   110

enum SHOPIN_SPRITE_VIC_BANKS {
    SPR_BANK_PIG_ANIM1=0xbc,
};

// Sections and regions
#pragma section( pigPenLoaderData, 0 )
#pragma section( pigPenCode, 0 )
#pragma section( pigPenGfxDay, 0 )
#pragma section( pigPenSprites, 0 )
#pragma region( bankPigPenC, 0x8000, 0x9000, , MENU_BANK_PIG_PEN, { pigPenLoaderData, pigPenCode } )
#pragma region( bankPigPenG1, DAY_GFX_BMP, DAY_GFX_BMP+0x1400, , MENU_BANK_PIG_PEN, { pigPenGfxDay } )
#pragma region( bankPigPenG3, MENU_SPRITE_SRC, MENU_SPRITE_SRC+0x0400, , MENU_BANK_PIG_PEN, { pigPenSprites } )

#pragma data ( pigPenGfxDay )
__export const char pigPenGfx1[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/pigPen.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/pigPen.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/pigPen.kla"
};

#pragma data ( pigPenSprites )
__export const char pigPenGfx3[] = {
    #embed 0x0400 20 "assets/sprites/pig.spd"
};

// menu code is in ROM - data in RAM
#pragma code ( pigPenCode )
#pragma data ( data )

__interrupt static void _pigPenShowSprites(){
    vic.spr_enable   = 0b00000011;
    vic.spr_expand_x = 0b00000000;
    vic.spr_expand_y = 0b00000000;
    vic.spr_priority = 0b00000000;
    vic.spr_mcolor0  = PIGPEN_SMC1;
    vic.spr_mcolor1  = PIGPEN_SMC2;
    vic.spr_multi    = 0b11111110;
    vic.spr_msbx     = 0b00000000;

    vic.spr_pos[0].x = PIGPEN_SX;
    vic.spr_pos[1].x = PIGPEN_SX;
    vic.spr_pos[0].y = PIGPEN_SY;
    vic.spr_pos[1].y = PIGPEN_SY;
    vic.spr_color[0] = VCOL_BLACK;
    vic.spr_color[1] = PIGPEN_SC;

    byte sprPtr = SPR_BANK_PIG_ANIM1;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+0] = sprPtr+1;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+1] = sprPtr;
}

static void _ppMenu1(){
    updateStatusBar(p"Chlewik menu, opcja 1");
}
static void _ppMenu2(){
    updateStatusBar(p"Chlewik menu, opcja 2");
}
static void _ppMenu3(){
    updateStatusBar(p"Chlewik menu, opcja 3");
}

const struct MenuOption PIG_PEN_MENU[] = {
    { TXT_IDX_MENU_PIGPEN1, '1', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_ppMenu1, 0, 1, 1},
    { TXT_IDX_MENU_PIGPEN2, '2', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_ppMenu2, 0, 1, 2},
    { TXT_IDX_MENU_PIGPEN3, '3', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_ppMenu3, 0, 1, 3},
    { TXT_IDX_EXIT_TO_MAP, KEY_ARROW_LEFT, SCREEN_TRANSITION, UI_LF+UI_SELECT, &showMenu, MENU_BANK_MAP_VILLIAGE_1, 2, 5},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_SPLIT_MC_TXT, UI_LF, &showMenu, MENU_BANK_MAIN_MENU, 2, 5},
    END_MENU_CHOICES
};

static void _menuHandler(void){
    mnu_isGfxLoaded = false;
    loadMenuGfx(true);
    loadMenuSprites();

    sprBankPointer = ((char *)GFX_1_BASE + 64*(SPR_BANK_PIG_ANIM1+1));
    copyCharToSprite('0', 0, 0, sprBankPointer);
    copyCharToSprite('0', 1, 0, sprBankPointer);

    // Prepare output window
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, SCREEN_Y_START, SCREEN_WIDTH, SCREEN_HEIGHT);
    cwin_clear(&cw);
    
    displayMenu(PIG_PEN_MENU);
}

#pragma data ( pigPenLoaderData )

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &menuNoop,
    .loadMenuGfx     = &menuGfxLoaderSingleBitmap,
    .loadMenuSprites = &menuSpriteLoader,
    .showMenu        = &_menuHandler,
    .showSprites     = &_pigPenShowSprites,
    .updateMenu      = &menuNoop,
};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
