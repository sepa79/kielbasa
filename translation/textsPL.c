#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>

#pragma data ( txtPlTxtData )

// =============================================================================
// PLANT_STATE array (8)
// =============================================================================
const char TXT_PLANT_STATE_NAME_1[] = s" - - - -";
const char TXT_PLANT_STATE_NAME_2[] = s"Kie\x5ekuje";
const char TXT_PLANT_STATE_NAME_3[] = s"Ro\x61nie  ";
const char TXT_PLANT_STATE_NAME_4[] = s"Dojrzewa";
const char TXT_PLANT_STATE_NAME_5[] = s"Dojrza\x5ee";
const char TXT_PLANT_STATE_NAME_6[] = s"Zgni\x5ee  ";

// =============================================================================
// Main texts
// =============================================================================
const char TXT_MENU_CANCEL[] = s"\x1fAnuluj";
const char TXT_MENU_EXIT[] = s"\x1fPowr\x60t";
// fixed width - 28
const char TXT_TASK_LIST_HEADER[] = underline(s"Zadanie ") "\x7f" underline(s"       Opis        ");

const char TXT_MENU_MAIN1[] = underline(s"1") s"Pole";
const char TXT_MENU_MAIN2[] = underline(s"2") s"Chlewik";
const char TXT_MENU_MAIN3[] = underline(s"3") s"W\x5ddzarnia";
const char TXT_MENU_MAIN4[] = underline(s"4") s"Sklep";
const char TXT_MENU_MAIN5[] = underline(s"5") s"Za\x5eoga";

// =============================================================================
// Options
// =============================================================================
const char TXT_MENU_OPTIONS_LANG[] = s"\xb1J\x5dzyk / Language";
const char TXT_MENU_OPTIONS_MSX[] = s"\xb2Muzyka on/off";
const char TXT_OPTIONS_HEADER[] = s"Ustawienia gry";

// =============================================================================
// Farmland
// =============================================================================
// 4 options in Common
const char TXT_MENU_FARMLAND5[] = s"\xb1Uprawa";
const char TXT_MENU_FARMLAND6[] = s"\xb2Opieka";
const char TXT_MENU_FARMLAND7[] = s"\xb3Zbiory";
// fixed width 
const char TXT_FIELD_LIST_HEADER[] =
    underline(s"Mg")
    "\x7f"
    underline(s" Uprawa  ")
    "\x7f"
    underline(s"  Stan  ")
    "\x7f"
    underline(s"T  ")
    "\x7f"
    underline(s"S1 ")
    "\x7f"
    underline(s"S2 ")
    "\x7f"
    underline(s"S3 ")
;

// 7 "Field: "
const char TXT_FARM_FIELD_TXT[]      = s"Pole:  ";
// 8
const char TXT_FARM_PLANT_TXT[]      = s"Uprawa: ";
// plant names - 10
const char TXT_PLANT_NAME_NONE[]     = s" - - - - -";
const char TXT_PLANT_NAME_POTATO[]   = s"Zimnioki  ";
const char TXT_PLANT_NAME_LUPINE[]   = "\x67" s"ubin     ";
const char TXT_PLANT_NAME_WHEAT[]    = s"Pszenica  ";
const char TXT_PLANT_NAME_CORN[]     = s"Kukurydza ";

// Task names & descriptions (8)
const char TXT_TASK_FARMLAND_FARM[]  = s"Uprawa  ";
const char TXT_TASK_FARMLAND_CARE[]  = s"Opieka  ";
const char TXT_TASK_FARMLAND_REAP[]  = s"Zbiory  ";
const char TXT_TASK_DSC_FARMLAND_FIELD[]  = s"Pole"; // used in task description
const char TXT_TASK_DSC_FARMLAND_POTATO[] = s"ziemniaki"; // used in task description
const char TXT_TASK_DSC_FARMLAND_LUPINE[] = s"\x5eubin"; // used in task description
const char TXT_TASK_DSC_FARMLAND_WHEAT[]  = s"pszenica"; // used in task description
const char TXT_TASK_DSC_FARMLAND_CORN[]   = s"kukurydza"; // used in task description

// =============================================================================
// Pig pen
// =============================================================================
const char TXT_MENU_PIGPEN1[] = s"\xb1Karmienie";
const char TXT_MENU_PIGPEN2[] = s"\xb2Sprz\x5btanie";
const char TXT_MENU_PIGPEN3[] = s"\xb3Przebudowa";

const char TXT_MENU_SMOKEHOUSE1[] = s"\xb1Przepisy";
const char TXT_MENU_SMOKEHOUSE2[] = s"\xb2Sprz\x5btanie";
const char TXT_MENU_SMOKEHOUSE3[] = s"\xb3Przebudowa";

const char TXT_MENU_SHOPOUT1[] = s"\xb1Wejd\x62";
const char TXT_MENU_SHOPOUT2[] = s"\xb2Rozmowa";
const char TXT_SHOP_DESCRIPTION[] = s"Sklep wieobranzowy 'GS', otwarty 7-11 i 15-19, codziennie.";

const char TXT_MENU_SHOPIN1[] = s"\xb1Kup og\x60ra";
const char TXT_MENU_SHOPIN2[] = s"\xb2Rozmowa";

const char TXT_MENU_CREW1[] = s"\xb1Slot 1";
const char TXT_MENU_CREW2[] = s"\xb2Slot 2";
const char TXT_MENU_CREW3[] = s"\xb3Slot 3";
const char TXT_MENU_CREW4[] = s"\xb4Slot 4";

// =============================================================================
// Characters
// =============================================================================
const char TXT_STAT_INT[] = s"M\x5bdro""\x61\x5c";
const char TXT_STAT_STR[] = s"Si\x5ea";
const char TXT_STAT_CUN[] = s"Spryt";

const char TXT_SKILL_ANI[] = s"Hodowla";
const char TXT_SKILL_FRM[] = s"Rolnictwo";
const char TXT_SKILL_BTH[] = s"Masarstwo";
const char TXT_SKILL_TRD[] = s"Handel";

// names are in Common
const char TXT_CHAR_1_STORY[] = s"Janusz ca\x5ee \x63ycie pracowa\x5e\nw PGR. Na niczym si\x5d tak\nnaprawd\x5d nie zna, ale za to\nma na\x5eogi i jest leniwy.\nJanusz to posta\x5c startowa.";
const char TXT_CHAR_2_STORY[] = p"Zdzisiek nie jest popularny\nwe wsi. Ludziska pami\x5dtaj\x5b,\n\x63e jego ojciec by\x5e folks-\ndojczem. Zdzisia wyr\x60\x63nia\ninteligencja po tacie.";
const char TXT_CHAR_3_STORY[] = p"Historia Wie\x61ka M test M test.";
const char TXT_CHAR_4_STORY[] = p"Historia J\x60zka.";

// =============================================================================
// Status Bar
// =============================================================================
const char SB_PAUSE[]       = s"   P  A  U  Z  A   ";
const char SB_UNPAUSE[]     = s"   Normalna pr\x5ddko\x61\x5c gry   ";
const char SB_WELCOME[]     = s"    Witaj! +/-/Space - kontrola czasu    Kielbasa Empire - wersja bardzo alpha.    "
                              s"Credits:    "
                              s"    Wsparcie programistyczne i kompilatory: KickC - Rex/Camelot, Oscar64 - DrMortalWombat    "
                              s"    Gra: Kod - Sepa, Gfx - Juras, Model3 & Saturnz, Msx - Taurus, Support - Angry Andy, Fonts - Sepa    ";
const char SB_SHOP_CLOSED[] = s"   Sklep jest zamkniety   ";

// =============================================================================
// Arrays
// =============================================================================
#pragma data ( txtPlPlantTypes )
__export const char* PLANT_TYPES_TXT[] = {
    TXT_PLANT_NAME_NONE,
    TXT_PLANT_NAME_POTATO,
    TXT_PLANT_NAME_LUPINE,
    TXT_PLANT_NAME_WHEAT,
    TXT_PLANT_NAME_CORN,
};

#pragma data ( txtPlPlantStateNames )
__export const char* PLANT_STATE_NAMES[6] = {
    TXT_PLANT_STATE_NAME_1,
    TXT_PLANT_STATE_NAME_2,
    TXT_PLANT_STATE_NAME_3,
    TXT_PLANT_STATE_NAME_4,
    TXT_PLANT_STATE_NAME_5,
    TXT_PLANT_STATE_NAME_6
};

#pragma data ( txtPlMainArray )
__export const char* TXT[] = {
    TXT_MENU_CANCEL,
    TXT_MENU_EXIT,
    TXT_TASK_EMPTY_NAME,
    TXT_TASK_EMPTY_DESCRIPTION,
    TXT_TASK_LIST_HEADER,

    TXT_MENU_MAIN1,
    TXT_MENU_MAIN2,
    TXT_MENU_MAIN3,
    TXT_MENU_MAIN4,
    TXT_MENU_MAIN5,

    TXT_MENU_OPTIONS_LANG,
    TXT_MENU_OPTIONS_MSX,
    TXT_OPTIONS_HEADER,

    TXT_MENU_FARMLAND1,
    TXT_MENU_FARMLAND2,
    TXT_MENU_FARMLAND3,
    TXT_MENU_FARMLAND4,
    TXT_MENU_FARMLAND5,
    TXT_MENU_FARMLAND6,
    TXT_MENU_FARMLAND7,
    TXT_FIELD_LIST_HEADER,
    TXT_FARM_FIELD_TXT,
    TXT_FARM_PLANT_TXT,

    TXT_TASK_FARMLAND_FARM,
    TXT_TASK_FARMLAND_CARE,
    TXT_TASK_FARMLAND_REAP,
    TXT_TASK_DSC_FARMLAND_FIELD,
    TXT_TASK_DSC_FARMLAND_POTATO,
    TXT_TASK_DSC_FARMLAND_LUPINE,
    TXT_TASK_DSC_FARMLAND_WHEAT,
    TXT_TASK_DSC_FARMLAND_CORN,

    TXT_MENU_PIGPEN1,
    TXT_MENU_PIGPEN2,
    TXT_MENU_PIGPEN3,

    TXT_MENU_SMOKEHOUSE1,
    TXT_MENU_SMOKEHOUSE2,
    TXT_MENU_SMOKEHOUSE3,

    TXT_MENU_SHOPOUT1,
    TXT_MENU_SHOPOUT2,
    TXT_SHOP_DESCRIPTION,

    TXT_MENU_SHOPIN1,
    TXT_MENU_SHOPIN2,

    TXT_MENU_CREW1,
    TXT_MENU_CREW2,
    TXT_MENU_CREW3,
    TXT_MENU_CREW4,

    TXT_STAT_INT,
    TXT_STAT_STR,
    TXT_STAT_CUN,
    TXT_SKILL_ANI,
    TXT_SKILL_FRM,
    TXT_SKILL_BTH,
    TXT_SKILL_TRD,

    TXT_CHAR_1_NAME,
    TXT_CHAR_1_STORY,
    TXT_CHAR_2_NAME,
    TXT_CHAR_2_STORY,
    TXT_CHAR_3_NAME,
    TXT_CHAR_3_STORY,
    TXT_CHAR_4_NAME,
    TXT_CHAR_4_STORY,

    SB_PAUSE,
    SB_UNPAUSE,
    SB_WELCOME,
    SB_SHOP_CLOSED
};

// =============================================================================
// Copy code
// =============================================================================
#pragma code ( txtCommonCode )
#pragma data ( txtCommonData )

void loadTranslation(){
    char i = 0;
    do {
#assign y 0
#repeat
        TRANSLATION_DST[y + i] = TRANSLATION_SRC[y + i];
#assign y y + 0x100
#until y == 0x100*0xf
        i++;
    } while (i != 0);
}

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )