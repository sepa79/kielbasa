#ifndef EASYFLASH_BANKS_H
#define EASYFLASH_BANKS_H

#include <c64/memmap.h>

enum {
    TITLE_SCREEN_BANK=1,
    MAIN_GFX_BANK,
    TRANSLATION_PL_BANK,
    TRANSLATION_EN_BANK,
    TICKS_BANK,
    TASKS_BANK,
    MUSIC_BANK,
    MUSIC_BANK_RADIO_1,
    MUSIC_BANK_RADIO_2,
    MUSIC_BANK_RETRO_1,
    MENU_BANK_MAIN_MENU,
    MENU_BANK_TV_SCREEN,
// special menus are various options menus, they DO NOT have standard loaders etc - not using shared menu system code
// log menu mounts in 8k crt mode
    MENU_BANK_SPECIAL_MENUS,
    MENU_BANK_FARMLAND,
    MENU_BANK_PIG_PEN,
    MENU_BANK_KITCHEN,
    MENU_BANK_SHOP,
    MENU_BANK_SHOP_IN,
    MENU_BANK_CREW,
    MENU_BANK_PIGSLE_MENU,
    MENU_BANK_PIGSLE_COMMAND_2,
    MENU_BANK_PIGSLE_COMMAND_GFX_1,
    MENU_BANK_MAP_VILLIAGE_1,
    MENU_BANK_MAP_VILLIAGE_2,
    MENU_BANK_MAP_VILLIAGE_3,
    MENU_BANK_HERMANS_HOUSE,
    MENU_BANK_SULEJUKOWAS_HOUSE,
    MENU_BANK_FISHING_MENU,
    MENU_BANK_FISHING_MENU_2,
    MENU_BANK_CHURCH_MENU,
    MENU_BANK_BATTLE_MENU,
    MENU_BANK_RESPAWN,
    MENU_BANK_PIGSLE_INTRO,
};

// Set CRT bank, return previous bank that was set.
char setBank(char bank);

extern volatile char curport;

#pragma compile("easyFlashBanks.c")

#endif
