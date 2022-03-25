// =============================================================================
.macro underlinedTextPart(text) {
    .fill text.size(), text.charAt(i)+$80
}
.macro underlinedText(text) {
    underlinedTextPart(text)
    .byte 0
}

.macro getSprite(spritePic, spriteNo) {
    .for (var y=0; y<21; y++)
        .for (var x=0; x<3; x++)
            .byte spritePic.getMulticolorByte(x + spriteNo * 3, y)
    .byte 2
}
.macro getHiresSprite(spritePic, spriteNo) {
    .for (var y=0; y<21; y++)
        .for (var x=0; x<3; x++)
            .byte spritePic.getSinglecolorByte(x + spriteNo * 3, y) 
    .byte 2
}
.const HALF_KOALA = "C64FILE, Bitmap=$0000, BE=$0f00, ScreenRam=$1f40, SE=$2120, ColorRam=$2328, CE=$2508, BackgroundColor = $2710"
.const FONT_64C = "C64FILE"
// =============================================================================

.segment CARTRIDGE_FILE [outBin="%O"]
    .segmentout [segments="bankStart_00_0"] // title screen bitmap
    .segmentout [segments="bankStart_00_1"] // ef startup code, loader, colors for title screen
    .segmentout [segments="bank_01_0_gfx"] // sprite bank
    .segmentout [segments="bank_01_1_msx"]
    .segmentout [segments="bank_02_code"]
    .segmentout [segments="bank_03_0_data"]

    .segmentout [segments="bank_03_1_textPL"]
    .segmentout [segments="bank_04_0_EMPTY"]
    .segmentout [segments="bank_04_1_textEN"]
    .segmentout [segments="bank_05_0_EMPTY"]
    .segmentout [segments="bank_05_1_EMPTY"]
    .segmentout [segments="bank_06_0_EMPTY"]
    .segmentout [segments="bank_06_1_EMPTY"]
    .segmentout [segments="bank_07_0_EMPTY"]
    .segmentout [segments="bank_07_1_EMPTY"]
    .segmentout [segments="bank_08_0_EMPTY"]
    .segmentout [segments="bank_08_1_EMPTY"]
    .segmentout [segments="bank_09_0_EMPTY"]
    .segmentout [segments="bank_09_1_EMPTY"]

    .segmentout [segments="bank_10_0_gfx"] // main menu picture 1
    .segmentout [segments="bank_10_1_code"] // main menu picture 1
    .segmentout [segments="bank_11_0_gfx"] // Farmland
    .segmentout [segments="bank_11_1_code"] // Farmland
    .segmentout [segments="bank_12_0_gfx"] // PigPen
    .segmentout [segments="bank_12_1_code"] // PigPen
    .segmentout [segments="bank_13_0_gfx"] // Smoke House
    .segmentout [segments="bank_13_1_code"] // Smoke House
    .segmentout [segments="bank_14_0_gfx"] // Shop outside
    .segmentout [segments="bank_14_1_code"] // Shop
    .segmentout [segments="bank_15_0_gfx"] // Shop inside
    .segmentout [segments="bank_15_1_code"] // Employment

    // night gfx
    .segmentout [segments="bank_16_0_gfx"] // main menu picture night
    .segmentout [segments="bank_16_1_EMPTY"]
    .segmentout [segments="bank_17_0_gfx"] // farmland picture night
    .segmentout [segments="bank_17_1_EMPTY"]


.segmentdef bankStart_00_0 [start=$8000, min=$8000, max=$9fff, fill]
.segmentdef bankStart_00_1 [segments="efStartup, loaderC000, loaderData, titlePicColors, startupVectors"]
    .segmentdef efStartup [start=$a000, min=$a000, max=$a0ff, fill]
    .segmentdef loaderC000 [start=$a100]
    .segmentdef loaderData [start=$a400]
    .segmentdef titlePicColors [start=$b000, min=$b000, max=$bff9, fill]
    .segmentdef startupVectors [startAfter="titlePicColors"]
.segmentdef loader [start=$0400]
.segmentdef loaderDataSeg [startAfter="loader"]

.segmentdef bank_01_0_gfx [segments="spritesLow, fonts, spritesHi", start=$c000, min=$c000, max=$dfff, fill]
    .segmentdef spritesLow [start=$c400, min=$c400, max=$cfff, fill]
    .segmentdef fonts [start=$d000]
    .segmentdef spritesHi [start=$d800, min=$d800, max=$dfff, fill]

.segmentdef bank_01_1_msx [min=$6000, max=$7fff, fill]

.segmentdef bank_02_code [segments="Code", min=$1000, max=$4fff, fill]
    .segmentdef Code [start=$1000]
.segmentdef bank_03_0_data [segments="Data", min=$5000, max=$6fff, fill]
    .segmentdef Data [start=$5000]


.segmentdef bank_03_1_textPL [segments="textArrayPL, textPL", min=$6000, max=$7fff, fill]
    .segmentdef textArrayPL [start=$6000]
    .segmentdef textPL [startAfter="textArrayPL"]

.segmentdef bank_04_0_EMPTY [min=$8000, max=$9fff, fill, fillByte=$FF]

.segmentdef bank_04_1_textEN [segments="textArrayEN, textEN", min=$6000, max=$7fff, fill]
    .segmentdef textArrayEN [start=$6000]
    .segmentdef textEN [startAfter="textArrayEN"]

.segmentdef bank_05_0_EMPTY [min=$8000, max=$9fff, fill, fillByte=$FF]
.segmentdef bank_05_1_EMPTY [min=$8000, max=$9fff, fill, fillByte=$FF]
.segmentdef bank_06_0_EMPTY [min=$8000, max=$9fff, fill, fillByte=$FF]
.segmentdef bank_06_1_EMPTY [min=$8000, max=$9fff, fill, fillByte=$FF]
.segmentdef bank_07_0_EMPTY [min=$8000, max=$9fff, fill, fillByte=$FF]
.segmentdef bank_07_1_EMPTY [min=$8000, max=$9fff, fill, fillByte=$FF]
.segmentdef bank_08_0_EMPTY [min=$8000, max=$9fff, fill, fillByte=$FF]
.segmentdef bank_08_1_EMPTY [min=$8000, max=$9fff, fill, fillByte=$FF]
.segmentdef bank_09_0_EMPTY [min=$8000, max=$9fff, fill, fillByte=$FF]
.segmentdef bank_09_1_EMPTY [min=$8000, max=$9fff, fill, fillByte=$FF]


.segmentdef bank_10_0_gfx [min=$8000, max=$9fff, fill]
.segmentdef bank_10_1_code [min=$2000, max=$3fff, fill]
    .segmentdef mainMenuCode [start=$7000]
    .segmentdef mainMenuData [startAfter="mainMenuCode"]

.segmentdef bank_11_0_gfx [min=$8000, max=$9fff, fill]
.segmentdef bank_11_1_code [min=$2000, max=$3fff, fill]
    .segmentdef farmlandCode [start=$7000]
    .segmentdef farmlandData [startAfter="farmlandCode"]

.segmentdef bank_12_0_gfx [min=$8000, max=$9fff, fill]
.segmentdef bank_12_1_code [min=$2000, max=$3fff, fill]
    .segmentdef pigPenCode [start=$7000]
    .segmentdef pigPenData [startAfter="pigPenCode"]

.segmentdef bank_13_0_gfx [min=$8000, max=$9fff, fill]
.segmentdef bank_13_1_code [min=$2000, max=$3fff, fill]
    .segmentdef smokeHouseCode [start=$7000]
    .segmentdef smokeHouseData [startAfter="smokeHouseCode"]

.segmentdef bank_14_0_gfx [min=$8000, max=$9fff, fill]
.segmentdef bank_14_1_code [min=$2000, max=$3fff, fill]
    .segmentdef shopCode [start=$7000]
    .segmentdef shopData [startAfter="shopCode"]

.segmentdef bank_15_0_gfx [min=$8000, max=$9fff, fill]
.segmentdef bank_15_1_code [min=$2000, max=$3fff, fill]
    .segmentdef employmentCode [start=$7000]
    .segmentdef employmentData [startAfter="employmentCode"]

// night gfx
.segmentdef bank_16_0_gfx [min=$8000, max=$9fff, fill]
.segmentdef bank_16_1_EMPTY [min=$8000, max=$9fff, fill, fillByte=$FF]
.segmentdef bank_17_0_gfx [min=$8000, max=$9fff, fill]
.segmentdef bank_17_1_EMPTY [min=$8000, max=$9fff, fill, fillByte=$FF]

// =============================================================================
// Static Segment definitions below
// =============================================================================

// =============================================================================
// Startup routine for Easyflash - $8000
// Title picture
.segment bankStart_00_0

// =============================================================================
    .var titlePic1a = LoadBinary("assets/multicolorGfx/kielbasaEmpire.kla", BF_KOALA)
    *=$8000; .fill titlePic1a.getBitmapSize(), titlePic1a.getBitmap(i)

// =============================================================================
// Startup routine for Easyflash - $e000, moved to $a000 after a while
// Bootstrap code and title picture color data
.segment bankStart_00_1
// =============================================================================
.segment efStartup

    ; // .label EASYFLASH_BANK    = $DE00
    .label EASYFLASH_CONTROL = $DE02
    .const EASYFLASH_LED     = $80
    .const EASYFLASH_16K     = $07
    .const EASYFLASH_ULTIMAX = $05
    .const EASYFLASH_KILL    = $04

    // This code runs in Ultimax mode after reset, so this memory becomes
    // visible at $E000..$FFFF first and must contain a reset vector
    .pseudopc $e000 {
coldStart:
        // === the reset vector points here ===
        sei
        ldx #$ff
        txs
        cld

        // enable VIC (e.g. RAM refresh)
        lda #8
        sta $d016

        // write to RAM to make sure it starts up correctly (=> RAM datasheets)
startWait:
        sta $0100, x
        dex
        bne startWait
        
        // copy the final start-up code to RAM (bottom of CPU stack)
        ldx #00
l1:
        lda startUpCode, x
        sta $0100, x
        dex
        bne l1
        jmp $0100
startUpCode:
    }
    .pseudopc $0100 {
        // === this code is copied to the stack area, does some inits ===
        // === scans the keyboard and kills the cartridge or          ===
        // === starts the main application                            ===

        // get Cart ROM to $8000 and $A000 - show KERNAL rom
        lda #EASYFLASH_16K + EASYFLASH_LED
        sta EASYFLASH_CONTROL

        // Check if one of the magic kill keys is pressed
        // This should be done in the same way on any EasyFlash cartridge!

        // Prepare the CIA to scan the keyboard
        lda #$7f
        sta $dc00   // pull down row 7 (DPA)

        ldx #$ff
        stx $dc02   // DDRA $ff = output (X is still $ff from copy loop)
        inx
        stx $dc03   // DDRB $00 = input

        // Read the keys pressed on this row
        lda $dc01   // read coloumns (DPB)

        // Restore CIA registers to the state after (hard) reset
        stx $dc02   // DDRA input again
        stx $dc00   // Now row pulled down

        // Check if one of the magic kill keys was pressed
        and #$e0    // only leave "Run/Stop", "Q" and "C="
        cmp #$e0
        bne kill    // branch if one of these keys is pressed

        // same init stuff the kernel calls after reset
        ldx #0
        stx $d016
        jsr $ff84   // Initialise I/O

        // These may not be needed - depending on what you''ll do
        jsr $ff87   // Initialise System Constants
        jsr $ff8a   // Restore Kernal Vectors
        jsr $ff81   // Initialize screen editor

        // screen off
        lda #%00001011
        sta $d011

        // call loader routine
        ldx #0
    loop:
        .for (var i=0; i<$3; i++) {
            lda $a100+i*$100,x
            sta $c000+i*$100,x
        }
        inx
        beq done
        jmp loop
    done:
        jmp $c000

kill:
        lda #EASYFLASH_KILL
        sta EASYFLASH_CONTROL
        jmp ($fffc) // reset
    }
startUpEnd:

.segment loaderC000
.pseudopc $c000 {
        // move real loader to 0400
        ldx #0
    !loop:
        .for (var i=0; i<$c; i++) {
            lda $a400+i*$100,x
            sta $0400+i*$100,x
        }
        inx
        beq !done+
        jmp !loop-
    !done:

        ldx #0
    !loop:
    
        // move code in, so we can call __start
        lda #2
        sta EASYFLASH_BANK
        .for (var i=0; i<$40; i++) {
            lda $8000+i*$100,x
            sta $1000+i*$100,x
        }
        // data
        lda #3
        sta EASYFLASH_BANK

        .for (var i=0; i<$20; i++) {
            lda $8000+i*$100,x
            sta $5000+i*$100,x
        }
        
        inx
        beq !done+
        jmp !loop-
    !done:
        jmp $1000
}

// got some space in this bank, lets use it for loader code
.segment titlePicColors
    .var titlePic1b = LoadBinary("assets/multicolorGfx/kielbasaEmpire.kla", BF_KOALA)
    *=$b000; .fill titlePic1b.getScreenRamSize(), titlePic1b.getScreenRam(i)
    *=$b400; .fill titlePic1b.getColorRamSize(), titlePic1b.getColorRam(i)

.segment startupVectors
    *=$bffa
.pseudopc $fffa {
        .word reti        // NMI
        .word coldStart   // RESET

        // we don''t need the IRQ vector and can put RTI here to save space :)
    reti:
        rti
        .byte $ff
}
