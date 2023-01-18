#ifndef IRQ_SPRITE_CONTROLLER_H
#define IRQ_SPRITE_CONTROLLER_H
#include <character/character.h>

void setCharacterSlotPic(struct CharacterStruct * charPtr);
void setCharacterSlotIcon(struct CharacterStruct * charPtr, const char * taskIconPtr);
void setErrorCursor();
void setNormalCursor();
__interrupt void setSpritesTopScr();
void setSpritesBottomScr();
void showUiSpritesTop();
void showUiSpritesBottom();
void drawBattery(struct CharacterStruct * charPtr);

#pragma compile("irqSpriteController.c")

#endif