#ifndef CALENDAR_H
#define CALENDAR_H
#include <stdbool.h>
#include <c64/types.h>

extern volatile signed char cal_currentTemp;
extern volatile byte cal_currentRain;

extern volatile byte cal_dateHour;
extern volatile byte cal_dateDay;
extern volatile byte cal_dateMonth;
extern volatile byte cal_dateYearH;
extern volatile byte cal_dateYearL;
extern volatile byte cal_dateWeek;
extern volatile byte cal_moonPhase;
extern volatile bool cal_isDay;
extern volatile byte cal_dateWeekDay;
#define MOON_PHASES 4

extern const byte MONTH_DAYS[12];

// ranges are done using AND, so we want %111 or similar here
#define TEMP_RND_RANGE 7
#define TEMP_RND_SHIFT 3
#define TEMP_RND_RANGE_WEEKLY 3
#define TEMP_RND_SHIFT_WEEKLY 1
extern const signed char WEEKLY_AVG_TEMP[53];
extern const byte WEEKLY_AVG_RAIN[53];

#define RAIN_RND_RANGE 3
#define RAIN_RND_SHIFT 2
#define RAIN_RND_RANGE_WEEKLY 3
#define RAIN_RND_SHIFT_WEEKLY 1
extern const byte WEEKLY_SUNRISE[53];
extern const byte WEEKLY_SUNSET[53];

void timeTick();

#pragma compile("calendar.c")

#endif
