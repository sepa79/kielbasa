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

// remember previous character; mandatory for optimized bars drawing
static char character_old = 0;
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

#pragma data ( crewData )
// bars height table
static const char bar_height[101] =   {
    BARS_Y_POSITION_MAX - 0,
    BARS_Y_POSITION_MAX - 1,
    BARS_Y_POSITION_MAX - 1,
    BARS_Y_POSITION_MAX - 1,
    #assign ry 4
    #repeat
        BARS_Y_POSITION_MAX - ((ry * BAR_PART_HEIGHT) / 2),
    #assign ry ry + 1
    #until ry == 101
    #undef ry
};

static void _drawBarsFor(char character_new) {

    //
    // we drawing from this data (two structures):
    //
    // const byte allCharacters[CHARACTER_COUNT].stat[3]              = { {3,3,3}, {2,3,2}, {4,2,4}, {3,3,4}};
    // const byte allCharacters[CHARACTER_COUNT].skill[SKILL_COUNT]   = { {3,3,4,3}, {2,6,1,1}, {1,1,1,7}, {6,2,1,1}};
    //

    // PREPARE FOR DRAWING BARS

    char params_old[] = { 0, 0, 0, 0, 0, 0, 0 };
    char params_new[] = { 0, 0, 0, 0, 0, 0, 0 };

    // if there was character choosen previously
    if ( character_old != NO_CHARACTER ) {
        // get old character bars data
        params_old[0] = allCharacters[character_old].stat[0]*10;
        params_old[1] = allCharacters[character_old].stat[1]*10;
        params_old[2] = allCharacters[character_old].stat[2]*10;

        params_old[3] = allCharacters[character_old].skill[0];
        params_old[4] = allCharacters[character_old].skill[1];
        params_old[5] = allCharacters[character_old].skill[2];
        params_old[6] = allCharacters[character_old].skill[3];
    }

    // get new character bars data
    params_new[0] = allCharacters[character_new].stat[0]*10;
    params_new[1] = allCharacters[character_new].stat[1]*10;
    params_new[2] = allCharacters[character_new].stat[2]*10;

    params_new[3] = allCharacters[character_new].skill[0];
    params_new[4] = allCharacters[character_new].skill[1];
    params_new[5] = allCharacters[character_new].skill[2];
    params_new[6] = allCharacters[character_new].skill[3];

    // DRAW BARS

    // bar x position
    char x_draw = BARS_X_POSITION;

    for ( char i=0; i<PARAMS_COUNT; i++ ) {

        // invert coords
        // char bar_level_old = bar_height[ params_old[i] ];
        // char bar_level_new = bar_height[ params_new[i] ];

        // // draw or erase bar or bar part
        // int diff = bar_level_new - bar_level_old;
        // if ( diff < 0 ) {
        //     for ( char y=bar_level_old; y>=bar_level_new; y-- ) {
        //         _drawByteK( x_draw, y, BAR_PATTERN );
        //     }
        // } else if ( diff > 0 ) {
        //     for ( char y=bar_level_old; y<bar_level_new; y++ ) {
        //         _drawByteK( x_draw, y, BAR_PATTERN_CLEAR );
        //     }
        // }

        // // next bar x position
        // if(i == 2){
        //     // make a gap between Skills and Stats
        //     x_draw += BARS_X_COORDS_GAP;
        //     x_draw += BARS_X_COORDS_GAP;
        // }
        // x_draw += BARS_X_COORDS_GAP;
    }

    // remember character, so we can later draw/clear only difference on the bar
    character_old = character_new;
}

static void _prepareBars(){
    // // set screen colors under bars
    // for ( unsigned y=3; y<11; y++ ) {
    //     for ( unsigned x=15; x<20; x+=2 ) { 
    //         GFX_1_SCR[y*40+x] = 0x96;
    //     }
    //     for ( unsigned x=21; x<33; x+=2 ) { 
    //         GFX_1_SCR[y*40+x] = 0x97;
    //     }
    // }
}

// menu code is in ROM - data in RAM
#pragma code ( crewCode )
#pragma data ( data )

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

// Shows character data on the left side of the screen.
static void _showCharacterDetails(byte character){
    // Prepare output window
    CharWin cd;
    cwin_init(&cd, GFX_1_SCR, CHARACTER_DATA_X, CHARACTER_DATA_Y, CHARACTER_DATA_W, CHARACTER_DATA_H);
    // cwin_clear(&cd);
    cwin_putat_string_raw(&cd, 11, 1, TXT[allCharacters[character].nameIdx], VCOL_YELLOW);

    CharWin cStory;
    cwin_init(&cStory, GFX_1_SCR, CHARACTER_STORY_X, CHARACTER_STORY_Y, CHARACTER_STORY_W, CHARACTER_STORY_H);
    cwin_fill(&cStory, 32, VCOL_BLUE);
    cwin_write_string_raw(&cStory, TXT[allCharacters[character].storyTextIdx]);

    cwin_putat_string_raw(&cd, 0, 0, TXT_CACHE[LTXT_IDX_CREW_STAT_STR], VCOL_WHITE);
    cwin_putat_string_raw(&cd, 0, 2, TXT_CACHE[LTXT_IDX_CREW_STAT_CUN], VCOL_WHITE);
    cwin_putat_string_raw(&cd, 0, 4, TXT_CACHE[LTXT_IDX_CREW_STAT_INT], VCOL_WHITE);

    byte str[2];
    sprintf(str, "%u", allCharacters[character].stat[STAT_STR]);
    cwin_putat_string_raw(&cd, 6, 0, str, VCOL_GREEN);
    sprintf(str, "%u", allCharacters[character].stat[STAT_CUN]);
    cwin_putat_string_raw(&cd, 6, 2, str, VCOL_GREEN);
    sprintf(str, "%u", allCharacters[character].stat[STAT_INT]);
    cwin_putat_string_raw(&cd, 6, 4, str, VCOL_GREEN);

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

    // _drawBarsFor(character);
    _setCharacterPic(character);
}

static void _crewMenu1(){
    _showCharacterDetails(0);
}
static void _crewMenu2(){
    _showCharacterDetails(1);
}
static void _crewMenu3(){
    _showCharacterDetails(2);
}
static void _crewMenu4(){
    _showCharacterDetails(3);
}

const struct MenuOption CREW_MENU[] = {
    { TXT_IDX_MENU_CREW1, '1', SCREEN_FULL_MC_TXT, UI_SELECT & UI_HIDE, &_crewMenu1, 0, 1, 1},
    { TXT_IDX_MENU_CREW2, '2', SCREEN_FULL_MC_TXT, UI_SELECT & UI_HIDE, &_crewMenu2, 0, 1, 2},
    { TXT_IDX_MENU_CREW3, '3', SCREEN_FULL_MC_TXT, UI_SELECT & UI_HIDE, &_crewMenu3, 0, 1, 3},
    { TXT_IDX_MENU_CREW4, '4', SCREEN_FULL_MC_TXT, UI_SELECT & UI_HIDE, &_crewMenu4, 0, 1, 4},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_TRANSITION, UI_LF & UI_HIDE, &showMenu, MENU_BANK_MAIN_MENU, 2, 5},
    END_MENU_CHOICES
};

static void _menuHandler(void){
    mnu_isGfxLoaded = false;
    loadMenuGfx();

    // copy text crew cache from cat to memory
    loadCacheTxt(TXT_CREW_CACHE_INDEX);

    // Bars and portrait
    // _prepareBars();
    // char select_character = character_old;
    // character_old = NO_CHARACTER;       // fix bars drawing bug
    // _showCharacterDetails(select_character);

    // Prepare output window
    cwin_init(&cw, GFX_1_SCR, 0, 13, 40, 11);
    cwin_clear(&cw);
    displayMenu(CREW_MENU);
    mcScrBackground = VCOL_ORANGE;
    _displayBackground();

    _crewMenu1();
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
