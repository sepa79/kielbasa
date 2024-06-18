
#include <c64/vic.h>
#include <assets/assetsSettings.h>
#include <c64/joystick.h>
#include <c64/sprites.h>
#include <c64/easyflash.h>
#include <c64/memmap.h>
#include <c64/rasterirq.h>

#include <engine/easyFlashBanks.h>

#include <menu/pigsleCommand.h>
#include <miniGame/pigsleCmdIrq.h>
#include <miniGame/pigsleCmdAnims.h>
// #include <miniGame/pigsleCmdMain.h>

#define CROSSHAIR_MIN_X 24-6
#define CROSSHAIR_MAX_X 320 + 6
#define CROSSHAIR_MIN_Y 50
#define CROSSHAIR_MAX_Y 172

#pragma code ( pigsleCommandRAMCode )
#pragma data ( pigsleCommandRAMData )

RIRQCode topPlane, topPests, middlePests, cannonAnims, open;

volatile byte _flashDelay = 2;
volatile byte crosshairBank = PIGSLE_CMD_ANIM_CROSSHAIR_LOADED_BANK;
// ================================================================================
// Top raster
// ================================================================================

static void _animB29Plane(){
    // vic.color_border++;
    vic.spr_multi    = 0b11110000;
    
    // explosion
    vic.spr_mcolor0 = VCOL_LT_GREY;
    vic.spr_mcolor1 = VCOL_MED_GREY;

    // vic.spr_color[0] = GFX_1_SCR[(PIGSLE_CMD_ANIM_B29_BANK + 1 + 0) * 64];
    vic.spr_color[0] = VCOL_DARK_GREY;
    vic.spr_color[1] = VCOL_DARK_GREY;
    vic.spr_color[2] = VCOL_DARK_GREY;
    vic.spr_color[3] = VCOL_DARK_GREY;
    vic.spr_color[4] = VCOL_BLACK;
    vic.spr_color[5] = VCOL_ORANGE;
    vic.spr_color[6] = VCOL_PURPLE;
    vic.spr_color[7] = VCOL_ORANGE;
    
    GFX_1_SCR[OFFSET_SPRITE_PTRS+0] = PIGSLE_CMD_ANIM_B29_BANK+4;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+1] = PIGSLE_CMD_ANIM_B29_BANK+5;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+2] = PIGSLE_CMD_ANIM_B29_BANK+6;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+3] = PIGSLE_CMD_ANIM_B29_BANK+7;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+4] = PIGSLE_CMD_ANIM_B29_BANK+0;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+5] = PIGSLE_CMD_ANIM_B29_BANK+1;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+6] = PIGSLE_CMD_ANIM_B29_BANK+2;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+7] = PIGSLE_CMD_ANIM_B29_BANK+3;

    char visibleSprites = 0b11111111;
    if(TheB29Plane.x >= 72){
        spr_move(0, TheB29Plane.x-72, 20);
        spr_move(4, TheB29Plane.x-72, 20);
    } else {
        visibleSprites = 0b11101110;
    }

    if(TheB29Plane.x >= 48){
        spr_move(1, TheB29Plane.x-48, 20);
        spr_move(5, TheB29Plane.x-48, 20);
    } else {
        visibleSprites = 0b11001100;
    }

    if(TheB29Plane.x >= 24){
        spr_move(2, TheB29Plane.x-24, 20);
        spr_move(6, TheB29Plane.x-24, 20);
    } else {
        visibleSprites = 0b10001000;
    }

    if(TheB29Plane.x >= 0){
        spr_move(3, TheB29Plane.x, 20);
        spr_move(7, TheB29Plane.x, 20);
    } else {
        visibleSprites = 0b00000000;
        TheB29Plane.inProgress = false;
    }

    TheB29Plane.x--;
    vic.spr_enable = visibleSprites;
    // vic.color_border--;
}

__interrupt void _pigsleCmdSpriteInit(){
    vic.spr_multi   = 0b11111110;
    
    // explosion
    vic.spr_mcolor0 = VCOL_WHITE;
    vic.spr_mcolor1 = VCOL_RED;

    vic.spr_color[0] = SPR_JOY_CURSOR_COLORS[joyCursor.colorIdx];
    GFX_1_SCR[OFFSET_SPRITE_PTRS+0] = crosshairBank;

    vic.spr_msbx = explosionsOver255 | pestDropsOver255;
    vic.spr_enable = explosionsVisible | pestDropsVisible | 1;
}

static void _pestDropSprites(){
    vic.spr_pos[4].y = pestDropAnimY[0];
    vic.spr_pos[5].y = pestDropAnimY[1];
    vic.spr_pos[6].y = pestDropAnimY[2];
    vic.spr_pos[7].y = pestDropAnimY[3];
    vic.spr_pos[4].x = pestDropAnimX[0];
    vic.spr_pos[5].x = pestDropAnimX[1];
    vic.spr_pos[6].x = pestDropAnimX[2];
    vic.spr_pos[7].x = pestDropAnimX[3];
    vic.spr_color[4] = VCOL_GREEN;
    vic.spr_color[5] = VCOL_GREEN;
    vic.spr_color[6] = VCOL_GREEN;
    vic.spr_color[7] = VCOL_GREEN;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+4] = pestDropAnimBank[0];
    GFX_1_SCR[OFFSET_SPRITE_PTRS+5] = pestDropAnimBank[1];
    GFX_1_SCR[OFFSET_SPRITE_PTRS+6] = pestDropAnimBank[2];
    GFX_1_SCR[OFFSET_SPRITE_PTRS+7] = pestDropAnimBank[3];
}

static void _explosionSprites(){
    vic.spr_pos[1].y = explosionAnimY[0];
    vic.spr_pos[2].y = explosionAnimY[1];
    vic.spr_pos[3].y = explosionAnimY[2];
    vic.spr_pos[1].x = explosionAnimX[0];
    vic.spr_pos[2].x = explosionAnimX[1];
    vic.spr_pos[3].x = explosionAnimX[2];
    vic.spr_color[1] = VCOL_YELLOW;
    vic.spr_color[2] = VCOL_YELLOW;
    vic.spr_color[3] = VCOL_YELLOW;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+1] = explosionAnimBank[0];
    GFX_1_SCR[OFFSET_SPRITE_PTRS+2] = explosionAnimBank[1];
    GFX_1_SCR[OFFSET_SPRITE_PTRS+3] = explosionAnimBank[2];
}

__interrupt void pigsleCmdIrq_topPlane() {
    // vic.color_border++;

    vic.color_back = VCOL_BLUE;
    if(TheB29Plane.inProgress){
        _animB29Plane();
    }

    // vic.color_border--;
}

__interrupt void pigsleCmdIrq_topPests() {
    // vic.color_border++;

    _pestDropSprites();
    _pigsleCmdSpriteInit();
    // vic.color_border--;
    _explosionSprites();

    _flashDelay--;
    if(!_flashDelay){
        joyCursor.colorIdx++;
        if(SPR_JOY_CURSOR_COLORS[joyCursor.colorIdx] == 0){
            joyCursor.colorIdx = 0;
        }
        _flashDelay = 2;
    }
    // vic.color_border++;
    // Poll joystick
    joy_poll(0);

    // // Move crosshair coordinates
    CrossX += 1 * joyx[0]; CrossY += 1 * joyy[0];

    // Stop at edges of screen
    if (CrossX < CROSSHAIR_MIN_X)
        CrossX = CROSSHAIR_MIN_X;
    else if (CrossX > CROSSHAIR_MAX_X)
        CrossX = CROSSHAIR_MAX_X;
    if (CrossY < CROSSHAIR_MIN_Y)
        CrossY = CROSSHAIR_MIN_Y;
    else if (CrossY > CROSSHAIR_MAX_Y)
        CrossY = CROSSHAIR_MAX_Y;

    // Move crosshair sprite
    spr_move(0, CrossX, CrossY);

    // Check button
    if (joyb[0]){
        // Avoid quickfire and bouncing
        if (CrossDelay == 0){
            CrossP = true;
            // Request fire from non interrupt code
            CrossDelay = 5;
        }
    }
    else if (CrossDelay > 0)
        CrossDelay--;

    if(!CrossDelay && efree)
        crosshairBank = PIGSLE_CMD_ANIM_CROSSHAIR_LOADED_BANK;

    vic_waitLine(55);
    vic.color_back = VCOL_BLACK;
    playMsx();

    byte _prevRomCfgPC = ((byte *)0x01)[0];
    char pport = setPort(MMAP_ROM);
    char pbank = setBank(MENU_BANK_PIGSLE_COMMAND_2);

    if(CrossX < 60)
        copyCannonL60();
    else if(CrossX < 140)
        copyCannonL75();
    else if(CrossX < 180)
        copyCannonUp();
    else if(CrossX < 260)
        copyCannonR75();
    else
        copyCannonR60();

    setBank(pbank);
    setPort(pport);
    // vic.color_border--;
}

__interrupt void pigsleCmdIrq_middlePests() {
    vic.color_border++;

    vic.color_border--;
}

__interrupt void pigsleCmdIrq_cannonAnims() {
    // vic.color_border++;
    playMsx();
    // vic.color_border--;
}

__interrupt void pigsleCmdIrq_openBorder() {
    vic_waitLine(250);
    vic.color_back = VCOL_BROWN;
    // vic.color_border++;
    vic.spr_enable = 0b00000000;
    // vic.color_border--;
}

void initRasterIRQ_Pigsle(){
    // vic.color_border--;
    // clean 0xffff - so we don't have artefacts when we open borders
    ((char *)0xffff)[0] = 0;

    // initialize raster IRQ
    // rirq_init(true);
    // topPlane, topPests, middlePests, cannonAnims, open
    // Top - Plane
    rirq_build(&topPlane, 2);
    rirq_write(&topPlane, 0, &vic.ctrl1, VIC_CTRL1_BMM | VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3 );
    rirq_call(&topPlane, 1, pigsleCmdIrq_topPlane);
    rirq_set(0, IRQ_TOP_PLANE, &topPlane);

    // Top - Pests
    rirq_build(&topPests, 1);
    rirq_call(&topPests, 0, pigsleCmdIrq_topPests);
    rirq_set(1, IRQ_TOP_PESTS, &topPests);

    // Middle - Pests
    // rirq_build(&middlePests, 1);
    // rirq_call(&middlePests, 0, pigsleCmdIrq_middlePests);
    // rirq_set(2, IRQ_MIDDLE_PESTS, &middlePests);

    // Bottom - Cannon anims
    rirq_build(&cannonAnims, 1);
    rirq_call(&cannonAnims, 0, pigsleCmdIrq_cannonAnims);
    rirq_set(2, IRQ_CANNON, &cannonAnims);

    // Open border raster IRQ
    rirq_build(&open, 2);
    // Reduce vertical screen size to fool VIC counter
    rirq_write(&open, 0, &vic.ctrl1, VIC_CTRL1_BMM |VIC_CTRL1_DEN | 3);
    rirq_call(&open, 1, pigsleCmdIrq_openBorder);
    // Place it into the last line of the screen
    rirq_set(3, IRQ_FRAME_OPEN, &open);

    // clear remaining irqs
    for( char i=4; i<16;i++){
        rirq_clear(i);
    }
    // sort the raster IRQs
    rirq_sort();
}