#include <c64/vic.h>
#include <c64/types.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>
#include <c64/memmap.h>
#include <c64/easyflash.h>

#include <engine/easyFlashBanks.h>
#include <engine/gameState.h>
#include <engine/spriteText.h>
#include <assets/mainGfx.h>
#include <menu/menuSystem.h>
#include <menu/optionsMenu.h>
#include <menu/taskManagerMenu.h>
#include <menu/taskManagerPrioMenu.h>
#include <menu/logMenu.h>
#include <engine/gameSettings.h>
#include <engine/uiHandler.h>
#include <engine/joystick.h>
#include <translation/common.h>

#define BORDER_XPOS_LEFT 24
#define BORDER_YPOS_TOP 50

// used with sprite drawing methods
const byte colOffset[6] = {0,1,2, 64,65,66};
const byte rowOffset[3] = {0,21,42};
// set it to sprite bank that you wish to use copyCharToSprite() on
byte * sprBankPointer;

volatile struct JOY_CURSOR joyCursor = {true, false, 0, 0, 0, 0, 0};
// used by all menus
CharWin cw;

// static bool _fullScreenMenuOpen = false;
static byte _lastElementInMenu = 0;
struct MenuOption *currentMenu;
static byte _menuUiMode = 0;
static byte _key = 0;
#define NOT_ATTACHED 0xff
static byte _joyU = 0;
static byte _joyD = 0;
static byte _joyL = 0;
static byte _joyR = 0;
static byte _joyF = 0;
static byte _joyLF = 0;
static bool _joyFirePressed = false;
static byte _selectionsCount = 0;

static void _setJoyCursorPos(byte menuPos){
    if(currentMenu[menuPos].uiMode & UI_SELECT){
        joyCursor.x = (currentMenu[menuPos].x + cw.sx) * 8 + BORDER_XPOS_LEFT - 1;
        joyCursor.y = (currentMenu[menuPos].y + cw.sy) * 8 + BORDER_YPOS_TOP - 1;
        setNormalCursor();
    } else {
        // hide cursor
        joyCursor.x = 0;
    }
}

/* Displays text and colors, sets _lastElementInMenu */
static void _displayMenuText(){
    byte i = 0;
    _selectionsCount = 0;
    _joyL  = NOT_ATTACHED;
    _joyR  = NOT_ATTACHED;
    _joyU  = NOT_ATTACHED;
    _joyD  = NOT_ATTACHED;
    _joyF  = NOT_ATTACHED;
    _joyLF = NOT_ATTACHED;

    while (currentMenu[i].key != 0){
        if(!(currentMenu[i].uiMode & UI_HIDE)){
            // display text
            byte idx = currentMenu[i].textIdx;
            cwin_putat_string_raw(&cw, currentMenu[i].x, currentMenu[i].y, TXT[idx], VCOL_MED_GREY);
        }

        // Map joystick to keys
        if(currentMenu[i].uiMode & UI_L){
            _joyL = i;
        } else if(currentMenu[i].uiMode & UI_R){
            _joyR = i;
        } else if(currentMenu[i].uiMode & UI_U){
            _joyU = i;
        } else if(currentMenu[i].uiMode & UI_D){
            _joyD = i;
        } else if(currentMenu[i].uiMode & UI_F){
            _joyF = i;
        } else if(currentMenu[i].uiMode & UI_LF){
            _joyLF = i;
        }
        
        // count number of options, if none - disable the unassigned keys to prevent endless loops on dec/incmenupos
        if(currentMenu[i].uiMode & UI_SELECT){
            _selectionsCount++;
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
    clearStatusBar();
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
        if(currentMenu[joyCursor.menuPos].uiMode & UI_SELECT){
            // yep, exit loop
            done = true;
        // } else {
        //     // there is nothing to iterate through, debug
        //     vic.color_border--;
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
        if(currentMenu[joyCursor.menuPos].uiMode & UI_SELECT){
            // yep, exit loop
            done = true;
        // } else {
        //     // there is nothing to iterate through, debug
        //     vic.color_border--;
        }
    }
    // only call update pos if movement occured
    _setJoyCursorPos(joyCursor.menuPos);
}

static void _moveJoyThroughMenuAndAssignKeys(){
    _joyFirePressed = false;
    if(!joyCursor.moveDelayCurrent){
        if(!(_joy2Status & JOY_UP )){
            if(_joyU != NOT_ATTACHED){
                _key = currentMenu[_joyU].key;
            } else if (_selectionsCount) {
                _decMenuPos();
            }
        } else 
        if(!(_joy2Status & JOY_DOWN )){
            if(_joyD != NOT_ATTACHED){
               _key = currentMenu[_joyD].key;
            } else if (_selectionsCount) {
                _incMenuPos();
            }
        } else 
        if(!(_joy2Status & JOY_LEFT)){
            if(_joyL != NOT_ATTACHED){
                _key = currentMenu[_joyL].key;
            } else if (_selectionsCount) {
                _decMenuPos();
            }
        } else 
        if(!(_joy2Status & JOY_RIGHT)){
            if(_joyR != NOT_ATTACHED){
               _key = currentMenu[_joyR].key;
            } else if (_selectionsCount) {
                _incMenuPos();
            }
        }

        if(!(_joy2Status & JOY_FIRE)){
            // Fire pressed! lets wait and see if it is a Long Fire or not
            byte timer = 0;
            while (!(_joy2Status & JOY_FIRE)){
                // vic.color_border++;
                timer++;
                vic_waitFrame();
            }
            if(timer > 50){
                // Long fire - blindly attach key, wherever it was set or not is irrelevant
                _key = currentMenu[_joyLF].key;
                // now wait for fire to be released
                while(!(_joy2Status & JOY_FIRE)){
                    vic.color_border++;
                }
            } else {
                // normal fire - if there is no key attached, select current menu pos unless hidden
                if(_joyF != NOT_ATTACHED){
                    _key = currentMenu[_joyF].key;
                } else {
                    _joyFirePressed = true;
                }
            }
        }
        // set delay, without it it's impossible to control stuff, joy cursor moves too fast
        joyCursor.moveDelayCurrent = JOY_CURSOR_MOVE_DELAY_INIT;
    }
}

static byte previousScreenMode = 0;

static void _prepareFullScreenMenu() {
    // if we are in menu already, don't store previous screen
    if(!gms_inSpecialMenu){
        previousScreenMode = currentScreenMode;
        gms_inSpecialMenu = true;
    }
    switchScreenTo(SCREEN_TRANSITION);
    gms_disableTimeControls = true;
    gms_gameSpeed = SPEED_PAUSED;
    updateGameSpeed();
}

// used by F1-F7 menus, goes back to menu the Fx was called from
void backToPreviousMenu(){
    gms_disableTimeControls = false;
    // This will simply call currently mounted menu again
    showMenu();
    switchScreenTo(previousScreenMode);
    gms_inSpecialMenu = false;
}

// check keys that were pressed, including joystick, and load appropriate menu option
void checkKeys(){
    // get the key
    keyb_poll();
    _key = keyb_codes[keyb_key & 0x7f];
    // now get the joystick - some keys might be same as joy, so these will simulate keypresses if needed
    _moveJoyThroughMenuAndAssignKeys();

    // TODO: Rename to something useful, like selectedElement
    byte i = 0;
    bool selected = false;

    while (i <= _lastElementInMenu){
        if (_key == currentMenu[i].key){
            // vic.color_border = VCOL_RED;
            if(!(currentMenu[i].uiMode & UI_HIDE)){
                joyCursor.menuPos = i;
                _setJoyCursorPos(joyCursor.menuPos);
            }
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
                } else {
                    gms_gameSpeed = SPEED_NORMAL;
                }
                updateGameSpeed();
            }
        }
        // help & options
        if (_key == KEY_F1) {
            _prepareFullScreenMenu();
            showOptionsMenu();
            return;
        // task manager
        } else if (_key == KEY_F3) {
            _prepareFullScreenMenu();
            showTaskManagerMenu();
            return;
        // task manager priorities
        } else if (_key == KEY_F4) {
            _prepareFullScreenMenu();
            showTaskManagerPrioMenu();
            return;
        // logs
        } else if (_key == KEY_F7) {
            _prepareFullScreenMenu();
            showLogMenu();
            return;
        }

        // joystick
        // check if fire pressed & if so read which element Joy was pointing at
        if(_joyFirePressed){
            i = joyCursor.menuPos;
            selected = true;
        }
    }

    if(selected){
        // vic.color_border = VCOL_BLUE;
        // block any accidental key presses during transition
        joyCursor.enabled = false;
        // open new menu
        // switch IRQs if needed
        switchScreenTo(currentMenu[i].screenMode);

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
    setTimeSpeedIcon(gms_gameSpeed);
}

// put new text on the scroll
void updateStatusBar(const char * text){
    // reset flash for text
    isc_statusTextColorIdx = 0;
    setNormalCursor();
    textToSprite((char *)text, 4, SPR_TXT_UP_1);
}
void updateStatusBarError(const char * text){
    setErrorCursor();
    textToSprite((char *)text, 4, SPR_TXT_UP_1);
}
// clear status bar
void clearStatusBar(){
    updateStatusBar(p"                                    ");
}
// Does not need to be called more than once - at new game
void initUI(){
    // vic.color_border--;

    char str[6*3+1];
    char tmp[3];
    sprBankPointer = SPR_DATE_TXT1;
    // draw hour
    sprintf(str, "%02u:%02u     %d%d%5u", GS.calendar.dateHour, GS.calendar.dateMinute, GS.calendar.dateYearH, GS.calendar.dateYearL, GS.cash);

    // day, month with a . (+0x40)
    sprintf(tmp, "%02u", GS.calendar.dateDay);
    str[6] = tmp[0];
    str[7] = tmp[1]+0x40;

    sprintf(tmp, "%02u", GS.calendar.dateMonth);
    str[8] = tmp[0];
    str[9] = tmp[1]+0x40;

    str[17] = 28; // zl
    str[18] = 0;
    textToSprite(str, 2, SPR_DATE_TXT1);
    // vic.color_border++;
}

void updateMoney(){
    char str[6+1];
    sprintf(str, "%5u", GS.cash);
    str[5] = 28; // zl
    str[6] = 0;
    textToSpriteAt(str, 2, SPR_DATE_TXT1, 0, 2);
}

void updateMinute(byte newMinute){
    sprBankPointer = SPR_DATE_TXT1;

    char num2str[3];
    sprintf(num2str, "%02d", newMinute);
    copyCharToSprite(num2str[0], 3, 0, sprBankPointer);
    copyCharToSprite(num2str[1], 4, 0, sprBankPointer);
}

void updateHour(byte newHour){
    sprBankPointer = SPR_DATE_TXT1;

    byte d = newHour;

    if(newHour > 19) {
        d = newHour - 20;
        if(GS.calendar.dateHour < 20)
            copyCharToSprite('2', 0, 0, sprBankPointer);
    } else if(newHour > 9){
        d = newHour - 10;
        if(GS.calendar.dateHour < 10)
            copyCharToSprite('1', 0, 0, sprBankPointer);
    } else {
        if(GS.calendar.dateHour > 9)
            copyCharToSprite('0', 0, 0, sprBankPointer);
    }
    copyCharToSprite(d+48, 1, 0, sprBankPointer);
}

void updateDay(byte newDay){
    sprBankPointer = SPR_DATE_TXT1;

    byte d = newDay;

    if(newDay > 29) {
        d = newDay - 30;
        if(GS.calendar.dateDay < 30)
            copyCharToSprite('3', 0, 1, sprBankPointer);
    } else if(newDay > 19) {
        d = newDay - 20;
        if(GS.calendar.dateDay < 20)
            copyCharToSprite('2', 0, 1, sprBankPointer);
    } else if(newDay > 9){
        d = newDay - 10;
        if(GS.calendar.dateDay < 10)
            copyCharToSprite('1', 0, 1, sprBankPointer);
    } else {
        if(GS.calendar.dateDay > 9)
            copyCharToSprite('0', 0, 1, sprBankPointer);
    }
    copyCharToSprite(d+112, 1, 1, sprBankPointer);
}

void updateMonth(byte newMonth){
    sprBankPointer = SPR_DATE_TXT1;

    byte d = newMonth;

    if(newMonth > 9){
        d = newMonth - 10;
        if(GS.calendar.dateMonth < 10)
            copyCharToSprite('1', 2, 1, sprBankPointer);
    } else {
        if(GS.calendar.dateMonth > 9)
            copyCharToSprite('0', 2, 1, sprBankPointer);
    }
    copyCharToSprite(d+112, 3, 1, sprBankPointer);
}

void drawYearH(){
    sprBankPointer = SPR_DATE_TXT1;
    copyCharToSprite(GS.calendar.dateYearH + 48, 4, 1, sprBankPointer);
}

void drawYearL(){
    sprBankPointer = SPR_DATE_TXT1;
    copyCharToSprite(GS.calendar.dateYearL + 48, 5, 1, sprBankPointer);
}