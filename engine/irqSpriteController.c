#include <c64/vic.h>
#include <c64/memmap.h>
#include <c64/easyflash.h>
#include <c64/cia.h>
#include <string.h>

#include <menu/menuSystem.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/gameSettings.h>
#include <assets/mainGfx.h>
#include <character/character.h>

volatile char isc_statusTextColorIdx = 0;
static char prvWeatherIcon = 255;

char* characterSlotSpriteBarPtr[CHARACTER_SLOTS] = {
    SPR_CHARACTER_BAR1,
    SPR_CHARACTER_BAR2,
    SPR_CHARACTER_BAR3,
    SPR_CHARACTER_BAR4
};

static char _batteryColors[4] = {VCOL_BLUE, VCOL_BLUE, VCOL_BLUE, VCOL_BLUE};

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

// copy sprite from sprite cache to static banks
static void _copySpriteToBank(char * source, char* dest){
        char pbank = setBank(MAIN_GFX_BANK);
        memcpy(dest, source, 63);
        dest[63] = source[63] & 0b00001111;
        setBank(pbank);
}

// copy weather sprite
void setWeatherIcon(char sprIdx){
    if(sprIdx != prvWeatherIcon){
        _copySpriteToBank(AuxResources.WEATHER_ICONS + 64 * sprIdx, SPR_WEATHER_ICON);
        prvWeatherIcon = sprIdx;
    }
}

// copy time speed icon
void setTimeSpeedIcon(char sprIdx){
    _copySpriteToBank(AuxResources.TIME_ICONS + 64 * sprIdx, SPR_TIME_ICON);
}

// copy face to character portrait
void setCharacterSlotPic(char charIdx){
    char charSlot = allCharacters[charIdx].slot;
    if(charSlot != NO_SLOT){
        _copySpriteToBank(allCharacters[charIdx].picture, characterSlotSpritePicPtr[charSlot]);
    }
}

// copy task icon to character's SPR_CHARACTER_BARX
void setCharacterSlotIcon(char charIdx, char * taskIconPtr){
    char charSlot = allCharacters[charIdx].slot;
    if(charSlot != NO_SLOT){
        _copySpriteToBank(taskIconPtr, characterSlotSpriteBarPtr[charSlot]);
    }
}

void drawBattery(char charIdx){
    char charSlot = allCharacters[charIdx].slot;
    char energy = allCharacters[charIdx].energy;
    if(charSlot != NO_SLOT){
        char * charBarPtr = characterSlotSpriteBarPtr[charSlot];
        char max = BATTERY_LEVEL[energy];
        _batteryColors[charSlot] = BATTERY_COLOR[max];
        
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
    }
}

__interrupt void setSpritesTopScr(){
    if(gms_enableMenuSprites){
        // store/restore memory config, as we need to ensure BASIC ROM is on
        char pport = setPort(MMAP_ROM);
        char pbank = setBank(mnu_menuBank);

        showSprites();
        setPort(pport);
        setBank(pbank);
    }
}

void setSpritesBottomScr(){
    // vic.BORDER_COLOR--;
    vic.spr_expand_x = 0b00000000;
    vic.spr_expand_y = 0b00000000;
    vic.spr_priority = 0b00000000;
    vic.spr_multi    = 0b00000000;
    
    vic.spr_msbx = 0b00000000;

    GFX_1_SCR[OFFSET_SPRITE_PTRS] = SPR_BANK_JOY_CURSOR1;

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
    gms_frameCount++;
    vic.spr_enable = 0;
    vic.memptr = d018_UI;
    cia2.pra = dd00_UI;

    vic.spr_expand_x = 0b00000000;
    vic.spr_expand_y = 0b00000000;
    vic.spr_priority = 0b00000000;
    vic.spr_multi    = 0b00001000;

    vic.spr_pos[0].y = 24;
    vic.spr_pos[1].y = 24;
    vic.spr_pos[2].y = 24;
    vic.spr_pos[3].y = 24;
    vic.spr_pos[4].y = 24;
    vic.spr_pos[5].y = 24;
    vic.spr_pos[6].y = 24;
    vic.spr_pos[7].y = 24;
    
    vic.spr_mcolor0 = SPR_WEATHER_MULTICOLOR_1;
    vic.spr_mcolor1 = SPR_WEATHER_MULTICOLOR_2;

    vic.spr_msbx = 0b00001100;

    UI_SCR[OFFSET_SPRITE_PTRS+0] = SPR_BANK_DATE_TXT1;
    UI_SCR[OFFSET_SPRITE_PTRS+1] = SPR_BANK_DATE_TXT2;
    UI_SCR[OFFSET_SPRITE_PTRS+2] = SPR_BANK_TIME_ICON;
    UI_SCR[OFFSET_SPRITE_PTRS+3] = SPR_BANK_WEATHER_ICON;
    UI_SCR[OFFSET_SPRITE_PTRS+4] = SPR_BANK_TXT_UP_1;
    UI_SCR[OFFSET_SPRITE_PTRS+5] = SPR_BANK_TXT_UP_2;
    UI_SCR[OFFSET_SPRITE_PTRS+6] = SPR_BANK_TXT_UP_3;
    UI_SCR[OFFSET_SPRITE_PTRS+7] = SPR_BANK_TXT_UP_4;

    vic.spr_pos[0].x = 50-12;
    vic.spr_pos[1].x = 50+12;
    vic.spr_pos[2].x = 60-24-10;
    vic.spr_pos[3].x = 60;
    vic.spr_pos[4].x = 160-24;
    vic.spr_pos[5].x = 160;
    vic.spr_pos[6].x = 160+24;
    vic.spr_pos[7].x = 160+48;

    char pport = setPort(MMAP_NO_ROM);
    vic.spr_color[2] = SPR_TIME_ICON[63];
    vic.spr_color[3] = SPR_WEATHER_ICON[63];
    vic.spr_color[0] = VCOL_MED_GREY;//DATE;
    vic.spr_color[1] = VCOL_MED_GREY;//DATE;
    if(joyCursor.error){
        vic.spr_color[4] = SPR_JOY_CURSOR_COLORS_ERROR[joyCursor.colorIdx];
        vic.spr_color[5] = SPR_JOY_CURSOR_COLORS_ERROR[joyCursor.colorIdx];
        vic.spr_color[6] = SPR_JOY_CURSOR_COLORS_ERROR[joyCursor.colorIdx];
        vic.spr_color[7] = SPR_JOY_CURSOR_COLORS_ERROR[joyCursor.colorIdx];
    } else {
        vic.spr_color[4] = SPR_JOY_CURSOR_COLORS[isc_statusTextColorIdx];
        vic.spr_color[5] = SPR_JOY_CURSOR_COLORS[isc_statusTextColorIdx];
        vic.spr_color[6] = SPR_JOY_CURSOR_COLORS[isc_statusTextColorIdx];
        vic.spr_color[7] = SPR_JOY_CURSOR_COLORS[isc_statusTextColorIdx];
        if(isc_statusTextColorIdx < 14){
            isc_statusTextColorIdx++;
        }
    }
    setPort(pport);

    vic.spr_enable = 0b11111111;
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

    UI_SCR[OFFSET_SPRITE_PTRS+0] = SPR_BANK_CHARACTER_PORTRAIT1;
    UI_SCR[OFFSET_SPRITE_PTRS+1] = SPR_BANK_CHARACTER_BAR1;
    UI_SCR[OFFSET_SPRITE_PTRS+2] = SPR_BANK_CHARACTER_PORTRAIT2;
    UI_SCR[OFFSET_SPRITE_PTRS+3] = SPR_BANK_CHARACTER_BAR2;
    UI_SCR[OFFSET_SPRITE_PTRS+4] = SPR_BANK_CHARACTER_PORTRAIT3;
    UI_SCR[OFFSET_SPRITE_PTRS+5] = SPR_BANK_CHARACTER_BAR3;
    UI_SCR[OFFSET_SPRITE_PTRS+6] = SPR_BANK_CHARACTER_PORTRAIT4;
    UI_SCR[OFFSET_SPRITE_PTRS+7] = SPR_BANK_CHARACTER_BAR4;

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
    vic.spr_pos[6].x = 24 + SPACER_WIDTH*7 + SPRITE_WIDTH*6 -255;
    // char bar
    vic.spr_pos[7].x = 24 + SPACER_WIDTH*8 + SPRITE_WIDTH*7 - 10 -255;

    vic.spr_pos[0].y = 2;
    vic.spr_pos[1].y = 2;
    vic.spr_pos[2].y = 2;
    vic.spr_pos[3].y = 2;
    vic.spr_pos[4].y = 2;
    vic.spr_pos[5].y = 2;
    vic.spr_pos[6].y = 2;
    vic.spr_pos[7].y = 2;

    char pport = setPort(MMAP_NO_ROM);
    vic.spr_color[0] = SPR_CHARACTER_PORTRAIT1[63];
    vic.spr_color[1] = _batteryColors[0];
    vic.spr_color[2] = SPR_CHARACTER_PORTRAIT2[63];
    vic.spr_color[3] = _batteryColors[1];
    vic.spr_color[4] = SPR_CHARACTER_PORTRAIT3[63];
    vic.spr_color[5] = _batteryColors[2];
    vic.spr_color[6] = SPR_CHARACTER_PORTRAIT4[63];
    vic.spr_color[7] = _batteryColors[3];
    setPort(pport);

    vic.spr_enable = 0b11111111;
    // indicate frame position
    gms_framePos = FRAME_BOTTOM;
    // vic.color_border++;
}

void showMapSpritesBottom(){
    // vic.color_border--;
    vic.spr_expand_x = 0b00000000;
    vic.spr_expand_y = 0b00000000;
    vic.spr_priority = 0b00000000;
    vic.spr_multi    = 0b10000001;
    
    vic.spr_mcolor0  = SPR_CHARACTER_PORTRAIT_MULTICOLOR_1;
    vic.spr_mcolor1  = SPR_CHARACTER_PORTRAIT_MULTICOLOR_2;

    vic.spr_msbx = 0b11000000;

    // reuse portrait banks, we will write something else there
    UI_SCR[OFFSET_SPRITE_PTRS+0] = SPR_BANK_CHARACTER_PORTRAIT1;
    UI_SCR[OFFSET_SPRITE_PTRS+1] = SPR_BANK_CHARACTER_BAR1;
    UI_SCR[OFFSET_SPRITE_PTRS+2] = SPR_BANK_CHARACTER_PORTRAIT2;
    UI_SCR[OFFSET_SPRITE_PTRS+3] = SPR_BANK_CHARACTER_BAR2;
    UI_SCR[OFFSET_SPRITE_PTRS+4] = SPR_BANK_CHARACTER_PORTRAIT3;
    UI_SCR[OFFSET_SPRITE_PTRS+5] = SPR_BANK_CHARACTER_BAR3;
    UI_SCR[OFFSET_SPRITE_PTRS+6] = SPR_BANK_CHARACTER_PORTRAIT4;
    UI_SCR[OFFSET_SPRITE_PTRS+7] = SPR_BANK_CHARACTER_BAR4;

    #define SPACER_WIDTH 16
    #define SPRITE_WIDTH 24
    // char
    vic.spr_pos[0].x = 24 + SPACER_WIDTH*1 + SPRITE_WIDTH*0;
    // char bar
    vic.spr_pos[1].x = 24 + SPACER_WIDTH*2 + SPRITE_WIDTH*1 - 10;
    // char
    vic.spr_pos[2].x = 24 + SPACER_WIDTH*4 + SPRITE_WIDTH*2;
    // char bar
    vic.spr_pos[3].x = 24 + SPACER_WIDTH*4 + SPRITE_WIDTH*3;
    // char
    vic.spr_pos[4].x = 24 + SPACER_WIDTH*4 + SPRITE_WIDTH*4;
    // char bar
    vic.spr_pos[5].x = 24 + SPACER_WIDTH*4 + SPRITE_WIDTH*5;
    // char
    vic.spr_pos[6].x = 24 + SPACER_WIDTH*7 + SPRITE_WIDTH*6 -255;
    // char bar
    vic.spr_pos[7].x = 24 + SPACER_WIDTH*8 + SPRITE_WIDTH*7 - 10 -255;

    vic.spr_pos[0].y = 2;
    vic.spr_pos[1].y = 2;
    vic.spr_pos[2].y = 2;
    vic.spr_pos[3].y = 2;
    vic.spr_pos[4].y = 2;
    vic.spr_pos[5].y = 2;
    vic.spr_pos[6].y = 2;
    vic.spr_pos[7].y = 2;

    char pport = setPort(MMAP_NO_ROM);
    vic.spr_color[0] = SPR_CHARACTER_PORTRAIT1[63];
    vic.spr_color[1] = _batteryColors[0];
    vic.spr_color[2] = VCOL_MED_GREY;
    vic.spr_color[3] = VCOL_MED_GREY;
    vic.spr_color[4] = VCOL_MED_GREY;
    vic.spr_color[5] = VCOL_MED_GREY;
    vic.spr_color[6] = SPR_CHARACTER_PORTRAIT4[63];
    vic.spr_color[7] = _batteryColors[3];
    setPort(pport);

    vic.spr_enable = 0b11111111;
    // indicate frame position
    gms_framePos = FRAME_BOTTOM;
    // vic.color_border++;
}
