#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>

#pragma data ( txtPlTxtData )

const char TXT_PL_PLANT_NAME_NONE[] = {0x20, 0x2d, 0x20, 0x2d, 0x20, 0x2d, 0x20, 0x2d, 0x20, 0x2d, 0x00};
const char TXT_PL_PLANT_NAME_POTATO[] = {0x5a, 0x09, 0x05, 0x0d, 0x0e, 0x09, 0x01, 0x0b, 0x09, 0x20, 0x00};
const char TXT_PL_PLANT_NAME_LUPINE[] = {0x67, 0x15, 0x02, 0x09, 0x0e, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
const char TXT_PL_PLANT_NAME_WHEAT[] = {0x50, 0x13, 0x1a, 0x05, 0x0e, 0x09, 0x03, 0x01, 0x20, 0x20, 0x00};
const char TXT_PL_PLANT_NAME_CORN[] = {0x4b, 0x15, 0x0b, 0x15, 0x12, 0x19, 0x04, 0x1a, 0x01, 0x20, 0x00};
const char TXT_PL_PLANT_STAGE_NONE[] = {0x20, 0x2d, 0x20, 0x2d, 0x20, 0x2d, 0x20, 0x2d, 0x00};
const char TXT_PL_PLANT_STAGE_SOW_TASK_ASSIGNED[] = {0x5a, 0x3a, 0x20, 0x53, 0x09, 0x05, 0x17, 0x20, 0x00};
const char TXT_PL_PLANT_STAGE_SPROUT[] = {0x4b, 0x09, 0x05, 0x5e, 0x0b, 0x15, 0x0a, 0x05, 0x00};
const char TXT_PL_PLANT_STAGE_GROWTH[] = {0x52, 0x0f, 0x61, 0x0e, 0x09, 0x05, 0x00};
const char TXT_PL_PLANT_STAGE_RIPEN[] = {0x44, 0x0f, 0x0a, 0x12, 0x1a, 0x05, 0x17, 0x01, 0x00};
const char TXT_PL_PLANT_STAGE_READY[] = {0x44, 0x0f, 0x0a, 0x12, 0x1a, 0x01, 0x5e, 0x19, 0x00};
const char TXT_PL_PLANT_STAGE_REAP_TASK_ASSIGNED[] = {0x5a, 0x3a, 0x20, 0x5a, 0x02, 0x09, 0x60, 0x12, 0x00};
const char TXT_PL_MENU_CANCEL[] = {0x1f, 0x41, 0x0e, 0x15, 0x0c, 0x15, 0x0a, 0x00};
const char TXT_PL_MENU_EXIT[] = {0x1f, 0x50, 0x0f, 0x17, 0x12, 0x60, 0x14, 0x00};
const char TXT_PL_TASK_LIST_HEADER[] = {0xda, 0x81, 0x84, 0x81, 0x8e, 0x89, 0x85, 0xa0, 0xfe, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xcf, 0x90, 0x89, 0x93, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0x00};
const char TXT_PL_MENU_MAIN1[] = {0xf1, 0x50, 0x0f, 0x0c, 0x05, 0x00};
const char TXT_PL_MENU_MAIN2[] = {0xf2, 0x43, 0x08, 0x0c, 0x05, 0x17, 0x09, 0x0b, 0x00};
const char TXT_PL_MENU_MAIN3[] = {0xf3, 0x57, 0x5d, 0x04, 0x1a, 0x01, 0x12, 0x0e, 0x09, 0x01, 0x00};
const char TXT_PL_MENU_MAIN4[] = {0xf4, 0x53, 0x0b, 0x0c, 0x05, 0x10, 0x00};
const char TXT_PL_MENU_MAIN5[] = {0xf5, 0x5a, 0x01, 0x5e, 0x0f, 0x07, 0x01, 0x00};
const char TXT_PL_MENU_MAIN6[] = {0xf6, 0x50, 0x09, 0x07, 0x13, 0x09, 0x0c, 0x05, 0x20, 0x43, 0x0f, 0x0d, 0x0d, 0x01, 0x0e, 0x04, 0x00};
const char TXT_PL_MENU_OPTIONS_LANG[] = {0xf1, 0x4a, 0x5d, 0x1a, 0x19, 0x0b, 0x20, 0x2f, 0x20, 0x4c, 0x01, 0x0e, 0x07, 0x15, 0x01, 0x07, 0x05, 0x00};
const char TXT_PL_MENU_OPTIONS_MSX[] = {0xf2, 0x4d, 0x15, 0x1a, 0x19, 0x0b, 0x01, 0x20, 0x0f, 0x0e, 0x2f, 0x0f, 0x06, 0x06, 0x00};
const char TXT_PL_OPTIONS_HEADER[] = {0x55, 0x13, 0x14, 0x01, 0x17, 0x09, 0x05, 0x0e, 0x09, 0x01, 0x20, 0x07, 0x12, 0x19, 0x00};
const char TXT_PL_TASK_MANAGER_PRIO_HEADER[] = {0x4d, 0x01, 0x0e, 0x01, 0x07, 0x05, 0x12, 0x20, 0x10, 0x12, 0x09, 0x0f, 0x12, 0x19, 0x14, 0x05, 0x14, 0x60, 0x17, 0x20, 0x1a, 0x01, 0x04, 0x01, 0x5f, 0x00};
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_1[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xfe, 0xda, 0x84, 0x8f, 0x8c, 0x8e, 0x8f, 0xe1, 0xdc, 0xfe, 0xa0, 0xda, 0x81, 0x84, 0x81, 0x8e, 0x89, 0x85, 0x00};
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_2[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7e, 0x20, 0x7e, 0x52, 0x7e, 0x4d, 0x7e, 0x20, 0x20, 0x7e, 0x20, 0x20, 0x7e, 0x52, 0x7e, 0x4d, 0x7e, 0x20, 0x00};
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_3[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7e, 0x20, 0x7e, 0x0f, 0x7e, 0x01, 0x7e, 0x20, 0x20, 0x7e, 0x20, 0x20, 0x7e, 0x0f, 0x7e, 0x01, 0x7e, 0x20, 0x00};
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_4[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7e, 0x48, 0x7e, 0x0c, 0x7e, 0x13, 0x7e, 0x20, 0x20, 0x7e, 0x20, 0x48, 0x7e, 0x0c, 0x7e, 0x13, 0x7e, 0x20, 0x00};
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_5[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7e, 0x0f, 0x7e, 0x0e, 0x7e, 0x01, 0x7e, 0x48, 0x20, 0x7e, 0x20, 0x0f, 0x7e, 0x0e, 0x7e, 0x01, 0x7e, 0x48, 0x00};
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_6[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7e, 0x04, 0x7e, 0x09, 0x7e, 0x12, 0x7e, 0x01, 0x20, 0x7e, 0x20, 0x04, 0x7e, 0x09, 0x7e, 0x12, 0x7e, 0x01, 0x00};
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_7[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7e, 0x0f, 0x7e, 0x03, 0x7e, 0x13, 0x7e, 0x0e, 0x20, 0x7e, 0x20, 0x0f, 0x7e, 0x03, 0x7e, 0x13, 0x7e, 0x0e, 0x00};
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_8[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7e, 0x17, 0x7e, 0x14, 0x7e, 0x14, 0x7e, 0x04, 0x20, 0x7e, 0x20, 0x17, 0x7e, 0x14, 0x7e, 0x14, 0x7e, 0x04, 0x00};
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_9[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7e, 0x0c, 0x7e, 0x17, 0x7e, 0x17, 0x7e, 0x05, 0x20, 0x7e, 0x20, 0x0c, 0x7e, 0x17, 0x7e, 0x17, 0x7e, 0x05, 0x00};
const char TXT_PL_TASK_MANAGER_PRIO_TABLE_HEADER_10[] = {0xd0, 0x8f, 0x93, 0x94, 0x81, 0xdc, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xfe, 0x81, 0xfe, 0x8f, 0xfe, 0x8f, 0xfe, 0x8c, 0xa0, 0xfe, 0xa0, 0x81, 0xfe, 0x8f, 0xfe, 0x8f, 0xfe, 0x8c, 0x00};
const char TXT_PL_MENU_TASK_MANAGER_W[] = {0x17, 0x00};
const char TXT_PL_MENU_TASK_MANAGER_S[] = {0x13, 0x00};
const char TXT_PL_MENU_TASK_MANAGER_A[] = {0x01, 0x00};
const char TXT_PL_MENU_TASK_MANAGER_D[] = {0x04, 0x00};
const char TXT_PL_MENU_TASK_MANAGER_PLUS[] = {0x2b, 0x00};
const char TXT_PL_MENU_TASK_MANAGER_MINUS[] = {0x2d, 0x00};
const char TXT_PL_TASK_MANAGER_HEADER[] = {0x4d, 0x01, 0x0e, 0x01, 0x07, 0x05, 0x12, 0x20, 0x1a, 0x01, 0x04, 0x01, 0x5f, 0x00};
const char TXT_PL_TASK_MANAGER_TABLE_HEADER[] = {0xda, 0x81, 0x84, 0x81, 0x8e, 0x89, 0x85, 0xa0, 0xfe, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xcf, 0x90, 0x89, 0x93, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0x00};
const char TXT_PL_MENU_FARMLAND5[] = {0xf1, 0x55, 0x10, 0x12, 0x01, 0x17, 0x01, 0x00};
const char TXT_PL_MENU_FARMLAND6[] = {0xf2, 0x4f, 0x10, 0x09, 0x05, 0x0b, 0x01, 0x00};
const char TXT_PL_MENU_FARMLAND7[] = {0xf3, 0x5a, 0x02, 0x09, 0x0f, 0x12, 0x19, 0x00};
const char TXT_PL_FIELD_LIST_HEADER[] = {0xcd, 0x87, 0xfe, 0xa0, 0xd5, 0x90, 0x92, 0x81, 0x97, 0x81, 0xa0, 0xa0, 0xfe, 0xa0, 0xa0, 0xd3, 0x94, 0x81, 0x8e, 0xa0, 0xa0, 0xfe, 0xd4, 0xa0, 0xa0, 0xfe, 0xd3, 0xb1, 0xa0, 0xfe, 0xd3, 0xb2, 0xa0, 0xfe, 0xd3, 0xb3, 0xa0, 0x00};
const char TXT_PL_FARM_FIELD_TXT[] = {0x50, 0x0f, 0x0c, 0x05, 0x3a, 0x20, 0x20, 0x00};
const char TXT_PL_FARM_PLANT_TXT[] = {0x55, 0x10, 0x12, 0x01, 0x17, 0x01, 0x3a, 0x20, 0x00};
const char TXT_PL_TASK_FARMLAND_FARM[] = {0x55, 0x10, 0x12, 0x01, 0x17, 0x01, 0x20, 0x20, 0x00};
const char TXT_PL_TASK_FARMLAND_CARE[] = {0x4f, 0x10, 0x09, 0x05, 0x0b, 0x01, 0x20, 0x20, 0x00};
const char TXT_PL_TASK_FARMLAND_REAP[] = {0x5a, 0x02, 0x09, 0x0f, 0x12, 0x19, 0x20, 0x20, 0x00};
const char TXT_PL_TASK_DSC_FARMLAND_FIELD[] = {0x50, 0x0f, 0x0c, 0x05, 0x00};
const char TXT_PL_TASK_DSC_FARMLAND_POTATO[] = {0x1a, 0x09, 0x05, 0x0d, 0x0e, 0x09, 0x01, 0x0b, 0x09, 0x00};
const char TXT_PL_TASK_DSC_FARMLAND_LUPINE[] = {0x5e, 0x15, 0x02, 0x09, 0x0e, 0x00};
const char TXT_PL_TASK_DSC_FARMLAND_WHEAT[] = {0x10, 0x13, 0x1a, 0x05, 0x0e, 0x09, 0x03, 0x01, 0x00};
const char TXT_PL_TASK_DSC_FARMLAND_CORN[] = {0x0b, 0x15, 0x0b, 0x15, 0x12, 0x19, 0x04, 0x1a, 0x01, 0x00};
const char TXT_PL_MENU_PIGPEN1[] = {0xf1, 0x4b, 0x01, 0x12, 0x0d, 0x09, 0x05, 0x0e, 0x09, 0x05, 0x00};
const char TXT_PL_MENU_PIGPEN2[] = {0xf2, 0x53, 0x10, 0x12, 0x1a, 0x5b, 0x14, 0x01, 0x0e, 0x09, 0x05, 0x00};
const char TXT_PL_MENU_PIGPEN3[] = {0xf3, 0x50, 0x12, 0x1a, 0x05, 0x02, 0x15, 0x04, 0x0f, 0x17, 0x01, 0x00};
const char TXT_PL_MENU_SMOKEHOUSE1[] = {0xf1, 0x50, 0x12, 0x1a, 0x05, 0x10, 0x09, 0x13, 0x19, 0x00};
const char TXT_PL_MENU_SMOKEHOUSE2[] = {0xf2, 0x53, 0x10, 0x12, 0x1a, 0x5b, 0x14, 0x01, 0x0e, 0x09, 0x05, 0x00};
const char TXT_PL_MENU_SMOKEHOUSE3[] = {0xf3, 0x50, 0x12, 0x1a, 0x05, 0x02, 0x15, 0x04, 0x0f, 0x17, 0x01, 0x00};
const char TXT_PL_MENU_SHOPOUT1[] = {0xf1, 0x57, 0x05, 0x0a, 0x04, 0x62, 0x00};
const char TXT_PL_MENU_SHOPOUT2[] = {0xf2, 0x52, 0x0f, 0x1a, 0x0d, 0x0f, 0x17, 0x01, 0x00};
const char TXT_PL_SHOP_DESCRIPTION[] = {0x53, 0x0b, 0x0c, 0x05, 0x10, 0x20, 0x17, 0x09, 0x05, 0x0c, 0x0f, 0x02, 0x12, 0x01, 0x0e, 0x63, 0x0f, 0x17, 0x19, 0x20, 0x27, 0x47, 0x53, 0x27, 0x2c, 0x20, 0x0f, 0x14, 0x17, 0x01, 0x12, 0x14, 0x19, 0x20, 0x37, 0x2d, 0x31, 0x31, 0x20, 0x09, 0x20, 0x31, 0x35, 0x2d, 0x31, 0x39, 0x2c, 0x20, 0x03, 0x0f, 0x04, 0x1a, 0x09, 0x05, 0x0e, 0x0e, 0x09, 0x05, 0x2e, 0x00};
const char TXT_PL_MENU_SHOPIN1[] = {0xf1, 0x4b, 0x15, 0x10, 0x20, 0x0f, 0x07, 0x60, 0x12, 0x01, 0x00};
const char TXT_PL_MENU_SHOPIN2[] = {0xf2, 0x52, 0x0f, 0x1a, 0x0d, 0x0f, 0x17, 0x01, 0x00};
const char TXT_PL_MENU_CREW1[] = {0xf1, 0x53, 0x0c, 0x0f, 0x14, 0x20, 0x31, 0x00};
const char TXT_PL_MENU_CREW2[] = {0xf2, 0x53, 0x0c, 0x0f, 0x14, 0x20, 0x32, 0x00};
const char TXT_PL_MENU_CREW3[] = {0xf3, 0x53, 0x0c, 0x0f, 0x14, 0x20, 0x33, 0x00};
const char TXT_PL_MENU_CREW4[] = {0xf4, 0x53, 0x0c, 0x0f, 0x14, 0x20, 0x34, 0x00};
const char TXT_PL_STAT_INT[] = {0x4d, 0x5b, 0x04, 0x12, 0x0f, 0x61, 0x5c, 0x00};
const char TXT_PL_STAT_STR[] = {0x53, 0x09, 0x5e, 0x01, 0x00};
const char TXT_PL_STAT_CUN[] = {0x53, 0x10, 0x12, 0x19, 0x14, 0x00};
const char TXT_PL_SKILL_ANI[] = {0x48, 0x0f, 0x04, 0x0f, 0x17, 0x0c, 0x01, 0x00};
const char TXT_PL_SKILL_FRM[] = {0x52, 0x0f, 0x0c, 0x0e, 0x09, 0x03, 0x14, 0x17, 0x0f, 0x00};
const char TXT_PL_SKILL_BTH[] = {0x4d, 0x01, 0x13, 0x01, 0x12, 0x13, 0x14, 0x17, 0x0f, 0x00};
const char TXT_PL_SKILL_TRD[] = {0x48, 0x01, 0x0e, 0x04, 0x05, 0x0c, 0x00};
const char TXT_PL_CHAR_1_STORY[] = {0x4a, 0x01, 0x0e, 0x15, 0x13, 0x1a, 0x20, 0x03, 0x01, 0x5e, 0x05, 0x20, 0x63, 0x19, 0x03, 0x09, 0x05, 0x20, 0x10, 0x12, 0x01, 0x03, 0x0f, 0x17, 0x01, 0x5e, 0x20, 0x17, 0x20, 0x50, 0x47, 0x52, 0x2e, 0x20, 0x4e, 0x01, 0x20, 0x0e, 0x09, 0x03, 0x1a, 0x19, 0x0d, 0x20, 0x13, 0x09, 0x5d, 0x20, 0x14, 0x01, 0x0b, 0x20, 0x0e, 0x01, 0x10, 0x12, 0x01, 0x17, 0x04, 0x5d, 0x20, 0x0e, 0x09, 0x05, 0x20, 0x1a, 0x0e, 0x01, 0x2c, 0x20, 0x01, 0x0c, 0x05, 0x20, 0x1a, 0x01, 0x20, 0x14, 0x0f, 0x20, 0x0d, 0x01, 0x20, 0x20, 0x0e, 0x01, 0x5e, 0x0f, 0x07, 0x09, 0x20, 0x09, 0x20, 0x0a, 0x05, 0x13, 0x14, 0x20, 0x0c, 0x05, 0x0e, 0x09, 0x17, 0x19, 0x2e, 0x20, 0x4a, 0x01, 0x0e, 0x15, 0x13, 0x1a, 0x20, 0x14, 0x0f, 0x20, 0x10, 0x0f, 0x13, 0x14, 0x01, 0x5c, 0x20, 0x13, 0x14, 0x01, 0x12, 0x14, 0x0f, 0x17, 0x01, 0x2e, 0x00};
const char TXT_PL_CHAR_2_STORY[] = {0x5a, 0x04, 0x1a, 0x09, 0x13, 0x09, 0x05, 0x0b, 0x20, 0x0e, 0x09, 0x05, 0x20, 0x0a, 0x05, 0x13, 0x14, 0x20, 0x10, 0x0f, 0x10, 0x15, 0x0c, 0x01, 0x12, 0x0e, 0x19, 0x20, 0x17, 0x05, 0x20, 0x17, 0x13, 0x09, 0x2e, 0x20, 0x4c, 0x15, 0x04, 0x1a, 0x09, 0x13, 0x0b, 0x01, 0x20, 0x10, 0x01, 0x0d, 0x09, 0x5d, 0x14, 0x01, 0x0a, 0x5b, 0x2c, 0x20, 0x63, 0x05, 0x20, 0x0a, 0x05, 0x07, 0x0f, 0x20, 0x0f, 0x0a, 0x03, 0x09, 0x05, 0x03, 0x20, 0x02, 0x19, 0x5e, 0x20, 0x06, 0x0f, 0x0c, 0x0b, 0x13, 0x2d, 0x04, 0x0f, 0x0a, 0x03, 0x1a, 0x05, 0x0d, 0x2e, 0x20, 0x5a, 0x04, 0x1a, 0x09, 0x13, 0x09, 0x01, 0x20, 0x17, 0x19, 0x12, 0x60, 0x63, 0x0e, 0x09, 0x01, 0x20, 0x09, 0x0e, 0x14, 0x05, 0x0c, 0x09, 0x07, 0x05, 0x0e, 0x03, 0x0a, 0x01, 0x20, 0x10, 0x0f, 0x20, 0x14, 0x01, 0x03, 0x09, 0x05, 0x2e, 0x00};
const char TXT_PL_CHAR_3_STORY[] = {0x54, 0x15, 0x14, 0x01, 0x0a, 0x20, 0x02, 0x5d, 0x04, 0x1a, 0x09, 0x05, 0x20, 0x48, 0x09, 0x13, 0x14, 0x0f, 0x12, 0x09, 0x01, 0x20, 0x57, 0x09, 0x05, 0x61, 0x0b, 0x01, 0x00};
const char TXT_PL_CHAR_4_STORY[] = {0x48, 0x09, 0x13, 0x14, 0x0f, 0x12, 0x09, 0x01, 0x20, 0x4a, 0x60, 0x1a, 0x0b, 0x01, 0x00};
const char SB_PL_PAUSE[] = {0x20, 0x20, 0x20, 0x50, 0x20, 0x20, 0x41, 0x20, 0x20, 0x55, 0x20, 0x20, 0x5a, 0x20, 0x20, 0x41, 0x20, 0x20, 0x20, 0x00};
const char SB_PL_UNPAUSE[] = {0x20, 0x20, 0x20, 0x4e, 0x0f, 0x12, 0x0d, 0x01, 0x0c, 0x0e, 0x01, 0x20, 0x10, 0x12, 0x5d, 0x04, 0x0b, 0x0f, 0x61, 0x5c, 0x20, 0x07, 0x12, 0x19, 0x20, 0x20, 0x20, 0x00};
const char SB_PL_WELCOME[] = {0x20, 0x20, 0x20, 0x20, 0x57, 0x09, 0x14, 0x01, 0x0a, 0x21, 0x20, 0x2b, 0x2f, 0x2d, 0x2f, 0x53, 0x10, 0x01, 0x03, 0x05, 0x20, 0x2d, 0x20, 0x0b, 0x0f, 0x0e, 0x14, 0x12, 0x0f, 0x0c, 0x01, 0x20, 0x03, 0x1a, 0x01, 0x13, 0x15, 0x20, 0x20, 0x20, 0x20, 0x4b, 0x09, 0x05, 0x0c, 0x02, 0x01, 0x13, 0x01, 0x20, 0x45, 0x0d, 0x10, 0x09, 0x12, 0x05, 0x20, 0x2d, 0x20, 0x17, 0x05, 0x12, 0x13, 0x0a, 0x01, 0x20, 0x02, 0x01, 0x12, 0x04, 0x1a, 0x0f, 0x20, 0x01, 0x0c, 0x10, 0x08, 0x01, 0x2e, 0x20, 0x20, 0x20, 0x20, 0x20, 0x43, 0x12, 0x05, 0x04, 0x09, 0x14, 0x13, 0x3a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x57, 0x13, 0x10, 0x01, 0x12, 0x03, 0x09, 0x05, 0x20, 0x10, 0x12, 0x0f, 0x07, 0x12, 0x01, 0x0d, 0x09, 0x13, 0x14, 0x19, 0x03, 0x1a, 0x0e, 0x05, 0x20, 0x09, 0x20, 0x0b, 0x0f, 0x0d, 0x10, 0x09, 0x0c, 0x01, 0x14, 0x0f, 0x12, 0x3a, 0x20, 0x4f, 0x13, 0x03, 0x01, 0x12, 0x36, 0x34, 0x20, 0x2d, 0x20, 0x44, 0x12, 0x4d, 0x0f, 0x12, 0x14, 0x01, 0x0c, 0x57, 0x0f, 0x0d, 0x02, 0x01, 0x14, 0x20, 0x20, 0x20, 0x20, 0x20, 0x47, 0x12, 0x01, 0x3a, 0x20, 0x4b, 0x0f, 0x04, 0x20, 0x2d, 0x20, 0x53, 0x05, 0x10, 0x01, 0x2c, 0x20, 0x4b, 0x0f, 0x16, 0x01, 0x0c, 0x2c, 0x20, 0x47, 0x06, 0x18, 0x20, 0x2d, 0x20, 0x4a, 0x15, 0x12, 0x01, 0x13, 0x2c, 0x20, 0x4d, 0x0f, 0x04, 0x05, 0x0c, 0x33, 0x20, 0x26, 0x20, 0x53, 0x01, 0x14, 0x15, 0x12, 0x0e, 0x1a, 0x2c, 0x20, 0x4d, 0x13, 0x18, 0x20, 0x2d, 0x20, 0x43, 0x12, 0x09, 0x13, 0x10, 0x13, 0x2c, 0x20, 0x53, 0x15, 0x10, 0x10, 0x0f, 0x12, 0x14, 0x20, 0x2d, 0x20, 0x41, 0x0e, 0x07, 0x12, 0x19, 0x20, 0x41, 0x0e, 0x04, 0x19, 0x2c, 0x20, 0x46, 0x0f, 0x0e, 0x14, 0x13, 0x20, 0x2d, 0x20, 0x53, 0x05, 0x10, 0x01, 0x20, 0x20, 0x20, 0x20, 0x20, 0x53, 0x10, 0x05, 0x03, 0x09, 0x01, 0x0c, 0x0e, 0x05, 0x20, 0x10, 0x0f, 0x04, 0x1a, 0x09, 0x5d, 0x0b, 0x0f, 0x17, 0x01, 0x0e, 0x09, 0x01, 0x20, 0x04, 0x0c, 0x01, 0x20, 0x52, 0x05, 0x18, 0x2f, 0x43, 0x01, 0x0d, 0x05, 0x0c, 0x0f, 0x14, 0x20, 0x1a, 0x01, 0x20, 0x10, 0x0f, 0x0d, 0x0f, 0x03, 0x20, 0x10, 0x12, 0x1a, 0x19, 0x20, 0x4b, 0x09, 0x03, 0x0b, 0x43, 0x20, 0x00};
const char SB_PL_SHOP_CLOSED[] = {0x20, 0x20, 0x20, 0x53, 0x0b, 0x0c, 0x05, 0x10, 0x20, 0x0a, 0x05, 0x13, 0x14, 0x20, 0x1a, 0x01, 0x0d, 0x0b, 0x0e, 0x09, 0x5d, 0x14, 0x19, 0x20, 0x20, 0x20, 0x00};

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
    TXT_PL_MENU_MAIN6,
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

