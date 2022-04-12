#ifndef IRQ_SPRITE_CONTROLLER_H
#define IRQ_SPRITE_CONTROLLER_H

void setCharacterSlotPic(char charSlot, const char * picturePtr);
void setCharacterSlotIcon(char charSlot, const char * taskIconPtr);
void setErrorCursor();
void setNormalCursor();
__interrupt void setSpritesTopScr();
void setSpritesBottomScr();
void showUiSpritesTop();
void showUiSpritesBottom();
void drawBattery(char charSlot, char energy);

#pragma compile("irqSpriteController.c")

#endif