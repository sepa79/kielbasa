#include <c64/vic.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>
#include <c64/easyflash.h>
#include <c64/sprites.h>

#include <menu/menuSystem.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/uiHandler.h>
#include <engine/irqHandler.h>
#include <engine/gameSettings.h>

#include <menu/pigsleCommand.h>
// #include <miniGame/pigsleCmdMain.h>


// ---------------------------------------------------------------------------------------------
// Main screen and sprite bank + loaders code
// ---------------------------------------------------------------------------------------------

#pragma data ( pigsleCommandGfx1 )
// __export const char pigsleCommandGfxBg[] = {
//     #embed 0x2713 0x0002 "assets/multicolorGfx/dzialoKoval.kla"
//     // #embed 0x2713 0x0002 "assets/multicolorGfx/flak_88_dark.kla"
// };

__export const char pigsleCommandGfxBgBmp[] = {
    #embed 8000 2 lzo "assets/multicolorGfx/dzialoKoval.kla"
};
__export const char pigsleCommandGfxBgC1[] = {
    #embed 1000 0x1f42 lzo "assets/multicolorGfx/dzialoKoval.kla"
};
__export const char pigsleCommandGfxBgC2[] = {
    #embed 1000 0x232a lzo "assets/multicolorGfx/dzialoKoval.kla"
};



const char PIGSLE_CMD_SPR_AIM[] = {
    #embed 0x0080 20 "assets/sprites/crosshair.spd"
};
const char PIGSLE_CMD_SPR_BOOM_1[] = {
    #embed 0xffff 20 "assets/sprites/wybuch.spd"
};
const char PIGSLE_CMD_SPR_BOOM_2[] = {
    #embed 0xffff 20 "assets/sprites/wybuch2.spd"
};
const char PIGSLE_CMD_SPR_B29[] = {
    #embed 0xffff 20 "assets/sprites/b29.spd"
};
const char PIGSLE_CMD_SPR_PESTS[] = {
    #embed 0xffff 20 "assets/sprites/pests2.spd"
};

// this code needs to be in main block, as it switches banks
#pragma code ( pigsleCommandRAMCode )
#pragma data ( pigsleCommandRAMData )

void pigsleScreenInit(void){
    char pbank = setBank(MENU_BANK_PIGSLE_COMMAND_GFX_1);
    oscar_expand_lzo_buf(GFX_2_BMP, pigsleCommandGfxBgBmp);
    oscar_expand_lzo_buf(GFX_2_SCR, pigsleCommandGfxBgC1);
    oscar_expand_lzo_buf(COLOR_RAM, pigsleCommandGfxBgC2);

    setBank(pbank);
}

void pigsleSpriteLoader(){
    // vic.color_border = VCOL_RED;
    // vic.color_back  = VCOL_RED;

    char pbank = setBank(MENU_BANK_PIGSLE_COMMAND_GFX_1);
    // ROM on, I/O off - as we will copy to RAM under I/O ports
    char pport = mmap_set(MMAP_ALL_ROM);

    memcpy((char *)GFX_1_SPR_DST_ADR, PIGSLE_CMD_SPR_AIM, 0x80);
    // memcpy((char *)GFX_1_SPR_DST_ADR+0x80, PIGSLE_CMD_SPR_BOOM_1, 64*EXPLOSION_ANIM_FRAMES);
    // memcpy((char *)GFX_1_SPR_DST_ADR+0x80+64*EXPLOSION_ANIM_FRAMES, PIGSLE_CMD_SPR_BOOM_2, 64*EXPLOSION_ANIM_FRAMES);
    #pragma unroll(page)
    for(int i=0; i<64*EXPLOSION_ANIM_FRAMES; i++){
       ((volatile char*) GFX_1_SPR_DST_ADR)[0x80 + i] = ((char*)PIGSLE_CMD_SPR_BOOM_1)[i];
       ((volatile char*) GFX_1_SPR_DST_ADR)[0x80+64*EXPLOSION_ANIM_FRAMES + i] = ((char*)PIGSLE_CMD_SPR_BOOM_2)[i];
    }
    #pragma unroll(page)
    for(int i=0; i<64*DROP_ANIM_FRAMES*2; i++){
       ((volatile char*) GFX_1_SPR_DST_ADR)[0x80+64*EXPLOSION_ANIM_FRAMES*2 + i] = ((char*)PIGSLE_CMD_SPR_PESTS)[i];
    }
    #pragma unroll(page)
    for(int i=0; i<64*8; i++){
       ((volatile char*) GFX_1_SPR_DST_ADR)[0x80+64*(EXPLOSION_ANIM_FRAMES*2 + DROP_ANIM_FRAMES*2) + i] = ((char*)PIGSLE_CMD_SPR_B29)[i];
    }

    // turn ROMS and I/O back on, so that we don't get a problem when bank tries to be switched but I/O is not visible
    mmap_set(pport);
    setBank(pbank);
}

// ---------------------------------------------------------------------------------------------
// Menu code
// ---------------------------------------------------------------------------------------------
#pragma code ( pigsleCommandCode )
#pragma data ( data )

const struct MenuOption PIGSLE_COMMAND_MENU[] = {
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_TRANSITION, UI_LF+UI_HIDE, &showMenu, MENU_BANK_HERMANS_HOUSE, 2, 5},
    END_MENU_CHOICES
};

static void _pigsleCmdCodeLoader(){
    // source is where the regionPigsleCommandRam section starts in real mem
    memcpy(MENU_CODE_DST, (char *)0xb000, 0x1000);
}

// Noop - just return - to satisfy menu handlers
static void _pigsleCmdNoop(){
    return;
}
__interrupt static void _pigsleSpriteCmdNoop(){
    return;
}

#pragma code ( pigsleCommandRAMCode )
#pragma data ( pigsleCommandRAMData )

// static void _pigsleCmdInit(){
//     splashScreen(false, 1);
//     vic.spr_enable   = 0b00000000;


//     playSong(PIGSLE_CMD_SONG);

//     // Load GFX
//     pigsleScreenInit();
//     loadMenuSprites();
//     // loadMenuGfx();


//     // Init sprite system
//     // spr_init(GFX_1_SCR);
//     // bm_init(&sbm, GFX_1_BMP, 40, 25);

//     // splashScreen(false, 2);
//     // // Init bitmap
//     // vic_setmode(VICM_HIRES_MC, GFX_1_SCR, GFX_1_BMP);

//     // // start game state machine
//     // gameState(GS_READY);

//     // // main loop
//     // for(;;){
//     //     // vic.color_border++;

//     //     gameLoop();
//     //     vic_waitFrame();
//     //     rirq_wait();
//     //     // vic.color_border--;

//     // }
// }

static void _pigsleMenuHandler(void){
    // need to add no-sprite handler, where splash can work nicely
    // something like splash on sprites, or fly away?
    // splashScreen(false, 1);

    loadMenuSprites();
    // loadMenuGfx();
    pigsleScreenInit();
    playSong(PIGSLE_CMD_SONG);
    displayMenu(PIGSLE_COMMAND_MENU);

    switchScreenTo(SCREEN_PIGSLE);
    // Init bitmap
    vic_setmode(VICM_HIRES_MC, GFX_1_SCR, GFX_1_BMP);

    // splashScreen(false, 2);

    // TODO: return to main menu
}

static void _pigsleCmdLoop(void){
    // main loop
    vic.color_border++;

    // gameLoop();
    // vic_waitFrame();
    // rirq_wait();
    vic.color_border--;

}

#pragma data ( pigsleCommandLoaderData )

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &_pigsleCmdCodeLoader,
    .loadMenuGfx     = &pigsleScreenInit,
    .loadMenuSprites = &pigsleSpriteLoader,
    .showMenu        = &_pigsleMenuHandler,
    .showSprites     = &spriteNoop,
    .updateMenu      = &menuNoop,
    // .runMenuLoop     = &_pigsleCmdLoop,
    .runMenuLoop     = &menuNoop,
};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
