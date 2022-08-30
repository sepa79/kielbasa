#ifndef EASYFLASH_BANKS_H
#define EASYFLASH_BANKS_H

#define TITLE_SCREEN_BANK 1
#define MAIN_GFX_BANK 2
#define TRANSLATION_PL_BANK 3
#define TRANSLATION_EN_BANK 4

#define TICKS_BANK 5

#define MUSIC_BANK 6
#define MUSIC_BANK_RADIO_1 7

#define MENU_BANK_MAIN_MENU 10
#define MENU_BANK_FARMLAND 11
#define MENU_BANK_PIG_PEN 12
#define MENU_BANK_SMOKE_HOUSE 13
#define MENU_BANK_SHOP 14
#define MENU_BANK_SHOP_IN 15
#define MENU_BANK_CREW 16
#define MENU_BANK_PIGSLE_COMMAND_1 17
#define MENU_BANK_PIGSLE_COMMAND_2 18
#define MENU_BANK_PIGSLE_COMMAND_GFX_1 19

// Set main bank, reset history.
void setBank(char bank);
// Change bank, advance history.
void changeBank(char bank);
// Restore provious bank from history.
void restoreBank();
// Reset to previously set main, remove history.
void resetBank();
// Change to main, advance hostory.
void changeBankToMain();

#pragma compile("easyFlashBanks.c")

#endif