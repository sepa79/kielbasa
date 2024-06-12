#include <c64/keyboard.h>
#include <c64/memmap.h>
#include <c64/cia.h>
#include <c64/vic.h>
#include <c64/charwin.h>


// Shared code/data region, copied from easyflash bank 0 to ram during startup
// Code in main region is compressed, so can go over 0x4000 here
#pragma stacksize( 1024 )
#pragma heapsize( 0 )
#pragma section( startup, 0 )
#pragma region( startup, 0x0100, 0x0200, , , { startup } )
#pragma region( main, 0x0400, 0x5c00, , , { code, data, bss, heap, stack } )

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )

int main(void){
    // Enable ROM
    setPort(MMAP_ROM);
    // screen off
    vic.ctrl1 = VIC_CTRL1_BMM | VIC_CTRL1_RSEL | 3;
    vic.spr_enable   = 0b00000000;

    // Init CIAs (no kernal rom was executed so far)
    cia_init();

    __asm {
        // same init stuff the kernel calls after reset
        // ldx #0
        // stx $d016
        jsr $ff84   // Initialise I/O

        // These may not be needed - depending on what you''ll do
        // jsr $ff87   // Initialise System Constants
        // jsr $ff8a   // Restore Kernal Vectors
        // jsr $ff81   // Initialize screen editor
    }
    

}
