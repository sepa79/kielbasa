#include <c64/vic.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>
#include <c64/easyflash.h>
#include <c64/memmap.h>
#include <c64/sprites.h>

#include <menu/menuSystem.h>
#include <menu/pigsleCommand.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/uiHandler.h>
#include <miniGame/pigsleCmdIrq.h>
#include <miniGame/pigsleCmdMain.h>
#include <miniGame/pigsleCmdAnims.h>

// Sections and regions in pigsleCmdMain.h

// ---------------------------------------------------------------------------------------------
// Menu code
// ---------------------------------------------------------------------------------------------
#pragma code ( pigsleCommandCode )
static void _pigsleCmdCodeLoader(){
    // source is where the regionPigsleCommandRam section starts in real mem
    memcpy(MENU_CODE_DST, (char *)0xb000, 0x1000);
}

// Noop - just return - to satisfy menu handlers
static void _pigsleCmdNoop(){
    return;
}

#pragma code ( pigsleCommandRAMCode )
#pragma data ( pigsleCommandRAMData )
#include <c64/rasterirq.h>
RIRQCode bottom, top, borderTop, open;

const struct MenuOption PIGSLE_COMMAND_MENU[] = {
    END_MENU_CHOICES
};

static void _pigsleCmdInit(void){
    // stop IRQs and change to ours
    __asm {
        sei
        // init music
        lda #MSX_ROM
        sta $01
        lda #$02
        jsr MSX_INIT
    }
    // if you use the mmap_trampoline() you have to call the mmap_set() at least once to init the shadow variable
    mmap_set(MMAP_ROM);

    __asm {
        cli
    }


    // Activate trampoline
    mmap_trampoline();
    // Disable CIA interrupts, we do not want interference
    // with our joystick interrupt
    cia_init();

    // initialize raster IRQ
    rirq_init(true);

    // Top - MSX
    rirq_build(&top, 2);
    rirq_write(&top, 0, &vic.ctrl1, VIC_CTRL1_BMM | VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3 );
    rirq_call(&top, 1, pigsleCmdIrq1_C);
    rirq_set(0, IRQ1RAS, &top);

    // Bottom - MSX, Joy
    rirq_build(&bottom, 1);
    rirq_call(&bottom, 0, pigsleCmdIrq2_C);
    rirq_set(1, IRQ2RAS, &bottom);

    rirq_build(&borderTop, 1);
    rirq_call(&borderTop, 0, pigsleCmdIrq);
    rirq_set(2, 80, &borderTop);

    // Build open border raster IRQ
    rirq_build(&open, 1);
    // Reduce vertical screen size to fool VIC counter
    rirq_write(&open, 0, &vic.ctrl1, VIC_CTRL1_BMM |VIC_CTRL1_DEN | 3);
    // Place it into the last line of the screen
    rirq_set(3, 50 + 200 - 3, &open);

    // sort the raster IRQs
    rirq_sort();

    // start raster IRQ processing
    rirq_start();

    // Load GFX
    pigsleScreenInit();
    pigsleSpriteLoader();

    // Init cross hair sprite
    vic.spr_enable   = 0b00000000;
    spr_init(GFX_1_SCR);
    
    vic.spr_expand_x = 0b00000000;
    vic.spr_expand_y = 0b00000000;
    vic.spr_priority = 0b00000000;
    vic.spr_multi    = 0b11111110;
    vic.spr_msbx     = 0b00000000;
    
    // explosion
    vic.spr_mcolor0 = VCOL_WHITE;
    vic.spr_mcolor1 = VCOL_RED;

    GFX_1_SCR[OFFSET_SPRITE_PTRS+0] = PIGSLE_CMD_ANIM_CROSSHAIR_LOADED_BANK;
    vic.spr_color[0] = VCOL_WHITE;

    // Init bitmap
    vic_setmode(VICM_HIRES_MC, GFX_1_SCR, GFX_1_BMP);

    // splash and turn screen on
    splashScreen(true, 3);
    
    // sprites 
    // spr_set(0, true, CrossX + 14, CrossY + 40, PIGSLE_CMD_ANIM_AIM_BANK, 1, false, false, false);

    // start game state machine
    gameState(GS_READY);

    // main loop
    for(;;)
    {
        // vic.color_border++;

        gameLoop();
        // vic_waitFrame();
        rirq_wait();
        // vic.color_border--;

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
};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
