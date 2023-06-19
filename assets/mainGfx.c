#include <c64/vic.h>
#include <engine/easyFlashBanks.h>
#include <c64/memmap.h>
#include <string.h>

#include <assets/assetsSettings.h>
#include "mainGfx.h"

#pragma data ( mainGfxSprites )

// TODO: Proper init of the sprites
__export const struct SPRITES SpriteResources = {{
    #embed 0x40 3 "assets/sprites/joyCursor.spd"
}};

// ===========================================================================================
// Aux gfx - to be copied into sprites
// ===========================================================================================
#pragma data ( mainGfxAux )

__export const struct AUX_GFX AuxResources = {{
    #embed 0xffff 20 "assets/sprites/taskIcons.spd"
},{
    #embed 0xffff 20 "assets/sprites/faces.spd"
},{
    #embed 0xffff 20 "assets/sprites/timeIcons.spd"
},{
    #embed 0xffff 20 "assets/sprites/weather.spd"
}};

// ===========================================================================================
// Fonts
// ===========================================================================================
#pragma data(mainGfxFont)
__export const char GAME_FONT[0x800] = {
    // #embed 0xffff 2 "assets/fonts.64c"
    #embed ctm_chars "assets/gameFont.ctm"
};

// ===========================================================================================
// Data Loader
// ===========================================================================================
#pragma code ( mainGfxCode )
#pragma data ( mainGfxData )

// TODO: if there is space, provide unpacked routines to speed it up. Done often, worth it.

// Loading fonts and sprites
static void _loadMainGfx(){
    // ROM on, I/O off - as we will copy to RAM under I/O ports
    // mmap_set(MMAP_ALL_ROM);

    // sprites - cursor, to GFX1
    memcpy((char *)GFX_1_SPR_DST_ADR, GFX_1_SPR_SRC, 0x40);
    // sprites - UI - skip first UI sprite as its copied elsewhere already
    memset((char *)UI_SPR_ADR, 0, 0x800);
    // These sprites might be empty - that's fine, banks will be used later and data will be copied in
    // memcpy((char *)UI_SPR_ADR, GFX_1_SPR_SRC+0x40, 0x800);
    // turn ROMS and I/O back on, so that we don't get a problem when bank tries to be switched but I/O is not visible
    // mmap_set(MMAP_ROM);
}

static void _loadMainFont(){
    // ROM on, I/O off - as we will copy to RAM under I/O ports
    char pport = setPort(MMAP_ALL_ROM);
    // fonts & aux (but only half of the cart region atm - copy as needed later)
    char i = 0;
    do {
#assign y 0
#repeat
        GFX_1_FNT[y + i] = GFX_1_FNT_SRC[y + i];
        // AUX GFX will be read from CRT directly
        // GFX_1_AUX[y + i] = GFX_1_AUX_SRC[y + i];
        // GFX_1_AUX[y + 0x800 + i] = GFX_1_AUX_SRC[y + 0x800 + i];
#assign y y + 0x100
#until y == 0x100*8
        i++;
    } while (i != 0);

    // turn ROMS and I/O back on, so that we don't get a problem when bank tries to be switched but I/O is not visible
    setPort(pport);
}
// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )

// used often, kept in RAM
// __export const char SPR_TIME_ICONS_COLORS[8] = {0x1, 0xb, 0xc, 0xf, 0x3, 0xf, 0xc, 0xb};
// __export const char SPR_WEATHER_COLORS[10] = {0x7, 0x7, 0x7, 0xe, 0x7, 0x1, 0xb, 0xb, 0xb, 0xb};
// 0 is end of colors, use 0x10 for black
__export const char SPR_JOY_CURSOR_COLORS[16] = {0x10, 0x10, 0xb, 0xb, 0xf, 0xf, 0x3, 0x3, 0x1, 0x3, 0x3, 0xf, 0xf, 0xb, 0xb, 0x0};
__export const char SPR_JOY_CURSOR_COLORS_ERROR[16] = {0x10, 0x10, 0x9, 0x9, 0x2, 0x2, 0x8, 0x8, 0xa, 0x8, 0x8, 0x2, 0x2, 0x9, 0x9, 0x0};

// RAM wrapper
#pragma code ( code )
#pragma data ( data )

void loadMainGfx(){
    byte pBank = setBank(MAIN_GFX_BANK);
    _loadMainGfx();
    setBank(pBank);
}
void loadMainFont(){
    byte pBank = setBank(MAIN_GFX_BANK);
    _loadMainFont();
    setBank(pBank);
}