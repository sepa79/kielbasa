

#ifndef COMMON_TRANSLATION_H
#define COMMON_TRANSLATION_H

//
//
// This file is auto-generated by 'create_translations_yaml_jinja2.py' script.
//
//


// DrMortalWombat's magic spell to simplify using underlied chars (+$80 in Kielbasa's charset)
#define underline(x) \
""\
#pragma charmap(1, 129, 127)\
x\
#pragma charmap(1, 1, 127)

enum TEXT_INDEXES {
    TXT_IDX_BATTLE_MENU_ATTACK,
    TXT_IDX_BATTLE_MENU_DEFEND,
    TXT_IDX_MENU_EXIT,
    TXT_IDX_EXIT_TO_MAP,
    TXT_IDX_MENU_OPTIONS_LANG,
    TXT_IDX_MENU_OPTIONS_MSX,
    TXT_IDX_OPTIONS_HEADER,
    TXT_IDX_OPTIONS_HELP1,
    TXT_IDX_OPTIONS_HELP2,
    TXT_IDX_OPTIONS_HELP3,
    TXT_IDX_OPTIONS_HELP4,
    TXT_IDX_OPTIONS_HELP5,
    TXT_IDX_MUSIC_OPTIONS_HEADER,
    SB_IDX_MENU_OPTIONS_MSX_MENU,
    TXT_IDX_LOG_MENU_HEADER,
    TXT_IDX_LOG_MENU_LOG_TABLE_HEADER,
    TXT_IDX_MENU_SELECT,
    TXT_IDX_MENU_W,
    TXT_IDX_MENU_S,
    TXT_IDX_MENU_A,
    TXT_IDX_MENU_D,
    TXT_IDX_TASK_MANAGER_HEADER,
    TXT_IDX_TASK_MANAGER_TABLE_HEADER,
    TXT_IDX_FIELD_LIST_HEADER,
    TXT_IDX_FARM_FIELD_TXT,
    TXT_IDX_FARM_PLANT_TXT,
    TXT_IDX_STAT_INT,
    TXT_IDX_STAT_STR,
    TXT_IDX_STAT_CUN,
    TXT_IDX_SKILL_ANI,
    TXT_IDX_SKILL_FRM,
    TXT_IDX_SKILL_BTH,
    TXT_IDX_SKILL_TRD,
    SB_IDX_TASK_ADDED,
    SB_IDX_TASKS_FULL,
    SB_IDX_WELCOME,
    TXT_IDX_CONFIRM_TXT,
    TXT_IDX_CONFIRM_ERROR,
    TXT_IDX_CONFIRM_YES,
    TXT_IDX_CONFIRM_NO,
    TXT_IDX_MENU_CREW1,
    TXT_IDX_MENU_CREW2,
    TXT_IDX_MENU_CREW3,
    TXT_IDX_MENU_CREW4,
    TXT_IDX_CHAR_1_NAME,
    TXT_IDX_CHAR_1_STORY,
    TXT_IDX_CHAR_2_NAME,
    TXT_IDX_CHAR_2_STORY,
    TXT_IDX_CHAR_3_NAME,
    TXT_IDX_CHAR_3_STORY,
    TXT_IDX_CHAR_4_NAME,
    TXT_IDX_CHAR_4_STORY,
    TXT_IDX_MENU_FARMLAND_PLOW,
    TXT_IDX_MENU_FARMLAND_SOW,
    TXT_IDX_MENU_FARMLAND_REAP,
    TXT_IDX_DESC_FARMLAND_PLOW,
    TXT_IDX_DESC_FARMLAND_SOW,
    TXT_IDX_DESC_FARMLAND_REAP,
    TXT_IDX_TASK_FARMLAND_PLOW,
    TXT_IDX_TASK_FARMLAND_FARM,
    TXT_IDX_TASK_FARMLAND_REAP,
    TXT_IDX_TASK_DSC_FARMLAND_FIELD,
    TXT_IDX_TASK_DSC_FARMLAND_POTATO,
    TXT_IDX_TASK_DSC_FARMLAND_LUPINE,
    TXT_IDX_TASK_DSC_FARMLAND_WHEAT,
    TXT_IDX_TASK_DSC_FARMLAND_CORN,
    TXT_IDX_MENU_AIR_DEF,
    TXT_IDX_SAVE_POTATOES,
    TXT_IDX_TASK_LIST_HEADER,
    TXT_IDX_MENU_MAIN1,
    TXT_IDX_MENU_MAIN2,
    TXT_IDX_MENU_MAIN3,
    TXT_IDX_MENU_KITCHEN1,
    TXT_IDX_MENU_KITCHEN2,
    TXT_IDX_MENU_KITCHEN3,
    TXT_IDX_MENU_KITCHEN4,
    TXT_IDX_MENU_KITCHEN5,
    TXT_IDX_MENU_KITCHEN6,
    TXT_IDX_MENU_KITCHEN7,
    SB_IDX_KITCHEN_CANNOT_EAT,
    SB_IDX_TASK_KITCHEN_BAKE_BREAD_STORAGE_FULL,
    SB_IDX_TASK_KITCHEN_BAKE_BREAD_NO_WHEAT,
    TXT_IDX_TASK_KITCHEN,
    TXT_IDX_TASK_DSC_KITCHEN_BAKE_BREAD,
    TXT_IDX_KITCHEN_TASK_OFF,
    TXT_IDX_KITCHEN_TASK_ON,
    TXT_IDX_KITCHEN_EATING_STYLE_LIGHT,
    TXT_IDX_KITCHEN_EATING_STYLE_MEDIUM,
    TXT_IDX_KITCHEN_EATING_STYLE_FULL,
    TXT_IDX_KITCHEN_PREFER_SHOP,
    TXT_IDX_KITCHEN_PREFER_HOME,
    TXT_IDX_KITCHEN_PREFER_POTATO,
    TXT_IDX_KITCHEN_PREFER_CORN,
    TXT_IDX_KITCHEN_MISSING,
    TXT_IDX_KITCHEN_MISSING_BREAD,
    TXT_IDX_KITCHEN_MISSING_MEAT,
    TXT_IDX_KITCHEN_MISSING_VEG,
    TXT_IDX_MENU_PIGPEN1,
    TXT_IDX_MENU_PIGPEN2,
    TXT_IDX_MENU_PIGPEN3,
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
    TXT_IDX_MENU_TASK_MANAGER_PLUS,
    TXT_IDX_MENU_TASK_MANAGER_MINUS,
    TXT_IDX_MENU_SHOPIN1,
    TXT_IDX_MENU_SHOPIN2,
    TXT_IDX_MENU_SHOPIN3,
    TXT_IDX_MENU_SHOPIN4,
    SB_IDX_SHOP_CLOSED,
    TXT_IDX_SHOP_DESCRIPTION,
    TXT_IDX_MENU_SHOPOUT1,
    TXT_IDX_MENU_SHOPOUT2,
    TXT_IDX_TASK_EMPTY_NAME,
    TXT_IDX_TASK_EMPTY_DESCRIPTION,
    SB_IDX_MENU_TV_SCREEN,
    TV_IDX_MENU_TV_SCREEN_BILLS
};


//      "pragma_label": MainArray
//      "array_label": TXT


//      common: 'Janusz Dobrowolski'
static const char TXT_CHAR_1_NAME[] = {0x4a, 0x01, 0x0e, 0x15, 0x13, 0x1a, 0x20, 0x44, 0x0f, 0x02, 0x12, 0x0f, 0x17, 0x0f, 0x0c, 0x13, 0x0b, 0x09, 0x00};
//      common: 'Zdzisław Hermann'
static const char TXT_CHAR_2_NAME[] = {0x5a, 0x04, 0x1a, 0x09, 0x13, 0x5e, 0x01, 0x17, 0x20, 0x48, 0x05, 0x12, 0x0d, 0x01, 0x0e, 0x0e, 0x00};
//      common: 'Wiesław C.'
static const char TXT_CHAR_3_NAME[] = {0x57, 0x09, 0x05, 0x13, 0x5e, 0x01, 0x17, 0x20, 0x43, 0x2e, 0x00};
//      common: 'Józef Kołodziej'
static const char TXT_CHAR_4_NAME[] = {0x4a, 0x60, 0x1a, 0x05, 0x06, 0x20, 0x4b, 0x0f, 0x5e, 0x0f, 0x04, 0x1a, 0x09, 0x05, 0x0a, 0x00};
//      common: '        '
static const char TXT_TASK_EMPTY_NAME[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
//      common: '                  '
static const char TXT_TASK_EMPTY_DESCRIPTION[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};


#endif
