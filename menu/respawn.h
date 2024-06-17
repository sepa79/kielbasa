#ifndef RESPAWN_H
#define RESPAWN_H
#include <menu/menuSystem.h>

// Sections and regions
#pragma section(respawnLoaderData, 0)
#pragma section(respawnCode, 0)
#pragma section(respawnData, 0)
#pragma section(respawnMedia, 0)
#pragma section(respawnSprites, 0)
#pragma region(regionRespawnC, 0x8000, 0x9000, , MENU_BANK_RESPAWN, { respawnLoaderData, respawnCode, respawnData })
#pragma region(regionRespawnG1, DAY_GFX_BMP, DAY_GFX_BMP + 0x3000, , MENU_BANK_RESPAWN, { respawnMedia })

extern const struct MenuOption RESPAWN_MENU[];

#pragma compile("respawn.c")
#endif  // RESPAWN_H
