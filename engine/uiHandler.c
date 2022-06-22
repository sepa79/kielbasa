#include <c64/vic.h>
#include <c64/types.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>
#include <c64/memmap.h>

#include <menu/menuSystem.h>
#include <menu/optionsMenu.h>
#include <menu/taskManagerMenu.h>
#include <menu/taskManagerPrioMenu.h>
#include <engine/gameSettings.h>
#include <engine/uiHandler.h>
#include <engine/joystick.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>

#define BORDER_XPOS_LEFT 24
#define BORDER_YPOS_TOP 50

// used with sprite drawing methods
const byte colOffset[6] = {0,1,2, 64,65,66};
const byte rowOffset[3] = {0,21,42};
byte * sprBankPointer;

volatile struct JOY_CURSOR joyCursor = {true, false, 0, 0, 0, 0, 0};
CharWin cw;

static bool _fullScreenMenuOpen = false;
static byte _lastElementInMenu = 0;
struct MenuOption *currentMenu;
static byte _menuUiMode = 0;
static byte _key = 0;
#define NOT_ATTACHED 0xff
static byte _joyU = 0;
static byte _joyD = 0;
static byte _joyL = 0;
static byte _joyR = 0;

static void _setJoyCursorPos(byte menuPos){
    joyCursor.x = (currentMenu[menuPos].x + cw.sx) * 8 + BORDER_XPOS_LEFT - 1;
    joyCursor.y = (currentMenu[menuPos].y + cw.sy) * 8 + BORDER_YPOS_TOP - 1;

    // set delay, without it it's impossible to control stuff, joy cursor moves too fast
    joyCursor.moveDelayCurrent = JOY_CURSOR_MOVE_DELAY_INIT;

    setNormalCursor();
}

/* Displays text and colors, sets _lastElementInMenu */
static void _displayMenuText(){
    byte i = 0;
    _joyL = NOT_ATTACHED;
    _joyR = NOT_ATTACHED;
    _joyU = NOT_ATTACHED;
    _joyD = NOT_ATTACHED;

    while (currentMenu[i].key != 0){
        // display text
        byte idx = currentMenu[i].textIdx;
        cwin_putat_string_raw(&cw, currentMenu[i].x, currentMenu[i].y, TXT[idx], VCOL_MED_GREY);

        // detect what kind of menu we have - LR or UD
        if(currentMenu[i].uiMode & (UI_LR + UI_UD)){
            _menuUiMode = currentMenu[i].uiMode;
        } else if(currentMenu[i].uiMode & UI_L){
            _joyL = i;
        } else if(currentMenu[i].uiMode & UI_R){
            _joyR = i;
        } else if(currentMenu[i].uiMode & UI_U){
            _joyU = i;
        } else if(currentMenu[i].uiMode & UI_D){
            _joyD = i;
        }
        
        i++;
    }
    _lastElementInMenu = i - 1;
}

/* Display menu on bottom screen half */
void displayMenu(struct MenuOption * menu){
    currentMenu = menu;
    // clearing screen is the responsibility of each menu

    // display menu text
    // vic.color_border = YELLOW;
    _displayMenuText();
    // set joystick handler to current menu
    joyCursor.menuPos = 0;
    // // vic.color_border--;
    _setJoyCursorPos(joyCursor.menuPos);
}

static void _incMenuPos(){
    bool done = false;
    while(!done){
        joyCursor.menuPos++;
        // check if we need to go around
        if(currentMenu[joyCursor.menuPos].key == 0){
            joyCursor.menuPos = 0;
        }
        //check if next element is ok to move to
        if(currentMenu[joyCursor.menuPos].uiMode & (UI_LR + UI_UD)){
            // yep, exit loop
            done = true;
        }
    }
    // only call update pos if movement occured
    _setJoyCursorPos(joyCursor.menuPos);
}
static void _decMenuPos(){
    bool done = false;
    while(!done){
        // go around
        if(joyCursor.menuPos == 0){
            joyCursor.menuPos = _lastElementInMenu;
        } else {
            joyCursor.menuPos--;
        }
        //check if next element is ok to move to
        if(currentMenu[joyCursor.menuPos].uiMode & (UI_LR + UI_UD)){
            // yep, exit loop
            done = true;
        }
    }
    // only call update pos if movement occured
    _setJoyCursorPos(joyCursor.menuPos);
}

static void _moveJoyThroughMenuUD(){
    if(!joyCursor.moveDelayCurrent){
        if(!(_joy2Status & JOY_UP)){
            _decMenuPos();
        }
        if(!(_joy2Status & JOY_DOWN)){
            _incMenuPos();
        }
        if(!(_joy2Status & JOY_LEFT) && _joyL != NOT_ATTACHED){
           _key = currentMenu[_joyL].key;
        }
        if(!(_joy2Status & JOY_RIGHT) && _joyR != NOT_ATTACHED){
           _key = currentMenu[_joyR].key;
        }
    }
}
static void _moveJoyThroughMenuLR(){
    if(!joyCursor.moveDelayCurrent){
        if(!(_joy2Status & JOY_UP) && _joyU != NOT_ATTACHED){
           _key = currentMenu[_joyU].key;
        }
        if(!(_joy2Status & JOY_DOWN) && _joyD != NOT_ATTACHED){
           _key = currentMenu[_joyD].key;
        }
        if(!(_joy2Status & JOY_LEFT)){
            _decMenuPos();
        }
        if(!(_joy2Status & JOY_RIGHT)){
            _incMenuPos();
        }
    }
}

static bool _checkJoyFire(){
    if(!joyCursor.moveDelayCurrent){
        if(!(_joy2Status & JOY_FIRE)){
            // set delay, without it it's impossible to control stuff, joy cursor moves too fast
            joyCursor.moveDelayCurrent = JOY_CURSOR_MOVE_DELAY_INIT;
            return true;
        }
    }
    return false;
}

// check keys that were pressed, including joystick, and load appropriate menu option
void checkKeys(){
    // get the key
    keyb_poll();
    _key = keyb_codes[keyb_key & 0x7f];
    // now get the joystick - some keys might be same as joy, so these will simulate keypresses if needed
    if(_menuUiMode == UI_UD) {
        _moveJoyThroughMenuUD();
    } else {
        _moveJoyThroughMenuLR();
    }

    byte i = 0;
    bool selected = false;

    while (i <= _lastElementInMenu){
        if (_key == currentMenu[i].key){
            // vic.color_border = VCOL_RED;
            joyCursor.menuPos = i;
            _setJoyCursorPos(joyCursor.menuPos);
            selected = true;
            break;
        }
        i++;
    }

    // none pressed? check for magic keys and handle joystick movement
    if(!selected){
        // vic.color_border++;
        // game speed control
        if(!gms_disableTimeControls){
            if (_key == '+'){
                if(gms_gameSpeed < SPEED_FASTEST){
                    gms_gameSpeed++;
                    updateGameSpeed();
                }
            } else if (_key == '-'){
                if(gms_gameSpeed > 0){
                    gms_gameSpeed--;
                    updateGameSpeed();
                }
            } else if (_key == ' '){
                if(gms_gameSpeed){
                    gms_gameSpeed = SPEED_PAUSED;
                    updateStatusBar(TXT[SB_IDX_PAUSE]);
                } else {
                    gms_gameSpeed = SPEED_NORMAL;
                    updateStatusBar(TXT[SB_IDX_UNPAUSE]);
                }
                updateGameSpeed();
            }
        }
        // help & options
        if (_key == KEY_F1) {
            // don't go to menu while in menu
            if(!_fullScreenMenuOpen){
                _fullScreenMenuOpen = true;
                gms_disableTimeControls = true;
                // vic.color_border = VCOL_BLUE;
                gms_gameSpeed = SPEED_PAUSED;
                updateGameSpeed();
                showOptionsMenu();
            }
            return;
        // task manager
        } else if (_key == KEY_F3) {
            // don't go to menu while in menu
            if(!_fullScreenMenuOpen){
                _fullScreenMenuOpen = true;
                gms_disableTimeControls = true;
                // vic.color_border = VCOL_BLUE;
                gms_gameSpeed = SPEED_PAUSED;
                updateGameSpeed();
                showTaskManagerMenu();
            }
            return;
        } else if (_key == KEY_F4) {
            // don't go to menu while in menu
            if(!_fullScreenMenuOpen){
                _fullScreenMenuOpen = true;
                gms_disableTimeControls = true;
                // vic.color_border = VCOL_BLUE;
                gms_gameSpeed = SPEED_PAUSED;
                updateGameSpeed();
                showTaskManagerPrioMenu();
            }
            return;
        }

        // joystick
        // _moveJoyThroughMenu();
        // check if fire pressed & if so read which element Joy was pointing at
        if(_checkJoyFire()){
            i = joyCursor.menuPos;
            selected = true;
        }
    }

    if(selected){
        // vic.color_border = VCOL_BLUE;

        // whatever it is, its not an options menu, as its handled above
        _fullScreenMenuOpen = false;
        // block any accidental key presses during transition
        joyCursor.enabled = false;
        // open new menu
        byte bank = currentMenu[i].bank;
        void (*jmpPtr)(void) = currentMenu[i].jmpPtr;
        // if menu is in any special bank - open it
        if(bank != 0){
            loadMenu(bank);
        }
        (*jmpPtr)();
        // its ok now to enable keys
        joyCursor.enabled = true;
    }

    // vic.color_border = VCOL_BLACK;
}

void updateGameSpeed(){
    gms_gameSpeedWait = GMS_WAIT_TIMES[gms_gameSpeed];
}

// put new text on the scroll
void updateStatusBar(const byte * text){
    SB_TEXT = text;
}

void copyCharToSprite(byte c, byte col, byte row){
    byte i = 0;
    byte si = colOffset[col] + rowOffset[row];
    unsigned int pos = c;
    pos = pos * 7;
    mmap_set(MMAP_NO_BASIC);
    do{
        sprBankPointer[si] = CALENDAR_FONTS[pos+i];
        i++;
        si++;
        si++;
        si++;
    } while (i<7);
    mmap_set(MMAP_ROM);
}

// Does not need to be called more than once - at new game
void drawFullDate(){
    // vic.color_border--;

    cal_dateHour = 13;
    cal_dateDay = 1;
    cal_dateMonth = 1;
    cal_dateYearH = 8;
    cal_dateYearL = 9;

    sprBankPointer = SpriteResources.DATE_TXT;
    // draw hour
    copyCharToSprite('1', 0, 0);
    copyCharToSprite('3', 1, 0);
    // 58 is :
    copyCharToSprite(':', 2, 0);
    copyCharToSprite('0', 3, 0);
    copyCharToSprite('0', 4, 0);

    // copy date to sprite
    copyCharToSprite('0', 0, 1);
    copyCharToSprite(39, 1, 1);
    copyCharToSprite('0', 2, 1);
    copyCharToSprite(39, 3, 1);

    // year
    drawYearL();
    drawYearH();

    // Cash
    copyCharToSprite('2', 0, 2);
    copyCharToSprite('4', 1, 2);
    copyCharToSprite('5', 2, 2);
    copyCharToSprite('0', 3, 2);
    copyCharToSprite('0', 4, 2);
    copyCharToSprite(28, 5, 2); //28 is 'zl'

    sprBankPointer = SpriteResources.CURRENCY_TXT;

    // toilet paper
    copyCharToSprite(50, 1, 2);
    // vic.color_border++;
}

void updateHour(byte newHour){
    sprBankPointer = SpriteResources.DATE_TXT;

    byte d = newHour;

    if(newHour > 19) {
        d = newHour - 20;
        if(cal_dateHour < 20)
            copyCharToSprite('2', 0, 0);
    } else if(newHour > 9){
        d = newHour - 10;
        if(cal_dateHour < 10)
            copyCharToSprite('1', 0, 0);
    } else {
        if(cal_dateHour > 9)
            copyCharToSprite('0', 0, 0);
    }
    copyCharToSprite(d+48, 1, 0);
}

void updateDay(byte newDay){
    sprBankPointer = SpriteResources.DATE_TXT;

    byte d = newDay;

    if(newDay > 29) {
        d = newDay - 30;
        if(cal_dateDay < 30)
            copyCharToSprite('3', 0, 1);
    } else if(newDay > 19) {
        d = newDay - 20;
        if(cal_dateDay < 20)
            copyCharToSprite('2', 0, 1);
    } else if(newDay > 9){
        d = newDay - 10;
        if(cal_dateDay < 10)
            copyCharToSprite('1', 0, 1);
    } else {
        if(cal_dateDay > 9)
            copyCharToSprite('0', 0, 1);
    }
    copyCharToSprite(d+38, 1, 1);
}

void updateMonth(byte newMonth){
    sprBankPointer = SpriteResources.DATE_TXT;

    byte d = newMonth;

    if(newMonth > 9){
        d = newMonth - 10;
        if(cal_dateMonth < 10)
            copyCharToSprite('1', 2, 1);
    } else {
        if(cal_dateMonth > 9)
            copyCharToSprite('0', 2, 1);
    }
    copyCharToSprite(d+38, 3, 1);
}

void drawYearH(){
    sprBankPointer = SpriteResources.DATE_TXT;
    copyCharToSprite(cal_dateYearH + 48, 4, 1);
}

void drawYearL(){
    sprBankPointer = SpriteResources.DATE_TXT;
    copyCharToSprite(cal_dateYearL + 48, 5, 1);
}