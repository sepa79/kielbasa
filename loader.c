#include <c64/keyboard.h>
#include <c64/memmap.h>
#include <c64/cia.h>
#include <c64/vic.h>
#include <c64/easyflash.h>
#include <c64/charwin.h>

#include <engine/easyFlashBanks.h>
#include <assets/titleScreen.h>
#include <assets/mainGfx.h>
#include <assets/music.h>
#include <translation/textsPL.h>
#include <translation/textsEN.h>
#include "kielbasa.h"

// Shared code/data region, copied from easyflash bank 0 to ram during startup
#pragma region( main, 0x0900, 0x4000, , , { code, data, bss, heap, stack } )

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )

// stage:
//  1 - black to bright
//  2 - bright to black
//  3 - full
void splashScreen(bool turnOn, char stage) {
    // splash screen
    char color[8] = {0x0, 0xe,0xe,0x3, 0xf,0xc,0xb, 0x0};
    char i = 0;

    if(stage & 1){
        do {
            // wait for raster
            vic_waitTop();
            vic.color_border = color[i];
            vic.color_back = color[i];
            vic_waitFrame();
            vic_waitFrame();
            vic_waitFrame();
            i++;
        } while (i<4);
    }

    if(turnOn){
        // screen on
        vic.ctrl1 = VIC_CTRL1_BMM | VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3;
    } else {
        // screen off
        vic.ctrl1 = VIC_CTRL1_BMM | VIC_CTRL1_RSEL | 3;
    }

    i = 4;
    // continue splash
    if(stage & 2){
        do {
            // wait for raster
            vic_waitTop();
            vic.color_border = color[i];
            vic.color_back = color[i];
            vic_waitFrame();
            vic_waitFrame();
            vic_waitFrame();
            i++;
        } while (i<8);
    }
}

int main(void){
    // Enable ROM
    mmap_set(MMAP_ROM);
    // screen off
    vic.ctrl1 = VIC_CTRL1_BMM | VIC_CTRL1_RSEL | 3;

    // Init CIAs (no kernal rom was executed so far)
    cia_init();

    __asm {
        // same init stuff the kernel calls after reset
        // ldx #0
        // stx $d016
        jsr $ff84   // Initialise I/O

        // These may not be needed - depending on what you''ll do
        // jsr $ff87   // Initialise System Constants
        // jsr $ff8a   // Restore Kernal Vectors
        // jsr $ff81   // Initialize screen editor
    }
    
    // this goes directly from ROM - need to switch bank
    eflash.bank = TITLE_SCREEN_BANK;
    loadAndShowTitleScreen();

    // splash and turn screen on
    splashScreen(true, 3);

    // get the main sprites, fonts etc
    eflash.bank = MAIN_GFX_BANK;
    loadMainGfx();

    eflash.bank = TRANSLATION_PL_BANK;
    loadTranslation();

    // ready steady GO
    do { keyb_poll(); } while (!keyb_key);
    keyb_key = 0;

    // splash and turn screen off
    splashScreen(false, 1);

    eflash.bank = MUSIC_BANK;
    loadMusic();

    mainLoop();
}

//-------------------------------------------------------------
// could do with a better home
//-------------------------------------------------------------
inline signed char babs(signed char val){
    if(val < 0){
        return -val;
    }
    return val;
}

void cwin_write_string_raw(CharWin * win, const char * buffer)
{
	char * dp = win->sp;
	for(char y=0; y<win->wy; y++)
	{
		for(char x=0; x<win->wx; x++)
		{
			char ch = *buffer;
			if (ch)
			{
				dp[x] = ch;
				buffer++;
			}
			else
				dp[x] = ' ';
		}
		dp += 40;
	}	

}