#include <c64/vic.h>
#include <c64/cia.h>
#include <c64/types.h>
#include <c64/memmap.h>
#include <c64/rasterirq.h>

#include <engine/joystick.h>
#include <assets/assetsSettings.h>
#include <engine/gameSettings.h>
#include <translation/common.h>

// screen will be split at these lines
#define IRQ_RASTER_TOP_MC_SCREEN 0x2e
#define IRQ_RASTER_MIDDLE_TXT_SCREEN 0x8f
// special irqs to control sprites
#define IRQ_RASTER_BOTTOM_SCROLL_ETC 0xee
#define IRQ_RASTER_TOP_UI_SPRITES 0x01

static byte _scrollIt = 7;
static byte _emptyStatus[] = {0xff, 0};
#define statusLine 24*40

static byte _prevRomCfg;

// init joysticks to neutral pos
static byte _joy1Status = 255;
static byte _joy2Status = 127;

/* ================================================================================
Play msx, if enabled
================================================================================ */
void playMsx(){
    if(gms_enableMusic){
        byte _prevRomCfgPC = ((byte *)0x01)[0];
        __asm {
            lda #MSX_ROM
            sta $01
            jsr MSX_PLAY
        };
        ((byte *)0x01)[0] = _prevRomCfgPC;
    }
}

/* ================================================================================
Deal with joystick anims etc
================================================================================ */
void joyUpdate(){
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
}
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
__interrupt static void IRQ_topMCGfxScreen() {
    // vic.color_back++;
    // vic.color_border++;
    // Select GFX screen
    // vic_setmode(VICM_HIRES_MC, GFX_1_SCR, GFX_1_BMP);
    vic.ctrl1 = VIC_CTRL1_BMM | VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3;
    vic.ctrl2 = VIC_CTRL2_MCM | VIC_CTRL2_CSEL | 0;
    vic.memptr = d018_gfx1;

    // show any sprites the menu might have
    setSpritesTopScr();
    // tick the game
    _timeControl();

    // indicate frame position
    gms_framePos = FRAME_TOP;

    // vic.color_back--;
    // vic.color_border--;
}

__interrupt static void IRQ_topTxtScreen() {
    // vic.color_back++;
    // vic.color_border++;
    // Select TEXT screen
    vic.ctrl1 = VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3;
    vic.ctrl2 = VIC_CTRL2_CSEL | 0;
    vic.memptr = d018_txt1;

    // tick the game
    _timeControl();

    // indicate frame position
    gms_framePos = FRAME_TOP;

    // vic.color_back--;
    // vic.color_border--;
}

__interrupt static void IRQ_topMCTxtScreen() {
    // vic.color_back++;
    // vic.color_border++;
    // Select TEXT screen
    vic.ctrl1 = VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3;
    vic.ctrl2 = VIC_CTRL2_CSEL | VIC_CTRL2_MCM;
    vic.memptr = d018_txt1;

    // tick the game
    _timeControl();

    // indicate frame position
    gms_framePos = FRAME_TOP;

    // vic.color_back--;
    // vic.color_border--;
}

// /* ================================================================================
// Middle raster/split
// ================================================================================ */
__interrupt static void IRQ_middleScreenMsx() {
    playMsx();

    // tick the game
    _timeControl();

    // indicate frame position
    gms_framePos = FRAME_MIDDLE;
}

__interrupt static void IRQ_middleTxtScreen_C() {
    // Select TEXT screen
    vic.ctrl1 = VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3;
    vic.ctrl2 = VIC_CTRL2_CSEL | 0;
    vic.memptr = d018_txt1;

    setSpritesBottomScr();
    IRQ_middleScreenMsx();
}

void IRQ_middleTxtScreen(){
    __asm {
        // inc $d020
        // wait for raster
        ldy #IRQ_RASTER_MIDDLE_TXT_SCREEN+1
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
    l5: ldx #$08
    l6: dex
        bne l6
        nop
        nop
        nop
        iny
        cpy $d012
        beq l7
        cpy $ea
    l7: ldx #$01
    l8: dex
        bne l8
        nop
        iny
        cpy $d012
        bne l9
    l9: ldx #$02
    la: dex
        bne la
        // call C routine
        jsr IRQ_middleTxtScreen_C
    }
}
/* ================================================================================
Control IRQ
================================================================================ */
__interrupt static void IRQ_bottomScrollAndUISprites_C() {
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

    // vic.color_border++;
    playMsx();

    if((char)--_scrollIt==0xff) {
        _scrollIt = 7;
        // Hard scroll
        for(byte i=0;i<39;i++) {
            GFX_1_SCR[statusLine+i]=GFX_1_SCR[statusLine+i+1];
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

    _timeControl();
    // vic.color_border--;

    joyUpdate();

    // vic.color_border--;
    // vic.color_back--;
}

void IRQ_bottomScrollAndUISprites(){
    __asm {
        // wait for raster
        ldy #IRQ_RASTER_BOTTOM_SCROLL_ETC+1
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
        jsr IRQ_bottomScrollAndUISprites_C
    }
}
//********************************************
__interrupt static void IRQ_topUISprites() {

    // VICII->BORDER_COLOR = BLUE;
    showUiSpritesTop();
    // vic.color_border--;
}

/* ================================================================================
Init routine
================================================================================ */
RIRQCode rirqc_topUISprites, rirqc_botomUISprites, rirqc_topScreen, rirqc_middleScreen;

// main init raster must be called first, this one just remaps some IRQs
void initRasterIRQ_TxtMode(){
    // Top - switch to txt, play music
    rirq_build(&rirqc_topScreen, 2);
    rirq_call(&rirqc_topScreen, 0, IRQ_topTxtScreen);
    rirq_call(&rirqc_topScreen, 1, setSpritesBottomScr);
    rirq_set(1, IRQ_RASTER_TOP_MC_SCREEN, &rirqc_topScreen);

    // Middle - play msx
    rirq_build(&rirqc_middleScreen, 1);
    rirq_call(&rirqc_middleScreen, 0, IRQ_middleScreenMsx);
    rirq_set(2, IRQ_RASTER_MIDDLE_TXT_SCREEN, &rirqc_middleScreen);

    // sort the raster IRQs
    rirq_sort();
}

// main init raster must be called first, this one just remaps some IRQs
void initRasterIRQ_MCTxtMode(){
    // Top - switch to txt, play music
    rirq_build(&rirqc_topScreen, 1);
    rirq_call(&rirqc_topScreen, 0, IRQ_topMCTxtScreen);
    rirq_set(1, IRQ_RASTER_TOP_MC_SCREEN, &rirqc_topScreen);

    // Middle - play msx
    rirq_build(&rirqc_middleScreen, 1);
    rirq_call(&rirqc_middleScreen, 0, IRQ_middleScreenMsx);
    rirq_set(2, IRQ_RASTER_MIDDLE_TXT_SCREEN, &rirqc_middleScreen);

    // sort the raster IRQs
    rirq_sort();
}

// main init raster must be called first, this one just remaps some IRQs
void initRasterIRQ_HiresTxtMode(){
    // Top - switch to txt, play music
    rirq_build(&rirqc_topScreen, 1);
    rirq_call(&rirqc_topScreen, 0, IRQ_topTxtScreen);
    rirq_set(1, IRQ_RASTER_TOP_MC_SCREEN, &rirqc_topScreen);

    // Middle - play msx
    rirq_build(&rirqc_middleScreen, 1);
    rirq_call(&rirqc_middleScreen, 0, IRQ_middleScreenMsx);
    rirq_set(2, IRQ_RASTER_MIDDLE_TXT_SCREEN, &rirqc_middleScreen);

    // sort the raster IRQs
    rirq_sort();
}

// main IRQ routine
void initRasterIRQ_SplitMCTxt(){
    // Top
    rirq_build(&rirqc_topUISprites, 1);
    // rirq_write(&topUISprites, 0, &vic.ctrl1, VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3 );
    // rirq_write(&topUISprites, 1, &vic.ctrl2, VIC_CTRL2_CSEL | VIC_CTRL2_MCM );
    rirq_call(&rirqc_topUISprites, 0, IRQ_topUISprites);
    rirq_set(0, IRQ_RASTER_TOP_UI_SPRITES, &rirqc_topUISprites);

    // Top - switch to MC GFX
    rirq_build(&rirqc_topScreen, 1);
    rirq_call(&rirqc_topScreen, 0, IRQ_topMCGfxScreen);
    rirq_set(1, IRQ_RASTER_TOP_MC_SCREEN, &rirqc_topScreen);

    // Middle - switch to TXT, play msx
    rirq_build(&rirqc_middleScreen, 1);
    rirq_call(&rirqc_middleScreen, 0, IRQ_middleTxtScreen);
    rirq_set(2, IRQ_RASTER_MIDDLE_TXT_SCREEN, &rirqc_middleScreen);

    // Bottom - Open borders + Scroll + Sprites
    rirq_build(&rirqc_botomUISprites, 1);
    rirq_call(&rirqc_botomUISprites, 0, IRQ_bottomScrollAndUISprites);
    // Place it into the last line of the screen
    rirq_set(3, IRQ_RASTER_BOTTOM_SCROLL_ETC, &rirqc_botomUISprites);

    // sort the raster IRQs
    rirq_sort();

    // GFX_1_SCR, GFX_1_FNT
    cia2.pra = dd00_gfx1;
    vic.memptr = d018_gfx1;
}

void initRasterIRQ(){

    // stop IRQs and change to ours
    rirq_stop();
    
    // msx off
    ((byte *)0xd418)[0] &= ~0xf;
    // screen off, sprites off
    // vic.ctrl1 = VIC_CTRL1_BMM | VIC_CTRL1_RSEL | 3;
    vic.spr_enable   = 0b00000000;

    __asm {
        // init music
        lda #MSX_ROM
        sta $01
        lda #$01
        jsr MSX_INIT
    }

    // if you use the mmap_trampoline() you have to call the mmap_set() at least once to init the shadow variable
    mmap_set(MMAP_ROM);
    // Activate trampoline
    mmap_trampoline();
    // Disable CIA interrupts, we do not want interference
    // with our joystick interrupt
    cia_init();
    // clean 0xffff - so we don't have artefacts when we open borders
    ((char *)0xffff)[0] = 0;

    // initialize raster IRQ
    rirq_init(true);

    // load standard IRQ routine
    initRasterIRQ_SplitMCTxt();

    __asm {
        // set ALL_ROM as default
        lda #$27
        sta $00
        lda #$37
        sta _prevRomCfg
        sta $01
    }

    // start raster IRQ processing
    rirq_start();
}

void switchScreenToFullTxt(){
    rirq_wait();
    initRasterIRQ_TxtMode();
}

void switchScreenToSplitMcTxt(){
    rirq_wait();
    initRasterIRQ_SplitMCTxt();
}

void switchScreenToFullMCTxt(){
    rirq_wait();
    initRasterIRQ_MCTxtMode();
}

void switchScreenToFullHiresTxt(){
    rirq_wait();
    initRasterIRQ_HiresTxtMode();
}
