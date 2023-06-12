#ifndef SPRITE_TEXT_H
#define SPRITE_TEXT_H

void copyCharToSprite(char c, char col, char row, char * sprBankPtr);
void textToSprite(char * str, char width, char * textToSpriteBankPt);

#pragma compile("spriteText.c")

#endif