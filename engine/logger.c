#include <c64/types.h>
#include <c64/memmap.h>

#include <engine/logger.h>
#include <engine/gameState.h>

#pragma section( logMsgsSection, 0 )
#pragma region( logMsgsRegion, 0xa000, 0xb000, , , { logMsgsSection } )

#pragma data ( logMsgsSection )
struct LogMsg LOG_MSGS[LOG_MAX_MSGS] = {{ 0 }};
#pragma data ( data )

byte log_index = 0;

// clears out log memory, resets index
void loggerInit(){
    log_index = 0;
    memset(((char *)0xa000), 0, 0x1000);
}

// Sets the timestamp, msgId and moves log_index up. User needs to set LOG_DATA on its own, like:
// - For simple log with some text, max chars == LOG_DATA_SIZE
//    memcpy(LOG_DATA, p"Game Start", LOG_DATA_SIZE);
//    logger(LOG_INFO | LOG_MSG_TEXT);
// - Contexts - no text, just encoded data decrypted by LogMenu.h. Data[0] is specyfying context.
void logger(byte msgId) {
    // build and store LogMsg
    LOG_MSGS[log_index].hour  = GS.calendar.dateHour;
    LOG_MSGS[log_index].day   = GS.calendar.dateDay;
    LOG_MSGS[log_index].month = GS.calendar.dateMonth;
    LOG_MSGS[log_index].yearH = GS.calendar.dateYearH;
    LOG_MSGS[log_index].yearL = GS.calendar.dateYearL;
    LOG_MSGS[log_index].msgId = msgId;

    log_index++;
    if(log_index == LOG_MAX_MSGS){
        log_index = 0;
    }
}
