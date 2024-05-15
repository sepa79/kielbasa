#ifndef TEXTS_PL_H
#define TEXTS_PL_H

//
//
// This file is auto-generated by 'create_translations_yaml_jinja2.py' script.
//
//


//      "pragma_label":  Txt
//      "array_label":   TXT
//      "indexes_count": 83
//      "array_length":  1710

#pragma section( plTxt, 0 )
#pragma section( plTxtData, 0 )
#pragma region( bankPlTxtArray, 0x8000, 0x80a6, , TRANSLATION_PL_BANK, { plTxt}, 0x5c00 )
#pragma region( bankPlTxtData, 0x80a6, 0x8754, , TRANSLATION_PL_BANK, { plTxtData}, 0x5ca6 )

//      "pragma_label":  TxtCrew
//      "array_label":   TXT_CREW
//      "indexes_count": 7
//      "array_length":  62

#pragma section( plTxtCrew, 0 )
#pragma section( plTxtCrewData, 0 )
#pragma region( bankPlTxtCrewArray, 0x8755, 0x8763, , TRANSLATION_PL_BANK, { plTxtCrew}, 0x6400 )
#pragma region( bankPlTxtCrewData, 0x8763, 0x87a1, , TRANSLATION_PL_BANK, { plTxtCrewData}, 0x640e )

//      "pragma_label":  TxtFarmland
//      "array_label":   TXT_FARMLAND
//      "indexes_count": 32
//      "array_length":  434

#pragma section( plTxtFarmland, 0 )
#pragma section( plTxtFarmlandData, 0 )
#pragma region( bankPlTxtFarmlandArray, 0x87a2, 0x87e2, , TRANSLATION_PL_BANK, { plTxtFarmland}, 0x6400 )
#pragma region( bankPlTxtFarmlandData, 0x87e2, 0x8994, , TRANSLATION_PL_BANK, { plTxtFarmlandData}, 0x6440 )

//      "pragma_label":  TxtKitchen
//      "array_label":   TXT_KITCHEN
//      "indexes_count": 9
//      "array_length":  198

#pragma section( plTxtKitchen, 0 )
#pragma section( plTxtKitchenData, 0 )
#pragma region( bankPlTxtKitchenArray, 0x8995, 0x89a7, , TRANSLATION_PL_BANK, { plTxtKitchen}, 0x6400 )
#pragma region( bankPlTxtKitchenData, 0x89a7, 0x8a6d, , TRANSLATION_PL_BANK, { plTxtKitchenData}, 0x6412 )

//      "pragma_label":  TxtVillageMap
//      "array_label":   TXT_VILLAGE_MAP
//      "indexes_count": 22
//      "array_length":  550

#pragma section( plTxtVillageMap, 0 )
#pragma section( plTxtVillageMapData, 0 )
#pragma region( bankPlTxtVillageMapArray, 0x8a6e, 0x8a9a, , TRANSLATION_PL_BANK, { plTxtVillageMap}, 0x6400 )
#pragma region( bankPlTxtVillageMapData, 0x8a9a, 0x8cc0, , TRANSLATION_PL_BANK, { plTxtVillageMapData}, 0x642c )

//      "pragma_label":  TxtOptionsMenu
//      "array_label":   TXT_OPTIONS_MENU
//      "indexes_count": 7
//      "array_length":  162

#pragma section( plTxtOptionsMenu, 0 )
#pragma section( plTxtOptionsMenuData, 0 )
#pragma region( bankPlTxtOptionsMenuArray, 0x8cc1, 0x8ccf, , TRANSLATION_PL_BANK, { plTxtOptionsMenu}, 0x6400 )
#pragma region( bankPlTxtOptionsMenuData, 0x8ccf, 0x8d71, , TRANSLATION_PL_BANK, { plTxtOptionsMenuData}, 0x640e )

//      "pragma_label":  TxtTaskManager
//      "array_label":   TXT_TASK_MANAGER
//      "indexes_count": 11
//      "array_length":  426

#pragma section( plTxtTaskManager, 0 )
#pragma section( plTxtTaskManagerData, 0 )
#pragma region( bankPlTxtTaskManagerArray, 0x8d72, 0x8d88, , TRANSLATION_PL_BANK, { plTxtTaskManager}, 0x6400 )
#pragma region( bankPlTxtTaskManagerData, 0x8d88, 0x8f32, , TRANSLATION_PL_BANK, { plTxtTaskManagerData}, 0x6416 )


// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )

void loadTranslation();

#pragma compile("textsPL.c")
#endif