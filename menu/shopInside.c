#include <c64/vic.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>
#include <c64/easyflash.h>
#include <c64/memmap.h>

#include <engine/gameState.h>
#include <menu/menuSystem.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <assets/music.h>
#include <engine/uiHandler.h>

#define SHOP_INSIDE_ANIM_1_FRAMES 6
#define SHOP_INSIDE_ANIM_1_DELAY 15
#define SHOP_INSIDE_ANIM_1_C 5
#define SHOP_INSIDE_ANIM_1_Y 2
#define SHOP_INSIDE_SMC1 8
#define SHOP_INSIDE_SMC2 9

volatile byte animGherkinFrame = 0;
volatile byte animGherkinDelay = SHOP_INSIDE_ANIM_1_DELAY;
volatile byte animGherkinY = SHOP_INSIDE_ANIM_1_Y;

enum SHOPIN_SPRITE_VIC_BANKS {
    SPR_BANK_GHERKIN_ANIM1=0xbc,
    SPR_BANK_GHERKIN_ANIM2,
    SPR_BANK_GHERKIN_ANIM3,
    SPR_BANK_GHERKIN_ANIM4,
    SPR_BANK_GHERKIN_ANIM5,
    SPR_BANK_GHERKIN_ANIM6,
};

// not really needed
#define SPR_GHERKIN_ANIM1 ((char *)GFX_1_BASE + 64*SPR_BANK_GHERKIN_ANIM1)
#define SPR_GHERKIN_ANIM2 ((char *)GFX_1_BASE + 64*SPR_BANK_GHERKIN_ANIM2)
#define SPR_GHERKIN_ANIM3 ((char *)GFX_1_BASE + 64*SPR_BANK_GHERKIN_ANIM3)
#define SPR_GHERKIN_ANIM4 ((char *)GFX_1_BASE + 64*SPR_BANK_GHERKIN_ANIM4)
#define SPR_GHERKIN_ANIM5 ((char *)GFX_1_BASE + 64*SPR_BANK_GHERKIN_ANIM5)
#define SPR_GHERKIN_ANIM6 ((char *)GFX_1_BASE + 64*SPR_BANK_GHERKIN_ANIM6)

// Sections and regions
#pragma section( shopInLoaderData, 0 )
#pragma section( shopInCode, 0 )
#pragma section( shopInGfxDay, 0 )
#pragma section( shopInSprites, 0 )
#pragma region( bankShopInC, 0x8000, 0x9000, , MENU_BANK_SHOP_IN, { shopInLoaderData, shopInCode } )
#pragma region( bankShopInG1, DAY_GFX_BMP, DAY_GFX_BMP+0x1400, , MENU_BANK_SHOP_IN, { shopInGfxDay } )
#pragma region( bankShopInG3, MENU_SPRITE_SRC, MENU_SPRITE_SRC+0x0400, , MENU_BANK_SHOP_IN, { shopInSprites } )

#pragma data ( shopInGfxDay )
__export const char shopInGfx1[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/sklep_230723.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/sklep_230723.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/sklep_230723.kla"
};

#pragma data ( shopInSprites )
__export const char shopInGfx3[] = {
    #embed 0x0400 16 "assets/sprites/gherkinAnim.spd"
};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )

void _goBackToPrvMenu(){
    // vic.color_back++;
    joyCursor.enabled = false;

    playSong(MAIN_MENU_SONG);
    loadMenu(MENU_BANK_SHOP);
    showMenu();
    joyCursor.enabled = true;
    // vic.color_back--;
}

// temp menus, in RAM so that data is visible to updateStatusBar()

static void _siMenu2(){
    updateStatusBar(s"Sklep menu, opcja 2     ");
}

// menu code is in ROM - data in RAM
#pragma code ( shopInCode )
#pragma data ( data )


static void _siMenu1(){
    // TODO: Add gherkin's dissapearance
    if(GS.cash > 10){
            GS.cash -= 10;
            updateMoney();
            // TODO: replace with 'eatFood', it should update UI, too
            allCharacters[0].bonusTime += 1;
            allCharacters[0].bonusAmountMin += 1;
    }
}

// buy bread
static void _siMenu3(){
    if(GS.cash > foodItems[FOOD_SHOP_BREAD].price){
        if(addKitchenItem(FOOD_SHOP_BREAD)){
            GS.cash -= foodItems[FOOD_SHOP_BREAD].price;
            updateMoney();
        }
    }
}
// buy canned meats
static void _siMenu4(){
    if(GS.cash > foodItems[FOOD_CANNED_MEAT].price){
        if(addKitchenItem(FOOD_CANNED_MEAT)){
            GS.cash -= foodItems[FOOD_CANNED_MEAT].price;
            updateMoney();
        }
    }
}

const struct MenuOption SHOP_INSIDE_MENU[] = {
    { TXT_IDX_MENU_SHOPIN1, '1', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_siMenu1, 0, 1, 1},
    { TXT_IDX_MENU_SHOPIN2, '2', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_siMenu2, 0, 1, 2},
    { TXT_IDX_MENU_SHOPIN3, '3', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_siMenu3, 0, 1, 3},
    { TXT_IDX_MENU_SHOPIN4, '4', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_siMenu4, 0, 1, 4},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_TRANSITION, UI_LF, &_goBackToPrvMenu, 0, 2, 6},
    END_MENU_CHOICES
};

__interrupt static void _shopInsideShowSprites(){
    vic.spr_enable   = 0b00000001;
    vic.spr_expand_x = 0b00000000;
    vic.spr_expand_y = 0b00000000;
    vic.spr_priority = 0b00000000;
    vic.spr_mcolor0  = SHOP_INSIDE_SMC1;
    vic.spr_mcolor1  = SHOP_INSIDE_SMC2;
    vic.spr_multi    = 0b11111111;
    vic.spr_msbx     = 0b00000000;

    vic.spr_pos[0].x = 31;
    vic.spr_pos[0].y = 114 + animGherkinY;
    vic.spr_color[0] = SHOP_INSIDE_ANIM_1_C;

    byte sprPtr = SPR_BANK_GHERKIN_ANIM1 + animGherkinFrame;
    GFX_1_SCR[OFFSET_SPRITE_PTRS] = sprPtr;

    animGherkinDelay--;
    if(animGherkinDelay == 0){
        animGherkinDelay = SHOP_INSIDE_ANIM_1_DELAY;
        
        animGherkinFrame++;
        if(animGherkinFrame == SHOP_INSIDE_ANIM_1_FRAMES)
            animGherkinFrame = 0;

        animGherkinY++;
        if(animGherkinY > SHOP_INSIDE_ANIM_1_Y)
            animGherkinY = 0;
    }
}

static void _printPrices(){
    char str[11] = "";
    sprintf(str, "%9u", foodItems[FOOD_SHOP_BREAD].price);
    str[9] = 28; // zl
    str[10] = 0;
    cwin_putat_string_raw(&cw, 20, 3, str, VCOL_GREEN);
    sprintf(str, "%9u", foodItems[FOOD_CANNED_MEAT].price);
    str[9] = 28; // zl
    str[10] = 0;
    cwin_putat_string_raw(&cw, 20, 4, str, VCOL_GREEN);
}

static void _menuHandler(void){
    animGherkinFrame = 0;
    animGherkinDelay = SHOP_INSIDE_ANIM_1_DELAY;
    
    mnu_isGfxLoaded = false;
    loadMenuGfx();
    loadMenuSprites();

    // play radio music
    playNextRadioSong();

    // Prepare output window
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, SCREEN_Y_START, SCREEN_WIDTH, SCREEN_HEIGHT);
    cwin_clear(&cw);
    
    displayMenu(SHOP_INSIDE_MENU);
    _printPrices();
    switchScreenTo(SCREEN_SPLIT_MC_TXT);
}

#pragma data ( shopInLoaderData )

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &menuNoop,
    .loadMenuGfx     = &menuGfxLoaderSingleBitmap,
    .loadMenuSprites = &menuSpriteLoader,
    .showMenu        = &_menuHandler,
    .showSprites     = &_shopInsideShowSprites,
    .updateMenu      = &menuNoop,
    .runMenuLoop     = &menuNoop,
};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
