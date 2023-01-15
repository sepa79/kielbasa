// title screen doubles as main Init - loader uses this crt bank to store all temporary init procedures.

#include <c64/vic.h>
#include <c64/cia.h>
#include <string.h>
#include <stdlib.h>

#include <engine/logger.h>
#include <engine/easyFlashBanks.h>
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
    #embed 0xffff 2 "assets/multicolorGfx/title150622.kla"
    // #embed 0xffff 2 "assets/multicolorGfx/kielbasaEmpire.kla"
};

#pragma code ( titleScreenCRTCode )
#pragma data ( gameInitData )

static void _loadFullKoalaToBMP2(){
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

}

void loadTitleScreen(){
    vic.color_border  = VCOL_BLACK;
    vic.color_back  = VCOL_BLACK;

    _loadFullKoalaToBMP2();

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
    initGame();
}

//-----------------------------------------------------------------------------------------
#pragma code ( gameInitRAMCode )
#pragma data ( gameInitData )
//-----------------------------------------------------------------------------------------

void initGame(){
    loadTitleScreen();
    // splash and turn screen on
    splashScreen(true, 3);

    setBank(MUSIC_BANK);
    loadMusic(MSX_MAIN_THEME_SID_IDX);

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

    // ready steady GO
    do { keyb_poll(); rand();} while (!keyb_key);
    keyb_key = 0;

    // get the main sprites
    loadMainGfx();

    // splash and turn screen off
    // splashScreen(false, 1);

    mainLoop();
}

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )