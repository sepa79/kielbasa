#include <stdlib.h>
#include <stdbool.h>
#include <c64/types.h>

#include "calendar.h"
#include <character/character.h>
#include <assets/mainGfx.h>
#include <tick/farmlandTick.h>


// Sections and regions
// #pragma section( ticksCode, 0 )
// #pragma section( ticksData, 0 )
// #pragma region( ticksCodeRegion, 0x8000, 0xbfff, , MENU_BANK_PIGSLE_COMMAND, { ticksCode, ticksData } )

volatile signed char cal_currentTemp;
volatile byte cal_currentRain;

volatile byte cal_dateHour  = 13;
volatile byte cal_dateDay   = 1;
volatile byte cal_dateMonth = 1;
volatile byte cal_dateYearH = 8;
volatile byte cal_dateYearL = 9;
volatile byte cal_dateWeek  = 0;
volatile byte cal_moonPhase = 0;
volatile bool cal_isDay = true;

volatile byte cal_dateWeekDay = 1; // starts monday

signed char rndTempWeekly;
signed char rndRainWeekly;
byte sunRaiseHour = 7;
byte sunSetHour = 15;

volatile byte weatherSprite = 0; // cache for night, hourly tick updates isc_weatherSprite

// static data
// #pragma data ( ticksData )

const byte MONTH_DAYS[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
// temp is in C
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

// ticks code
// #pragma code ( ticksCode )

static void _weatherTick(){
    unsigned int rnd = rand();
    byte rnd0 = rnd & TEMP_RND_RANGE;
    byte rnd1 = rnd & RAIN_RND_RANGE;

    cal_currentTemp = rnd0 - TEMP_RND_SHIFT + rndTempWeekly;
    cal_currentTemp += WEEKLY_AVG_TEMP[cal_dateWeek];

    signed char tempRain = rnd1 - RAIN_RND_SHIFT + rndRainWeekly + WEEKLY_AVG_RAIN[cal_dateWeek];

    if(tempRain < 0) {
        cal_currentRain = 0;
    } else if(tempRain > 4) {
        // max weather sprites
        cal_currentRain = 4;
    } else {
        cal_currentRain = (char) tempRain;
    }

    weatherSprite = cal_currentRain;

    if(cal_currentTemp < 1)
        if(weatherSprite == 3)
            weatherSprite = 5;

    // gotoxy(0, 24);
    // printf("Dzien  : %-3u %-3u %-3d %-3u", rnd0, rnd1, rndTemp, weatherSprite);
}

static void _weeklyWeatherRandomizer(){
    unsigned int  rnd = rand();
    byte rnd0 = rnd & TEMP_RND_RANGE_WEEKLY;
    byte rnd1 = rnd & RAIN_RND_RANGE_WEEKLY;
    rndTempWeekly = rnd0 - TEMP_RND_SHIFT_WEEKLY;
    rndRainWeekly = rnd1 - RAIN_RND_SHIFT_WEEKLY;
    // gotoxy(0, 23);
    // printf("Tydzien: %-3u %-3u %-3d %-3d", rnd0, rnd1, rndTempWeekly, rndRainWeekly);
}

static void _yearTick(){
    if(cal_dateYearL < 9){
        cal_dateYearL++;
    } else {
        cal_dateYearL = 0;

        if(cal_dateYearH < 9){
            cal_dateYearH++;
        } else {
            cal_dateYearH = 0;
        }
        drawYearH();
    }
    drawYearL();
    cal_dateWeek = 0;
}

static void _monthTick(){
    if(cal_dateMonth < 12){
        updateMonth(cal_dateMonth+1);
        cal_dateMonth++;
    } else {
        updateMonth(1);
        cal_dateMonth = 1;

        _yearTick();
    }
}

static void _dayTick(){
    if(cal_dateDay < MONTH_DAYS[cal_dateMonth-1]){
        updateDay(cal_dateDay+1);
        cal_dateDay++;
    } else {
        updateDay(1);
        cal_dateDay = 1;
        _monthTick();
    }
    
    cal_dateWeekDay++;
    if(cal_dateWeekDay > 7){
        cal_dateWeekDay = 1;
        cal_dateWeek++;
        // add weekly temp/rain randomizer
        _weeklyWeatherRandomizer();
        cal_moonPhase++;
        if(cal_moonPhase == MOON_PHASES)
            cal_moonPhase = 0;
    }

    _weatherTick();
    // that might need to go to ROM
    farmlandTick();
    sunRaiseHour = WEEKLY_SUNRISE[cal_dateWeek];
    sunSetHour = WEEKLY_SUNSET[cal_dateWeek];

    // Handle any installed callback - so that we can get 'refresh' on a screen the game is on
    updateMenu();
}

void timeTick(){
    if(cal_dateHour < 23){
        updateHour(cal_dateHour+1);
        cal_dateHour++;
    } else {
        updateHour(0);
        cal_dateHour = 0;
        _dayTick();
    }
    // day/night cycle
    if(cal_dateHour == sunRaiseHour){
        // restore the sun icon
        isc_weatherSprite = weatherSprite;
        // reload gfx to day one
        cal_isDay = true;
        loadMenuGfx(cal_isDay);
    } else if (cal_dateHour == sunSetHour){
        // update isc_weatherSprite to night version
        isc_weatherSprite = 6 + cal_moonPhase;
        // reload gfx to might one
        cal_isDay = false;
        loadMenuGfx(cal_isDay);
    }

    // sleep cycle
    if(cal_dateHour > 20 || cal_dateHour < 5) {
        // eat, then sleep well my little redneck - change task sprite once only
        if(cal_dateHour == 21){
            setCharacterSlotIcon(0, SPR_TASK_EAT);
            setCharacterSlotIcon(1, SPR_TASK_EAT);
            setCharacterSlotIcon(2, SPR_TASK_EAT);
            setCharacterSlotIcon(3, SPR_TASK_EAT);
        }
        if(cal_dateHour == 22){
            setCharacterSlotIcon(0, SPR_TASK_SLEEP);
            setCharacterSlotIcon(1, SPR_TASK_SLEEP);
            setCharacterSlotIcon(2, SPR_TASK_SLEEP);
            setCharacterSlotIcon(3, SPR_TASK_SLEEP);

            // reset busy stats
            for(byte charSlot=0;charSlot<CHARACTER_SLOTS;charSlot++){
                // only check active chars
                if(characterSlots[charSlot] != NO_CHARACTER){
                    byte charIdx = characterSlots[charSlot];
                    allChars_busy[charIdx] = false;
                }
            }
        }
        sleepTick();
    } else {
        // temp - tasksTick should do it later
        if(cal_dateHour == 5){
            setCharacterSlotIcon(0, SPR_TASK_MIA);
            setCharacterSlotIcon(1, SPR_TASK_MIA);
            setCharacterSlotIcon(2, SPR_TASK_MIA);
            setCharacterSlotIcon(3, SPR_TASK_MIA);
        }
        // tasks are done during daytime only
        tasksTick();
    }

}

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
