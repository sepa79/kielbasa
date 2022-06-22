#ifndef TEXTS_PL_H
#define TEXTS_PL_H

// Sections and regions
#pragma section( txtPlPlantTypes, 0 )
#pragma section( txtPlPlantStateNames, 0 )
#pragma section( txtPlMainArray, 0 )
#pragma section( txtPlTxtData, 0 )
#pragma region( bankTxtPl1, 0x8000, 0x8050, , TRANSLATION_PL_BANK, { txtPlPlantTypes }, 0x6000 )
#pragma region( bankTxtPl2, 0x8050, 0x8100, , TRANSLATION_PL_BANK, { txtPlPlantStateNames }, 0x6050 )
#pragma region( bankTxtPlArray, 0x8100, 0x8200, , TRANSLATION_PL_BANK, { txtPlMainArray }, 0x6100 )
#pragma region( bankTxtPlData, 0x8200, 0x8eff, , TRANSLATION_PL_BANK, { txtPlTxtData }, 0x6200 )

#pragma section( txtCommonCode, 0 )
#pragma section( txtCommonData, 0 )
#pragma region( bankTxtCommonCode, 0x8f00, 0x8fff, , {TRANSLATION_PL_BANK, TRANSLATION_EN_BANK}, { txtCommonCode, txtCommonData } )

#pragma code ( txtCommonCode )
#pragma data ( txtCommonData )

void loadTranslation();

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )

#pragma compile("textsPL.c")
#endif
