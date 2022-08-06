#include <c64/keyboard.h>
#include <c64/memmap.h>
#include <c64/cia.h>
#include <c64/vic.h>
// #include <c64/charwin.h>
#include <c64/sprites.h>
#include <c64/joystick.h>
#include <c64/rasterirq.h>

#include <assets/assetsSettings.h>
#include "common.h"
#include "stonkaIrq.h"

// Joystick and crosshair control
volatile int		CrossX = 160, CrossY = 100;
volatile bool	CrossP = false;
volatile char	CrossDelay = 0;

// Interrupts for msx1 and joystick/msx2 routine
RIRQCode	bottom, top;

__export const char GFX_FILE[] = {
    #embed 0xffff 2 "assets/multicolorGfx/88_31.07.22.kla"
};
__export const char MSX_FILE[] = {
    // #embed 0xffff 2 "assets/music/FarmGame.out"
    #embed 0xffff 136 "assets/music/FarmGame.sid"
};

#define STONKA_KOALA_BMP GFX_FILE
#define STONKA_KOALA_SCR ((char *)GFX_FILE + 0x1f40)
#define STONKA_KOALA_COL ((char *)GFX_FILE + 0x2328)

char SPR_FILE[] = {
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x28, 0x00,
	0x00, 0x28, 0x00,
	0x00, 0x28, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,

	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x14, 0x00,
	0x00, 0x69, 0x00,
	0x00, 0xAA, 0x00,
	0x00, 0xAA, 0x00,
	0x00, 0x69, 0x00,
	0x00, 0x14, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,

	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x3C, 0x00,
	0x00, 0xD7, 0x00,
	0x03, 0x69, 0xC0,
	0x01, 0xAA, 0x40,
	0x02, 0xAA, 0x80,
	0x02, 0xAA, 0x80,
	0x02, 0xAA, 0x80,
	0x02, 0xAA, 0x80,
	0x01, 0xAA, 0x40,
	0x03, 0x69, 0xC0,
	0x00, 0xD7, 0x00,
	0x00, 0x3C, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,

	0x00, 0x00, 0x00,
	0x00, 0x3C, 0x00,
	0x00, 0xFF, 0x00,
	0x03, 0xD7, 0xC0,
	0x03, 0x69, 0xF0,
	0x0D, 0xAA, 0x70,
	0x0E, 0xAA, 0x7C,
	0x3E, 0xAA, 0xBC,
	0x36, 0xAA, 0x9C,
	0x36, 0xAA, 0x9C,
	0x36, 0xAA, 0x9C,
	0x36, 0xAA, 0x9C,
	0x36, 0xAA, 0x9C,
	0x0E, 0xAA, 0xB0,
	0x0D, 0xAA, 0x70,
	0x0F, 0x69, 0xC0,
	0x03, 0xD7, 0xC0,
	0x00, 0xFF, 0x00,
	0x00, 0x3C, 0x00,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,

	0x00, 0xFF, 0x00,
	0x03, 0xD7, 0xC0,
	0x0F, 0x69, 0xF0,
	0x0D, 0xAA, 0x70,
	0x3E, 0xAA, 0x7C,
	0x36, 0xAA, 0x9C,
	0xF6, 0xAA, 0x9F,
	0xDA, 0xAA, 0xAF,
	0xDA, 0xAA, 0xA7,
	0xDA, 0xAA, 0xA7,
	0xDA, 0xAA, 0xA7,
	0xDA, 0xAA, 0xA7,
	0xFA, 0xAA, 0xAC,
	0x36, 0xAA, 0xAC,
	0x36, 0xAA, 0x9C,
	0x3D, 0xAA, 0x7C,
	0x0D, 0xAA, 0x70,
	0x0F, 0x69, 0xF0,
	0x03, 0xD7, 0xC0,
	0x00, 0xFF, 0x00,
	0x00, 0x00, 0x00,

	0x00, 0xFF, 0x00,
	0x03, 0x55, 0xC0,
	0x0D, 0xAA, 0x70,
	0x06, 0xAA, 0x90,
	0x36, 0x82, 0x9C,
	0x1A, 0x00, 0xAC,
	0xDA, 0x00, 0xA7,
	0xE8, 0x00, 0x27,
	0xE8, 0x00, 0x2B,
	0xE8, 0x00, 0x2B,
	0xE8, 0x00, 0x2B,
	0xE8, 0x00, 0x2B,
	0xD8, 0x00, 0x24,
	0x1A, 0x00, 0xA4,
	0x3A, 0x00, 0xAC,
	0x36, 0x82, 0xAC,
	0x06, 0xAA, 0x90,
	0x0D, 0xAA, 0x70,
	0x03, 0x55, 0xC0,
	0x00, 0xFF, 0x00,
	0x00, 0x00, 0x00,

	0x00, 0xFF, 0x00,
	0x03, 0x55, 0xC0,
	0x0D, 0xAA, 0x70,
	0x06, 0x00, 0x90,
	0x34, 0x00, 0x1C,
	0x18, 0x00, 0x2C,
	0xD0, 0x00, 0x27,
	0xE0, 0x00, 0x07,
	0xE0, 0x00, 0x0B,
	0xE0, 0x00, 0x0B,
	0xE0, 0x00, 0x0B,
	0xE0, 0x00, 0x0B,
	0xD0, 0x00, 0x04,
	0x18, 0x00, 0x04,
	0x38, 0x00, 0x2C,
	0x34, 0x00, 0x2C,
	0x06, 0x00, 0x90,
	0x0D, 0xAA, 0x70,
	0x03, 0x55, 0xC0,
	0x00, 0xFF, 0x00,
	0x00, 0x00, 0x00,

	0x00, 0xFF, 0x00,
	0x03, 0xAA, 0xC0,
	0x0E, 0x00, 0xB0,
	0x08, 0x00, 0x20,
	0x34, 0x00, 0x1C,
	0x24, 0x00, 0x1C,
	0xE0, 0x00, 0x0B,
	0xE0, 0x00, 0x0B,
	0xE0, 0x00, 0x0B,
	0xE0, 0x00, 0x0B,
	0xE0, 0x00, 0x0B,
	0xE0, 0x00, 0x0B,
	0xE0, 0x00, 0x08,
	0x20, 0x00, 0x08,
	0x30, 0x00, 0x0C,
	0x34, 0x00, 0x1C,
	0x08, 0x00, 0x20,
	0x0E, 0x00, 0xB0,
	0x03, 0xAA, 0xC0,
	0x00, 0xFF, 0x00,
	0x00, 0x00, 0x00,

	0x00, 0xFF, 0x00,
	0x03, 0x00, 0xC0,
	0x0C, 0x00, 0x30,
	0x00, 0x00, 0x00,
	0x30, 0x00, 0x0C,
	0x00, 0x00, 0x0C,
	0xC0, 0x00, 0x03,
	0xC0, 0x00, 0x03,
	0xC0, 0x00, 0x03,
	0xC0, 0x00, 0x03,
	0xC0, 0x00, 0x03,
	0xC0, 0x00, 0x03,
	0xC0, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x30, 0x00, 0x0C,
	0x30, 0x00, 0x0C,
	0x00, 0x00, 0x00,
	0x0C, 0x00, 0x30,
	0x03, 0x00, 0xC0,
	0x00, 0xFF, 0x00,
	0x00, 0x00, 0x00
};

void loadGfx(){
    vic.color_border = VCOL_BLACK;
    vic.color_back  = VCOL_BLACK;

    // load colors
    char i = 0;
    do {
#assign y 0
#repeat
        GFX_1_SCR[y + i] = STONKA_KOALA_SCR[y + i];
        COLOR_RAM[y + i] = STONKA_KOALA_COL[y + i];
#assign y y + 256
#until y == 1024
        i++;
    } while (i != 0);

    // load bitmap
    i = 0;
    do {
#assign y 0
#repeat
        GFX_1_BMP[y + i] = STONKA_KOALA_BMP[y + i];
#assign y y + 256
#until y == 8192
        i++;
    } while (i != 0);

    // sprites
    i = 0;
    do {
#assign y 0
#repeat
        GFX_1_SPR[y + i] = SPR_FILE[y + i];
#assign y y + 0x100
#until y == 0x100*0x10
        i++;
    } while (i != 0);
}

void loadMusic(){
    char i = 0;
    do {
#assign y 0
#repeat
         ((volatile char*) MSX_DST_ADR)[y + i] = ((char*) MSX_FILE)[y + i];
#assign y y + 256
#until y == 0x2000
        i++;
    } while (i != 0);

}

int main(void){
    // screen off
    vic.ctrl1 = VIC_CTRL1_BMM | VIC_CTRL1_RSEL | 3;

    loadMusic();
    __asm {
        // init music
        lda #$02
        jsr MSX_INIT
    }

	// Activate trampoline
	mmap_trampoline();
	// if you use the mmap_trampoline() you have to call the mmap_set() at least once to init the shadow variable
	mmap_set(MMAP_ROM);

	// Disable CIA interrupts, we do not want interference
	// with our joystick interrupt
	cia_init();

	// initialize raster IRQ
	rirq_init(true);

    loadGfx();

	// Top - MSX
	rirq_build(&top, 1);
	rirq_call(&top, 0, msxIrq1);
	rirq_set(0, 57, &top);

	// Bottom - MSX, Joy
	rirq_build(&bottom, 1);
	rirq_call(&bottom, 0, msxIrq2);
	rirq_set(1, 250, &bottom);

	// sort the raster IRQs
	rirq_sort();

	// start raster IRQ processing
	rirq_start();

    // vic_setmode(VICM_HIRES_MC, GFX_1_SCR, GFX_1_BMP);
    vic.ctrl2 = VIC_CTRL2_MCM | VIC_CTRL2_CSEL | 0;
    cia2.pra = dd00_gfx1;
    vic.memptr = d018_gfx1;

    // splash and turn screen on
    splashScreen(true, 3);

    // ready steady GO
    // do { keyb_poll(); } while (!keyb_key);
    // keyb_key = 0;

	// Init cross hair sprite
	spr_init(GFX_1_SCR);
	spr_set(0, true, CrossX + 14, CrossY + 40, 64, 1, false, false, false);
	for(;;)
	{
		// game_loop();
		rirq_wait();
	}

	return 0;
}
