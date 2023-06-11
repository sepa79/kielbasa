#include <engine/easyFlashBanks.h>
#include <c64/easyflash.h>
#include <c64/vic.h>

char curbank = 0;
char curport = 0;

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


// copy of methods using different vars, to be used on IRQs, in order to avoid any race conditions
volatile char curbankIrq = 0;
volatile char curportIrq = 0;

char setBankIrq(char bank){
    char pbank  = curbankIrq;
    curbankIrq  = bank;
    eflash.bank = bank;
    return pbank;
}

char setPortIrq(char port){
    char pport  = curportIrq;
    curportIrq  = port;
    mmap_set(port);
    return pport;
}