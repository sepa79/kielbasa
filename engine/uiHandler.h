#ifndef UI_HANDLER_H
#define UI_HANDLER_H
#include <c64/types.h>
#include <c64/charwin.h>

struct JOY_CURSOR {
    // should it be shown, and also should the game run - freeze during menu changes
    bool enabled;
    // normal or error cursor
    bool error;
    // iterator for colors
    byte colorIdx;
    // which menu position is selected
    byte menuPos;
    // can we move it, or not yet (delay so it does not jump around too quickly)
    byte moveDelayCurrent;
    // screen X pos
    unsigned int x;
    // screen y pos
    byte y;
};

extern volatile struct JOY_CURSOR joyCursor;

void initUI();
void displayMenu(struct MenuOption * menu);
void backToPreviousMenu();
void checkKeys();
void updateStatusBar(const char * text);
void updateStatusBarError(const char * text);
void clearStatusBar();
void updateGameSpeed();
void updateMinute(char newMinute);
void updateHour(char newHour);
void updateDay(char newDay);
void updateMonth(char newMonth);
void updateMoney();
void drawYearH();
void drawYearL();

// global CharWin
extern CharWin cw;

#pragma compile("uiHandler.c")

#endif
