#include <c64/keyboard.h>
#include <c64/vic.h>
#include <c64/memmap.h>
#include <c64/sprites.h>
#include <c64/joystick.h>
#include <c64/rasterirq.h>
#include <c64/cia.h>
#include <gfx/mcbitmap.h>
#include <string.h>
#include <stdlib.h>

#include <assets/assetsSettings.h>
#include "common.h"
#include "stonkaIrq.h"

// Joystick and crosshair control
volatile int  CrossX = 160, CrossY = 100;
volatile bool CrossP = false;
volatile char CrossDelay = 0;

#define IRQ1RAS 50
#define IRQ2RAS IRQ1RAS + 152

// Interrupts for msx1 and joystick/msx2 routine
RIRQCode bottom, top;

// ---------------------------------------------------------------------------------------------
// Sprites, gfx and their load routines
// ---------------------------------------------------------------------------------------------

__export const char GFX_FILE[] = {
    #embed 0xffff 2 "assets/multicolorGfx/dziao_pion_2.kla"
};
// __export const char CANNON_FILE[] = {
//     #embed 0xffff 2 "assets/multicolorGfx/dzialoAnim.kla"
// };
__export const char MSX_FILE[] = {
    // #embed 0xffff 2 "assets/music/FarmGame.out"
    #embed 0xffff 136 "assets/music/FarmGame.sid"
};

#define STONKA_KOALA_BMP GFX_FILE
#define STONKA_KOALA_SCR ((char *)GFX_FILE + 0x1f40)
#define STONKA_KOALA_COL ((char *)GFX_FILE + 0x2328)
#define STONKA_ANIM_BMP CANNON_FILE
#define STONKA_ANIM_SCR ((char *)CANNON_FILE + 0x1f40)
#define STONKA_ANIM_COL ((char *)CANNON_FILE + 0x2328)

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

void screen_init(void){
    vic.color_border = VCOL_BLACK;
    vic.color_back  = VCOL_BLACK;
    // load colors
    char i = 0;
    do {
#assign _y 0
#repeat
        GFX_1_SCR[_y + i] = STONKA_KOALA_SCR[_y + i];
        COLOR_RAM[_y + i] = STONKA_KOALA_COL[_y + i];
#assign _y _y + 256
#until _y == 1024
        i++;
    } while (i != 0);

    // load bitmap
    i = 0;
    do {
#assign _y 0
#repeat
        GFX_1_BMP[_y + i] = STONKA_KOALA_BMP[_y + i];
#assign _y _y + 256
#until _y == 8192
        i++;
    } while (i != 0);
}

void loadGfx(){
    screen_init();
    // sprites
    char i = 0;
    do {
#assign _y 0
#repeat
        GFX_1_SPR[_y + i] = SPR_FILE[_y + i];
#assign _y _y + 0x100
#until _y < 0x40*0x09
        i++;
    } while (i != 0);
}

void loadMusic(){
    char i = 0;
    do {
#assign _y 0
#repeat
         ((volatile char*) MSX_DST_ADR)[_y + i] = ((char*) MSX_FILE)[_y + i];
#assign _y _y + 256
#until _y == 0x2000
        i++;
    } while (i != 0);

}

// void copyCannonUp(){

// }
// void copyCannonL60(){
//     // load bitmap - 8x5 chars square, starting 0,0
//     for(char y=0; y<5; y++){
//         #pragma unroll(full)
//         for(char x=0; x<8*8; x++)
//             GFX_1_BMP[y*40*8 + x] = STONKA_ANIM_BMP[y*40*8 + x];
//     }
// }

// ---------------------------------------------------------------------------------------------
// DrMortalWombat's great screen writing code
// ---------------------------------------------------------------------------------------------
// Charset assets
const char MissileChars[] = {
#embed "assets/missilechars.64c"    
};
// Display bitmap
Bitmap            sbm;
// const ClipRect    scr = { 0, 0, 320, 200 };

// Expand an 8x8 charactor to 16x16 on screen
void char_put(char cx, char cy, char c, char color){
    // Get pointer to glyph data
    const char * sp = MissileChars + 8 * c;

    // Loop over all pixel
    for(char y=0; y<8; y++)
    {
        char cl = sp[y];
        for(char x=0; x<8; x++)
        {
            // Draw two pixel if bit is set
            if (cl & 128)
            {
                bmmc_put(&sbm, cx + 2 * x, cy + 2 * y + 0, color);
                bmmc_put(&sbm, cx + 2 * x, cy + 2 * y + 1, color);
            }

            // Next bit
            cl <<= 1;
        }
    }
}

// Write a zero terminated string on screen
void char_write(char cx, char cy, const char * s, char color)
{
    // Loop over all characters
    while (*s)
    {
        char_put(cx, cy, *s, color);
        s++;
        cx += 16;
    }
}

// ---------------------------------------------------------------------------------------------
// Game code
// ---------------------------------------------------------------------------------------------

// enum GameState
// {
//     GS_READY,     // Getting ready
//     GS_PLAYING,   // Playing the game
//     GS_END        // Wait for restart
// };

// // State of the game
// struct Game
// {
//     GameState state;    
//     byte      score, count;

// } TheGame;    // Only one game, so global variable


// void game_state(GameState state)
// {

//     TheGame.state = state;

//     switch(state)
//     {
//     case GS_READY:    
//         // Start of new game
//         // score_reset();
//         screen_init();
//         // char_write(40, 100, s"READY PLAYER 1", 3);
//         TheGame.count = 150;
//         break;

//     case GS_PLAYING:
//         // Avoid old fire request
//         CrossP = false;

//         // Setup display
//         screen_init();
//         // missile_init();
//         // explosion_init();
//         // icbm_init();

//         TheGame.count = 15;
//         break;

//     case GS_END:
//         // char_write(104, 92, s"THE END", 0);
//         TheGame.count = 120;
//         break;
//     }
// }

// // Main game play code
// void game_play(void)
// {
//     // Check if fire request
//     if (CrossP)
//     {
//         // Find lauch site
//         int    sx = 160;
//         if (CrossX < 120)
//             sx = 24;
//         else if (CrossX > 200)
//             sx = 296

//         // Fire missile

//         // Reset request
//         CrossP = false;
//     }

//     // Wait for next ICMB to enter the game
//     if (!--TheGame.count)
//     {    

//         // Next lauch time
//         TheGame.count = 8 + (rand() & 63);
//     }

//     // Advance defending missiles by four pixels
//     // for(char i=0; i<4; i++)        
//     //     missile_animate();

//     // Advance ICBMs
//     // icbm_animate();

//     // Show explosions
//     // explosion_animate();
// }

// // Main game loop, entered every VSYNC
// void game_loop(){
//     switch(TheGame.state){
//     case GS_READY:
//         if (!--TheGame.count)
//             game_state(GS_PLAYING);
//         break;

//     case GS_PLAYING:
//         game_play();

//         // Check for level and game end coditions
//         break;

//     case GS_END:
//         if (!--TheGame.count)
//             game_state(GS_READY);
//         break;
//     }
// }


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
    rirq_set(0, IRQ1RAS, &top);

    // Bottom - MSX, Joy
    rirq_build(&bottom, 1);
    rirq_call(&bottom, 0, msxIrq2);
    rirq_set(1, IRQ2RAS, &bottom);

    // sort the raster IRQs
    rirq_sort();

    // start raster IRQ processing
    rirq_start();

    // vic.ctrl2 = VIC_CTRL2_MCM | VIC_CTRL2_CSEL | 0;
    // cia2.pra = dd00_gfx1;
    // vic.memptr = d018_gfx1;
	vic_setmode(VICM_HIRES_MC, GFX_1_SCR, GFX_1_BMP);

    // Init bitmap
    bm_init(&sbm, GFX_1_BMP, 40, 25);

    // splash and turn screen on
    splashScreen(true, 3);

    // ready steady GO
    // do { keyb_poll(); } while (!keyb_key);
    // keyb_key = 0;

	bmmcu_rect_fill(&sbm, 0, 8, 320, 176, 2);	
    char_write(40, 100, s"Ratuj kartofle!", 3);
    // Init cross hair sprite
    spr_init(GFX_1_SCR);
    spr_set(0, true, CrossX + 14, CrossY + 40, 16, 1, false, false, false);
    // copyCannonL60();
    // start game state machine
	// game_state(GS_READY);

    for(;;)
    {
        // game_loop();
        rirq_wait();
    }

    return 0;
}
