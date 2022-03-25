#ifndef MAIN_GFX_H
#define MAIN_GFX_H
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>

#define SPR_CHARACTER_PORTRAIT_MULTICOLOR_1 0x01
#define SPR_CHARACTER_PORTRAIT_MULTICOLOR_2 0x02
#define SPR_WEATHER_MULTICOLOR_1 0x01
#define SPR_WEATHER_MULTICOLOR_2 0x0f

enum SPRITE_VIC_BANKS {
    SPR_BANK_JOY_CURSOR1=SPRITE_BANKS_START_VALUE,
    SPR_BANK_JOY_CURSOR2,
    SPR_BANK_CHARACTER_BAR1,
    SPR_BANK_CHARACTER_BAR2,
    SPR_BANK_CHARACTER_BAR3,
    SPR_BANK_CHARACTER_BAR4,
    SPR_BANK_CHARACTER_PORTRAIT1,
    SPR_BANK_CHARACTER_PORTRAIT2,
    SPR_BANK_CHARACTER_PORTRAIT3,
    SPR_BANK_CHARACTER_PORTRAIT4,
    SPR_BANK_DATE_TXT1,
    SPR_BANK_DATE_TXT2,
    SPR_BANK_TIME_ICON1,
    SPR_BANK_TIME_ICON2,
    SPR_BANK_TIME_ICON3,
    SPR_BANK_TIME_ICON4,
    SPR_BANK_TIME_ICON5,
    SPR_BANK_TIME_ICON6,
    SPR_BANK_TIME_ICON7,
    SPR_BANK_TIME_ICON8,
    SPR_BANK_CURRENCY1,
    SPR_BANK_CURRENCY2,
    SPR_BANK_CURRENCY3,
    SPR_BANK_CURRENCY_TXT,
    SPR_BANK_WEATHER1,
    SPR_BANK_WEATHER2,
    SPR_BANK_WEATHER3,
    SPR_BANK_WEATHER4,
    SPR_BANK_WEATHER5,
    SPR_BANK_WEATHER6,
    SPR_BANK_WEATHER7,
    SPR_BANK_WEATHER8,
    SPR_BANK_WEATHER9,
    SPR_BANK_WEATHER10
};

// #define SPR_JOY_CURSOR1         ((char *)GFX_1_BASE + 64*SPR_BANK_JOY_CURSOR1)
// #define SPR_JOY_CURSOR2         ((char *)GFX_1_BASE + 64*SPR_BANK_JOY_CURSOR2)
#define SPR_CHARACTER_BAR1      ((char *)GFX_1_BASE + 64*SPR_BANK_CHARACTER_BAR1)
#define SPR_CHARACTER_BAR2      ((char *)GFX_1_BASE + 64*SPR_BANK_CHARACTER_BAR2)
#define SPR_CHARACTER_BAR3      ((char *)GFX_1_BASE + 64*SPR_BANK_CHARACTER_BAR3)
#define SPR_CHARACTER_BAR4      ((char *)GFX_1_BASE + 64*SPR_BANK_CHARACTER_BAR4)
// #define SPR_CHARACTER_PORTRAIT1 ((char *)GFX_1_BASE + 64*SPR_BANK_CHARACTER_PORTRAIT1)
// #define SPR_CHARACTER_PORTRAIT2 ((char *)GFX_1_BASE + 64*SPR_BANK_CHARACTER_PORTRAIT2)
// #define SPR_CHARACTER_PORTRAIT3 ((char *)GFX_1_BASE + 64*SPR_BANK_CHARACTER_PORTRAIT3)
// #define SPR_CHARACTER_PORTRAIT4 ((char *)GFX_1_BASE + 64*SPR_BANK_CHARACTER_PORTRAIT4)
// #define SPR_DATE_TXT1           ((char *)GFX_1_BASE + 64*SPR_BANK_DATE_TXT1)
// #define SPR_DATE_TXT2           ((char *)GFX_1_BASE + 64*SPR_BANK_DATE_TXT2)
// #define SPR_TIME_ICON1          ((char *)GFX_1_BASE + 64*SPR_BANK_TIME_ICON1)
// #define SPR_TIME_ICON2          ((char *)GFX_1_BASE + 64*SPR_BANK_TIME_ICON2)
// #define SPR_TIME_ICON3          ((char *)GFX_1_BASE + 64*SPR_BANK_TIME_ICON3)
// #define SPR_TIME_ICON4          ((char *)GFX_1_BASE + 64*SPR_BANK_TIME_ICON4)
// #define SPR_TIME_ICON5          ((char *)GFX_1_BASE + 64*SPR_BANK_TIME_ICON5)
// #define SPR_TIME_ICON6          ((char *)GFX_1_BASE + 64*SPR_BANK_TIME_ICON6)
// #define SPR_TIME_ICON7          ((char *)GFX_1_BASE + 64*SPR_BANK_TIME_ICON7)
// #define SPR_TIME_ICON8          ((char *)GFX_1_BASE + 64*SPR_BANK_TIME_ICON8)
// #define SPR_CURRENCY1           ((char *)GFX_1_BASE + 64*SPR_BANK_CURRENCY1)
// #define SPR_CURRENCY2           ((char *)GFX_1_BASE + 64*SPR_BANK_CURRENCY2)
// #define SPR_CURRENCY3           ((char *)GFX_1_BASE + 64*SPR_BANK_CURRENCY3)
// #define SPR_CURRENCY_TXT        ((char *)GFX_1_BASE + 64*SPR_BANK_CURRENCY_TXT)
// #define SPR_WEATHER1            ((char *)GFX_1_BASE + 64*SPR_BANK_WEATHER1)
// #define SPR_WEATHER2            ((char *)GFX_1_BASE + 64*SPR_BANK_WEATHER2)
// #define SPR_WEATHER3            ((char *)GFX_1_BASE + 64*SPR_BANK_WEATHER3)
// #define SPR_WEATHER4            ((char *)GFX_1_BASE + 64*SPR_BANK_WEATHER4)
// #define SPR_WEATHER5            ((char *)GFX_1_BASE + 64*SPR_BANK_WEATHER5)
// #define SPR_WEATHER6            ((char *)GFX_1_BASE + 64*SPR_BANK_WEATHER6)
// #define SPR_WEATHER7            ((char *)GFX_1_BASE + 64*SPR_BANK_WEATHER7)
// #define SPR_WEATHER8            ((char *)GFX_1_BASE + 64*SPR_BANK_WEATHER8)
// #define SPR_WEATHER9            ((char *)GFX_1_BASE + 64*SPR_BANK_WEATHER9)
// #define SPR_WEATHER10           ((char *)GFX_1_BASE + 64*SPR_BANK_WEATHER10)

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

struct SPRITES {
    // sprite cursor - 2x
    char JOY_CURSOR[64*2];
    // empty battery 4x
    char CHARACTER_BARS[64*4];
    // char pictures
    char CHARACTER_PORTRAITS[64*4];
    // Date icons - empty, as game draws over these sprites
    char DATE_TXT[64*2];
    // time icons
    char TIME_ICONS[64*8];
    // toilet paper - 'gold' currency - and space for 2 more
    char CURRENCY[64*3];
    // text sprite, show number of currency items held by player
    char CURRENCY_TXT[64*1]; 
    // weather pictures
    char WEATHER[64*10];
};

const struct AUX_GFX {
    // task icons
    char TASK_ICONS[64*14];
}

extern __export const struct SPRITES SpriteResources;
extern __export const struct AUX_GFX AuxResources;

extern const char SPR_CURRENCY_COLORS[1];
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
#pragma region( bankMainGfx1, GFX_1_SPR_SRC_ADR, GFX_1_AUX_SRC_ADR, , MAIN_GFX_BANK, { mainGfxSprites }, GFX_1_SPR_DST_ADR )
#pragma region( bankMainGfx2, GFX_1_AUX_SRC_ADR, GFX_1_AUX_SRC_ADR + 0x1000, , MAIN_GFX_BANK, { mainGfxAux })
// !!! $1000 gap here for Faces or some other sprites
#pragma region( bankMainGfx3, GFX_1_FNT_SRC_ADR, 0xb800, , MAIN_GFX_BANK, { mainGfxFont }, GFX_1_FNT_DST_ADR )
#pragma region( bankMainGfx4, 0xb800, 0xbfff, , MAIN_GFX_BANK, { mainGfxCode, mainGfxData } )

#pragma code ( mainGfxCode )
#pragma data ( mainGfxData )

void loadMainGfx();

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )

#pragma compile("mainGfx.c")

#endif