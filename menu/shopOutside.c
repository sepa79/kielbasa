#include <c64/vic.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>

#include <engine/gameState.h>
#include <menu/menuSystem.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/uiHandler.h>
#include <engine/gameSettings.h>

#define SHOP_OPEN_WEEK_O1 7
#define SHOP_OPEN_WEEK_C1 11
#define SHOP_OPEN_WEEK_O2 15
#define SHOP_OPEN_WEEK_C2 19

#define SHOP_OUTSIDE_ANIM_FRAMES 12
#define SHOP_OUTSIDE_ANIM_X 24+100
#define SHOP_OUTSIDE_ANIM_Y 50+50
#define SHOP_OUTSIDE_ANIM_C1 1
#define SHOP_OUTSIDE_ANIM_C2 1
#define SHOP_OUTSIDE_SMC1 9
#define SHOP_OUTSIDE_SMC2 8
#define SHOP_OUTSIDE_SPR_BANK 0xbc
#define SHOP_OUTSIDE_SPR_BANK_ANIM 0xbd

volatile char animFrame = 0;
volatile char animDelay = 1;

// Sections and regions
#pragma section( shopOutLoaderData, 0 )
#pragma section( shopOutCode, 0 )
#pragma section( shopOutData, 0 )
#pragma section( shopOutGfxData, 0 )
#pragma section( shopOutGfxDay, 0 )
#pragma section( shopOutGfxNight, 0 )
#pragma region( bankShopOutC, 0x8000, 0x9000, , MENU_BANK_SHOP, { shopOutLoaderData, shopOutCode, shopOutData } )
#pragma region( bankShopOutG1, DAY_GFX_BMP, DAY_GFX_BMP+0x12c0+12, , MENU_BANK_SHOP, { shopOutGfxData, shopOutGfxDay } )
#pragma region( bankShopOutG2, NIGHT_GFX_BMP+12, 0xbc00, , MENU_BANK_SHOP, { shopOutGfxNight } )

#pragma data ( shopOutGfxDay )

__export const char shopOutGfx1a[] = {
    #embed 0x0f00 0x0002 rle "assets/multicolorGfx/shopOutside_31.10.22.kla"
};
__export const char shopOutGfx1b[] = {
    #embed 0x01e0 0x1f42 rle "assets/multicolorGfx/shopOutside_31.10.22.kla"
};
__export const char shopOutGfx1c[] = {
    #embed 0x01e0 0x232a rle "assets/multicolorGfx/shopOutside_31.10.22.kla"
};

#pragma data ( shopOutGfxNight )
__export const char shopOutGfx2a[] = {
    #embed 0x0f00 0x0002 rle "assets/multicolorGfx/shopOutside_31.10.22.kla"
};
__export const char shopOutGfx2b[] = {
    #embed 0x01e0 0x1f42 rle "assets/multicolorGfx/shopOutside_31.10.22.kla"
};
__export const char shopOutGfx2c[] = {
    #embed 0x01e0 0x232a rle "assets/multicolorGfx/shopOutside_31.10.22.kla"
};

__export const char shopOutSprites[] = {
    #embed 0xffff 20 "assets/sprites/piesGora.spd"
};

#pragma data ( shopOutGfxData )
__export static const GfxData gfxData = {
    .DayBmp   = shopOutGfx1a,
    .DayC1    = shopOutGfx1b,
    .DayC2    = shopOutGfx1c,
    .NightBmp = shopOutGfx2a,
    .NightC1  = shopOutGfx2b,
    .NightC2  = shopOutGfx2c,
};

// const data on CRT
#pragma data ( shopOutData )
static const char shopOutAnimDelays[SHOP_OUTSIDE_ANIM_FRAMES] = { 64, 8, 6, 3, 6, 8, 255, 8, 6, 3, 6, 8 };

// menu code is in ROM - data in RAM
#pragma code ( shopOutCode )
#pragma data ( data )

static void _menuSpriteLoader(){
    memcpy((volatile char*) MENU_SPRITE_DST, shopOutSprites, 0x40*26);
}

static bool _checkIfOpen(){
    // weekdays
    if(GS.calendar.dateHour >= SHOP_OPEN_WEEK_O1)
        if(GS.calendar.dateHour < SHOP_OPEN_WEEK_C1)
            return true;

    if(GS.calendar.dateHour >= SHOP_OPEN_WEEK_O2)
        if(GS.calendar.dateHour < SHOP_OPEN_WEEK_C2)
            return true;

    return false;
}


__interrupt static void _shopOutsideShowSprites(){
    vic.spr_enable   = 0b00001111;
    vic.spr_expand_x = 0b00000000;
    vic.spr_expand_y = 0b00000000;
    vic.spr_priority = 0b00000000;
    vic.spr_mcolor0  = SHOP_OUTSIDE_SMC1;
    vic.spr_mcolor1  = SHOP_OUTSIDE_SMC2;
    vic.spr_multi    = 0b00001100;
    vic.spr_msbx     = 0b00000000;

    vic.spr_pos[0].x = SHOP_OUTSIDE_ANIM_X;
    vic.spr_pos[1].x = SHOP_OUTSIDE_ANIM_X;
    vic.spr_pos[2].x = SHOP_OUTSIDE_ANIM_X;
    vic.spr_pos[3].x = SHOP_OUTSIDE_ANIM_X;
    vic.spr_pos[0].y = SHOP_OUTSIDE_ANIM_Y+21;
    vic.spr_pos[1].y = SHOP_OUTSIDE_ANIM_Y;
    vic.spr_pos[2].y = SHOP_OUTSIDE_ANIM_Y+21;
    vic.spr_pos[3].y = SHOP_OUTSIDE_ANIM_Y;

    vic.spr_color[0] = VCOL_BLACK;
    vic.spr_color[1] = VCOL_BLACK;
    vic.spr_color[2] = SHOP_OUTSIDE_ANIM_C1;
    vic.spr_color[3] = SHOP_OUTSIDE_ANIM_C2;

    char sprPtr = SHOP_OUTSIDE_SPR_BANK_ANIM + animFrame;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+0] = SHOP_OUTSIDE_SPR_BANK+13;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+1] = sprPtr+13;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+2] = SHOP_OUTSIDE_SPR_BANK;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+3] = sprPtr;

    animDelay--;
    if(animDelay == 0){
        animDelay = shopOutAnimDelays[animFrame];
        
        animFrame++;
        if(animFrame == SHOP_OUTSIDE_ANIM_FRAMES)
            animFrame = 0;
    }
}

// Switching code generation back to shared section
// this method must operate from RAM, as it switches banks
#pragma code ( code )
#pragma data ( data )
static void _getInside(){
    if(!_checkIfOpen()){
        updateStatusBar(TXT[SB_IDX_SHOP_CLOSED]);
        setErrorCursor();
        return;
    }

    switchScreenTo(SCREEN_TRANSITION);

    joyCursor.enabled = false;
    gms_disableTimeControls = true;
    gms_gameSpeed = SPEED_PAUSED;
    updateGameSpeed();
    loadMenu(MENU_BANK_SHOP_IN);
    showMenu();
    joyCursor.enabled = true;
}

// menu code is in ROM - data in RAM
#pragma code ( shopOutCode )
#pragma data ( data )

// static void _soMenu2(){
//     updateStatusBar("Sklep menu, opcja 2");
// }

const struct MenuOption SHOP_OUTSIDE_MENU[] = {
    { TXT_IDX_MENU_SHOPOUT1, '1', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_getInside, 0, 1, 4},
    // { TXT_IDX_MENU_SHOPOUT2, '2', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_soMenu2, 0, 1, 5},
    { TXT_IDX_EXIT_TO_MAP, KEY_ARROW_LEFT, SCREEN_TRANSITION, UI_LF+UI_SELECT, &showMenu, MENU_BANK_MAP_VILLIAGE_1, 1, 8},
    // { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_SPLIT_MC_TXT, UI_LF, &showMenu, MENU_BANK_MAIN_MENU, 2, 7},
    END_MENU_CHOICES
};

static void _menuHandler(void){
    loadMenuGfx();
    loadMenuSprites();
    // Prepare output window
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, SCREEN_Y_START, SCREEN_WIDTH, SCREEN_HEIGHT);
    cwin_clear(&cw);
    cwin_putat_string_raw(&cw, 0, 0, TXT[TXT_IDX_SHOP_DESCRIPTION], VCOL_GREEN);
    
    gms_disableTimeControls = false;
    displayMenu(SHOP_OUTSIDE_MENU);
    switchScreenTo(SCREEN_SPLIT_MC_TXT);
}

#pragma data ( shopOutLoaderData )

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &menuNoop,
    .loadMenuGfx     = &menuGfxLoaderRle,
    .loadMenuSprites = &_menuSpriteLoader,
    .showMenu        = &_menuHandler,
    .showSprites     = &_shopOutsideShowSprites,
    .updateMenu      = &menuNoop,
    .runMenuLoop     = &menuNoop,
};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
