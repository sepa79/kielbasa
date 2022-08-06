#ifndef STONKA_IRQ_H
#define STONKA_IRQ_H

__interrupt void msxIrq1();
__interrupt void msxIrq2();

#pragma compile("stonkaIrq.c")

#endif