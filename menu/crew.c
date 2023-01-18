#include <c64/charwin.h>
#include <c64/keyboard.h>
#include <c64/vic.h>
#include <c64/types.h>
#include <stdio.h>

#include <character/character.h>
#include <menu/menuSystem.h>
#include <menu/crew.h>
#include <engine/gameSettings.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>

#pragma data ( data )

// remember previous character; mandatory for optimized bars drawing
volatile char character_old = 0;

#pragma data ( crewGfxDay )

__export const char crewGfx1[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/ramka1_koval.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/ramka1_koval.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/ramka1_koval.kla"
};

#pragma code ( crewCodeRam )

// copy character hidden sprite data to visible screen sprite
static void _setCharacterPic(char character_nr){

    // if any character is choosen
    if(character_nr != NO_CHARACTER){

        // if we are in the TOP part of the frame - wait for the MIDDLE part
        if(gms_framePos == FRAME_TOP){
            while(gms_framePos != FRAME_MIDDLE){};
        }

        // copy
        char pbank = setBank(MAIN_GFX_BANK);

        #pragma unroll (page)
        for(char i=0; i<63; i++) {
            // ((char*) MENU_SPRITE_DST)[i] = allChars_picture[character_nr][i];
        }

        setBank(pbank);
    }
}

// small and fast way to write byte on gfx screen
static void _drawByteK(int x, int y, char b) {
    unsigned addr = 40 * (y & ~7) + ((x & ~7) | (y & 7));
    GFX_1_BMP[addr] = b;
}

// bars height table
const char bar_height[] =   {
    BARS_Y_POSITION_MAX-0*BAR_PART_HEIGHT, BARS_Y_POSITION_MAX-1*BAR_PART_HEIGHT,
    BARS_Y_POSITION_MAX-2*BAR_PART_HEIGHT, BARS_Y_POSITION_MAX-3*BAR_PART_HEIGHT,
    BARS_Y_POSITION_MAX-4*BAR_PART_HEIGHT, BARS_Y_POSITION_MAX-5*BAR_PART_HEIGHT,
    BARS_Y_POSITION_MAX-6*BAR_PART_HEIGHT, BARS_Y_POSITION_MAX-7*BAR_PART_HEIGHT,
    BARS_Y_POSITION_MAX-8*BAR_PART_HEIGHT, BARS_Y_POSITION_MAX-9*BAR_PART_HEIGHT
};

static void _drawBarsFor(char character_new) {

    // //
    // // we drawing from this data (two structures):
    // //
    // // const byte allChars_stats[CHARACTER_COUNT][3]              = { {3,3,3}, {2,3,2}, {4,2,4}, {3,3,4}};
    // // const byte allChars_skills[CHARACTER_COUNT][SKILL_COUNT]   = { {3,3,4,3}, {2,6,1,1}, {1,1,1,7}, {6,2,1,1}};
    // //

    // // PREPARE FOR DRAWING BARS

    // char params_old[] = { 0, 0, 0, 0, 0, 0, 0 };
    // char params_new[] = { 0, 0, 0, 0, 0, 0, 0 };

    // // if there was character choosen previously
    // if ( character_old < CHARACTER_SLOTS ) {
    //     // get old character bars data
    //     params_old[0] = allChars_stats[character_old][0];
    //     params_old[1] = allChars_stats[character_old][1];
    //     params_old[2] = allChars_stats[character_old][2];

    //     params_old[3] = allChars_skills[character_old][0];
    //     params_old[4] = allChars_skills[character_old][1];
    //     params_old[5] = allChars_skills[character_old][2];
    //     params_old[6] = allChars_skills[character_old][3];
    // }

    // // get new character bars data
    // params_new[0] = allChars_stats[character_new][0];
    // params_new[1] = allChars_stats[character_new][1];
    // params_new[2] = allChars_stats[character_new][2];

    // params_new[3] = allChars_skills[character_new][0];
    // params_new[4] = allChars_skills[character_new][1];
    // params_new[5] = allChars_skills[character_new][2];
    // params_new[6] = allChars_skills[character_new][3];

    // // DRAW BARS

    // // bar x position
    // char x_draw = BARS_X_POSITION;

    // for ( char i=0; i<PARAMS_COUNT; i++ ) {

    //     // invert coords
    //     char bar_level_old = bar_height[ params_old[i] ];
    //     char bar_level_new = bar_height[ params_new[i] ];

    //     // draw or erase bar or bar part
    //     int diff = bar_level_new - bar_level_old;
    //     if ( diff < 0 ) {
    //         for ( char y=bar_level_old; y>=bar_level_new; y-- ) {
    //             _drawByteK( x_draw, y, BAR_PATTERN );
    //         }
    //     } else if ( diff > 0 ) {
    //         for ( char y=bar_level_old; y<bar_level_new; y++ ) {
    //             _drawByteK( x_draw, y, BAR_PATTERN_CLEAR );
    //         }
    //     }

    //     // next bar x position
    //     x_draw += BARS_X_COORDS_GAP;
    // }

    // // remember character, so we can later draw/clear only difference on the bar
    // character_old = character_new;
}

static void _prepareBars(){
    // set screen colors under bars
    for ( unsigned y=3; y<11; y++ ) {
        for ( unsigned x=15; x<30; x+=2 ) { 
            GFX_1_SCR[y*40+x] = 0x97;
        }
    }
}

// menu code is in ROM - data in RAM
#pragma code ( crewCode )
#pragma data ( data )

static void _crewCodeLoader() {
    memcpy(MENU_CODE_DST, (char *)NIGHT_GFX_BMP, 0x1000);
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

    vic.spr_pos[0].x = 5*8+24;
    vic.spr_pos[0].y = 5*8+50;
    vic.spr_color[0] = SpriteResources.CHARACTER_PORTRAITS[64*0 + 63];

    GFX_1_SCR[OFFSET_SPRITE_PTRS] = SPR_BANK_CREW_SCREEN_CHARACTER_PORTRAIT;
}

// Shows character data on the left side of the screen.
static void _showCharacterDetails(byte character){
    // // Prepare output window
    // CharWin cd;
    // cwin_init(&cd, GFX_1_SCR, COL_OFFSET_CHARACTERDATA, SCREEN_Y_START, SCREEN_WIDTH-COL_OFFSET_CHARACTERDATA, SCREEN_HEIGHT);
    // cwin_clear(&cd);
    // cwin_putat_string_raw(&cd, 0, 0, TXT[allChars_nameIdx[character]], VCOL_YELLOW);

    // CharWin cStory;
    // cwin_init(&cStory, GFX_1_SCR, COL_OFFSET_CHARACTERDATA, SCREEN_Y_START+1, SCREEN_WIDTH-COL_OFFSET_CHARACTERDATA, 6);
    // cwin_fill(&cStory, 32, VCOL_LT_GREY);
    // cwin_write_string_raw(&cStory, TXT[allChars_storyTxtIdx[character]]);

    // cwin_putat_string_raw(&cd, 0, 7, TXT[TXT_IDX_STAT_INT], VCOL_LT_BLUE);
    // cwin_putat_string_raw(&cd, 0, 8, TXT[TXT_IDX_STAT_STR], VCOL_LT_BLUE);
    // cwin_putat_string_raw(&cd, 0, 9, TXT[TXT_IDX_STAT_CUN], VCOL_LT_BLUE);

    // byte str[2];
    // sprintf(str, "%u", allChars_stats[character][STAT_INT]);
    // cwin_putat_string_raw(&cd, 9, 7, str, VCOL_GREEN);
    // sprintf(str, "%u", allChars_stats[character][STAT_STR]);
    // cwin_putat_string_raw(&cd, 9, 8, str, VCOL_GREEN);
    // sprintf(str, "%u", allChars_stats[character][STAT_CUN]);
    // cwin_putat_string_raw(&cd, 9, 9, str, VCOL_GREEN);

    // cwin_putat_string_raw(&cd, 11,  7, TXT[TXT_IDX_SKILL_ANI], VCOL_LT_BLUE);
    // cwin_putat_string_raw(&cd, 11,  8, TXT[TXT_IDX_SKILL_FRM], VCOL_LT_BLUE);
    // cwin_putat_string_raw(&cd, 11,  9, TXT[TXT_IDX_SKILL_BTH], VCOL_LT_BLUE);
    // cwin_putat_string_raw(&cd, 11, 10, TXT[TXT_IDX_SKILL_TRD], VCOL_LT_BLUE);

    // sprintf(str, "%u", allChars_skills[character][SKILL_BREEDING]);
    // cwin_putat_string_raw(&cd, 22,  7, str, VCOL_GREEN);
    // sprintf(str, "%u", allChars_skills[character][SKILL_FARMING]);
    // cwin_putat_string_raw(&cd, 22,  8, str, VCOL_GREEN);
    // sprintf(str, "%u", allChars_skills[character][SKILL_BUTCHERY]);
    // cwin_putat_string_raw(&cd, 22,  9, str, VCOL_GREEN);
    // sprintf(str, "%u", allChars_skills[character][SKILL_TRADE]);
    // cwin_putat_string_raw(&cd, 22, 10, str, VCOL_GREEN);

    // _drawBarsFor(character);
    // _setCharacterPic(character);
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
    { TXT_IDX_MENU_CREW1, '1', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_emMenu1, 0, 1, 1},
    { TXT_IDX_MENU_CREW2, '2', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_emMenu2, 0, 1, 2},
    { TXT_IDX_MENU_CREW3, '3', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_emMenu3, 0, 1, 3},
    { TXT_IDX_MENU_CREW4, '4', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_emMenu4, 0, 1, 4},
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_SPLIT_MC_TXT, UI_LF, &showMenu, MENU_BANK_MAIN_MENU, 2, 5},
    END_MENU_CHOICES
};

static void _menuHandler(void){
    // mnu_isGfxLoaded = false;
    // loadMenuGfx(cal_isDay);

    // // Bars and portrait
    // _prepareBars();
    // char select_character = character_old;
    // character_old = NO_CHARACTER;       // fix bars drawing bug
    // _showCharacterDetails(select_character);

    // // Prepare output window
    // cwin_init(&cw, GFX_1_SCR, 0, 13, 40, 11);
    // cwin_clear(&cw);

    // displayMenu(CREW_MENU);
}

#pragma data ( crewLoaderData )

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &_crewCodeLoader,
    .loadMenuGfx     = &menuGfxLoaderSingleBitmap,
    .loadMenuSprites = &menuSpriteLoader,
    .showMenu        = &_menuHandler,
    .showSprites     = &_showStatsSprites,
    .updateMenu      = &menuNoop,
};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
