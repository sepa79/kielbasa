#include <c64/vic.h>
#include <c64/easyflash.h>
#include <c64/sprites.h>
#include <string.h>
#include <c64/memmap.h>
#include <c64/keyboard.h>

#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <miniGame/villiageMapMain.h>

// ---------------------------------------------------------------------------------------------
// Main screen and sprite bank + loaders code
// ---------------------------------------------------------------------------------------------

#pragma data ( villiageMapGfx1 )

static const char ro_attribs[] = {
    #embed "assets/charGfx/VilliageMapHiresMain - CharAttribs_L1.bin"	
};

static const char ro_chars[] = {
    #embed "assets/charGfx/VilliageMapHiresMain - Chars.bin"	
};

static const char ro_map[] = {
    #embed "assets/charGfx/VilliageMapHiresMain - (8bpc, 32x24) Map.bin"	
};

static const char ro_tiles[] = {
    #embed "assets/charGfx/VilliageMapHiresMain - Tiles.bin"	
};

// static const char ro_tileAttribs[] = {
//     #embed "assets/charGfx/kielbasaMapS2 - TileAttribs_L1.bin"	
// };

#pragma code ( villiageMapGfx1Loaders )
#pragma data ( villiageMapRAMData )

static void _screenInit(){
    // ROM on, I/O off - as we will copy to RAM under I/O ports
    mmap_set(0b00110011);
    memcpy(GFX_1_FNT, ro_chars, sizeof(ro_chars));
    // turn ROMS and I/O back on, so that we don't get a problem when bank tries to be switched but I/O is not visible
    mmap_set(MMAP_ROM);
    vic.color_back = VCOL_BROWN;
    vic.color_border = VCOL_BLACK;
    vic.color_back1 = VCOL_DARK_GREY;
    vic.color_back2 = VCOL_LT_GREY;
}

// this code needs to be in main block, as it switches banks
#pragma code ( villiageMapRAMCode )
#pragma data ( villiageMapRAMData )

static void _spriteLoader(){
}

void villiageMapScreenInit(void){
    // vic.color_border = VCOL_RED;
    // vic.color_back  = VCOL_RED;
    // mmap_set(MMAP_ROM);
    char pbank = setBank(MENU_BANK_MAP_VILLIAGE_1);
    _screenInit();
    setBank(pbank);
}

void villiageMapSpriteLoader(){
    char pbank = setBank(MENU_BANK_MAP_VILLIAGE_1);
    _spriteLoader();
    setBank(pbank);
}

#pragma code ( villiageMapRAMCode )
#pragma data ( villiageMapRAMData )
#include <c64/rasterirq.h>
RIRQCode topMap, msxMap, openMap;

// ---------------------------------------------------------------------------------------------
// Game code
// ---------------------------------------------------------------------------------------------
#pragma code ( villiageMapCode )
#pragma data ( villiageMapRAMData )
static char ro_xmap[V_MAP_SIZE_X * V_MAP_SIZE_Y];

// copy base map from ROM, add any specials to it
void tiles_remap(void)
{
    for(char y=0; y<V_MAP_SIZE_Y; y++)
    {
        for(char x=0; x<V_MAP_SIZE_X; x++)
        {
            ro_xmap[V_MAP_SIZE_X * y + x] = ro_map[V_MAP_SIZE_X * y + x];
        }
    }
}

void tiles_put4x4row0(char * dp, char * cp, const char * mp, const char * tp)
{
	for(char tx=0; tx<10; tx++)
	{
		const char  * ti = tp + mp[tx] * 16;

#assign cx 0
#repeat
		{
			char ci = ti[cx];
			dp[cx] = ci;
			cp[cx] = ro_attribs[ci];
		}
#assign cx cx + 1
#until cx == 4

		dp += 4;
		cp += 4;
	}
}

void tiles_put4x4row1(char * dp, char * cp, const char * mp, const char * tp)
{
	const char  * ti = tp + mp[0] * 16 + 1;

#assign cx 0
#repeat
	{
		char ci = ti[cx];
		dp[cx] = ci;
		cp[cx] = ro_attribs[ci];
	}
#assign cx cx + 1
#until cx == 3

	dp += 3;
	cp += 3;

	for(char tx=1; tx<10; tx++)
	{
		ti = tp + mp[tx] * 16;

#assign cx 0
#repeat
		{
			char ci = ti[cx];
			dp[cx] = ci;
			cp[cx] = ro_attribs[ci];
		}
#assign cx cx + 1
#until cx == 4

		dp += 4;
		cp += 4;
	}

	ti = tp + mp[10] * 16;

#assign cx 0
#repeat
	{
		char ci = ti[cx];
		dp[cx] = ci;
		cp[cx] = ro_attribs[ci];
	}
#assign cx cx + 1
#until cx == 1
}

void tiles_put4x4row2(char * dp, char * cp, const char * mp, const char * tp)
{
	const char  * ti = tp + mp[0] * 16 + 2;

#assign cx 0
#repeat
	{
		char ci = ti[cx];
		dp[cx] = ci;
		cp[cx] = ro_attribs[ci];
	}
#assign cx cx + 1
#until cx == 2

	dp += 2;
	cp += 2;

	for(char tx=1; tx<10; tx++)
	{
		ti = tp + mp[tx] * 16;

#assign cx 0
#repeat
		{
			char ci = ti[cx];
			dp[cx] = ci;
			cp[cx] = ro_attribs[ci];
		}
#assign cx cx + 1
#until cx == 4

		dp += 4;
		cp += 4;
	}

	ti = tp + mp[10] * 16;

#assign cx 0
#repeat
	{
		char ci = ti[cx];
		dp[cx] = ci;
		cp[cx] = ro_attribs[ci];
	}
#assign cx cx + 1
#until cx == 2
}

void tiles_put4x4row3(char * dp, char * cp, const char * mp, const char * tp)
{
	const char  * ti = tp + mp[0] * 16 + 3;

#assign cx 0
#repeat
	{
		char ci = ti[cx];
		dp[cx] = ci;
		cp[cx] = ro_attribs[ci];
	}
#assign cx cx + 1
#until cx == 1

	dp += 1;
	cp += 1;

	for(char tx=1; tx<10; tx++)
	{
		ti = tp + mp[tx] * 16;

#assign cx 0
#repeat
		{
			char ci = ti[cx];
			dp[cx] = ci;
			cp[cx] = ro_attribs[ci];
		}
#assign cx cx + 1
#until cx == 4

		dp += 4;
		cp += 4;
	}

	ti = tp + mp[10] * 16;

#assign cx 0
#repeat
	{
		char ci = ti[cx];
		dp[cx] = ci;
		cp[cx] = ro_attribs[ci];
	}
#assign cx cx + 1
#until cx == 3
}

void tiles_put4x4(const char * mp, char ox, char oy)
{
    char * dp = GFX_1_SCR, * cp = COLOR_RAM;

    mp += V_MAP_SIZE_X * (oy >> 2) + (ox >> 2);
    oy &= 3;
    ox &= 3;

    for(char ty=0; ty<24; ty++)
    {
        switch (ox)
        {
            case 0:
                tiles_put4x4row0(dp, cp, mp, ro_tiles + 4 * oy);
                break;
            case 1:
                tiles_put4x4row1(dp, cp, mp, ro_tiles + 4 * oy);
                break;
            case 2:
                tiles_put4x4row2(dp, cp, mp, ro_tiles + 4 * oy);
                break;
            case 3:
                tiles_put4x4row3(dp, cp, mp, ro_tiles + 4 * oy);
                break;
        }
        dp += 40;
        cp += 40;

        oy ++;
        if (oy == 4)
        {
            mp += V_MAP_SIZE_X;
            oy = 0;
        }
    }
}


// Main game loop, entered every VSYNC
void mapGameLoop(){
    tiles_put4x4(ro_xmap, vMapX, vMapY);
}

void villiageMapInit(){
    splashScreen(false, 1);
    // stop IRQs and change to ours
    rirq_stop();

    // msx off
    ((byte *)0xd418)[0] &= ~0xf;
    // screen off, sprites off
    // vic.ctrl1 = VIC_CTRL1_BMM | VIC_CTRL1_RSEL | 3;
    vic.spr_enable = 0b00000000;

    __asm {
        // init music
        lda #MSX_ROM
        sta $01
        lda #$04
        jsr MSX_INIT
    }
    // if you use the mmap_trampoline() you have to call the mmap_set() at least once to init the shadow variable
    mmap_set(MMAP_ROM);
    // Activate trampoline
    mmap_trampoline();
    // Disable CIA interrupts, we do not want interference
    // with our joystick interrupt
    cia_init();
    // clean 0xffff - so we don't have artefacts when we open borders
    ((char *)0xffff)[0] = 0;

    // initialize raster IRQ
    // switchScreenToFullMCTxt();
    switchScreenToFullHiresTxt();

    // Load GFX
    villiageMapScreenInit();
    // villiageMapSpriteLoader();
    tiles_remap();

    splashScreen(false, 2);
    // start raster IRQ processing
    rirq_start();
    mapGameLoop();
}
