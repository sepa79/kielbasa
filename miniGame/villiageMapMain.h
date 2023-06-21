#ifndef VILLIAGE_MAP_MAIN_H
#define VILLIAGE_MAP_MAIN_H

#define V_MAP_SIZE_X 64
#define V_MAP_SIZE_Y 64
// first 0xXX tiles are fields, remap them to point to memory location where we got visualised fields
#define RAM_TILES_COUNT 0x2f

#pragma section( villiageMapLoaderData, 0 )
#pragma section( villiageMapCode, 0 )
#pragma section( villiageMapDisplayCode, 0 )
#pragma section( villiageMapData, 0 )
#pragma section( villiageMapData2, 0 )
#pragma section( villiageMapRAMCode, 0 )
#pragma section( villiageMapRAMData, 0 )
#pragma section( villiageMapGfx1, 0 )
#pragma section( villiageMapGfx1Loaders, 0 )
// place for other code and data that is copied to RAM
#pragma region( regionVilliageMapRam1, 0x8000, 0xafff, , MENU_BANK_MAP_VILLIAGE_1, { villiageMapLoaderData, villiageMapCode, villiageMapGfx1, villiageMapGfx1Loaders } )
#pragma region( regionVilliageMapRam2, 0xb000, 0xbfff, , MENU_BANK_MAP_VILLIAGE_1, { villiageMapRAMCode, villiageMapRAMData }, 0x7000 )
// place for display code and maps
#pragma region( regionVilliageMapReg1, 0x8000, 0xbfff, , MENU_BANK_MAP_VILLIAGE_2, { villiageMapDisplayCode, villiageMapData } )
#pragma region( regionVilliageMapReg2, 0x8000, 0xbfff, , MENU_BANK_MAP_VILLIAGE_3, { villiageMapData2 } )

#pragma data ( villiageMapRAMData )
#pragma code ( villiageMapRAMCode )

typedef char char256[256];
typedef char char1024[1024];

// clolor map + main char attribs, day light and materials (walkable, water etc) - defined as colorMap[CHAR_ATTRIBS]
extern char256 * const colorMap;
extern const char1024 allLightMaps[4];

// lightmap for night maps
extern char * lightMap;
// which map file holds main attribs, use like colorMap[CHAR_ATTRIBS]
#define CHAR_ATTRIBS 2
typedef char mapTile[16];
extern char * ramTiles;
extern const char * romTiles;
extern char moonDetailLevel;

// load and init routines, from MENU_BANK_MAP_VILLIAGE_1
void buildRamTiles();
void villiageMapScreenInit();
// display routine, from MENU_BANK_MAP_VILLIAGE_2
void villiageMapDraw();

#pragma code ( villiageMapCode )
void villiageMapGameLoop();

#pragma compile("villiageMapMain.c")

#pragma code ( code )
#pragma data ( data )

#endif