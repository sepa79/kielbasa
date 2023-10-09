#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>

//
//
// This file is auto-generated by 'create_translations.py' script.
//
//

#pragma data ( txtPlTxtData )



//     "pragma_label": MainArray,
//     "array_label": TXT,


//     "pl": "ü Wyjdź"
const char TXT_PL_MENU_EXIT[] = {0x1f, 0x7c, 0x20, 0x57, 0x19, 0x0a, 0x04, 0x62, 0x00};
//     "pl": "ü Mapa"
const char TXT_PL_EXIT_TO_MAP[] = {0x1f, 0x7c, 0x20, 0x4d, 0x01, 0x10, 0x01, 0x00};
//     "pl": "Język / Language"
const char TXT_PL_MENU_OPTIONS_LANG[] = {0x71, 0x4a, 0x5d, 0x1a, 0x19, 0x0b, 0x20, 0x2f, 0x20, 0x4c, 0x01, 0x0e, 0x07, 0x15, 0x01, 0x07, 0x05, 0x00};
//     "pl": "Ustawienia muzyki"
const char TXT_PL_MENU_OPTIONS_MSX[] = {0x72, 0x55, 0x13, 0x14, 0x01, 0x17, 0x09, 0x05, 0x0e, 0x09, 0x01, 0x20, 0x0d, 0x15, 0x1a, 0x19, 0x0b, 0x09, 0x00};
//     "pl": "Ustawienia gry"
const char TXT_PL_OPTIONS_HEADER[] = {0x55, 0x13, 0x14, 0x01, 0x17, 0x09, 0x05, 0x0e, 0x09, 0x01, 0x20, 0x07, 0x12, 0x19, 0x00};
//     "pl": "Znaczenie ikon UI:"
const char TXT_PL_OPTIONS_HELP1[] = {0x5a, 0x0e, 0x01, 0x03, 0x1a, 0x05, 0x0e, 0x09, 0x05, 0x20, 0x09, 0x0b, 0x0f, 0x0e, 0x20, 0x55, 0x49, 0x3a, 0x00};
//     "pl": "ü - Długi fire (1s lub dłużej)"
const char TXT_PL_OPTIONS_HELP2[] = {0x7c, 0x20, 0x2d, 0x20, 0x44, 0x5e, 0x15, 0x07, 0x09, 0x20, 0x06, 0x09, 0x12, 0x05, 0x20, 0x28, 0x31, 0x13, 0x20, 0x0c, 0x15, 0x02, 0x20, 0x04, 0x5e, 0x15, 0x63, 0x05, 0x0a, 0x29, 0x00};
//     "pl": "ý - Fire"
const char TXT_PL_OPTIONS_HELP3[] = {0x7d, 0x20, 0x2d, 0x20, 0x46, 0x09, 0x12, 0x05, 0x00};
//     "pl": "û - Klawisz 'Return'"
const char TXT_PL_OPTIONS_HELP4[] = {0x7b, 0x20, 0x2d, 0x20, 0x4b, 0x0c, 0x01, 0x17, 0x09, 0x13, 0x1a, 0x20, 0x27, 0x52, 0x05, 0x14, 0x15, 0x12, 0x0e, 0x27, 0x00};
//     "pl": "½ - 'Escape' w menu i na mapie"
const char TXT_PL_OPTIONS_HELP5[] = {0x1f, 0x20, 0x2d, 0x20, 0x27, 0x45, 0x13, 0x03, 0x01, 0x10, 0x05, 0x27, 0x20, 0x17, 0x20, 0x0d, 0x05, 0x0e, 0x15, 0x20, 0x09, 0x20, 0x0e, 0x01, 0x20, 0x0d, 0x01, 0x10, 0x09, 0x05, 0x00};
//     "pl": "Ustawienia muzyki"
const char TXT_PL_MUSIC_OPTIONS_HEADER[] = {0x55, 0x13, 0x14, 0x01, 0x17, 0x09, 0x05, 0x0e, 0x09, 0x01, 0x20, 0x0d, 0x15, 0x1a, 0x19, 0x0b, 0x09, 0x00};
//     "pl": "Wybór muzyki"
//           "  M on/off  "
//           "   ü Wyjdź  "
const char SB_PL_MENU_OPTIONS_MSX_MENU[] = {0x57, 0x19, 0x02, 0x60, 0x12, 0x20, 0x0d, 0x15, 0x1a, 0x19, 0x0b, 0x09, 0x20, 0x20, 0x4d, 0x20, 0x0f, 0x0e, 0x2f, 0x0f, 0x06, 0x06, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7c, 0x20, 0x57, 0x19, 0x0a, 0x04, 0x62, 0x20, 0x20, 0x00};
//     "pl": "Logi i wydarzenia"
const char TXT_PL_LOG_MENU_HEADER[] = {0x4c, 0x0f, 0x07, 0x09, 0x20, 0x09, 0x20, 0x17, 0x19, 0x04, 0x01, 0x12, 0x1a, 0x05, 0x0e, 0x09, 0x01, 0x00};
//     "pl": " Data   | Opis                          "
const char TXT_PL_LOG_MENU_LOG_TABLE_HEADER[] = {0xa0, 0xc4, 0x81, 0x94, 0x81, 0xa0, 0xa0, 0xa0, 0xfe, 0xa0, 0xcf, 0x90, 0x89, 0x93, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0x00};
//     "pl": "ûý"
const char TXT_PL_MENU_SELECT[] = {0x7b, 0x7d, 0x00};
//     "pl": "w"
const char TXT_PL_MENU_W[] = {0x17, 0x00};
//     "pl": "s"
const char TXT_PL_MENU_S[] = {0x13, 0x00};
//     "pl": "a"
const char TXT_PL_MENU_A[] = {0x01, 0x00};
//     "pl": "d"
const char TXT_PL_MENU_D[] = {0x04, 0x00};
//     "pl": "Manager zadań"
const char TXT_PL_TASK_MANAGER_HEADER[] = {0x4d, 0x01, 0x0e, 0x01, 0x07, 0x05, 0x12, 0x20, 0x1a, 0x01, 0x04, 0x01, 0x5f, 0x00};
//     "pl": "Zadanie |       Opis        "
const char TXT_PL_TASK_MANAGER_TABLE_HEADER[] = {0xda, 0x81, 0x84, 0x81, 0x8e, 0x89, 0x85, 0xa0, 0xfe, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xcf, 0x90, 0x89, 0x93, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0x00};
//     "pl": "¼#  Pole   |   Stan   "
const char TXT_PL_FIELD_LIST_HEADER[] = {0x9e, 0xa3, 0xa0, 0xa0, 0xd0, 0x8f, 0x8c, 0x85, 0xa0, 0xa0, 0xa0, 0xfe, 0xa0, 0xa0, 0xa0, 0xd3, 0x94, 0x81, 0x8e, 0xa0, 0xa0, 0xa0, 0x00};
//     "pl": "Pole:  "
const char TXT_PL_FARM_FIELD_TXT[] = {0x50, 0x0f, 0x0c, 0x05, 0x3a, 0x20, 0x20, 0x00};
//     "pl": "Uprawa: "
const char TXT_PL_FARM_PLANT_TXT[] = {0x55, 0x10, 0x12, 0x01, 0x17, 0x01, 0x3a, 0x20, 0x00};
//     "pl": "Mądrość"
const char TXT_PL_STAT_INT[] = {0x4d, 0x5b, 0x04, 0x12, 0x0f, 0x61, 0x5c, 0x00};
//     "pl": "Siła"
const char TXT_PL_STAT_STR[] = {0x53, 0x09, 0x5e, 0x01, 0x00};
//     "pl": "Spryt"
const char TXT_PL_STAT_CUN[] = {0x53, 0x10, 0x12, 0x19, 0x14, 0x00};
//     "pl": "Hodowla   "
const char TXT_PL_SKILL_ANI[] = {0x48, 0x0f, 0x04, 0x0f, 0x17, 0x0c, 0x01, 0x20, 0x20, 0x20, 0x00};
//     "pl": "Rolnictwo "
const char TXT_PL_SKILL_FRM[] = {0x52, 0x0f, 0x0c, 0x0e, 0x09, 0x03, 0x14, 0x17, 0x0f, 0x20, 0x00};
//     "pl": "Gotowanie "
const char TXT_PL_SKILL_BTH[] = {0x47, 0x0f, 0x14, 0x0f, 0x17, 0x01, 0x0e, 0x09, 0x05, 0x20, 0x00};
//     "pl": "Handel    "
const char TXT_PL_SKILL_TRD[] = {0x48, 0x01, 0x0e, 0x04, 0x05, 0x0c, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "   Zadanie  "
//           "  dodane do "
//           "   kolejki  "
const char SB_PL_TASK_ADDED[] = {0x20, 0x20, 0x20, 0x5a, 0x01, 0x04, 0x01, 0x0e, 0x09, 0x05, 0x20, 0x20, 0x20, 0x20, 0x04, 0x0f, 0x04, 0x01, 0x0e, 0x05, 0x20, 0x04, 0x0f, 0x20, 0x20, 0x20, 0x20, 0x0b, 0x0f, 0x0c, 0x05, 0x0a, 0x0b, 0x09, 0x20, 0x20, 0x00};
//     "pl": "   Kolejka  "
//           "    zadań   "
//           "    pełna   "
const char SB_PL_TASKS_FULL[] = {0x20, 0x20, 0x20, 0x4b, 0x0f, 0x0c, 0x05, 0x0a, 0x0b, 0x01, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x1a, 0x01, 0x04, 0x01, 0x5f, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x05, 0x5e, 0x0e, 0x01, 0x20, 0x20, 0x20, 0x00};
//     "pl": " +/-/Space: "
//           "prędkość gry"
//           "  F1: Pomoc "
const char SB_PL_WELCOME[] = {0x20, 0x2b, 0x2f, 0x2d, 0x2f, 0x53, 0x10, 0x01, 0x03, 0x05, 0x3a, 0x20, 0x10, 0x12, 0x5d, 0x04, 0x0b, 0x0f, 0x61, 0x5c, 0x20, 0x07, 0x12, 0x19, 0x20, 0x20, 0x46, 0x31, 0x3a, 0x20, 0x50, 0x0f, 0x0d, 0x0f, 0x03, 0x20, 0x00};
//     "pl": "Wybierz:"
const char TXT_PL_CONFIRM_TXT[] = {0x57, 0x19, 0x02, 0x09, 0x05, 0x12, 0x1a, 0x3a, 0x00};
//     "pl": "    Akcja   "
//           "   wymaga   "
//           " akceptacji "
const char TXT_PL_CONFIRM_ERROR[] = {0x20, 0x20, 0x20, 0x20, 0x41, 0x0b, 0x03, 0x0a, 0x01, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x17, 0x19, 0x0d, 0x01, 0x07, 0x01, 0x20, 0x20, 0x20, 0x20, 0x01, 0x0b, 0x03, 0x05, 0x10, 0x14, 0x01, 0x03, 0x0a, 0x09, 0x20, 0x00};
//     "pl": "  Tak   "
const char TXT_PL_CONFIRM_YES[] = {0x20, 0x20, 0x54, 0x01, 0x0b, 0x20, 0x20, 0x20, 0x00};
//     "pl": "  Nie   "
const char TXT_PL_CONFIRM_NO[] = {0x20, 0x20, 0x4e, 0x09, 0x05, 0x20, 0x20, 0x20, 0x00};
//     "pl": "Slot 1"
const char TXT_PL_MENU_CREW1[] = {0x71, 0x53, 0x0c, 0x0f, 0x14, 0x20, 0x31, 0x00};
//     "pl": "Slot 2"
const char TXT_PL_MENU_CREW2[] = {0x72, 0x53, 0x0c, 0x0f, 0x14, 0x20, 0x32, 0x00};
//     "pl": "Slot 3"
const char TXT_PL_MENU_CREW3[] = {0x73, 0x53, 0x0c, 0x0f, 0x14, 0x20, 0x33, 0x00};
//     "pl": "Slot 4"
const char TXT_PL_MENU_CREW4[] = {0x74, 0x53, 0x0c, 0x0f, 0x14, 0x20, 0x34, 0x00};
//     "pl": "Janusz całe życie pracował wPGR. Na niczym się tak      naprawdę nie zna, ale za to ma nałogi i jest leniwy.    Janusz to postać startowa."
const char TXT_PL_CHAR_1_STORY[] = {0x4a, 0x01, 0x0e, 0x15, 0x13, 0x1a, 0x20, 0x03, 0x01, 0x5e, 0x05, 0x20, 0x63, 0x19, 0x03, 0x09, 0x05, 0x20, 0x10, 0x12, 0x01, 0x03, 0x0f, 0x17, 0x01, 0x5e, 0x20, 0x17, 0x50, 0x47, 0x52, 0x2e, 0x20, 0x4e, 0x01, 0x20, 0x0e, 0x09, 0x03, 0x1a, 0x19, 0x0d, 0x20, 0x13, 0x09, 0x5d, 0x20, 0x14, 0x01, 0x0b, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0e, 0x01, 0x10, 0x12, 0x01, 0x17, 0x04, 0x5d, 0x20, 0x0e, 0x09, 0x05, 0x20, 0x1a, 0x0e, 0x01, 0x2c, 0x20, 0x01, 0x0c, 0x05, 0x20, 0x1a, 0x01, 0x20, 0x14, 0x0f, 0x20, 0x0d, 0x01, 0x20, 0x0e, 0x01, 0x5e, 0x0f, 0x07, 0x09, 0x20, 0x09, 0x20, 0x0a, 0x05, 0x13, 0x14, 0x20, 0x0c, 0x05, 0x0e, 0x09, 0x17, 0x19, 0x2e, 0x20, 0x20, 0x20, 0x20, 0x4a, 0x01, 0x0e, 0x15, 0x13, 0x1a, 0x20, 0x14, 0x0f, 0x20, 0x10, 0x0f, 0x13, 0x14, 0x01, 0x5c, 0x20, 0x13, 0x14, 0x01, 0x12, 0x14, 0x0f, 0x17, 0x01, 0x2e, 0x00};
//     "pl": "Zdzisiek nie jest popularny we wsi. Ludziska pamiętają, że jego ojciec był folks-   dojczem. Zdzisia wyróżnia   inteligencja po tacie."
const char TXT_PL_CHAR_2_STORY[] = {0x5a, 0x04, 0x1a, 0x09, 0x13, 0x09, 0x05, 0x0b, 0x20, 0x0e, 0x09, 0x05, 0x20, 0x0a, 0x05, 0x13, 0x14, 0x20, 0x10, 0x0f, 0x10, 0x15, 0x0c, 0x01, 0x12, 0x0e, 0x19, 0x20, 0x17, 0x05, 0x20, 0x17, 0x13, 0x09, 0x2e, 0x20, 0x4c, 0x15, 0x04, 0x1a, 0x09, 0x13, 0x0b, 0x01, 0x20, 0x10, 0x01, 0x0d, 0x09, 0x5d, 0x14, 0x01, 0x0a, 0x5b, 0x2c, 0x20, 0x63, 0x05, 0x20, 0x0a, 0x05, 0x07, 0x0f, 0x20, 0x0f, 0x0a, 0x03, 0x09, 0x05, 0x03, 0x20, 0x02, 0x19, 0x5e, 0x20, 0x06, 0x0f, 0x0c, 0x0b, 0x13, 0x2d, 0x20, 0x20, 0x20, 0x04, 0x0f, 0x0a, 0x03, 0x1a, 0x05, 0x0d, 0x2e, 0x20, 0x5a, 0x04, 0x1a, 0x09, 0x13, 0x09, 0x01, 0x20, 0x17, 0x19, 0x12, 0x60, 0x63, 0x0e, 0x09, 0x01, 0x20, 0x20, 0x20, 0x09, 0x0e, 0x14, 0x05, 0x0c, 0x09, 0x07, 0x05, 0x0e, 0x03, 0x0a, 0x01, 0x20, 0x10, 0x0f, 0x20, 0x14, 0x01, 0x03, 0x09, 0x05, 0x2e, 0x00};
//     "pl": "Tutaj będzie historia       Wieśka"
const char TXT_PL_CHAR_3_STORY[] = {0x54, 0x15, 0x14, 0x01, 0x0a, 0x20, 0x02, 0x5d, 0x04, 0x1a, 0x09, 0x05, 0x20, 0x08, 0x09, 0x13, 0x14, 0x0f, 0x12, 0x09, 0x01, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x57, 0x09, 0x05, 0x61, 0x0b, 0x01, 0x00};
//     "pl": "Józek jest wujkiem Janusza. Tak właściwie to jego       gospodarstwo. Janusz musi   się wykazać, to może je     odziedziczy."
const char TXT_PL_CHAR_4_STORY[] = {0x4a, 0x60, 0x1a, 0x05, 0x0b, 0x20, 0x0a, 0x05, 0x13, 0x14, 0x20, 0x17, 0x15, 0x0a, 0x0b, 0x09, 0x05, 0x0d, 0x20, 0x4a, 0x01, 0x0e, 0x15, 0x13, 0x1a, 0x01, 0x2e, 0x20, 0x54, 0x01, 0x0b, 0x20, 0x17, 0x5e, 0x01, 0x61, 0x03, 0x09, 0x17, 0x09, 0x05, 0x20, 0x14, 0x0f, 0x20, 0x0a, 0x05, 0x07, 0x0f, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x07, 0x0f, 0x13, 0x10, 0x0f, 0x04, 0x01, 0x12, 0x13, 0x14, 0x17, 0x0f, 0x2e, 0x20, 0x4a, 0x01, 0x0e, 0x15, 0x13, 0x1a, 0x20, 0x0d, 0x15, 0x13, 0x09, 0x20, 0x20, 0x20, 0x13, 0x09, 0x5d, 0x20, 0x17, 0x19, 0x0b, 0x01, 0x1a, 0x01, 0x5c, 0x2c, 0x20, 0x14, 0x0f, 0x20, 0x0d, 0x0f, 0x63, 0x05, 0x20, 0x0a, 0x05, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0f, 0x04, 0x1a, 0x09, 0x05, 0x04, 0x1a, 0x09, 0x03, 0x1a, 0x19, 0x2e, 0x00};
//     "pl": " ûý Zaoraj pole "
const char TXT_PL_MENU_FARMLAND_PLOW[] = {0x20, 0x7b, 0x7d, 0x20, 0x5a, 0x01, 0x0f, 0x12, 0x01, 0x0a, 0x20, 0x10, 0x0f, 0x0c, 0x05, 0x20, 0x00};
//     "pl": "ûý Uprawiaj pole"
const char TXT_PL_MENU_FARMLAND_SOW[] = {0x7b, 0x7d, 0x20, 0x55, 0x10, 0x12, 0x01, 0x17, 0x09, 0x01, 0x0a, 0x20, 0x10, 0x0f, 0x0c, 0x05, 0x00};
//     "pl": "    ûý Zbiory   "
const char TXT_PL_MENU_FARMLAND_REAP[] = {0x20, 0x20, 0x20, 0x20, 0x7b, 0x7d, 0x20, 0x5a, 0x02, 0x09, 0x0f, 0x12, 0x19, 0x20, 0x20, 0x20, 0x00};
//     "pl": "Przygotowuje pole   pod uprawę    "
const char TXT_PL_DESC_FARMLAND_PLOW[] = {0x50, 0x12, 0x1a, 0x19, 0x07, 0x0f, 0x14, 0x0f, 0x17, 0x15, 0x0a, 0x05, 0x20, 0x10, 0x0f, 0x0c, 0x05, 0x20, 0x20, 0x20, 0x10, 0x0f, 0x04, 0x20, 0x15, 0x10, 0x12, 0x01, 0x17, 0x5d, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": " Uprawia wybraną      roślinę     "
const char TXT_PL_DESC_FARMLAND_SOW[] = {0x20, 0x55, 0x10, 0x12, 0x01, 0x17, 0x09, 0x01, 0x20, 0x17, 0x19, 0x02, 0x12, 0x01, 0x0e, 0x5b, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x12, 0x0f, 0x61, 0x0c, 0x09, 0x0e, 0x5d, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "   Zbierz plony                   "
const char TXT_PL_DESC_FARMLAND_REAP[] = {0x20, 0x20, 0x20, 0x5a, 0x02, 0x09, 0x05, 0x12, 0x1a, 0x20, 0x10, 0x0c, 0x0f, 0x0e, 0x19, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "Orka    "
const char TXT_PL_TASK_FARMLAND_PLOW[] = {0x4f, 0x12, 0x0b, 0x01, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "Uprawa  "
const char TXT_PL_TASK_FARMLAND_FARM[] = {0x55, 0x10, 0x12, 0x01, 0x17, 0x01, 0x20, 0x20, 0x00};
//     "pl": "Zbiory  "
const char TXT_PL_TASK_FARMLAND_REAP[] = {0x5a, 0x02, 0x09, 0x0f, 0x12, 0x19, 0x20, 0x20, 0x00};
//     "pl": "Pole"
const char TXT_PL_TASK_DSC_FARMLAND_FIELD[] = {0x50, 0x0f, 0x0c, 0x05, 0x00};
//     "pl": "ziemniaki"
const char TXT_PL_TASK_DSC_FARMLAND_POTATO[] = {0x1a, 0x09, 0x05, 0x0d, 0x0e, 0x09, 0x01, 0x0b, 0x09, 0x00};
//     "pl": "łubin"
const char TXT_PL_TASK_DSC_FARMLAND_LUPINE[] = {0x5e, 0x15, 0x02, 0x09, 0x0e, 0x00};
//     "pl": "pszenica"
const char TXT_PL_TASK_DSC_FARMLAND_WHEAT[] = {0x10, 0x13, 0x1a, 0x05, 0x0e, 0x09, 0x03, 0x01, 0x00};
//     "pl": "kukurydza"
const char TXT_PL_TASK_DSC_FARMLAND_CORN[] = {0x0b, 0x15, 0x0b, 0x15, 0x12, 0x19, 0x04, 0x1a, 0x01, 0x00};
//     "pl": "Stonka (Uwaga: Bez powrotu do gry!)"
const char TXT_PL_MENU_AIR_DEF[] = {0x71, 0x53, 0x14, 0x0f, 0x0e, 0x0b, 0x01, 0x20, 0x28, 0x55, 0x17, 0x01, 0x07, 0x01, 0x3a, 0x20, 0x42, 0x05, 0x1a, 0x20, 0x10, 0x0f, 0x17, 0x12, 0x0f, 0x14, 0x15, 0x20, 0x04, 0x0f, 0x20, 0x07, 0x12, 0x19, 0x21, 0x29, 0x00};
//     "pl": "Ratuj kartofle!"
const char TXT_PL_SAVE_POTATOES[] = {0x52, 0x01, 0x14, 0x15, 0x0a, 0x20, 0x0b, 0x01, 0x12, 0x14, 0x0f, 0x06, 0x0c, 0x05, 0x21, 0x00};
//     "pl": "Zadanie |       Opis        "
const char TXT_PL_TASK_LIST_HEADER[] = {0xda, 0x81, 0x84, 0x81, 0x8e, 0x89, 0x85, 0xa0, 0xfe, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xcf, 0x90, 0x89, 0x93, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0x00};
//     "pl": "Kuchnia"
const char TXT_PL_MENU_MAIN1[] = {0x71, 0x4b, 0x15, 0x03, 0x08, 0x0e, 0x09, 0x01, 0x00};
//     "pl": "Załoga"
const char TXT_PL_MENU_MAIN2[] = {0x72, 0x5a, 0x01, 0x5e, 0x0f, 0x07, 0x01, 0x00};
//     "pl": "Wiadomości"
const char TXT_PL_MENU_MAIN3[] = {0x73, 0x57, 0x09, 0x01, 0x04, 0x0f, 0x0d, 0x0f, 0x61, 0x03, 0x09, 0x00};
//     "pl": "Upiecz chleb"
const char TXT_PL_MENU_KITCHEN1[] = {0x71, 0x55, 0x10, 0x09, 0x05, 0x03, 0x1a, 0x20, 0x03, 0x08, 0x0c, 0x05, 0x02, 0x00};
//     "pl": "Piecz codziennie"
const char TXT_PL_MENU_KITCHEN2[] = {0x72, 0x50, 0x09, 0x05, 0x03, 0x1a, 0x20, 0x03, 0x0f, 0x04, 0x1a, 0x09, 0x05, 0x0e, 0x0e, 0x09, 0x05, 0x00};
//     "pl": "Na śniadanie"
const char TXT_PL_MENU_KITCHEN3[] = {0x73, 0x4e, 0x01, 0x20, 0x61, 0x0e, 0x09, 0x01, 0x04, 0x01, 0x0e, 0x09, 0x05, 0x00};
//     "pl": "Na kolację jedz"
const char TXT_PL_MENU_KITCHEN4[] = {0x74, 0x4e, 0x01, 0x20, 0x0b, 0x0f, 0x0c, 0x01, 0x03, 0x0a, 0x5d, 0x20, 0x0a, 0x05, 0x04, 0x1a, 0x00};
//     "pl": "Preferuj żarcie"
const char TXT_PL_MENU_KITCHEN5[] = {0x75, 0x50, 0x12, 0x05, 0x06, 0x05, 0x12, 0x15, 0x0a, 0x20, 0x63, 0x01, 0x12, 0x03, 0x09, 0x05, 0x00};
//     "pl": "Ulubione warzywo"
const char TXT_PL_MENU_KITCHEN6[] = {0x76, 0x55, 0x0c, 0x15, 0x02, 0x09, 0x0f, 0x0e, 0x05, 0x20, 0x17, 0x01, 0x12, 0x1a, 0x19, 0x17, 0x0f, 0x00};
//     "pl": "Zjedz teraz drugie śniadanie"
const char TXT_PL_MENU_KITCHEN7[] = {0x77, 0x5a, 0x0a, 0x05, 0x04, 0x1a, 0x20, 0x14, 0x05, 0x12, 0x01, 0x1a, 0x20, 0x04, 0x12, 0x15, 0x07, 0x09, 0x05, 0x20, 0x61, 0x0e, 0x09, 0x01, 0x04, 0x01, 0x0e, 0x09, 0x05, 0x00};
//     "pl": "   Jesteś   "
//           "  najedzony "
//           "            "
const char SB_PL_KITCHEN_CANNOT_EAT[] = {0x20, 0x20, 0x20, 0x4a, 0x05, 0x13, 0x14, 0x05, 0x61, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0e, 0x01, 0x0a, 0x05, 0x04, 0x1a, 0x0f, 0x0e, 0x19, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "Brak miejsca"
//           "      w     "
//           "  spiżarni  "
const char SB_PL_TASK_KITCHEN_BAKE_BREAD_STORAGE_FULL[] = {0x42, 0x12, 0x01, 0x0b, 0x20, 0x0d, 0x09, 0x05, 0x0a, 0x13, 0x03, 0x01, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x17, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x13, 0x10, 0x09, 0x63, 0x01, 0x12, 0x0e, 0x09, 0x20, 0x20, 0x00};
//     "pl": "    Brak    "
//           "  pszenicy  "
//           "  na chleb  "
const char SB_PL_TASK_KITCHEN_BAKE_BREAD_NO_WHEAT[] = {0x20, 0x20, 0x20, 0x20, 0x42, 0x12, 0x01, 0x0b, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x13, 0x1a, 0x05, 0x0e, 0x09, 0x03, 0x19, 0x20, 0x20, 0x20, 0x20, 0x0e, 0x01, 0x20, 0x03, 0x08, 0x0c, 0x05, 0x02, 0x20, 0x20, 0x00};
//     "pl": "Kuchnia "
const char TXT_PL_TASK_KITCHEN[] = {0x4b, 0x15, 0x03, 0x08, 0x0e, 0x09, 0x01, 0x20, 0x00};
//     "pl": "Upiecz chleb"
const char TXT_PL_TASK_DSC_KITCHEN_BAKE_BREAD[] = {0x55, 0x10, 0x09, 0x05, 0x03, 0x1a, 0x20, 0x03, 0x08, 0x0c, 0x05, 0x02, 0x00};
//     "pl": "Nie                  "
const char TXT_PL_KITCHEN_TASK_OFF[] = {0x4e, 0x09, 0x05, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "Tak                  "
const char TXT_PL_KITCHEN_TASK_ON[] = {0x54, 0x01, 0x0b, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "Chleb                "
const char TXT_PL_KITCHEN_EATING_STYLE_LIGHT[] = {0x43, 0x08, 0x0c, 0x05, 0x02, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "Chleb, mięso         "
const char TXT_PL_KITCHEN_EATING_STYLE_MEDIUM[] = {0x43, 0x08, 0x0c, 0x05, 0x02, 0x2c, 0x20, 0x0d, 0x09, 0x5d, 0x13, 0x0f, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "Chleb, mięso, warzywa"
const char TXT_PL_KITCHEN_EATING_STYLE_FULL[] = {0x43, 0x08, 0x0c, 0x05, 0x02, 0x2c, 0x20, 0x0d, 0x09, 0x5d, 0x13, 0x0f, 0x2c, 0x20, 0x17, 0x01, 0x12, 0x1a, 0x19, 0x17, 0x01, 0x00};
//     "pl": "Sklepowe             "
const char TXT_PL_KITCHEN_PREFER_SHOP[] = {0x53, 0x0b, 0x0c, 0x05, 0x10, 0x0f, 0x17, 0x05, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "Domowe               "
const char TXT_PL_KITCHEN_PREFER_HOME[] = {0x44, 0x0f, 0x0d, 0x0f, 0x17, 0x05, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "Ziemniak             "
const char TXT_PL_KITCHEN_PREFER_POTATO[] = {0x5a, 0x09, 0x05, 0x0d, 0x0e, 0x09, 0x01, 0x0b, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "Kukurydza            "
const char TXT_PL_KITCHEN_PREFER_CORN[] = {0x4b, 0x15, 0x0b, 0x15, 0x12, 0x19, 0x04, 0x1a, 0x01, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "Brak:       "
const char TXT_PL_KITCHEN_MISSING[] = {0x42, 0x12, 0x01, 0x0b, 0x3a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "chleba"
const char TXT_PL_KITCHEN_MISSING_BREAD[] = {0x03, 0x08, 0x0c, 0x05, 0x02, 0x01, 0x00};
//     "pl": "mięsa"
const char TXT_PL_KITCHEN_MISSING_MEAT[] = {0x0d, 0x09, 0x5d, 0x13, 0x01, 0x00};
//     "pl": "warzyw"
const char TXT_PL_KITCHEN_MISSING_VEG[] = {0x17, 0x01, 0x12, 0x1a, 0x19, 0x17, 0x00};
//     "pl": "Karmienie"
const char TXT_PL_MENU_PIGPEN1[] = {0x71, 0x4b, 0x01, 0x12, 0x0d, 0x09, 0x05, 0x0e, 0x09, 0x05, 0x00};
//     "pl": "Sprzątanie"
const char TXT_PL_MENU_PIGPEN2[] = {0x72, 0x53, 0x10, 0x12, 0x1a, 0x5b, 0x14, 0x01, 0x0e, 0x09, 0x05, 0x00};
//     "pl": "Przebudowa"
const char TXT_PL_MENU_PIGPEN3[] = {0x73, 0x50, 0x12, 0x1a, 0x05, 0x02, 0x15, 0x04, 0x0f, 0x17, 0x01, 0x00};
//     "pl": "Manager priorytetów zadań"
const char TXT_PL_TASK_MANAGER_PRIO_HEADER[] = {0x4d, 0x01, 0x0e, 0x01, 0x07, 0x05, 0x12, 0x20, 0x10, 0x12, 0x09, 0x0f, 0x12, 0x19, 0x14, 0x05, 0x14, 0x60, 0x17, 0x20, 0x1a, 0x01, 0x04, 0x01, 0x5f, 0x00};
//     "pl": "                     |Zdolność| Zadanie"
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_1[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xfe, 0xda, 0x84, 0x8f, 0x8c, 0x8e, 0x8f, 0xe1, 0xdc, 0xfe, 0xa0, 0xda, 0x81, 0x84, 0x81, 0x8e, 0x89, 0x85, 0x00};
//     "pl": "                     | |R|G|  |  |R|G| "
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_2[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7e, 0x20, 0x7e, 0x52, 0x7e, 0x47, 0x7e, 0x20, 0x20, 0x7e, 0x20, 0x20, 0x7e, 0x52, 0x7e, 0x47, 0x7e, 0x20, 0x00};
//     "pl": "                     | |o|o|  |  |o|o| "
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_3[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7e, 0x20, 0x7e, 0x0f, 0x7e, 0x0f, 0x7e, 0x20, 0x20, 0x7e, 0x20, 0x20, 0x7e, 0x0f, 0x7e, 0x0f, 0x7e, 0x20, 0x00};
//     "pl": "                     |H|l|t|  | H|l|t| "
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_4[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7e, 0x48, 0x7e, 0x0c, 0x7e, 0x14, 0x7e, 0x20, 0x20, 0x7e, 0x20, 0x48, 0x7e, 0x0c, 0x7e, 0x14, 0x7e, 0x20, 0x00};
//     "pl": "                     |o|n|o|H | o|n|o|H"
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_5[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7e, 0x0f, 0x7e, 0x0e, 0x7e, 0x0f, 0x7e, 0x48, 0x20, 0x7e, 0x20, 0x0f, 0x7e, 0x0e, 0x7e, 0x0f, 0x7e, 0x48, 0x00};
//     "pl": "                     |d|i|w|a | d|i|w|a"
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_6[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7e, 0x04, 0x7e, 0x09, 0x7e, 0x17, 0x7e, 0x01, 0x20, 0x7e, 0x20, 0x04, 0x7e, 0x09, 0x7e, 0x17, 0x7e, 0x01, 0x00};
//     "pl": "                     |o|c|a|n | o|c|a|n"
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_7[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7e, 0x0f, 0x7e, 0x03, 0x7e, 0x01, 0x7e, 0x0e, 0x20, 0x7e, 0x20, 0x0f, 0x7e, 0x03, 0x7e, 0x01, 0x7e, 0x0e, 0x00};
//     "pl": "                     |w|t|n|d | w|t|n|d"
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_8[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7e, 0x17, 0x7e, 0x14, 0x7e, 0x0e, 0x7e, 0x04, 0x20, 0x7e, 0x20, 0x17, 0x7e, 0x14, 0x7e, 0x0e, 0x7e, 0x04, 0x00};
//     "pl": "                     |l|w|i|e | l|w|i|e"
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_9[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7e, 0x0c, 0x7e, 0x17, 0x7e, 0x09, 0x7e, 0x05, 0x20, 0x7e, 0x20, 0x0c, 0x7e, 0x17, 0x7e, 0x09, 0x7e, 0x05, 0x00};
//     "pl": "Postać               |a|o|e|l | a|o|e|l"
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_10[] = {0xd0, 0x8f, 0x93, 0x94, 0x81, 0xdc, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xfe, 0x81, 0xfe, 0x8f, 0xfe, 0x85, 0xfe, 0x8c, 0xa0, 0xfe, 0xa0, 0x81, 0xfe, 0x8f, 0xfe, 0x85, 0xfe, 0x8c, 0x00};
//     "pl": "+"
const char TXT_PL_MENU_TASK_MANAGER_PLUS[] = {0x2b, 0x00};
//     "pl": "-"
const char TXT_PL_MENU_TASK_MANAGER_MINUS[] = {0x2d, 0x00};
//     "pl": "Kup ogóra"
const char TXT_PL_MENU_SHOPIN1[] = {0x71, 0x4b, 0x15, 0x10, 0x20, 0x0f, 0x07, 0x60, 0x12, 0x01, 0x00};
//     "pl": "Rozmowa"
const char TXT_PL_MENU_SHOPIN2[] = {0x72, 0x52, 0x0f, 0x1a, 0x0d, 0x0f, 0x17, 0x01, 0x00};
//     "pl": "Kup chleb"
const char TXT_PL_MENU_SHOPIN3[] = {0x73, 0x4b, 0x15, 0x10, 0x20, 0x03, 0x08, 0x0c, 0x05, 0x02, 0x00};
//     "pl": "Kup Turystyczną"
const char TXT_PL_MENU_SHOPIN4[] = {0x74, 0x4b, 0x15, 0x10, 0x20, 0x54, 0x15, 0x12, 0x19, 0x13, 0x14, 0x19, 0x03, 0x1a, 0x0e, 0x5b, 0x00};
//     "pl": " Sklep jest "
//           " zamknięty  "
//           "            "
const char SB_PL_SHOP_CLOSED[] = {0x20, 0x53, 0x0b, 0x0c, 0x05, 0x10, 0x20, 0x0a, 0x05, 0x13, 0x14, 0x20, 0x20, 0x1a, 0x01, 0x0d, 0x0b, 0x0e, 0x09, 0x5d, 0x14, 0x19, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "Sklep wielobranżowy 'GS', otwarty 7-11 i15-19, codziennie."
const char TXT_PL_SHOP_DESCRIPTION[] = {0x53, 0x0b, 0x0c, 0x05, 0x10, 0x20, 0x17, 0x09, 0x05, 0x0c, 0x0f, 0x02, 0x12, 0x01, 0x0e, 0x63, 0x0f, 0x17, 0x19, 0x20, 0x27, 0x47, 0x53, 0x27, 0x2c, 0x20, 0x0f, 0x14, 0x17, 0x01, 0x12, 0x14, 0x19, 0x20, 0x37, 0x2d, 0x31, 0x31, 0x20, 0x09, 0x31, 0x35, 0x2d, 0x31, 0x39, 0x2c, 0x20, 0x03, 0x0f, 0x04, 0x1a, 0x09, 0x05, 0x0e, 0x0e, 0x09, 0x05, 0x2e, 0x00};
//     "pl": "Wejdź"
const char TXT_PL_MENU_SHOPOUT1[] = {0x71, 0x57, 0x05, 0x0a, 0x04, 0x62, 0x00};
//     "pl": "Rozmowa"
const char TXT_PL_MENU_SHOPOUT2[] = {0x72, 0x52, 0x0f, 0x1a, 0x0d, 0x0f, 0x17, 0x01, 0x00};
//     "pl": " Wiadomości "
//           "            "
//           "  ½ü Wyjdź  "
const char SB_PL_MENU_TV_SCREEN[] = {0x20, 0x57, 0x09, 0x01, 0x04, 0x0f, 0x0d, 0x0f, 0x61, 0x03, 0x09, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x1f, 0x7c, 0x20, 0x57, 0x19, 0x0a, 0x04, 0x62, 0x20, 0x20, 0x00};
//     "pl": "  Budżet "
//           "         "
//           "Rachunki:"
//           "         "
//           "Renta:   "
//           "         "
const char TV_PL_MENU_TV_SCREEN_BILLS[] = {0x20, 0x20, 0x42, 0x15, 0x04, 0x63, 0x05, 0x14, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x52, 0x01, 0x03, 0x08, 0x15, 0x0e, 0x0b, 0x09, 0x3a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x52, 0x05, 0x0e, 0x14, 0x01, 0x3a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};



//     "pragma_label": FieldSizes,
//     "array_label": FIELD_SIZES,


//     "pl": "Małe   "
const char TXT_PL_FIELD_SMALL[] = {0x4d, 0x01, 0x5e, 0x05, 0x20, 0x20, 0x20, 0x00};
//     "pl": "Średnie"
const char TXT_PL_FIELD_MEDIUM[] = {0x6a, 0x12, 0x05, 0x04, 0x0e, 0x09, 0x05, 0x00};
//     "pl": "Duże   "
const char TXT_PL_FIELD_BIG[] = {0x44, 0x15, 0x63, 0x05, 0x20, 0x20, 0x20, 0x00};
//     "pl": "Wielkie"
const char TXT_PL_FIELD_VBIG[] = {0x57, 0x09, 0x05, 0x0c, 0x0b, 0x09, 0x05, 0x00};



//     "pragma_label": LocationNames,
//     "array_label": LOCATION_NAMES,


//     "pl": "                        "
const char TXT_PL_NOTHING[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "     Dom          ý     "
const char TXT_PL_FARM_HOUSE[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x44, 0x0f, 0x0d, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7d, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "   Stodoła        ý     "
const char TXT_PL_FARM_BARN[] = {0x20, 0x20, 0x20, 0x53, 0x14, 0x0f, 0x04, 0x0f, 0x5e, 0x01, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7d, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "   Chlewik        ý     "
const char TXT_PL_FARM_PIGPEN[] = {0x20, 0x20, 0x20, 0x43, 0x08, 0x0c, 0x05, 0x17, 0x09, 0x0b, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7d, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": " Ściernisko             "
const char TXT_PL_FARM_EMPTY[] = {0x20, 0x6a, 0x03, 0x09, 0x05, 0x12, 0x0e, 0x09, 0x13, 0x0b, 0x0f, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "    Garaż               "
const char TXT_PL_FARM_GARAGE[] = {0x20, 0x20, 0x20, 0x20, 0x47, 0x01, 0x12, 0x01, 0x63, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "Wielkie pole            "
const char TXT_PL_FARM_VBIG_FIELD[] = {0x57, 0x09, 0x05, 0x0c, 0x0b, 0x09, 0x05, 0x20, 0x10, 0x0f, 0x0c, 0x05, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "  Małe pole             "
const char TXT_PL_FARM_SMALL_FIELD_1[] = {0x20, 0x20, 0x4d, 0x01, 0x5e, 0x05, 0x20, 0x10, 0x0f, 0x0c, 0x05, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "  Małe pole             "
const char TXT_PL_FARM_SMALL_FIELD_2[] = {0x20, 0x20, 0x4d, 0x01, 0x5e, 0x05, 0x20, 0x10, 0x0f, 0x0c, 0x05, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "Średnie pole            "
const char TXT_PL_FARM_MEDIUM_FIELD[] = {0x6a, 0x12, 0x05, 0x04, 0x0e, 0x09, 0x05, 0x20, 0x10, 0x0f, 0x0c, 0x05, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "    Sklep         ý     "
const char TXT_PL_SHOP[] = {0x20, 0x20, 0x20, 0x20, 0x53, 0x0b, 0x0c, 0x05, 0x10, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7d, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "   Kościół              "
const char TXT_PL_CHURCH[] = {0x20, 0x20, 0x20, 0x4b, 0x0f, 0x61, 0x03, 0x09, 0x60, 0x5e, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": " Dom Hermana      ý     "
const char TXT_PL_HERMAN[] = {0x20, 0x44, 0x0f, 0x0d, 0x20, 0x48, 0x05, 0x12, 0x0d, 0x01, 0x0e, 0x01, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7d, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": " Sulejukowa             "
const char TXT_PL_SULEJUKOWA[] = {0x20, 0x53, 0x15, 0x0c, 0x05, 0x0a, 0x15, 0x0b, 0x0f, 0x17, 0x01, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": " Wiejski dom            "
const char TXT_PL_HOUSE_1[] = {0x20, 0x57, 0x09, 0x05, 0x0a, 0x13, 0x0b, 0x09, 0x20, 0x04, 0x0f, 0x0d, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": " Wiejski dom            "
const char TXT_PL_HOUSE_2[] = {0x20, 0x57, 0x09, 0x05, 0x0a, 0x13, 0x0b, 0x09, 0x20, 0x04, 0x0f, 0x0d, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "    Szosa         û     "
const char TXT_PL_MAIN_ROAD[] = {0x20, 0x20, 0x20, 0x20, 0x53, 0x1a, 0x0f, 0x13, 0x01, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7b, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "   Droga         ûû     "
const char TXT_PL_VILLIAGE_ROAD[] = {0x20, 0x20, 0x20, 0x44, 0x12, 0x0f, 0x07, 0x01, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7b, 0x7b, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": " Polna droga     ûûû    "
const char TXT_PL_DIRT_ROAD[] = {0x20, 0x50, 0x0f, 0x0c, 0x0e, 0x01, 0x20, 0x04, 0x12, 0x0f, 0x07, 0x01, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7b, 0x7b, 0x7b, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "   Studnia       úú     "
const char TXT_PL_WELL[] = {0x20, 0x20, 0x20, 0x53, 0x14, 0x15, 0x04, 0x0e, 0x09, 0x01, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7a, 0x7a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "  Sławojka              "
const char TXT_PL_TOILET[] = {0x20, 0x20, 0x53, 0x5e, 0x01, 0x17, 0x0f, 0x0a, 0x0b, 0x01, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "   Pomost               "
const char TXT_PL_WHARF[] = {0x20, 0x20, 0x20, 0x50, 0x0f, 0x0d, 0x0f, 0x13, 0x14, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};



//     "pragma_label": PlantStageNames,
//     "array_label": PLANT_STAGE_NAMES,


//     "pl": "        "
const char TXT_PL_PLANT_STAGE_NONE[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "Z: Orka "
const char TXT_PL_PLANT_STAGE_PLOW_TASK_ASSIGNED[] = {0x5a, 0x3a, 0x20, 0x4f, 0x12, 0x0b, 0x01, 0x20, 0x00};
//     "pl": "Zaorane "
const char TXT_PL_PLANT_STAGE_PLOWED[] = {0x5a, 0x01, 0x0f, 0x12, 0x01, 0x0e, 0x05, 0x20, 0x00};
//     "pl": "Z: Siew "
const char TXT_PL_PLANT_STAGE_SOW_TASK_ASSIGNED[] = {0x5a, 0x3a, 0x20, 0x53, 0x09, 0x05, 0x17, 0x20, 0x00};
//     "pl": "Kiełkuje"
const char TXT_PL_PLANT_STAGE_SPROUT[] = {0x4b, 0x09, 0x05, 0x5e, 0x0b, 0x15, 0x0a, 0x05, 0x00};
//     "pl": "Rośnie  "
const char TXT_PL_PLANT_STAGE_GROWTH[] = {0x52, 0x0f, 0x61, 0x0e, 0x09, 0x05, 0x20, 0x20, 0x00};
//     "pl": "Dojrzewa"
const char TXT_PL_PLANT_STAGE_RIPEN[] = {0x44, 0x0f, 0x0a, 0x12, 0x1a, 0x05, 0x17, 0x01, 0x00};
//     "pl": "Dojrzałe"
const char TXT_PL_PLANT_STAGE_READY[] = {0x44, 0x0f, 0x0a, 0x12, 0x1a, 0x01, 0x5e, 0x05, 0x00};
//     "pl": "Z: Zbiór"
const char TXT_PL_PLANT_STAGE_REAP_TASK_ASSIGNED[] = {0x5a, 0x3a, 0x20, 0x5a, 0x02, 0x09, 0x60, 0x12, 0x00};



//     "pragma_label": PlantTypes,
//     "array_label": PLANT_TYPES_TXT,


//     "pl": "Puste     "
const char TXT_PL_PLANT_NAME_NONE[] = {0x50, 0x15, 0x13, 0x14, 0x05, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "Ziemniaki "
const char TXT_PL_PLANT_NAME_POTATO[] = {0x5a, 0x09, 0x05, 0x0d, 0x0e, 0x09, 0x01, 0x0b, 0x09, 0x20, 0x00};
//     "pl": "Łubin     "
const char TXT_PL_PLANT_NAME_LUPINE[] = {0x67, 0x15, 0x02, 0x09, 0x0e, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "pl": "Pszenica  "
const char TXT_PL_PLANT_NAME_WHEAT[] = {0x50, 0x13, 0x1a, 0x05, 0x0e, 0x09, 0x03, 0x01, 0x20, 0x20, 0x00};
//     "pl": "Kukurydza "
const char TXT_PL_PLANT_NAME_CORN[] = {0x4b, 0x15, 0x0b, 0x15, 0x12, 0x19, 0x04, 0x1a, 0x01, 0x20, 0x00};





//     "pragma_label": MainArray,
//     "array_label": TXT,

#pragma data ( txtPlMainArray )
__export const char* TXT[] = {
    TXT_PL_MENU_EXIT,
    TXT_PL_EXIT_TO_MAP,
    TXT_PL_MENU_OPTIONS_LANG,
    TXT_PL_MENU_OPTIONS_MSX,
    TXT_PL_OPTIONS_HEADER,
    TXT_PL_OPTIONS_HELP1,
    TXT_PL_OPTIONS_HELP2,
    TXT_PL_OPTIONS_HELP3,
    TXT_PL_OPTIONS_HELP4,
    TXT_PL_OPTIONS_HELP5,
    TXT_PL_MUSIC_OPTIONS_HEADER,
    SB_PL_MENU_OPTIONS_MSX_MENU,
    TXT_PL_LOG_MENU_HEADER,
    TXT_PL_LOG_MENU_LOG_TABLE_HEADER,
    TXT_PL_MENU_SELECT,
    TXT_PL_MENU_W,
    TXT_PL_MENU_S,
    TXT_PL_MENU_A,
    TXT_PL_MENU_D,
    TXT_PL_TASK_MANAGER_HEADER,
    TXT_PL_TASK_MANAGER_TABLE_HEADER,
    TXT_PL_FIELD_LIST_HEADER,
    TXT_PL_FARM_FIELD_TXT,
    TXT_PL_FARM_PLANT_TXT,
    TXT_PL_STAT_INT,
    TXT_PL_STAT_STR,
    TXT_PL_STAT_CUN,
    TXT_PL_SKILL_ANI,
    TXT_PL_SKILL_FRM,
    TXT_PL_SKILL_BTH,
    TXT_PL_SKILL_TRD,
    SB_PL_TASK_ADDED,
    SB_PL_TASKS_FULL,
    SB_PL_WELCOME,
    TXT_PL_CONFIRM_TXT,
    TXT_PL_CONFIRM_ERROR,
    TXT_PL_CONFIRM_YES,
    TXT_PL_CONFIRM_NO,
    TXT_PL_MENU_CREW1,
    TXT_PL_MENU_CREW2,
    TXT_PL_MENU_CREW3,
    TXT_PL_MENU_CREW4,
    TXT_CHAR_1_NAME,
    TXT_PL_CHAR_1_STORY,
    TXT_CHAR_2_NAME,
    TXT_PL_CHAR_2_STORY,
    TXT_CHAR_3_NAME,
    TXT_PL_CHAR_3_STORY,
    TXT_CHAR_4_NAME,
    TXT_PL_CHAR_4_STORY,
    TXT_PL_MENU_FARMLAND_PLOW,
    TXT_PL_MENU_FARMLAND_SOW,
    TXT_PL_MENU_FARMLAND_REAP,
    TXT_PL_DESC_FARMLAND_PLOW,
    TXT_PL_DESC_FARMLAND_SOW,
    TXT_PL_DESC_FARMLAND_REAP,
    TXT_PL_TASK_FARMLAND_PLOW,
    TXT_PL_TASK_FARMLAND_FARM,
    TXT_PL_TASK_FARMLAND_REAP,
    TXT_PL_TASK_DSC_FARMLAND_FIELD,
    TXT_PL_TASK_DSC_FARMLAND_POTATO,
    TXT_PL_TASK_DSC_FARMLAND_LUPINE,
    TXT_PL_TASK_DSC_FARMLAND_WHEAT,
    TXT_PL_TASK_DSC_FARMLAND_CORN,
    TXT_PL_MENU_AIR_DEF,
    TXT_PL_SAVE_POTATOES,
    TXT_PL_TASK_LIST_HEADER,
    TXT_PL_MENU_MAIN1,
    TXT_PL_MENU_MAIN2,
    TXT_PL_MENU_MAIN3,
    TXT_PL_MENU_KITCHEN1,
    TXT_PL_MENU_KITCHEN2,
    TXT_PL_MENU_KITCHEN3,
    TXT_PL_MENU_KITCHEN4,
    TXT_PL_MENU_KITCHEN5,
    TXT_PL_MENU_KITCHEN6,
    TXT_PL_MENU_KITCHEN7,
    SB_PL_KITCHEN_CANNOT_EAT,
    SB_PL_TASK_KITCHEN_BAKE_BREAD_STORAGE_FULL,
    SB_PL_TASK_KITCHEN_BAKE_BREAD_NO_WHEAT,
    TXT_PL_TASK_KITCHEN,
    TXT_PL_TASK_DSC_KITCHEN_BAKE_BREAD,
    TXT_PL_KITCHEN_TASK_OFF,
    TXT_PL_KITCHEN_TASK_ON,
    TXT_PL_KITCHEN_EATING_STYLE_LIGHT,
    TXT_PL_KITCHEN_EATING_STYLE_MEDIUM,
    TXT_PL_KITCHEN_EATING_STYLE_FULL,
    TXT_PL_KITCHEN_PREFER_SHOP,
    TXT_PL_KITCHEN_PREFER_HOME,
    TXT_PL_KITCHEN_PREFER_POTATO,
    TXT_PL_KITCHEN_PREFER_CORN,
    TXT_PL_KITCHEN_MISSING,
    TXT_PL_KITCHEN_MISSING_BREAD,
    TXT_PL_KITCHEN_MISSING_MEAT,
    TXT_PL_KITCHEN_MISSING_VEG,
    TXT_PL_MENU_PIGPEN1,
    TXT_PL_MENU_PIGPEN2,
    TXT_PL_MENU_PIGPEN3,
    TXT_PL_TASK_MANAGER_PRIO_HEADER,
    TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_1,
    TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_2,
    TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_3,
    TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_4,
    TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_5,
    TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_6,
    TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_7,
    TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_8,
    TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_9,
    TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_10,
    TXT_PL_MENU_TASK_MANAGER_PLUS,
    TXT_PL_MENU_TASK_MANAGER_MINUS,
    TXT_PL_MENU_SHOPIN1,
    TXT_PL_MENU_SHOPIN2,
    TXT_PL_MENU_SHOPIN3,
    TXT_PL_MENU_SHOPIN4,
    SB_PL_SHOP_CLOSED,
    TXT_PL_SHOP_DESCRIPTION,
    TXT_PL_MENU_SHOPOUT1,
    TXT_PL_MENU_SHOPOUT2,
    TXT_TASK_EMPTY_NAME,
    TXT_TASK_EMPTY_DESCRIPTION,
    SB_PL_MENU_TV_SCREEN,
    TV_PL_MENU_TV_SCREEN_BILLS
};


//     "pragma_label": FieldSizes,
//     "array_label": FIELD_SIZES,

#pragma data ( txtPlFieldSizes )
__export const char* FIELD_SIZES[] = {
    TXT_PL_FIELD_SMALL,
    TXT_PL_FIELD_MEDIUM,
    TXT_PL_FIELD_BIG,
    TXT_PL_FIELD_VBIG
};


//     "pragma_label": LocationNames,
//     "array_label": LOCATION_NAMES,

#pragma data ( txtPlLocationNames )
__export const char* LOCATION_NAMES[] = {
    TXT_PL_NOTHING,
    TXT_PL_FARM_HOUSE,
    TXT_PL_FARM_BARN,
    TXT_PL_FARM_PIGPEN,
    TXT_PL_FARM_EMPTY,
    TXT_PL_FARM_GARAGE,
    TXT_PL_FARM_VBIG_FIELD,
    TXT_PL_FARM_SMALL_FIELD_1,
    TXT_PL_FARM_SMALL_FIELD_2,
    TXT_PL_FARM_MEDIUM_FIELD,
    TXT_PL_SHOP,
    TXT_PL_CHURCH,
    TXT_PL_HERMAN,
    TXT_PL_SULEJUKOWA,
    TXT_PL_HOUSE_1,
    TXT_PL_HOUSE_2,
    TXT_PL_MAIN_ROAD,
    TXT_PL_VILLIAGE_ROAD,
    TXT_PL_DIRT_ROAD,
    TXT_PL_WELL,
    TXT_PL_TOILET,
    TXT_PL_WHARF
};


//     "pragma_label": PlantStageNames,
//     "array_label": PLANT_STAGE_NAMES,

#pragma data ( txtPlPlantStageNames )
__export const char* PLANT_STAGE_NAMES[] = {
    TXT_PL_PLANT_STAGE_NONE,
    TXT_PL_PLANT_STAGE_PLOW_TASK_ASSIGNED,
    TXT_PL_PLANT_STAGE_PLOWED,
    TXT_PL_PLANT_STAGE_SOW_TASK_ASSIGNED,
    TXT_PL_PLANT_STAGE_SPROUT,
    TXT_PL_PLANT_STAGE_GROWTH,
    TXT_PL_PLANT_STAGE_RIPEN,
    TXT_PL_PLANT_STAGE_READY,
    TXT_PL_PLANT_STAGE_REAP_TASK_ASSIGNED
};


//     "pragma_label": PlantTypes,
//     "array_label": PLANT_TYPES_TXT,

#pragma data ( txtPlPlantTypes )
__export const char* PLANT_TYPES_TXT[] = {
    TXT_PL_PLANT_NAME_NONE,
    TXT_PL_PLANT_NAME_POTATO,
    TXT_PL_PLANT_NAME_LUPINE,
    TXT_PL_PLANT_NAME_WHEAT,
    TXT_PL_PLANT_NAME_CORN
};

// =============================================================================
// Copy code
// =============================================================================
#pragma code ( code )
#pragma data ( data )

void loadTranslation(){
    memcpy(TRANSLATION_DST, TRANSLATION_SRC, 0x1000);
}
// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
