#ifndef TITLE_SCREEN_IRQ_H
#define TITLE_SCREEN_IRQ_H

__interrupt void titleScreenIrq1();
__interrupt void titleScreenIrq2();

#pragma compile("titleScreenIrq.c")

#endif