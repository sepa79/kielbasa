#ifndef IRQ_SPRITE_CONTROLLER_H
#define IRQ_SPRITE_CONTROLLER_H
#include <character/character.h>

extern volatile char isc_statusTextColorIdx;
void setWeatherIcon(char sprIdx);
void setTimeSpeedIcon(char sprIdx);
void setCharacterSlotPic(char charIdx);
void setCharacterSlotIcon(char charIdx, char * taskIconPtr);
void setErrorCursor();
void setNormalCursor();
__interrupt void setSpritesTopScr();
void setSpritesBottomScr();
void showUiSpritesTop();
void showUiSpritesBottom();
void showMapSpritesBottom();
void drawBattery(char charIdx);

#pragma compile("irqSpriteController.c")

#endif