#include <c64/vic.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>

#include <menu/menuSystem.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <engine/spriteText.h>
#include <assets/assetsSettings.h>
#include <engine/uiHandler.h>
#include <tasks/taskManager.h>
#include <character/character.h>
#include <tick/kitchenTick.h>
#include <tasks/kitchenTask.h>

enum FARMLAND_SPRITE_VIC_BANKS {
    SPR_BANK_FOOD_1=0xbc,
    SPR_BANK_FOOD_2,
    SPR_BANK_FOOD_3,
    SPR_BANK_FOOD_4,
};

#define SPR_FOOD_1      ((char *)GFX_1_BASE + 64*SPR_BANK_FOOD_1)
#define SPR_FOOD_2      ((char *)GFX_1_BASE + 64*SPR_BANK_FOOD_2)
#define SPR_FOOD_3      ((char *)GFX_1_BASE + 64*SPR_BANK_FOOD_3)
#define SPR_FOOD_4      ((char *)GFX_1_BASE + 64*SPR_BANK_FOOD_4)

// Sections and regions
#pragma section( kitchenLoaderData, 0 )
#pragma section( kitchenCode, 0 )
#pragma section( kitchenGfxDay, 0 )
#pragma section( kitchenSprites, 0 )

#pragma region( regionSmokeHouseC, 0x8000, 0x9000, , MENU_BANK_KITCHEN, { kitchenLoaderData, kitchenCode } )
#pragma region( regionSmokeHouseG1, DAY_GFX_BMP, DAY_GFX_BMP+0x1400, , MENU_BANK_KITCHEN, { kitchenGfxDay } )
#pragma region( regionSmokeHouseG2, MENU_SPRITE_SRC, MENU_SPRITE_SRC+0x0400, , MENU_BANK_KITCHEN, { kitchenSprites } )

#pragma data ( kitchenGfxDay )
__export const char kitchenGfx1[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/kuchnia_210623.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/kuchnia_210623.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/kuchnia_210623.kla"
};

#pragma data ( kitchenSprites )
__export static char SPR_DATA_UI[64*4] = {0};

// menu code is in ROM - data in RAM
#pragma code ( kitchenCode )
#pragma data ( data )

__interrupt static void _menuShowSprites(){
    vic.spr_enable   = 0b00001111;
    vic.spr_expand_x = 0b00000000;
    vic.spr_expand_y = 0b00000000;
    vic.spr_priority = 0b00000000;
    vic.spr_multi    = 0b10000000;
    vic.spr_msbx     = 0b00001100;

    // food ui sprites
    vic.spr_pos[0].x = 24+148;
    vic.spr_pos[1].x = 24+196;
    vic.spr_pos[2].x = 12;
    vic.spr_pos[3].x = 60;
    vic.spr_pos[0].y = 50+67;
    vic.spr_pos[1].y = 50+67;
    vic.spr_pos[2].y = 50+67;
    vic.spr_pos[3].y = 50+67;

    vic.spr_color[0] = VCOL_MED_GREY;
    vic.spr_color[1] = VCOL_MED_GREY;
    vic.spr_color[2] = VCOL_MED_GREY;
    vic.spr_color[3] = VCOL_MED_GREY;

    GFX_1_SCR[OFFSET_SPRITE_PTRS+0] = SPR_BANK_FOOD_1;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+1] = SPR_BANK_FOOD_2;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+2] = SPR_BANK_FOOD_3;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+3] = SPR_BANK_FOOD_4;

}

static void _updateView(){
    byteToSprite(GS.kitchen.storage[FOOD_SHOP_BREAD], SPR_FOOD_1);
    byteToSprite(GS.kitchen.storage[FOOD_HOME_BREAD], SPR_FOOD_2);
    byteToSprite(GS.kitchen.storage[FOOD_CANNED_MEAT], SPR_FOOD_3);
    byteToSprite(GS.kitchen.storage[FOOD_SOUSAGE], SPR_FOOD_4);

    if(GS.kitchen.bakeBreadDaily){
        cwin_putat_string_raw(&cw, 19, 2, TXT[TXT_IDX_KITCHEN_TASK_ON], VCOL_GREEN);
    } else {
        cwin_putat_string_raw(&cw, 19, 2, TXT[TXT_IDX_KITCHEN_TASK_OFF], VCOL_DARK_GREY);
    }
    cwin_putat_string_raw(&cw, 19, 3, TXT[TXT_IDX_KITCHEN_EATING_STYLE_LIGHT + GS.kitchen.breakfastType], VCOL_DARK_GREY);
    cwin_putat_string_raw(&cw, 19, 4, TXT[TXT_IDX_KITCHEN_EATING_STYLE_LIGHT + GS.kitchen.supperType], VCOL_DARK_GREY);
    cwin_putat_string_raw(&cw, 19, 5, TXT[TXT_IDX_KITCHEN_PREFER_SHOP + GS.kitchen.preferHomeFood], VCOL_DARK_GREY);
    cwin_putat_string_raw(&cw, 19, 6, TXT[TXT_IDX_KITCHEN_PREFER_POTATO + GS.kitchen.preferCorn], VCOL_DARK_GREY);

}

static void _kitchenBakeBread(){
    addBakeBreadTask();
}

static void _kitchenBakeBreadDaily(){
    GS.kitchen.bakeBreadDaily = !GS.kitchen.bakeBreadDaily;
    _updateView();
}

static void _kitchenBreakfastType(){
    if(GS.kitchen.breakfastType < MEAL_TYPE_FULL){
        GS.kitchen.breakfastType ++;
    } else {
        GS.kitchen.breakfastType = MEAL_TYPE_LIGHT;
    }
    _updateView();
}

static void _kitchenSupperType(){
    if(GS.kitchen.supperType < MEAL_TYPE_FULL){
        GS.kitchen.supperType ++;
    } else {
        GS.kitchen.supperType = MEAL_TYPE_LIGHT;
    }
    _updateView();
}

static void _kitchenFoodPreferrence(){
    GS.kitchen.preferHomeFood = !GS.kitchen.preferHomeFood;
    _updateView();
}
static void _kitchenVegPreferrence(){
    GS.kitchen.preferCorn = !GS.kitchen.preferCorn;
    _updateView();
}

const struct MenuOption KITCHEN_MENU[] = {
    { TXT_IDX_MENU_KITCHEN1, '1', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_kitchenBakeBread, 0, 1, 1},
    { TXT_IDX_MENU_KITCHEN2, '2', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_kitchenBakeBreadDaily, 0, 1, 2},
    { TXT_IDX_MENU_KITCHEN3, '3', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_kitchenBreakfastType, 0, 1, 3},
    { TXT_IDX_MENU_KITCHEN4, '4', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_kitchenSupperType, 0, 1, 4},
    { TXT_IDX_MENU_KITCHEN5, '5', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_kitchenFoodPreferrence, 0, 1, 5},
    { TXT_IDX_MENU_KITCHEN6, '6', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_kitchenVegPreferrence, 0, 1, 6},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_TRANSITION, UI_LF, &showMenu, MENU_BANK_MAIN_MENU, 2, 11},
    END_MENU_CHOICES
};

static void _menuHandler(void){
    mnu_isGfxLoaded = false;
    loadMenuGfx();
    loadMenuSprites();

    // Prepare output window
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, SCREEN_Y_START, SCREEN_WIDTH, SCREEN_HEIGHT);
    cwin_clear(&cw);

    displayMenu(KITCHEN_MENU);
    switchScreenTo(SCREEN_SPLIT_MC_TXT);

    _updateView();
}

#pragma data ( kitchenLoaderData )

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &menuNoop,
    .loadMenuGfx     = &menuGfxLoaderSingleBitmap,
    .loadMenuSprites = &menuSpriteLoader,
    .showMenu        = &_menuHandler,
    .showSprites     = &_menuShowSprites,
    .updateMenu      = &_updateView,
    .runMenuLoop     = &menuNoop,
};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
