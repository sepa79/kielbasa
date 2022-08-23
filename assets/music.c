#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>

#pragma data ( musicPlayer )

__export const char music[] = {
    // #embed 0xffff 2 "assets/music/Piggy_8000.out"
    // #embed 0xffff 2 "assets/music/FarmGame.out"
    #embed 0xffff 136 "assets/music/FarmGame.sid"
};

#pragma data ( radioMsx1 )
__export const char radioMsx01[] = {
    #embed 0xffff 136 "assets/music/FarmGameRadio1.sid"
};

#pragma code ( musicCode )
#pragma data ( musicData )

void loadMusic(){
    char i = 0;
    do {
#assign y 0
#repeat
         ((volatile char*) MSX_DST_ADR)[y + i] = ((char*) MSX_SRC_ADR)[y + i];
#assign y y + 256
#until y == 0x2000
        i++;
    } while (i != 0);

}

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )