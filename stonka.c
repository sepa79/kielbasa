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

// Make some room
#pragma region(main, 0x0a00, 0xc800, , , {code, data, bss, heap, stack} )

// Joystick and crosshair control
volatile int  CrossX = 160, CrossY = 100;
volatile bool CrossP = false;
volatile char CrossDelay = 0;

#define IRQ1RAS 50
#define IRQ2RAS IRQ1RAS + 152

// Interrupts for msx1 and joystick/msx2 routine
RIRQCode bottom, top;
// Display bitmap
Bitmap sbm;
const ClipRect scr = { 0, 0, 320, 200 };

#define ANIM_EXPLOSION_DELAY 20
#define ANIM_DELAY 2

// Structure for a explosion
struct Explosion{
    int         x, y;    // Center of circle
    char        f;       // frame
    char        d;       // delay
    char        s;       // sprite index
    Explosion * next;    // Next explosion in list
};

// Storage space for explosion
#define EXPLOSION_COUNT 3
Explosion explosions[EXPLOSION_COUNT+1];
// First free and first used explosion
Explosion * efree, * eused;

// ---------------------------------------------------------------------------------------------
// Sprites, gfx and their load routines
// ---------------------------------------------------------------------------------------------

__export const char GFX_FILE[] = {
    #embed 0xffff 2 "assets/multicolorGfx/554.kla"
};
// const char CANNON_FILE[] = {
//     #embed 0xffff 2 "assets/multicolorGfx/dzialoAnim.kla"
// };
// __export const char MSX_FILE[] = {
//     #embed 0xffff 136 "assets/music/FarmGame.sid"
// };
// #define MSX_SIZE 5386-136

#define STONKA_KOALA_BMP GFX_FILE
#define STONKA_KOALA_SCR ((char *)GFX_FILE + 0x1f40)
#define STONKA_KOALA_COL ((char *)GFX_FILE + 0x2328)
// #define STONKA_ANIM_BMP CANNON_FILE
// #define STONKA_ANIM_SCR ((char *)CANNON_FILE + 0x1f40)
// #define STONKA_ANIM_COL ((char *)CANNON_FILE + 0x2328)

#define ANIM_EXPLOSION_BANK 16
#define ANIM_AIM_BANK 16
__export const char SPR_FILE[] = {
    #embed 0xffff 20 "assets/sprites/wybuch.spd"
    // #embed 0xffff 20 "assets/sprites/wybuch2.spd"
};

void screen_init(void){
    vic.color_border = VCOL_BLACK;
    vic.color_back  = VCOL_BLACK;
    // delete any texts that might have been on screen
    bmmcu_rect_fill(&sbm, 0, 0, 320, 100, 0);

//     // load colors
//     char i = 0;
//     do {
// #assign _y 0
// #repeat
//         GFX_1_SCR[_y + i] = STONKA_KOALA_SCR[_y + i];
//         COLOR_RAM[_y + i] = STONKA_KOALA_COL[_y + i];
// #assign _y _y + 256
// #until _y == 1024
//         i++;
//     } while (i != 0);

//     // load bitmap
//     i = 0;
//     do {
// #assign _y 0
// #repeat
//         GFX_1_BMP[_y + i] = STONKA_KOALA_BMP[_y + i];
// #assign _y _y + 256
// #until _y == 8192
//         i++;
//     } while (i != 0);
}

void loadGfx(){
    memcpy(GFX_1_SCR, STONKA_KOALA_SCR, 1000);
    memcpy(COLOR_RAM, STONKA_KOALA_COL, 1000);
    memcpy(GFX_1_BMP, STONKA_KOALA_BMP, 0x1f40);
    screen_init();

    // sprites
    memcpy(GFX_1_SPR, SPR_FILE, 0x40*17);
//     char i = 0;
//     do {
// #assign _y 0
// #repeat
//         GFX_1_SPR[_y + i] = SPR_FILE[_y + i];
// #assign _y _y + 0x100
// #until _y < 0x40*0x09
//         i++;
//     } while (i != 0);
}

// void loadMusic(){
//     // memcpy(MSX_DST_ADR, MSX_FILE, MSX_SIZE);
//     char i = 0;
//     do {
// #assign _y 0
// #repeat
//          ((volatile char*) MSX_DST_ADR)[_y + i] = ((char*) MSX_FILE)[_y + i];
// #assign _y _y + 256
// #until _y < MSX_SIZE
//         i++;
//     } while (i != 0);
// }

#define CANNON_X_POS 16
#define CANNON_Y_POS 19

// copy cannon bitmap - 8x5 chars square, starting 0,0
void copyCannonUp(){
    #define CANNON_FRAME 0
    #pragma unroll(full)
    for(char y=0; y<5; y++)
    {
        #pragma unroll(full)
        for(char x=0; x<8*8; x++)
        {
            GFX_1_BMP[40 * 8 * (CANNON_Y_POS + y) + x + CANNON_X_POS*8] = STONKA_KOALA_BMP[40 * 8 * y + x + CANNON_FRAME*8*8];
        }
    }

    #pragma unroll(full)
    for(char y=0; y<5; y++)
    {
        #pragma unroll(full)
        for(char x=0; x<8; x++)
        {
            GFX_1_SCR[40 * (CANNON_Y_POS + y) + x + CANNON_X_POS] = STONKA_KOALA_SCR[40*y + x + CANNON_FRAME*8];
            COLOR_RAM[40 * (CANNON_Y_POS + y) + x + CANNON_X_POS] = STONKA_KOALA_COL[40*y + x + CANNON_FRAME*8];
        }
    }
}

void copyCannonL60(){
    #define CANNON_FRAME 1
    #pragma unroll(full)
    for(char y=0; y<5; y++)
    {
        #pragma unroll(full)
        for(char x=0; x<8*8; x++)
        {
            GFX_1_BMP[40 * 8 * (CANNON_Y_POS + y) + x + CANNON_X_POS*8] = STONKA_KOALA_BMP[40 * 8 * y + x + CANNON_FRAME*8*8];
        }
    }

    #pragma unroll(full)
    for(char y=0; y<5; y++)
    {
        #pragma unroll(full)
        for(char x=0; x<8; x++)
        {
            GFX_1_SCR[40 * (CANNON_Y_POS + y) + x + CANNON_X_POS] = STONKA_KOALA_SCR[40*y + x + CANNON_FRAME*8];
            COLOR_RAM[40 * (CANNON_Y_POS + y) + x + CANNON_X_POS] = STONKA_KOALA_COL[40*y + x + CANNON_FRAME*8];
        }
    }
}
void copyCannonR60(){
    #define CANNON_FRAME 2
    #pragma unroll(full)
    for(char y=0; y<5; y++)
    {
        #pragma unroll(full)
        for(char x=0; x<8*8; x++)
        {
            GFX_1_BMP[40 * 8 * (CANNON_Y_POS + y) + x + CANNON_X_POS*8] = STONKA_KOALA_BMP[40 * 8 * y + x + CANNON_FRAME*8*8];
        }
    }

    #pragma unroll(full)
    for(char y=0; y<5; y++)
    {
        #pragma unroll(full)
        for(char x=0; x<8; x++)
        {
            GFX_1_SCR[40 * (CANNON_Y_POS + y) + x + CANNON_X_POS] = STONKA_KOALA_SCR[40*y + x + CANNON_FRAME*8];
            COLOR_RAM[40 * (CANNON_Y_POS + y) + x + CANNON_X_POS] = STONKA_KOALA_COL[40*y + x + CANNON_FRAME*8];
        }
    }
}
void copyCannonL75(){
    #define CANNON_FRAME 3
    #pragma unroll(full)
    for(char y=0; y<5; y++)
    {
        #pragma unroll(full)
        for(char x=0; x<8*8; x++)
        {
            GFX_1_BMP[40 * 8 * (CANNON_Y_POS + y) + x + CANNON_X_POS*8] = STONKA_KOALA_BMP[40 * 8 * y + x + CANNON_FRAME*8*8];
        }
    }

    #pragma unroll(full)
    for(char y=0; y<5; y++)
    {
        #pragma unroll(full)
        for(char x=0; x<8; x++)
        {
            GFX_1_SCR[40 * (CANNON_Y_POS + y) + x + CANNON_X_POS] = STONKA_KOALA_SCR[40*y + x + CANNON_FRAME*8];
            COLOR_RAM[40 * (CANNON_Y_POS + y) + x + CANNON_X_POS] = STONKA_KOALA_COL[40*y + x + CANNON_FRAME*8];
        }
    }
}
void copyCannonR75(){
    #define CANNON_FRAME 4
    #pragma unroll(full)
    for(char y=0; y<5; y++)
    {
        #pragma unroll(full)
        for(char x=0; x<8*8; x++)
        {
            GFX_1_BMP[40 * 8 * (CANNON_Y_POS + y) + x + CANNON_X_POS*8] = STONKA_KOALA_BMP[40 * 8 * y + x + CANNON_FRAME*8*8];
        }
    }

    #pragma unroll(full)
    for(char y=0; y<5; y++)
    {
        #pragma unroll(full)
        for(char x=0; x<8; x++)
        {
            GFX_1_SCR[40 * (CANNON_Y_POS + y) + x + CANNON_X_POS] = STONKA_KOALA_SCR[40*y + x + CANNON_FRAME*8];
            COLOR_RAM[40 * (CANNON_Y_POS + y) + x + CANNON_X_POS] = STONKA_KOALA_COL[40*y + x + CANNON_FRAME*8];
        }
    }
}


// ---------------------------------------------------------------------------------------------
// DrMortalWombat's great screen writing code
// ---------------------------------------------------------------------------------------------
// Charset assets
const char MissileChars[] = {
#embed "assets/missilechars.64c"
};

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

// Initialize explosion list
void explosion_init(void){
    // No explosion active
    eused = nullptr;

    // First free explosion element
    efree = explosions;
    // Build list
    for(char i=0; i<EXPLOSION_COUNT; i++){
        explosions[i].s = i + 1; // sprite 0 is aim, so +1 here
        explosions[i].f = 0;
        explosions[i].d = ANIM_DELAY;
        explosions[i].next = explosions + i + 1;
    }
    // Terminate last element
    explosions[EXPLOSION_COUNT].next = nullptr;
}

// Start a new explosion
void explosion_start(int x, int y){
    // Free slot in list of explosions?
    if (efree) {
        // Move entry from free to used list
        Explosion * e = efree;
        efree = e->next;
        e->next = eused;
        eused = e;

        // Initialize position and size
        e->x = x;
        e->y = y;
        spr_set(e->s, true, e->x + 14, e->y + 40, e->f + ANIM_EXPLOSION_BANK, 2, true, false, false);
    }
}

// Animate all explosions
void explosion_animate(void){
    // Loop over active explosions with "e", use "ep" to point
    // to previous explosion, so we can remove the current explosion
    // from the list
    Explosion * e = eused, * ep = nullptr;
    while (e){
        // Remember next entry in list
        Explosion * en = e->next;

        e->d--;
        if(!e->d){
            e->d = ANIM_DELAY;
            // Increment phase
            e->f++;
        }
        // first ANIM_EXPLOSION_DELAY frames are delay
        if (e->f > ANIM_EXPLOSION_DELAY) {
            spr_set(e->s, true, e->x + 14, e->y + 40, e->f + ANIM_EXPLOSION_BANK - ANIM_EXPLOSION_DELAY, 2, true, false, false);
        }
        // End of explosion live
        if (e->f == 9 + ANIM_EXPLOSION_DELAY) {
            // Remove explosion from used list
            if (ep)
                ep->next = e->next;
            else
                eused = e->next;

            // disable sprite, reset anim
            spr_set(e->s, false, e->x + 14, e->y + 40, ANIM_EXPLOSION_BANK, 2, true, false, false);
            e->f = 0;
            e->d = ANIM_DELAY;

            // Prepend it to free list
            e->next = efree;
            efree = e;
        }
        else
            ep = e;

        // set sprite

        // Next explosion in list
        e = en;
    }
}

enum GameState
{
    GS_READY,     // Getting ready
    GS_PLAYING,   // Playing the game
    GS_END        // Wait for restart
};

// State of the game
struct Game
{
    GameState state;
    byte      score, count;

} TheGame;    // Only one game, so global variable

void game_state(GameState state){

    TheGame.state = state;

    switch(state) {
    case GS_READY:
        // Start of new game
        // score_reset();
        screen_init();
        char_write(31, 60, s"Ratuj kartofle!", 1);
        TheGame.count = 15;
        break;

    case GS_PLAYING:
        // Avoid old fire request
        CrossP = false;

        // Setup display
        screen_init();
        // missile_init();
        explosion_init();
        // icbm_init();

        TheGame.count = 15;
        break;

    case GS_END:
        char_write(104, 60, s"THE END", 0);
        TheGame.count = 120;
        break;
    }
}

// Main game play code
void game_play(void)
{
    // Check if fire request
    if (CrossP) {
        // boom!
        explosion_start(CrossX, CrossY);
        // Reset request
        CrossP = false;
    }

    // Wait for next ICMB to enter the game
    if (!--TheGame.count){

        // Next lauch time
        TheGame.count = 8 + (rand() & 63);
    }

    // Advance defending missiles by four pixels
    // for(char i=0; i<4; i++)
    //     missile_animate();

    // Advance ICBMs
    // icbm_animate();

    // Show explosions
    explosion_animate();
}

// Main game loop, entered every VSYNC
void game_loop(){
    switch(TheGame.state){
    case GS_READY:
        if (!--TheGame.count)
            game_state(GS_PLAYING);
        break;

    case GS_PLAYING:
        game_play();

        // Check for level and game end coditions
        break;

    case GS_END:
        if (!--TheGame.count)
            game_state(GS_READY);
        break;
    }
}


int main(void){
    // screen off
    vic.ctrl1 = VIC_CTRL1_BMM | VIC_CTRL1_RSEL | 3;

    // loadMusic();
    // __asm {
    //     // init music
    //     lda #$02
    //     jsr MSX_INIT
    // }

    // Activate trampoline
    mmap_trampoline();
    // if you use the mmap_trampoline() you have to call the mmap_set() at least once to init the shadow variable
    mmap_set(MMAP_NO_ROM);

    // Disable CIA interrupts, we do not want interference
    // with our joystick interrupt
    cia_init();

    // initialize raster IRQ
    rirq_init(true);


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

    // Init bitmap
    vic_setmode(VICM_HIRES_MC, GFX_1_SCR, GFX_1_BMP);
    bm_init(&sbm, GFX_1_BMP, 40, 25);
    loadGfx();
    copyCannonUp();

    // splash and turn screen on
    splashScreen(true, 3);

    // ready steady GO
    // do { keyb_poll(); } while (!keyb_key);
    // keyb_key = 0;

    // Init cross hair sprite
    spr_init(GFX_1_SCR);
    spr_set(0, true, CrossX + 14, CrossY + 40, 16, 1, false, false, false);
    
    // start game state machine
    game_state(GS_READY);
    // vic.color_back = VCOL_BROWN;

    for(;;)
    {
        game_loop();
        rirq_wait();
    }

    return 0;
}

