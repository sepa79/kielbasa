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

static const char TXT_MENU_FARMLAND1[] = "<";
static const char TXT_MENU_FARMLAND2[] = ">";
static const char TXT_MENU_FARMLAND3[] = {27,0}; // '['
static const char TXT_MENU_FARMLAND4[] = {29,0}; // ']'

static const char TXT_CHAR_1_NAME[] = p"Janusz Dobrowolski";
static const char TXT_CHAR_2_NAME[] = p"Zdzis\x5eaw Hermann";
static const char TXT_CHAR_3_NAME[] = p"Wies\x5eaw Dobrowolski";
static const char TXT_CHAR_4_NAME[] = p"J\x60zef Ko\x5eodziej";

// fixed width - 8
static const char TXT_TASK_EMPTY_NAME[] = "        ";
// fixed width - 18
static const char TXT_TASK_EMPTY_DESCRIPTION[] = "                  ";

#endif
