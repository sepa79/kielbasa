#include <c64/vic.h>
#include <c64/charwin.h>
#include <c64/rasterirq.h>

#include <menu/menuSystem.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <engine/spriteText.h>
#include <assets/assetsSettings.h>

// Sections and regions
#pragma section(tvScreenLoaderData, 0)
#pragma section(tvScreenCode, 0)
#pragma section(tvScreenData, 0)
#pragma section(tvScreenGfx, 0)
#pragma section(tvScreenFnt, 0)
#pragma section(tvScreenSprites, 0)

#pragma region(regionTvScreenC, 0x8000, 0x9000, , MENU_BANK_TV_SCREEN, { tvScreenLoaderData, tvScreenCode, tvScreenData })
#pragma region(regionTvScreenG1, DAY_GFX_BMP, DAY_GFX_BMP + 0x2711, , MENU_BANK_TV_SCREEN, { tvScreenGfx })
#pragma region(regionTvScreenG2, DAY_GFX_BMP + 0x2711, DAY_GFX_BMP + 0x2711 + 1024, , MENU_BANK_TV_SCREEN, { tvScreenFnt })

#pragma data(tvScreenGfx)
__export const char tvScreenGfx1[] = {
    #embed 0xffff 2 "assets/multicolorGfx/tv.kla"
};

#pragma data(tvScreenFnt)
__export const char tvScreenFont[] = {
    #embed 1024 "assets/gameFont - Chars.bin"
};

#pragma data(tvScreenData)

const char * scrolltext[] = {
"This sample uses",
"six sprites",
"multiplexed six",
"times to provide",
"twelve lines of",
"text with eighteen",
"characters each",
"",
"each character is",
"extracted from a",
"custom font into a",
"new sprite and the",
"sprites are then",
"moved up the",
"screen",
"",
"interrupts are",
"used to switch the",
"sprite vertical",
"position and",
"graphics",
"data every two",
"character lines"
"",
"",
"",
"",
};

#pragma code(tvScreenCode)
#pragma data(data)

// c800 is free to use by menu
char * spriteset = ((char *)0xc800);

static void _loadFullKoalaToBMP2(){
    // load colors
    char i = 0;
    do {
#assign y 0
#repeat
        GFX_2_SCR[y + i] = MENU_FULL_KOALA_SCR[y + i];
        COLOR_RAM[y + i] = MENU_FULL_KOALA_COL[y + i];
#assign y y + 256
#until y == 1024
        i++;
    } while (i != 0);

    // load bitmap
    i = 0;
    do {
#assign y 0
#repeat
        GFX_2_BMP[y + i] = MENU_FULL_KOALA_BMP[y + i];
#assign y y + 256
#until y == 8192
        i++;
    } while (i != 0);
#undef y
}


static void readline(char * dp, char n){
    const char * sp = scrolltext[n];

    // Count number of characters

    char s = 0;
    while (sp[s])
        s++;
    
    // Left padding

    char l = (18 - s) >> 1;
    char i = 0;
    while (i < l)
        dp[i++] = ' ';

    // Text in center
    s = 0;
    while (sp[s])
        dp[i++] = sp[s++];

    // Right padding
    while (i < 18)
        dp[i++] = ' ';
}

// Expand six characters into sprite
static void expandline(const char * line, char sppos, char ty){
    // Target position for first character
    char * dp = spriteset + 64 * sppos + 3 * ty;


    char xl = 0;
    for(char x=0; x<6; x++)
    {
        // Source character data
        const char * sp = tvScreenFont + 8 * (line[x] & 0x1f);

        // Copy eight rows
        dp[ 0] = sp[0];	dp[ 3] = sp[1];	dp[ 6] = sp[2];	dp[ 9] = sp[3];
        dp[12] = sp[4];	dp[15] = sp[5];	dp[18] = sp[6];	dp[21] = sp[7];

        // Next character
        dp++;
        xl++;

        // Advance to next sprite
        if (xl == 3)
        {
            dp += 61;
            xl = 0;
        }
    }
}


RIRQCode * spmux[6], final;
// Vertical pixel offset
sbyte oy = 0;
// Index of top left sprite
char sy = 128;
// Buffer for single line	
char line[20];
// Index into text to scroll
char lpos = 0;

static void init(){
    rirq_init(true);
    spr_init(UI_SCR);

    // Loop over five rows of virtual sprites
    for(char i=0; i<5; i++)
    {
        // Allocate large rirq command with 12 writes
        RIRQCode * sp = rirq_alloc(12);
        spmux[i] = sp;

        // Six sprites per row
        for(char x=0; x<6; x++)
        {
            // Vertical sprite position
            rirq_write(sp, 2 * x + 0, &vic.spr_pos[x].y, 48 * (i + 1) + oy);

            // Sprite graphic
            rirq_write(sp, 2 * x + 1, UI_SCR + 0x3f8 + x, sy + 1 + i);
        }

        // Place in list
        rirq_set(i, 48 * i + 46 + oy, spmux[i]);
    }

    // Final irq at bottom of screen
    rirq_build(&final, 0);
    rirq_set(5, 250, &final);

    // Placing top sprites
    for(int x=0; x<6; x++)
        spr_set(x, true, 40 + 48 * x, oy, 128, 1, false, true, true);

    // Initial sort and start of processing
    rirq_sort();
    rirq_start();
}

static void _mainLoop() {

    for(;;)
    {
        // Wait for end of irq list
        rirq_wait();

        // Advance one pixel line
        oy--;
        if (oy < 0)
        {
            // Advance one row of sprites
            oy += 48;
            sy += 6;
            if (sy == 128 + 36)
                sy = 128;
        }

        // Update interrupt position
        for(char i=0; i<5; i++)
        {
            int ty = 48 * i + 46 + oy;

            // No interrupts below screen bottom
            if (ty < 250)
                rirq_move(i, ty);	
            else
                rirq_clear(i);
        }

        // Sort list
        rirq_sort();

        // Reset row of top level sprites
        char sty = sy;
        for(int x=0; x<6; x++){
            spr_move(x, 40 + 48 * x, oy); 
            spr_image(x, sty);
            sty ++;
        }

        // Update write data in interrupt list	
        for(char i=0; i<5; i++){
            RIRQCode * sp = spmux[i];

            // Check sprite rotation
            if (sty == 128 + 36)
                sty = 128;

            // Update vertical position and sprite data
            char py = 48 * (i + 1) + oy;
            for(char x=0; x<6; x++){
                rirq_data(sp, 2 * x + 0, py);
                rirq_data(sp, 2 * x + 1, sty);
                sty ++;
            }
        }

        // Expand next line of text into bottom row of
        // sprites while invisible
        switch (oy){
        case 46:
        case 42:
            readline(line, lpos);
            lpos++;
            if (lpos == 28)
                lpos = 0;
            break;
        case 45:
            expandline(line +  0, sty - 6 - 128, 0);
            break;
        case 44:
            expandline(line +  6, sty - 4 - 128, 0);
            break;
        case 43:
            expandline(line + 12, sty - 2 - 128, 0);
            break;

        case 41:
            expandline(line +  0, sty - 6 - 128, 12);
            break;
        case 40:
            expandline(line +  6, sty - 4 - 128, 12);
            break;
        case 39:
            expandline(line + 12, sty - 2 - 128, 12);
            break;
        }
    }
}

const struct MenuOption TV_SCREEN_MENU[] = {
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_SPLIT_MC_TXT, UI_LF+UI_HIDE, &showMenu, MENU_BANK_MAIN_MENU, 2, 11},
    END_MENU_CHOICES
};

static void _menuHandler(void) {
    loadMenuGfx();
    // init();
    switchScreenTo(SCREEN_MC_GFX);

    displayMenu(TV_SCREEN_MENU);
    // _mainLoop();
}

#pragma data(tvScreenLoaderData)

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &menuNoop,
    .loadMenuGfx     = &_loadFullKoalaToBMP2,
    .loadMenuSprites = &menuNoop,
    .showMenu        = &_menuHandler,
    .showSprites     = &spriteNoop,
    .updateMenu      = &menuNoop,
    .runMenuLoop     = &menuNoop,
};

// Switching code generation back to shared section
#pragma code(code)
#pragma data(data)
