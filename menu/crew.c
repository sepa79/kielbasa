#include <c64/charwin.h>
#include <c64/keyboard.h>
#include <c64/vic.h>
#include <c64/types.h>
#include <stdio.h>

#include <engine/gameState.h>
#include <character/character.h>
#include <menu/menuSystem.h>
#include <menu/crew.h>
#include <engine/gameSettings.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>

#pragma data ( data )

static volatile char charPicColor = 0;
#pragma data ( crewGfxDay )

// gfx and its fonts
__export const char crewScr[] = {
    #embed "assets/charGfx/zaloga - (8bpc, 40x25) Map.bin"
};
__export const char crewFnt[] = {
    #embed 0x400 0x400 "assets/charGfx/zaloga - Chars.bin"
};
__export const char crewCol[] = {
    #embed "assets/charGfx/zaloga - (8bpc, 40x25) ColourMap.bin"
};

#pragma code ( crewCodeRam )

// copy character hidden sprite data to visible screen sprite
static void _setCharacterPic(char character_nr){

    // if we are in the TOP part of the frame - wait for the MIDDLE part
    if(gms_framePos == FRAME_TOP){
        while(gms_framePos != FRAME_MIDDLE){};
    }

    // copy
    char pbank = setBank(MAIN_GFX_BANK);

    #pragma unroll (page)
    for(char i=0; i<63; i++) {
        ((char*) MENU_SPRITE_DST)[i] = allCharacters[character_nr].picture[i];
    }
    charPicColor = allCharacters[character_nr].picture[63];

    setBank(pbank);
}

static void _fontLoader(){
    char pbank = setBank(MENU_BANK_CREW);
    char pport = setPort(MMAP_ROM);

    // rom to buffer -> GFX_1_SCR
    memcpy(GFX_1_SCR, crewFnt, 0x400);
    // switch IO off
    setPort(MMAP_RAM);
    // buffer to RAM under IO
    memcpy(GFX_1_FNT+0x400, GFX_1_SCR, 0x400);
    setPort(pport);
}

static void _closeMenu(){
    // reload fonts
    char pBank = setBank(MAIN_GFX_BANK);
    char pport = setPort(MMAP_ROM);

    // rom to buffer -> GFX_1_SCR
    memcpy(GFX_1_SCR, GFX_1_FNT_SRC+0x400, 0x400);
    // switch IO off
    setPort(MMAP_RAM);
    // buffer to RAM under IO
    memcpy(GFX_1_FNT+0x400, GFX_1_SCR, 0x400);
    setPort(pport);
    setBank(pBank);

    // switchScreenTo(SCREEN_TRANSITION);
    loadMenu(MENU_BANK_MAIN_MENU);
    showMenu();
}

#pragma data ( crewData )

#define BARS_X_POSITION 26
#define BARS_Y_POSITION 16
#define BAR_0 0xb5

static void _drawBarsFor(char character_new) {
    for ( char i=0; i<SKILL_COUNT; i++ ) {

        char heightPixels = lmuldiv16s(allCharacters[character_new].skill[i], 48, 100);
        char heightChars = heightPixels/8;
        char reminder = heightPixels - heightChars*8;

        char * barPlace = GFX_1_SCR + 40*BARS_Y_POSITION+BARS_X_POSITION + (i*3);
        for ( char ci=0; ci<6; ci++ ) {
            if(heightChars > ci){
                barPlace[0] = BAR_0+8;
            } else if(heightChars == ci){
                barPlace[0] = BAR_0+reminder;
            } else {
                barPlace[0] = BAR_0;
            }
            barPlace -= 40;
        }

    }
}

// menu code is in ROM - data in RAM
#pragma code ( crewCode )
#pragma data ( data )
static char _selectedChar = 0;

static void _crewCodeLoader() {
    memcpy(MENU_CODE_DST, (char *)NIGHT_GFX_BMP, 0x1000);
}

static void _displayBackground(){

    memcpy(GFX_1_SCR, crewScr, 25*40);
    // colors
    memcpy(COLOR_RAM, crewCol, 25*40);

    // setBank(pbank);
    vic.color_back1 = VCOL_BLACK;
    vic.color_back2 = VCOL_BROWN;
}

__interrupt static void _showStatsSprites() {
    vic.spr_enable   = 0b00000001;
    vic.spr_expand_x = 0b00000001;
    vic.spr_expand_y = 0b00000001;
    vic.spr_priority = 0b00000000;
    vic.spr_mcolor0  = SPR_CHARACTER_PORTRAIT_MULTICOLOR_1;
    vic.spr_mcolor1  = SPR_CHARACTER_PORTRAIT_MULTICOLOR_2;
    vic.spr_multi    = 0b00000001;
    vic.spr_msbx     = 0b00000000;

    vic.spr_pos[0].x = 2*8+24+2;
    vic.spr_pos[0].y = 2*8+50+3;
    vic.spr_color[0] = charPicColor;

    GFX_1_SCR[OFFSET_SPRITE_PTRS] = SPR_BANK_CREW_SCREEN_CHARACTER_PORTRAIT;
    // vic.color_border--;
}


#define CHAR_0 0x20
#define CHAR_1 0xee
#define CHAR_2 0xef
#define CHAR_3 0xf0
const char NUM_BAR[10][4] = {
    {CHAR_0, CHAR_0, CHAR_0, 0},
    {CHAR_1, CHAR_0, CHAR_0, 0},
    {CHAR_2, CHAR_0, CHAR_0, 0},
    {CHAR_3, CHAR_0, CHAR_0, 0},
    {CHAR_3, CHAR_1, CHAR_0, 0},
    {CHAR_3, CHAR_2, CHAR_0, 0},
    {CHAR_3, CHAR_3, CHAR_0, 0},
    {CHAR_3, CHAR_3, CHAR_1, 0},
    {CHAR_3, CHAR_3, CHAR_2, 0},
    {CHAR_3, CHAR_3, CHAR_3, 0},
};

// Shows character data on the left side of the screen.
static void _showCharacterDetails(byte character){
    // Prepare output window
    CharWin cd;
    cwin_init(&cd, GFX_1_SCR, CHARACTER_DATA_X, CHARACTER_DATA_Y, CHARACTER_DATA_W, CHARACTER_DATA_H);
    // cwin_clear(&cd);
    cwin_putat_string_raw(&cd, 13, 3, TXT[allCharacters[character].nameIdx], VCOL_YELLOW);
    cwin_putat_string_raw(&cd, 13, 5, TXT[allCharacters[character].surnameIdx], VCOL_YELLOW);

    CharWin cStory;
    cwin_init(&cStory, GFX_1_SCR, CHARACTER_STORY_X, CHARACTER_STORY_Y, CHARACTER_STORY_W, CHARACTER_STORY_H);
    cwin_fill(&cStory, 32, VCOL_BLUE);
    cwin_write_string_raw(&cStory, TXT[allCharacters[character].storyTextIdx]);

    cwin_putat_string_raw(&cd, 0, 0, TXT_CACHE[LTXT_IDX_CREW_STAT_STR], VCOL_WHITE);
    cwin_putat_string_raw(&cd, 0, 2, TXT_CACHE[LTXT_IDX_CREW_STAT_AGI], VCOL_WHITE);
    cwin_putat_string_raw(&cd, 0, 4, TXT_CACHE[LTXT_IDX_CREW_STAT_INT], VCOL_WHITE);

    byte str[2];
    sprintf(str, "%u", allCharacters[character].stat[STAT_STR]);
    cwin_putat_string_raw(&cd, 8, 0, str, VCOL_GREEN);
    cwin_putat_string_raw(&cd, 5, 0, NUM_BAR[allCharacters[character].stat[STAT_STR]], 15);
    sprintf(str, "%u", allCharacters[character].stat[STAT_AGI]);
    cwin_putat_string_raw(&cd, 8, 2, str, VCOL_GREEN);
    cwin_putat_string_raw(&cd, 5, 2, NUM_BAR[allCharacters[character].stat[STAT_AGI]], 15);
    sprintf(str, "%u", allCharacters[character].stat[STAT_INT]);
    cwin_putat_string_raw(&cd, 8, 4, str, VCOL_GREEN);
    cwin_putat_string_raw(&cd, 5, 4, NUM_BAR[allCharacters[character].stat[STAT_INT]], 15);

    // cwin_putat_string_raw(&cd, 11,  7, TXT_CACHE[LTXT_IDX_CREW_SKILL_ANI], VCOL_LT_BLUE);
    // cwin_putat_string_raw(&cd, 11,  8, TXT_CACHE[LTXT_IDX_CREW_SKILL_FRM], VCOL_LT_BLUE);
    // cwin_putat_string_raw(&cd, 11,  9, TXT_CACHE[LTXT_IDX_CREW_SKILL_BTH], VCOL_LT_BLUE);
    // cwin_putat_string_raw(&cd, 11, 10, TXT_CACHE[LTXT_IDX_CREW_SKILL_TRD], VCOL_LT_BLUE);

    // sprintf(str, "%u", allCharacters[character].skill[SKILL_ANIMALS]);
    // cwin_putat_string_raw(&cd, 22,  7, str, VCOL_GREEN);
    // sprintf(str, "%u", allCharacters[character].skill[SKILL_FARMING]);
    // cwin_putat_string_raw(&cd, 22,  8, str, VCOL_GREEN);
    // sprintf(str, "%u", allCharacters[character].skill[SKILL_COOKING]);
    // cwin_putat_string_raw(&cd, 22,  9, str, VCOL_GREEN);
    // sprintf(str, "%u", allCharacters[character].skill[SKILL_TRADE]);
    // cwin_putat_string_raw(&cd, 22, 10, str, VCOL_GREEN);

    _drawBarsFor(character);
    _setCharacterPic(character);
}

static void _left(){
    _showCharacterDetails(0);
}
static void _right(){
    _showCharacterDetails(1);
}
static void _up(){
    if(_selectedChar > 0){
        _selectedChar--;
    } else {
        _selectedChar = CHARACTER_COUNT-1;
    }
    _showCharacterDetails(_selectedChar);
}
static void _down(){
    if(_selectedChar < CHARACTER_COUNT-1){
        _selectedChar++;
    } else {
        _selectedChar = 0;
    }
    _showCharacterDetails(_selectedChar);
}

const struct MenuOption CREW_MENU[] = {
    // keep at 1st position, as fire is automatically assigned to this menu entry (bug that became feature)
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_TRANSITION, UI_F & UI_HIDE, &_closeMenu, 0, 2, 5},
//     { TXT_IDX_MENU_A, 'a', SCREEN_FULL_MC_TXT, UI_L+UI_HIDE, &_left, 0, 1, 2},
//     { TXT_IDX_MENU_D, 'd', SCREEN_FULL_MC_TXT, UI_R+UI_HIDE, &_right, 0, 1, 2},
    { TXT_IDX_MENU_W, 'w', SCREEN_FULL_MC_TXT, UI_U+UI_HIDE, &_up, 0, 7, 2},
    { TXT_IDX_MENU_S, 's', SCREEN_FULL_MC_TXT, UI_D+UI_HIDE, &_down, 0, 9, 2},

    END_MENU_CHOICES
};

static void _menuHandler(void){
    mnu_isGfxLoaded = false;
    loadMenuGfx();

    // copy text crew cache from cat to memory
    loadCacheTxt(TXT_CREW_CACHE_INDEX);

    // Prepare output window
    cwin_init(&cw, GFX_1_SCR, 0, 13, 40, 11);
    cwin_clear(&cw);
    displayMenu(CREW_MENU);
    // set global var responsible for background color - used by IRQ Handler
    mcScrBackground = VCOL_ORANGE;
    _displayBackground();

    _selectedChar = 0;
    _showCharacterDetails(_selectedChar);
    switchScreenTo(SCREEN_FULL_MC_TXT);
    updateStatusBar(TXT[SB_IDX_MENU_CREW]);
}

#pragma data ( crewLoaderData )

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &_crewCodeLoader,
    .loadMenuGfx     = &_fontLoader,
    .loadMenuSprites = &menuSpriteLoader,
    .showMenu        = &_menuHandler,
    .showSprites     = &_showStatsSprites,
    .updateMenu      = &menuNoop,
    .runMenuLoop     = &menuNoop,
};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
