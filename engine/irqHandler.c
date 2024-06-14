#include <c64/vic.h>
#include <c64/cia.h>
#include <c64/types.h>
#include <c64/rasterirq.h>
#include <c64/memmap.h>

#include <engine/joystick.h>
#include <engine/irqHandler.h>
#include <assets/assetsSettings.h>
#include <engine/gameSettings.h>
#include <assets/music.h>
#include <translation/common.h>

// screen will be split at these lines
#define IRQ_RASTER_TOP_MC_SCREEN 0x2e
#define IRQ_RASTER_MIDDLE_TXT_SCREEN 0x91
// special irqs to control sprites
#define IRQ_RASTER_BOTTOM_UI 0xf9
#define IRQ_RASTER_TOP_UI_SPRITES 0x01

volatile char mcScrBackground = VCOL_BLACK;
volatile bool map_2ndScreen = true;

// used to check if move to given tile is possible
// irqs update it to point to current screen (as we do double buffer)
char * mapScreen;

// init joysticks to neutral pos
static byte _joy1Status = 255;
static byte _joy2Status = 127;

byte currentScreenMode = 0;

// cache for msx sync marker
volatile byte _msxMarker = 0;

void clearMsxSync(){
    _msxMarker = 0;
}
void waitForMsxSync(){
    do {} while(_msxMarker == 0);
    _msxMarker = 0;
}

/* ================================================================================
Play msx, if enabled
================================================================================ */
static void playMsx(){
    if(gms_enableMusic){
        if(gms_musicPlayNow){

            byte _prevRomCfgPC = ((byte *)0x01)[0];
            __asm {
                lda #MSX_ROM
                sta $01
                jsr MSX_PLAY
                lda $803f
                cmp #$10
                bne ex
                sta _msxMarker // copy to marker
                lda #$00       // wipe marker in player
                sta $803f
            ex:
            };
            ((byte *)0x01)[0] = _prevRomCfgPC;
        }
        // if it's 1x speed, skip next frame
        if(gms_musicPlayCount == SPEED_1X) {
            // vic.color_border--;
            gms_musicPlayNow = !gms_musicPlayNow;
        }
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

    // set map sprites
    setSpritesTopScr();
    
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

    while (vic.raster != 50){}
    
    // show any sprites the menu might have
    setSpritesTopScr();

    // set desired background color
    vic.color_back = mcScrBackground;

    // tick the game
    _timeControl();

    // indicate frame position
    gms_framePos = FRAME_TOP;

    // vic.color_back--;
    // vic.color_border--;
}

__interrupt static void IRQ_topMCTxtBoomBoxScreen() {
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
    // vic.color_back--;
    // vic.color_border--;
}

/* ================================================================================
Middle raster/split
================================================================================ */
__interrupt static void IRQ_middleNoScreen() {
    // vic.color_border++;
    playMsx();

    // indicate frame position
    gms_framePos = FRAME_MIDDLE;
    // vic.color_border--;
}

__interrupt static void IRQ_middleScreenMsx() {
    playMsx();

    // tick the game
    _timeControl();

    // indicate frame position
    gms_framePos = FRAME_MIDDLE;
}

__interrupt void IRQ_middleTxtScreen(){

    __asm {
        ldx #$07
    la: dex
        bne la
        nop
        nop
        nop
    }
    vic.ctrl1 = VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3;
    vic.ctrl2 = VIC_CTRL2_CSEL | 0;
    vic.memptr = d018_txt1;

    // Select TEXT screen
    setSpritesBottomScr();
    IRQ_middleScreenMsx();
}


char register1_value = 0;
char register2_value = 0;
char cassette_anim_delay = 0;

// used in Music Menu, displays Boombox anims
__interrupt void IRQ_middleMCTxtBoomBoxScreen(){
    // run after playMsx so that we have player visible in RAM
    IRQ_middleScreenMsx();

    #define BAR_L3 0xea
    #define BAR_L2 BAR_L3+5
    #define BAR_L1 BAR_L3+10
    #define BAR_FULL BAR_L3+13

    #define eq_linia 0xc000+12+40*14
    __asm {
equalizer:

        lda $d41c       // music register 1
        cmp register1_value
        beq no_change1
        sta register1_value
        lsr
        lsr
        lsr
        lsr
        tay
        // cpy #16
        // bcs no_change1

        jsr draw_equ

no_change1:

        lda $83c7  //   music register 2
        cmp register2_value
        beq no_change2
        sta register2_value
        lsr 
        lsr 
        lsr 
        lsr
        tay
//         cpy #16
//         bcs no_change2

        jsr draw_equ

no_change2:

        ldx #16
nxt:
        dex
        bmi exitEq
        lda eq_linia,x 
        cmp #BAR_L1
        bcc nxt_line1
        dec eq_linia,x 
        bne nxt


nxt_line1:

        lda eq_linia+40,x 
        cmp #BAR_L2
        bcc nxt_line2
        dec eq_linia+40,x 
        bne nxt
        jmp exitEq

nxt_line2:

        lda eq_linia+80,x 
        cmp #BAR_L3+1
        bcc nxt
        dec eq_linia+80,x 
        bne nxt
        jmp exitEq

draw_equ:

        lda #BAR_FULL
        sta eq_linia,y 
        sec 
        sbc #5
        sta eq_linia+40,y 
        sbc #5
        sta eq_linia+80,y 
        rts 

exitEq:

    }

    #define TAPE_FNT_S 0xf8
    #define TAPE_FNT_E 0xfc
    #define tape1 0xc000+16+40*19
    #define tape2 0xc000+23+40*19
    // tape anim
    if(gms_enableMusic){
        __asm{
            inc cassette_anim_delay
            lda cassette_anim_delay
            cmp #6
            bne cass_q
            lda #0
            sta cassette_anim_delay

            inc tape1
            inc tape2
            inc tape1+40
            inc tape2+40

            lda tape1
            cmp #TAPE_FNT_E
            bne cass_q
            lda #TAPE_FNT_S
            sta tape1
            sta tape2
            lda #TAPE_FNT_E
            sta tape1+40
            sta tape2+40
    cass_q:
        }
    }

    // speaker anim
    char ul;
    char ur;
    char ml;
    char mr;
    char ll;
    char lr;
    
    // get sound form buffer
    char sound = *(char *)0xd41c;
    sound = (sound >> 2) & 3;
    if(sound == 2){
        // big
        ul = 0x85;
        ur = 0x86;
        ml = 0x87;
        mr = 0x88;
        ll = 0x89;
        lr = 0x8a;
    } else {
        // small
        ul = 0x85+6;
        ur = 0x86+6;
        ml = 0x87+6;
        mr = 0x88+6;
        ll = 0x89+6;
        lr = 0x8a+6;
    }
    #define spk1 ((char *)0xc000+5+40*17)
    #define spk2 ((char *)0xc000+33+40*17)
    spk1[0] = ul;
    spk1[1] = ur;
    spk1[0+40] = ml;
    spk1[1+40] = mr;
    spk1[0+80] = ll;
    spk1[1+80] = lr;
    spk2[0] = ul;
    spk2[1] = ur;
    spk2[0+40] = ml;
    spk2[1+40] = mr;
    spk2[0+80] = ll;
    spk2[1+80] = lr;

}

/* ================================================================================
Bottom UI
================================================================================ */
static void IRQ_bottomMsxEtc() {
    // vic.color_border++;
    // vic.color_back++;

    // UI sprite bank
    vic.memptr = d018_UI;
    cia2.pra = dd00_UI;

    // vic.color_border++;
    playMsx();
    if(!gms_enableMusic){
        // wait a few lines as msx is off, we don't want to desync the screen.
        while (vic.raster != 0x10){}
    }

    _timeControl();
    // vic.color_border--;

    joyUpdate();

    // vic.color_border--;
    // vic.color_back--;
}
//********************************************
__interrupt static void IRQ_bottomUI() {
    // wait for right line
    // while (vic.raster != 0xfa){}
    // Set screen height to 24 lines - this is done after the border should have started drawing - so it wont start
    //reset scrreen color
    vic.color_back = VCOL_BLACK;
    vic.ctrl1 &= (0xff^VIC_CTRL1_RSEL);
    while (vic.raster != 0xfc){}
    // reset to normal screen height
    vic.ctrl1 = VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3;
    showUiSpritesBottom();
    IRQ_bottomMsxEtc();
}

__interrupt static void IRQ_bottomMapUI() {
    // wait for right line
    // while (vic.raster != 0xfa){}
    // Set screen height to 24 lines - this is done after the border should have started drawing - so it wont start
    vic.ctrl1 &= (0xff^VIC_CTRL1_RSEL);
    while (vic.raster != 0xfc){}
    // reset to normal screen height
    vic.ctrl1 = VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3;
    showMapSpritesBottom();
    IRQ_bottomMsxEtc();
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
RIRQCode rirqc_topUISprites, rirqc_bottomUI, rirqc_topScreen, rirqc_middleScreen, openBorder;

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
void initRasterIRQ_MCTxtBoomBoxMode(){
    // Top - switch to txt
    rirq_build(&rirqc_topScreen, 1);
    rirq_call(&rirqc_topScreen, 0, IRQ_topMCTxtBoomBoxScreen);
    rirq_set(1, IRQ_RASTER_TOP_MC_SCREEN, &rirqc_topScreen);

    // Middle - play msx
    rirq_build(&rirqc_middleScreen, 1);
    rirq_call(&rirqc_middleScreen, 0, IRQ_middleMCTxtBoomBoxScreen);
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
    
    // Bottom Sprites

    rirq_build(&rirqc_bottomUI, 1);
    rirq_call(&rirqc_bottomUI, 0, IRQ_bottomMapUI);
    // Place it into the last line of the screen
    rirq_set(3, IRQ_RASTER_BOTTOM_UI, &rirqc_bottomUI);

    // sort the raster IRQs
    rirq_sort();
}

// main init raster must be called first, this one just remaps some IRQs
void initRasterIRQ_MCGfxMode(){
    // Top - switch to txt
    rirq_build(&rirqc_topScreen, 1);
    rirq_call(&rirqc_topScreen, 0, IRQ_topMCGfxScreen);
    rirq_set(1, IRQ_RASTER_TOP_MC_SCREEN, &rirqc_topScreen);

    // Middle - play msx
    rirq_build(&rirqc_middleScreen, 1);
    rirq_call(&rirqc_middleScreen, 0, IRQ_middleScreenMsx);
    rirq_set(2, IRQ_RASTER_MIDDLE_TXT_SCREEN, &rirqc_middleScreen);
    
    // Bottom Sprites

    rirq_build(&rirqc_bottomUI, 1);
    rirq_call(&rirqc_bottomUI, 0, IRQ_bottomMapUI);
    // Place it into the last line of the screen
    rirq_set(3, IRQ_RASTER_BOTTOM_UI, &rirqc_bottomUI);

    // sort the raster IRQs
    rirq_sort();
}

// main init raster must be called first, this one just remaps some IRQs
void initRasterIRQ_Transition(){
    // Top UI
    rirq_build(&rirqc_topUISprites, 1);
    rirq_call(&rirqc_topUISprites, 0, IRQ_topUISprites);
    rirq_set(0, IRQ_RASTER_TOP_UI_SPRITES, &rirqc_topUISprites);

    // Top - switch to blank
    rirq_build(&rirqc_topScreen, 1);
    rirq_call(&rirqc_topScreen, 0, IRQ_topNoScreen);
    rirq_set(1, IRQ_RASTER_TOP_MC_SCREEN, &rirqc_topScreen);

    // Middle - play msx
    rirq_build(&rirqc_middleScreen, 1);
    rirq_call(&rirqc_middleScreen, 0, IRQ_middleNoScreen);
    rirq_set(2, IRQ_RASTER_MIDDLE_TXT_SCREEN, &rirqc_middleScreen);

    // Bottom - Open borders + Sprites
    rirq_build(&rirqc_bottomUI, 1);
    rirq_call(&rirqc_bottomUI, 0, IRQ_bottomUI);
    // Place it into the last line of the screen
    rirq_set(3, IRQ_RASTER_BOTTOM_UI, &rirqc_bottomUI);

    vic.color_border--;
    for( char i=4; i<16;i++){
        rirq_clear(i);
    }
    vic.color_border++;

    // sort the raster IRQs
    rirq_sort();
}

// main IRQ routine
void initRasterIRQ_SplitMCTxt(){
    // Top - switch to MC GFX
    rirq_build(&rirqc_topScreen, 1);
    rirq_call(&rirqc_topScreen, 0, IRQ_topMCGfxScreen);
    rirq_set(1, IRQ_RASTER_TOP_MC_SCREEN, &rirqc_topScreen);

    // Middle - switch to TXT, play msx
    rirq_build(&rirqc_middleScreen, 1);
    rirq_call(&rirqc_middleScreen, 0, IRQ_middleTxtScreen);
    rirq_set(2, IRQ_RASTER_MIDDLE_TXT_SCREEN, &rirqc_middleScreen);

    // Bottom - Open borders + Sprites
    rirq_build(&rirqc_bottomUI, 1);
    rirq_call(&rirqc_bottomUI, 0, IRQ_bottomUI);
    // Place it into the last line of the screen
    rirq_set(3, IRQ_RASTER_BOTTOM_UI, &rirqc_bottomUI);

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
    vic.spr_enable = 0b00000000;

    // if you use the mmap_trampoline() you have to call the mmap_set() at least once to init the shadow variable
    // set ALL_ROM as default
    setPort(MMAP_ROM);
    // Activate trampoline
    mmap_trampoline();
    // Disable CIA interrupts, we do not want interference
    // with our joystick interrupt
    cia_init();
    // clean 0xffff - so we don't have artefacts when we open borders
    // NOPE - messes up trampoline
    // ((char *)0xffff)[0] = 0;

    // initialize raster IRQ
    rirq_init(true);

    // load standard IRQ routine
    initRasterIRQ_Transition();
    initRasterIRQ_SplitMCTxt();
    currentScreenMode = SCREEN_SPLIT_MC_TXT;
    
    // start raster IRQ processing
    rirq_start();
}

void switchScreenTo(byte screenMode){
    if(currentScreenMode != screenMode){
        currentScreenMode = screenMode;
        // used to be 0, but without music it tends to hang menus - waits forever
        vic_waitLine(10);
        // rirq_stop();
        // vic.color_border++;

        switch (screenMode) {
            case SCREEN_SPLIT_MC_TXT:
                initRasterIRQ_SplitMCTxt();
                break;
            case SCREEN_FULL_TXT:
                initRasterIRQ_TxtMode();
                break;
            case SCREEN_FULL_MC_TXT:
                initRasterIRQ_MCTxtMode();
                break;
            case SCREEN_MC_TXT_BOOMBOX:
                initRasterIRQ_MCTxtBoomBoxMode();
                break;
            case SCREEN_HIRES_TXT:
                initRasterIRQ_HiresTxtMode();
                break;
            case SCREEN_MC_GFX:
                initRasterIRQ_MCGfxMode();
                break;
            case SCREEN_TRANSITION:
                initRasterIRQ_Transition();
                break;
            // in menu/fishing.c
            case SCREEN_FISHING:
                vic_waitLine(80);
                initRasterIRQ_Fishing();
                break;
        }

        // rirq_start();
        // vic.color_border--;
    }
}