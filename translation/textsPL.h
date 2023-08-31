#ifndef TEXTS_PL_H
#define TEXTS_PL_H

//
//
// This file is auto-generated by 'create_translations.py' script.
//
//

// Sections and regions
#pragma section( txtPlPlantTypes, 0 )
#pragma section( txtPlFieldSizes, 0 )
//#pragma section( txtPlPlantStateNames, 0 )
#pragma section( txtPlPlantStageNames, 0 )
#pragma section( txtPlLocationNames, 0 )
#pragma section( txtPlMainArray, 0 )
#pragma section( txtPlTxtData, 0 )
#pragma region( bankTxtPl1, 0x8000, 0x8100, ,TRANSLATION_PL_BANK, {
    txtPlPlantTypes,
    txtPlFieldSizes,
//    txtPlPlantStateNames,
    txtPlPlantStageNames,
    txtPlLocationNames
}, 0x6000 )

#pragma region( bankTxtPlArray, 0x8100, 0x8200, , TRANSLATION_PL_BANK, { txtPlMainArray }, 0x6100 )
#pragma region( bankTxtPlData, 0x8200, 0x8fff, , TRANSLATION_PL_BANK, { txtPlTxtData }, 0x6200 )

#pragma code ( code )
#pragma data ( data )

void loadTranslation();

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )


#pragma compile("textsPL.c")
#endif