.segment CARTRIDGE_FILE [outBin="%O"]
    .segmentout [segments="bankStart_00_0"]
    .segmentout [segments="bankStart_00_1"]
    .segmentout [segments="bankStart_01_0"]
    .segmentout [segments="bankStart_01_1"]
    .segmentout [segments="bankStart_02_0"]
    .segmentout [segments="bankStart_02_1"]

.segmentdef bankStart_00_0 [start=$8000, min=$8000, max=$9fff, fill]
.segmentdef bankStart_00_1 [start=$e000, min=$e000, max=$ffff, fill]

.segmentdef bankStart_01_0 [segments="Startup, Code, Data", min=$8000, max=$9fff, fill]
.segmentdef Startup [start=$8000]
.segmentdef Code [startAfter="Startup"]
.segmentdef Data [startAfter="Code"]
.segment Startup

.segmentdef bankStart_01_1 [min=$a000, max=$bfff, fill, fillByte=$2]
.segment bankStart_01_1
.segmentdef bankStart_02_0 [min=$8000, max=$9fff, fill, fillByte=$3]
.segment bankStart_02_0
.segmentdef bankStart_02_1 [min=$a000, max=$bfff, fill, fillByte=$4]
.segment bankStart_02_1

// =============================================================================
.segment bankStart_00_0

.label EASYFLASH_BANK    = $DE00
.label EASYFLASH_CONTROL = $DE02
.const EASYFLASH_LED     = $80
.const EASYFLASH_16K     = $07
.const EASYFLASH_KILL    = $04

// This code resides on LOROM, it becomes visible at $8000
.pseudopc $8000 {

    // === the main application entry point ===
    // copy the main code to $C000 (or whereever) - we don't run it here
    // since the banking would make it invisible
    // it may be a good idea to let exomizer do this in real life
    ldx #0
lp1:
    lda mainEf,x
    sta $c000,x
    dex
    bne lp1
    jmp $c000
}
mainEf:
.pseudopc $C000 {
    // Switch to bank 1, get a byte from LOROM and HIROM
    lda #1
    sta EASYFLASH_BANK
    lda $8000
    ldx $a000
    // and put them to the screen, we should see "A" and "B" there
    sta $0400
    stx $0401

    // Switch to bank 2, get a byte from LOROM and HIROM
    lda #2
    sta EASYFLASH_BANK
    lda $8000
    ldx $a000
    // and put them to the screen, we should see "C" and "D" there
    sta $0400 + 40
    stx $0401 + 40

    // effect!
lp2:
    dec $d020
    jmp lp2
}

// =============================================================================
// 00:1:0000 (HIROM, bank 0)
.segment bankStart_00_1

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
        ldx #(startUpEnd - startUpCode)
l1:
        lda startUpCode, x
        sta $0100, x
        dex
        bpl l1
        jmp $0100
    }
startUpCode:
    .pseudopc $0100 {
        // === this code is copied to the stack area, does some inits ===
        // === scans the keyboard and kills the cartridge or          ===
        // === starts the main application                            ===
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

        // These may not be needed - depending on what you'll do
        jsr $ff87   // Initialise System Constants
        jsr $ff8a   // Restore Kernal Vectors
        jsr $ff81   // Initialize screen editor

        // start the application code
        jmp $8000

kill:
        lda #EASYFLASH_KILL
        sta EASYFLASH_CONTROL
        jmp ($fffc) // reset
    }
startUpEnd:

        // fill it up to $FFFA to put the vectors there
        //.align $ffff, $fffa, $ff
        *=$fffa
        .word reti        // NMI
        .word coldStart   // RESET

        // we don't need the IRQ vector and can put RTI here to save space :)
reti:
        rti
        .byte $ff
