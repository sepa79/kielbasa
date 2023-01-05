#ifndef IRQ_HANDLER_H
#define IRQ_HANDLER_H

extern const char * SB_TEXT;
// stores screen mode on change
extern char currentScreenMode;
// used by F1-F7 menus to store previous screen mode, to be able to change back
extern char previousScreenMode;

enum SCREEN_MODE {
// Restore the screen using 'previousScreenMode'
    SCREEN_RESTORE,
// Partial IRQ init - full TXT mode, with regular UI on top & bottom, MSX, normal txt screen sprite handlers used.
    SCREEN_SPLIT_MC_TXT,
// Partial IRQ init - split screen mode, with regular UI on top & bottom, MSX, MC on top half with sprites, text on bottom half with sprites.
    SCREEN_FULL_TXT,
// Partial IRQ init - full MC TXT mode, with regular UI on top & bottom, MSX, no sprite handlers.
    SCREEN_MC_TXT,
// Partial IRQ init - full Hires (normal) TXT mode, with regular UI on top & bottom, MSX, no sprite handlers.
    SCREEN_HIRES_TXT,
    SCREEN_PIGSLE_COMMAND,
};

void playMsx();
void joyUpdate();
void switchScreenTo(byte screenMode);

// Main IRQ init - must be called first, sets up all the bits.
void initRasterIRQ();
// // Partial IRQ init - full TXT mode, with regular UI on top & bottom, MSX, normal txt screen sprite handlers used.
// void switchScreenToFullTxt();
// // Partial IRQ init - split screen mode, with regular UI on top & bottom, MSX, MC on top half with sprites, text on bottom half with sprites.
// void switchScreenToSplitMcTxt();
// // Partial IRQ init - full MC TXT mode, with regular UI on top & bottom, MSX, no sprite handlers.
// void switchScreenToFullMCTxt();
// // Partial IRQ init - full Hires (normal) TXT mode, with regular UI on top & bottom, MSX, no sprite handlers.
// void switchScreenToFullHiresTxt();

#pragma compile("irqHandler.c")

#endif