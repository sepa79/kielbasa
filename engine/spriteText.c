#include <c64/vic.h>

#include <engine/spriteText.h>
#include <engine/easyFlashBanks.h>
#include <assets/mainGfx.h>

// ===========================================================================================
// In MainGfxCode, to be close to the sprites and fonts it copies, and to save some RAM
// ===========================================================================================
#pragma code ( mainGfxCode )
#pragma data ( mainGfxData )

// used with sprite drawing methods
static const char colOffset[12] = {0,1,2, 64,65,66, 128,129,130, 192,193,194};
static const char rowOffset[3] = {0,21,42};
static char offset = 0;

static void _charToSprite(char c, char * sprBankPtr){
    char i = 0;
    unsigned int pos = c;
    pos = pos * 8;
    do{
        // +0x400 as these are underlinned fonts, already shrunk and moved up nicely so suitable for sprites
        sprBankPtr[0] = ((char *)GFX_1_FNT_SRC_ADR+0x400)[pos+i];
        i++;
        sprBankPtr++;
        sprBankPtr++;
        sprBankPtr++;
    } while (i<7);
}

void _copyCharToSprite(char c, char col, char row, char * sprBankPtr){
    char i = 0;
    char si = colOffset[col] + rowOffset[row];
    _charToSprite(c, sprBankPtr+si);
}

void _textToSprite(char * str, char width, char * sprBankPtr, char col, char row){
    char c = str[0];
    char it = 0;
    char maxCol = width*3;
    
    char * finalSprBankPtr = sprBankPtr + colOffset[col] + rowOffset[row];
    while (c != 0) {
        _charToSprite(c, finalSprBankPtr);

        if(++col == maxCol){
            col = 0;
            if(++row == 3){
                sprBankPtr += width*64;
                row = 0;
            }
        }
        finalSprBankPtr = sprBankPtr + colOffset[col] + rowOffset[row];

        c = str[++it];
    };
}

void _intToWeightToSprite(unsigned int num, char * sprBankPtr){
    char str[10] = {32, 32, 32, 32, 32, 32, 32, 32, 32, 0};
    char num2str[6];
    utoa(num, num2str, 10);
    if(num > 9999) {
        str[0] = num2str[0];
        str[1] = num2str[1]+0x40;
        str[2] = num2str[2];

        str[5] = s't';

    } else if(num > 999) {
        str[0] = num2str[0]+0x40;
        str[1] = num2str[1];
        str[2] = num2str[2];

        str[5] = s't';
    } else if(num > 99) {
        str[0] = num2str[0];
        str[1] = num2str[1];
        str[2] = num2str[2];

        str[4] = s'k';
        str[5] = s'g';
    } else if(num > 9) {
        str[1] = num2str[0];
        str[2] = num2str[1];

        str[4] = s'k';
        str[5] = s'g';
    } else {
        str[2] = num2str[0];

        str[4] = s'k';
        str[5] = s'g';
    }
    _textToSprite(str, 1, sprBankPtr, 0, 0);
}


void _byteToSprite(char byte, char * sprBankPtr){
    char str[10];
    sprintf(str, "%3d      ", byte);
    _textToSprite(str, 1, sprBankPtr, 0, 0);
}

// __interrupt void _setSpriteTextBlock(char sprBankStart, unsigned int xPos, char yPos, char xSize, char ySize, char color){
//     char sprId = 0;
//     for(char y=0;y<ySize;y++){
//         for(char x=0;x<xSize;x++){
//             vic_sprxy(sprId, xPos + x*24, yPos + y*21);
//             vic.spr_color[sprId] = VCOL_MED_GREY;
//             GFX_2_SCR[OFFSET_SPRITE_PTRS+sprId] = sprBankStart + sprId;
//             sprId++;
//         }
//     }
// }

// ===========================================================================================
// Ram wrappers
#pragma code ( code )
#pragma data ( data )
// ===========================================================================================

void copyCharToSprite(char c, char col, char row, char * sprBankPtr){
    char pbank = setBank(MAIN_GFX_BANK);
    _copyCharToSprite(c, col, row, sprBankPtr);
    setBank(pbank);
}


void textToSprite(char * str, char width, char * sprBankPtr){
    char pbank = setBank(MAIN_GFX_BANK);
    _textToSprite(str, width, sprBankPtr, 0, 0);
    setBank(pbank);
}

void textToSpriteAt(char * str, char width, char * sprBankPtr, char col, char row){
    char pbank = setBank(MAIN_GFX_BANK);
    _textToSprite(str, width, sprBankPtr, col, row);
    setBank(pbank);
}

// change the int like 10 into string like ' 10 kg' - padded to be useful on sprites
void intToWeightToSprite(unsigned int num, char * sprBankPtr){
    char pbank = setBank(MAIN_GFX_BANK);
    _intToWeightToSprite(num, sprBankPtr);
    setBank(pbank);
}

// simplistic one to put byte to sprite as a number, wipe rest of the sprite
// used in kitchen and in similar places where single sprite is used for displaying
// small number of items in storage
void byteToSprite(char byte, char * sprBankPtr){
    char pbank = setBank(MAIN_GFX_BANK);
    _byteToSprite(byte, sprBankPtr);
    setBank(pbank);
}

// __interrupt void setSpriteTextBlock(char sprBankStart, unsigned int xPos, char yPos, char xSize, char ySize, char color){
//     char pbank = setBank(MAIN_GFX_BANK);
//     _setSpriteTextBlock(sprBankStart, xPos, yPos, xSize, ySize, color);
//     setBank(pbank);
// }
