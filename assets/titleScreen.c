#include <c64/vic.h>
#include <c64/cia.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>

#pragma data ( titleScreenGfx )

__export const char titleScreen[] = {
    #embed 0xffff 2 "assets/multicolorGfx/title150622.kla"
    // #embed 0xffff 2 "assets/multicolorGfx/kielbasaEmpire.kla"
};

#pragma code ( titleScreenCode )
#pragma data ( titleScreenData )

static void _loadFullKoalaToBMP2(){
    // load colors
    char i = 0;
    do {
#assign y 0
#repeat
        GFX_2_SCR[y + i] = FULL_KOALA_SCR[y + i];
        COLOR_RAM[y + i] = FULL_KOALA_COL[y + i];
#assign y y + 256
#until y == 1024
        i++;
    } while (i != 0);

    // load bitmap
    i = 0;
    do {
#assign y 0
#repeat
        GFX_2_BMP[y + i] = FULL_KOALA_BMP[y + i];
#assign y y + 256
#until y == 8192
        i++;
    } while (i != 0);

}

void loadTitleScreen(){
    vic.color_border  = VCOL_BLACK;
    vic.color_back  = VCOL_BLACK;

    _loadFullKoalaToBMP2();

    // vic_setmode(VICM_HIRES_MC, GFX_2_SCR, GFX_2_BMP);
    vic.ctrl2 = VIC_CTRL2_MCM | VIC_CTRL2_CSEL | 0;
    cia2.pra = dd00_gfx2;
    vic.memptr = d018_gfx2;
    // screen needs to be enabled later like below:
    // vic.ctrl1 = VIC_CTRL1_BMM | VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3;
}

// Switching code generation back to shared section
#pragma code ( mainCode )
#pragma data ( mainData )
