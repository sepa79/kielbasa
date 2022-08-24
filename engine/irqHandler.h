#ifndef IRQ_HANDLER_H
#define IRQ_HANDLER_H

// Switching code generation to shared section
#pragma code ( mainCode )
#pragma data ( mainData )

const char * SB_TEXT;

void initRasterIRQ();
void splitScreenIRQ1();
void splitScreenIRQ2();
void controlIRQ1();
void controlIRQ2();

#pragma compile("irqHandler.c")

#endif