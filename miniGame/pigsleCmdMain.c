#include <c64/vic.h>
#include <c64/easyflash.h>
#include <c64/sprites.h>
#include <string.h>

#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <miniGame/pigsleCmdMain.h>

// ---------------------------------------------------------------------------------------------
// Main screen and sprite bank + loaders code
// ---------------------------------------------------------------------------------------------

#pragma data ( pigsleCommandGfx1 )
__export const char pigsleCommandGfxBg[] = {
    #embed 0x2713 0x0002 "assets/multicolorGfx/dzialo110922.kla"
    // #embed 0x2713 0x0002 "assets/multicolorGfx/flak_88_dark.kla"
};

const char PIGSLE_CMD_SPR_FILE_AIM[] = {
    #embed 0x0080 20 "assets/sprites/crosshair.spd"
};
const char PIGSLE_CMD_SPR_FILE_1[] = {
    #embed 0xffff 20 "assets/sprites/wybuch.spd"
};
const char PIGSLE_CMD_SPR_FILE_2[] = {
    #embed 0xffff 20 "assets/sprites/wybuch2.spd"
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
    memcpy((char *)GFX_1_SPR_DST_ADR, PIGSLE_CMD_SPR_FILE_AIM, 0x80);
    // memcpy((char *)GFX_1_SPR_DST_ADR+0x40, PIGSLE_CMD_SPR_FILE, 0x400);
    #pragma unroll(page)
    for(int i=0; i<64*ANIM_FRAMES; i++){
       ((volatile char*) GFX_1_SPR_DST_ADR)[0x80 + i] = ((char*)PIGSLE_CMD_SPR_FILE_1)[i];
       ((volatile char*) GFX_1_SPR_DST_ADR)[0x80+64*ANIM_FRAMES + i] = ((char*)PIGSLE_CMD_SPR_FILE_2)[i];
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

// Joystick and crosshair control
volatile int  CrossX = 159;
volatile char CrossY = 100;
volatile bool CrossP = false;
volatile char CrossDelay = 0;

// First free and first used explosion
Explosion * efree;
Explosion * eused;
Explosion explosions[EXPLOSION_COUNT];

// ---------------------------------------------------------------------------------------------
// DrMortalWombat's great screen writing code
// ---------------------------------------------------------------------------------------------
#pragma data ( pigsleCommandConsts )

// Charset assets
const char MissileChars[] = {
    #embed "assets/missilechars.64c"
};

#pragma code ( pigsleCommandRAMCode )
#pragma data ( pigsleCommandRAMData )
#include <gfx/mcbitmap.h>

// Display bitmap
Bitmap sbm;

// Expand an 8x8 charactor to 16x16 on screen
void char_put(char cx, char cy, char c, char color){
    // Get pointer to glyph data
    const char * sp = MissileChars + 8 * c;

    // Loop over all pixel
    for(char y=0; y<8; y++)
    {
        char cl = sp[y];
        for(char x=0; x<8; x++)
        {
            // Draw two pixel if bit is set
            if (cl & 128)
            {
                bmmc_put(&sbm, cx + 2 * x, cy + 2 * y + 0, color);
                bmmc_put(&sbm, cx + 2 * x, cy + 2 * y + 1, color);
            }

            // Next bit
            cl <<= 1;
        }
    }
}

// Write a zero terminated string on screen
void char_write(char cx, char cy, const char * s, char color)
{
    changeBank(MENU_BANK_PIGSLE_COMMAND_1);
    // Loop over all characters
    while (*s)
    {
        char_put(cx, cy, *s, color);
        s++;
        cx += 16;
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
static void explosion_init(void){
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

// Start a new explosion
static void explosion_start(int x, int y){
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
static void explosion_animate(void){
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
struct Game {
    GameState state;
    byte      score, count;

} TheGame;    // Only one game, so global variable

void game_state(GameState state){

    TheGame.state = state;

    switch(state) {
    case GS_READY:
        // Start of new game
        // score_reset();
        pigsleScreenInit();
        // bmmcu_rect_fill(&sbm, 0, 56, 320, 24, 2);
        memset(GFX_1_SCR+7*40, 1, 3*40);
        memset(COLOR_RAM+7*40, 1, 3*40);
        memset(GFX_1_BMP+7*40*8, 1, 3*40*8);
        char_write(31, 60, s"Ratuj kartofle!", 3);
        TheGame.count = 255;
        break;

    case GS_PLAYING:
        // Avoid old fire request
        CrossP = false;

        // Setup display
        pigsleScreenInit();
        // missile_init();
        explosion_init();
        // icbm_init();

        TheGame.count = 15;
        break;

    case GS_END:
        // char_write(104, 60, s"THE END", 0);
        TheGame.count = 120;
        break;
    }
}

// Main game play code
static void game_play(void)
{
    // vic.color_border++;
    // Check if fire request
    if (CrossP) {
        // change crosshair
        GFX_1_SCR[OFFSET_SPRITE_PTRS+0] = PIGSLE_CMD_ANIM_CROSSHAIR_EMPTY_BANK;
        // boom!
        explosion_start(CrossX, CrossY);
        // Reset request
        CrossP = false;
    }

    // Wait for next ICMB to enter the game
    if (!--TheGame.count){

        // Next lauch time
        TheGame.count = 8 + (rand() & 63);
    }

    // Advance defending missiles by four pixels
    // for(char i=0; i<4; i++)
    //     missile_animate();

    // Advance ICBMs
    // icbm_animate();

    // Show explosions
    explosion_animate();
}

// Main game loop, entered every VSYNC
static void game_loop(){

    switch(TheGame.state){
    case GS_READY:
        if (!--TheGame.count)
            game_state(GS_PLAYING);
        break;

    case GS_PLAYING:
        game_play();

        // Check for level and game end coditions
        break;

    case GS_END:
        if (!--TheGame.count)
            game_state(GS_READY);
        break;
    }

}
