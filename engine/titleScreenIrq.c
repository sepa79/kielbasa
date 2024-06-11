#include <assets/assetsSettings.h>
#include <c64/vic.h>

#include <assets/mainGfx.h>

//-----------------------------------------------------------------------------------------
#pragma code ( gameInitRAMCode )
#pragma data ( gameInitData )
//-----------------------------------------------------------------------------------------

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

volatile char _colorIdx = 0;
volatile char _delay = 3;
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

    _delay --;
    if(!_delay){
        _delay = 3;
        _colorIdx++;
        if(SPR_JOY_CURSOR_COLORS[_colorIdx] == 0){
            _colorIdx = 0;
        }
        vic.spr_color[0] = SPR_JOY_CURSOR_COLORS[_colorIdx];
        vic.spr_color[1] = SPR_JOY_CURSOR_COLORS[_colorIdx];
        vic.spr_color[2] = SPR_JOY_CURSOR_COLORS[_colorIdx];
        vic.spr_color[3] = SPR_JOY_CURSOR_COLORS[_colorIdx];
        vic.spr_color[4] = SPR_JOY_CURSOR_COLORS[_colorIdx];
        vic.spr_color[5] = SPR_JOY_CURSOR_COLORS[_colorIdx];
    }

    __asm {
        asl $d019   // Ack interrupt
        jmp $ea81   // System IRQ routine
    }
}
//-----------------------------------------------------------------------------------------
#pragma code ( code )
#pragma data ( data )
//-----------------------------------------------------------------------------------------
