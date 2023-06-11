#ifndef IRQ_SPRITE_CONTROLLER_H
#define IRQ_SPRITE_CONTROLLER_H
#include <character/character.h>

extern volatile char isc_statusTextColorIdx;
// TODO: move non-interrupt methods to UI handler or other place
void setWeatherIcon(char sprIdx);
void setTimeSpeedIcon(char sprIdx);
void setCharacterSlotPic(char charIdx);
void setCharacterSlotIcon(char charIdx, char * taskIconPtr);
void setErrorCursor();
void setNormalCursor();
void drawBattery(char charIdx);
__interrupt void setSpritesTopScr();
__interrupt void setSpritesBottomScr();
__interrupt void showUiSpritesTop();
__interrupt void showUiSpritesBottom();
__interrupt void showMapSpritesBottom();

#pragma compile("irqSpriteController.c")

#endif