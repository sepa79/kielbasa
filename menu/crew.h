#ifndef EMPLOYMENT_H
#define EMPLOYMENT_H

#pragma section( crewLoaderData, 0 )
#pragma section( crewCode, 0 )
#pragma section( crewData, 0 )
#pragma section( crewCodeRam, 0 )
#pragma section( crewGfxDay, 0 )
#pragma region( regionCrewC, 0x8000, 0x9000, , MENU_BANK_CREW, { crewLoaderData, crewCode, crewData } )
#pragma region( regionCrewG1, DAY_GFX_BMP, DAY_GFX_BMP+0x12c0, , MENU_BANK_CREW, { crewGfxDay } )

// will be on cartridge under 0xa2c0 (NIGHT_GFX_BMP) but compiled under 0x7000
#pragma region( regionCrewRam, NIGHT_GFX_BMP, NIGHT_GFX_BMP+0x1000, , MENU_BANK_CREW, { crewCodeRam }, 0x7000 )

// column offset for printing character data
#define CHARACTERDATA_X 2
#define CHARACTERDATA_Y 9
#define CHARACTERDATA_W 17
#define CHARACTERDATA_H 15

#define SPR_BANK_CREW_SCREEN_CHARACTER_PORTRAIT 0xbc

// drawing bars
#define PARAMS_COUNT        7
#define BARS_X_POSITION     120     // align to 8
#define BARS_Y_POSITION_MAX 82
#define BARS_X_COORDS_GAP   16      // draw new bar every 16 bits ( 2 bytes )
#define BAR_PART_HEIGHT     1
#define BAR_MAX_HEIGHT      ( 50 * BAR_PART_HEIGHT )
#define BAR_PATTERN         0x2a    // 0x2a = 0b00101010
#define BAR_PATTERN_CLEAR   0x00    // delete bar pattern

extern const struct MenuOption CREW_MENU[];

#pragma compile("crew.c")
#endif
