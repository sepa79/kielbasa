#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>

#pragma data ( txtEnTxtData )

const char TXT_EN_PLANT_NAME_NONE[] = {0x2d, 0x20, 0x2d, 0x20, 0x2d, 0x20, 0x2d, 0x20, 0x2d, 0x20, 0x00};
const char TXT_EN_PLANT_NAME_POTATO[] = {0x50, 0x0f, 0x14, 0x01, 0x14, 0x0f, 0x05, 0x13, 0x20, 0x20, 0x00};
const char TXT_EN_PLANT_NAME_LUPINE[] = {0x4c, 0x15, 0x10, 0x09, 0x0e, 0x05, 0x20, 0x20, 0x20, 0x20, 0x00};
const char TXT_EN_PLANT_NAME_WHEAT[] = {0x57, 0x08, 0x05, 0x01, 0x14, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
const char TXT_EN_PLANT_NAME_CORN[] = {0x43, 0x0f, 0x12, 0x0e, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
const char TXT_EN_PLANT_STAGE_NONE[] = {0x2d, 0x20, 0x2d, 0x20, 0x2d, 0x20, 0x2d, 0x20, 0x00};
const char TXT_EN_PLANT_STAGE_SOW_TASK_ASSIGNED[] = {0x54, 0x3a, 0x20, 0x50, 0x0c, 0x01, 0x0e, 0x14, 0x00};
const char TXT_EN_PLANT_STAGE_SPROUT[] = {0x53, 0x10, 0x12, 0x0f, 0x15, 0x14, 0x13, 0x00};
const char TXT_EN_PLANT_STAGE_GROWTH[] = {0x47, 0x12, 0x0f, 0x17, 0x13, 0x00};
const char TXT_EN_PLANT_STAGE_RIPEN[] = {0x52, 0x09, 0x10, 0x05, 0x0e, 0x13, 0x00};
const char TXT_EN_PLANT_STAGE_READY[] = {0x52, 0x05, 0x01, 0x04, 0x19, 0x00};
const char TXT_EN_PLANT_STAGE_REAP_TASK_ASSIGNED[] = {0x54, 0x3a, 0x20, 0x52, 0x05, 0x01, 0x10, 0x00};
const char TXT_EN_MENU_CANCEL[] = {0x1f, 0x43, 0x01, 0x0e, 0x03, 0x05, 0x0c, 0x00};
const char TXT_EN_MENU_EXIT[] = {0x1f, 0x42, 0x01, 0x03, 0x0b, 0x00};
const char TXT_EN_TASK_LIST_HEADER[] = {0xa0, 0xa0, 0xd4, 0x81, 0x93, 0x8b, 0xa0, 0xa0, 0xfe, 0xa0, 0xa0, 0xa0, 0xa0, 0xc4, 0x85, 0x93, 0x83, 0x92, 0x89, 0x90, 0x94, 0x89, 0x8f, 0x8e, 0xa0, 0xa0, 0xa0, 0xa0, 0x00};
const char TXT_EN_MENU_MAIN1[] = {0xf1, 0x46, 0x01, 0x12, 0x0d, 0x0c, 0x01, 0x0e, 0x04, 0x00};
const char TXT_EN_MENU_MAIN2[] = {0xf2, 0x50, 0x09, 0x07, 0x10, 0x05, 0x0e, 0x00};
const char TXT_EN_MENU_MAIN3[] = {0xf3, 0x53, 0x0d, 0x0f, 0x0b, 0x05, 0x08, 0x0f, 0x15, 0x13, 0x05, 0x00};
const char TXT_EN_MENU_MAIN4[] = {0xf4, 0x53, 0x08, 0x0f, 0x10, 0x00};
const char TXT_EN_MENU_MAIN5[] = {0xf5, 0x43, 0x12, 0x05, 0x17, 0x00};
const char TXT_EN_MENU_MAIN6[] = {0xf6, 0x50, 0x09, 0x07, 0x13, 0x09, 0x0c, 0x05, 0x20, 0x43, 0x0f, 0x0d, 0x0d, 0x01, 0x0e, 0x04, 0x00};
const char TXT_EN_MENU_OPTIONS_LANG[] = {0xf1, 0x4c, 0x01, 0x0e, 0x07, 0x15, 0x01, 0x07, 0x05, 0x20, 0x2f, 0x20, 0x4a, 0x5d, 0x1a, 0x19, 0x0b, 0x00};
const char TXT_EN_MENU_OPTIONS_MSX[] = {0xf2, 0x4d, 0x15, 0x13, 0x09, 0x03, 0x20, 0x0f, 0x0e, 0x2f, 0x0f, 0x06, 0x06, 0x00};
const char TXT_EN_OPTIONS_HEADER[] = {0x47, 0x01, 0x0d, 0x05, 0x20, 0x13, 0x05, 0x14, 0x14, 0x09, 0x0e, 0x07, 0x13, 0x00};
const char TXT_EN_TASK_MANAGER_PRIO_HEADER[] = {0x54, 0x01, 0x13, 0x0b, 0x20, 0x50, 0x12, 0x09, 0x0f, 0x12, 0x09, 0x14, 0x19, 0x20, 0x4d, 0x01, 0x0e, 0x01, 0x07, 0x05, 0x12, 0x00};
const char TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_1[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xfe, 0xd3, 0x8b, 0x89, 0x8c, 0x8c, 0xa0, 0xa0, 0xa0, 0xfe, 0xa0, 0xd4, 0x81, 0x93, 0x8b, 0xa0, 0xa0, 0xa0, 0x00};
const char TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_2[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7e, 0x20, 0x7e, 0x20, 0x7e, 0x20, 0x7e, 0x20, 0x20, 0x7e, 0x20, 0x20, 0x7e, 0x20, 0x7e, 0x20, 0x7e, 0x20, 0x00};
const char TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_3[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7e, 0x42, 0x7e, 0x20, 0x7e, 0x42, 0x7e, 0x20, 0x20, 0x7e, 0x20, 0x42, 0x7e, 0x20, 0x7e, 0x42, 0x7e, 0x20, 0x00};
const char TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_4[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7e, 0x12, 0x7e, 0x46, 0x7e, 0x15, 0x7e, 0x20, 0x20, 0x7e, 0x20, 0x12, 0x7e, 0x46, 0x7e, 0x15, 0x7e, 0x20, 0x00};
const char TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_5[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7e, 0x05, 0x7e, 0x01, 0x7e, 0x14, 0x7e, 0x20, 0x20, 0x7e, 0x20, 0x05, 0x7e, 0x01, 0x7e, 0x14, 0x7e, 0x20, 0x00};
const char TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_6[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7e, 0x05, 0x7e, 0x12, 0x7e, 0x03, 0x7e, 0x54, 0x20, 0x7e, 0x20, 0x05, 0x7e, 0x12, 0x7e, 0x03, 0x7e, 0x54, 0x00};
const char TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_7[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7e, 0x04, 0x7e, 0x0d, 0x7e, 0x08, 0x7e, 0x12, 0x20, 0x7e, 0x20, 0x04, 0x7e, 0x0d, 0x7e, 0x08, 0x7e, 0x12, 0x00};
const char TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_8[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7e, 0x09, 0x7e, 0x09, 0x7e, 0x05, 0x7e, 0x01, 0x20, 0x7e, 0x20, 0x09, 0x7e, 0x09, 0x7e, 0x05, 0x7e, 0x01, 0x00};
const char TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_9[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7e, 0x0e, 0x7e, 0x0e, 0x7e, 0x12, 0x7e, 0x04, 0x20, 0x7e, 0x20, 0x0e, 0x7e, 0x0e, 0x7e, 0x12, 0x7e, 0x04, 0x00};
const char TXT_EN_TASK_MANAGER_PRIO_TABLE_HEADER_10[] = {0xc3, 0x88, 0x81, 0x92, 0x81, 0x83, 0x94, 0x85, 0x92, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xfe, 0x87, 0xfe, 0x87, 0xfe, 0x99, 0xfe, 0x85, 0xa0, 0xfe, 0xa0, 0x87, 0xfe, 0x87, 0xfe, 0x99, 0xfe, 0x85, 0x00};
const char TXT_EN_MENU_TASK_MANAGER_W[] = {0x17, 0x00};
const char TXT_EN_MENU_TASK_MANAGER_S[] = {0x13, 0x00};
const char TXT_EN_MENU_TASK_MANAGER_A[] = {0x01, 0x00};
const char TXT_EN_MENU_TASK_MANAGER_D[] = {0x04, 0x00};
const char TXT_EN_MENU_TASK_MANAGER_PLUS[] = {0x2b, 0x00};
const char TXT_EN_MENU_TASK_MANAGER_MINUS[] = {0x2d, 0x00};
const char TXT_EN_TASK_MANAGER_HEADER[] = {0x54, 0x01, 0x13, 0x0b, 0x20, 0x4d, 0x01, 0x0e, 0x01, 0x07, 0x05, 0x12, 0x00};
const char TXT_EN_TASK_MANAGER_TABLE_HEADER[] = {0xa0, 0xa0, 0xd4, 0x81, 0x93, 0x8b, 0xa0, 0xa0, 0xfe, 0xa0, 0xa0, 0xa0, 0xa0, 0xc4, 0x85, 0x93, 0x83, 0x92, 0x89, 0x90, 0x94, 0x89, 0x8f, 0x8e, 0xa0, 0xa0, 0xa0, 0xa0, 0x00};
const char TXT_EN_MENU_FARMLAND5[] = {0xf1, 0x47, 0x12, 0x0f, 0x17, 0x00};
const char TXT_EN_MENU_FARMLAND6[] = {0xf2, 0x43, 0x01, 0x12, 0x05, 0x00};
const char TXT_EN_MENU_FARMLAND7[] = {0xf3, 0x52, 0x05, 0x01, 0x10, 0x00};
const char TXT_EN_FIELD_LIST_HEADER[] = {0xcd, 0x87, 0xfe, 0xa0, 0xc6, 0x81, 0x92, 0x8d, 0x89, 0x8e, 0x87, 0xa0, 0xfe, 0xa0, 0xd3, 0x94, 0x81, 0x94, 0x85, 0xa0, 0xa0, 0xfe, 0xd4, 0xa0, 0xa0, 0xfe, 0xd3, 0xb1, 0xa0, 0xfe, 0xd3, 0xb2, 0xa0, 0xfe, 0xd3, 0xb3, 0xa0, 0x00};
const char TXT_EN_FARM_FIELD_TXT[] = {0x46, 0x09, 0x05, 0x0c, 0x04, 0x3a, 0x20, 0x00};
const char TXT_EN_FARM_PLANT_TXT[] = {0x47, 0x12, 0x0f, 0x17, 0x09, 0x0e, 0x07, 0x3a, 0x00};
const char TXT_EN_TASK_FARMLAND_FARM[] = {0x46, 0x01, 0x12, 0x0d, 0x00};
const char TXT_EN_TASK_FARMLAND_CARE[] = {0x43, 0x01, 0x12, 0x05, 0x00};
const char TXT_EN_TASK_FARMLAND_REAP[] = {0x52, 0x05, 0x01, 0x10, 0x00};
const char TXT_EN_TASK_DSC_FARMLAND_FIELD[] = {0x46, 0x09, 0x05, 0x0c, 0x04, 0x00};
const char TXT_EN_TASK_DSC_FARMLAND_POTATO[] = {0x10, 0x0f, 0x14, 0x01, 0x14, 0x0f, 0x05, 0x13, 0x00};
const char TXT_EN_TASK_DSC_FARMLAND_LUPINE[] = {0x0c, 0x15, 0x10, 0x09, 0x0e, 0x05, 0x00};
const char TXT_EN_TASK_DSC_FARMLAND_WHEAT[] = {0x17, 0x08, 0x05, 0x01, 0x14, 0x00};
const char TXT_EN_TASK_DSC_FARMLAND_CORN[] = {0x03, 0x0f, 0x12, 0x0e, 0x00};
const char TXT_EN_MENU_PIGPEN1[] = {0xf1, 0x46, 0x05, 0x05, 0x04, 0x00};
const char TXT_EN_MENU_PIGPEN2[] = {0xf2, 0x43, 0x0c, 0x05, 0x01, 0x0e, 0x15, 0x10, 0x00};
const char TXT_EN_MENU_PIGPEN3[] = {0xf3, 0x55, 0x10, 0x07, 0x12, 0x01, 0x04, 0x05, 0x00};
const char TXT_EN_MENU_SMOKEHOUSE1[] = {0xf1, 0x52, 0x05, 0x03, 0x09, 0x10, 0x05, 0x13, 0x00};
const char TXT_EN_MENU_SMOKEHOUSE2[] = {0xf2, 0x43, 0x0c, 0x05, 0x01, 0x0e, 0x15, 0x10, 0x00};
const char TXT_EN_MENU_SMOKEHOUSE3[] = {0xf3, 0x55, 0x10, 0x07, 0x12, 0x01, 0x04, 0x05, 0x00};
const char TXT_EN_MENU_SHOPOUT1[] = {0xf1, 0x47, 0x0f, 0x20, 0x09, 0x0e, 0x00};
const char TXT_EN_MENU_SHOPOUT2[] = {0xf2, 0x43, 0x08, 0x01, 0x14, 0x00};
const char TXT_EN_SHOP_DESCRIPTION[] = {0x41, 0x20, 0x07, 0x05, 0x0e, 0x05, 0x12, 0x01, 0x0c, 0x20, 0x13, 0x14, 0x0f, 0x12, 0x05, 0x2c, 0x20, 0x0f, 0x10, 0x05, 0x0e, 0x20, 0x37, 0x2d, 0x31, 0x31, 0x2c, 0x20, 0x14, 0x08, 0x05, 0x0e, 0x20, 0x31, 0x35, 0x2d, 0x31, 0x39, 0x2c, 0x20, 0x05, 0x16, 0x05, 0x12, 0x19, 0x04, 0x01, 0x19, 0x00};
const char TXT_EN_MENU_SHOPIN1[] = {0xf1, 0x42, 0x15, 0x19, 0x20, 0x07, 0x08, 0x05, 0x12, 0x0b, 0x09, 0x0e, 0x00};
const char TXT_EN_MENU_SHOPIN2[] = {0xf2, 0x43, 0x08, 0x01, 0x14, 0x00};
const char TXT_EN_MENU_CREW1[] = {0xf1, 0x43, 0x08, 0x01, 0x12, 0x20, 0x31, 0x00};
const char TXT_EN_MENU_CREW2[] = {0xf2, 0x43, 0x08, 0x01, 0x12, 0x20, 0x32, 0x00};
const char TXT_EN_MENU_CREW3[] = {0xf3, 0x43, 0x08, 0x01, 0x12, 0x20, 0x33, 0x00};
const char TXT_EN_MENU_CREW4[] = {0xf4, 0x43, 0x08, 0x01, 0x12, 0x20, 0x34, 0x00};
const char TXT_EN_STAT_INT[] = {0x57, 0x09, 0x13, 0x04, 0x0f, 0x0d, 0x00};
const char TXT_EN_STAT_STR[] = {0x53, 0x14, 0x12, 0x05, 0x0e, 0x07, 0x14, 0x08, 0x00};
const char TXT_EN_STAT_CUN[] = {0x43, 0x15, 0x0e, 0x0e, 0x09, 0x0e, 0x07, 0x00};
const char TXT_EN_SKILL_ANI[] = {0x42, 0x12, 0x05, 0x05, 0x04, 0x09, 0x0e, 0x07, 0x00};
const char TXT_EN_SKILL_FRM[] = {0x46, 0x01, 0x12, 0x0d, 0x09, 0x0e, 0x07, 0x00};
const char TXT_EN_SKILL_BTH[] = {0x42, 0x15, 0x14, 0x03, 0x08, 0x05, 0x12, 0x19, 0x00};
const char TXT_EN_SKILL_TRD[] = {0x54, 0x12, 0x01, 0x04, 0x05, 0x00};
const char TXT_EN_CHAR_1_STORY[] = {0x4a, 0x01, 0x0e, 0x15, 0x13, 0x1a, 0x20, 0x13, 0x10, 0x05, 0x0e, 0x14, 0x20, 0x08, 0x09, 0x13, 0x20, 0x17, 0x08, 0x0f, 0x0c, 0x05, 0x20, 0x0c, 0x09, 0x06, 0x05, 0x20, 0x17, 0x0f, 0x12, 0x0b, 0x09, 0x0e, 0x07, 0x20, 0x09, 0x0e, 0x20, 0x50, 0x47, 0x52, 0x2e, 0x20, 0x48, 0x05, 0x20, 0x04, 0x0f, 0x05, 0x13, 0x20, 0x0e, 0x0f, 0x14, 0x20, 0x12, 0x05, 0x01, 0x0c, 0x0c, 0x19, 0x20, 0x05, 0x18, 0x03, 0x05, 0x0c, 0x20, 0x09, 0x0e, 0x20, 0x01, 0x0e, 0x19, 0x14, 0x08, 0x09, 0x0e, 0x07, 0x2c, 0x20, 0x02, 0x15, 0x14, 0x20, 0x14, 0x08, 0x01, 0x0e, 0x0b, 0x06, 0x15, 0x0c, 0x0c, 0x19, 0x20, 0x08, 0x05, 0x20, 0x09, 0x13, 0x20, 0x0c, 0x01, 0x1a, 0x19, 0x20, 0x01, 0x0e, 0x04, 0x20, 0x01, 0x0e, 0x20, 0x01, 0x04, 0x04, 0x09, 0x03, 0x14, 0x2e, 0x20, 0x48, 0x05, 0x20, 0x09, 0x13, 0x20, 0x19, 0x0f, 0x15, 0x12, 0x20, 0x0d, 0x01, 0x09, 0x0e, 0x20, 0x03, 0x08, 0x01, 0x12, 0x01, 0x03, 0x14, 0x05, 0x12, 0x2e, 0x00};
const char TXT_EN_CHAR_2_STORY[] = {0x5a, 0x04, 0x1a, 0x09, 0x13, 0x09, 0x05, 0x0b, 0x20, 0x09, 0x13, 0x20, 0x0e, 0x0f, 0x14, 0x20, 0x16, 0x05, 0x12, 0x19, 0x20, 0x10, 0x0f, 0x10, 0x15, 0x0c, 0x01, 0x12, 0x09, 0x0e, 0x20, 0x14, 0x08, 0x05, 0x20, 0x16, 0x09, 0x0c, 0x0c, 0x09, 0x01, 0x07, 0x05, 0x2e, 0x20, 0x46, 0x0f, 0x0c, 0x0b, 0x13, 0x20, 0x12, 0x05, 0x0d, 0x05, 0x2d, 0x0d, 0x02, 0x05, 0x12, 0x20, 0x14, 0x08, 0x01, 0x14, 0x20, 0x08, 0x09, 0x13, 0x20, 0x06, 0x01, 0x14, 0x08, 0x05, 0x12, 0x20, 0x17, 0x01, 0x13, 0x20, 0x01, 0x20, 0x16, 0x0f, 0x0c, 0x0b, 0x13, 0x04, 0x05, 0x15, 0x13, 0x14, 0x03, 0x08, 0x2e, 0x20, 0x48, 0x05, 0x20, 0x09, 0x13, 0x20, 0x02, 0x12, 0x09, 0x07, 0x08, 0x14, 0x20, 0x2d, 0x20, 0x0c, 0x09, 0x0b, 0x05, 0x20, 0x08, 0x09, 0x13, 0x20, 0x06, 0x01, 0x14, 0x08, 0x05, 0x12, 0x20, 0x2d, 0x20, 0x01, 0x0e, 0x04, 0x20, 0x14, 0x08, 0x01, 0x14, 0x20, 0x0d, 0x01, 0x0b, 0x05, 0x13, 0x20, 0x08, 0x09, 0x0d, 0x20, 0x13, 0x14, 0x01, 0x0e, 0x04, 0x20, 0x0f, 0x15, 0x14, 0x2e, 0x00};
const char TXT_EN_CHAR_3_STORY[] = {0x57, 0x09, 0x05, 0x13, 0x09, 0x05, 0x0b, 0x27, 0x13, 0x20, 0x13, 0x14, 0x0f, 0x12, 0x19, 0x20, 0x17, 0x09, 0x0c, 0x0c, 0x20, 0x02, 0x05, 0x20, 0x08, 0x05, 0x12, 0x05, 0x2e, 0x00};
const char TXT_EN_CHAR_4_STORY[] = {0x4a, 0x60, 0x1a, 0x05, 0x0b, 0x27, 0x13, 0x20, 0x13, 0x14, 0x0f, 0x12, 0x19, 0x20, 0x17, 0x09, 0x0c, 0x0c, 0x20, 0x02, 0x05, 0x20, 0x08, 0x05, 0x12, 0x05, 0x2e, 0x00};
const char SB_EN_PAUSE[] = {0x20, 0x20, 0x20, 0x50, 0x20, 0x20, 0x41, 0x20, 0x20, 0x55, 0x20, 0x20, 0x53, 0x20, 0x20, 0x45, 0x20, 0x20, 0x00};
const char SB_EN_UNPAUSE[] = {0x20, 0x20, 0x20, 0x4e, 0x0f, 0x12, 0x0d, 0x01, 0x0c, 0x20, 0x07, 0x01, 0x0d, 0x05, 0x20, 0x13, 0x10, 0x05, 0x05, 0x04, 0x20, 0x20, 0x20, 0x00};
const char SB_EN_WELCOME[] = {0x20, 0x20, 0x20, 0x20, 0x48, 0x05, 0x0c, 0x0c, 0x0f, 0x21, 0x20, 0x2b, 0x2f, 0x2d, 0x2f, 0x53, 0x10, 0x01, 0x03, 0x05, 0x20, 0x2d, 0x20, 0x14, 0x09, 0x0d, 0x05, 0x20, 0x03, 0x0f, 0x0e, 0x14, 0x12, 0x0f, 0x0c, 0x20, 0x20, 0x20, 0x20, 0x4b, 0x09, 0x05, 0x0c, 0x02, 0x01, 0x13, 0x01, 0x20, 0x45, 0x0d, 0x10, 0x09, 0x12, 0x05, 0x20, 0x2d, 0x20, 0x16, 0x05, 0x12, 0x19, 0x20, 0x05, 0x01, 0x12, 0x0c, 0x19, 0x20, 0x01, 0x0c, 0x10, 0x08, 0x01, 0x2e, 0x20, 0x20, 0x20, 0x20, 0x20, 0x43, 0x12, 0x05, 0x04, 0x09, 0x14, 0x13, 0x3a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x43, 0x0f, 0x04, 0x05, 0x20, 0x13, 0x15, 0x10, 0x10, 0x0f, 0x12, 0x14, 0x20, 0x01, 0x0e, 0x04, 0x20, 0x03, 0x0f, 0x0d, 0x10, 0x09, 0x0c, 0x05, 0x12, 0x3a, 0x20, 0x4f, 0x13, 0x03, 0x01, 0x12, 0x36, 0x34, 0x20, 0x2d, 0x20, 0x44, 0x12, 0x4d, 0x0f, 0x12, 0x14, 0x01, 0x0c, 0x57, 0x0f, 0x0d, 0x02, 0x01, 0x14, 0x20, 0x20, 0x20, 0x20, 0x20, 0x47, 0x01, 0x0d, 0x05, 0x3a, 0x20, 0x43, 0x0f, 0x04, 0x05, 0x20, 0x2d, 0x20, 0x53, 0x05, 0x10, 0x01, 0x2c, 0x20, 0x4b, 0x0f, 0x16, 0x01, 0x0c, 0x2c, 0x20, 0x47, 0x06, 0x18, 0x20, 0x2d, 0x20, 0x4a, 0x15, 0x12, 0x01, 0x13, 0x2c, 0x20, 0x4d, 0x0f, 0x04, 0x05, 0x0c, 0x33, 0x20, 0x26, 0x20, 0x53, 0x01, 0x14, 0x15, 0x12, 0x0e, 0x1a, 0x2c, 0x20, 0x4d, 0x13, 0x18, 0x20, 0x2d, 0x20, 0x54, 0x01, 0x15, 0x12, 0x15, 0x13, 0x2c, 0x20, 0x53, 0x15, 0x10, 0x10, 0x0f, 0x12, 0x14, 0x20, 0x2d, 0x20, 0x41, 0x0e, 0x07, 0x12, 0x19, 0x20, 0x41, 0x0e, 0x04, 0x19, 0x2c, 0x20, 0x46, 0x0f, 0x0e, 0x14, 0x13, 0x20, 0x2d, 0x20, 0x53, 0x05, 0x10, 0x01, 0x20, 0x20, 0x20, 0x20, 0x20, 0x53, 0x10, 0x05, 0x03, 0x09, 0x01, 0x0c, 0x20, 0x14, 0x08, 0x01, 0x0e, 0x0b, 0x13, 0x20, 0x06, 0x0f, 0x12, 0x20, 0x4b, 0x09, 0x03, 0x0b, 0x43, 0x20, 0x13, 0x15, 0x10, 0x10, 0x0f, 0x12, 0x14, 0x3a, 0x20, 0x4b, 0x09, 0x03, 0x0b, 0x43, 0x20, 0x2d, 0x20, 0x52, 0x05, 0x18, 0x2f, 0x43, 0x01, 0x0d, 0x05, 0x0c, 0x0f, 0x14, 0x2c, 0x00};
const char SB_EN_SHOP_CLOSED[] = {0x20, 0x20, 0x20, 0x53, 0x08, 0x0f, 0x10, 0x20, 0x09, 0x13, 0x20, 0x03, 0x0c, 0x0f, 0x13, 0x05, 0x04, 0x20, 0x20, 0x20, 0x00};

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
    TXT_EN_MENU_MAIN6,
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

