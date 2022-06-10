#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>

#pragma data ( txtEnTxtData )

const char TXT_EN_PLANT_NAME_NONE[] =  s"- - - - - ";
const char TXT_EN_PLANT_NAME_POTATO[] =  s"Potatoes  ";
const char TXT_EN_PLANT_NAME_LUPINE[] =  s"Lupine    ";
const char TXT_EN_PLANT_NAME_WHEAT[] =  s"Wheat     ";
const char TXT_EN_PLANT_NAME_CORN[] =  s"Corn      ";
const char TXT_EN_PLANT_STAGE_NONE[] =  s"- - - - ";
const char TXT_EN_PLANT_STAGE_SOW_TASK_ASSIGNED[] =  s"T\y3a Plant";
const char TXT_EN_PLANT_STAGE_SPROUT[] =  s"Sprouts";
const char TXT_EN_PLANT_STAGE_GROWTH[] =  s"Grows";
const char TXT_EN_PLANT_STAGE_RIPEN[] =  s"Ripens";
const char TXT_EN_PLANT_STAGE_READY[] =  s"Ready";
const char TXT_EN_PLANT_STAGE_REAP_TASK_ASSIGNED[] =  s"T\y3a Reap";
const char TXT_EN_MENU_CANCEL[] =  s"\y1fCancel";
const char TXT_EN_MENU_EXIT[] =  s"\y1fBack";
const char TXT_EN_TASK_LIST_HEADER[] =  s"\ya0\ya0\yd4\y81\y93\y8b\ya0\ya0\y7f\ya0\ya0\ya0\ya0\yc4\y85\y93\y83\y92\y89\y90\y94\y89\y8f\y8e\ya0\ya0\ya0\ya0";
const char TXT_EN_MENU_MAIN1[] =  s"\yb1Farmland";
const char TXT_EN_MENU_MAIN2[] =  s"\yb2Pigpen";
const char TXT_EN_MENU_MAIN3[] =  s"\yb3Smokehouse";
const char TXT_EN_MENU_MAIN4[] =  s"\yb4Shop";
const char TXT_EN_MENU_MAIN5[] =  s"\yb5Crew";
const char TXT_EN_MENU_OPTIONS_LANG[] =  s"\yb1Language \y2f J\y5dzyk";
const char TXT_EN_MENU_OPTIONS_MSX[] =  s"\yb2Music on\y2foff";
const char TXT_EN_OPTIONS_HEADER[] =  s"Game settings";
const char TXT_EN_TASK_MANAGER_PRIO_HEADER[] =  s"Task Priority Manager";
const char TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_1[] =  s"Task manager";
const char TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_2[] =  s"Task manager";
const char TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_3[] =  s"Task manager";
const char TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_4[] =  s"Task manager";
const char TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_5[] =  s"Task manager";
const char TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_6[] =  s"Task manager";
const char TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_7[] =  s"Task manager";
const char TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_8[] =  s"Task manager";
const char TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_9[] =  s"Task manager";
const char TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_10[] =  s"Task manager";
const char TXT_EN_MENU_TASK_MANAGER_W[] =  s"w";
const char TXT_EN_MENU_TASK_MANAGER_S[] =  s"s";
const char TXT_EN_MENU_TASK_MANAGER_A[] =  s"a";
const char TXT_EN_MENU_TASK_MANAGER_D[] =  s"d";
const char TXT_EN_MENU_TASK_MANAGER_PLUS[] =  s"\y2b";
const char TXT_EN_MENU_TASK_MANAGER_MINUS[] =  s"-";
const char TXT_EN_TASK_MANAGER_HEADER[] =  s"Task Manager";
const char TXT_EN_TASK_MANAGER_TABLE_HEADER[] =  s"\ya0\ya0\yd4\y81\y93\y8b\ya0\ya0\y7f\ya0\ya0\ya0\ya0\yc4\y85\y93\y83\y92\y89\y90\y94\y89\y8f\y8e\ya0\ya0\ya0\ya0";
const char TXT_EN_MENU_FARMLAND5[] =  s"\yb1Grow";
const char TXT_EN_MENU_FARMLAND6[] =  s"\yb2Care";
const char TXT_EN_MENU_FARMLAND7[] =  s"\yb3Reap";
const char TXT_EN_FIELD_LIST_HEADER[] =  s"\ycd\y87\y7f\ya0\yc6\y81\y92\y8d\y89\y8e\y87\ya0\y7f\ya0\yd3\y94\y81\y94\y85\ya0\ya0\y7f\yd4\ya0\ya0\y7f\yd3\yb1\ya0\y7f\yd3\yb2\ya0\y7f\yd3\yb3\ya0";
const char TXT_EN_FARM_FIELD_TXT[] =  s"Field\y3a ";
const char TXT_EN_FARM_PLANT_TXT[] =  s"Growing\y3a";
const char TXT_EN_TASK_FARMLAND_FARM[] =  s"Farm";
const char TXT_EN_TASK_FARMLAND_CARE[] =  s"Care";
const char TXT_EN_TASK_FARMLAND_REAP[] =  s"Reap";
const char TXT_EN_TASK_DSC_FARMLAND_FIELD[] =  s"Field";
const char TXT_EN_TASK_DSC_FARMLAND_POTATO[] =  s"potatoes";
const char TXT_EN_TASK_DSC_FARMLAND_LUPINE[] =  s"lupine";
const char TXT_EN_TASK_DSC_FARMLAND_WHEAT[] =  s"wheat";
const char TXT_EN_TASK_DSC_FARMLAND_CORN[] =  s"corn";
const char TXT_EN_MENU_PIGPEN1[] =  s"\yb1Feed";
const char TXT_EN_MENU_PIGPEN2[] =  s"\yb2Cleanup";
const char TXT_EN_MENU_PIGPEN3[] =  s"\yb3Upgrade";
const char TXT_EN_MENU_SMOKEHOUSE1[] =  s"\yb1Recipies";
const char TXT_EN_MENU_SMOKEHOUSE2[] =  s"\yb2Cleanup";
const char TXT_EN_MENU_SMOKEHOUSE3[] =  s"\yb3Upgrade";
const char TXT_EN_MENU_SHOPOUT1[] =  s"\yb1Go in";
const char TXT_EN_MENU_SHOPOUT2[] =  s"\yb2Chat";
const char TXT_EN_SHOP_DESCRIPTION[] =  s"A general store\y2c open 7-11\y2c then 15-19\y2c everyday";
const char TXT_EN_MENU_SHOPIN1[] =  s"\yb1Buy gherkin";
const char TXT_EN_MENU_SHOPIN2[] =  s"\yb2Chat";
const char TXT_EN_MENU_CREW1[] =  s"\yb1Char 1";
const char TXT_EN_MENU_CREW2[] =  s"\yb2Char 2";
const char TXT_EN_MENU_CREW3[] =  s"\yb3Char 3";
const char TXT_EN_MENU_CREW4[] =  s"\yb4Char 4";
const char TXT_EN_STAT_INT[] =  s"Wisdom";
const char TXT_EN_STAT_STR[] =  s"Strength";
const char TXT_EN_STAT_CUN[] =  s"Cunning";
const char TXT_EN_SKILL_ANI[] =  s"Breeding";
const char TXT_EN_SKILL_FRM[] =  s"Farming";
const char TXT_EN_SKILL_BTH[] =  s"Butchery";
const char TXT_EN_SKILL_TRD[] =  s"Trade";
const char TXT_EN_CHAR_1_STORY[] =  s"Janusz spent his whole life working in PGR\y2e He does not really excel in anything\y2c but thankfully he is lazy and an addict\y2e He is your main character\y2e";
const char TXT_EN_CHAR_2_STORY[] =  s"Zdzisiek is not very popularin the villiage\y2e Folks reme-mber that his father was a volksdeustch\y2e He is bright - like his father - and that makes him stand out\y2e";
const char TXT_EN_CHAR_3_STORY[] =  s"Wiesiek\y27s story will be here\y2e";
const char TXT_EN_CHAR_4_STORY[] =  s"J\y60zek\y27s story will be here\y2e";
const char SB_EN_PAUSE[] =  s"   P  A  U  S  E  ";
const char SB_EN_UNPAUSE[] =  s"   Normal game speed   ";
const char SB_EN_WELCOME[] =  s"    Hello\y21 \y2b\y2f-\y2fSpace - time control    Kielbasa Empire - very early alpha\y2e    Credits\y3a        Code support and compilers\y3a KickC - Rex\y2fCamelot    Oscar64 - DrMortalWombat    Game\y3a Code - Sepa\y2c Gfx - Juras\y2c Model3 \y26 Saturnz\y2c Msx - Taurus\y2c Support - Angry Andy\y2c Fonts - Sepa ";
const char SB_EN_SHOP_CLOSED[] =  s"   Shop is closed   ";

#pragma data ( txtEnPlantTypes )
__export const char* PLANT_TYPES_TXT_EN[] = {
    TXT_EN_PLANT_NAME_NONE,
    TXT_EN_PLANT_NAME_POTATO,
    TXT_EN_PLANT_NAME_LUPINE,
    TXT_EN_PLANT_NAME_WHEAT,
    TXT_EN_PLANT_NAME_CORN
};

#pragma data ( txtEnPlantStateNames )
__export const char* PLANT_STAGE_NAMES_EN[] = {
    TXT_EN_PLANT_STAGE_NONE,
    TXT_EN_PLANT_STAGE_SOW_TASK_ASSIGNED,
    TXT_EN_PLANT_STAGE_SPROUT,
    TXT_EN_PLANT_STAGE_GROWTH,
    TXT_EN_PLANT_STAGE_RIPEN,
    TXT_EN_PLANT_STAGE_READY,
    TXT_EN_PLANT_STAGE_REAP_TASK_ASSIGNED
};

#pragma data ( txtEnMainArray )
__export const char* TXT_EN[] = {
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
    TXT_EN_TASK_MANAGER_PRIO_HEADER,
    TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_1,
    TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_2,
    TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_3,
    TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_4,
    TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_5,
    TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_6,
    TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_7,
    TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_8,
    TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_9,
    TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_10,
    TXT_EN_MENU_TASK_MANAGER_W,
    TXT_EN_MENU_TASK_MANAGER_S,
    TXT_EN_MENU_TASK_MANAGER_A,
    TXT_EN_MENU_TASK_MANAGER_D,
    TXT_EN_MENU_TASK_MANAGER_PLUS,
    TXT_EN_MENU_TASK_MANAGER_MINUS,
    TXT_EN_TASK_MANAGER_HEADER,
    TXT_EN_TASK_MANAGER_TABLE_HEADER,
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

