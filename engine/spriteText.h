#ifndef SPRITE_TEXT_H
#define SPRITE_TEXT_H

// set it to sprite bank that you wish to use textToSprite() on
extern char * textToSpriteBankPt;

void textToSprite(char * str, char width);

#pragma compile("spriteText.c")

#endif