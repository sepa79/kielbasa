#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H
#include <c64/types.h>

// game speed control
enum GMS_WAIT_TIMES_ENUM {
    WAIT_TIME_PAUSED=255,
    WAIT_TIME_SLOWEST=254,
    WAIT_TIME_SLOWER=128,
    WAIT_TIME_SLOW=96,
    WAIT_TIME_NORMAL=64,
    WAIT_TIME_FAST=32,
    WAIT_TIME_FASTER=8,
    WAIT_TIME_FASTEST=0
};
static const byte GMS_WAIT_TIMES[8] = {WAIT_TIME_PAUSED, WAIT_TIME_SLOWEST, WAIT_TIME_SLOWER, WAIT_TIME_SLOW, WAIT_TIME_NORMAL, WAIT_TIME_FAST, WAIT_TIME_FASTER, WAIT_TIME_FASTEST};

enum GMS_GAME_SPEEDS {SPEED_PAUSED, SPEED_SLOWEST, SPEED_SLOWER, SPEED_SLOW, SPEED_NORMAL, SPEED_FAST, SPEED_FASTER, SPEED_FASTEST};

// Determines if main() can do a next loop iteration - will only do if 0
extern volatile byte gms_gameSpeed;
extern volatile byte gms_gameSpeedWait;
// frame indicator - for taks that are to be executed once per frame
// IRQs set it frame pos, game loop sets it to false after tasks are done
enum GMS_FRAME_POS {
    FRAME_TOP,
    FRAME_MIDDLE,
    FRAME_BOTTOM,
    FRAME_TOP_BORDER,
    FRAME_UNKNOWN,
};

extern volatile byte gms_framePos;

// // enable full screen text mode
// extern volatile bool gms_textMode;

// certain menus block map time passing - options
extern volatile bool gms_inSpecialMenu;

// certain menus block time passing - options, inside shop
extern volatile bool gms_disableTimeControls;

// in some cases menu sprites might need to be disabled - during data load and transitions
extern volatile bool gms_enableMenuSprites;

// music on/off
extern volatile bool gms_enableMusic;
extern volatile char gms_musicPlayCount;
extern volatile bool gms_musicPlayNow;

// *****************************
// External vars, kept here to simplify imports
// *****************************

// Bank for Weather Sprite, handled by irqSpriteController
extern volatile byte isc_weatherSprite;

// breaking naming convention as it used to be a #define
extern volatile char JOY_CURSOR_MOVE_DELAY_INIT;

// debug variable, counts frames (showUiSpritesTop() increases it every time it is called)
extern volatile char gms_frameCount;

enum Events {
    EVENT_NONE,
    EVENT_END_OF_MONTH,
    EVENT_XMAS,
    EVENT_RESPAWN_AT_HOME,
};

extern Events gms_event;

#pragma compile("gameSettings.c")
#endif