
#include <c64/vic.h>
#include <c64/charwin.h>

// stage:
//  1 - black to bright
//  2 - bright to black
//  3 - full
void splashScreen(bool turnOn, char stage) {
    // splash screen
    static const char color[8] = {0x0, 0xe,0xe,0x3, 0xf,0xc,0xb, 0x0};
    char i = 0;

    if(stage & 1){
        do {
            // wait for raster
            vic_waitTop();
            vic.color_border = color[i];
            vic.color_back = color[i];
            vic_waitFrame();
            vic_waitFrame();
            vic_waitFrame();
            i++;
        } while (i<4);
    }

    if(turnOn){
        // screen on
        vic.ctrl1 = VIC_CTRL1_BMM | VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3;
    } else {
        // screen off
        vic.ctrl1 = VIC_CTRL1_BMM | VIC_CTRL1_RSEL | 3;
    }

    i = 4;
    // continue splash
    if(stage & 2){
        do {
            // wait for raster
            vic_waitTop();
            vic.color_border = color[i];
            vic.color_back = color[i];
            vic_waitFrame();
            vic_waitFrame();
            vic_waitFrame();
            i++;
        } while (i<8);
    }
}
