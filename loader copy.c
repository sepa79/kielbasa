#pragma extension("bin")
#pragma link("kielbasa.ld")
#pragma struct_model(classic)
#pragma zp_reserve(0x73..0x8a)  // CHRGET routine: fetches next character of BASIC program text
#pragma zp_reserve(0x91)        // Keyboard Flag. 127 = STOP , 223 = C= , 239 = SPACE , 251 = CTRL , 255 = no key pressed
#pragma zp_reserve(0x99)        // Current input device: defaults to 0 = keyboard
#pragma zp_reserve(0xa0..0xa2)  // Software jiffy clock, updated by KERNAL IRQ every 1/60 second
#pragma zp_reserve(0xc0)        // Cassette motor flag 0 = off, 1 = on
#pragma zp_reserve(0xc5)        // Matrix coordinate of last pressed key, 64 = none
#pragma zp_reserve(0xc6)        // Number of characters in keyboard buffer
#pragma zp_reserve(0xcb)        // Index to keyboard decoding table for currently pressed key, 64 = no key was depressed
#pragma zp_reserve(0xcc)        // Flash cursor flag 0 = yes, otherwise no
#pragma zp_reserve(0xd0)        // Input from 0 = keyboard or 3 = screen
#pragma zp_reserve(0xf5..0xf6)  // Keyboard decoding table

#pragma var_model(global_ma)
// #pragma var_model(mem)

#include <c64.h>
#include "assets/music/music.h"
#include "assets/gfxBank01_0.h"
#include "assets/gfxBanks.h"
#include "translation/common.h"
#include "translation/textsPL.h"
#include "translation/textsEN.h"

char * const EASYFLASH_BANK    = (char *)$DE00;
char * const EASYFLASH_CONTROL = (char *)$DE02;

// title screen source location - bank 00_1
char * const TITLE_GFX_SCR_S    = (char *)$b000;
char * const TITLE_GFX_COLOR_S  = (char *)$b400;

// used throughout the game - 2 screens in banks 1 and 2
__export char * const GFX_1_BMP = (char *)$e000;
__export char * const GFX_1_FNT = (char *)$d000;
__export char * const GFX_1_SCR = (char *)$c000;

__export char * const GFX_2_BMP = (char *)$a000;
__export char * const GFX_2_SCR = (char *)$8000;
__export char * const GFX_HK_COL_LOW = (char *)$8f00;
// __export char * const GFX_HK_COL_HI = (char *)$af00;

#include "kielbasa.h"

#pragma data_seg(loaderData) // $a100
__export char loaderCodeData[] = kickasm {{
    .segmentout [segments="loader"]
    .segmentout [segments="loaderDataSeg"]
}};

#pragma code_seg(loader) // $0800
#pragma data_seg(loaderDataSeg)


// load menu code
__export void loadMenuCode(char bank){
    *EASYFLASH_BANK = bank;
    *PROCPORT = %00110111;
    kickasm(clobbers "AX") {{
        ldx #0
    !loop:
        .for (var i=0; i<$20; i++) {
            lda $a000+i*$100,x
            sta $7000+i*$100,x
        }
        inx
        beq !done+
        jmp !loop-
    !done:
    }};
    // *PROCPORT = %00110111;
}

// load translation
__export void loadTranslation(char bank){
    *EASYFLASH_BANK = bank;
    *PROCPORT = %00110111;
    kickasm(clobbers "AX") {{
        ldx #0
    !loop:
        .for (var i=0; i<$10; i++) {
            lda $a000+i*$100,x
            sta $6000+i*$100,x
        }
        inx
        beq !done+
        jmp !loop-
    !done:
    }};
    // *PROCPORT = %00110111;
}

// Loading fonts and sprites
__export void _loadFontsAndSprites8000ToGfx1(char bank){
    *EASYFLASH_BANK = bank;
    *PROCPORT = %00110011;
    kickasm(clobbers "AX", uses GFX_1_FNT, uses GFX_1_SCR) {{
        ldx #0
    !loop:
        .for (var i=0; i<$c; i++) {
            lda $8400+i*$100,x
            sta GFX_1_SCR+$400+i*$100,x
        }
        .for (var i=0; i<8; i++) {
            lda $9000+i*$100,x
            sta GFX_1_FNT+i*$100,x
        }
        inx
        beq done
        jmp !loop-
    done:
    }};
    *PROCPORT = %00110111;
}

// Bitmap from CartBank 0 (8000) to Vic Bank 2, A000
__export void _loadGfx8000ToGfx2(char bank){
    *EASYFLASH_BANK = bank;

    kickasm(clobbers "AX", uses GFX_2_BMP) {{
        ldx #0
    !loop:
        .for (var i=0; i<$20; i++) {
            lda $8000+i*$100,x
            sta GFX_2_BMP+i*$100,x
        }
        inx
        beq done
        jmp !loop-
    done:
    }};
}

// .var mmPicA = LoadBinary("assets/multicolorGfx/last.kla", HALF_KOALA)
// *=$8000; .fill mmPicA.getBitmapSize(), mmPicA.getBitmap(i)
// *=$9040; .fill mmPicA.getColorRamSize(), mmPicA.getColorRam(i)
// *=$9400; .fill mmPicA.getScreenRamSize(), mmPicA.getScreenRam(i)

// Half Koala from CartBank 0 (8000) to Vic Bank 3, c000
__export void loadHalfKoalaAndSpritesToGfx1(char bank){
    *EASYFLASH_BANK = bank;

    kickasm(clobbers "AXY", uses GFX_1_BMP, uses GFX_1_SCR, uses GFX_HK_COL_LOW) {{

        lda #$37
        sta $01

        lda #$c0 // should be somewhere in the middle of the bottom screen
        cmp RASTER
        bne *-3

        // make it black
        ldx #00
        ldy #$4
    !loop:
        lda #$00
        .for (var i=0; i<12; i++) {
            sta GFX_1_SCR+i*40,x
            sta COLORRAM+i*40,x
        }

        stx $ff
        sec
        lda #19
        sbc $ff
        tax
        lda #$00
        .for (var i=0; i<12; i++) {
            sta GFX_1_SCR+20+i*40,x
            sta COLORRAM+20+i*40,x
        }
        ldx $ff

        dey
        bne !xLoop+;

        lda #$c0 // should be somewhere in the middle of the bottom screen
        cmp RASTER
        bne *-3
        ldy #$4

    !xLoop:
        
        inx
        cpx #20
        beq !done+
        jmp !loop-
    !done:

        // load picture
        ldx #0
    !loop:

        .for (var i=0; i<$10; i++) {
            lda $8000+i*$100,x
            sta GFX_1_BMP+i*$100,x
        }
        //load sprites
        .for (var i=0; i<$6; i++) {
            lda $92c0+i*$100,x
            sta GFX_1_BMP + $0f00+i*$100,x
        }
        
        inx
        beq !done+
        jmp !loop-
    !done:

        lda #$37
        sta $01

        lda #$c0 // should be somewhere in the middle of the bottom screen
        cmp RASTER
        bne *-3

        // restore colors
        ldx #19
        ldy #$4
    !loop:
        .for (var i=0; i<12; i++) {
            lda $90e0    +0+i*40,x
            sta GFX_1_SCR+0+i*40,x

            lda GFX_HK_COL_LOW +0+i*40,x
            sta COLORRAM       +0+i*40,x
        }

        stx $ff
        sec
        lda #19
        sbc $ff
        tax
        .for (var i=0; i<12; i++) {
            lda $90e0    +20+i*40,x
            sta GFX_1_SCR+20+i*40,x

            lda GFX_HK_COL_LOW +20+i*40,x
            sta COLORRAM       +20+i*40,x
        }
        ldx $ff

        dey
        bne !xLoop+;

        lda #$c0 // should be somewhere in the middle of the bottom screen
        cmp RASTER
        bne *-3
        ldy #$4

    !xLoop:
        dex
        cpx #$ff
        beq !done+
        jmp !loop-
    !done:
    
    }};

}

void _loadTitleScreen(){
    VICII->BORDER_COLOR = BLACK;
    VICII->BG_COLOR = BLACK;
    *EASYFLASH_BANK = 0;

    kickasm(clobbers "AX", uses COLORRAM, uses TITLE_GFX_COLOR_S, uses TITLE_GFX_SCR_S, uses GFX_2_SCR) {{
        ldx #0
    !loop:
        // colors
        .for (var i=0; i<4; i++) {
            lda TITLE_GFX_SCR_S+i*$100,x
            sta GFX_2_SCR+i*$100,x
            lda TITLE_GFX_COLOR_S+i*$100,x
            sta COLORRAM+i*$100,x
        }
        inx
        bne !loop-
    done:
    }};

    _loadGfx8000ToGfx2(0);

    *D016 = VICII_MCM | VICII_CSEL | 0;
    vicSelectGfxBank(GFX_2_SCR);
    *D018 = toD018(GFX_2_SCR, GFX_2_BMP);

    char color[8] = {$0, $e,$e,$3, $f,$c,$b, $0};
    char i = 0;
    do {
        // wait for raster
        do {} while (VICII->RASTER != $25);
        VICII->BORDER_COLOR = color[i];
        VICII->BG_COLOR = color[i];
        for(int x: 0..2000){}
        i++;
    } while (i<9);

    *D011 = VICII_BMM | VICII_DEN | VICII_RSEL | 3;
}

// Raster IRQ Routine playing music
__interrupt(rom_sys_c64) void irq_play(){
    byte prevRomCfg = (*(unsigned char*) (PROCPORT));
    *PROCPORT = %00110111;
    
    VICII->BORDER_COLOR++;
    // Play SID
    // (*musicPlay)();
    // Acknowledge the IRQ
    VICII->IRQ_STATUS = IRQ_RASTER;
    VICII->BORDER_COLOR--;
    *PROCPORT = prevRomCfg;
}

void _loadMainMusicTheme(){
    // switch bank to 1
    *EASYFLASH_BANK = 1;

    kickasm(clobbers "AX", uses MUSIC_S, uses MUSIC_D) {{
        ldx #0
    !loop:
        .for (var i=0; i<$10; i++) {
            lda MUSIC_S+i*$100,x
            sta MUSIC_D+i*$100,x
        }
        inx
        beq done
        jmp !loop-
    done:
    }};

    asm { sei }
    // (*musicInit)();
    // Disable CIA 1 Timer IRQ
    CIA1->INTERRUPT = CIA_INTERRUPT_CLEAR;
    // Set raster line to $fd
    VICII->CONTROL1 &=$7f;
    VICII->RASTER = $fd;
    // Enable Raster Interrupt
    VICII->IRQ_ENABLE = IRQ_RASTER;
    // Set the IRQ routine
    *KERNEL_IRQ = &irq_play;
    asm { cli }
}

void main(){
    *PROCPORT_DDR = PROCPORT_DDR_MEMORY_MASK;
    // 1. Show title screen, then copy stuff around
    _loadTitleScreen();

    // 2. Play some music
    _loadMainMusicTheme();

    // 3. Copy Fonts/Sprite Gfx to Vic Bank 1
    _loadFontsAndSprites8000ToGfx1(1);
    
    // load text
    loadTranslation(TRANSLATION_PL_BANK);
    // wait for keypress
    while(!getIn()); // without music it skips the first one
    while(!getIn());
    mainLoop();
}

#pragma code_seg(Code)
#pragma data_seg(Data)
