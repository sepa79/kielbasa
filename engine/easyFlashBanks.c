#include <engine/easyFlashBanks.h>
#include <c64/easyflash.h>
#include <c64/vic.h>
volatile char curbank = 0;

char setBank(char bank){
    char pbank  = curbank;
    curbank     = bank;
    eflash.bank = bank;
    return pbank;
}