
#include <c64/vic.h>
#include <assets/assetsSettings.h>
#include <c64/joystick.h>
#include <c64/sprites.h>
#include <c64/easyflash.h>
#include <c64/memmap.h>

#include <engine/easyFlashBanks.h>
#include <miniGame/villiageMapMain.h>

#pragma code ( villiageMapRAMCode )
#pragma data ( villiageMapRAMData )

static byte _prevRomCfgPC;
// ================================================================================
// Top raster
// ================================================================================
static void _playMsx(){
    if(gms_enableMusic){
        _prevRomCfgPC = ((byte *)0x01)[0];
        __asm {
            lda #MSX_ROM
            sta $01
            jsr MSX_PLAY
        };
        ((byte *)0x01)[0] = _prevRomCfgPC;
    }
}

__interrupt void _villiageMapSpriteInit(){

}

__interrupt void villiageMapIrq_topMap() {
    vic.color_border++;
    _playMsx();
    vic.color_border--;
}


__interrupt void villiageMapIrq_msx2() {
    vic.color_border++;
    _playMsx();
    vic.color_border--;
}

__interrupt void villiageMapIrq_openBorder() {
    vic_waitLine(250);
    vic.color_back = VCOL_BROWN;
    // vic.color_border++;
    vic.spr_enable = 0b00000000;
    // vic.color_border--;
}