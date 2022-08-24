// Switching code generation to shared section
#pragma code ( mainCode )
#pragma data ( mainData )

#include <assets/assetsSettings.h>
#include <c64/vic.h>

#define IRQ1RAS 50
#define IRQ2RAS IRQ1RAS + 152

// Raster IRQ Routine playing music
__interrupt void titleScreenIrq1(){
    // vic.color_border++;
    // play music

    __asm {
        lda #MSX_ROM
        sta $01
        jsr MSX_PLAY
        // dec $d020
        lda #$37
        sta $01
    }
    // set the irq to 2nd routine
    *(void **)0x0314 = titleScreenIrq2;
    vic.ctrl1 &= 0x7f;
    vic.raster = IRQ2RAS;

    __asm {
        asl $d019   // Ack interrupt
        jmp $ea81   // System IRQ routine
    }
}

__interrupt void titleScreenIrq2(){
    // vic.color_border++;
    __asm {
        lda #MSX_ROM
        sta $01
        jsr MSX_PLAY
        // dec $d020
        lda #$37
        sta $01
    }
    // set the irq to 1st routine
    *(void **)0x0314 = titleScreenIrq1;
    vic.ctrl1 &= 0x7f;
    vic.raster = IRQ1RAS;

    __asm {
        asl $d019   // Ack interrupt
        jmp $ea81   // System IRQ routine
    }
}
