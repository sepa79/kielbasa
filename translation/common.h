#ifndef COMMON_TRANSLATION_H
#define COMMON_TRANSLATION_H

//
//
// This file is auto-generated by 'create_translations.py' script.
//
//


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
    TXT_IDX_EXIT_TO_MAP,
    TXT_IDX_MENU_OPTIONS_LANG,
    TXT_IDX_MENU_OPTIONS_MSX,
    TXT_IDX_OPTIONS_HEADER,
    TXT_IDX_MUSIC_OPTIONS_HEADER,
    TXT_IDX_MENU_OPTIONS_MSX_PLAY,
    TXT_IDX_MENU_OPTIONS_MSX_ON_OFF,
    TXT_IDX_LOG_MENU_HEADER,
    TXT_IDX_LOG_MENU_LOG_TABLE_HEADER,
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
    TXT_IDX_TASK_KITCHEN,
    TXT_IDX_TASK_DSC_KITCHEN_BAKE_BREAD,
    TXT_IDX_MENU_PIGPEN1,
    TXT_IDX_MENU_PIGPEN2,
    TXT_IDX_MENU_PIGPEN3,
    TXT_IDX_MENU_KITCHEN1,
    TXT_IDX_MENU_KITCHEN2,
    TXT_IDX_MENU_KITCHEN2_ON,
    TXT_IDX_MENU_KITCHEN2_OFF,
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
    SB_IDX_TASK_ADDED,
    SB_IDX_TASKS_FULL,
    SB_IDX_WELCOME,
    HELP_IDX_ABOUT,
    SB_IDX_SHOP_CLOSED
};

//     "pragma_label": MainArray,
//     "array_label": TXT,

//     "common": "        "
static const char TXT_TASK_EMPTY_NAME[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "common": "                  "
static const char TXT_TASK_EMPTY_DESCRIPTION[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//     "common": "À"
static const char TXT_MENU_FARMLAND1[] = {0x1c, 0x00};
//     "common": "÷"
static const char TXT_MENU_FARMLAND2[] = {0x77, 0x00};
//     "common": "["
static const char TXT_MENU_FARMLAND3[] = {0x1b, 0x00};
//     "common": "]"
static const char TXT_MENU_FARMLAND4[] = {0x1d, 0x00};
//     "common": "Janusz Dobrowolski"
static const char TXT_CHAR_1_NAME[] = {0x4a, 0x01, 0x0e, 0x15, 0x13, 0x1a, 0x20, 0x44, 0x0f, 0x02, 0x12, 0x0f, 0x17, 0x0f, 0x0c, 0x13, 0x0b, 0x09, 0x00};
//     "common": "Zdzisław Hermann"
static const char TXT_CHAR_2_NAME[] = {0x5a, 0x04, 0x1a, 0x09, 0x13, 0x5e, 0x01, 0x17, 0x20, 0x48, 0x05, 0x12, 0x0d, 0x01, 0x0e, 0x0e, 0x00};
//     "common": "Wiesław Dobrowolski"
static const char TXT_CHAR_3_NAME[] = {0x57, 0x09, 0x05, 0x13, 0x5e, 0x01, 0x17, 0x20, 0x44, 0x0f, 0x02, 0x12, 0x0f, 0x17, 0x0f, 0x0c, 0x13, 0x0b, 0x09, 0x00};
//     "common": "Józef Kołodziej"
static const char TXT_CHAR_4_NAME[] = {0x4a, 0x60, 0x1a, 0x05, 0x06, 0x20, 0x4b, 0x0f, 0x5e, 0x0f, 0x04, 0x1a, 0x09, 0x05, 0x0a, 0x00};


#endif

