#include <c64/vic.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>
#include <c64/easyflash.h>
#include <c64/memmap.h>
#include <gfx/mcbitmap.h>

#include <menu/menuSystem.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/uiHandler.h>
#include <engine/pigsleCommandIrq.h>

// Sections and regions
#pragma section( pigsleCommandLoaderData, 0 )
#pragma section( pigsleCommandCode, 0 )
#pragma section( pigsleCommandGfx1, 0 )
#pragma section( pigsleCommandGfx1Loaders, 0 )
#pragma region( bankpigsleCommandC, 0x8000, 0xbfff, , MENU_BANK_PIGSLE_COMMAND, { pigsleCommandLoaderData, pigsleCommandCode } )
#pragma region( bankpigsleCommandG1, 0x8000, 0xbfff, , MENU_BANK_PIGSLE_COMMAND_GFX_1, { pigsleCommandGfx1, pigsleCommandGfx1Loaders } )

// ---------------------------------------------------------------------------------------------
// Cannon Animation and sprite bank + loaders code
// ---------------------------------------------------------------------------------------------

#pragma data ( pigsleCommandGfx1 )
__export const char pigsleCommandGfxBg[] = {
    #embed 0x2713 0x0002 "assets/multicolorGfx/dziao_13.08.22_final.kla"
};

// this should never be in mem, just used by loaders code
const char pigsleCommandGfxCannonAnim[] = {
    #embed 0xffff 0x0002 "assets/multicolorGfx/flak_88_10.08.22.kla"
};

#define PIGSLE_CMD_ANIM_EXPLOSION_BANK 16
#define PIGSLE_CMD_ANIM_AIM_BANK 16
__export const char PIGSLE_CMD_SPR_FILE[] = {
    #embed 0xffff 20 "assets/sprites/wybuch.spd"
    // #embed 0xffff 20 "assets/sprites/wybuch2.spd"
};

#pragma code ( pigsleCommandGfx1Loaders )
#pragma data ( mainData )

static void _loadFullKoala(){
    // load colors
    char i = 0;
    do {
#assign y 0
#repeat
        GFX_1_SCR[y + i] = FULL_KOALA_SCR[y + i];
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
        GFX_1_BMP[y + i] = FULL_KOALA_BMP[y + i];
#assign y y + 256
#until y == 8192
        i++;
    } while (i != 0);

}

static void _spriteLoader(){
    char i = 0;
    do {
#assign y 0
#repeat
       ((volatile char*) GFX_1_SPR_DST_ADR)[y + i] = ((char*)PIGSLE_CMD_SPR_FILE)[y + i];
#assign y y + 0x100
#until y == 0x0400
        i++;
    } while (i != 0);
}

// ---------------------------------------------------------------------------------------------
// Variables for main Pigsle code
// ---------------------------------------------------------------------------------------------

// menu code is in ROM - data in RAM
#pragma code ( pigsleCommandCode )
#pragma data ( mainData )

// Joystick and crosshair control
int  CrossX = 160;
char CrossY = 100;
bool CrossP = false;
char CrossDelay = 0;

// Display bitmap
Bitmap sbm;

// ---------------------------------------------------------------------------------------------
// Cannon Animation handling
// ---------------------------------------------------------------------------------------------

#define STONKA_KOALA_BMP pigsleCommandGfxCannonAnim
#define STONKA_KOALA_SCR ((char *)pigsleCommandGfxCannonAnim + 0x1f40)
#define STONKA_KOALA_COL ((char *)pigsleCommandGfxCannonAnim + 0x2328)

#define CANNON_X_POS 16
#define CANNON_Y_POS 19

// load bitmap - 8x5 chars square, starting 0,0
void copyCannonUp(){
#define CANNON_FRAME 0
#assign _y 0
#repeat
#assign _x 0
#repeat
    GFX_1_BMP[40 * 8 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS*8] = STONKA_KOALA_BMP[40 * 8 * _y + _x + CANNON_FRAME*8*8];
#assign _x _x + 1
#until _x == 8*8
#assign _y _y + 1
#until _y == 5

#assign _y 0
#repeat
#assign _x 0
#repeat
    GFX_1_SCR[40 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS] = STONKA_KOALA_SCR[40*_y + _x + CANNON_FRAME*8];
    COLOR_RAM[40 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS] = STONKA_KOALA_COL[40*_y + _x + CANNON_FRAME*8];
#assign _x _x + 1
#until _x == 8
#assign _y _y + 1
#until _y == 5
}

// load bitmap - 8x5 chars square
void copyCannonL60(){
#define CANNON_FRAME 1
#assign _y 0
#repeat
#assign _x 0
#repeat
    GFX_1_BMP[40 * 8 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS*8] = STONKA_KOALA_BMP[40 * 8 * _y + _x + CANNON_FRAME*8*8];
#assign _x _x + 1
#until _x == 8*8
#assign _y _y + 1
#until _y == 5

#assign _y 0
#repeat
#assign _x 0
#repeat
    GFX_1_SCR[40 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS] = STONKA_KOALA_SCR[40*_y + _x + CANNON_FRAME*8];
    COLOR_RAM[40 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS] = STONKA_KOALA_COL[40*_y + _x + CANNON_FRAME*8];
#assign _x _x + 1
#until _x == 8
#assign _y _y + 1
#until _y == 5
}

// load bitmap - 8x5 chars square
void copyCannonR60(){
#define CANNON_FRAME 2
#assign _y 0
#repeat
#assign _x 0
#repeat
    GFX_1_BMP[40 * 8 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS*8] = STONKA_KOALA_BMP[40 * 8 * _y + _x + CANNON_FRAME*8*8];
#assign _x _x + 1
#until _x == 8*8
#assign _y _y + 1
#until _y == 5

#assign _y 0
#repeat
#assign _x 0
#repeat
    GFX_1_SCR[40 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS] = STONKA_KOALA_SCR[40*_y + _x + CANNON_FRAME*8];
    COLOR_RAM[40 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS] = STONKA_KOALA_COL[40*_y + _x + CANNON_FRAME*8];
#assign _x _x + 1
#until _x == 8
#assign _y _y + 1
#until _y == 5
}

// load bitmap - 8x5 chars square
void copyCannonL75(){
#define CANNON_FRAME 3
#assign _y 0
#repeat
#assign _x 0
#repeat
    GFX_1_BMP[40 * 8 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS*8] = STONKA_KOALA_BMP[40 * 8 * _y + _x + CANNON_FRAME*8*8];
#assign _x _x + 1
#until _x == 8*8
#assign _y _y + 1
#until _y == 5

#assign _y 0
#repeat
#assign _x 0
#repeat
    GFX_1_SCR[40 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS] = STONKA_KOALA_SCR[40*_y + _x + CANNON_FRAME*8];
    COLOR_RAM[40 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS] = STONKA_KOALA_COL[40*_y + _x + CANNON_FRAME*8];
#assign _x _x + 1
#until _x == 8
#assign _y _y + 1
#until _y == 5
}

// load bitmap - 8x5 chars square, starting 0,0
void copyCannonR75(){
#define CANNON_FRAME 4
#assign _y 0
#repeat
#assign _x 0
#repeat
    GFX_1_BMP[40 * 8 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS*8] = STONKA_KOALA_BMP[40 * 8 * _y + _x + CANNON_FRAME*8*8];
#assign _x _x + 1
#until _x == 8*8
#assign _y _y + 1
#until _y == 5

#assign _y 0
#repeat
#assign _x 0
#repeat
    GFX_1_SCR[40 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS] = STONKA_KOALA_SCR[40*_y + _x + CANNON_FRAME*8];
    COLOR_RAM[40 * (CANNON_Y_POS + _y) + _x + CANNON_X_POS] = STONKA_KOALA_COL[40*_y + _x + CANNON_FRAME*8];
#assign _x _x + 1
#until _x == 8
#assign _y _y + 1
#until _y == 5
}

// ---------------------------------------------------------------------------------------------
// Other code
// ---------------------------------------------------------------------------------------------

// Noop - just return - to satisfy menu handlers
static void _pigsleCmdNoop(){
    return;
}

const struct MenuOption PIGSLE_COMMAND_MENU[] = {
    END_MENU_CHOICES
};

// this code needs to be in main block, as it switches banks
#pragma code ( mainCode )
#pragma data ( mainData )

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
    eflash.bank = MENU_BANK_PIGSLE_COMMAND_GFX_1;
    _loadFullKoala();
    _spriteLoader();
    eflash.bank = MENU_BANK_PIGSLE_COMMAND;

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

    // main loop
    for(;;)
    {
        // game_loop();
        // rirq_wait();
    }

}

#pragma data ( pigsleCommandLoaderData )

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &_pigsleCmdNoop,
    .loadMenuGfx     = nullptr,
    .loadMenuSprites = &_pigsleCmdNoop,
    .showMenu        = &_pigsleCmdInit,
    .showSprites     = &_pigsleCmdNoop,
    .updateMenu      = &_pigsleCmdNoop,
}

// Switching code generation back to shared section
#pragma code ( mainCode )
#pragma data ( mainData )
