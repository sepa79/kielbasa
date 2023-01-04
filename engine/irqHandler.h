#ifndef IRQ_HANDLER_H
#define IRQ_HANDLER_H

const char * SB_TEXT;

void playMsx();
void joyUpdate();

// Main IRQ init - must be called first, sets up all the bits.
void initRasterIRQ();
// Partial IRQ init - full TXT mode, with regular UI on top & bottom, MSX, normal txt screen sprite handlers used.
void switchScreenToFullTxt();
// Partial IRQ init - split screen mode, with regular UI on top & bottom, MSX, MC on top half with sprites, text on bottom half with sprites.
void switchScreenToSplitMcTxt();
// Partial IRQ init - full MC TXT mode, with regular UI on top & bottom, MSX, no sprite handlers.
void switchScreenToFullMCTxt();
// Partial IRQ init - full Hires (normal) TXT mode, with regular UI on top & bottom, MSX, no sprite handlers.
void switchScreenToFullHiresTxt();

#pragma compile("irqHandler.c")

#endif