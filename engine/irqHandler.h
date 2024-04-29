#ifndef IRQ_HANDLER_H
#define IRQ_HANDLER_H

// screen will be split at these lines
#define IRQ_RASTER_TOP_MC_SCREEN 0x2e
#define IRQ_RASTER_MIDDLE_TXT_SCREEN 0x91
// special irqs to control sprites
#define IRQ_RASTER_BOTTOM_UI 0xf9
#define IRQ_RASTER_TOP_UI_SPRITES 0x01

// scroll text pointer
extern const char * SB_TEXT;

// used by villiage map, for double buffering and loading fonts seamlesly
extern volatile bool map_2ndScreen;
extern char * mapScreen;

enum SCREEN_MODE {
// Restore the screen using 'previousScreenMode', dummy entry for the sake of Menu struct readability
    SCREEN_RESTORE,

// Partial IRQ init - split screen mode, with regular UI on top & bottom, MSX, MC on top half with sprites, text on bottom half with sprites.
    SCREEN_SPLIT_MC_TXT,

// Partial IRQ init - full TXT mode, with regular UI on top & bottom, MSX, normal txt screen sprite handlers used.
    SCREEN_FULL_TXT,

// Partial IRQ init - full MC TXT mode, with regular UI on top & bottom, MSX and sprite handlers.
    SCREEN_FULL_MC_TXT,

// Partial IRQ init - full MC TXT mode, with regular UI on top & bottom, MSX, no sprite handlers.
    SCREEN_MC_TXT_BOOMBOX,

// Partial IRQ init - full Hires (normal) TXT mode, with regular UI on top & map UI at the bottom, MSX, no sprite handlers. Used for Map.
    SCREEN_HIRES_TXT,

// Partial IRQ init - full MC GFX mode, with regular UI on top & bottom, MSX, normal txt screen sprite handlers used.
    SCREEN_MC_GFX,
    
// Changing screen modes - keep screen off
    SCREEN_TRANSITION,
    SCREEN_PIGSLE_COMMAND,
    SCREEN_FISHING,
};

void playMsx();
void joyUpdate();
void switchScreenTo(byte screenMode);

// Main IRQ init - must be called first, sets up all the bits.
void initRasterIRQ();

#pragma compile("irqHandler.c")

#endif