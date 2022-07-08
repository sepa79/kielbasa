#ifndef ASSETS_SETTINGS_H
#define ASSETS_SETTINGS_H

#define OFFSET_SPRITE_PTRS 0x3f8

// **************************************
// source addresses on CRT for Koala gfx
// **************************************
#define FULL_KOALA_BMP ((char *)0x8000)
#define FULL_KOALA_SCR ((char *)0x9f40)
#define FULL_KOALA_COL ((char *)0xa328)

// ************************************************************************************
// base values used in pragmas etc, pointers and values are calculated from them later
// ************************************************************************************

// used in calcucations
#define GFX_1_BASE 0xc000
// sprites that are always in Vic bank
// src in cart bank - used in mainGfx.c to define regions
#define GFX_1_SPR_SRC_ADR 0x8000
// where to put in memory
#define GFX_1_SPR_DST_ADR 0xc400
// (unsigned int)(GFX_1_SPR_DST_ADR / 0x40) - the start bank of sprites
#define SPRITE_BANKS_START_VALUE 0x10

// any sprites and data that are only copied into Vic bank as needed (can be outside VIC addressable memory)
#define GFX_1_AUX_SRC_ADR 0x9000
// #define GFX_1_AUX_DST_ADR 0xa000

// main font
#define GFX_1_FNT_SRC_ADR 0xb000
#define GFX_1_FNT_DST_ADR 0xd800

// screen and bitmap addresses
#define GFX_1_SCR_ADR 0xc000
#define GFX_1_BMP_ADR 0xe000
// used for title screen
#define SEC_SCR_ADR 0xc000
#define SEC_BMP_ADR 0xe000

// music
#define MSX_SRC_ADR 0x8000
#define MSX_DST_ADR 0x8000
#define MSX_INIT $8000
#define MSX_PLAY $8003
#define MSX_ROM $36

// *****************************
// pointers made from the above
// *****************************
#define GFX_1_BMP ((char *)GFX_1_BMP_ADR)
#define GFX_1_SCR ((char *)GFX_1_SCR_ADR)
// auxiliary sprites, sprite fonts and color tables, copied to destination bank on use
#define GFX_1_AUX_SRC ((char *)GFX_1_AUX_SRC_ADR)
#define GFX_1_AUX ((volatile char *)GFX_1_AUX_DST_ADR)
#define GFX_1_FNT_SRC ((char *)GFX_1_FNT_SRC_ADR)
#define GFX_1_FNT ((volatile char *)GFX_1_FNT_DST_ADR)
#define GFX_1_SPR_SRC ((char *)GFX_1_SPR_SRC_ADR)
#define GFX_1_SPR ((volatile char *)GFX_1_SPR_DST_ADR)

#define GFX_2_BMP ((char *)SEC_BMP_ADR)
#define GFX_2_SCR ((char *)SEC_SCR_ADR)

// *********************************
// values calculated from the above
// *********************************
#define dd00_gfx2 ((SEC_SCR_ADR ^ 0xffff) >> 14)
#define d018_gfx2 (((unsigned)SEC_SCR_ADR >> 6) & 0xf0) | (((unsigned)SEC_BMP_ADR >> 10) & 0x0e)

#define dd00_gfx1 ((GFX_1_SCR_ADR ^ 0xffff) >> 14)
#define d018_gfx1 (((unsigned)GFX_1_SCR_ADR >> 6) & 0xf0) | (((unsigned)GFX_1_BMP_ADR >> 10) & 0x0e)
#define d018_txt1 (((unsigned)GFX_1_SCR_ADR >> 6) & 0xf0) | (((unsigned)GFX_1_FNT_DST_ADR >> 10) & 0x0e)

// *******************
// additional defines
// *******************
#define COLOR_RAM ((char *)0xd800)
#define TRANSLATION_SRC ((char *)0x8000)
#define TRANSLATION_DST ((char *)0x6000)

// static char * const MENU_CODE_SRC = (char *)0x8000;
// static char * const MENU_CODE_DST = (char *)0x7000;

#define TBL_V "\x7e"

#endif
