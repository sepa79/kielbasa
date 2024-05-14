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
#define CHARACTER_DATA_X 9
#define CHARACTER_DATA_Y 2
#define CHARACTER_DATA_W 28
#define CHARACTER_DATA_H 5

#define CHARACTER_STORY_X 2
#define CHARACTER_STORY_Y 9
#define CHARACTER_STORY_W 17
#define CHARACTER_STORY_H 15

#define SPR_BANK_CREW_SCREEN_CHARACTER_PORTRAIT 0xbc

extern const struct MenuOption CREW_MENU[];

#pragma compile("crew.c")
#endif
