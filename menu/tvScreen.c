#include <c64/vic.h>
#include <c64/charwin.h>
#include <c64/rasterirq.h>
#include <string.h>

#include <menu/menuSystem.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <engine/spriteText.h>
#include <assets/assetsSettings.h>
#include <assets/mainGfx.h>
#include <engine/gameSettings.h>

// Sections and regions
#pragma section(tvScreenLoaderData, 0)
#pragma section(tvScreenCode, 0)
#pragma section(tvScreenGfx, 0)
// #pragma section(tvScreenFnt, 0)
#pragma section(tvScreenSprites, 0)

#pragma region(regionTvScreenC, 0x8000, 0x9000, , MENU_BANK_TV_SCREEN, { tvScreenLoaderData, tvScreenCode })
#pragma region(regionTvScreenG1, DAY_GFX_BMP, DAY_GFX_BMP + 0x2711, , MENU_BANK_TV_SCREEN, { tvScreenGfx })
// #pragma region(regionTvScreenG2, DAY_GFX_BMP + 0x2711, DAY_GFX_BMP + 0x2711 + 1024, , MENU_BANK_TV_SCREEN, { tvScreenFnt })

#pragma data(tvScreenGfx)
__export const char tvScreenGfx1[] = {
    #embed 0xffff 2 "assets/multicolorGfx/tv53.kla"
};

// #pragma data(tvScreenFnt)
// __export const char tvScreenFont[] = {
//     #embed 1024 "assets/gameFont - Chars.bin"
// };

#pragma code(tvScreenCode)
#pragma data(data)

// c800 is free to use by menu
#define SPRITE_BLOCK 0x20
#define SPRITE_BLOCK_POINTER ((char *)0xc800)

__interrupt static void _menuShowSprites(){
    // too complex to put into irq if made as separate library, Oscar complains
    vic.spr_expand_x = 0b11111111;
    vic.spr_expand_y = 0b11111111;
    vic.spr_priority = 0b00000000;
    vic.spr_multi    = 0b00000000;

    #define xPos   108
    #define yPos   100
    #define xSize  3
    #define ySize  2

    char sprId = 0;
    for(char y=0;y<ySize;y++){
        #pragma unroll(full)
        for(char x=0;x<xSize;x++){
            vic_sprxy(sprId, xPos + x*48, yPos + y*42);
            vic.spr_color[sprId] = VCOL_BLUE ;
            GFX_2_SCR[OFFSET_SPRITE_PTRS+sprId] = SPRITE_BLOCK + sprId;
            sprId++;
        }
    }
}

static void _loadFullKoalaToBMP2(){
    // load colors
    char i = 0;
    do {
#assign y 0
#repeat
        GFX_2_SCR[y + i] = MENU_FULL_KOALA_SCR[y + i];
        COLOR_RAM[y + i] = MENU_FULL_KOALA_COL[y + i];
#assign y y + 256
#until y == 1024
        i++;
    } while (i != 0);

    // load bitmap
    i = 0;
    do {
#assign y 0
#repeat
        GFX_2_BMP[y + i] = MENU_FULL_KOALA_BMP[y + i];
#assign y y + 256
#until y == 8192
        i++;
    } while (i != 0);
#undef y
}

const struct MenuOption TV_SCREEN_MENU[] = {
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_TRANSITION, UI_LF+UI_HIDE, &revertPreviousMenu, 0, 2, 11},
    END_MENU_CHOICES
};

static void _menuHandler(void) {
    gms_disableTimeControls = true;
    gms_gameSpeed = SPEED_PAUSED;
    updateGameSpeed();
    
    loadMenuGfx();

    memset(SPRITE_BLOCK_POINTER, 0, 0x40*8);
    textToSprite((char*)TXT[TV_IDX_MENU_TV_SCREEN_BILLS], 3, SPRITE_BLOCK_POINTER);

    char str[10] = "";
    sprintf(str, "%8d", GS.bills);
    str[8] = 28; // zl
    str[9] = 0;
    textToSpriteAt(str, 3, SPRITE_BLOCK_POINTER+64*3, 0, 0);

    sprintf(str, "%8d", GS.pension);
    str[8] = 28; // zl
    str[9] = 0;
    textToSpriteAt(str, 3, SPRITE_BLOCK_POINTER+64*3, 0, 2);

    switchScreenTo(SCREEN_MC_GFX);
    displayMenu(TV_SCREEN_MENU);
    updateStatusBar(TXT[SB_IDX_MENU_TV_SCREEN]);
}

#pragma data(tvScreenLoaderData)

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &menuNoop,
    .loadMenuGfx     = &_loadFullKoalaToBMP2,
    .loadMenuSprites = &menuNoop,
    .showMenu        = &_menuHandler,
    .showSprites     = &_menuShowSprites,
    .updateMenu      = &menuNoop,
    .runMenuLoop     = &menuNoop,
};

// Switching code generation back to shared section
#pragma code(code)
#pragma data(data)
