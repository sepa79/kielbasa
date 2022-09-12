#ifndef TITLE_SCREEN_IRQ_H
#define TITLE_SCREEN_IRQ_H

//-----------------------------------------------------------------------------------------
#pragma code ( gameInitRAMCode )
#pragma data ( gameInitData )
//-----------------------------------------------------------------------------------------

__interrupt void titleScreenIrq1();
__interrupt void titleScreenIrq2();

//-----------------------------------------------------------------------------------------
#pragma code ( code )
#pragma data ( data )
//-----------------------------------------------------------------------------------------

#pragma compile("titleScreenIrq.c")

#endif