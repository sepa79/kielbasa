unsigned char sbits[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

/* Show sprite, sprIdx is 0 based */
void spriteShow(unsigned char sprIdx)
{
    __asm{
        ldy #sprIdx            // get the index of passed parameter
        lda (fp), y
        tax
        lda sbits, x
        ora $d015
        sta $d015
    };
}

void spriteHide(unsigned char sprIdx)
{
    __asm{
        ldy #sprIdx            // get the index of passed parameter
        lda (fp), y
        tax
        lda sbits, x
        eor #$ff
        and $d015
        sta $d015
    };
}

void spritePos(unsigned char sprIdx, unsigned int x, unsigned char y)
{
    __asm {
        ldy #sprIdx          // get the index of passed parameter
        lda (fp), y
        asl
        tax
        
        ldy #y               // y first, as its less hassle
        lda (fp), y         
        sta $d001,x

        ldy #x               // now the x
        lda (fp), y          // low byte of X
        sta $d000,x

        // high bit for X
        // ldy #sprIdx          // get the index of passed parameter
        // lda (fp), y
        // tax
        txa                  // get the index of passed parameter
        clc
        ror
        tax
        lda sbits, x
        tax                  // get the bit representing sprite ready in X
        ldy #x               // now the x part 2
        iny
        lda (fp), y          // high byte of X
        beq zeroIt
        txa
        ora $d010
        jmp setHX
    zeroIt:
        txa
        eor #$ff
        and $d010
    setHX:
        sta $d010
    };
}

/* Currently hardcoded to $07f8 */
void spriteBank(unsigned char sprIdx, unsigned char bank)
{
    __asm{
        ldy #sprIdx          // get the index of passed parameter
        lda (fp), y
        tax
        ldy #bank            // bank
        lda (fp), y
        sta $07F8, x
    };
}
