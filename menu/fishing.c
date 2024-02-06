#include <c64/vic.h>
#include <c64/cia.h>
#include <c64/charwin.h>
#include <string.h>
#include <c64/keyboard.h>
#include <c64/rasterirq.h>

#include <menu/menuSystem.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/uiHandler.h>
#include <engine/irqHandler.h>
#include <engine/gameSettings.h>

#include <miniGame/fishing.cpp>

#pragma data(fishingMenuGfxDay)
__export const char fishingMenuGfx1[] = {
    #embed 0xffff 2 "assets/multicolorGfx/fishing_060224a.kla"
};
#pragma data(fishingMenuSprites)
__export const char fishingMenuSprites[] = {
    #embed 0xffff 20 "assets/sprites/rybki.spd"
};

#pragma code(fishingMenuRAMCode)
#pragma data(fishingMenuRAMData)

#define FISH_COUNT 6
#define FISH_MAX_SPEED 2

volatile Fish allFish[FISH_COUNT];

RIRQCode rirqc_frow0, rirqc_frow1, rirqc_frow2, rirqc_frow3;
#define IRQ_RASTER_FROW0 0x70
#define IRQ_RASTER_FROW1 0x80
#define IRQ_RASTER_FROW2 0xa8
#define IRQ_RASTER_FROW3 0xd0
#define FISH_LEVEL_OFFSET 7
#define FISH_Y_SPREAD 16
const char fishLevel[3] = 
{
    IRQ_RASTER_FROW1+FISH_LEVEL_OFFSET,
    IRQ_RASTER_FROW2+FISH_LEVEL_OFFSET,
    IRQ_RASTER_FROW3+FISH_LEVEL_OFFSET,
};

// Thanks Blumba!
inline void _vic_sprxy2(byte s, int x, int y)
{
    vic.spr_pos[s].y = y;
    vic.spr_pos[s + 1].y = y;
    vic.spr_pos[s].x = x & 0xff;
    vic.spr_pos[s + 1].x = x & 0xff;
    if (x & 0x100)
        vic.spr_msbx |= 3 << s;
    else
        vic.spr_msbx &= ~(3 << s);
}

__interrupt static void IRQ_initFishingRows() {
    vic.color_border--;
    vic.spr_expand_x = 0b00000000;
    vic.spr_expand_y = 0b00000000;
    vic.spr_priority = 0b11110000;
    vic.spr_multi    = 0b10101010;
    vic.spr_enable   = 0b11111111;
    vic.color_border++;
    vic.color_back = VCOL_CYAN;

}

__interrupt static void IRQ_fishingRow1() {
    vic.color_border--;

    _vic_sprxy2(2, allFish[0].posX, allFish[0].posY);
    _vic_sprxy2(4, allFish[1].posX, allFish[1].posY);
    _vic_sprxy2(6, allFish[2].posX, allFish[2].posY);

    vic.spr_color[2] = VCOL_BLACK;
    vic.spr_color[4] = VCOL_BLACK;
    vic.spr_color[6] = VCOL_BLACK;
    vic.spr_color[3] = allFish[0].color;
    vic.spr_color[5] = allFish[1].color;
    vic.spr_color[7] = allFish[2].color;

    char b = allFish[0].baseSprBank + allFish[0].frame;
    GFX_2_SCR[OFFSET_SPRITE_PTRS+2] = b+1;
    GFX_2_SCR[OFFSET_SPRITE_PTRS+3] = b;
    b = allFish[1].baseSprBank + allFish[1].frame;
    GFX_2_SCR[OFFSET_SPRITE_PTRS+4] = b+1;
    GFX_2_SCR[OFFSET_SPRITE_PTRS+5] = b;
    b = allFish[2].baseSprBank + allFish[2].frame;
    GFX_2_SCR[OFFSET_SPRITE_PTRS+6] = b+1;
    GFX_2_SCR[OFFSET_SPRITE_PTRS+7] = b;

    vic.color_border++;
}
__interrupt static void IRQ_fishingRow2() {
    vic.color_border--;

    _vic_sprxy2(2, allFish[3].posX, allFish[3].posY);
    _vic_sprxy2(4, allFish[4].posX, allFish[4].posY);

    vic.spr_color[2] = VCOL_BLACK;
    vic.spr_color[4] = VCOL_BLACK;
    vic.spr_color[3] = allFish[3].color;
    vic.spr_color[5] = allFish[4].color;

    char b = allFish[3].baseSprBank + allFish[3].frame;
    GFX_2_SCR[OFFSET_SPRITE_PTRS+2] = b+1;
    GFX_2_SCR[OFFSET_SPRITE_PTRS+3] = b;
    b = allFish[4].baseSprBank + allFish[4].frame;
    GFX_2_SCR[OFFSET_SPRITE_PTRS+4] = b+1;
    GFX_2_SCR[OFFSET_SPRITE_PTRS+5] = b;

    vic.spr_priority = 0b11111100;

    vic.color_border++;
}
__interrupt static void IRQ_fishingRow3() {
    vic.color_border--;

    _vic_sprxy2(2, allFish[5].posX, allFish[5].posY);

    vic.spr_color[2] = VCOL_BLACK;
    vic.spr_color[3] = allFish[5].color;

    char b = allFish[5].baseSprBank + allFish[5].frame;
    GFX_2_SCR[OFFSET_SPRITE_PTRS+2] = b+1;
    GFX_2_SCR[OFFSET_SPRITE_PTRS+3] = b;

    vic.spr_priority = 0b11111100;

    vic.color_border++;
}


__interrupt static void IRQ_topFishing() {
    // vic.color_border++;
    // Select GFX screen
    vic.ctrl1 = VIC_CTRL1_BMM | VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3;
    vic.ctrl2 = VIC_CTRL2_MCM | VIC_CTRL2_CSEL | 0;
    vic.memptr = d018_gfx1;
    cia2.pra = dd00_gfx1;

    // show any sprites the menu might have
    setSpritesTopScr();

    // indicate frame position
    gms_framePos = FRAME_TOP;

    playMsx();

    // vic.color_border--;
}

__interrupt static void IRQ_bottomUI() {
    // revert color
    vic.color_back = VCOL_BLACK;
    // Set screen height to 24 lines - this is done after the border should have started drawing - so it wont start
    vic.ctrl1 &= (0xff^VIC_CTRL1_RSEL);
    while (vic.raster != 0xfc){}
    // reset to normal screen height
    vic.ctrl1 = VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3;
    showUiSpritesBottom();
    // UI sprite bank
    vic.memptr = d018_UI;
    cia2.pra = dd00_UI;

    // vic.color_border++;
    playMsx();
    if(!gms_enableMusic){
        // wait a few lines as msx is off, we don't want to desync the screen.
        while (vic.raster != 0x10){}
    }

    joyUpdate();

}

void initRasterIRQ_Fishing(){
    // vic.color_border--;

    // Top - switch to txt
    rirq_build(&rirqc_topScreen, 1);
    rirq_call(&rirqc_topScreen, 0, IRQ_topFishing);
    rirq_set(1, IRQ_RASTER_TOP_MC_SCREEN, &rirqc_topScreen);

    // Middle - play msx
    // rirq_build(&rirqc_middleScreen, 1);
    // rirq_call(&rirqc_middleScreen, 0, IRQ_middleScreenMsx);
    // rirq_set(2, IRQ_RASTER_MIDDLE_TXT_SCREEN, &rirqc_middleScreen);
    rirq_clear(2);
    // Bottom Sprites

    rirq_build(&rirqc_bottomUI, 1);
    rirq_call(&rirqc_bottomUI, 0, IRQ_bottomUI);
    rirq_set(3, IRQ_RASTER_BOTTOM_UI, &rirqc_bottomUI);

    // Init all fish sprites
    rirq_build(&rirqc_frow0, 1);
    rirq_call(&rirqc_frow0, 0, IRQ_initFishingRows);
    rirq_set(10, IRQ_RASTER_FROW0, &rirqc_frow0);

    // 1st row of fish
    rirq_build(&rirqc_frow1, 1);
    rirq_call(&rirqc_frow1, 0, IRQ_fishingRow1);
    rirq_set(11, IRQ_RASTER_FROW1, &rirqc_frow1);

    // 2nd row of fish
    rirq_build(&rirqc_frow2, 1);
    rirq_call(&rirqc_frow2, 0, IRQ_fishingRow2);
    rirq_set(12, IRQ_RASTER_FROW2, &rirqc_frow2);

    // 3rd row of fish
    rirq_build(&rirqc_frow3, 1);
    rirq_call(&rirqc_frow3, 0, IRQ_fishingRow3);
    rirq_set(13, IRQ_RASTER_FROW3, &rirqc_frow3);

    // sort the raster IRQs
    rirq_sort();
    // vic.color_border++;
}

#pragma code(fishingMenuCode)
#pragma data(data)
const char fishColors[5] = {3, 7, 13, 10, 14};

// destination table
__striped char * const yDstT[200] = {
#for(i,200) GFX_1_BMP + 40 * (i & ~7)  + (i & 7),
};

// source table
__striped char * const ySrcT[200] = {
#for(i,200) MENU_FULL_KOALA_BMP + 40 * (i & ~7)  + (i & 7),
};

const char maskT[] = {0b00111111,0b00111111, 0b11001111,0b11001111, 0b11110011,0b11110011, 0b11111100,0b11111100};

static void _drawPoint(char x, char y) {
    char m = x & 7;
    // x must be > 7 and < 256, don't draw near edge of screen
    char xo = (x & ~ 7) - 8;

    yDstT[y][xo] = ySrcT[y][xo];
    xo += 8;
    yDstT[y][xo] = ySrcT[y][xo] & maskT[m];
    xo += 8;
    yDstT[y][xo] = ySrcT[y][xo];
}

static char _rnd(char num){
    return ((rand() & 0xff) * num) >> 8;
}

static Fish _initFish(char level, int x){
    Fish fish;
    fish.posY = fishLevel[level] + _rnd(FISH_Y_SPREAD);
    fish.posX = 50 + x;
    fish.baseSprBank = 0x11 + _rnd(6)*2;
    fish.color = fishColors[_rnd(5)];
    fish.frame = 0;
    fish.speed = _rnd(FISH_MAX_SPEED) + 1;
    fish.level = level;
    fish.speedCounter = _rnd(25) + 15;
    return fish;
}

static void _initAllFish(){
    allFish[0] = _initFish(0, _rnd(255));
    allFish[1] = _initFish(0, _rnd(255));
    allFish[2] = _initFish(0, _rnd(255));
    allFish[3] = _initFish(1, _rnd(255));
    allFish[4] = _initFish(1, _rnd(255));
    allFish[5] = _initFish(2, _rnd(255));
}

static void _fishLoop(){
    // vic.color_border--;
    // char x=50;
    // for(char y=50;y<100;y++){
    //     // x++;
    //     _drawPoint(y, y);
    // }
    // vic.color_border++;

    vic.color_border++;
    for(char fi=0;fi<FISH_COUNT;fi++){
        if(allFish[fi].posX > 3){
            allFish[fi].posX -= allFish[fi].speed;


            allFish[fi].speedCounter--;
            // check if fish should change speed
            if(!allFish[fi].speedCounter){
                allFish[fi].speedCounter = _rnd(25) + 15;
                allFish[fi].speed = _rnd(FISH_MAX_SPEED) + 1;
            }

        } else {
            allFish[fi] = _initFish(allFish[fi].level, 300);
        }
    }
    vic.color_border--;
}


// copy fishingMenuRAMCode
static void _fishingMenuCodeLoader(){
    memcpy(MENU_CODE_DST, (char *)0x8800, 0x0800);
}

void fishingMenuSpriteLoader(){
    // save us some trouble, don't overwrite main cursor (+64 below)
    memcpy((char *)GFX_1_SPR_DST_ADR+64, fishingMenuSprites, 12*64);
}

const struct MenuOption FISHING_MENU[] = {
    // Add the "Exit to Map" option as shown in the example
    { TXT_IDX_EXIT_TO_MAP, KEY_ARROW_LEFT, SCREEN_TRANSITION, UI_LF + UI_HIDE, &showMenu, MENU_BANK_MAP_VILLIAGE_1, 2, 5 },
    END_MENU_CHOICES
};

static void _menuHandler() {
    loadMenuGfx();
    loadMenuSprites();

    displayMenu(FISHING_MENU);
    _initAllFish();
    switchScreenTo(SCREEN_FISHING);
}

#pragma data(fishingMenuLoaderData)

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &_fishingMenuCodeLoader,
    .loadMenuGfx     = &loadFullKoalaToBMP2,
    .loadMenuSprites = &fishingMenuSpriteLoader,
    .showMenu        = &_menuHandler,
    .showSprites     = &spriteNoop,
    .updateMenu      = &menuNoop,
    .runMenuLoop     = &_fishLoop,
};

// Switching code generation back to shared section
#pragma code(code)
#pragma data(data)
