#include <c64/vic.h>

#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>

#pragma data ( mainGameMsx )

__export const char music[] = {
    // #embed 0xffff 2 "assets/music/Piggy_8000.out"
    // #embed 0xffff 2 "assets/music/FarmGame.out"
    #embed 0xffff 136 "assets/music/FarmGame.sid"
};

#pragma data ( radioMsx1 )
__export const char radioMsx01[] = {
    #embed 0xffff 136 "assets/music/FarmGameRadio1.sid"
};

#pragma data ( radioMsx2 )
__export const char radioMsx02[] = {
    #embed 0xffff 136 "assets/music/FarmGameRadio2.sid"
};

#pragma data ( retroMsx1 )
__export const char retroMsx01[] = {
    #embed 0xffff 136 "assets/music/FarmGameRadioRetro.sid"
};

#pragma code ( musicCode )
#pragma data ( musicData )

// this better be called when music IRQs are not trying to use the player...
// TODO: could be moved to ROM and unpacked
void loadMusic(char sidIdx){
    // vic.color_back++;
    if(sidIdx == 0){
        char i = 0;
        do {
#assign y 0
#repeat
            ((volatile char*) MSX_DST_ADR)[y + i] = ((char*) MSX_SRC_ADR_0)[y + i];
#assign y y + 256
#until y == 0x2000
            i++;
        } while (i != 0);
    } else if(sidIdx == 1){
        char i = 0;
        do {
#assign y 0
#repeat
            ((volatile char*) MSX_DST_ADR)[y + i] = ((char*) MSX_SRC_ADR_1)[y + i];
#assign y y + 256
#until y == 0x1d00
            i++;
        } while (i != 0);
    }
}

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )