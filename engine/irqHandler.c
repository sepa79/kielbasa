#include <c64/vic.h>
#include <c64/cia.h>
#include <c64/types.h>

#include <engine/joystick.h>
#include <assets/assetsSettings.h>
#include <engine/gameSettings.h>
#include <translation/common.h>

/* screen will be split at these lines, sprites will be set to rasterPosX - RASTER_OVERHEAD */
#define RASTER1_POS 0x2e
#define RASTER2_POS 0x8f
#define CONTROL_IRQ1_POS 0xee
#define CONTROL_IRQ2_POS 0x00

static byte _scrollIt = 7;
static byte _emptyStatus[] = {0xff, 0};
#define statusLine 24*40

static byte _prevRomCfg;

// init joysticks to neutral pos
static byte _joy1Status = 255;
static byte _joy2Status = 127;

/* ================================================================================
Deal with time control, pause, character selection etc.
================================================================================ */
static void _timeControl() {
    if(gms_gameSpeedWait != WAIT_TIME_PAUSED){
        if(gms_gameSpeedWait > 0){
            gms_gameSpeedWait--;
        }
    }
}

/* ================================================================================
Top raster/split
================================================================================ */
__interrupt static void splitScreenIRQ1_C() {
    // vic.color_back++;
    // vic.color_border++;
    // Select GFX screen
    // vic_setmode(VICM_HIRES_MC, GFX_1_SCR, GFX_1_BMP);
    vic.ctrl1 = VIC_CTRL1_BMM | VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3;
    vic.ctrl2 = VIC_CTRL2_MCM | VIC_CTRL2_CSEL | 0;
    vic.memptr = d018_gfx1;

    // store/restore memory config, as we need to ensure BASIC ROM is on
    _prevRomCfg = ((byte *)0x01)[0];
    // show any sprites the menu might have
    setSpritesTopScr();
    ((byte *)0x01)[0] = _prevRomCfg;
    // tick the game
    _timeControl();

    // set the irq to 2nd routine
    *(void **)0x0314 = splitScreenIRQ2;
    vic.raster = RASTER2_POS;

    // indicate frame position
    gms_framePos = FRAME_TOP;

    // vic.color_back--;
    // vic.color_border--;
}

// a bit awkward looking, but necessary so Oscar64 can handle IRQs in assembly and C correctly.
void splitScreenIRQ1(){
    __asm {
        // inc $d020
        // wait for raster
        ldy #RASTER1_POS+1
    l1: cpy $d012
        bne l1
        ldx #$09
    l2: dex
        bne l2
        nop
        nop
        nop
        iny
        cpy $d012
        beq l5
        nop
        nop
    l5: ldx #$09
    l6: dex
        bne l6
        nop
        nop
        nop
        iny
        cpy $d012
        beq l7
        cpy $ea
    l7: ldx #$09
    l8: dex
        bne l8
        nop
        iny
        cpy $d012
        bne l9
    l9: ldx #$04
    la: dex
        bne la

        // call C routine
        jsr splitScreenIRQ1_C

        asl $d019   // Ack interrupt
        jmp $ea81   // System IRQ routine
    }
}

// /* ================================================================================
// Bottom raster/split
// ================================================================================ */
__interrupt static void splitScreenIRQ2_C() {
    // vic.color_back++;
    // vic.color_border++;
    // Select TEXT screen
    vic.ctrl1 = VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3;
    vic.ctrl2 = VIC_CTRL2_CSEL | 0;
    vic.memptr = d018_txt1;

    setSpritesBottomScr();

    // tick the game
    _timeControl();

    // set the irq to 3rd routine
    *(void **)0x0314 = controlIRQ1;
    vic.raster = CONTROL_IRQ1_POS;

    // indicate frame position
    gms_framePos = FRAME_MIDDLE;

    // vic.color_back--;
    // vic.color_border--;
}

void splitScreenIRQ2(){
    __asm {
        // inc $d020
        // wait for raster
        ldy #RASTER2_POS+1
    l1: cpy $d012
        bne l1
        ldx #$09
    l2: dex
        bne l2
        nop
        nop
        nop
        iny
        cpy $d012
        beq l5
        nop
        nop
    l5: ldx #$09
    l6: dex
        bne l6
        nop
        nop
        nop
        iny
        cpy $d012
        beq l7
        cpy $ea
    l7: ldx #$06
    l8: dex
        bne l8
        nop
        iny
        cpy $d012
        bne l9
    // l9: ldx #$01
    // la: dex
    //     bne la
    l9:
        // call C routine
        jsr splitScreenIRQ2_C

        asl $d019   // Ack interrupt
        jmp $ea81   // System IRQ routine
    }
}

/* ================================================================================
Control IRQ
================================================================================ */
__interrupt static void controlIRQ1_C() {
    // vic.color_border++;
    // vic.color_back++;
    
    // Soft scroll
    vic.ctrl2 = _scrollIt;
    showUiSpritesBottom();
    
    // wait for right line
    while (vic.raster != 0xfa){}

    // Set screen height to 24 lines - this is done after the border should have started drawing - so it wont start
    vic.ctrl1 &= (0xff^VIC_CTRL1_RSEL);
    while (vic.raster != 0xfc){}
    // no more scrolling, reset it
    vic.ctrl2 = VIC_CTRL2_CSEL | 0;
    // vic.color_border--;

    // Set screen height back to 25 lines (preparing for the next screen)
    vic.ctrl1 |= VIC_CTRL1_RSEL;

    // check if we are in full TXT mode
    // vic.color_border++;
    if(gms_enableMusic){
        _prevRomCfg = ((byte *)0x01)[0];
        __asm {
            lda #$36
            sta $01
            jsr $8003
        };
        ((byte *)0x01)[0] = _prevRomCfg;
    }

    if(--_scrollIt==0xff) {
        _scrollIt = 7;
        // Hard scroll
        for(byte i=0;i<39;i++) {
            GFX_1_SCR[statusLine+i]=GFX_1_SCR[statusLine+i+1]
        }
        // Render next char
        byte c = *SB_TEXT;
        if(c==0) {
            SB_TEXT = _emptyStatus;
            c = *SB_TEXT;
        }
        GFX_1_SCR[statusLine+39] = c;
        SB_TEXT++;
    }

    // time tick
    if(!gms_textMode){
        _timeControl();
    }
    // vic.color_border--;

    // update joy cursor/read keyboard only if cursor is enabled
    if(joyCursor.enabled){
        _joy1Status = ((byte *)0xdc00)[1];
        _joy2Status = ((byte *)0xdc00)[0];
        // vic.color_border--;
        // cursor is sprite 0, always
        // make it flash
        joyCursor.colorIdx++;
        if(SPR_JOY_CURSOR_COLORS[joyCursor.colorIdx] == 0){
            joyCursor.colorIdx = 0;
        }
        // handle joy delay
        if(joyCursor.moveDelayCurrent > 0){
            joyCursor.moveDelayCurrent--;
        }
    }

    // set the irq to 4th routine
    *(void **)0x0314 = controlIRQ2;
    vic.ctrl1 &= 0x7f;
    vic.raster = CONTROL_IRQ2_POS;

    // indicate frame position
    gms_framePos = FRAME_BOTTOM;

    // vic.color_border--;
    // vic.color_back--;
}

void controlIRQ1(){
    __asm {
        // wait for raster
        ldy #CONTROL_IRQ1_POS+1
    l1: cpy $d012
        bne l1
        ldx #$09
    l2: dex
        bne l2
        nop
        nop
        nop
        iny
        cpy $d012
        beq l5
        nop
        nop
    l5: ldx #$09
    l6: dex
        bne l6
        nop
        nop
        nop
        iny
        cpy $d012
        beq l7
        cpy $ea
    l7: ldx #$09
    l8: dex
        bne l8
        nop
        iny
        cpy $d012
        bne l9
    l9: ldx #$06
    la: dex
        bne la

        // call C routine
        jsr controlIRQ1_C

        asl $d019   // Ack interrupt
        jmp $ea81   // System IRQ routine
    }
}
//********************************************
__interrupt static void controlIRQ2_C() {

    // VICII->BORDER_COLOR = BLUE;
    showUiSpritesTop();

    if(gms_textMode){
        // set the irq to 1st control routine
        *(void **)0x0314 = controlIRQ1;
        vic.raster = CONTROL_IRQ1_POS;
    } else {
        // set the irq to 1st routine
        *(void **)0x0314 = splitScreenIRQ1;
        vic.raster = RASTER1_POS;
    }

    // indicate frame position
    gms_framePos = FRAME_TOP_BORDER;

    // vic.color_border--;
}

void controlIRQ2(){
    __asm {
        // inc $d020
        // call C routine
        jsr controlIRQ2_C
        asl $d019   // Ack interrupt
        jmp $ea81   // System IRQ routine
    }
}

/* ================================================================================
Init routine
================================================================================ */
void initRasterIRQ(){
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
        lda #$36
        sta $01
        jsr $8000
    }
    // vic.color_back++;

    // GFX_1_SCR, GFX_1_FNT
    cia2.pra = dd00_gfx1;
    vic.memptr = d018_gfx1;

    *(void **)0x0314 = splitScreenIRQ1;     // Install interrupt routine
    vic.intr_enable = 1;                    // Enable raster interrupt
    vic.ctrl1 &= 0x7f;                      // Set raster line for IRQ
    vic.raster = RASTER1_POS;

    __asm {
        // set ALL_ROM as default
        lda #$27
        sta $00
        lda #$37
        sta _prevRomCfg
        sta $01
        cli
    }
}
