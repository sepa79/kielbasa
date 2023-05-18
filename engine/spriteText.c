#include <engine/spriteText.h>
#include <engine/easyFlashBanks.h>
#include <assets/mainGfx.h>

// used with sprite drawing methods
static const char colOffset[9] = {0,1,2, 64,65,66, 128,129,130, 192,193,194};
static const char rowOffset[3] = {0,21,42};
// set it to sprite bank that you wish to use copyCharToSprite() on
char * textSprBankPt;
static char offset = 0;

// width is in sprites - 3 means 12 chars will fit
// routine will keep writing row by row, so make sure text is going to fit in
void textToSprite(char * str, char width){
    char c = 0;
    char it = 0;
    char col = 0;
    char row = 0;
    char maxCol = width*3;
    
    char * finalSprBankPtr = textSprBankPt;
    do {
        c = str[it++];
        _charToSprite(c, finalSprBankPtr);

        if(++col == maxCol){
            col = 0;
            if(++row == 3){
                textSprBankPt += width*64;
                row = 0;
            }
        }
        finalSprBankPtr = textSprBankPt + colOffset[col] + rowOffset[row];

    } while (c != 0);
}

static void _charToSprite(char c, char * sprBankPtr){
    char i = 0;
    unsigned int pos = c;
    pos = pos * 7;
    char pbank = setBank(MAIN_GFX_BANK);
    do{
        sprBankPtr = AuxResources.SPRITE_FONTS[pos+i];
        i++;
        sprBankPtr++;
        sprBankPtr++;
        sprBankPtr++;
    } while (i<7);
    setBank(pbank);
}
