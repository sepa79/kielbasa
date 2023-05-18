#ifndef SPRITE_TEXT_H
#define SPRITE_TEXT_H

// set it to sprite bank that you wish to use textToSprite() on
extern byte * textSprBankPt;

void textToSprite(char * str, char width);

#endif