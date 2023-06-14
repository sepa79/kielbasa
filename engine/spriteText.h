#ifndef SPRITE_TEXT_H
#define SPRITE_TEXT_H

void copyCharToSprite(char c, char col, char row, char * sprBankPtr);
void textToSprite(char * str, char width, char * sprBankPtr);
void intToWeightToSprite(unsigned int num, char * sprBankPtr);
void byteToSprite(char byte, char * sprBankPtr);

#pragma compile("spriteText.c")

#endif