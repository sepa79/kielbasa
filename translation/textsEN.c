#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>

#pragma data ( txtEnTxtData )

// =============================================================================
// PLANT_STATE array (8)
// =============================================================================
const char TXT_EN_PLANT_STATE_NAME_1[] = s"- - - - ";
const char TXT_EN_PLANT_STATE_NAME_2[] = s"Sprouts ";
const char TXT_EN_PLANT_STATE_NAME_3[] = s"Grows   ";
const char TXT_EN_PLANT_STATE_NAME_4[] = s"Ripens  ";
const char TXT_EN_PLANT_STATE_NAME_5[] = s"Ready   ";
const char TXT_EN_PLANT_STATE_NAME_6[] = s"Rots    ";

// =============================================================================
// Main texts
// =============================================================================
const char TXT_EN_MENU_CANCEL[] = s"\y1fCancel";
const char TXT_EN_MENU_EXIT[] = s"\y1fBack";
// fixed width - 28
const char TXT_EN_TASK_LIST_HEADER[] = underline(s"  Task  ") "\y7f" underline(s"    Description    ");

const char TXT_EN_MENU_MAIN1[] = s"\yb1Farmland";
const char TXT_EN_MENU_MAIN2[] = s"\yb2Pigpen";
const char TXT_EN_MENU_MAIN3[] = s"\yb3Smokehouse";
const char TXT_EN_MENU_MAIN4[] = s"\yb4Shop";
const char TXT_EN_MENU_MAIN5[] = s"\yb5Crew";

// =============================================================================
// Options
// =============================================================================
const char TXT_EN_MENU_OPTIONS_LANG[] = s"\yb1Language / J\y5dzyk";
const char TXT_EN_MENU_OPTIONS_MSX[] = s"\yb2Music on/off";
const char TXT_EN_OPTIONS_HEADER[] = s"Game settings";

// =============================================================================
// Task manager
// =============================================================================
const char TXT_EN_TASK_MANAGER_HEADER[] = s"Task manager";
const char TXT_EN_TASK_MANAGER_TABLE_HEADER_1[] = s"Task manager";
const char TXT_EN_TASK_MANAGER_TABLE_HEADER_2[] = s"Task manager";
const char TXT_EN_TASK_MANAGER_TABLE_HEADER_3[] = s"Task manager";
const char TXT_EN_TASK_MANAGER_TABLE_HEADER_4[] = s"Task manager";
const char TXT_EN_TASK_MANAGER_TABLE_HEADER_5[] = s"Task manager";
const char TXT_EN_TASK_MANAGER_TABLE_HEADER_6[] = s"Task manager";
const char TXT_EN_TASK_MANAGER_TABLE_HEADER_7[] = s"Task manager";
const char TXT_EN_TASK_MANAGER_TABLE_HEADER_8[] = s"Task manager";
const char TXT_EN_TASK_MANAGER_TABLE_HEADER_9[] = s"Task manager";
const char TXT_EN_TASK_MANAGER_TABLE_HEADER_10[] = s"Task manager";
const char TXT_EN_MENU_TASK_MANAGER_W[] = s"w";
const char TXT_EN_MENU_TASK_MANAGER_S[] = s"s";
const char TXT_EN_MENU_TASK_MANAGER_A[] = s"a";
const char TXT_EN_MENU_TASK_MANAGER_D[] = s"d";
const char TXT_EN_MENU_TASK_MANAGER_PLUS[] = s"+";
const char TXT_EN_MENU_TASK_MANAGER_MINUS[] = s"-";

// =============================================================================
// Farmland
// =============================================================================
// 4 options in Common
const char TXT_EN_MENU_FARMLAND5[] = s"\yb1Grow";
const char TXT_EN_MENU_FARMLAND6[] = s"\yb2Care";
const char TXT_EN_MENU_FARMLAND7[] = s"\yb3Reap";
// fixed width 
const char TXT_EN_FIELD_LIST_HEADER[] = 
    underline(s"Mg")
    "\y7f"
    underline(s" Farming ")
    "\y7f"
    underline(s" State  ")
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
const char TXT_EN_FARM_FIELD_TXT[]      = s"Field: ";
// 8
const char TXT_EN_FARM_PLANT_TXT[]      = s"Growing:";
// plant names - 10
const char TXT_EN_PLANT_NAME_NONE[]     = s"- - - - - ";
const char TXT_EN_PLANT_NAME_POTATO[]   = s"Potatoes  ";
const char TXT_EN_PLANT_NAME_LUPINE[]   = s"Lupine    ";
const char TXT_EN_PLANT_NAME_WHEAT[]    = s"Wheat     ";
const char TXT_EN_PLANT_NAME_CORN[]     = s"Corn      ";

// Task names & descriptions (8)
const char TXT_EN_TASK_FARMLAND_FARM[]  = s"Farm    ";
const char TXT_EN_TASK_FARMLAND_CARE[]  = s"Care    ";
const char TXT_EN_TASK_FARMLAND_REAP[]  = s"Reap    ";
const char TXT_EN_TASK_DSC_FARMLAND_FIELD[]  = s"Field"; // used in task description
const char TXT_EN_TASK_DSC_FARMLAND_POTATO[] = s"potatoes"; // used in task description
const char TXT_EN_TASK_DSC_FARMLAND_LUPINE[] = s"lupine"; // used in task description
const char TXT_EN_TASK_DSC_FARMLAND_WHEAT[]  = s"wheat"; // used in task description
const char TXT_EN_TASK_DSC_FARMLAND_CORN[]   = s"corn"; // used in task description

// =============================================================================
// Pig pen
// =============================================================================
const char TXT_EN_MENU_PIGPEN1[] = s"\yb1Feed";
const char TXT_EN_MENU_PIGPEN2[] = s"\yb2Cleanup";
const char TXT_EN_MENU_PIGPEN3[] = s"\yb3Upgrade";

const char TXT_EN_MENU_SMOKEHOUSE1[] = s"\yb1Recipies";
const char TXT_EN_MENU_SMOKEHOUSE2[] = s"\yb2Cleanup";
const char TXT_EN_MENU_SMOKEHOUSE3[] = s"\yb3Upgrade";

const char TXT_EN_MENU_SHOPOUT1[] = s"\yb1Go in";
const char TXT_EN_MENU_SHOPOUT2[] = s"\yb2Chat";
const char TXT_EN_SHOP_DESCRIPTION[] = s"A general store, open 7-11, then 15-19, everyday";

const char TXT_EN_MENU_SHOPIN1[] = s"\yb1Buy gherkin";
const char TXT_EN_MENU_SHOPIN2[] = s"\yb2Chat";

const char TXT_EN_MENU_CREW1[] = s"\yb1Char 1";
const char TXT_EN_MENU_CREW2[] = s"\yb2Char 2";
const char TXT_EN_MENU_CREW3[] = s"\yb3Char 3";
const char TXT_EN_MENU_CREW4[] = s"\yb4Char 4";

// =============================================================================
// Characters
// =============================================================================
const char TXT_EN_STAT_INT[] = s"Wisdom";
const char TXT_EN_STAT_STR[] = s"Strength";
const char TXT_EN_STAT_CUN[] = s"Cunning";

const char TXT_EN_SKILL_ANI[] = s"Breeding";
const char TXT_EN_SKILL_FRM[] = s"Farming";
const char TXT_EN_SKILL_BTH[] = s"Butchery";
const char TXT_EN_SKILL_TRD[] = s"Trade";

// names are in Common
const char TXT_EN_CHAR_1_STORY[] = s"Janusz spent his whole life working in PGR. He does not really excel in anything, but thankfully he is lazy and an addict. He is your main character.";
const char TXT_EN_CHAR_2_STORY[] = s"Zdzisiek is not very popularin the villiage. Folks reme-mber that his father was a volksdeustch. He is bright - like his father - and that makes him stand out.";
const char TXT_EN_CHAR_3_STORY[] = s"Wiesiek's story will be here.";
const char TXT_EN_CHAR_4_STORY[] = s"J\y60zek's story will be here.";

// =============================================================================
// Status Bar
// =============================================================================
const char SB_EN_PAUSE[]       = s"   P  A  U  S  E   ";
const char SB_EN_UNPAUSE[]     = s"   Normal game speed   ";
const char SB_EN_WELCOME[]     = s"    Hello! +/-/Space - time control    Kielbasa Empire - very early alpha.    "
                                          s"Credits:    "
                                          s"    Code support and compilers: KickC - Rex/Camelot    Oscar64 - DrMortalWombat    "
                                          s"Game: Code - Sepa, Gfx - Juras, Model3 & Saturnz, Msx - Taurus, Support - Angry Andy, Fonts - Sepa ";
const char SB_EN_SHOP_CLOSED[] = s"   Shop is closed   ";


// =============================================================================
// Arrays
// =============================================================================
#pragma data ( txtEnPlantTypes )
__export const char* PLANT_TYPES_TXT_EN[] = {
    TXT_EN_PLANT_NAME_NONE,
    TXT_EN_PLANT_NAME_POTATO,
    TXT_EN_PLANT_NAME_LUPINE,
    TXT_EN_PLANT_NAME_WHEAT,
    TXT_EN_PLANT_NAME_CORN,
};

#pragma data ( txtEnPlantStateNames )
__export const char* PLANT_STATE_NAMES_EN[6] = {
    TXT_EN_PLANT_STATE_NAME_1,
    TXT_EN_PLANT_STATE_NAME_2,
    TXT_EN_PLANT_STATE_NAME_3,
    TXT_EN_PLANT_STATE_NAME_4,
    TXT_EN_PLANT_STATE_NAME_5,
    TXT_EN_PLANT_STATE_NAME_6
};

#pragma data ( txtEnMainArray )
__export const char* TXT_EN_ARRAY[] = {
    TXT_EN_MENU_CANCEL,
    TXT_EN_MENU_EXIT,
    TXT_TASK_EMPTY_NAME,
    TXT_TASK_EMPTY_DESCRIPTION,
    TXT_EN_TASK_LIST_HEADER,

    TXT_EN_MENU_MAIN1,
    TXT_EN_MENU_MAIN2,
    TXT_EN_MENU_MAIN3,
    TXT_EN_MENU_MAIN4,
    TXT_EN_MENU_MAIN5,

    TXT_EN_MENU_OPTIONS_LANG,
    TXT_EN_MENU_OPTIONS_MSX,
    TXT_EN_OPTIONS_HEADER,

    TXT_EN_TASK_MANAGER_HEADER,
    TXT_EN_TASK_MANAGER_TABLE_HEADER_1,
    TXT_EN_TASK_MANAGER_TABLE_HEADER_2,
    TXT_EN_TASK_MANAGER_TABLE_HEADER_3,
    TXT_EN_TASK_MANAGER_TABLE_HEADER_4,
    TXT_EN_TASK_MANAGER_TABLE_HEADER_5,
    TXT_EN_TASK_MANAGER_TABLE_HEADER_6,
    TXT_EN_TASK_MANAGER_TABLE_HEADER_7,
    TXT_EN_TASK_MANAGER_TABLE_HEADER_8,
    TXT_EN_TASK_MANAGER_TABLE_HEADER_9,
    TXT_EN_TASK_MANAGER_TABLE_HEADER_10,
    TXT_EN_MENU_TASK_MANAGER_W,
    TXT_EN_MENU_TASK_MANAGER_S,
    TXT_EN_MENU_TASK_MANAGER_A,
    TXT_EN_MENU_TASK_MANAGER_D,
    TXT_EN_MENU_TASK_MANAGER_PLUS,
    TXT_EN_MENU_TASK_MANAGER_MINUS,

    TXT_MENU_FARMLAND1,
    TXT_MENU_FARMLAND2,
    TXT_MENU_FARMLAND3,
    TXT_MENU_FARMLAND4,
    TXT_EN_MENU_FARMLAND5,
    TXT_EN_MENU_FARMLAND6,
    TXT_EN_MENU_FARMLAND7,
    TXT_EN_FIELD_LIST_HEADER,
    TXT_EN_FARM_FIELD_TXT,
    TXT_EN_FARM_PLANT_TXT,

    TXT_EN_TASK_FARMLAND_FARM,
    TXT_EN_TASK_FARMLAND_CARE,
    TXT_EN_TASK_FARMLAND_REAP,
    TXT_EN_TASK_DSC_FARMLAND_FIELD,
    TXT_EN_TASK_DSC_FARMLAND_POTATO,
    TXT_EN_TASK_DSC_FARMLAND_LUPINE,
    TXT_EN_TASK_DSC_FARMLAND_WHEAT,
    TXT_EN_TASK_DSC_FARMLAND_CORN,

    TXT_EN_MENU_PIGPEN1,
    TXT_EN_MENU_PIGPEN2,
    TXT_EN_MENU_PIGPEN3,

    TXT_EN_MENU_SMOKEHOUSE1,
    TXT_EN_MENU_SMOKEHOUSE2,
    TXT_EN_MENU_SMOKEHOUSE3,

    TXT_EN_MENU_SHOPOUT1,
    TXT_EN_MENU_SHOPOUT2,
    TXT_EN_SHOP_DESCRIPTION,

    TXT_EN_MENU_SHOPIN1,
    TXT_EN_MENU_SHOPIN2,

    TXT_EN_MENU_CREW1,
    TXT_EN_MENU_CREW2,
    TXT_EN_MENU_CREW3,
    TXT_EN_MENU_CREW4,

    TXT_EN_STAT_INT,
    TXT_EN_STAT_STR,
    TXT_EN_STAT_CUN,
    TXT_EN_SKILL_ANI,
    TXT_EN_SKILL_FRM,
    TXT_EN_SKILL_BTH,
    TXT_EN_SKILL_TRD,

    TXT_CHAR_1_NAME,
    TXT_EN_CHAR_1_STORY,
    TXT_CHAR_2_NAME,
    TXT_EN_CHAR_2_STORY,
    TXT_CHAR_3_NAME,
    TXT_EN_CHAR_3_STORY,
    TXT_CHAR_4_NAME,
    TXT_EN_CHAR_4_STORY,
    
    SB_EN_PAUSE,
    SB_EN_UNPAUSE,
    SB_EN_WELCOME,
    SB_EN_SHOP_CLOSED
};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )