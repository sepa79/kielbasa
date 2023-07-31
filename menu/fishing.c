#include <c64/vic.h>
#include <c64/charwin.h>
#include <string.h>
#include <c64/keyboard.h>

#include <math.h>

#include <menu/menuSystem.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/uiHandler.h>
#include <engine/gameSettings.h>

// Sections and regions
#pragma section(fishingMenuLoaderData, 0)
#pragma section(fishingMenuCode, 0)
#pragma section(fishingMenuGfxDay, 0)
#pragma section(fishingMenuSprites, 0)
#pragma region(bankFishingMenuC, 0x8000, 0x9000, , MENU_BANK_FISHING_MENU, { fishingMenuLoaderData, fishingMenuCode })
#pragma region(bankFishingMenuG1, DAY_GFX_BMP, DAY_GFX_BMP + 0x2711, , MENU_BANK_FISHING_MENU, { fishingMenuGfxDay })
#pragma region(bankFishingMenuG2, MENU_SPRITE_SRC, MENU_SPRITE_SRC + 0x0a80, , MENU_BANK_FISHING_MENU, { fishingMenuSprites })

#pragma data(fishingMenuGfxDay)
__export const char fishingMenuGfx1[] = {
    #embed 0xffff 2 "assets/multicolorGfx/fishing.kla"
};
#pragma data(fishingMenuSprites)
__export const char fishingMenuSprites[] = {
    #embed 0xffff 20 "assets/sprites/rybki.spd"
};

#pragma code(fishingMenuCode)
#pragma data(data)

void fishingMenuSpriteLoader(){
    memcpy((char *)GFX_1_SPR_DST_ADR, fishingMenuSprites, 0x0a80);
}

const struct MenuOption FISHING_MENU[] = {
    // Add the "Exit to Map" option as shown in the example
    { TXT_IDX_EXIT_TO_MAP, KEY_ARROW_LEFT, SCREEN_TRANSITION, UI_LF + UI_HIDE, &showMenu, MENU_BANK_MAP_VILLIAGE_1, 2, 5 },
    END_MENU_CHOICES
};

// sinus table for circular movement
int sintab[128];

static void _menuHandler() {
    loadMenuGfx();
    loadMenuSprites();

	// calculate sine table
	for(int i=0; i<128; i++)
		sintab[i] = (int)(70 * sin(i * PI / 64));

	// initialize sprites
	vspr_init(GFX_2_SCR);

	// initialize sprites
	for(int i=0; i<16; i++){
		vspr_set(i, 30 + 16 * i, 220 - 8 * i, GFX_1_SPR_DST_ADR / 64 + i, (i & 7) + 8);
	}

    switchScreenTo(SCREEN_MC_GFX);
    displayMenu(FISHING_MENU);


	// initial sort and update
	vspr_sort();
	vspr_update();
	rirq_sort();

	// start raster IRQ processing
	rirq_start();

	// animation loop
	unsigned j = 0;
	for(;;)
	{
		// move sprites around
		char	k = j >> 4;
		for(char i=0; i<16; i++)
		{
			vspr_move(i, 200 + sintab[(j + 8 * i) & 127] + sintab[k & 127], 150 + sintab[(j + 8 * i + 32) & 127] + sintab[(k + 32) & 127]);
		}
		j++;

		// sort virtual sprites by y position
		vspr_sort();

		// wait for raster IRQ to reach and of frame
		rirq_wait();

		// update sprites back to normal and set up raster IRQ for second set
		vspr_update();

		// sort raster IRQs
		rirq_sort();
	}

}

#pragma data(fishingMenuLoaderData)

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &menuNoop,
    .loadMenuGfx     = &loadFullKoalaToBMP2,
    .loadMenuSprites = &fishingMenuSpriteLoader,
    .showMenu        = &_menuHandler,
    .showSprites     = &spriteNoop,
    .updateMenu      = &menuNoop,
    .runMenuLoop     = &menuNoop,
};

// Switching code generation back to shared section
#pragma code(code)
#pragma data(data)
