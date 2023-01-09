#ifndef VILLIAGE_MAP_MAIN_H
#define VILLIAGE_MAP_MAIN_H

#define V_MAP_SIZE_X 64
#define V_MAP_SIZE_Y 64

#pragma section( villiageMapLoaderData, 0 )
#pragma section( villiageMapCode, 0 )
#pragma section( villiageMapDisplayCode, 0 )
#pragma section( villiageMapData, 0 )
#pragma section( villiageMapRAMCode, 0 )
#pragma section( villiageMapRAMData, 0 )
#pragma section( villiageMapGfx1, 0 )
#pragma section( villiageMapGfx1Loaders, 0 )
// place for other code and data that is copied to RAM
#pragma region( regionVilliageMapRam1, 0x8000, 0xafff, , MENU_BANK_MAP_VILLIAGE_1, { villiageMapLoaderData, villiageMapCode, villiageMapGfx1, villiageMapGfx1Loaders } )
#pragma region( regionVilliageMapRam2, 0xb000, 0xbfff, , MENU_BANK_MAP_VILLIAGE_1, { villiageMapRAMCode, villiageMapRAMData }, 0x7000 )
// place for display code and maps
#pragma region( regionVilliageMapCrt, 0x8000, 0xbfff, , MENU_BANK_MAP_VILLIAGE_2, { villiageMapDisplayCode, villiageMapData } )

#pragma data ( villiageMapRAMData )
#pragma code ( villiageMapRAMCode )
// load and init routines, from MENU_BANK_MAP_VILLIAGE_1
void villiageMapScreenInit();
void villiageMapSpriteLoader();
void villiageMapInit();
// display routine, from MENU_BANK_MAP_VILLIAGE_2
void villiageMapDraw();

#pragma code ( villiageMapCode )
void villiageMapGameLoop();

#pragma compile("villiageMapMain.c")

#pragma code ( code )
#pragma data ( data )

#endif