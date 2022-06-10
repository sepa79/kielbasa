#ifndef TEXTS_EN_H
#define TEXTS_EN_H

// Sections and regions
#pragma section( txtEnPlantTypes, 0 )
#pragma section( txtEnPlantStateNames, 0 )
#pragma section( txtEnMainArray, 0 )
#pragma section( txtEnTxtData, 0 )
#pragma region( bankTxtEn1, 0x8000, 0x8050, , TRANSLATION_EN_BANK, { txtEnPlantTypes }, 0x6000 )
#pragma region( bankTxtEn2, 0x8050, 0x8100, , TRANSLATION_EN_BANK, { txtEnPlantStateNames }, 0x6050 )
#pragma region( bankTxtEnArray, 0x8100, 0x8200, , TRANSLATION_EN_BANK, { txtEnMainArray }, 0x6100 )
#pragma region( bankTxtEnData, 0x8200, 0x8eff, , TRANSLATION_EN_BANK, { txtEnTxtData }, 0x6200 )

#pragma compile("textsEN.c")
#endif
