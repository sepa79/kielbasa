#ifndef IRQ_HANDLER_H
#define IRQ_HANDLER_H

const char * SB_TEXT;

void playMsx();
void joyUpdate();

void initRasterIRQ();
void switchScreenToFullTxt();
void switchScreenToSplitMcTxt();
void switchScreenToFullMCTxt();
void switchScreenToFullHiresTxt();

#pragma compile("irqHandler.c")

#endif