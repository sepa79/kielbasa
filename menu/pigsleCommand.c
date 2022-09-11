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
#include <miniGame/pigsleCmdMain.h>
#include <miniGame/pigsleCmdIrq.h>
#include <miniGame/pigsleCmdAnims.h>

// Sections and regions
#pragma section( pigsleCommandLoaderData, 0 )
#pragma section( pigsleCommandCode, 0 )
#pragma section( pigsleCommandConsts, 0 )
#pragma region( regionPigsleCommandCrt, 0x8000, 0xafff, , MENU_BANK_PIGSLE_COMMAND_1, { pigsleCommandLoaderData, pigsleCommandCode, pigsleCommandConsts } )

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

const struct MenuOption PIGSLE_COMMAND_MENU[] = {
    END_MENU_CHOICES
};

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
    mmap_set(MMAP_ROM);


    *(void **)0x0314 = pigsleCmdIrq1;     // Install interrupt routine
    vic.intr_enable = 1;             // Enable raster interrupt
    vic.ctrl1 &= 0x7f;               // Set raster line for IRQ
    vic.raster = 0;

    __asm {
        cli
    }

    // Load GFX
    pigsleScreenInit();
    pigsleSpriteLoader();
    // changeBank(MENU_BANK_PIGSLE_COMMAND_2);
    // copyCannonUp();
    // restoreBank();

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
    // bm_init(&sbm, GFX_1_BMP, 40, 25);

    // splash and turn screen on
    splashScreen(true, 3);
    
    // sprites 
    // spr_set(0, true, CrossX + 14, CrossY + 40, PIGSLE_CMD_ANIM_AIM_BANK, 1, false, false, false);
    vic.spr_enable = 0b00000001;

    // init crosshair pos
    CrossX = 159;
    CrossY = 100;
    CrossP = false;
    CrossDelay = 0;
    
    // start game state machine
    game_state(GS_READY);

    // main loop
    for(;;)
    {
        // vic.color_border++;

        game_loop();
        vic_waitFrame();
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
