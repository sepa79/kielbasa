#ifndef EASYFLASH_BANKS_H
#define EASYFLASH_BANKS_H

#include <c64/memmap.h>

#define TITLE_SCREEN_BANK 1
#define MAIN_GFX_BANK 2
#define TRANSLATION_PL_BANK 3
#define TRANSLATION_EN_BANK 4

#define TICKS_BANK 5
#define TASKS_BANK 6

#define MUSIC_BANK 7
#define MUSIC_BANK_RADIO_1 8
#define MUSIC_BANK_RETRO_1 9

#define MENU_BANK_MAIN_MENU 10
// special menus are various options menus, they DO NOT have standard loaders etc - not using shared menu system code
// log menu mounts in 8k crt mode
#define MENU_BANK_SPECIAL_MENUS 11
#define MENU_BANK_FARMLAND 12
#define MENU_BANK_PIG_PEN 13
#define MENU_BANK_KITCHEN 14
#define MENU_BANK_SHOP 15
#define MENU_BANK_SHOP_IN 16
#define MENU_BANK_CREW 17
#define MENU_BANK_PIGSLE_COMMAND_1 18
#define MENU_BANK_PIGSLE_COMMAND_2 19
#define MENU_BANK_PIGSLE_COMMAND_GFX_1 20
#define MENU_BANK_MAP_VILLIAGE_1 21
#define MENU_BANK_MAP_VILLIAGE_2 22
#define MENU_BANK_MAP_VILLIAGE_3 23

// Set CRT bank, return previous bank that was set.
char setBank(char bank);
// Set Port ($01), return previous value that was set.
char setPort(char port);

// Set CRT bank, return previous bank that was set. Inside IRQ use only.
char setBankIrq(char bank);
// Set Port ($01), return previous value that was set. Inside IRQ use only.
char setPortIrq(char port);

#pragma compile("easyFlashBanks.c")

#endif
