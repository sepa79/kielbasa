#ifndef IRQ_SPRITE_CONTROLLER_H
#define IRQ_SPRITE_CONTROLLER_H

// Switching code generation to shared section
#pragma code ( mainCode )
#pragma data ( mainData )

extern volatile char isc_weatherSprite;

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