#include <c64/types.h>

#include <engine/logger.h>
#include <tick/calendar.h>

struct LogMsg LOG_MSGS[LOG_MAX_MSGS];
byte log_index = 0;

void logger(byte msgId) {
    // build and store LogMsg
    LOG_MSGS[log_index].hour  = cal_dateHour;
    LOG_MSGS[log_index].day   = cal_dateDay;
    LOG_MSGS[log_index].month = cal_dateMonth;
    LOG_MSGS[log_index].yearH = cal_dateYearH;
    LOG_MSGS[log_index].yearL = cal_dateYearL;
    LOG_MSGS[log_index].msgId = msgId;
    // LOG_MSGS[log_index].data[0] = data[0];
    // LOG_MSGS[log_index].data[1] = data[1];
    // LOG_MSGS[log_index].data[2] = data[2];
    // LOG_MSGS[log_index].data[3] = data[3];
    // LOG_MSGS[log_index].data[4] = data[4];
    // LOG_MSGS[log_index].data[5] = data[5];
    // LOG_MSGS[log_index].data[6] = data[6];
    // LOG_MSGS[log_index].data[7] = data[7];
    // LOG_MSGS[log_index].data[8] = data[8];
    // LOG_MSGS[log_index].data[9] = data[9];

    log_index++;
    if(log_index >= LOG_MAX_MSGS)
        log_index = 0;
}
