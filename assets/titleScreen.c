// title screen doubles as main Init - loader uses this crt bank to store all temporary init procedures.

#include <c64/vic.h>
#include <c64/cia.h>
#include <string.h>
#include <stdlib.h>
#include <oscar.h>

#include <engine/logger.h>
#include <engine/easyFlashBanks.h>
#include <engine/spriteText.h>
#include <assets/assetsSettings.h>
#include <engine/titleScreenIrq.h>
#include <assets/mainGfx.h>
#include <assets/music.h>
#include <translation/textsPL.h>
#include <translation/textsEN.h>
#include "kielbasa.h"
#include "common.h"

#pragma data ( titleScreenGfx )

__export const char titleScreen[] = {
    #embed 0xffff 2 "assets/multicolorGfx/title_new_150223.kla"
    // #embed 0xffff 2 "assets/multicolorGfx/kielbasaEmpire.kla"
};

#pragma code ( titleScreenCRTCode )
#pragma data ( gameInitData )

static void _loadFullKoalaToBMP2_title(){
    // load colors
    char i = 0;
    do {
#assign y 0
#repeat
        GFX_2_SCR[y + i] = FULL_KOALA_SCR[y + i];
        COLOR_RAM[y + i] = FULL_KOALA_COL[y + i];
#assign y y + 256
#until y == 1024
        i++;
    } while (i != 0);

    // load bitmap
    i = 0;
    do {
#assign y 0
#repeat
        GFX_2_BMP[y + i] = FULL_KOALA_BMP[y + i];
#assign y y + 256
#until y == 8192
        i++;
    } while (i != 0);
#undef y

}

void loadTitleScreen(){
    vic.color_border  = VCOL_BLACK;
    vic.color_back  = VCOL_BLACK;

    _loadFullKoalaToBMP2_title();

    // vic_setmode(VICM_HIRES_MC, GFX_2_SCR, GFX_2_BMP);
    vic.ctrl2 = VIC_CTRL2_MCM | VIC_CTRL2_CSEL | 0;
    cia2.pra = dd00_gfx2;
    vic.memptr = d018_gfx2;
    // screen needs to be enabled later like below:
    // vic.ctrl1 = VIC_CTRL1_BMM | VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3;

}
// load Init and IRQ code into temp space and go
void loadAndRunInit(){
    loggerInit();
    // bankTitleCode2 is b200 in crt, see titleScreen.h
    memcpy((char *)0x7000, (char *)0xb200, 0x07ff);
    initGameStartScreen();
}

//-----------------------------------------------------------------------------------------
#pragma code ( gameInitRAMCode )
#pragma data ( gameInitData )
//-----------------------------------------------------------------------------------------

void initGameStartScreen(){
    loadTitleScreen();
    // splash and turn screen on
    splashScreen(true, 3);

    setBank(MUSIC_BANK);
    // loadMusic(&PLAYLIST[MSX_MAIN_THEME]);
    oscar_expand_rle((char*)MSX_DST_ADR, (char*)MSX_DST_ADR);

    __asm {
        sei
        lda #$7f
        sta $dc0d  //disable timer interrupts which can be generated by the two CIA chips
        sta $dd0d  //the kernal uses such an interrupt to flash the cursor and scan the keyboard, so we better
                   //stop it.

        lda $dc0d  //by reading this two registers we negate any pending CIA irqs.
        lda $dd0d  //if we don't do this, a pending CIA irq might occur after we finish setting up our irq.
                   //we don't want that to happen.
        
        // init music
        lda #MSX_ROM
        sta $01
        lda #$00
        jsr MSX_INIT
    }
    *(void **)0x0314 = titleScreenIrq1;     // Install interrupt routine
    vic.intr_enable = 1;             // Enable raster interrupt
    vic.ctrl1 &= 0x7f;               // Set raster line for IRQ
    vic.raster = 0;

    __asm {
        cli
    }

    setBank(TRANSLATION_EN_BANK);
    loadTranslation();

    // show sprites for menu text
    #define TITLE_SPR_POS_X 12
    #define TITLE_SPR_POS_Y 53

    vic.spr_enable = 0;
    vic.spr_expand_x = 0b00000000;
    vic.spr_expand_y = 0b00000000;
    vic.spr_priority = 0b00000000;
    vic.spr_multi    = 0b00000000;

    vic.spr_pos[0].y = TITLE_SPR_POS_Y;
    vic.spr_pos[1].y = TITLE_SPR_POS_Y;
    vic.spr_pos[2].y = TITLE_SPR_POS_Y;
    // 2nd row
    vic.spr_pos[3].y = TITLE_SPR_POS_Y+21;
    vic.spr_pos[4].y = TITLE_SPR_POS_Y+21;
    vic.spr_pos[5].y = TITLE_SPR_POS_Y+21;
    // vic.spr_pos[6].y = TITLE_SPR_POS_Y+21;
    // vic.spr_pos[7].y = TITLE_SPR_POS_Y+21;
    
    vic.spr_msbx = 0b11111111;

    GFX_2_SCR[OFFSET_SPRITE_PTRS+0] = 0x10;
    GFX_2_SCR[OFFSET_SPRITE_PTRS+1] = 0x11;
    GFX_2_SCR[OFFSET_SPRITE_PTRS+2] = 0x12;
    GFX_2_SCR[OFFSET_SPRITE_PTRS+3] = 0x13;
    GFX_2_SCR[OFFSET_SPRITE_PTRS+4] = 0x14;
    GFX_2_SCR[OFFSET_SPRITE_PTRS+5] = 0x15;
    // GFX_2_SCR[OFFSET_SPRITE_PTRS+6] = 0x16;
    // GFX_2_SCR[OFFSET_SPRITE_PTRS+7] = 0x17;

    vic.spr_pos[0].x = TITLE_SPR_POS_X+24*0;
    vic.spr_pos[1].x = TITLE_SPR_POS_X+24*1;
    vic.spr_pos[2].x = TITLE_SPR_POS_X+24*2;
    // 2nd row
    vic.spr_pos[3].x = TITLE_SPR_POS_X+24*0;
    vic.spr_pos[4].x = TITLE_SPR_POS_X+24*1;
    vic.spr_pos[5].x = TITLE_SPR_POS_X+24*2;
    // vic.spr_pos[6].x = TITLE_SPR_POS_X+24*3;
    // vic.spr_pos[7].x = TITLE_SPR_POS_X+24*4;

    vic.spr_color[0] = VCOL_MED_GREY;
    vic.spr_color[1] = VCOL_MED_GREY;
    vic.spr_color[2] = VCOL_MED_GREY;
    vic.spr_color[3] = VCOL_MED_GREY;
    vic.spr_color[4] = VCOL_MED_GREY;
    vic.spr_color[5] = VCOL_MED_GREY;
    // vic.spr_color[6] = VCOL_MED_GREY;
    // vic.spr_color[7] = VCOL_MED_GREY;


    memset((char *)0xc400, 0, 64*6);
    textToSprite((char *)s"  Press   any key to start     a       new     game.", 3, (char *)0xc400);
    vic.spr_enable = 0b00111111;

    // ready steady GO
    do { keyb_poll(); rand();} while (!keyb_key);
    keyb_key = 0;
    // disable sprites
    vic.spr_enable = 0;

    // get the main sprites
    loadMainGfx();

    // splash and turn screen off
    // splashScreen(false, 1);

    mainLoop();
}

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )