#include <c64/types.h>

#include <engine/logger.h>
#include <tick/calendar.h>

struct LogMsg LOG_MSGS[LOG_MAX_MSGS];
byte log_index = 0;

// sets the timestamp, msgId and moves log_index up. user needs to set LOG_DATA on its own, like:
// - Simple log some text, max chars == LOG_DATA_SIZE
//    memcpy(LOG_DATA, p"Game Start", 10);
//    logger(LOG_INFO | LOG_MSG_TEXT);
// - Sublevels - no text, just encoded data decrypted by LogMenu.h

void logger(byte msgId) {
    // build and store LogMsg
    LOG_MSGS[log_index].hour  = cal_dateHour;
    LOG_MSGS[log_index].day   = cal_dateDay;
    LOG_MSGS[log_index].month = cal_dateMonth;
    LOG_MSGS[log_index].yearH = cal_dateYearH;
    LOG_MSGS[log_index].yearL = cal_dateYearL;
    LOG_MSGS[log_index].msgId = msgId;

    log_index++;
    if(log_index >= LOG_MAX_MSGS)
        log_index = 0;
}
