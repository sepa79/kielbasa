#ifndef IRQ_SPRITE_CONTROLLER_H
#define IRQ_SPRITE_CONTROLLER_H

void setCharacterSlotPic(char slot, char picture);
void setCharacterSlotIcon(char slot, char * taskIconPtr);
void setErrorCursor();
void setNormalCursor();
__interrupt void setSpritesTopScr();
void setSpritesBottomScr();
void showUiSpritesTop();
void showUiSpritesBottom();
void drawBattery(char slot, char energy);

#pragma compile("irqSpriteController.c")

#endif