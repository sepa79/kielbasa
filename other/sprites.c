#define VIC ((byte*)0xd000)
#define SPRITE_LOC_X ((byte*)0xd000)
#define SPRITE_LOC_Y ((byte*)0xd001)
#define SPRITE_HIGH_X ((byte*)0xd010)
#define SPRITE_POINTERS ((byte*)0x07f8)

/* Show sprite (index is 0 based) */
void spriteShow(byte sprIdx)
{
    VIC[0x15] |= 1 << sprIdx;
}
#pragma native(spriteShow)

/* Hide sprite (index is 0 based) */
void spriteHide(byte sprIdx)
{
    VIC[0x15] &= ~(1 << sprIdx);
}
#pragma native(spriteHide)

/* Position sprite (index is 0 based) */
void spritePos(byte sprIdx, unsigned int x, byte y)
{
    char sprIdx2 = sprIdx << 1;
    SPRITE_LOC_Y[sprIdx2] = y;
    byte lowX = x & 0xff;
    SPRITE_LOC_X[sprIdx2] = lowX;
    if(x > 255) {
        SPRITE_HIGH_X[0] |= 1 << sprIdx;
    } else {
        SPRITE_HIGH_X[0] &= ~(1 << sprIdx);
    }
}
#pragma native(spritePos)

/* Set bank (index is 0 based).
Currently hardcoded to $07f8 */
void spriteBank(byte sprIdx, byte bank)
{
    SPRITE_POINTERS[sprIdx] = bank;
}
#pragma native(spriteBank)
