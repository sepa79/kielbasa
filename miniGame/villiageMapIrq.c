
#include <c64/vic.h>
#include <assets/assetsSettings.h>
#include <c64/joystick.h>
#include <c64/sprites.h>
#include <c64/easyflash.h>
#include <c64/memmap.h>

#include <engine/easyFlashBanks.h>
#include <miniGame/villiageMapMain.h>
#include <engine/gameSettings.h>

#pragma code ( villiageMapRAMCode )
#pragma data ( villiageMapRAMData )

// ================================================================================
// Top raster
// ================================================================================
__interrupt void _villiageMapSpriteInit(){

}

__interrupt void villiageMapIrq_topMap() {
    // vic.color_back = VCOL_BLACK;
    vic.color_border++;

    showUiSpritesTop();
    playMsx();

    vic.color_back = VCOL_BROWN;
    // indicate frame position
    gms_framePos = FRAME_TOP_BORDER;
    vic.color_border--;
}


__interrupt void villiageMapIrq_msx2() {
    vic.color_border++;
    playMsx();
    // indicate frame position
    gms_framePos = FRAME_MIDDLE;

    vic.color_border--;
}

__interrupt void villiageMapIrq_openBorder() {
    // vic_waitLine(250);
    vic.color_back = VCOL_BLACK;
    vic.color_border++;
    showUiSpritesBottom();
    vic.color_border--;

    // indicate frame position
    gms_framePos = FRAME_BOTTOM;
}