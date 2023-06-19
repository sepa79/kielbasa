#include <c64/easyflash.h>
#include <c64/vic.h>

#include <menu/menuSystem.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/gameSettings.h>

#pragma code ( code )
#pragma data ( data )

// Global var used with menuGfxLoaderSingleBitmap(isDay)
bool mnu_isGfxLoaded = false;
volatile char mnu_menuBank = 0;

// Switch bank, load any code into RAM if needed.
// Default noop loader is _menuNoop().
void loadMenu(char bank){
    mnu_menuBank = bank;
    setBank(bank);
    ((Loaders *)0x8000)->loadMenuCode();
}

// Call currently mounted CRT bank's loadMenuGfx(isDay) function.
// Default loader is _menuGfxLoader(bool isDay).
// Default noop loader is _menuNoop().
void loadMenuGfx(bool isDay){
    char pbank = setBank(mnu_menuBank);
    ((Loaders *)0x8000)->loadMenuGfx(isDay);
    setBank(pbank);
}

// Call currently mounted CRT bank's loadMenuSprites() function.
// Default loader is _menuSpriteLoader().
// Default noop loader is _menuNoop().
void loadMenuSprites(){
    gms_enableMenuSprites = false;
    ((Loaders *)0x8000)->loadMenuSprites();
    gms_enableMenuSprites = true;
}

// Re-mounts last loaded CRT bank and starts its showMenu() function.
// No defaults here.
void showMenu(){
    setBank(mnu_menuBank);
    ((Loaders *)0x8000)->showMenu();
}

// Call currently mounted CRT bank's showSprites() function - has to be interrupt safe, called every frame on top Split IRQ.
// Default noop function is _menuNoop().
__interrupt void showSprites(){
    char pbank = setBank(mnu_menuBank);
    ((Loaders *)0x8000)->showSprites();
    setBank(pbank);
}

// Call currently mounted CRT bank's updateMenu() function, every midnight.
// Default noop function is _menuNoop().
void updateMenu(){
    // don't update when in any of the option menus - time is stopped, they handle their own screens
    if(!gms_disableTimeControls){
        char pbank = setBank(mnu_menuBank);
        ((Loaders *)0x8000)->updateMenu();
        setBank(pbank);
    }
}

// check what menu is actually open and update it if it matches chosen bank
void updateMenuIfIn(char bank){
    if(bank == mnu_menuBank)
        updateMenu();
}

// map and other game menus have their own loops, this allows to call them from the main loop
void runMenuLoop(){
    char pbank = setBank(mnu_menuBank);
    ((Loaders *)0x8000)->runMenuLoop();
    setBank(pbank);
}

#pragma section( sharedMenuCode, 0 )
#pragma region( sharedMenuCodeBank, 0xbc00, 0xc000, , {
    MENU_BANK_MAIN_MENU,
    MENU_BANK_FARMLAND,
    MENU_BANK_PIG_PEN,
    MENU_BANK_KITCHEN,
    MENU_BANK_SHOP,
    MENU_BANK_SHOP_IN,
    MENU_BANK_CREW
    }, { sharedMenuCode } )

// Code shared by all menu banks
#pragma code( sharedMenuCode )

// *****************************************************
// loader routines
// *****************************************************

// Noop - just return.
void menuNoop(){
    return;
}
// Noop - just return.
__interrupt void spriteNoop(){
    return;
}

// wait until raster hits middle of the screen
static void _waitForScreenMiddle(){
    // unknown place? skip the checks
    if(gms_framePos != FRAME_UNKNOWN){
        if(gms_framePos == FRAME_MIDDLE){
            while(gms_framePos == FRAME_MIDDLE){};
        }
        // now wait until we move to middle of the screen
        while(gms_framePos != FRAME_MIDDLE){};
    }
}

// fill colors black
static void _fillHalfGfxBlack(){
    char i = 0;
    do {
#assign y 0
#repeat
        GFX_1_SCR[y + i] = 0;
        COLOR_RAM[y + i] = 0;
#assign y y + 0xf0
#until y == 0x1e0
        i++;
    } while (i != 0xf0);
}

static void _loadDayGfx(){
    _waitForScreenMiddle();
    _fillHalfGfxBlack();

    // load bitmap
    char i = 0;
    do {
#assign y 0
#repeat
        GFX_1_BMP[y + i] = ((char*)DAY_GFX_BMP)[y + i];
#assign y y + 0x100
#until y == 0x0f00
        i++;
    } while (i != 0);

    _waitForScreenMiddle();

    // load colors
    i = 0;
    do {
#assign y 0
#repeat
        GFX_1_SCR[y + i] = ((char*)DAY_GFX_SCR)[y + i];
        COLOR_RAM[y + i] = ((char*)DAY_GFX_COL)[y + i];
#assign y y + 0xf0
#until y == 0x1e0
        i++;
    } while (i != 0xf0);

}

static void _loadNightGfx(){
    _waitForScreenMiddle();
    _fillHalfGfxBlack();

    // load bitmap
    char i = 0;
    do {
#assign y 0
#repeat
        GFX_1_BMP[y + i] = ((char*)NIGHT_GFX_BMP)[y + i];
#assign y y + 0x100
#until y == 0x0f00
        i++;
    } while (i != 0);

    _waitForScreenMiddle();

    // load colors
    i = 0;
    do {
#assign y 0
#repeat
        GFX_1_SCR[y + i] = ((char*)NIGHT_GFX_SCR)[y + i];
        COLOR_RAM[y + i] = ((char*)NIGHT_GFX_COL)[y + i];
#assign y y + 0xf0
#until y == 0x1e0
        i++;
    } while (i != 0xf0);
}

// Switches graphic according to day/night cycle
void menuGfxLoader(bool isDay){
    if(isDay){
        _loadDayGfx();
    } else {
        _loadNightGfx();
    }
}

// Loads single file and never changes it.
// Parameter is ignored, kept for compatibility.
// Menu should set the variable 'isGfxLoaded = false' in the menuHandler code, before calling 'loadMenuGfx(cal_isDay)'
void menuGfxLoaderSingleBitmap(bool isDay){
    if(!mnu_isGfxLoaded){
        _loadDayGfx();
        mnu_isGfxLoaded = true;
    }
}

void menuSpriteLoader(){
    char i = 0;
    do {
#assign y 0
#repeat
       ((volatile char*) MENU_SPRITE_DST)[y + i] = ((char*)MENU_SPRITE_SRC)[y + i];
#assign y y + 0x100
#until y == 0x0400
        i++;
    } while (i != 0);
}

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )