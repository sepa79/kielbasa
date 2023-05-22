#include <c64/vic.h>
#include <c64/cia.h>
#include <c64/types.h>
#include <c64/memmap.h>
#include <c64/rasterirq.h>

#include <engine/joystick.h>
#include <engine/irqHandler.h>
#include <assets/assetsSettings.h>
#include <engine/gameSettings.h>
#include <translation/common.h>

// screen will be split at these lines
#define IRQ_RASTER_TOP_MC_SCREEN 0x2e
#define IRQ_RASTER_MIDDLE_TXT_SCREEN 0x91
// special irqs to control sprites
#define IRQ_RASTER_BOTTOM_SCROLL_ETC 0xee
#define IRQ_RASTER_TOP_UI_SPRITES 0x01

volatile bool map_2ndScreen = true;
const char * fontCopySrc = nullptr;
volatile char * fontCopyDst = GFX_1_FNT2;
volatile bool fontCopyDone = true;

// used to check if move to given tile is possible
char * mapScreen;

static byte _scrollIt = 7;
static byte _emptyStatus[] = {0xff, 0};
#define statusLine 24*40

static byte _prevRomCfg;

// init joysticks to neutral pos
static byte _joy1Status = 255;
static byte _joy2Status = 127;

byte currentScreenMode = 0;

/* ================================================================================
Play msx, if enabled
================================================================================ */
static void playMsx(){
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
static void joyUpdate(){
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
    vic.ctrl1 = VIC_CTRL1_BMM | VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3;
    vic.ctrl2 = VIC_CTRL2_MCM | VIC_CTRL2_CSEL | 0;
    vic.memptr = d018_gfx1;
    cia2.pra = dd00_gfx1;

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
    cia2.pra = dd00_gfx1;

    // tick the game
    _timeControl();

    // indicate frame position
    gms_framePos = FRAME_TOP;

    // vic.color_back--;
    // vic.color_border--;
}
__interrupt static void IRQ_topHiresTxtScreen() {
    // vic.color_back++;
    // vic.color_border++;
    // Select TEXT screen
    vic.ctrl1 = VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3;
    vic.ctrl2 = VIC_CTRL2_CSEL | 0;
    cia2.pra = dd00_gfx1;
    if(map_2ndScreen){
        vic.memptr = d018_txt2_3;
        mapScreen = GFX_1_SCR3;
    } else {
        vic.memptr = d018_txt2_2;
        mapScreen = GFX_1_SCR2;
    }

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
    cia2.pra = dd00_gfx1;

    // tick the game
    _timeControl();

    // indicate frame position
    gms_framePos = FRAME_TOP;

    // vic.color_back--;
    // vic.color_border--;
}

__interrupt static void IRQ_topNoScreen() {
    // vic.color_back++;
    // vic.color_border++;
    // screen off
    vic.ctrl1 = VIC_CTRL1_BMM | VIC_CTRL1_RSEL | 3;

    // indicate frame position
    gms_framePos = FRAME_TOP;
    if(!fontCopyDone) {
        // ROM on, I/O off - as we will copy to RAM under I/O ports
        mmap_set(0b00110011);

        char i = 0;
        do {
#assign y 0
#repeat
        fontCopyDst[y + i] = fontCopySrc[y + i];
#assign y y + 0x100
#until y == 0x100*1
            i++;
        } while (i != 0);

        fontCopyDst += 1*256;
        fontCopySrc += 1*256;
        if(fontCopyDst == GFX_1_FNT2+2048){
            fontCopyDone = true;
            fontCopyDst = GFX_1_FNT2;
        }
        // turn ROMS and I/O back on, so that we don't get a problem when bank tries to be switched but I/O is not visible
        mmap_set(MMAP_ROM);
    }
    // vic.color_back--;
    // vic.color_border--;
}

/* ================================================================================
Middle raster/split
================================================================================ */
__interrupt static void IRQ_middleNoScreen() {
    playMsx();

    // indicate frame position
    gms_framePos = FRAME_MIDDLE;
}

__interrupt static void IRQ_middleScreenMsx() {
    playMsx();

    // tick the game
    _timeControl();

    // indicate frame position
    gms_framePos = FRAME_MIDDLE;
}

__interrupt static void IRQ_middleTxtScreen_C() {
    // Select TEXT screen
    setSpritesBottomScr();
    IRQ_middleScreenMsx();
}

void IRQ_middleTxtScreen(){

    __asm {
        ldx #$08
    la: dex
        bne la
    }
    vic.ctrl1 = VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3;
    vic.ctrl2 = VIC_CTRL2_CSEL | 0;
    vic.memptr = d018_txt1;

    IRQ_middleTxtScreen_C();
    
}
/* ================================================================================
Control IRQ
================================================================================ */
__interrupt static void IRQ_bottomScrollAndUISprites_C() {
    // vic.color_border++;
    // vic.color_back++;
    
    // Soft scroll
    // vic.ctrl1 = VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3;
    vic.ctrl2 = _scrollIt;
    vic.memptr = d018_txt1;

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

    // UI sprite bank
    vic.memptr = d018_UI;
    cia2.pra = dd00_UI;

    // vic.color_border++;
    playMsx();
    if(!gms_enableMusic){
        // wait a few lines as msx is off, we don't want to desync the screen.
        while (vic.raster != 0x1){}
    }
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
    l9: ldx #$04
    la: dex
        bne la

        // call C routine
        jsr IRQ_bottomScrollAndUISprites_C
    }
}
//********************************************
__interrupt static void IRQ_bottomScrollAndMapSprites_C() {
    // vic.color_border++;
    // vic.color_back++;
    
    // Soft scroll
    // vic.ctrl1 = VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3;
    vic.ctrl2 = _scrollIt;
    vic.memptr = d018_txt1;

    showMapSpritesBottom();
    
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

    // UI sprite bank
    vic.memptr = d018_UI;
    cia2.pra = dd00_UI;

    // vic.color_border++;
    playMsx();
    if(!gms_enableMusic){
        // wait a few lines as msx is off, we don't want to desync the screen.
        while (vic.raster != 0x1){}
    }
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

void IRQ_bottomScrollAndMapSprites(){
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
    l9: ldx #$04
    la: dex
        bne la

        // call C routine
        jsr IRQ_bottomScrollAndMapSprites_C
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
    // Top - switch to txt
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
    // Top - switch to txt
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
    // Top - switch to txt
    rirq_build(&rirqc_topScreen, 1);
    rirq_call(&rirqc_topScreen, 0, IRQ_topHiresTxtScreen);
    rirq_set(1, IRQ_RASTER_TOP_MC_SCREEN, &rirqc_topScreen);

    // Middle - play msx
    rirq_build(&rirqc_middleScreen, 1);
    rirq_call(&rirqc_middleScreen, 0, IRQ_middleScreenMsx);
    rirq_set(2, IRQ_RASTER_MIDDLE_TXT_SCREEN, &rirqc_middleScreen);
    
    // Bottom - Open borders + Scroll + Sprites
    rirq_build(&rirqc_botomUISprites, 1);
    rirq_call(&rirqc_botomUISprites, 0, IRQ_bottomScrollAndMapSprites);
    // Place it into the last line of the screen
    rirq_set(3, IRQ_RASTER_BOTTOM_SCROLL_ETC, &rirqc_botomUISprites);

    // sort the raster IRQs
    rirq_sort();
}

// main init raster must be called first, this one just remaps some IRQs
void initRasterIRQ_Transition(){
    // Top - switch to blank
    rirq_build(&rirqc_topScreen, 1);
    rirq_call(&rirqc_topScreen, 0, IRQ_topNoScreen);
    rirq_set(1, IRQ_RASTER_TOP_MC_SCREEN, &rirqc_topScreen);

    // Middle - play msx
    rirq_build(&rirqc_middleScreen, 1);
    rirq_call(&rirqc_middleScreen, 0, IRQ_middleNoScreen);
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
    currentScreenMode = SCREEN_SPLIT_MC_TXT;
    
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

void switchScreenTo(byte screenMode){
    if(currentScreenMode != screenMode){
        currentScreenMode = screenMode;
        rirq_wait();

        switch (screenMode) {
            case SCREEN_SPLIT_MC_TXT:
                initRasterIRQ_SplitMCTxt();
                break;
            case SCREEN_FULL_TXT:
                initRasterIRQ_TxtMode();
                break;
            case SCREEN_MC_TXT:
                initRasterIRQ_MCTxtMode();
                break;
            case SCREEN_HIRES_TXT:
                initRasterIRQ_HiresTxtMode();
                break;
            case SCREEN_TRANSITION:
                initRasterIRQ_Transition();
                break;
        }
    }
}