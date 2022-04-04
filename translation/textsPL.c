#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>

#pragma data ( txtPlTxtData )

// =============================================================================
// PLANT_STATE array (8)
// =============================================================================
const char TXT_PLANT_STATE_NAME_1[] = s" - - - -";
const char TXT_PLANT_STATE_NAME_2[] = s"Kie\y5ekuje";
const char TXT_PLANT_STATE_NAME_3[] = s"Ro\y61nie  ";
const char TXT_PLANT_STATE_NAME_4[] = s"Dojrzewa";
const char TXT_PLANT_STATE_NAME_5[] = s"Dojrza\y5ee";
const char TXT_PLANT_STATE_NAME_6[] = s"Zgni\y5ee  ";

// =============================================================================
// Main texts
// =============================================================================
const char TXT_MENU_CANCEL[] = s"\y1fAnuluj";
const char TXT_MENU_EXIT[] = s"\y1fPowr\y60t";
// fixed width - 28
const char TXT_TASK_LIST_HEADER[] = underline(s"Zadanie ") "\y7f" underline(s"       Opis        ");

const char TXT_MENU_MAIN1[] = underline(s"1") s"Pole";
const char TXT_MENU_MAIN2[] = underline(s"2") s"Chlewik";
const char TXT_MENU_MAIN3[] = underline(s"3") s"W\y5ddzarnia";
const char TXT_MENU_MAIN4[] = underline(s"4") s"Sklep";
const char TXT_MENU_MAIN5[] = underline(s"5") s"Za\y5eoga";

// =============================================================================
// Options
// =============================================================================
const char TXT_MENU_OPTIONS_LANG[] = s"\yb1J\y5dzyk / Language";
const char TXT_MENU_OPTIONS_MSX[] = s"\yb2Muzyka on/off";
const char TXT_OPTIONS_HEADER[] = s"Ustawienia gry";

// =============================================================================
// Task manager
// =============================================================================
const char TXT_TASK_MANAGER_HEADER[] = s"Manager zada\y5f";
const char TXT_TASK_MANAGER_TABLE_HEADER_1[] = 
//1234567890123456789012345678901234567890
s"                     \y7f"underline(s"Zdolno")"\ye1\ydc\y7f"underline(s" Zadanie");
const char TXT_TASK_MANAGER_TABLE_HEADER_2[] = 
"                     \y7eH\y7eR\y7eM\y7eH \y7e H\y7eR\y7eM\y7eH";
const char TXT_TASK_MANAGER_TABLE_HEADER_3[] = 
underline(s"Posta")"\ydc"underline("               ")"\y7f"underline(s"o")"\y7f"underline(s"o")"\y7f"underline(s"a")"\y7f"underline(s"a ")"\y7f"underline(s" o")"\y7f"underline(s"o")"\y7f"underline(s"a")"\y7f"underline(s"a");
const char TXT_MENU_TASK_MANAGER_W[] = s"w";
const char TXT_MENU_TASK_MANAGER_S[] = s"s";
const char TXT_MENU_TASK_MANAGER_A[] = s"a";
const char TXT_MENU_TASK_MANAGER_D[] = s"d";
const char TXT_MENU_TASK_MANAGER_PLUS[] = s"-";
const char TXT_MENU_TASK_MANAGER_MINUS[] = s"+";

// =============================================================================
// Farmland
// =============================================================================
// 4 options in Common
const char TXT_MENU_FARMLAND5[] = s"\yb1Uprawa";
const char TXT_MENU_FARMLAND6[] = s"\yb2Opieka";
const char TXT_MENU_FARMLAND7[] = s"\yb3Zbiory";
// fixed width 
const char TXT_FIELD_LIST_HEADER[] =
    underline(s"Mg")
    "\y7f"
    underline(s" Uprawa  ")
    "\y7f"
    underline(s"  Stan  ")
    "\y7f"
    underline(s"T  ")
    "\y7f"
    underline(s"S1 ")
    "\y7f"
    underline(s"S2 ")
    "\y7f"
    underline(s"S3 ")
;

// 7 "Field: "
const char TXT_FARM_FIELD_TXT[]      = s"Pole:  ";
// 8
const char TXT_FARM_PLANT_TXT[]      = s"Uprawa: ";
// plant names - 10
const char TXT_PLANT_NAME_NONE[]     = s" - - - - -";
const char TXT_PLANT_NAME_POTATO[]   = s"Zimnioki  ";
const char TXT_PLANT_NAME_LUPINE[]   = "\y67" s"ubin     ";
const char TXT_PLANT_NAME_WHEAT[]    = s"Pszenica  ";
const char TXT_PLANT_NAME_CORN[]     = s"Kukurydza ";

// Task names & descriptions (8)
const char TXT_TASK_FARMLAND_FARM[]  = s"Uprawa  ";
const char TXT_TASK_FARMLAND_CARE[]  = s"Opieka  ";
const char TXT_TASK_FARMLAND_REAP[]  = s"Zbiory  ";
const char TXT_TASK_DSC_FARMLAND_FIELD[]  = s"Pole"; // used in task description
const char TXT_TASK_DSC_FARMLAND_POTATO[] = s"ziemniaki"; // used in task description
const char TXT_TASK_DSC_FARMLAND_LUPINE[] = s"\y5eubin"; // used in task description
const char TXT_TASK_DSC_FARMLAND_WHEAT[]  = s"pszenica"; // used in task description
const char TXT_TASK_DSC_FARMLAND_CORN[]   = s"kukurydza"; // used in task description

// =============================================================================
// Pig pen
// =============================================================================
const char TXT_MENU_PIGPEN1[] = s"\yb1Karmienie";
const char TXT_MENU_PIGPEN2[] = s"\yb2Sprz\y5btanie";
const char TXT_MENU_PIGPEN3[] = s"\yb3Przebudowa";

const char TXT_MENU_SMOKEHOUSE1[] = s"\yb1Przepisy";
const char TXT_MENU_SMOKEHOUSE2[] = s"\yb2Sprz\y5btanie";
const char TXT_MENU_SMOKEHOUSE3[] = s"\yb3Przebudowa";

const char TXT_MENU_SHOPOUT1[] = s"\yb1Wejd\y62";
const char TXT_MENU_SHOPOUT2[] = s"\yb2Rozmowa";
const char TXT_SHOP_DESCRIPTION[] = s"Sklep wieobranzowy 'GS', otwarty 7-11 i 15-19, codziennie.";

const char TXT_MENU_SHOPIN1[] = s"\yb1Kup og\y60ra";
const char TXT_MENU_SHOPIN2[] = s"\yb2Rozmowa";

const char TXT_MENU_CREW1[] = s"\yb1Slot 1";
const char TXT_MENU_CREW2[] = s"\yb2Slot 2";
const char TXT_MENU_CREW3[] = s"\yb3Slot 3";
const char TXT_MENU_CREW4[] = s"\yb4Slot 4";

// =============================================================================
// Characters
// =============================================================================
const char TXT_STAT_INT[] = s"M\y5bdro""\y61\y5c";
const char TXT_STAT_STR[] = s"Si\y5ea";
const char TXT_STAT_CUN[] = s"Spryt";

const char TXT_SKILL_ANI[] = s"Hodowla";
const char TXT_SKILL_FRM[] = s"Rolnictwo";
const char TXT_SKILL_BTH[] = s"Masarstwo";
const char TXT_SKILL_TRD[] = s"Handel";

// names are in Common
const char TXT_CHAR_1_STORY[] = s"Janusz ca\y5ee \y63ycie pracowa\y5e w PGR. Na niczym si\y5d tak naprawd\y5d nie zna, ale za toma na\y5eogi i jest leniwy. Janusz to posta\y5c startowa.";
const char TXT_CHAR_2_STORY[] = s"Zdzisiek nie jest popularny we wsi. Ludziska pami\y5dtaj\y5b, \y63e jego ojciec by\y5e folks-dojczem. Zdzisia wyr\y60\y63nia inteligencja po tacie.";
const char TXT_CHAR_3_STORY[] = s"Historia Wie\y61ka M test M test.";
const char TXT_CHAR_4_STORY[] = s"Historia J\y60zka.";

// =============================================================================
// Status Bar
// =============================================================================
const char SB_PAUSE[]       = s"   P  A  U  Z  A   ";
const char SB_UNPAUSE[]     = s"   Normalna pr\y5ddko\y61\y5c gry   ";
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

    TXT_TASK_MANAGER_HEADER,
    TXT_TASK_MANAGER_TABLE_HEADER_1,
    TXT_TASK_MANAGER_TABLE_HEADER_2,
    TXT_TASK_MANAGER_TABLE_HEADER_3,
    TXT_MENU_TASK_MANAGER_W,
    TXT_MENU_TASK_MANAGER_S,
    TXT_MENU_TASK_MANAGER_A,
    TXT_MENU_TASK_MANAGER_D,
    TXT_MENU_TASK_MANAGER_PLUS,
    TXT_MENU_TASK_MANAGER_MINUS,

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