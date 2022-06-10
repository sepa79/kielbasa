#ifndef COMMON_TRANSLATION_H
#define COMMON_TRANSLATION_H

// DrMortalWombat's magic spell to simplify using underlied chars (+$80 in Kielbasa's charset)
#define underline(x) \
""\
#pragma charmap(1, 129, 127)\
x\
#pragma charmap(1, 1, 127)

enum TEXT_INDEXES {

    TXT_IDX_MENU_CANCEL,
    TXT_IDX_MENU_EXIT,
    TXT_IDX_TASK_EMPTY_NAME,
    TXT_IDX_TASK_EMPTY_DESCRIPTION,
    TXT_IDX_TASK_LIST_HEADER,
    TXT_IDX_MENU_MAIN1,
    TXT_IDX_MENU_MAIN2,
    TXT_IDX_MENU_MAIN3,
    TXT_IDX_MENU_MAIN4,
    TXT_IDX_MENU_MAIN5,
    TXT_IDX_MENU_OPTIONS_LANG,
    TXT_IDX_MENU_OPTIONS_MSX,
    TXT_IDX_OPTIONS_HEADER,
    TXT_IDX_TASK_MANAGER_PRIO_HEADER,
    TXT_IDX_TASK_MANAGER_PRIO_TABLE_HEADER_1,
    TXT_IDX_TASK_MANAGER_PRIO_TABLE_HEADER_2,
    TXT_IDX_TASK_MANAGER_PRIO_TABLE_HEADER_3,
    TXT_IDX_TASK_MANAGER_PRIO_TABLE_HEADER_4,
    TXT_IDX_TASK_MANAGER_PRIO_TABLE_HEADER_5,
    TXT_IDX_TASK_MANAGER_PRIO_TABLE_HEADER_6,
    TXT_IDX_TASK_MANAGER_PRIO_TABLE_HEADER_7,
    TXT_IDX_TASK_MANAGER_PRIO_TABLE_HEADER_8,
    TXT_IDX_TASK_MANAGER_PRIO_TABLE_HEADER_9,
    TXT_IDX_TASK_MANAGER_PRIO_TABLE_HEADER_10,
    TXT_IDX_MENU_TASK_MANAGER_W,
    TXT_IDX_MENU_TASK_MANAGER_S,
    TXT_IDX_MENU_TASK_MANAGER_A,
    TXT_IDX_MENU_TASK_MANAGER_D,
    TXT_IDX_MENU_TASK_MANAGER_PLUS,
    TXT_IDX_MENU_TASK_MANAGER_MINUS,
    TXT_IDX_TASK_MANAGER_HEADER,
    TXT_IDX_TASK_MANAGER_TABLE_HEADER,
    TXT_IDX_MENU_FARMLAND1,
    TXT_IDX_MENU_FARMLAND2,
    TXT_IDX_MENU_FARMLAND3,
    TXT_IDX_MENU_FARMLAND4,
    TXT_IDX_MENU_FARMLAND5,
    TXT_IDX_MENU_FARMLAND6,
    TXT_IDX_MENU_FARMLAND7,
    TXT_IDX_FIELD_LIST_HEADER,
    TXT_IDX_FARM_FIELD_TXT,
    TXT_IDX_FARM_PLANT_TXT,
    TXT_IDX_TASK_FARMLAND_FARM,
    TXT_IDX_TASK_FARMLAND_CARE,
    TXT_IDX_TASK_FARMLAND_REAP,
    TXT_IDX_TASK_DSC_FARMLAND_FIELD,
    TXT_IDX_TASK_DSC_FARMLAND_POTATO,
    TXT_IDX_TASK_DSC_FARMLAND_LUPINE,
    TXT_IDX_TASK_DSC_FARMLAND_WHEAT,
    TXT_IDX_TASK_DSC_FARMLAND_CORN,
    TXT_IDX_MENU_PIGPEN1,
    TXT_IDX_MENU_PIGPEN2,
    TXT_IDX_MENU_PIGPEN3,
    TXT_IDX_MENU_SMOKEHOUSE1,
    TXT_IDX_MENU_SMOKEHOUSE2,
    TXT_IDX_MENU_SMOKEHOUSE3,
    TXT_IDX_MENU_SHOPOUT1,
    TXT_IDX_MENU_SHOPOUT2,
    TXT_IDX_SHOP_DESCRIPTION,
    TXT_IDX_MENU_SHOPIN1,
    TXT_IDX_MENU_SHOPIN2,
    TXT_IDX_MENU_CREW1,
    TXT_IDX_MENU_CREW2,
    TXT_IDX_MENU_CREW3,
    TXT_IDX_MENU_CREW4,
    TXT_IDX_STAT_INT,
    TXT_IDX_STAT_STR,
    TXT_IDX_STAT_CUN,
    TXT_IDX_SKILL_ANI,
    TXT_IDX_SKILL_FRM,
    TXT_IDX_SKILL_BTH,
    TXT_IDX_SKILL_TRD,
    TXT_IDX_CHAR_1_NAME,
    TXT_IDX_CHAR_1_STORY,
    TXT_IDX_CHAR_2_NAME,
    TXT_IDX_CHAR_2_STORY,
    TXT_IDX_CHAR_3_NAME,
    TXT_IDX_CHAR_3_STORY,
    TXT_IDX_CHAR_4_NAME,
    TXT_IDX_CHAR_4_STORY,
    SB_IDX_PAUSE,
    SB_IDX_UNPAUSE,
    SB_IDX_WELCOME,
    SB_IDX_SHOP_CLOSED
};

static const char TXT_TASK_EMPTY_NAME[] = s"        ";
static const char TXT_TASK_EMPTY_DESCRIPTION[] = s"                  ";
static const char TXT_MENU_FARMLAND1[] = s"\y3c";
static const char TXT_MENU_FARMLAND2[] = s"\y3e";
static const char TXT_MENU_FARMLAND3[] = s"\y1b";
static const char TXT_MENU_FARMLAND4[] = s"\y1d";
static const char TXT_CHAR_1_NAME[] = s"Janusz Dobrowolski";
static const char TXT_CHAR_2_NAME[] = s"Zdzis\y5eaw Hermann";
static const char TXT_CHAR_3_NAME[] = s"Wies\y5eaw Dobrowolski";
static const char TXT_CHAR_4_NAME[] = s"J\y60zef Ko\y5eodziej";


#endif

