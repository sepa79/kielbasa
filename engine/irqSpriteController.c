#include <c64/vic.h>
#include <c64/memmap.h>
#include <c64/easyflash.h>

#include <menu/menuSystem.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/gameSettings.h>
#include <assets/mainGfx.h>
#include <character/character.h>

volatile char isc_weatherSprite = 0;
char* characterSlotSpriteBarPtr[CHARACTER_SLOTS] = {
    SPR_CHARACTER_BAR1,
    SPR_CHARACTER_BAR2,
    SPR_CHARACTER_BAR3,
    SPR_CHARACTER_BAR4
};
char* characterSlotSpritePicPtr[CHARACTER_SLOTS] = {
    SPR_CHARACTER_PORTRAIT1,
    SPR_CHARACTER_PORTRAIT2,
    SPR_CHARACTER_PORTRAIT3,
    SPR_CHARACTER_PORTRAIT4
};

#define BATTERY_ROW_EMPTY 0b10000001
#define BATTERY_ROW_FULL  0b10111111
const char BATTERY_COLOR[18] = {0x5,0x5,0x5,0x5,0xd,0xd,0xd,0xd,0x8,0x8,0x8,0x8,0x2,0x2,0x2,0x2,0xb,0xb};
const char BATTERY_LEVEL[101] = {
    0x11,0x11,0x11,0x10,0x10,0x10,0x10,0x10,0x10,
    0xF,0xF,0xF,0xF,0xF,0xF,0xE,0xE,0xE,0xE,
    0xE,0xE,0xD,0xD,0xD,0xD,0xD,0xD,0xC,0xC,
    0xC,0xC,0xC,0xC,0xB,0xB,0xB,0xB,0xB,0xB,
    0xA,0xA,0xA,0xA,0xA,0xA,0x9,0x9,0x9,0x9,
    0x9,0x9,0x8,0x8,0x8,0x8,0x8,0x7,0x7,0x7,
    0x7,0x7,0x7,0x6,0x6,0x6,0x6,0x6,0x6,0x5,
    0x5,0x5,0x5,0x5,0x5,0x4,0x4,0x4,0x4,0x4,
    0x4,0x3,0x3,0x3,0x3,0x3,0x3,0x2,0x2,0x2,
    0x2,0x2,0x2,0x1,0x1,0x1,0x1,0x1,0x1,0x0,0x0,0x0
};

void setErrorCursor(){
    joyCursor.error = true;
    joyCursor.colorIdx = 0;
}

void setNormalCursor(){
    joyCursor.error = false;
    joyCursor.colorIdx = 0;
}

// copy face to character portrait
void setCharacterSlotPic(char charSlot, const char * picturePtr){
    // might be redundant, init checks it, and no pic should be set for empty charslot elsewhere
    if(characterSlots[charSlot] != NO_CHARACTER){
        char * charPicPtr = characterSlotSpritePicPtr[charSlot];
        char i = 0;
        char pbank = setBank(MAIN_GFX_BANK);
        // mmap_set(MMAP_NO_BASIC);
        do{
            charPicPtr[i]   = picturePtr[i];
            charPicPtr[i+1] = picturePtr[i+1];
            charPicPtr[i+2] = picturePtr[i+2];
            i++;
            i++;
            i++;
        } while (i<63);
        charPicPtr[63] = picturePtr[63];
        // mmap_set(MMAP_ROM);
        setBank(pbank);
    }
}

// copy task icon to character's SPR_CHARACTER_BARX
void setCharacterSlotIcon(char charSlot, const char * taskIconPtr){
    if(characterSlots[charSlot] != NO_CHARACTER){
        char * charBarPtr = characterSlotSpriteBarPtr[charSlot];
        char i = 1;
        char pbank = setBank(MAIN_GFX_BANK);
        // mmap_set(MMAP_NO_BASIC);
        do{
            charBarPtr[i]   = taskIconPtr[i];
            charBarPtr[i+1] = taskIconPtr[i+1];
            i++;
            i++;
            i++;
        } while (i<63);
        // mmap_set(MMAP_ROM);
        setBank(pbank);
    }
}

void drawBattery(char charSlot, char energy){
    // vic.BORDER_COLOR++;
    char * charBarPtr = characterSlotSpriteBarPtr[charSlot];
    char max = BATTERY_LEVEL[energy];
    charBarPtr[63] = BATTERY_COLOR[max];
    
    __asm{
        ldy #9
        ldx #0
        cpx max
        beq loop2s
        lda #BATTERY_ROW_EMPTY
    loop1:
        sta (charBarPtr),y
        iny
        iny
        iny
        inx
        cpx max
        bcc loop1
        cpx #$11
        bcs done
    loop2s:
        lda #BATTERY_ROW_FULL
    loop2:
        sta (charBarPtr),y
        iny
        iny
        iny
        inx
        cpx #$11
        bcc loop2
    done:
    };

    // vic.BORDER_COLOR--;
}

__interrupt void setSpritesTopScr(){
    if(gms_enableMenuSprites){
        // store/restore memory config, as we need to ensure BASIC ROM is on
        byte _prevRomCfg = ((byte *)0x01)[0];
        mmap_set(MMAP_ROM);
        showSprites();
        ((byte *)0x01)[0] = _prevRomCfg;
    }
}

void setSpritesBottomScr(){
    // vic.BORDER_COLOR--;
    vic.spr_expand_x = 0b00000000;
    vic.spr_expand_y = 0b00000000;
    vic.spr_priority = 0b00000000;
    vic.spr_multi    = 0b00000000;
    
    vic.spr_msbx = 0b00000000;

    GFX_1_SCR[OFFSET_SPRITE_PTRS] = SPR_BANK_JOY_CURSOR2;

    vic_sprxy(0, joyCursor.x, joyCursor.y);

    if(joyCursor.error){
        vic.spr_color[0] = SPR_JOY_CURSOR_COLORS_ERROR[joyCursor.colorIdx];
    } else {
        vic.spr_color[0] = SPR_JOY_CURSOR_COLORS[joyCursor.colorIdx];
    }
    vic.spr_enable = 0b00000001;
    // vic.BORDER_COLOR++;
}

void showUiSpritesTop(){
    vic.spr_enable = 0;
    
    // already set in the bottom IRQ
    // vic.spr_expand_x = 0b00000000;
    // vic.spr_expand_y = 0b00000000;
    // vic.spr_priority = 0b00000000;
    vic.spr_multi       = 0b00011000;

    vic.spr_pos[0].y = 24;
    vic.spr_pos[1].y = 24;
    vic.spr_pos[2].y = 24;
    vic.spr_pos[3].y = 24;
    vic.spr_pos[4].y = 24;
    vic.spr_pos[5].y = 24;
    // vic.spr_pos[6].y = 24;
    // vic.spr_pos[7].y = 24;
    
    vic.spr_mcolor0 = SPR_WEATHER_MULTICOLOR_1;
    vic.spr_mcolor1 = SPR_WEATHER_MULTICOLOR_2;

    vic.spr_msbx = 0b00111000;

    GFX_1_SCR[OFFSET_SPRITE_PTRS+0] = SPR_BANK_DATE_TXT1;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+1] = SPR_BANK_DATE_TXT2;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+2] = SPR_BANK_TIME_ICON1+gms_gameSpeed;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+3] = SPR_BANK_WEATHER1+isc_weatherSprite;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+4] = SPR_BANK_CURRENCY1;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+5] = SPR_BANK_CURRENCY_TXT;

    vic.spr_pos[0].x = 50-12;
    vic.spr_pos[1].x = 50+12;
    vic.spr_pos[2].x = 172;
    vic.spr_pos[3].x = 60-24-10;
    vic.spr_pos[4].x = 60;
    vic.spr_pos[5].x = 60;

    vic.spr_color[0] = VCOL_MED_GREY;//DATE;
    vic.spr_color[1] = VCOL_MED_GREY;//DATE;
    vic.spr_color[2] = SPR_TIME_ICONS_COLORS[gms_gameSpeed];
    vic.spr_color[3] = SPR_WEATHER_COLORS[isc_weatherSprite];
    vic.spr_color[4] = SPR_CURRENCY_COLORS[0];
    vic.spr_color[5] = VCOL_MED_GREY;

    vic.spr_enable = 0b00111111;
    gms_framePos = FRAME_TOP_BORDER;
}

void showUiSpritesBottom(){
    // vic.color_border--;

    vic.spr_expand_x = 0b00000000;
    vic.spr_expand_y = 0b00000000;
    vic.spr_priority = 0b00000000;
    vic.spr_multi    = 0b01010101;
    
    vic.spr_mcolor0  = SPR_CHARACTER_PORTRAIT_MULTICOLOR_1;
    vic.spr_mcolor1  = SPR_CHARACTER_PORTRAIT_MULTICOLOR_2;

    vic.spr_msbx = 0b11000000;

    GFX_1_SCR[OFFSET_SPRITE_PTRS+0] = SPR_BANK_CHARACTER_PORTRAIT1;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+1] = SPR_BANK_CHARACTER_BAR1;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+2] = SPR_BANK_CHARACTER_PORTRAIT2;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+3] = SPR_BANK_CHARACTER_BAR2;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+4] = SPR_BANK_CHARACTER_PORTRAIT3;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+5] = SPR_BANK_CHARACTER_BAR3;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+6] = SPR_BANK_CHARACTER_PORTRAIT4;
    GFX_1_SCR[OFFSET_SPRITE_PTRS+7] = SPR_BANK_CHARACTER_BAR4;

    #define SPACER_WIDTH 16
    #define SPRITE_WIDTH 24
    // char
    vic.spr_pos[0].x = 24 + SPACER_WIDTH*1 + SPRITE_WIDTH*0;
    // char bar
    vic.spr_pos[1].x = 24 + SPACER_WIDTH*2 + SPRITE_WIDTH*1 - 10;
    // char
    vic.spr_pos[2].x = 24 + SPACER_WIDTH*3 + SPRITE_WIDTH*2;
    // char bar
    vic.spr_pos[3].x = 24 + SPACER_WIDTH*4 + SPRITE_WIDTH*3 - 10;
    // char
    vic.spr_pos[4].x = 24 + SPACER_WIDTH*5 + SPRITE_WIDTH*4;
    // char bar
    vic.spr_pos[5].x = 24 + SPACER_WIDTH*6 + SPRITE_WIDTH*5 - 10;
    // char
    vic.spr_pos[6].x = 24 + SPACER_WIDTH*7 + SPRITE_WIDTH*6;
    // char bar
    vic.spr_pos[7].x = 24 + SPACER_WIDTH*8 + SPRITE_WIDTH*7 - 10;

    vic.spr_pos[0].y = 2;
    vic.spr_pos[1].y = 2;
    vic.spr_pos[2].y = 2;
    vic.spr_pos[3].y = 2;
    vic.spr_pos[4].y = 2;
    vic.spr_pos[5].y = 2;
    vic.spr_pos[6].y = 2;
    vic.spr_pos[7].y = 2;

    vic.spr_color[0] = SpriteResources.CHARACTER_PORTRAITS[64*0 + 63];
    vic.spr_color[1] = SpriteResources.CHARACTER_BARS[64*0 + 63];
    vic.spr_color[2] = SpriteResources.CHARACTER_PORTRAITS[64*1 + 63];
    vic.spr_color[3] = SpriteResources.CHARACTER_BARS[64*1 + 63];
    vic.spr_color[4] = SpriteResources.CHARACTER_PORTRAITS[64*2 + 63];
    vic.spr_color[5] = SpriteResources.CHARACTER_BARS[64*2 + 63];
    vic.spr_color[6] = SpriteResources.CHARACTER_PORTRAITS[64*3 + 63];
    vic.spr_color[7] = SpriteResources.CHARACTER_BARS[64*3 + 63];

    vic.spr_enable = 0b11111111;
    // indicate frame position
    gms_framePos = FRAME_BOTTOM;
    // vic.color_border++;
}
