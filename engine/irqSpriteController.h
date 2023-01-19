#ifndef IRQ_SPRITE_CONTROLLER_H
#define IRQ_SPRITE_CONTROLLER_H
#include <character/character.h>

void setCharacterSlotPic(char charIdx);
void setCharacterSlotIcon(char charIdx, const char * taskIconPtr);
void setErrorCursor();
void setNormalCursor();
__interrupt void setSpritesTopScr();
void setSpritesBottomScr();
void showUiSpritesTop();
void showUiSpritesBottom();
void drawBattery(char charIdx);

#pragma compile("irqSpriteController.c")

#endif