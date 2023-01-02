#ifndef IRQ_HANDLER_H
#define IRQ_HANDLER_H

const char * SB_TEXT;

void playMsx();
void joyUpdate();

void initRasterIRQ();
void splitScreenIRQ1();
void splitScreenIRQ2();
void controlIRQ1();
void controlIRQ2();

#pragma compile("irqHandler.c")

#endif