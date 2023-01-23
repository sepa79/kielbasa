#ifndef LOGGER_H
#define LOGGER_H

#include <c64/types.h>

// size of log array, its (6 + LOG_DATA_SIZE) * LOG_MAX_MSGS, in bytes
// keep sizes the powers of 2:
// 6+10=16
// 6+26=32
// ...
#define LOG_DATA_SIZE 10
#define LOG_SIZE LOG_DATA_SIZE + 6
#define LOG_MAX_MSGS 128

// log levels
#define LOG_INFO  0b00
#define LOG_DEBUG 0b01
#define LOG_ERROR 0b10

// shortcuts to use logs nicely
#define LOG_MSG LOG_MSGS[log_index]
#define LOG_DATA LOG_MSGS[log_index].data
// LOG_MSGS Data section shortcuts
// BYTE 0
#define LOG_DATA_CONTEXT data[0]
// ---------------------------------------------------------
// log sublevels and their data, used as data[0] in certain msgIds
// ---------------------------------------------------------

// TASKS - msgId LOG_MSG_TASK

// Contexts (byte 0 of data)
#define LOG_DATA_CONTEXT_TASK_ASSIGNED_TO_WORKER 1
#define LOG_DATA_CONTEXT_TASK_STATUS_UNKNOWN 2
#define LOG_DATA_CONTEXT_TASK_NEW_TASK 3
#define LOG_DATA_CONTEXT_TASK_REMOVE_TASK 4
#define LOG_DATA_CONTEXT_TASK_FARM_SOW_ENTRY 5
#define LOG_DATA_CONTEXT_TASK_FARM_SOW_EXIT 6
#define LOG_DATA_CONTEXT_TASK_FARM_SOW_DONE 7
#define LOG_DATA_CONTEXT_TASK_FARM_SOW_NOT_ENOUGH_ENERGY 8
#define LOG_DATA_CONTEXT_TASK_FARM_SOW_NOT_ENOUGH_SEEDS 9

#define LOG_DATA_CONTEXT_TASK_KITCHEN_BAKE_BREAD 20

#define LOG_DATA_CONTEXT_WEATHER_REPORT 100
// BYTE 1++ task data continues - shortcuts for writing
#define LOG_DATA_TASK_NAMEIDX data[1]
#define LOG_DATA_TASK_WORKER  data[2]
#define LOG_DATA_TASK_STATUS  data[3]
#define LOG_DATA_TASK_ID      data[4]
#define LOG_DATA_TASK_PARAMS1 data[5]
#define LOG_DATA_TASK_PARAMS2 data[6]
#define LOG_DATA_TASK_PARAMS3 data[7]
#define LOG_DATA_TASK_PARAMS4 data[8]
#define LOG_DATA_TASK_PARAMS5 data[9]

// msgId
#define LOG_MSG_TEXT   0b00000100
#define LOG_MSG_TASK   0b00001000
#define LOG_MSG_REPORT 0b00001100

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

void loggerInit();
void logger(byte msgId);

#pragma compile("logger.c")

#endif