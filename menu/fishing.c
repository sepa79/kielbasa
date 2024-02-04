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
    #embed 0xffff 2 "assets/multicolorGfx/fishing.kla"
};
#pragma data(fishingMenuSprites)
__export const char fishingMenuSprites[] = {
    #embed 0xffff 20 "assets/sprites/rybki.spd"
};

#pragma code(fishingMenuRAMCode)
#pragma data(fishingMenuRAMData)

volatile Fish allFish[9];
volatile char fishLevelIt = 0;

RIRQCode rirqc_frow1, rirqc_frow2, rirqc_frow3;
#define IRQ_RASTER_FROW1 0x88
#define IRQ_RASTER_FROW2 0xb0
#define IRQ_RASTER_FROW3 0xd8
#define FISH_LEVEL_OFFSET 6
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

__interrupt static void IRQ_rowFishing() {
    vic.color_border--;
    // TODO: set sprites on once somewhere
    vic.spr_expand_x = 0b00000000;
    vic.spr_expand_y = 0b00000000;
    vic.spr_priority = 0b00000000;
    vic.spr_multi    = 0b10101010;
    vic.spr_enable   = 0b11111111;

    char fishId = fishLevelIt * 3;
    #pragma unroll(full)
    for(char sprId=0;sprId<6;sprId+=2){
        int x = allFish[fishId].posX;
        char y = allFish[fishId].posY;
        char b = allFish[fishId].baseSprBank + allFish[fishId].frame;

        _vic_sprxy2(sprId, x, y);

        vic.spr_color[sprId] = VCOL_BLACK;
        vic.spr_color[sprId+1] = VCOL_YELLOW;
        GFX_2_SCR[OFFSET_SPRITE_PTRS+sprId] = b+1;
        GFX_2_SCR[OFFSET_SPRITE_PTRS+sprId+1] = b;
        fishId++;
    }

    fishLevelIt++;
    if(fishLevelIt > 2){
        fishLevelIt = 0;
    }

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

    // 1st row of fish
    rirq_build(&rirqc_frow1, 1);
    rirq_call(&rirqc_frow1, 0, IRQ_rowFishing);
    rirq_set(10, IRQ_RASTER_FROW1, &rirqc_frow1);

    // 2nd row of fish
    rirq_build(&rirqc_frow2, 1);
    rirq_call(&rirqc_frow2, 0, IRQ_rowFishing);
    rirq_set(11, IRQ_RASTER_FROW2, &rirqc_frow2);

    // 3rd row of fish
    rirq_build(&rirqc_frow3, 1);
    rirq_call(&rirqc_frow3, 0, IRQ_rowFishing);
    rirq_set(12, IRQ_RASTER_FROW3, &rirqc_frow3);

    // sort the raster IRQs
    rirq_sort();
    // vic.color_border++;
}

#pragma code(fishingMenuCode)
#pragma data(data)

static void _fishLoop(){
    vic.color_border++;
    for(char fi=0;fi<9;fi++){
        if(allFish[fi].posX > 0){
            allFish[fi].posX--;
        } else {
            allFish[fi].posX = 350;
        }
    }
    vic.color_border--;
}

static Fish _initFish(char level){
    unsigned int rnd = rand();
    Fish fish;
    fish.posY = fishLevel[level] + (rnd & 7);
    fish.posX = 50 + (rnd & 255);
    fish.baseSprBank = 0x11;
    fish.frame = 0;
    return fish;
}

static void _initAllFish(){
    for(char fi=0;fi<9;fi++){
        allFish[fi] = _initFish(fi/3);
    }
    fishLevelIt = 0;
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
