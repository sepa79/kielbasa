/* Show sprite, sprIdx is 0 based */
void spriteShow(unsigned char sprIdx);
void spriteHide(unsigned char sprIdx);
void spritePos(unsigned char sprIdx, unsigned int x, unsigned char y);
void spriteBank(unsigned char sprIdx, unsigned char bank);

const unsigned char SPRITE_MIN_Y = 50;
const unsigned char SPRITE_MAX_Y = 229;
const unsigned char SPRITE_MIN_X = 24;
const unsigned int SPRITE_MAX_X = 320;

#pragma compile("sprites.c")
