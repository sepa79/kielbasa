#include <c64/charwin.h>
#include <c64/keyboard.h>
#include <c64/vic.h>
#include <c64/types.h>
#include <stdio.h>

#include <character/character.h>
#include <menu/menuSystem.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>

// Sections and regions
#pragma section( crewLoaderData, 0 )
#pragma section( crewCode, 0 )
#pragma section( crewGfxDay, 0 )
#pragma section( crewGfxNight, 0 )
#pragma region( bankCrewC, 0x8000, 0x9000, , MENU_BANK_CREW, { crewLoaderData, crewCode } )
#pragma region( bankCrewG1, DAY_GFX_BMP, DAY_GFX_BMP+0x1400, , MENU_BANK_CREW, { crewGfxDay } )
#pragma region( bankCrewG2, NIGHT_GFX_BMP, NIGHT_GFX_BMP+0x1400, , MENU_BANK_CREW, { crewGfxNight } )

#pragma data ( crewGfxDay )
__export const char crewGfx1[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/ramka1.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/ramka1.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/ramka1.kla"
};
#pragma data ( crewGfxNight )
__export const char crewGfx2[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/ramka1.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/ramka1.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/ramka1.kla"
};

// menu code is in ROM - data in RAM
#pragma code ( crewCode )
#pragma data ( data )

// column offset for printing character data
#define COL_OFFSET_CHARACTERDATA 12

static byte _faceSprBank = 0;

static void _showStatsSprites() {
    vic.spr_enable   = 0b00000001;
    vic.spr_expand_x = 0b00000001;
    vic.spr_expand_y = 0b00000001;
    vic.spr_priority = 0b00000000;
    vic.spr_mcolor0  = SPR_CHARACTER_PORTRAIT_MULTICOLOR_1;
    vic.spr_mcolor1  = SPR_CHARACTER_PORTRAIT_MULTICOLOR_2;
    vic.spr_multi    = 0b00000001;
    vic.spr_msbx     = 0b00000000;

    vic.spr_pos[0].x = 5*8+24;
    vic.spr_pos[0].y = 5*8+50;
    vic.spr_color[0] =  SpriteResources.CHARACTER_PORTRAITS[64*0 + 63];

    GFX_1_SCR[OFFSET_SPRITE_PTRS] = SPR_BANK_CHARACTER_PORTRAIT1;
}

// Shows character data on the left side of the screen.
static void _showCharacterDetails(byte character){
    // Prepare output window
    CharWin cd;
    cwin_init(&cd, GFX_1_SCR, COL_OFFSET_CHARACTERDATA, SCREEN_Y_START, SCREEN_WIDTH-COL_OFFSET_CHARACTERDATA, SCREEN_HEIGHT);
    cwin_clear(&cd);
    cwin_putat_string_raw(&cd, 0, 0, TXT[allChars_nameIdx[character]], VCOL_YELLOW);

    CharWin cStory;
    cwin_init(&cStory, GFX_1_SCR, COL_OFFSET_CHARACTERDATA, SCREEN_Y_START+1, SCREEN_WIDTH-COL_OFFSET_CHARACTERDATA, 6);
    cwin_fill(&cStory, 32, VCOL_LT_GREY);
    cwin_write_string_raw(&cStory, TXT[allChars_storyTxtIdx[character]]);

    cwin_putat_string_raw(&cd, 0, 7, TXT[TXT_IDX_STAT_INT], VCOL_LT_BLUE);
    cwin_putat_string_raw(&cd, 0, 8, TXT[TXT_IDX_STAT_STR], VCOL_LT_BLUE);
    cwin_putat_string_raw(&cd, 0, 9, TXT[TXT_IDX_STAT_CUN], VCOL_LT_BLUE);

    byte str[2];
    sprintf(str, "%u", allChars_stats[character][STAT_INT]);
    cwin_putat_string_raw(&cd, 9, 7, str, VCOL_GREEN);
    sprintf(str, "%u", allChars_stats[character][STAT_STR]);
    cwin_putat_string_raw(&cd, 9, 8, str, VCOL_GREEN);
    sprintf(str, "%u", allChars_stats[character][STAT_CUN]);
    cwin_putat_string_raw(&cd, 9, 9, str, VCOL_GREEN);

    cwin_putat_string_raw(&cd, 11,  7, TXT[TXT_IDX_SKILL_ANI], VCOL_LT_BLUE);
    cwin_putat_string_raw(&cd, 11,  8, TXT[TXT_IDX_SKILL_FRM], VCOL_LT_BLUE);
    cwin_putat_string_raw(&cd, 11,  9, TXT[TXT_IDX_SKILL_BTH], VCOL_LT_BLUE);
    cwin_putat_string_raw(&cd, 11, 10, TXT[TXT_IDX_SKILL_TRD], VCOL_LT_BLUE);

    sprintf(str, "%u", allChars_skills[character][SKILL_BREEDING]);
    cwin_putat_string_raw(&cd, 22,  7, str, VCOL_GREEN);
    sprintf(str, "%u", allChars_skills[character][SKILL_FARMING]);
    cwin_putat_string_raw(&cd, 22,  8, str, VCOL_GREEN);
    sprintf(str, "%u", allChars_skills[character][SKILL_BUTCHERY]);
    cwin_putat_string_raw(&cd, 22,  9, str, VCOL_GREEN);
    sprintf(str, "%u", allChars_skills[character][SKILL_TRADE]);
    cwin_putat_string_raw(&cd, 22, 10, str, VCOL_GREEN);
}

static void _emMenu1(){
    _showCharacterDetails(0);
}
static void _emMenu2(){
    _showCharacterDetails(1);
}
static void _emMenu3(){
    _showCharacterDetails(2);
}
static void _emMenu4(){
    _showCharacterDetails(3);
}

const struct MenuOption CREW_MENU[] = {
    { TXT_IDX_MENU_CREW1, '1', UI_UD, &_emMenu1, 0, 1, 1},
    { TXT_IDX_MENU_CREW2, '2', UI_UD, &_emMenu2, 0, 1, 2},
    { TXT_IDX_MENU_CREW3, '3', UI_UD, &_emMenu3, 0, 1, 3},
    { TXT_IDX_MENU_CREW4, '4', UI_UD, &_emMenu4, 0, 1, 4},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, UI_UD, &showMenu, MENU_BANK_MAIN_MENU, 2, 5},
    END_MENU_CHOICES
};

static void _menuHandler(void){
    loadMenuGfx(cal_isDay);

    // Prepare output window
    cwin_init(&cw, GFX_1_SCR, 0, 13, 40, 11);
    cwin_clear(&cw);

    displayMenu(CREW_MENU);
}

#pragma data ( crewLoaderData )

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &menuNoop,
    .loadMenuGfx     = &menuGfxLoader,
    .loadMenuSprites = &menuSpriteLoader,
    .showMenu        = &_menuHandler,
    .showSprites     = &_showStatsSprites,
    .updateMenu      = &menuNoop,
};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
