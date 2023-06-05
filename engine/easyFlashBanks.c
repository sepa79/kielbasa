#include <engine/easyFlashBanks.h>
#include <c64/easyflash.h>
#include <c64/vic.h>
volatile char curbank = 0;
volatile char curport = 0;

char setBank(char bank){
    char pbank  = curbank;
    curbank     = bank;
    eflash.bank = bank;
    return pbank;
}

char setPort(char port){
    char pport  = curport;
    curport     = port;
    mmap_set(port);
    return pport;
}