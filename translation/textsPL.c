#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>

#pragma data ( txtPlTxtData )

const char TXT_PL_PLANT_NAME_NONE[] =  s" - - - - -";
const char TXT_PL_PLANT_NAME_POTATO[] =  s"Ziemniaki ";
const char TXT_PL_PLANT_NAME_LUPINE[] =  s"\y67ubin     ";
const char TXT_PL_PLANT_NAME_WHEAT[] =  s"Pszenica  ";
const char TXT_PL_PLANT_NAME_CORN[] =  s"Kukurydza ";
const char TXT_PL_PLANT_STAGE_NONE[] =  s" - - - -";
const char TXT_PL_PLANT_STAGE_SOW_TASK_ASSIGNED[] =  s"Z\y3a Siew ";
const char TXT_PL_PLANT_STAGE_SPROUT[] =  s"Kie\y5ekuje";
const char TXT_PL_PLANT_STAGE_GROWTH[] =  s"Ro\y61nie";
const char TXT_PL_PLANT_STAGE_RIPEN[] =  s"Dojrzewa";
const char TXT_PL_PLANT_STAGE_READY[] =  s"Dojrza\y5ey";
const char TXT_PL_PLANT_STAGE_REAP_TASK_ASSIGNED[] =  s"Z\y3a Zbi\y60r";
const char TXT_PL_MENU_CANCEL[] =  s"\y1fAnuluj";
const char TXT_PL_MENU_EXIT[] =  s"\y1fPowr\y60t";
const char TXT_PL_TASK_LIST_HEADER[] =  s"\yda\y81\y84\y81\y8e\y89\y85\ya0\y7f\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ycf\y90\y89\y93\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ya0";
const char TXT_PL_MENU_MAIN1[] =  s"\yb1Pole";
const char TXT_PL_MENU_MAIN2[] =  s"\yb2Chlewik";
const char TXT_PL_MENU_MAIN3[] =  s"\yb3W\y5ddzarnia";
const char TXT_PL_MENU_MAIN4[] =  s"\yb4Sklep";
const char TXT_PL_MENU_MAIN5[] =  s"\yb5Za\y5eoga";
const char TXT_PL_MENU_OPTIONS_LANG[] =  s"\yb1J\y5dzyk \y2f Language";
const char TXT_PL_MENU_OPTIONS_MSX[] =  s"\yb2Muzyka on\y2foff";
const char TXT_PL_OPTIONS_HEADER[] =  s"Ustawienia gry";
const char TXT_PL_TASK_MANAGER_PRIO_HEADER[] =  s"Manager priorytet\y60w zada\y5f";
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_1[] =  s"\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ya0\y7f\yda\y84\y8f\y8c\y8e\y8f\ye1\ydc\y7f\ya0\yda\y81\y84\y81\y8e\y89\y85";
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_2[] =  s"                     \y7e \y7eR\y7eM\y7e  \y7e  \y7eR\y7eM\y7e ";
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_3[] =  s"                     \y7e \y7eo\y7ea\y7e  \y7e  \y7eo\y7ea\y7e ";
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_4[] =  s"                     \y7eH\y7el\y7es\y7e  \y7e H\y7el\y7es\y7e ";
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_5[] =  s"                     \y7eo\y7en\y7ea\y7eH \y7e o\y7en\y7ea\y7eH";
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_6[] =  s"                     \y7ed\y7ei\y7er\y7ea \y7e d\y7ei\y7er\y7ea";
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_7[] =  s"                     \y7eo\y7ec\y7es\y7en \y7e o\y7ec\y7es\y7en";
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_8[] =  s"                     \y7ew\y7et\y7et\y7ed \y7e w\y7et\y7et\y7ed";
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_9[] =  s"                     \y7el\y7ew\y7ew\y7ee \y7e l\y7ew\y7ew\y7ee";
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_10[] =  s"\yd0\y8f\y93\y94\y81\ydc\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ya0\y7f\y81\y7f\y8f\y7f\y8f\y7f\y8c\ya0\y7f\ya0\y81\y7f\y8f\y7f\y8f\y7f\y8c";
const char TXT_PL_MENU_TASK_MANAGER_W[] =  s"w";
const char TXT_PL_MENU_TASK_MANAGER_S[] =  s"s";
const char TXT_PL_MENU_TASK_MANAGER_A[] =  s"a";
const char TXT_PL_MENU_TASK_MANAGER_D[] =  s"d";
const char TXT_PL_MENU_TASK_MANAGER_PLUS[] =  s"\y2b";
const char TXT_PL_MENU_TASK_MANAGER_MINUS[] =  s"-";
const char TXT_PL_TASK_MANAGER_HEADER[] =  s"Manager zada\y5f";
const char TXT_PL_TASK_MANAGER_TABLE_HEADER[] =  s"\yda\y81\y84\y81\y8e\y89\y85\ya0\y7f\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ycf\y90\y89\y93\ya0\ya0\ya0\ya0\ya0\ya0\ya0\ya0";
const char TXT_PL_MENU_FARMLAND5[] =  s"\yb1Uprawa";
const char TXT_PL_MENU_FARMLAND6[] =  s"\yb2Opieka";
const char TXT_PL_MENU_FARMLAND7[] =  s"\yb3Zbiory";
const char TXT_PL_FIELD_LIST_HEADER[] =  s"\ycd\y87\y7f\ya0\yd5\y90\y92\y81\y97\y81\ya0\ya0\y7f\ya0\ya0\yd3\y94\y81\y8e\ya0\ya0\y7f\yd4\ya0\ya0\y7f\yd3\yb1\ya0\y7f\yd3\yb2\ya0\y7f\yd3\yb3\ya0";
const char TXT_PL_FARM_FIELD_TXT[] =  s"Pole\y3a  ";
const char TXT_PL_FARM_PLANT_TXT[] =  s"Uprawa\y3a ";
const char TXT_PL_TASK_FARMLAND_FARM[] =  s"Uprawa  ";
const char TXT_PL_TASK_FARMLAND_CARE[] =  s"Opieka  ";
const char TXT_PL_TASK_FARMLAND_REAP[] =  s"Zbiory  ";
const char TXT_PL_TASK_DSC_FARMLAND_FIELD[] =  s"Pole";
const char TXT_PL_TASK_DSC_FARMLAND_POTATO[] =  s"ziemniaki";
const char TXT_PL_TASK_DSC_FARMLAND_LUPINE[] =  s"\y5eubin";
const char TXT_PL_TASK_DSC_FARMLAND_WHEAT[] =  s"przenica";
const char TXT_PL_TASK_DSC_FARMLAND_CORN[] =  s"kukurydza";
const char TXT_PL_MENU_PIGPEN1[] =  s"\yb1Karmienie";
const char TXT_PL_MENU_PIGPEN2[] =  s"\yb2Sprz\y5btanie";
const char TXT_PL_MENU_PIGPEN3[] =  s"\yb3Przebudowa";
const char TXT_PL_MENU_SMOKEHOUSE1[] =  s"\yb1Przepisy";
const char TXT_PL_MENU_SMOKEHOUSE2[] =  s"\yb2Sprz\y5btanie";
const char TXT_PL_MENU_SMOKEHOUSE3[] =  s"\yb3Przebudowa";
const char TXT_PL_MENU_SHOPOUT1[] =  s"\yb1Wejd\y62";
const char TXT_PL_MENU_SHOPOUT2[] =  s"\yb2Rozmowa";
const char TXT_PL_SHOP_DESCRIPTION[] =  s"Sklep wielobran\y63oy \y27GS\y27\y2c otwarty 7-11 i 15-19\y2c codziennie\y2e";
const char TXT_PL_MENU_SHOPIN1[] =  s"\yb1Kup og\y60ra";
const char TXT_PL_MENU_SHOPIN2[] =  s"\yb2Rozmowa";
const char TXT_PL_MENU_CREW1[] =  s"\yb1Slot 1";
const char TXT_PL_MENU_CREW2[] =  s"\yb2Slot 2";
const char TXT_PL_MENU_CREW3[] =  s"\yb3Slot 3";
const char TXT_PL_MENU_CREW4[] =  s"\yb4Slot 4";
const char TXT_PL_STAT_INT[] =  s"M\y5bdro\y61\y5c";
const char TXT_PL_STAT_STR[] =  s"Si\y5ea";
const char TXT_PL_STAT_CUN[] =  s"Spryt";
const char TXT_PL_SKILL_ANI[] =  s"Hodowla";
const char TXT_PL_SKILL_FRM[] =  s"Rolnictwo";
const char TXT_PL_SKILL_BTH[] =  s"Masarstwo";
const char TXT_PL_SKILL_TRD[] =  s"Handel";
const char TXT_PL_CHAR_1_STORY[] =  s"Janusz ca\y5ee \y63ycie pracowa\y5e w PGR\y2e Na niczym si\y5d tak naprawd\y5d nie zna\y2c ale za to ma na\y5eogi i jest leniwy\y2e Janusz to posta\y5c startowa\y2e";
const char TXT_PL_CHAR_2_STORY[] =  s"Zdzisiek nie jest popularny we wsi\y2e Ludziska pami\y5dtaj\y5b\y2c \y63e jego ojciec by\y5e folks-dojczem\y2e Zdzisia wyr\y60\y63nia inteligencja po tacie\y2e";
const char TXT_PL_CHAR_3_STORY[] =  s"Historia Wie\y61ka M test M test";
const char TXT_PL_CHAR_4_STORY[] =  s"Historia J\y60zka";
const char SB_PL_PAUSE[] =  s"   P  A  U  Z  A   ";
const char SB_PL_UNPAUSE[] =  s"   Normalna pr\y5ddko\y61\y5c gry   ";
const char SB_PL_WELCOME[] =  s"    Witaj\y21 \y2b\y2f-\y2fSpace - kontrola czasu    Kielbasa Empire - wersja bardzo alpha\y2e    Credits\y3a        Wsparcie programistyczne i kompilatory\y3a KickC - Rex\y2fCamelot\y2c Oscar64 - DrMortalWombat        Gra\y3a Kod - Sepa\y2c Gfx - Juras\y2c Model3 \y26 Saturnz\y2c Msx - Taurus\y2c Support - Angry Andy\y2c Fonts - Sepa    ";
const char SB_PL_SHOP_CLOSED[] =  s"   Sklep jest zamkniety   ";

#pragma data ( txtPlPlantTypes )
__export const char* PLANT_TYPES_TXT[] = {
    TXT_PL_PLANT_NAME_NONE,
    TXT_PL_PLANT_NAME_POTATO,
    TXT_PL_PLANT_NAME_LUPINE,
    TXT_PL_PLANT_NAME_WHEAT,
    TXT_PL_PLANT_NAME_CORN
};

#pragma data ( txtPlPlantStateNames )
__export const char* PLANT_STAGE_NAMES[] = {
    TXT_PL_PLANT_STAGE_NONE,
    TXT_PL_PLANT_STAGE_SOW_TASK_ASSIGNED,
    TXT_PL_PLANT_STAGE_SPROUT,
    TXT_PL_PLANT_STAGE_GROWTH,
    TXT_PL_PLANT_STAGE_RIPEN,
    TXT_PL_PLANT_STAGE_READY,
    TXT_PL_PLANT_STAGE_REAP_TASK_ASSIGNED
};

#pragma data ( txtPlMainArray )
__export const char* TXT[] = {
    TXT_PL_MENU_CANCEL,
    TXT_PL_MENU_EXIT,
    TXT_TASK_EMPTY_NAME,
    TXT_TASK_EMPTY_DESCRIPTION,
    TXT_PL_TASK_LIST_HEADER,
    TXT_PL_MENU_MAIN1,
    TXT_PL_MENU_MAIN2,
    TXT_PL_MENU_MAIN3,
    TXT_PL_MENU_MAIN4,
    TXT_PL_MENU_MAIN5,
    TXT_PL_MENU_OPTIONS_LANG,
    TXT_PL_MENU_OPTIONS_MSX,
    TXT_PL_OPTIONS_HEADER,
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
    TXT_PL_MENU_TASK_MANAGER_W,
    TXT_PL_MENU_TASK_MANAGER_S,
    TXT_PL_MENU_TASK_MANAGER_A,
    TXT_PL_MENU_TASK_MANAGER_D,
    TXT_PL_MENU_TASK_MANAGER_PLUS,
    TXT_PL_MENU_TASK_MANAGER_MINUS,
    TXT_PL_TASK_MANAGER_HEADER,
    TXT_PL_TASK_MANAGER_TABLE_HEADER,
    TXT_MENU_FARMLAND1,
    TXT_MENU_FARMLAND2,
    TXT_MENU_FARMLAND3,
    TXT_MENU_FARMLAND4,
    TXT_PL_MENU_FARMLAND5,
    TXT_PL_MENU_FARMLAND6,
    TXT_PL_MENU_FARMLAND7,
    TXT_PL_FIELD_LIST_HEADER,
    TXT_PL_FARM_FIELD_TXT,
    TXT_PL_FARM_PLANT_TXT,
    TXT_PL_TASK_FARMLAND_FARM,
    TXT_PL_TASK_FARMLAND_CARE,
    TXT_PL_TASK_FARMLAND_REAP,
    TXT_PL_TASK_DSC_FARMLAND_FIELD,
    TXT_PL_TASK_DSC_FARMLAND_POTATO,
    TXT_PL_TASK_DSC_FARMLAND_LUPINE,
    TXT_PL_TASK_DSC_FARMLAND_WHEAT,
    TXT_PL_TASK_DSC_FARMLAND_CORN,
    TXT_PL_MENU_PIGPEN1,
    TXT_PL_MENU_PIGPEN2,
    TXT_PL_MENU_PIGPEN3,
    TXT_PL_MENU_SMOKEHOUSE1,
    TXT_PL_MENU_SMOKEHOUSE2,
    TXT_PL_MENU_SMOKEHOUSE3,
    TXT_PL_MENU_SHOPOUT1,
    TXT_PL_MENU_SHOPOUT2,
    TXT_PL_SHOP_DESCRIPTION,
    TXT_PL_MENU_SHOPIN1,
    TXT_PL_MENU_SHOPIN2,
    TXT_PL_MENU_CREW1,
    TXT_PL_MENU_CREW2,
    TXT_PL_MENU_CREW3,
    TXT_PL_MENU_CREW4,
    TXT_PL_STAT_INT,
    TXT_PL_STAT_STR,
    TXT_PL_STAT_CUN,
    TXT_PL_SKILL_ANI,
    TXT_PL_SKILL_FRM,
    TXT_PL_SKILL_BTH,
    TXT_PL_SKILL_TRD,
    TXT_CHAR_1_NAME,
    TXT_PL_CHAR_1_STORY,
    TXT_CHAR_2_NAME,
    TXT_PL_CHAR_2_STORY,
    TXT_CHAR_3_NAME,
    TXT_PL_CHAR_3_STORY,
    TXT_CHAR_4_NAME,
    TXT_PL_CHAR_4_STORY,
    SB_PL_PAUSE,
    SB_PL_UNPAUSE,
    SB_PL_WELCOME,
    SB_PL_SHOP_CLOSED
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

