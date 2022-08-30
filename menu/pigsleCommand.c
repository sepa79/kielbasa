#include <c64/vic.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>
#include <c64/easyflash.h>
#include <c64/memmap.h>
#include <gfx/mcbitmap.h>
#include <c64/sprites.h>

#include <menu/menuSystem.h>
#include <menu/pigsleCommand.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/uiHandler.h>
#include <miniGame/pigsleCmdIrq.h>
#include <miniGame/pigsleCmdAnims.h>

// Sections and regions
#pragma section( pigsleCommandLoaderData, 0 )
#pragma section( pigsleCommandCode, 0 )
#pragma section( pigsleCommandConsts, 0 )
#pragma section( pigsleCommandRAMCode, 0 )
#pragma section( pigsleCommandRAMData, 0 )
#pragma region( bankpigsleCommandC, 0x8000, 0xbfff, , MENU_BANK_PIGSLE_COMMAND_1, { pigsleCommandLoaderData, pigsleCommandCode, pigsleCommandConsts } )

#pragma section( pigsleCommandGfx1, 0 )
#pragma section( pigsleCommandGfx1Loaders, 0 )
#pragma region( bankpigsleCommandG1, 0x8000, 0xbfff, , MENU_BANK_PIGSLE_COMMAND_GFX_1, { pigsleCommandGfx1, pigsleCommandGfx1Loaders } )

// ---------------------------------------------------------------------------------------------
// Cannon Animation and sprite bank + loaders code
// ---------------------------------------------------------------------------------------------

#pragma data ( pigsleCommandGfx1 )
__export const char pigsleCommandGfxBg[] = {
    #embed 0x2713 0x0002 "assets/multicolorGfx/dziao_13.08.22_final.kla"
};

#define PIGSLE_CMD_ANIM_EXPLOSION_BANK 16
#define PIGSLE_CMD_ANIM_AIM_BANK 16
__export const char PIGSLE_CMD_SPR_FILE[] = {
    #embed 0xffff 20 "assets/sprites/wybuch.spd"
    // #embed 0xffff 20 "assets/sprites/wybuch2.spd"
};

#pragma code ( pigsleCommandGfx1Loaders )
#pragma data ( data )

static void _screenInit(){
    // load colors
    char i = 0;
    do {
#assign _y 0
#repeat
        GFX_1_SCR[_y + i] = FULL_KOALA_SCR[_y + i];
        COLOR_RAM[_y + i] = FULL_KOALA_COL[_y + i];
#assign _y _y + 256
#until _y == 1024
        i++;
    } while (i != 0);

    // load bitmap
    i = 0;
    do {
#assign _y 0
#repeat
        GFX_1_BMP[_y + i] = FULL_KOALA_BMP[_y + i];
#assign _y _y + 256
#until _y == 8192
        i++;
    } while (i != 0);

}

static void _spriteLoader(){
    char i = 0;
    do {
#assign _y 0
#repeat
       ((volatile char*) GFX_1_SPR_DST_ADR)[_y + i] = ((char*)PIGSLE_CMD_SPR_FILE)[_y + i];
#assign _y _y + 0x100
#until _y == 0x0400
        i++;
    } while (i != 0);
}

// this code needs to be in main block, as it switches banks
#pragma code ( code )
#pragma data ( data )


static void screenInit(void){
    // vic.color_border = VCOL_RED;
    // vic.color_back  = VCOL_RED;
    // mmap_set(MMAP_ROM);
    changeBank(MENU_BANK_PIGSLE_COMMAND_GFX_1);
    _screenInit();
    restoreBank();
}

// ---------------------------------------------------------------------------------------------
// Variables for main Pigsle code
// ---------------------------------------------------------------------------------------------

// menu code is in ROM - data in RAM
#pragma code ( pigsleCommandCode )
#pragma data ( data )

// Joystick and crosshair control
volatile int  CrossX = 160;
volatile char CrossY = 100;
volatile bool CrossP = false;
volatile char CrossDelay = 0;

// Display bitmap
Bitmap sbm;

// // First free and first used explosion
// Explosion * efree, * eused;
// Explosion explosions[EXPLOSION_COUNT+1];

// // ---------------------------------------------------------------------------------------------
// // DrMortalWombat's great screen writing code
// // ---------------------------------------------------------------------------------------------
// #pragma data ( pigsleCommandConsts )
// // Charset assets
// const char MissileChars[] = {
// #embed "assets/missilechars.64c"
// };
// #pragma data ( data )

// // Expand an 8x8 charactor to 16x16 on screen
// void char_put(char cx, char cy, char c, char color){
//     // Get pointer to glyph data
//     const char * sp = MissileChars + 8 * c;

//     // Loop over all pixel
//     for(char y=0; y<8; y++)
//     {
//         char cl = sp[y];
//         for(char x=0; x<8; x++)
//         {
//             // Draw two pixel if bit is set
//             if (cl & 128)
//             {
//                 bmmc_put(&sbm, cx + 2 * x, cy + 2 * y + 0, color);
//                 bmmc_put(&sbm, cx + 2 * x, cy + 2 * y + 1, color);
//             }

//             // Next bit
//             cl <<= 1;
//         }
//     }
// }

// // Write a zero terminated string on screen
// void char_write(char cx, char cy, const char * s, char color)
// {
//     // Loop over all characters
//     while (*s)
//     {
//         char_put(cx, cy, *s, color);
//         s++;
//         cx += 16;
//     }
// }

// // ---------------------------------------------------------------------------------------------
// // Game code
// // ---------------------------------------------------------------------------------------------

// Noop - just return - to satisfy menu handlers
static void _pigsleCmdNoop(){
    return;
}

static void _pigsleCmdCodeLoader(){
    memcpy(MENU_CODE_DST, MENU_CODE_SRC, 1000);
}

const struct MenuOption PIGSLE_COMMAND_MENU[] = {
    END_MENU_CHOICES
};

// // Initialize explosion list
// static void explosion_init(void){
//     // No explosion active
//     eused = nullptr;

//     // First free explosion element
//     efree = explosions;
//     // Build list
//     for(char i=0; i<EXPLOSION_COUNT; i++){
//         explosions[i].s = i + 1; // sprite 0 is aim, so +1 here
//         explosions[i].f = 0;
//         explosions[i].d = ANIM_DELAY;
//         explosions[i].next = explosions + i + 1;
//     }
//     // Terminate last element
//     explosions[EXPLOSION_COUNT].next = nullptr;
// }

// // Start a new explosion
// static void explosion_start(int x, int y){
//     // Free slot in list of explosions?
//     if (efree) {
//         // Move entry from free to used list
//         Explosion * e = efree;
//         efree = e->next;
//         e->next = eused;
//         eused = e;

//         // Initialize position and size
//         e->x = x;
//         e->y = y;
//         spr_set(e->s, true, e->x + 14, e->y + 40, e->f + PIGSLE_CMD_ANIM_EXPLOSION_BANK, 2, true, false, false);
//     }
// }

// // Animate all explosions
// static void explosion_animate(void){
//     // Loop over active explosions with "e", use "ep" to point
//     // to previous explosion, so we can remove the current explosion
//     // from the list
//     Explosion * e = eused, * ep = nullptr;
//     while (e){
//         // Remember next entry in list
//         Explosion * en = e->next;

//         e->d--;
//         if(!e->d){
//             e->d = ANIM_DELAY;
//             // Increment phase
//             e->f++;
//         }
//         // first ANIM_EXPLOSION_DELAY frames are delay
//         if (e->f > ANIM_EXPLOSION_DELAY) {
//             spr_set(e->s, true, e->x + 14, e->y + 40, e->f + PIGSLE_CMD_ANIM_EXPLOSION_BANK - ANIM_EXPLOSION_DELAY, 2, true, false, false);
//         }
//         // End of explosion live
//         if (e->f == 9 + ANIM_EXPLOSION_DELAY) {
//             // Remove explosion from used list
//             if (ep)
//                 ep->next = e->next;
//             else
//                 eused = e->next;

//             // disable sprite, reset anim
//             spr_set(e->s, false, e->x + 14, e->y + 40, PIGSLE_CMD_ANIM_EXPLOSION_BANK, 2, true, false, false);
//             e->f = 0;
//             e->d = ANIM_DELAY;

//             // Prepend it to free list
//             e->next = efree;
//             efree = e;
//         }
//         else
//             ep = e;

//         // set sprite

//         // Next explosion in list
//         e = en;
//     }
// }

// // State of the game
// struct Game
// {
//     GameState state;
//     byte      score, count;

// } TheGame;    // Only one game, so global variable

// static void game_state(GameState state){

//     TheGame.state = state;

//     switch(state) {
//     case GS_READY:
//         // Start of new game
//         // score_reset();
//         screenInit();
//         char_write(31, 60, s"Ratuj kartofle!", 1);
//         TheGame.count = 25;
//         break;

//     case GS_PLAYING:
//         // Avoid old fire request
//         CrossP = false;

//         // Setup display
//         screenInit();
//         // missile_init();
//         explosion_init();
//         // icbm_init();

//         TheGame.count = 15;
//         break;

//     case GS_END:
//         char_write(104, 60, s"THE END", 0);
//         TheGame.count = 120;
//         break;
//     }
// }

// // Main game play code
// static void game_play(void)
// {
//     vic.color_border++;
//     // Check if fire request
//     if (CrossP) {
//         // boom!
//         explosion_start(CrossX, CrossY);
//         // Reset request
//         CrossP = false;
//     }

//     // Wait for next ICMB to enter the game
//     if (!--TheGame.count){

//         // Next lauch time
//         TheGame.count = 8 + (rand() & 63);
//     }

//     // Advance defending missiles by four pixels
//     // for(char i=0; i<4; i++)
//     //     missile_animate();

//     // Advance ICBMs
//     // icbm_animate();

//     // Show explosions
//     explosion_animate();
// }

// // Main game loop, entered every VSYNC
// static void game_loop(){

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

// this code needs to be in main block, as it switches banks
#pragma code ( code )
#pragma data ( data )

static void _pigsleCmdInit(void){
    // stop IRQs and change to ours
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
        lda #$02
        jsr MSX_INIT
    }

    // Load GFX
    mmap_set(MMAP_ROM);
    changeBank(MENU_BANK_PIGSLE_COMMAND_GFX_1);
    _screenInit();
    _spriteLoader();
    restoreBank();

    *(void **)0x0314 = pigsleCmdIrq1;     // Install interrupt routine
    vic.intr_enable = 1;             // Enable raster interrupt
    vic.ctrl1 &= 0x7f;               // Set raster line for IRQ
    vic.raster = 0;

    __asm {
        cli
    }

    // Init bitmap
    vic_setmode(VICM_HIRES_MC, GFX_1_SCR, GFX_1_BMP);
    bm_init(&sbm, GFX_1_BMP, 40, 25);
    copyCannonUp();

    // splash and turn screen on
    splashScreen(true, 3);

    // Init cross hair sprite
    spr_init(GFX_1_SCR);
    spr_set(0, true, CrossX + 14, CrossY + 40, 16, 1, false, false, false);
    
    // start game state machine
    // game_state(GS_READY);

    // main loop
    for(;;)
    {
        // vic.color_border++;

        // game_loop();
        // vic.color_border--;
        vic_waitFrame();

    }

}

#pragma data ( pigsleCommandLoaderData )

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &_pigsleCmdCodeLoader,
    .loadMenuGfx     = nullptr,
    .loadMenuSprites = &_pigsleCmdNoop,
    .showMenu        = &_pigsleCmdInit,
    .showSprites     = &_pigsleCmdNoop,
    .updateMenu      = &_pigsleCmdNoop,
}

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
