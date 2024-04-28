#include <engine/gameSettings.h>
#include <c64/types.h>

// Selects one of the elements from the Enum above, main loop does 
// reset the wait time like this: gms_gameSpeedWait = GMS_GAME_SPEEDS[gms_gameSpeed]
volatile byte gms_gameSpeed = SPEED_PAUSED;
// Determines if main() can do a next loop iteration - will only do if 0
// IRQs decrease the gms_gameSpeedWait
volatile byte gms_gameSpeedWait = WAIT_TIME_PAUSED;
// frame indicator - for taks that are to be executed once per frame
// IRQs set it true, game loop sets it to false after tasks are done
volatile byte gms_framePos = FRAME_UNKNOWN;

volatile bool gms_disableTimeControls = false;
volatile bool gms_inSpecialMenu = false;
volatile bool gms_enableMenuSprites = true;
volatile bool gms_enableMusic = false;
volatile char gms_frameCount = 0;

volatile char JOY_CURSOR_MOVE_DELAY_INIT = 8;

Events gms_event = EVENT_NONE;