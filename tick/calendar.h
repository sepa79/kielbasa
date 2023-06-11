#ifndef CALENDAR_H
#define CALENDAR_H
#include <stdbool.h>
#include <c64/types.h>
#include <engine/easyFlashBanks.h>

struct Calendar {
    volatile signed char currentTemp;
    volatile byte currentRain;

    volatile byte dateHour;
    volatile byte dateDay;
    volatile byte dateMonth;
    volatile byte dateYearH;
    volatile byte dateYearL;
    volatile byte dateWeek;
    volatile byte moonPhase;
    volatile bool isDay;
    volatile byte dateWeekDay;

    signed char rndTempWeekly;
    signed char rndRainWeekly;

    byte weatherSprite; // cache for night, hourly tick updates isc_weatherSprite
};

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

// Sections and regions
#pragma section( ticksCode, 0 )
#pragma section( ticksData, 0 )
#pragma region( ticksCodeRegion, 0x8000, 0xbfff, , TICKS_BANK, { ticksCode, ticksData } )

#pragma code ( ticksCode )
void timeTick();
#pragma code ( code )
void initCalendar(Calendar * cal);
void initCalendarAfterLoad();

#pragma compile("calendar.c")

#endif
