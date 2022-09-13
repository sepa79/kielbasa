
#include <c64/vic.h>
#include <assets/assetsSettings.h>
#include <c64/joystick.h>
#include <c64/sprites.h>
#include <c64/easyflash.h>
#include <c64/memmap.h>

#include <engine/easyFlashBanks.h>
#include <miniGame/pigsleCmdAnims.h>
#include <miniGame/pigsleCmdMain.h>

#define CROSSHAIR_MIN_X 24-6
#define CROSSHAIR_MAX_X 320 + 6
#define CROSSHAIR_MIN_Y 50
#define CROSSHAIR_MAX_Y 172

#pragma code ( pigsleCommandRAMCode )
#pragma data ( pigsleCommandRAMData )

static byte _prevRomCfgPC;
volatile byte _flashDelay = 2;

// ================================================================================
// Top raster
// ================================================================================
__interrupt void pigsleCmdIrq() {
    vic.color_border++;
    for(char i=0;i<20;i++){
        vic.color_border = 0;
    }
    vic.color_border--;
}

__interrupt void pigsleCmdIrq1_C() {
    if(gms_enableMusic){
        _prevRomCfgPC = ((byte *)0x01)[0];
        __asm {
            lda #MSX_ROM
            sta $01
            jsr MSX_PLAY
        };
        ((byte *)0x01)[0] = _prevRomCfgPC;
    }

    // Poll joystick
    joy_poll(0);

    // // Move crosshair coordinates
    CrossX += 1 * joyx[0]; CrossY += 1 * joyy[0];

    // Stop at edges of screen
    if (CrossX < CROSSHAIR_MIN_X)
        CrossX = CROSSHAIR_MIN_X;
    else if (CrossX > CROSSHAIR_MAX_X)
        CrossX = CROSSHAIR_MAX_X;
    if (CrossY < CROSSHAIR_MIN_Y)
        CrossY = CROSSHAIR_MIN_Y;
    else if (CrossY > CROSSHAIR_MAX_Y)
        CrossY = CROSSHAIR_MAX_Y;

    // Move crosshair sprite
    spr_move(0, CrossX, CrossY);

    // Check button
    if (joyb[0]){
        // Avoid quickfire and bouncing
        if (CrossDelay == 0){
            // Request fire from non interrupt code
            CrossP = true;
            CrossDelay = 5;
        }
    }
    else if (CrossDelay > 0)
        CrossDelay--;

    if(!CrossDelay && efree)
        GFX_1_SCR[OFFSET_SPRITE_PTRS+0] = PIGSLE_CMD_ANIM_CROSSHAIR_LOADED_BANK;

    _prevRomCfgPC = ((byte *)0x01)[0];
    mmap_set(MMAP_ROM);
    changeBank(MENU_BANK_PIGSLE_COMMAND_2);

    if(CrossX < 60)
        copyCannonL60();
    else if(CrossX < 140)
        copyCannonL75();
    else if(CrossX < 180)
        copyCannonUp();
    else if(CrossX < 260)
        copyCannonR75();
    else
        copyCannonR60();

    restoreBank();
    ((byte *)0x01)[0] = _prevRomCfgPC;

}

// // a bit awkward looking, but necessary so Oscar64 can handle IRQs in assembly and C correctly.
// void pigsleCmdIrq1(){
//     __asm {
//         // inc $d020

//         // call C routine
//         jsr pigsleCmdIrq1_C
//     }
//     // set the irq to 2nd routine
//     *(void **)0x0314 = pigsleCmdIrq2;
//     vic.ctrl1 &= 0x7f;
//     vic.raster = IRQ2RAS;

//     __asm {
//         // dec $d020
//         asl $d019   // Ack interrupt
//         jmp $ea81   // System IRQ routine
//     }
// }

// ================================================================================
// Bottom raster
// ================================================================================
__interrupt void pigsleCmdIrq2_C() {
    if(gms_enableMusic){
        _prevRomCfgPC = ((byte *)0x01)[0];
        __asm {
            lda #MSX_ROM
            sta $01
            jsr MSX_PLAY
        };
        ((byte *)0x01)[0] = _prevRomCfgPC;
    }

    _flashDelay--;
    if(!_flashDelay){
        joyCursor.colorIdx++;
        if(SPR_JOY_CURSOR_COLORS[joyCursor.colorIdx] == 0){
            joyCursor.colorIdx = 0;
        }
        vic.spr_color[0] = SPR_JOY_CURSOR_COLORS[joyCursor.colorIdx];
        _flashDelay = 2;
    }
}

// // a bit awkward looking, but necessary so Oscar64 can handle IRQs in assembly and C correctly.
// void pigsleCmdIrq2(){
//     __asm {
//         // inc $d020

//         // call C routine
//         jsr pigsleCmdIrq2_C
//     }
//     // set the irq to 1st routine
//     *(void **)0x0314 = pigsleCmdIrq1;
//     vic.ctrl1 &= 0x7f;
//     vic.raster = IRQ1RAS;

//     __asm {
//         // dec $d020
//         asl $d019   // Ack interrupt
//         jmp $ea81   // System IRQ routine
//     }
// }
