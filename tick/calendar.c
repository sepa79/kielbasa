#include <stdlib.h>
#include <stdbool.h>
#include <c64/types.h>

#include <engine/logger.h>

#include "calendar.h"
#include <character/character.h>
#include <assets/mainGfx.h>
#include <tick/farmlandTick.h>
#include <tick/characterTick.h>
#include <engine/easyFlashBanks.h>
#include <engine/gameSettings.h>

// dynamic data - in RAM
#pragma data ( data )

static byte sunRaiseHour;
static byte sunSetHour;
static byte weatherSprite; // cache for night, hourly tick updates isc_weatherSprite

// constant data
#pragma data ( ticksData )

const byte MONTH_DAYS[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
// temp is in Celsius
// https://meteomodel.pl/dane/srednie-miesieczne/?imgwid=353230295&par=tm&max_empty=2
const signed char WEEKLY_AVG_TEMP[53] = {
 -1,  -1,   0,   1,   1,   2,   1,   4,
  5,   2,   4,   6,   6,   4,  13,   7,
 12,  11,  12,  15,  15,  17,  15,  14,
 17,  18,  21,  21,  20,  22,  19,  18,
 21,  18,  13,  14,  12,  16,  12,   8,
  9,   9,  12,   9,   4,   0,  -4,  -3,
 -5,  -6,  -5,  -3,  -1
};

const byte WEEKLY_AVG_RAIN[53] = {
  1,   1,   1,   1,   1,   1,   2,   2,
  1,   1,   2,   1,   2,   1,   1,   3,
  1,   1,   3,   0,   1,   1,   3,   3,
  1,   1,   1,   2,   5,   2,   3,   2,
  1,   2,   4,   2,   2,   2,   2,   4,
  4,   2,   2,   4,   2,   2,   1,   2,
  2,   3,   3,   1,   1,
};

const byte WEEKLY_SUNRISE[53] = {
  7,   7,   7,   7,   7,   7,   6,   6,
  6,   6,   5,   5,   6,   5,   5,   5,
  5,   4,   4,   4,   4,   4,   4,   4,
  4,   4,   4,   4,   4,   4,   4,   5,
  5,   5,   5,   5,   6,   6,   6,   6,
  6,   7,   7,   6,   6,   6,   7,   7,
  7,   7,   7,   7,   7
};
const byte WEEKLY_SUNSET[53] = {
 15,  15,  15,  16,  16,  16,  16,  17,
 17,  17,  17,  17,  19,  19,  19,  19,
 19,  20,  20,  20,  20,  20,  20,  21,
 21,  21,  21,  20,  20,  20,  20,  20,
 20,  19,  19,  19,  18,  18,  18,  18,
 17,  17,  17,  16,  15,  15,  15,  15,
 15,  15,  15,  15,  15
};

// ---------------------------------------------------------------------------------------------
// ticks code
#pragma code ( ticksCode )

static void _weatherTick(){
    unsigned int rnd = rand();
    byte rnd0 = rnd & TEMP_RND_RANGE;
    byte rnd1 = rnd & RAIN_RND_RANGE;

    GS.calendar.currentTemp = rnd0 - TEMP_RND_SHIFT + GS.calendar.rndTempWeekly;
    GS.calendar.currentTemp += WEEKLY_AVG_TEMP[GS.calendar.dateWeek];

    signed char tempRain = rnd1 - RAIN_RND_SHIFT + GS.calendar.rndRainWeekly + WEEKLY_AVG_RAIN[GS.calendar.dateWeek];

    if(tempRain < 0) {
        GS.calendar.currentRain = 0;
    } else if(tempRain > 4) {
        // max weather sprites
        GS.calendar.currentRain = 4;
    } else {
        GS.calendar.currentRain = (char) tempRain;
    }

    weatherSprite = GS.calendar.currentRain;

    // snow instead of rain if temp 0 or below
    if(GS.calendar.currentTemp < 1)
        if(weatherSprite == 3)
            weatherSprite = 5;

    LOG_MSG.LOG_DATA_CONTEXT = LOG_DATA_CONTEXT_WEATHER_REPORT;
    LOG_MSG.data[1] = GS.calendar.currentTemp;
    LOG_MSG.data[2] = GS.calendar.currentRain;
    LOG_MSG.data[3] = rnd0;
    LOG_MSG.data[4] = rnd1;
    LOG_MSG.data[5] = GS.calendar.rndTempWeekly;
    LOG_MSG.data[6] = GS.calendar.rndRainWeekly;
    LOG_MSG.data[7] = WEEKLY_AVG_TEMP[GS.calendar.dateWeek];
    LOG_MSG.data[8] = WEEKLY_AVG_RAIN[GS.calendar.dateWeek];
    LOG_MSG.data[9] = 0;
    logger(LOG_DEBUG | LOG_MSG_REPORT);

    // gotoxy(0, 24);
    // printf("Dzien  : %-3u %-3u %-3d %-3u", rnd0, rnd1, rndTemp, weatherSprite);
}

static void _weeklyWeatherRandomizer(){
    unsigned int  rnd = rand();
    byte rnd0 = rnd & TEMP_RND_RANGE_WEEKLY;
    byte rnd1 = rnd & RAIN_RND_RANGE_WEEKLY;
    GS.calendar.rndTempWeekly = rnd0 - TEMP_RND_SHIFT_WEEKLY;
    GS.calendar.rndRainWeekly = rnd1 - RAIN_RND_SHIFT_WEEKLY;
    // gotoxy(0, 23);
    // printf("Tydzien: %-3u %-3u %-3d %-3d", rnd0, rnd1, rndTempWeekly, rndRainWeekly);
}

static void _yearTick(){
    if(GS.calendar.dateYearL < 9){
        GS.calendar.dateYearL++;
    } else {
        GS.calendar.dateYearL = 0;

        if(GS.calendar.dateYearH < 9){
            GS.calendar.dateYearH++;
        } else {
            GS.calendar.dateYearH = 0;
        }
        drawYearH();
    }
    drawYearL();
    GS.calendar.dateWeek = 0;
}

static void _monthTick(){
    if(GS.calendar.dateMonth < 12){
        updateMonth(GS.calendar.dateMonth+1);
        GS.calendar.dateMonth++;
    } else {
        updateMonth(1);
        GS.calendar.dateMonth = 1;
        _yearTick();
    }

    // monthly bills, display TV Screen
    gms_event = EVENT_END_OF_MONTH;
}

static void _dayTick(){
    if(GS.calendar.dateDay < MONTH_DAYS[GS.calendar.dateMonth-1]){
        updateDay(GS.calendar.dateDay+1);
        GS.calendar.dateDay++;
    } else {
        updateDay(1);
        GS.calendar.dateDay = 1;
        _monthTick();
    }
    
    GS.calendar.dateWeekDay++;
    if(GS.calendar.dateWeekDay > 7){
        GS.calendar.dateWeekDay = 1;
        GS.calendar.dateWeek++;
        // add weekly temp/rain randomizer
        _weeklyWeatherRandomizer();
        GS.calendar.moonPhase++;
        if(GS.calendar.moonPhase == MOON_PHASES)
            GS.calendar.moonPhase = 0;
    }

    _weatherTick();
    kitchenTick();
    farmlandTick();
    sunRaiseHour = WEEKLY_SUNRISE[GS.calendar.dateWeek];
    sunSetHour = WEEKLY_SUNSET[GS.calendar.dateWeek];

    // Handle any installed callback - so that we can get 'refresh' on a screen the game is on
    updateMenu();
}

// used by timeTick and by init
static void _setDay() {
    // restore the sun icon
    setWeatherIcon(weatherSprite);
    // reload gfx to day one
    GS.calendar.isDay = true;
}
// used by timeTick and by init
static void _setNight() {
    // update isc_weatherSprite to night version
    setWeatherIcon(6 + GS.calendar.moonPhase);
    // reload gfx to night one
    GS.calendar.isDay = false;
}

void hourTick(){
    if(GS.calendar.dateHour < 23){
        updateHour(GS.calendar.dateHour+1);
        GS.calendar.dateHour++;
    } else {
        updateHour(0);
        GS.calendar.dateHour = 0;
        _dayTick();
    }
    // day/night cycle
    if(GS.calendar.dateHour == sunRaiseHour){
        _setDay();
        // has to be separated from _setDay as it must not be called in init
        loadMenuGfx();
    } else if (GS.calendar.dateHour == sunSetHour){
        _setNight();
        loadMenuGfx();
    }
}

void timeTick(){
    hourTick();
    // sleep & food cycle
    // run the regen routine - depending on levels of food some energy comes back
    // run before sleep, as sleep looks at regen timers
    regenTick();
    if(GS.calendar.dateHour > 20 || GS.calendar.dateHour < 6) {
        if(GS.calendar.dateHour == 5){
            breakfastTick();
        }
        if(GS.calendar.dateHour == 21){
            supperTick();
        }

        // eat, then sleep well my little redneck - change task sprite once only
        if(GS.calendar.dateHour == 22){
            setCharacterSlotIcon(0, SPR_TASK_SLEEP);
            setCharacterSlotIcon(1, SPR_TASK_SLEEP);
            setCharacterSlotIcon(2, SPR_TASK_SLEEP);
            setCharacterSlotIcon(3, SPR_TASK_SLEEP);

            // reset busy stats
            unassignAllTasks();
        }
        // runs during breakfast and supper, too, considering it a 'rest' time
        sleepTick();
    } else {
        // temp - tasksTick should do it later
        if(GS.calendar.dateHour == 6){
            setCharacterSlotIcon(0, SPR_TASK_MIA);
            setCharacterSlotIcon(1, SPR_TASK_MIA);
            setCharacterSlotIcon(2, SPR_TASK_MIA);
            setCharacterSlotIcon(3, SPR_TASK_MIA);
        }
        // tasks are done during daytime only
        tasksTick();
        // now process guys that are MIA - their energy is also spent
        miaTick();
    }

    // redraw batteries
    for(byte charSlot=0;charSlot<CHARACTER_SLOTS;charSlot++){
        // only check active chars
        if(characterSlots[charSlot] != NO_CHARACTER){
            drawBattery(characterSlots[charSlot]);
        }
    }

}

//-----------------------------------------------------------------------------------------
// In Init bank
#pragma code ( gameInitRAMCode )
#pragma data ( gameInitData )
//-----------------------------------------------------------------------------------------

// fully init Calendar - new game
void initCalendar(Calendar * cal) {
    byte pbank = setBank(TICKS_BANK);
    setWeatherIcon(weatherSprite);

    cal->dateMinute  = 0;
    cal->dateHour    = 13;
    cal->dateDay     = 1;
    cal->dateMonth   = 5;
    cal->dateYearH   = 8;
    cal->dateYearL   = 9;
    cal->dateWeek    = 17;
    cal->moonPhase   = 0;
    cal->isDay       = true;
    cal->dateWeekDay = 1; // starts monday

    // set by _weatherTick()
    // cal->rndTempWeekly;
    // cal->rndRainWeekly;

    // update weather randoms
    _weatherTick();
    // set it twice to ensure it changes to correct one, as it only redraws on change
    setWeatherIcon(weatherSprite);

    setBank(pbank);
    initCalendarAfterLoad();
}

// used once game has been initialised or loaded
void initCalendarAfterLoad() {
    byte pbank = setBank(TICKS_BANK);
    // init sun
    sunRaiseHour = WEEKLY_SUNRISE[GS.calendar.dateWeek];
    sunSetHour = WEEKLY_SUNSET[GS.calendar.dateWeek];
    // day/night cycle
    if(GS.calendar.dateHour >= sunRaiseHour && GS.calendar.dateHour < sunSetHour){
        _setDay();
    } else {
        _setNight();
    }
    setBank(pbank);
}
//-----------------------------------------------------------------------------------------
// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
//-----------------------------------------------------------------------------------------