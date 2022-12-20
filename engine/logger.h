#ifndef LOGGER_H
#define LOGGER_H

#include <c64/types.h>

#define LOG_MAX_MSGS 64

// log levels
#define LOG_INFO  0b00
#define LOG_DEBUG 0b01
#define LOG_ERROR 0b10

// log sublevels, used as 1st byte of data in certain msgIds
#define LOG_TASK_ASSIGNED_TO_WORKER 1
#define LOG_TASK_STATUS_UNKNOWN 2
#define LOG_TASK_NEW_TASK 3
#define LOG_TASK_REMOVE_TASK 4

// msgId
#define LOG_MSG_TEXT 0b00000100
#define LOG_MSG_TASK 0b00001000

// Log structure
struct LogMsg {
    byte hour;
    byte day;
    byte month;
    byte yearH;
    byte yearL;
    /* Indicates how to decode this log message */
    byte msgId;
    byte data[10];
};

extern byte log_index;
extern struct LogMsg LOG_MSGS[LOG_MAX_MSGS];

#define LOG_DATA LOG_MSGS[log_index].data
void logger(byte msgId);

#pragma compile("logger.c")

#endif