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

// main font
#define GFX_1_FNT_SRC_ADR 0xb000
#define GFX_1_FNT_DST_ADR 0xd800
#define GFX_1_FNT2_DST_ADR 0xd000

// screen and bitmap addresses
#define GFX_1_SCR_ADR 0xc000
#define GFX_1_SCR2_ADR 0xe000
#define GFX_1_SCR3_ADR 0xe400
#define GFX_1_BMP_ADR 0xe000
// used for title screen
#define SEC_SCR_ADR 0xc000
#define SEC_BMP_ADR 0xe000
// used for UI sprites - they go to screen memory
#define UI_SCR_ADR 0xb800
#define UI_SPR_ADR 0xb000
#define UI_BMP_ADR 0x8000
// (unsigned int)(UI_SCR_ADR / 0x40) - the start bank of sprites
#define UI_SPRITE_BANKS_START_VALUE 0xc0
#define UI_SPR ((volatile char *)UI_SPR_ADR)

// any sprites and data that are only copied into Vic bank as needed (can be outside VIC addressable memory)
#define GFX_1_AUX_SRC_ADR 0x9000
#define GFX_1_AUX_DST_ADR UI_BMP_ADR

// music
// bound to PLAYLIST defined in optionsMenu.c
#define MAIN_MENU_SONG 2
#define VILLIAGE_MAP_SONG 5
// used in loader only
#define MSX_MAIN_THEME_SID_IDX 0
#define MSX_MAIN_THEME_SNG_IDX 1
#define MSX_SRC_ADR_0 0x8000
#define MSX_SRC_ADR_1 0xa000
#define MSX_DST_ADR 0x8000
#define MSX_INIT $8000
#define MSX_PLAY $8003
#define MSX_ROM $36

// *****************************
// pointers made from the above
// *****************************
#define GFX_1_BMP ((char *)GFX_1_BMP_ADR)
#define GFX_1_SCR ((char *)GFX_1_SCR_ADR)
#define GFX_1_SCR2 ((char *)GFX_1_SCR2_ADR)
#define GFX_1_SCR3 ((char *)GFX_1_SCR3_ADR)
#define UI_SCR ((char *)UI_SCR_ADR)
// auxiliary sprites, sprite fonts and color tables, copied to destination bank on use
#define GFX_1_AUX_SRC ((char *)GFX_1_AUX_SRC_ADR)
#define GFX_1_AUX ((volatile char *)GFX_1_AUX_DST_ADR)
#define GFX_1_FNT_SRC ((char *)GFX_1_FNT_SRC_ADR)
#define GFX_1_FNT ((volatile char *)GFX_1_FNT_DST_ADR)
#define GFX_1_FNT2 ((volatile char *)GFX_1_FNT2_DST_ADR)
#define GFX_1_SPR_SRC ((char *)GFX_1_SPR_SRC_ADR)
// #define GFX_1_SPR ((volatile char *)GFX_1_SPR_DST_ADR)

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
#define d018_txt2_1 (((unsigned)GFX_1_SCR_ADR >> 6) & 0xf0) | (((unsigned)GFX_1_FNT2_DST_ADR >> 10) & 0x0e)
#define d018_txt2_2 (((unsigned)GFX_1_SCR2_ADR >> 6) & 0xf0) | (((unsigned)GFX_1_FNT2_DST_ADR >> 10) & 0x0e)
#define d018_txt2_3 (((unsigned)GFX_1_SCR3_ADR >> 6) & 0xf0) | (((unsigned)GFX_1_FNT2_DST_ADR >> 10) & 0x0e)
#define dd00_UI ((UI_BMP_ADR ^ 0xffff) >> 14)
#define d018_UI (((unsigned)UI_SCR_ADR >> 6) & 0xf0) | (((unsigned)UI_BMP_ADR >> 10) & 0x0e)

// *******************
// additional defines
// *******************
#define COLOR_RAM ((char *)0xd800)
#define TRANSLATION_SRC ((char *)0x8000)
#define TRANSLATION_DST ((char *)0x6000)

static char * const MENU_CODE_SRC = (char *)0x8000;
static char * const MENU_CODE_DST = (char *)0x7000;

#define TBL_V "\x7e"

#endif
