#include <engine/easyFlashBanks.h>
#include <c64/easyflash.h>
#include <c64/vic.h>

#define MAX_BANK_HISTORY 10
volatile char bankHistory[MAX_BANK_HISTORY];
volatile char bankHistoryPos = 0;

void setBank(char bank){
    bankHistoryPos = 0;
    eflash.bank = bank;
    bankHistory[bankHistoryPos] = bank;
}

void changeBank(char bank){
    bankHistoryPos++;
    eflash.bank = bank;
    bankHistory[bankHistoryPos] = bank;
    
    vic.color_border++;
    // debug
    if(bankHistoryPos == MAX_BANK_HISTORY){
        for(;;){
            vic.color_border++;
        }
    }
}

void restoreBank(){
    vic.color_border--;

    if(bankHistoryPos > 0)
        bankHistoryPos--;
    
    eflash.bank = bankHistory[bankHistoryPos];
}

void resetBank(){
    setBank(bankHistory[0]);
}

void changeBankToMain(){
    changeBank(bankHistory[0]);
}
