#ifndef PIGSLE_INTRO_H
#define PIGSLE_INTRO_H
#include <menu/menuSystem.h>

// Sections and regions
#pragma section(pigsleIntroLoaderData, 0)
#pragma section(pigsleIntroCode, 0)
#pragma section(pigsleIntroData, 0)
#pragma section(pigsleIntroMedia, 0)
#pragma section(pigsleIntroSprites, 0)
#pragma region(regionPigsleIntroC, 0x8000, 0x9000, , MENU_BANK_PIGSLE_INTRO, { pigsleIntroLoaderData, pigsleIntroCode, pigsleIntroData })
#pragma region(regionPigsleIntroG1, DAY_GFX_BMP, DAY_GFX_BMP + 0x4000, , MENU_BANK_PIGSLE_INTRO, { pigsleIntroMedia })

extern const struct MenuOption PIGSLE_INTRO_MENU[];

#pragma compile("pigsleIntro.c")
#endif
