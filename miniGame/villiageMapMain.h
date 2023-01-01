#ifndef VILLIAGE_MAP_MAIN_H
#define VILLIAGE_MAP_MAIN_H

#define V_MAP_SIZE_X 32
#define V_MAP_SIZE_Y 24

#pragma section( villiageMapLoaderData, 0 )
#pragma section( villiageMapCode, 0 )
#pragma section( villiageMapConsts, 0 )
#pragma section( villiageMapRAMCode, 0 )
#pragma section( villiageMapRAMData, 0 )
#pragma section( villiageMapGfx1, 0 )
#pragma section( villiageMapGfx1Loaders, 0 )
#pragma region( regionVilliageMapCrt, 0x8000, 0xafff, , MENU_BANK_MAP_VILLIAGE_1, { villiageMapLoaderData, villiageMapCode, villiageMapConsts, villiageMapGfx1, villiageMapGfx1Loaders } )
#pragma region( regionVilliageMapRam, 0xb000, 0xbfff, , MENU_BANK_MAP_VILLIAGE_1, { villiageMapRAMCode, villiageMapRAMData }, 0x7000 )

#pragma data ( villiageMapRAMData )


#pragma code ( villiageMapRAMCode )
void villiageMapScreenInit();
void villiageMapSpriteLoader();
void villiageMapInit();

#pragma code ( villiageMapCode )
void mapGameLoop();

#pragma compile("villiageMapMain.c")

#pragma code ( code )
#pragma data ( data )

#endif