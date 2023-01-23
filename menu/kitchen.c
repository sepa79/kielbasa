#include <c64/vic.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>

#include <menu/menuSystem.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
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
    #embed 0x0f00 0x0002 "assets/multicolorGfx/kuchnia.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/kuchnia.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/kuchnia.kla"
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
    vic.spr_pos[0].y = 50+67+2;
    vic.spr_pos[1].y = 50+67+2;
    vic.spr_pos[2].y = 50+67+2;
    vic.spr_pos[3].y = 50+67+2;

    vic.spr_color[0] = VCOL_MED_GREY;
    vic.spr_color[1] = VCOL_MED_GREY;
    vic.spr_color[2] = VCOL_MED_GREY;
    vic.spr_color[3] = VCOL_MED_GREY;

    GFX_1_SCR[OFFSET_SPRITE_PTRS+0] = SPR_BANK_FOOD_1;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+1] = SPR_BANK_FOOD_2;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+2] = SPR_BANK_FOOD_3;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+3] = SPR_BANK_FOOD_4;

}


static void _updateSprite(unsigned int num) {
    byte num2str[6];
    utoa(num, num2str, 10);
    // need to max it at 100 or something, food storage is a char
    if(num > 99) {
        copyCharToSprite(num2str[0], 0, 0);
        copyCharToSprite(num2str[1], 1, 0);
        copyCharToSprite(num2str[2], 2, 0);

    } else {
        copyCharToSprite(s' ', 0, 0);
        copyCharToSprite(num2str[0], 1, 0);
        copyCharToSprite(num2str[1], 2, 0);
    }
}

static void _updateView(){
    sprBankPointer = SPR_FOOD_1;
    _updateSprite(kit_storage[FOOD_SHOP_BREAD]);
    sprBankPointer = SPR_FOOD_2;
    _updateSprite(kit_storage[FOOD_HOME_BREAD]);
    sprBankPointer = SPR_FOOD_3;
    _updateSprite(kit_storage[FOOD_CANNED_MEAT]);
    sprBankPointer = SPR_FOOD_4;
    _updateSprite(kit_storage[FOOD_SOUSAGE]);
}

static void _kitchenBakeBread(){
    // create Task
    struct Task task;
    // "Bake Bread"
    sprintf(task.desc, "%s",TXT[TXT_IDX_TASK_DSC_KITCHEN_BAKE_BREAD]);
    task.codeRef   = &bakeBreadTask;
    task.nameIdx   = TXT_IDX_TASK_KITCHEN;
    task.params[0] = 0;
    task.params[1] = 0;
    task.params[2] = 0;
    task.params[3] = 0;
    task.params[4] = 0;
    task.reqType   = SKILL_COOKING;
    task.icon      = SPR_TASK_COOK;
    task.status    = TASK_STATUS_NEW;
    addTask(&task);
}

static void _shMenu2(){
    updateStatusBar("   kitchen menu, opcja 2");
}
static void _shMenu3(){
    updateStatusBar("   kitchen menu, opcja 3");
}

const struct MenuOption KITCHEN_MENU[] = {
    { TXT_IDX_MENU_KITCHEN1, '1', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_kitchenBakeBread, 0, 1, 1},
    { TXT_IDX_MENU_KITCHEN2, '2', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_shMenu2, 0, 1, 2},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_SPLIT_MC_TXT, UI_LF, &showMenu, MENU_BANK_MAIN_MENU, 2, 5},
    END_MENU_CHOICES
};

static void _menuHandler(void){
    mnu_isGfxLoaded = false;
    loadMenuGfx(true);
    loadMenuSprites();

    // Prepare output window
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, SCREEN_Y_START, SCREEN_WIDTH, SCREEN_HEIGHT);
    cwin_clear(&cw);
    
    displayMenu(KITCHEN_MENU);
    _updateView();
}

#pragma data ( kitchenLoaderData )

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &menuNoop,
    .loadMenuGfx     = &menuGfxLoaderSingleBitmap,
    .loadMenuSprites = &menuSpriteLoader,
    .showMenu        = &_menuHandler,
    .showSprites     = &_menuShowSprites,
    .updateMenu      = &menuNoop,
};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
