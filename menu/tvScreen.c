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

#define TV_SCANLINE_START 80
#define TV_SCANLINE_END   200
volatile char line = TV_SCANLINE_START;

// #define delay 16
volatile char delay = 16;

__interrupt static void _menuShowSpritesX(){
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

    // about 16 bytes shorter, but harder to edit, and as it runs form CRT - space is not the issue
    // vic_sprxy(0, xPos + 0*48, yPos);
    // vic_sprxy(1, xPos + 1*48, yPos);
    // vic_sprxy(2, xPos + 2*48, yPos);
    // vic_sprxy(3, xPos + 0*48, yPos + 42);
    // vic_sprxy(4, xPos + 1*48, yPos + 42);
    // vic_sprxy(5, xPos + 2*48, yPos + 42);
    // vic.spr_color[0] = VCOL_BLUE ;
    // vic.spr_color[1] = VCOL_BLUE ;
    // vic.spr_color[2] = VCOL_BLUE ;
    // vic.spr_color[3] = VCOL_BLUE ;
    // vic.spr_color[4] = VCOL_BLUE ;
    // vic.spr_color[5] = VCOL_BLUE ;
    // GFX_2_SCR[OFFSET_SPRITE_PTRS+0] = SPRITE_BLOCK + 0;
    // GFX_2_SCR[OFFSET_SPRITE_PTRS+1] = SPRITE_BLOCK + 1;
    // GFX_2_SCR[OFFSET_SPRITE_PTRS+2] = SPRITE_BLOCK + 2;
    // GFX_2_SCR[OFFSET_SPRITE_PTRS+3] = SPRITE_BLOCK + 3;
    // GFX_2_SCR[OFFSET_SPRITE_PTRS+4] = SPRITE_BLOCK + 4;
    // GFX_2_SCR[OFFSET_SPRITE_PTRS+5] = SPRITE_BLOCK + 5;
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++
    // Bad TV effect
    // ++++++++++++++++++++++++++++++++++++++++++++++++++

    vic_waitLine(line);

    // vic.color_border--;
    __asm {
        lda #14 // light blue

        dec $d000
        dec $d002
        dec $d004
        dec $d006
        dec $d008
        dec $d00a

        ldx delay
    d1: dex
        bne d1

        sta $d027
        sta $d028
        sta $d029
        sta $d02a
        sta $d02b
        sta $d02c

        ldx delay
    d2: dex
        bne d2

        inc $d000
        inc $d002
        inc $d004
        inc $d006
        inc $d008
        inc $d00a

        ldx delay
    d3: dex
        bne d3

        inc $d000
        inc $d002
        inc $d004
        inc $d006
        inc $d008
        inc $d00a

        ldx delay
    d4: dex
        bne d4
        
        dec $d000
        dec $d002
        dec $d004
        dec $d006
        dec $d008
        dec $d00a

        ldx delay
    d5: dex
        bne d5
        
        dec $d000
        dec $d002
        dec $d004
        dec $d006
        dec $d008
        dec $d00a

        ldx delay
    d6: dex
        bne d6

        inc $d000
        inc $d002
        inc $d004
        inc $d006
        inc $d008
        inc $d00a

        lda #6 // blue
        sta $d027
        sta $d028
        sta $d029
        sta $d02a
        sta $d02b
        sta $d02c

    }
    // vic.color_border++;
    line++;
    if(line > TV_SCANLINE_END) {
        line = TV_SCANLINE_START;
        delay = delay > 2;
        if(delay == 0 ) {
            delay =(rand() & 0x1f) +1;
        }
    }

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
    .loadMenuGfx     = &loadFullKoalaToBMP2,
    .loadMenuSprites = &menuNoop,
    .showMenu        = &_menuHandler,
    .showSprites     = &_menuShowSpritesX,
    .updateMenu      = &menuNoop,
    .runMenuLoop     = &menuNoop,
};

// Switching code generation back to shared section
#pragma code(code)
#pragma data(data)
