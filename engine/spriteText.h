#ifndef SPRITE_TEXT_H
#define SPRITE_TEXT_H

void copyCharToSprite(char c, char col, char row, char * sprBankPtr);
/**
 * Width is in sprites - 4 means 12 chars will fit.
 * Routine will keep writing row by row, so make sure text is going to fit in.
*/
void textToSprite(char * str, char width, char * sprBankPtr);
void textToSpriteAt(char * str, char width, char * sprBankPtr, char col, char row);
void intToWeightToSprite(unsigned int num, char * sprBankPtr);
void byteToSprite(char byte, char * sprBankPtr);
// __interrupt void setSpriteTextBlock(char sprBankStart, unsigned int xPos, char yPos, char xSize, char ySize, char color);

#pragma compile("spriteText.c")

#endif