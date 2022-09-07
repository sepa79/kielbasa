
#include <c64/vic.h>
#include <assets/assetsSettings.h>
#include <c64/joystick.h>
#include <c64/sprites.h>
#include <c64/easyflash.h>
#include <c64/memmap.h>

#include <engine/easyFlashBanks.h>
#include <miniGame/pigsleCmdAnims.h>

#define IRQ1RAS 50
#define IRQ2RAS IRQ1RAS + 152

#pragma code ( pigsleCommandRAMCode )
#pragma data ( pigsleCommandRAMData )

static byte _prevRomCfgPC;

// ================================================================================
// Top raster
// ================================================================================
__interrupt static void pigsleCmdIrq1_C() {
    _prevRomCfgPC = ((byte *)0x01)[0];
    if(gms_enableMusic){
        __asm {
            lda #MSX_ROM
            sta $01
            jsr MSX_PLAY
        };
        ((byte *)0x01)[0] = _prevRomCfgPC;
    }

    // Poll joystick
    // joy_poll(0);

    // // Move crosshair coordinates
    CrossX += 2 * joyx[0]; CrossY += 2 * joyy[0];

    // Stop at edges of screen
    if (CrossX < 8)
        CrossX = 8;
    else if (CrossX > 312)
        CrossX = 312;
    if (CrossY < 20)
        CrossY = 20;
    else if (CrossY > 172)
        CrossY = 172;

    // Move crosshair sprite
    // spr_move(0, CrossX + 14, CrossY + 40);

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

    _prevRomCfgPC = ((byte *)0x01)[0];
    mmap_set(MMAP_ROM);
    changeBank(MENU_BANK_PIGSLE_COMMAND_1);

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

    // set the irq to 2nd routine
    *(void **)0x0314 = pigsleCmdIrq2;
    vic.ctrl1 &= 0x7f;
    vic.raster = IRQ2RAS;
}

// a bit awkward looking, but necessary so Oscar64 can handle IRQs in assembly and C correctly.
void pigsleCmdIrq1(){
    __asm {
        inc $d020

        // call C routine
        jsr pigsleCmdIrq1_C

        dec $d020
        asl $d019   // Ack interrupt
        jmp $ea81   // System IRQ routine
    }
}

// ================================================================================
// Bottom raster
// ================================================================================
__interrupt static void pigsleCmdIrq2_C() {
    if(gms_enableMusic){
        _prevRomCfgPC = ((byte *)0x01)[0];
        __asm {
            lda #MSX_ROM
            sta $01
            jsr MSX_PLAY
        };
        ((byte *)0x01)[0] = _prevRomCfgPC;
    }

    // set the irq to 1st routine
    *(void **)0x0314 = pigsleCmdIrq1;
    vic.ctrl1 &= 0x7f;
    vic.raster = IRQ1RAS;
}

// a bit awkward looking, but necessary so Oscar64 can handle IRQs in assembly and C correctly.
void pigsleCmdIrq2(){
    __asm {
        inc $d020

        // call C routine
        jsr pigsleCmdIrq2_C

        dec $d020
        asl $d019   // Ack interrupt
        jmp $ea81   // System IRQ routine
    }
}
