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
volatile char prev_character = 255;

#pragma data ( crewGfxDay )

__export const char crewGfx1[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/ramka1_koval.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/ramka1_koval.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/ramka1_koval.kla"
};

#pragma code ( crewCodeRam )

// copy character hidden sprite data to visible screen sprite
static void _setCharacterPic(char character_nr){

    // if we are in the TOP part of the frame - wait for the MIDDLE part
    if(gms_framePos == FRAME_TOP){
        while(gms_framePos != FRAME_MIDDLE){};
    }

    // if any character is choosen
    if(character_nr != NO_CHARACTER){

        char i = 0;
        changeBank(MAIN_GFX_BANK);

        // fast copy
        #pragma unroll (page)
        for(i=0; i<63; i++) {
            ((char*) MENU_SPRITE_DST)[i] = allChars_picture[character_nr][i];
        }

        restoreBank();
    }
}

// small and fast way to write byte on gfx screen
static void _drawByteK(int x, int y, char b) {

    // optimization ??? (to veryfy)
    //
    // unsigned y_masked = ( y & ~7 );
    // unsigned addr = ( y_masked << 5 ) + ( y_masked << 3 ) + ((x & ~7) | (y & 7));

    unsigned addr = 40 * (y & ~7) + ((x & ~7) | (y & 7));
    GFX_1_BMP[addr] = b;
}

// bars height table
const char bar_height[] =   {
    0*BAR_PART_HEIGHT,
    1*BAR_PART_HEIGHT, 2*BAR_PART_HEIGHT, 3*BAR_PART_HEIGHT,
    4*BAR_PART_HEIGHT, 5*BAR_PART_HEIGHT, 6*BAR_PART_HEIGHT,
    7*BAR_PART_HEIGHT, 8*BAR_PART_HEIGHT, 9*BAR_PART_HEIGHT
};
                                   
static void _drawBarsFor(char character) {

    //
    // we drawing from this data (two structures):
    //
    // const byte allChars_stats[CHARACTER_COUNT][3]              = { {3,3,3}, {2,3,2}, {4,2,4}, {3,3,4}};
    // const byte allChars_skills[CHARACTER_COUNT][SKILL_COUNT]   = { {3,3,4,3}, {2,6,1,1}, {1,1,1,7}, {6,2,1,1}};
    //

    // PREPARE FOR DRAWING BARS

    char params[PARAMS_COUNT] = { 1, 2, 3, 4, 5, 6, 7 };

    // (int) mandatory - signed values used (plus/minus)
    int params_diff[PARAMS_COUNT] = { 0, 0, 0, 0, 0, 0, 0 };

    if ( prev_character == NO_CHARACTER ) {
        // if there are no character selected - initialize bars array ( FIX )
        for(char i=0; i<PARAMS_COUNT; i++) {
            params[i] = 0;
        }
    } else if ( prev_character < CHARACTER_SLOTS ) {
        // get previous character bars data
        params[0] = allChars_stats[prev_character][0];
        params[1] = allChars_stats[prev_character][1];
        params[2] = allChars_stats[prev_character][2];

        params[3] = allChars_skills[prev_character][0];
        params[4] = allChars_skills[prev_character][1];
        params[5] = allChars_skills[prev_character][2];
        params[6] = allChars_skills[prev_character][3];
    }

    // calculate bars differences between previous character stats and actually choosen
    params_diff[0] = allChars_stats[character][0] - params[0];
    params_diff[1] = allChars_stats[character][1] - params[1];
    params_diff[2] = allChars_stats[character][2] - params[2];

    params_diff[3] = allChars_skills[character][0] - params[3];
    params_diff[4] = allChars_skills[character][1] - params[4];
    params_diff[5] = allChars_skills[character][2] - params[5];
    params_diff[6] = allChars_skills[character][3] - params[6];

    // bar x position
    char x_draw = BARS_X_POSITION;

    // DRAW BARS

    for ( char i=0; i<PARAMS_COUNT; i++ ) {

        if ( params_diff[i] != 0 ) {
            
            char bar_level     = BARS_Y_POSITION_MAX - bar_height[ params[i] ];
            char bar_new_level = bar_level - ( params_diff[i] * BAR_PART_HEIGHT );      // kick out multiplication out of the loop

            if ( params_diff[i] < 0 ) {
                // clear unused bar part ( downwards )
                for (int y=bar_level; y<=bar_new_level; y++ ) {
                    _drawByteK( x_draw, y, BAR_CLEAR_PATTERN );
                }
            } else if ( params_diff[i] > 0 ) {
                // draw visible bar part ( upwards )
                for (int y=bar_level; y>=bar_new_level; y-- ) {
                    _drawByteK( x_draw, y, BAR_PATTERN );
                }
            }
        }
        // next bar x position
        x_draw += BARS_X_COORDS_GAP;
    }
    prev_character = character;
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
    vic.spr_color[0] = SpriteResources.CHARACTER_PORTRAITS[64*0 + 63];

    GFX_1_SCR[OFFSET_SPRITE_PTRS] = SPR_BANK_CREW_SCREEN_CHARACTER_PORTRAIT;
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

    _drawBarsFor(character);
    _setCharacterPic(character);
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

    prev_character = 255;       // fix bars drawing bug
    _prepareBars();
    _showCharacterDetails(0);

    // Prepare output window
    cwin_init(&cw, GFX_1_SCR, 0, 13, 40, 11);
    cwin_clear(&cw);

    displayMenu(CREW_MENU);
}

#pragma data ( crewLoaderData )

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &_crewCodeLoader,
    .loadMenuGfx     = &menuGfxLoader,
    .loadMenuSprites = &menuSpriteLoader,
    .showMenu        = &_menuHandler,
    .showSprites     = &_showStatsSprites,
    .updateMenu      = &menuNoop,
};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
