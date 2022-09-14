#include <c64/vic.h>
#include <c64/easyflash.h>
#include <c64/sprites.h>
#include <string.h>
#include <c64/memmap.h>

#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <miniGame/pigsleCmdMain.h>
#include <miniGame/pigsleCmdIrq.h>
#include <miniGame/pigsleCmdAnims.h>

// ---------------------------------------------------------------------------------------------
// Main screen and sprite bank + loaders code
// ---------------------------------------------------------------------------------------------

#pragma data ( pigsleCommandGfx1 )
__export const char pigsleCommandGfxBg[] = {
    #embed 0x2713 0x0002 "assets/multicolorGfx/dzialoKoval.kla"
    // #embed 0x2713 0x0002 "assets/multicolorGfx/flak_88_dark.kla"
};

const char PIGSLE_CMD_SPR_AIM[] = {
    #embed 0x0080 20 "assets/sprites/crosshair.spd"
};
const char PIGSLE_CMD_SPR_BOOM_1[] = {
    #embed 0xffff 20 "assets/sprites/wybuch.spd"
};
const char PIGSLE_CMD_SPR_BOOM_2[] = {
    #embed 0xffff 20 "assets/sprites/wybuch2.spd"
};
const char PIGSLE_CMD_SPR_B29[] = {
    #embed 0xffff 20 "assets/sprites/b29.spd"
};

#pragma code ( pigsleCommandGfx1Loaders )
#pragma data ( pigsleCommandRAMData )

static void _screenInit(){
    // load colors
    #pragma unroll(page)
    for(int i=0; i<1000; i++){
        GFX_1_SCR[i] = pigsleCommandGfxBg[0x1f40 + i];
        COLOR_RAM[i] = pigsleCommandGfxBg[0x2328 + i];
    }

    // load bitmap
    #pragma unroll(page)
    for(int i=0; i<8000; i++){
        GFX_1_BMP[i] = pigsleCommandGfxBg[i];
    }
}

static void _spriteLoader(){
    memcpy((char *)GFX_1_SPR_DST_ADR, PIGSLE_CMD_SPR_AIM, 0x80);
    // memcpy((char *)GFX_1_SPR_DST_ADR+0x40, PIGSLE_CMD_SPR_FILE, 0x400);
    #pragma unroll(page)
    for(int i=0; i<64*ANIM_FRAMES; i++){
       ((volatile char*) GFX_1_SPR_DST_ADR)[0x80 + i] = ((char*)PIGSLE_CMD_SPR_BOOM_1)[i];
       ((volatile char*) GFX_1_SPR_DST_ADR)[0x80+64*ANIM_FRAMES + i] = ((char*)PIGSLE_CMD_SPR_BOOM_2)[i];
    }
    #pragma unroll(page)
    for(int i=0; i<64*8; i++){
       ((volatile char*) GFX_1_SPR_DST_ADR)[0x80+64*ANIM_FRAMES*2 + i] = ((char*)PIGSLE_CMD_SPR_B29)[i];
    }
}

// this code needs to be in main block, as it switches banks
#pragma code ( pigsleCommandRAMCode )
#pragma data ( pigsleCommandRAMData )

void pigsleScreenInit(void){
    // vic.color_border = VCOL_RED;
    // vic.color_back  = VCOL_RED;
    // mmap_set(MMAP_ROM);
    changeBank(MENU_BANK_PIGSLE_COMMAND_GFX_1);
    _screenInit();
    restoreBank();
}

void pigsleSpriteLoader(){
    changeBank(MENU_BANK_PIGSLE_COMMAND_GFX_1);
    _spriteLoader();
    restoreBank();
}
// ---------------------------------------------------------------------------------------------
// Variables for main Pigsle code
// ---------------------------------------------------------------------------------------------

// menu code is in ROM - data in RAM
#pragma code ( pigsleCommandCode )
#pragma data ( pigsleCommandRAMData )
#define INITIAL_X 160+24-11
#define INITIAL_Y 100+50

// Joystick and crosshair control
volatile int  CrossX = INITIAL_X;
volatile char CrossY = INITIAL_Y;
volatile bool CrossP = false;
volatile char CrossDelay = 0;

// First free and first used explosion
Explosion * efree;
Explosion * eused;
Explosion explosions[EXPLOSION_COUNT];

// ---------------------------------------------------------------------------------------------
// Screen writing code
// ---------------------------------------------------------------------------------------------
#pragma data ( pigsleCommandConsts )

// Charset assets
const char Chars[] = {
    #embed 768 2 "assets/game_font2.64c"
};

static char charX2[16] = {
    0b00000000,
    0b00000011,
    0b00001100,
    0b00001111,
    0b00110000,
    0b00110011,
    0b00111100,
    0b00111111,
    0b11000000,
    0b11000011,
    0b11001100,
    0b11001111,
    0b11110000,
    0b11110011,
    0b11111100,
    0b11111111
};

#pragma code ( pigsleCommandRAMCode )
#pragma data ( pigsleCommandRAMData )
#include <gfx/mcbitmap.h>
#include <c64/rasterirq.h>
RIRQCode topPlane, topPests, middlePests, cannonAnims, open;

// Display bitmap
Bitmap sbm;

static void _drawByte(const Bitmap * bm, int x, int y, char b){
    char * dp = bm->data + bm->cwidth * (y & ~7) + ((x & ~7) | (y & 7));
    *dp = b;
}

// Expand an 8x8 charactor to 16x16 on screen
static void _charPut(int cx, char cy, char c){
    // Get pointer to glyph data
    const char * sp = Chars + 8 * c;
    // expand X to be in pixels
    cx = cx * 8;
    // Loop over all bytes
    for(char y=0; y<8; y++){
        char cl = sp[y];
        char hb = charX2[cl >> 4];
        char lb = charX2[cl & 15];
        _drawByte(&sbm, cx+0, cy + 2 * y + 0, hb);
        _drawByte(&sbm, cx+8, cy + 2 * y + 0, lb);
        _drawByte(&sbm, cx+0, cy + 2 * y + 1, hb);
        _drawByte(&sbm, cx+8, cy + 2 * y + 1, lb);
    }
}

// Write a zero terminated string on screen. X is auto calculated (string should be 20 chars or less), y 0-199
static void charWrite(char cy, const char * s){
    changeBank(MENU_BANK_PIGSLE_COMMAND_1);
    
    // center the text
    char cx = 20 - strlen(s);

    // Loop over all characters
    while (*s){
        _charPut(cx, cy, *s);
        s++;
        cx += 2;
    }
    restoreBank();
}

// ---------------------------------------------------------------------------------------------
// Game code
// ---------------------------------------------------------------------------------------------
#pragma code ( pigsleCommandCode )
#pragma data ( pigsleCommandRAMData )

static void _randomizeExplosion(Explosion * e){
    char rnd = rand();
    char sprIdxOffset = 0;
    if(rnd > 0x80){
        sprIdxOffset = ANIM_FRAMES;
    }
    e->sprIdxOffset = sprIdxOffset; // sprite 0 is aim, so +1 here
}

// Initialize explosion list
static void _explosionInit(void){
    // No explosion active
    eused = nullptr;

    // First free explosion element
    efree = explosions;
    // Build list
    for(char i=0; i<EXPLOSION_COUNT; i++){
        _randomizeExplosion((Explosion *)explosions[i]);
        explosions[i].sprIdx = i + 1; // sprite 0 is aim, so +1 here
        explosions[i].frame = 0;
        explosions[i].delay = ANIM_DELAY;
        explosions[i].next = explosions + i + 1;
    }
    // Terminate last element
    explosions[EXPLOSION_COUNT-1].next = nullptr;
}

static void _aimInit(){
    // init crosshair pos
    CrossX = INITIAL_X;
    CrossY = INITIAL_Y;
    CrossP = false;
    CrossDelay = 0;
}

// Start a new explosion
static void _explosionStart(int x, int y){
    // Free slot in list of explosions?
    if (efree) {
        // Move entry from free to used list
        Explosion * e = efree;
        efree = e->next;
        e->next = eused;
        eused = e;

        // Initialize position and size
        e->x = x + EXPLOSION_X_OFFSET;
        e->y = y + EXPLOSION_Y_OFFSET;
        // x -1 to offset initial 'dot', later anim frames will not use it
        spr_set(e->sprIdx, true, e->x - 1, e->y, e->sprIdxOffset + e->frame + PIGSLE_CMD_ANIM_EXPLOSION_BANK, VCOL_YELLOW, true, false, false);
    }
}

// Animate all explosions
static void _explosionAnimate(void){
    // Loop over active explosions with "e", use "ep" to point
    // to previous explosion, so we can remove the current explosion
    // from the list
    Explosion * e = eused, * ep = nullptr;
    while (e){
        // Remember next entry in list
        Explosion * en = e->next;

        e->delay--;
        if(!e->delay){
            e->delay = ANIM_DELAY;
            // Increment phase
            e->frame++;
        }
        // first ANIM_EXPLOSION_DELAY frames are delay
        if (e->frame > ANIM_EXPLOSION_DELAY) {
            spr_set(e->sprIdx, true, e->x, e->y, e->sprIdxOffset + e->frame + PIGSLE_CMD_ANIM_EXPLOSION_BANK - ANIM_EXPLOSION_DELAY, VCOL_YELLOW, true, false, false);
        }
        // End of explosion live
        if (e->frame == 9 + ANIM_EXPLOSION_DELAY) {
            // Remove explosion from used list
            if (ep)
                ep->next = e->next;
            else
                eused = e->next;

            // disable sprite, reset anim
            spr_set(e->sprIdx, false, e->x, e->y, PIGSLE_CMD_ANIM_EXPLOSION_BANK, VCOL_YELLOW, true, false, false);
            e->frame = 0;
            e->delay = ANIM_DELAY;
            _randomizeExplosion(e);
            
            // Prepend it to free list
            e->next = efree;
            efree = e;
        }
        else
            ep = e;

        // set sprite

        // Next explosion in list
        e = en;
    }
}
// State of the game
volatile struct DropRun {
    bool inProgress;
    int x;

} TheDropDown;

static void _dropRunInit(){
    TheDropDown.x = 320+72;
    TheDropDown.inProgress = true;
}

// State of the game
struct Game {
    GameState state;
    byte      score;
    int       count;

} TheGame;    // Only one game, so global variable

void gameState(GameState state){

    TheGame.state = state;

    switch(state) {
    case GS_READY:
        // Start of new game
        // score_reset();
        pigsleScreenInit();
        bm_init(&sbm, GFX_1_BMP, 40, 25);

        memset(GFX_1_SCR+7*40, 0x67, 3*40);
        memset(COLOR_RAM+7*40, 2, 3*40);
        memset(GFX_1_BMP+7*40*8, 0, 3*40*8);
        // TODO: add txt to translations
        charWrite(60, s"Ratuj kartofle!");
        TheGame.count = 150;
        break;

    case GS_PLAYING:
        // Avoid old fire request
        CrossP = false;

        // Setup display
        pigsleScreenInit();
        _dropRunInit();
        _explosionInit();
        // icbm_init();
        _spriteInit();
        _aimInit();
        TheGame.count = 500;
        break;

    case GS_END:
        // char_write(104, 60, s"THE END", 0);
        TheGame.count = 120;
        break;
    }
}

// Main game play code
static void _gamePlay(void){
    // vic.color_border++;
    // Check if fire request
    if (CrossP) {
        // change crosshair
        GFX_1_SCR[OFFSET_SPRITE_PTRS+0] = PIGSLE_CMD_ANIM_CROSSHAIR_EMPTY_BANK;
        // boom!
        _explosionStart(CrossX, CrossY);
        // Reset request
        CrossP = false;
    }

    // Wait for next ICMB to enter the game
    if (!--TheGame.count){

        // Next lauch time
        TheGame.count = 500 + (rand() & 63);
        _dropRunInit();
    }

    // Advance defending missiles by four pixels
    // for(char i=0; i<4; i++)
    //     missile_animate();

    // Advance ICBMs
    // icbm_animate();

    // Show explosions
    _explosionAnimate();
}

// Main game loop, entered every VSYNC
void gameLoop(){

    switch(TheGame.state){
    case GS_READY:
        if (!--TheGame.count)
            gameState(GS_PLAYING);
        break;

    case GS_PLAYING:
        _gamePlay();

        // Check for level and game end coditions
        break;

    case GS_END:
        if (!--TheGame.count)
            gameState(GS_READY);
        break;
    }

}

void pigsleCmdInit(){
    splashScreen(false, 1);
    // stop IRQs and change to ours
    __asm {
        sei
    }
    // msx off
    ((byte *)0xd418)[0] &= ~0xf;
    // screen off, sprites off
    // vic.ctrl1 = VIC_CTRL1_BMM | VIC_CTRL1_RSEL | 3;
    vic.spr_enable   = 0b00000000;

    __asm {
        // init music
        lda #MSX_ROM
        sta $01
        lda #$02
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
    rirq_init(true);
    // topPlane, topPests, middlePests, cannonAnims, open
    // Top - Plane
    rirq_build(&topPlane, 2);
    rirq_write(&topPlane, 0, &vic.ctrl1, VIC_CTRL1_BMM | VIC_CTRL1_DEN | VIC_CTRL1_RSEL | 3 );
    rirq_call(&topPlane, 1, pigsleCmdIrq_topPlane);
    rirq_set(0, IRQ_TOP_PLANE, &topPlane);

    // Top - Pests
    rirq_build(&topPests, 1);
    rirq_call(&topPests, 0, pigsleCmdIrq_topPests);
    rirq_set(1, IRQ_TOP_PESTS, &topPests);

    // Middle - Pests
    rirq_build(&middlePests, 1);
    rirq_call(&middlePests, 0, pigsleCmdIrq_middlePests);
    rirq_set(2, IRQ_MIDDLE_PESTS, &middlePests);

    // Bottom - Cannon anims
    rirq_build(&cannonAnims, 1);
    rirq_call(&cannonAnims, 0, pigsleCmdIrq_cannonAnims);
    rirq_set(3, IRQ_CANNON, &cannonAnims);

    // Open border raster IRQ
    rirq_build(&open, 2);
    // Reduce vertical screen size to fool VIC counter
    rirq_write(&open, 0, &vic.ctrl1, VIC_CTRL1_BMM |VIC_CTRL1_DEN | 3);
    rirq_call(&open, 1, pigsleCmdIrq_openBorder);
    // Place it into the last line of the screen
    rirq_set(4, IRQ_FRAME_OPEN, &open);

    // sort the raster IRQs
    rirq_sort();

    // Load GFX
    pigsleScreenInit();
    pigsleSpriteLoader();
    vic.spr_expand_x = 0b00000000;
    vic.spr_expand_y = 0b00000000;
    vic.spr_priority = 0b00000000;
    
    // Init sprite system
    spr_init(GFX_1_SCR);
    
    splashScreen(false, 2);
    // Init bitmap
    vic_setmode(VICM_HIRES_MC, GFX_1_SCR, GFX_1_BMP);
    // start raster IRQ processing
    rirq_start();

    // start game state machine
    gameState(GS_READY);

    // main loop
    for(;;){
        // vic.color_border++;

        gameLoop();
        vic_waitFrame();
        rirq_wait();
        // vic.color_border--;

    }
}
