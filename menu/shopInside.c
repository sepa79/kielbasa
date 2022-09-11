#include <c64/vic.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>
#include <c64/easyflash.h>
#include <c64/memmap.h>

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
#pragma section( shopInGfxNight, 0 )
#pragma section( shopInSprites, 0 )
#pragma region( bankShopInC, 0x8000, 0x9000, , MENU_BANK_SHOP_IN, { shopInLoaderData, shopInCode } )
#pragma region( bankShopInG1, DAY_GFX_BMP, DAY_GFX_BMP+0x1400, , MENU_BANK_SHOP_IN, { shopInGfxDay } )
#pragma region( bankShopInG2, NIGHT_GFX_BMP, NIGHT_GFX_BMP+0x1400, , MENU_BANK_SHOP_IN, { shopInGfxNight } )
#pragma region( bankShopInG3, MENU_SPRITE_SRC, MENU_SPRITE_SRC+0x0400, , MENU_BANK_SHOP_IN, { shopInSprites } )

#pragma data ( shopInGfxDay )
__export const char shopInGfx1[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/shopInside.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/shopInside.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/shopInside.kla"
};
#pragma data ( shopInGfxNight )
__export const char shopInGfx2[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/shopInside.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/shopInside.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/shopInside.kla"
};

#pragma data ( shopInSprites )
__export const char shopInGfx3[] = {
    #embed 0x0400 16 "assets/sprites/gherkinAnim.spd"
};

// Switching code generation back to shared section
// TODO: this can go to menu memory section, be loaded on demand and then dumped when not needed
#pragma code ( code )
#pragma data ( data )

static char radioPlaylist = 0;

void _loadRadioMsx(){
    // vic.color_back++;
    joyCursor.enabled = false;
    if(gms_enableMusic) {
        // set Radio bank
        changeBank(MUSIC_BANK_RADIO_1);

        // stop music
        gms_enableMusic = false;
        ((byte *)0xd418)[0] &= ~0xf;

        // load different MSX file
        loadMusic();

        // init it
        __asm {
            lda #MSX_ROM
            sta $01
            lda radioPlaylist
            jsr MSX_INIT
        };

        // advance playlist
        radioPlaylist++;
        if(radioPlaylist > RADIO_1_SONGS)
            radioPlaylist = 0;

        // set ROM back
        mmap_set(MMAP_ROM);

        // reenable music
        // gms_musicSpeed2x = true;
        gms_enableMusic = true;

        // revert menu bank
        restoreBank();
    }
    joyCursor.enabled = true;
    // vic.color_back--;
}

void _goBackToPrvMenu(){
    // vic.color_back++;
    joyCursor.enabled = false;

    if(gms_enableMusic) {
        // set Main Music bank
        changeBank(MUSIC_BANK);

        // stop music
        gms_enableMusic = false;
        ((byte *)0xd418)[0] &= ~0xf;

        // load different MSX file
        loadMusic();

        // init it
        __asm {
            lda #MSX_ROM
            sta $01
            lda #$01
            jsr MSX_INIT
        };

        // set ROM back
        mmap_set(MMAP_ROM);

        // reenable music
        // gms_musicSpeed2x = false;
        gms_enableMusic = true;

        // revert menu bank
        restoreBank();
    }

    loadMenu(MENU_BANK_SHOP);
    showMenu();
    joyCursor.enabled = true;
    // vic.color_back--;
}

// menu code is in ROM - data in RAM
#pragma code ( shopInCode )
#pragma data ( data )

static void _siMenu1(){
    updateStatusBar("   Sklep menu, opcja 1");
}
static void _siMenu2(){
    updateStatusBar("   Sklep menu, opcja 2");
}

const struct MenuOption SHOP_INSIDE_MENU[] = {
    { TXT_IDX_MENU_SHOPIN1, '1', UI_LR, &_siMenu1, 0, 1, 1},
    { TXT_IDX_MENU_SHOPIN2, '2', UI_LR, &_siMenu2, 0, 1, 2},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, UI_LR, &_goBackToPrvMenu, 0, 2, 3},
    END_MENU_CHOICES
};

static void _shopInsideShowSprites(){
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

static void _menuHandler(void){
    animGherkinFrame = 0;
    animGherkinDelay = SHOP_INSIDE_ANIM_1_DELAY;
    
    loadMenuGfx(cal_isDay);
    loadMenuSprites();

    // load music
    _loadRadioMsx();

    // Prepare output window
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, SCREEN_Y_START, SCREEN_WIDTH, SCREEN_HEIGHT);
    cwin_clear(&cw);
    
    displayMenu(SHOP_INSIDE_MENU);
}

#pragma data ( shopInLoaderData )

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &menuNoop,
    .loadMenuGfx     = &menuGfxLoader,
    .loadMenuSprites = &menuSpriteLoader,
    .showMenu        = &_menuHandler,
    .showSprites     = &_shopInsideShowSprites,
    .updateMenu      = &menuNoop,
};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
