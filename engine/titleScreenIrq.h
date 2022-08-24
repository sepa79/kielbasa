#ifndef TITLE_SCREEN_IRQ_H
#define TITLE_SCREEN_IRQ_H

// Switching code generation to shared section
#pragma code ( mainCode )
#pragma data ( mainData )

__interrupt void titleScreenIrq1();
__interrupt void titleScreenIrq2();

#pragma compile("titleScreenIrq.c")

#endif