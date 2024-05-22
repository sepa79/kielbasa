#ifndef FISHING_MENU_H
#define FISHING_MENU_H

// Sections and regions
#pragma section(fishingMenuLoaderData, 0)
#pragma section(fishingMenuCode, 0)
#pragma section(fishingMenuRAMCode, 0)
#pragma section(fishingMenuRAMData, 0)
#pragma section(fishingMenuGfxDay, 0)
#pragma section(fishingMenuSprites, 0)
#pragma region(bankFishingMenuC, 0x8000, 0x8800, , MENU_BANK_FISHING_MENU, { fishingMenuLoaderData, fishingMenuCode })
#pragma region(bankFishingMenuRAM, 0x8800, 0x9000, , MENU_BANK_FISHING_MENU, { fishingMenuRAMCode, fishingMenuRAMData }, 0x7000 )
#pragma region(bankFishingMenuG1, DAY_GFX_BMP, DAY_GFX_BMP + 0x2711, , MENU_BANK_FISHING_MENU, { fishingMenuGfxDay })
#pragma region(bankFishingMenuG2, 0x8000, 0xC000, , MENU_BANK_FISHING_MENU_2, { fishingMenuSprites })

extern const struct MenuOption FISHING_MENU[];
extern const struct MenuOption FISHING_MENU2[];

#pragma code(fishingMenuRAMCode)
#pragma data(fishingMenuRAMData)

void initRasterIRQ_Fishing();

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )

#pragma compile("fishing.c")
#endif
