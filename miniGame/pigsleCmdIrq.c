
#include <c64/vic.h>
#include <assets/assetsSettings.h>
#include <c64/joystick.h>
#include <c64/sprites.h>
#include <c64/easyflash.h>
#include <c64/memmap.h>

#include <engine/easyFlashBanks.h>
#include <miniGame/pigsleCmdAnims.h>
#include <miniGame/pigsleCmdMain.h>

#define CROSSHAIR_MIN_X 24-6
#define CROSSHAIR_MAX_X 320 + 6
#define CROSSHAIR_MIN_Y 50
#define CROSSHAIR_MAX_Y 172

#pragma code ( pigsleCommandRAMCode )
#pragma data ( pigsleCommandRAMData )

static byte _prevRomCfgPC;
volatile byte _flashDelay = 2;

// ================================================================================
// Top raster
// ================================================================================
static void _playMsx(){
    if(gms_enableMusic){
        _prevRomCfgPC = ((byte *)0x01)[0];
        __asm {
            lda #MSX_ROM
            sta $01
            jsr MSX_PLAY
        };
        ((byte *)0x01)[0] = _prevRomCfgPC;
    }
}

__interrupt void _spriteInit(){
    vic.spr_multi    = 0b11111110;
    
    // explosion
    vic.spr_mcolor0 = VCOL_WHITE;
    vic.spr_mcolor1 = VCOL_RED;

    vic.spr_color[0] = VCOL_WHITE;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+0] = PIGSLE_CMD_ANIM_CROSSHAIR_LOADED_BANK;
    vic.spr_enable = 0b00000001;
}

static void _animDropDown(){
    vic.color_border++;
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
    if(TheDropDown.x > 72){
        spr_move(0, TheDropDown.x-72, 20);
        spr_move(4, TheDropDown.x-72, 20);
    } else {
        visibleSprites = 0b11101110;
    }

    if(TheDropDown.x > 48){
        spr_move(1, TheDropDown.x-48, 20);
        spr_move(5, TheDropDown.x-48, 20);
    } else {
        visibleSprites = 0b11001100;
    }

    if(TheDropDown.x > 24){
        spr_move(2, TheDropDown.x-24, 20);
        spr_move(6, TheDropDown.x-24, 20);
    } else {
        visibleSprites = 0b10001000;
    }

    if(TheDropDown.x > 0){
        spr_move(3, TheDropDown.x, 20);
        spr_move(7, TheDropDown.x, 20);
    } else {
        visibleSprites = 0b00000000;
        TheDropDown.inProgress = false;
    }

    TheDropDown.x--;
    vic.spr_enable = visibleSprites;
    vic.color_border--;
}

__interrupt void pigsleCmdIrq_topPlane() {
    vic.color_border++;

    vic.color_back = VCOL_BLUE;
    if(TheDropDown.inProgress){
        _animDropDown();
    }

    vic.color_border--;
}

__interrupt void pigsleCmdIrq_topPests() {
    vic.color_border++;

    _flashDelay--;
    if(!_flashDelay){
        joyCursor.colorIdx++;
        if(SPR_JOY_CURSOR_COLORS[joyCursor.colorIdx] == 0){
            joyCursor.colorIdx = 0;
        }
        vic.spr_color[0] = SPR_JOY_CURSOR_COLORS[joyCursor.colorIdx];
        _flashDelay = 2;
    }

    _spriteInit();

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

    _playMsx();
    vic.color_back = VCOL_BLACK;

    // Check button
    if (joyb[0]){
        // Avoid quickfire and bouncing
        if (CrossDelay == 0){
            // Request fire from non interrupt code
            CrossP = true;
            CrossDelay = 5;
        }
    }
    else if (CrossDelay > 0)
        CrossDelay--;

    if(!CrossDelay && efree)
        GFX_1_SCR[OFFSET_SPRITE_PTRS+0] = PIGSLE_CMD_ANIM_CROSSHAIR_LOADED_BANK;

    _prevRomCfgPC = ((byte *)0x01)[0];
    mmap_set(MMAP_ROM);
    changeBank(MENU_BANK_PIGSLE_COMMAND_2);

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

    restoreBank();
    ((byte *)0x01)[0] = _prevRomCfgPC;
    vic.color_border--;
}

__interrupt void pigsleCmdIrq_middlePests() {
    vic.color_border++;

    vic.color_border--;
}

__interrupt void pigsleCmdIrq_cannonAnims() {
    vic.color_border++;
    _playMsx();
    vic.color_border--;
}

__interrupt void pigsleCmdIrq_openBorder() {
    vic.color_back = VCOL_BROWN;
    vic.color_border++;
    vic.spr_enable = 0b00000000;
    vic.color_border--;
}