#ifndef MAIN_GFX_H
#define MAIN_GFX_H
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>

#define SPR_CHARACTER_PORTRAIT_MULTICOLOR_1 0x01
#define SPR_CHARACTER_PORTRAIT_MULTICOLOR_2 0x02
#define SPR_WEATHER_MULTICOLOR_1 0x01
#define SPR_WEATHER_MULTICOLOR_2 0x0f

enum SPRITE_VIC_BANKS {
    SPR_BANK_JOY_CURSOR1=0x10,
    SPR_BANK_JOY_CURSOR2,
    SPR_BANK_CHARACTER_PORTRAIT1=UI_SPRITE_BANKS_START_VALUE,
    SPR_BANK_CHARACTER_BAR1,
    SPR_BANK_CHARACTER_PORTRAIT2,
    SPR_BANK_CHARACTER_BAR2,
    SPR_BANK_CHARACTER_PORTRAIT3,
    SPR_BANK_CHARACTER_BAR3,
    SPR_BANK_CHARACTER_PORTRAIT4,
    SPR_BANK_CHARACTER_BAR4,
    SPR_BANK_DATE_TXT1,
    SPR_BANK_DATE_TXT2,
    SPR_BANK_TIME_ICON,
    SPR_BANK_WEATHER_ICON,
    SPR_BANK_TXT_UP_1,
    SPR_BANK_TXT_UP_2,
    SPR_BANK_TXT_UP_3,
    SPR_BANK_TXT_UP_4,
    SPR_BANK_TXT_BOTTOM_1,
    SPR_BANK_TXT_BOTTOM_2,
    SPR_BANK_TXT_BOTTOM_3,
    SPR_BANK_TXT_BOTTOM_4,
};

// #define SPR_JOY_CURSOR1         ((char *)GFX_1_BASE + 64*SPR_BANK_JOY_CURSOR1)
// #define SPR_JOY_CURSOR2         ((char *)GFX_1_BASE + 64*SPR_BANK_JOY_CURSOR2)
#define SPR_CHARACTER_BAR1      ((char *)UI_BMP_ADR + 64*SPR_BANK_CHARACTER_BAR1)
#define SPR_CHARACTER_BAR2      ((char *)UI_BMP_ADR + 64*SPR_BANK_CHARACTER_BAR2)
#define SPR_CHARACTER_BAR3      ((char *)UI_BMP_ADR + 64*SPR_BANK_CHARACTER_BAR3)
#define SPR_CHARACTER_BAR4      ((char *)UI_BMP_ADR + 64*SPR_BANK_CHARACTER_BAR4)
#define SPR_CHARACTER_PORTRAIT1 ((char *)UI_BMP_ADR + 64*SPR_BANK_CHARACTER_PORTRAIT1)
#define SPR_CHARACTER_PORTRAIT2 ((char *)UI_BMP_ADR + 64*SPR_BANK_CHARACTER_PORTRAIT2)
#define SPR_CHARACTER_PORTRAIT3 ((char *)UI_BMP_ADR + 64*SPR_BANK_CHARACTER_PORTRAIT3)
#define SPR_CHARACTER_PORTRAIT4 ((char *)UI_BMP_ADR + 64*SPR_BANK_CHARACTER_PORTRAIT4)
#define SPR_DATE_TXT1           ((char *)UI_BMP_ADR + 64*SPR_BANK_DATE_TXT1)
#define SPR_DATE_TXT2           ((char *)UI_BMP_ADR + 64*SPR_BANK_DATE_TXT2)
#define SPR_TIME_ICON           ((char *)UI_BMP_ADR + 64*SPR_BANK_TIME_ICON)
#define SPR_WEATHER_ICON        ((char *)UI_BMP_ADR + 64*SPR_BANK_WEATHER_ICON)
#define SPR_TXT_UP_1            ((char *)UI_BMP_ADR + 64*SPR_BANK_TXT_UP_1)
#define SPR_TXT_UP_2            ((char *)UI_BMP_ADR + 64*SPR_BANK_TXT_UP_2)
#define SPR_TXT_UP_3            ((char *)UI_BMP_ADR + 64*SPR_BANK_TXT_UP_3)
#define SPR_TXT_UP_4            ((char *)UI_BMP_ADR + 64*SPR_BANK_TXT_UP_4)
#define SPR_TXT_BOTTOM_1        ((char *)UI_BMP_ADR + 64*SPR_BANK_TXT_BOTTOM_1)
#define SPR_TXT_BOTTOM_2        ((char *)UI_BMP_ADR + 64*SPR_BANK_TXT_BOTTOM_2)
#define SPR_TXT_BOTTOM_3        ((char *)UI_BMP_ADR + 64*SPR_BANK_TXT_BOTTOM_3)
#define SPR_TXT_BOTTOM_4        ((char *)UI_BMP_ADR + 64*SPR_BANK_TXT_BOTTOM_4)

// these are just copied into SPR_CHARACTER_BARX as needed
#define SPR_TASK_FARM1   (AuxResources.TASK_ICONS)
#define SPR_TASK_FARM2   (AuxResources.TASK_ICONS + 64*1)
#define SPR_TASK_FARM3   (AuxResources.TASK_ICONS + 64*2)
#define SPR_TASK_SLEEP   (AuxResources.TASK_ICONS + 64*3)
#define SPR_TASK_FEED    (AuxResources.TASK_ICONS + 64*4)
#define SPR_TASK_AIRDEF  (AuxResources.TASK_ICONS + 64*5)
#define SPR_TASK_BUTCHER (AuxResources.TASK_ICONS + 64*6)
#define SPR_TASK_SHOP    (AuxResources.TASK_ICONS + 64*7)
#define SPR_TASK_MIA     (AuxResources.TASK_ICONS + 64*8)
#define SPR_TASK_DRINK   (AuxResources.TASK_ICONS + 64*9)
#define SPR_TASK_FIX     (AuxResources.TASK_ICONS + 64*10)
#define SPR_TASK_CLEAN1  (AuxResources.TASK_ICONS + 64*11)
#define SPR_TASK_CLEAN2  (AuxResources.TASK_ICONS + 64*12)
#define SPR_TASK_EAT     (AuxResources.TASK_ICONS + 64*13)
#define SPR_TASK_COOK    (AuxResources.TASK_ICONS + 64*14)
#define SPR_PORTRAIT01   (AuxResources.PORTRAITS)
#define SPR_PORTRAIT02   (AuxResources.PORTRAITS + 64*1)
#define SPR_PORTRAIT03   (AuxResources.PORTRAITS + 64*2)
#define SPR_PORTRAIT04   (AuxResources.PORTRAITS + 64*3)

struct SPRITES {
    // sprite cursor
    char JOY_CURSOR[64];
    // empty battery 4x
    char CHARACTER_BARS[64*4];
    // char pictures
    char CHARACTER_PORTRAITS[64*4];
    // Date icons - empty, as game draws over these sprites
    char DATE_TXT[64*2];
    // time icons
    char TIME_ICON[64];
    char WEATHER_ICON[64];
    char UI_TXT_UP[64*4];
    char UI_TXT_BOTTOM[64*4];
};

const struct AUX_GFX {
    // task icons
    char TASK_ICONS[64*16];
    // Portraits
    char PORTRAITS[64*4];
    // time icons
    char TIME_ICONS[64*8];
    // weather icons
    char WEATHER_ICONS[64*10];
    // sprite fonts
    char SPRITE_FONTS[59*7];
};

// #define TASK_ICONS  ((char *)GFX_1_AUX_DST_ADR)
// #define PORTRAITS  ((char *)GFX_1_AUX_DST_ADR+64*14)
// #define SPRITE_FONTS  ((char *)GFX_1_AUX_DST_ADR+64*18)

extern __export const struct SPRITES SpriteResources;
extern __export const struct AUX_GFX AuxResources;

extern const char SPR_TIME_ICONS_COLORS[8];
extern const char SPR_WEATHER_COLORS[10];
// 0 is end of colors, use 0x10 for black
extern const char SPR_JOY_CURSOR_COLORS[16];
extern const char SPR_JOY_CURSOR_COLORS_ERROR[16];

// Section and region for main gfx (fonts, sprites) and code to show it
#pragma section( mainGfxSprites, 0 )
#pragma section( mainGfxAux, 0 )
#pragma section( mainGfxFont, 0 )
#pragma section( mainGfxCode, 0 )
#pragma section( mainGfxData, 0 )
#pragma region( bankMainGfx1, GFX_1_SPR_SRC_ADR, GFX_1_AUX_SRC_ADR, , MAIN_GFX_BANK, { mainGfxSprites }, UI_SPR_ADR )
#pragma region( bankMainGfx2, GFX_1_AUX_SRC_ADR, GFX_1_AUX_SRC_ADR + 0x1000, , MAIN_GFX_BANK, { mainGfxAux })
// !!! $1000 gap here for Faces or some other sprites
#pragma region( bankMainGfx3, GFX_1_FNT_SRC_ADR, GFX_1_FNT_SRC_ADR + 0x0800, , MAIN_GFX_BANK, { mainGfxFont }, GFX_1_FNT_DST_ADR )
#pragma region( bankMainGfx4, GFX_1_FNT_SRC_ADR + 0x0800, GFX_1_FNT_SRC_ADR + 0x0fff, , MAIN_GFX_BANK, { mainGfxCode, mainGfxData } )

#pragma code ( mainGfxCode )
#pragma data ( mainGfxData )


// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )

void loadMainGfx();
void loadMainFont();

#pragma compile("mainGfx.c")

#endif